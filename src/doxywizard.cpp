/*************************************************************************
 *
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#include "mainwindow.h"

#ifdef WIN32
#include <windows.h>
#endif

void MainWindow::openConfig()
{
   if (querySave()) {
      m_ConfigFile = QFileDialog::getOpenFileName(this, tr("Open Configuration File"), m_struct.pathPrior);

      if (! m_ConfigFile.isEmpty()) {
         loadDoxCfg(m_ConfigFile);
      }
   }
}

void MainWindow::loadDoxCfg(const QString fname)
{
   m_ConfigFile = fname;

   // open the specified configuration file
   //BROOM  m_expert->loadConfig(absFileName);
   m_wizard->refresh();

   updateLaunchButtonState();

   setDoxygenTitle(false);
}

void MainWindow::saveDoxCfg()
{
   if (m_ConfigFile.isEmpty()) {
      return;
   }

   QFile f(m_ConfigFile);

   if (! f.open(QIODevice::WriteOnly)) { QMessageBox::warning(this, tr("Error saving"),
                           tr("Can not save the file ") + m_ConfigFile + f.error());
      return;
   }

   QTextStream t(&f);
   m_expert->writeConfig(t, false);

   updateConfigFileName(m_ConfigFile);
   setDoxygenTitle(false);
}

bool MainWindow::saveDoxCfgAs()
{    
   m_ConfigFile = QFileDialog::getSaveFileName(this, QString(), m_struct.pathPrior);

   if (m_ConfigFile.isEmpty()) {     
      return false;

   } else {
      saveDoxCfg();
   }

   return true;
}

void MainWindow::saveDoxFile()
{
   if (m_ConfigFile.isEmpty()) {
      saveDoxCfgAs();

   } else {
      saveDoxCfg();
   }
}

void MainWindow::updateConfigFileName(const QString &fileName)
{
   if (m_ConfigFile != fileName) {

      m_ConfigFile       = fileName;
      m_struct.pathPrior = QFileInfo(m_ConfigFile).path();

      json_Write(PATH_PRIOR);
      addRecentFile(m_ConfigFile);

      setDoxygenTitle(m_modified);
   }
}


// ** Defaults
void MainWindow::clearRecent()
{
   if (QMessageBox::question(this, tr("Clear the list of recent files?"),
                             tr("Do you want to clear the list of recently "
                                "loaded configuration files?"),
                             QMessageBox::Yes |
                             QMessageBox::Cancel) == QMessageBox::Yes) {
      m_recentMenu->clear();
      m_recentFiles.clear();

      for (int i = 0; i < RECENT_FILES_MAX; i++) {
         m_settings.setValue(QString().sprintf("recent/config%d", i++), QString::fromAscii(""));      
      }

      m_settings.sync();
   }

}

void MainWindow::makeDefaults()
{
   if (QMessageBox::question(this, tr("Use current setting at startup?"),
                             tr("Do you want to save the current settings "
                                "and use them next time Doxywizard starts?"),
                             QMessageBox::Save |
                             QMessageBox::Cancel) == QMessageBox::Save) {
      //printf("MainWindow:makeDefaults()\n");
      m_expert->saveSettings(&m_settings);
      m_settings.setValue(QString::fromAscii("wizard/loadsettings"), true);
      m_settings.sync();
   }
}


void MainWindow::resetToDefaults()
{
   if (QMessageBox::question(this, tr("Reset settings to their default values?"),
                             tr("Do you want to revert all settings back "
                                "to their original values?"),
                             QMessageBox::Reset |
                             QMessageBox::Cancel) == QMessageBox::Reset) {
      //printf("MainWindow:resetToDefaults()\n");
      m_expert->resetToDefaults();
      m_settings.setValue(QString::fromAscii("wizard/loadsettings"), false);
      m_settings.sync();
      m_wizard->refresh();
   }
}

void MainWindow::saveSettings()
{
  json_Write(CLOSE);
}

// **  Tabs
void MainWindow::addRecentFile(const QString &fileName)
{
   int i = m_recentFiles.indexOf(fileName);
   if (i != -1) {
      m_recentFiles.removeAt(i);
   }

   // not found
   if (m_recentFiles.count() < RECENT_FILES_MAX) {
      // append
      m_recentFiles.prepend(fileName);

   } else {
      // add + drop last item
      m_recentFiles.removeLast();
      m_recentFiles.prepend(fileName);
   }

   m_recentMenu->clear();
   i = 0;
   foreach( QString str, m_recentFiles ) {
      m_recentMenu->addAction(str);
      m_settings.setValue(QString().sprintf("recent/config%d", i++), str);
   }

   for (; i < RECENT_FILES_MAX; i++) {
      m_settings.setValue(QString().sprintf("recent/config%d", i++), QString::fromAscii(""));
   }
}

void MainWindow::selectTab(int id)
{
   if (id == 0) {
      m_wizard->refresh();
   } else if (id == 1) {
      m_expert->refresh();
   }
}

void MainWindow::selectRunTab()
{
   m_tabs->setCurrentIndex(2);
}


// ** Recent Files
void MainWindow::openRecent(QAction *action)
{
   if (querySave()) {
// BROOM      loadDoxCfg(action->text());
   }
}

void MainWindow::runDoxygen()
{
   if (!m_running) {
      QString doxygenPath;

#if defined(Q_OS_MACX)
      doxygenPath = qApp->applicationDirPath() + QString::fromAscii("/../Resources/");
      qDebug() << tr("CS Doxygen path: ") << doxygenPath;

      if ( !QFile(doxygenPath + QString::fromAscii("doxygen")).exists() ) {

         // No doygen binary in the resources, if there is a system doxygen binary, use that instead         
         if ( QFile(QString::fromAscii("/usr/local/bin/cs_doxygen")).exists() ) {
            doxygenPath = QString::fromAscii("/usr/local/bin/");

         } else {
            qDebug() << tr("Unable to find the doxygen command, make sure it is in your $$PATH");
            doxygenPath = QString::fromAscii("");
         }
      }
      qDebug() << tr("Getting doxygen from: ") << doxygenPath;
#endif

      QString destDir = m_ui->destDir->text();

      m_runProcess->setReadChannel(QProcess::StandardOutput);
      m_runProcess->setProcessChannelMode(QProcess::MergedChannels);
      m_runProcess->setWorkingDirectory(destDir);

      QStringList env = QProcess::systemEnvironment();

      // set PWD environment variable
      env.replaceInStrings(QRegExp(QString::fromAscii("^PWD=(.*)"), Qt::CaseInsensitive),
                           QString::fromAscii("PWD=") + destDir);

      m_runProcess->setEnvironment(env);

      QStringList args;
      args << QString::fromAscii("-b"); // make stdout unbuffered
      args << QString::fromAscii("-");  // read config from stdin

      m_outputLog->clear();
      m_runProcess->start(doxygenPath + QString::fromAscii("doxygen"), args);

      if (!m_runProcess->waitForStarted()) {
         m_outputLog->append(QString::fromAscii("*** Failed to run doxygen\n"));
         return;
      }
      QTextStream t(m_runProcess);
      m_expert->writeConfig(t, false);
      m_runProcess->closeWriteChannel();

      if (m_runProcess->state() == QProcess::NotRunning) {
         m_outputLog->append(QString::fromAscii("*** Failed to run doxygen\n"));
      } else {
         m_saveLog->setEnabled(false);
         m_running = true;
         m_run->setText(tr("Stop doxygen"));
         m_runStatus->setText(tr("Status: running"));
         m_timer->start(1000);
      }

   } else {
      m_running = false;
      m_run->setText(tr("Run doxygen"));
      m_runStatus->setText(tr("Status: not running"));
      m_runProcess->kill();
      m_timer->stop();      
   }
}

void MainWindow::readStdout()
{
   if (m_running) {
      QByteArray data = m_runProcess->readAllStandardOutput();
      QString text = QString::fromUtf8(data);
      if (!text.isEmpty()) {
         m_outputLog->append(text.trimmed());
      }
   }
}

void MainWindow::runComplete()
{
   if (m_running) {
      m_outputLog->append(tr("*** CS Doxygen has finished\n"));
   } else {
      m_outputLog->append(tr("*** Cancelled by user\n"));
   }

   m_outputLog->ensureCursorVisible();
   m_run->setText(tr("Run doxygen"));
   m_runStatus->setText(tr("Status: not running"));
   m_running = false;

   updateLaunchButtonState();

   m_saveLog->setEnabled(true);
}

void MainWindow::updateLaunchButtonState()
{
   m_launchHtml->setEnabled(m_expert->htmlOutputPresent(m_ui->destDir->text()));
}

void MainWindow::showHtmlOutput()
{
   QString indexFile = m_expert->getHtmlOutputIndex(m_ui->destDir->text());
   QFileInfo fi(indexFile);

   // TODO: the following does not seem to work with IE

#ifdef WIN32
   //QString indexUrl(QString::fromAscii("file:///"));
   ShellExecute(NULL, L"open", (LPCWSTR)fi.absoluteFilePath().utf16(), NULL, NULL, SW_SHOWNORMAL);
#else
   QString indexUrl(QString::fromAscii("file://"));
   indexUrl += fi.absoluteFilePath();
   QDesktopServices::openUrl(QUrl(indexUrl));
#endif

}

void MainWindow::saveLog()
{
   QString fn = QFileDialog::getSaveFileName(this, tr("Save log file"),
                                             m_ui->destDir->text() + QString::fromAscii("/doxygen_log.txt"));

   if (!fn.isEmpty()) {
      QFile f(fn);

      if (f.open(QIODevice::WriteOnly)) {
         QTextStream t(&f);
         t << m_outputLog->toPlainText();
         statusBar()->showMessage(tr("Output log saved"), messageTimeout);

      } else {
         QMessageBox::warning(0, tr("Warning"),
                              tr("Cannot open file ") + fn + tr(" for writing. Nothing saved!"), tr("ok"));
      }
   }
}

void MainWindow::showSettings()
{
   QString text;
   QTextStream t(&text);
   m_expert->writeConfig(t, true);
   m_outputLog->clear();
   m_outputLog->append(text);
   m_outputLog->ensureCursorVisible();
   m_saveLog->setEnabled(true);
}

void MainWindow::configChanged()
{
   setDoxygenTitle(true);
}

