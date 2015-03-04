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
 * Documents produced by DoxyPress are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#include "mainwindow.h"

#include <QFileInfo>
#include <QTextCursor>

#ifdef WIN32
#include <windows.h>
#endif

// ** Run Options
void MainWindow::runDoxyPress()
{
   if (! m_running) {
      // save the file
      saveDoxy();                

      QString doxyPressPath;

      if (! m_struct.doxyPress_Exe.isEmpty() ) {
         // user specified
         doxyPressPath = m_struct.doxyPress_Exe;

      } else {
         // test if DoxyPress in the same directory as the current app
         QString dir = QCoreApplication::applicationDirPath();

         QDir file(dir);
         if (file.exists("doxypress.exe")) {
            doxyPressPath = dir + "doxypress.exe";

         } else if (file.exists("doxypress")) {
            doxyPressPath = dir + "doxypress";

         } else {
            // search user path
            doxyPressPath = "doxypress";
         }
      }

      if (doxyPressPath.isEmpty()) {
         runText_Append("*** Unable to locate DoxyPress program\n");
         return;
      }

      QString outputDir = m_ui->output_dir->text();

      m_runProcess->setReadChannel(QProcess::StandardOutput);
      m_runProcess->setProcessChannelMode(QProcess::MergedChannels);
      m_runProcess->setWorkingDirectory(outputDir);

      QStringList args;
      args.append(m_curFile);
      args.append(" --b");           // make stdout unbuffered

      m_ui->runText->document()->clear();
      m_runProcess->start(doxyPressPath, args);

      if (! m_runProcess->waitForStarted()) {
         runText_Append( QString("*** Failed to run %1\n").arg(doxyPressPath));
         return;
      }            

      //
      if (m_runProcess->state() == QProcess::NotRunning) {
         runText_Append("*** Failed to run DoxyPress\n");

      } else {
         m_ui->save_log_PB->setEnabled(false);

         m_running = true;

         m_ui->run_PB->setText(tr("Stop DoxyPress"));
         m_ui->runStatus->setText(tr("DoxyPress: Running"));

//B      m_timer->start(1000);
      }

   } else {
      m_running = false;      

      m_ui->run_PB->setText(tr("Run DoxyPress"));
      m_ui->runStatus->setText(tr("DoxyPress is Idle"));

//B   m_runProcess->kill();
//B   m_timer->stop();
   }
}

void MainWindow::readStdout()      
{
   if (m_running) {      
      QString text = m_runProcess->readAllStandardOutput();

      if (! text.isEmpty()) {
         runText_Append(text.trimmed());
      }
   }
}

void MainWindow::runComplete()
{
   if (m_running) {
      runText_Append(tr("\n** DoxyPress has Completed\n"));
   } else {
      runText_Append(tr("\n** DoxyPress was Cancelled\n"));
   }

   m_ui->runText->ensureCursorVisible();

   m_ui->run_PB->setText(tr("Run DoxyPress"));
   m_ui->runStatus->setText(tr("Status: DoxyPress is not running"));

   m_running = false;

   updateRunButtons();
   m_syntaxParser->processSyntax();

   m_ui->save_log_PB->setEnabled(true);
}

//  **
void MainWindow::runText_Append(const QString &text)
{
   QTextCursor cursor(m_ui->runText->document());
   cursor.movePosition(QTextCursor::End);
   cursor.insertText(text);
}

QString MainWindow::getHtmlOutputIndex() const
{
   QString retval = this->pathName(m_curFile);

   QString outputDir = m_ui->output_dir->text();
   QString htmlDir   = m_ui->html_output->text();

   if (QFileInfo(outputDir).isAbsolute())  {
      // override
      retval = outputDir;

   } else if (! outputDir.isEmpty())   {
      // append
      retval += "/" + outputDir;
   }

   if (QFileInfo(htmlDir).isAbsolute())  {
      // override
      retval = htmlDir;

   } else if (! htmlDir.isEmpty())  {
      // append
      retval += "/" + htmlDir;
   }

   retval += "/index.html";

   return retval;
}


bool MainWindow::htmlOutputPresent() const
{
   bool generateHtml = m_ui->gen_html_CB1->isChecked();
   QString dir = m_ui->output_dir->text();

   if (! generateHtml || dir.isEmpty()) {
      return false;
   }

   QString pathHtml = getHtmlOutputIndex();
   QFileInfo fi(pathHtml);

   return (fi.exists() && fi.isFile());
}

void MainWindow::showHtmlOutput()
{
   QString pathHtml = getHtmlOutputIndex();
   QFileInfo fi(pathHtml);

   // BROOM -- the following does not seem to work with IE

/*
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
   QString logName = QFileDialog::getSaveFileName(this, tr("Save log file"),
                  m_ui->output_dir->text() + QString::fromAscii("/doxypress_log.txt"));

   if (! logName.isEmpty()) {
      QFile f(logName);

      if (f.open(QIODevice::WriteOnly)) {       

         QString data = m_ui->runText->toPlainText();
         f.write(data.toUtf8());

         statusBar()->showMessage(tr("Output log saved"));

      } else {
         QMessageBox::warning(this, tr("Error Saving: ") + logName, tr("Unable to save file: ") + f.error());

      }
   }
}

void MainWindow::updateRunButtons()
{
   if (m_ui->runText->toPlainText().isEmpty())  {
      m_ui->save_log_PB->setEnabled(false);
   } else {
      m_ui->save_log_PB->setEnabled(true);
   }

   bool isHtml = htmlOutputPresent();
   m_ui->display_PB->setEnabled(isHtml);
}

