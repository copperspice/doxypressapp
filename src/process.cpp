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

// ** Run Options
void MainWindow::runDoxyPress()
{
   if (! m_running) {
      QString path;

#if defined(Q_OS_MACX)
      path = qApp->applicationDirPath() + QString::fromAscii("/../Resources/");
      qDebug() << tr("DoxyPress path: ") << path;

      if ( ! QFile(path + QString::fromAscii("doxypress")).exists() ) {

         if ( QFile(QString::fromAscii("/usr/local/bin/doxypress")).exists() ) {
            path = QString::fromAscii("/usr/local/bin/");

         } else {
            qDebug() << tr("Unable to find DoxyPress, make sure it is in your $$PATH");
            path = QString::fromAscii("");
         }
      }

      qDebug() << tr("Running DoxyPress from: ") << path;
#endif

      QString destDir = m_ui->output_dir->text();

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
      m_runProcess->start(path + QString::fromAscii("doxypress"), args);

      if (! m_runProcess->waitForStarted()) {
         m_outputLog->append(QString::fromAscii("*** Failed to run DoxyPress\n"));
         return;
      }

   }


/*    brooom - add this back in

      QTextStream t(m_runProcess);
      m_expert->writeConfig(t, false);
      m_runProcess->closeWriteChannel();

      if (m_runProcess->state() == QProcess::NotRunning) {
         m_outputLog->append("*** Failed to run CS Doxygen\n");

      } else {
         m_saveLog->setEnabled(false);

         m_running = true;

         m_ui->run_PB->setText(tr("Stop CS Doxygen"));
         m_ui->runStatus->setText(tr("CS Doxygen: Running"));

         m_timer->start(1000);
      }

   } else {
      m_running = false;      

      m_ui->run_PB->setText(tr("Run DoxyPress"));
      m_ui->runStatus->setText(tr("DoxyPress is Idle"));

      m_runProcess->kill();
      m_timer->stop();      
   }

*/

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
      m_outputLog->append(tr("*** DoxyPress has completed\n"));
   } else {
      m_outputLog->append(tr("*** DoxyPress was cancelled\n"));
   }

   m_outputLog->ensureCursorVisible();

   m_run->setText(tr("Run DoxyPress"));
   m_runStatus->setText(tr("Status: DoxyPress is not running"));

   m_running = false;

   updateLaunchButtonState();

   m_saveLog->setEnabled(true);
}

bool MainWindow::htmlOutputPresent(const QString &workingDir) const
{      
   bool retval = false;

/*
   bool generateHtml = getBoolOption(m_options, QString::fromAscii("GENERATE_HTML"));

   if (! generateHtml || workingDir.isEmpty()) {
      return retval;
   }

   QString indexFile = getHtmlOutputIndex(workingDir);
   QFileInfo fi(indexFile);

   retval = (fi.exists() && fi.isFile());

*/

   return retval;
}

void MainWindow::showHtmlOutput()
{

/*   BROOM - put back in


   QString indexFile = m_expert->getHtmlOutputIndex(m_ui->source_output->text());
   QFileInfo fi(indexFile);

   // TODO: the following does not seem to work with IE

#ifdef WIN32   
   ShellExecute(NULL, L"open", (LPCWSTR)fi.absoluteFilePath().utf16(), NULL, NULL, SW_SHOWNORMAL);

#else
   QString indexUrl(QString::fromAscii("file://"));
   indexUrl += fi.absoluteFilePath();
   QDesktopServices::openUrl(QUrl(indexUrl));

#endif

*/

}

void MainWindow::saveLog()
{
   QString fn = QFileDialog::getSaveFileName(this, tr("Save log file"),
                                             m_ui->output_dir->text() + QString::fromAscii("/doxy_log.txt"));

   if (! fn.isEmpty()) {
      QFile f(fn);

      if (f.open(QIODevice::WriteOnly)) {
         QTextStream t(&f);
         t << m_outputLog->toPlainText();

         statusBar()->showMessage(tr("Output log saved"));

      } else {
         QMessageBox::warning(this, tr("Error Saving: ") + fn, tr("Unable to save file: ") + f.error());

      }
   }
}

void MainWindow::showSettings()
{

/*   BROOM - put back in
 *
   QString text;
   QTextStream t(&text);

   m_expert->writeConfig(t, true);
   m_outputLog->clear();
   m_outputLog->append(text);
   m_outputLog->ensureCursorVisible();
   m_saveLog->setEnabled(true);

*/

}






