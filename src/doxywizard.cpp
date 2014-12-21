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

void MainWindow::importDoxy()
{
   QMessageBox quest;
   quest.setWindowTitle(tr("CS Doxygen Import"));
   quest.setText( tr("Convert Doxygen configuration file to CS Doxygen format"));
   quest.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   quest.setDefaultButton(QMessageBox::No);

   int retval = quest.exec();

   if (retval == QMessageBox::Yes) {
      // BROOM - add code
      csMsg("user wants to do the convert");

   }
}

void MainWindow::newDoxy()
{
   csMsg("Create New Doxy file");
}

void MainWindow::openDoxy()
{
   if (querySave()) {
      QString fname = QFileDialog::getOpenFileName(this, tr("Open CS Doxygen CFG File"), m_struct.pathPrior);

      if (! fname.isEmpty()) {
         openDoxy_Internal(fname);
      }
   }
}

void MainWindow::openDoxy_Internal(const QString fname)
{
   m_ConfigFile = fname;

//   BROOM
//   m_expert->parseConfig(fileName, m_options);
//   m_wizard->refresh();

   updateLaunchButtonState();

   setDoxygenTitle(false);
}

void MainWindow::saveDox_Internal()
{
   if (m_ConfigFile.isEmpty()) {
      return;
   }

   QFile f(m_ConfigFile);

   if (! f.open(QIODevice::WriteOnly)) {
      QMessageBox::warning(this, tr("Error Saving: ") + m_ConfigFile, tr("Unable to save file: ") + f.error());
      return;
   }

   QTextStream t(&f);
   m_expert->writeConfig(t, false);   

   json_Write(PATH_PRIOR);

//   if (! m_struct.XXX.contains(m_ConfigFile)) {
//      addRecentFile(m_ConfigFile);
//   }

   setDoxygenTitle(false);
}

bool MainWindow::saveDoxyAs()
{    
   m_ConfigFile = QFileDialog::getSaveFileName(this, QString(), m_struct.pathPrior);

   if (m_ConfigFile.isEmpty()) {     
      return false;

   } else {
      saveDox_Internal();
   }

   return true;
}

void MainWindow::saveDoxy()
{
   if (m_ConfigFile.isEmpty()) {
      saveDoxyAs();

   } else {
      saveDox_Internal();
   }
}

//void MainWindow::configChanged()
//{
//   setDoxygenTitle(true);
//}


/*

// ** Settings Options
void MainWindow::makeDefaults()
{
   if (QMessageBox::question(this, tr("Use current setting at startup?"),
                             tr("Do you want to save the current settings "
                                "and use them next time Doxywizard starts?"),
                             QMessageBox::Save | QMessageBox::Cancel) == QMessageBox::Save) {

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
                             QMessageBox::Reset | QMessageBox::Cancel) == QMessageBox::Reset) {

      m_expert->resetToDefaults();
      m_settings.setValue(QString::fromAscii("wizard/loadsettings"), false);
      m_settings.sync();

      m_wizard->refresh();
   }
}

*/

void MainWindow::saveSettings()
{
  json_Write(CLOSE);
}


// ** Run Options
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
         m_outputLog->append(QString::fromAscii("*** Failed to run CS Doxygen\n"));
      } else {
         m_saveLog->setEnabled(false);
         m_running = true;
         m_run->setText(tr("Stop CS Doxygen"));
         m_runStatus->setText(tr("Status: running"));
         m_timer->start(1000);
      }

   } else {
      m_running = false;
      m_run->setText(tr("Run CS Doxygen"));
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

      if (! text.isEmpty()) {
         m_outputLog->append(text.trimmed());
      }
   }
}

void MainWindow::runComplete()
{
   if (m_running) {
      m_outputLog->append(tr("*** CS Doxygen has completed\n"));
   } else {
      m_outputLog->append(tr("*** CS Doxygen was cancelled\n"));
   }

   m_outputLog->ensureCursorVisible();
   m_run->setText(tr("Run CS Doxygen"));
   m_runStatus->setText(tr("Status: CS Doxygen is not running"));
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
                                             m_ui->destDir->text() + QString::fromAscii("/doxy_log.txt"));

   if (! fn.isEmpty()) {
      QFile f(fn);

      if (f.open(QIODevice::WriteOnly)) {
         QTextStream t(&f);
         t << m_outputLog->toPlainText();
         statusBar()->showMessage(tr("Output log saved"), messageTimeout);

      } else {
         QMessageBox::warning(this, tr("Error Saving: ") + fn, tr("Unable to save file: ") + f.error());

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


