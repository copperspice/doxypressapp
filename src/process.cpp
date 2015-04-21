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
      // save the project file
      saveDoxy();                

      QString doxyPressPath;

      if (! m_struct.doxyPressExe.trimmed().isEmpty() ) {
         // user specified
         doxyPressPath = m_struct.doxyPressExe;

      } else {
         // test if DoxyPress is in the same directory as the current app
         QString dir = QCoreApplication::applicationDirPath();

         QDir file(dir);
         if (file.exists("doxypress.exe")) {
            doxyPressPath = dir + "/doxypress.exe";

         } else if (file.exists("doxypress")) {
            doxyPressPath = dir + "/doxypress";

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

      QDir outf(outputDir);

      if (outf.exists()) {
         m_runProcess->setWorkingDirectory(outputDir);

      } else {
         runText_Append( QString("*** Your project 'Output Directory' tag has a value of: %1 \n"
                                 "This directory does not exist, please correct\n").arg(outputDir));
         return;

      }


      QStringList args;     
      args.append("--b");           // make stdout unbuffered
      args.append(m_curFile);

      clearOutput();
      m_runProcess->start(doxyPressPath, args);

      if (! m_runProcess->waitForStarted()) {
         // show possible error messages
         readStdout();

         runText_Append( QString("*** Failed to start %1\n").arg(doxyPressPath));

         return;
      }            

      //
      if (m_runProcess->state() == QProcess::NotRunning) {
         // show possible error messages
         readStdout();

         runText_Append( QString("*** Failed to run %1\n").arg(doxyPressPath));

      } else {
         m_ui->save_log_PB->setEnabled(false);
         m_ui->clear_PB->setEnabled(false);

         m_running = true;

         m_ui->run_PB->setText(tr("Stop DoxyPress"));
         m_ui->runStatus->setText(tr("DoxyPress is Running"));

//B      m_timer->start(1000);
      }

   } else {      
      // doxypress is running, stop it
      m_running = false;

      m_runProcess->kill();

      m_ui->run_PB->setText(tr("Run DoxyPress"));
      m_ui->runStatus->setText(tr("DoxyPress is Idle"));

//B   m_timer->stop();
   }
}

void MainWindow::readStdout()      
{
  if (m_running) {
      QString text = m_runProcess->readAllStandardOutput();

      if (! text.isEmpty()) {
         runText_Append(text);
      }
  }
}

void MainWindow::runComplete()
{
   if (m_running) {

      if (m_runProcess->exitStatus() == QProcess::CrashExit) {
         runText_Append(tr("\n** DoxyPress Aborted\n"));

      } else {
         runText_Append(tr("\n** DoxyPress has Completed\n"));
      }

   } else {
      runText_Append(tr("\n** DoxyPress was Cancelled\n"));
   }

   m_ui->runText->ensureCursorVisible();

   m_ui->run_PB->setText(tr("Run DoxyPress"));
   m_ui->runStatus->setText(tr("DoxyPress is Idle"));

   m_running = false;

   updateRunButtons();
   m_syntaxParser->processSyntax();

   m_ui->save_log_PB->setEnabled(true);
   m_ui->clear_PB->setEnabled(true);
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
   QFileInfo fileInfo(pathHtml);

   QString indexUrl = "file:///" + fileInfo.absoluteFilePath();
   bool ok = QDesktopServices::openUrl(QUrl(indexUrl));

   if (! ok)  {
      csError("Display HTML", "Unable to display generated documentation, " + indexUrl );
   }
}

void MainWindow::clearOutput()
{
   m_ui->runText->document()->clear();
}

void MainWindow::saveLog()
{
   QString logName = QFileDialog::getSaveFileName(this, tr("Save log file"),
                  m_ui->output_dir->text() + "/doxypress_log.txt");

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
      m_ui->clear_PB->setEnabled(false);

   } else {
      m_ui->save_log_PB->setEnabled(true);
      m_ui->clear_PB->setEnabled(true);

   }

   bool isHtml = htmlOutputPresent();
   m_ui->display_PB->setEnabled(isHtml);
}

