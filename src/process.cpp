/*************************************************************************
 *
 * Copyright (C) 2014-2017 Barbara Geller & Ansel Sermersheim
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
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

#include <QFileInfo>
#include <QTextCursor>

#include "mainwindow.h"
#include "dialog_args.h"
#include "dialog_find.h"

#ifdef WIN32
#include <windows.h>
#endif

// ** Run Options
void MainWindow::runDoxyPress()
{
   if (! m_running) {

      if (! saveDoxy()) {
         // save the current project file
         QString msg = m_curFile;

         if (msg.isEmpty()) {
            msg = "No project file name was selected, save aborted";
         } else  {
            msg += ", save project file aborted or failed";
         }

         runText_Append( QString("*** Unable to save DoxyPress project: %1\n").arg(msg));
         return;
      }

      QString doxyPressPath;

      if (! m_settings.doxyPressExe.trimmed().isEmpty() ) {
         // user specified
         doxyPressPath = m_settings.doxyPressExe;

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

      m_runProcess->setReadChannel(QProcess::StandardOutput);
      m_runProcess->setProcessChannelMode(QProcess::MergedChannels);

      QString projectDir = pathName(m_curFile);
      m_runProcess->setWorkingDirectory(projectDir);

      QStringList args;

      if (m_args.deleteHtml) {
         QString path = get_OutputDir() + "/" + m_ui->html_output->text();
         deleteOutputFiles(path);
      }

      // make stdout unbuffered
      args.append("--b");

      if (m_args.blank_layout) {
         args.append("--l");
         args.append(m_args.layout);
      }

      if (! m_args.style_html_header.isEmpty()) {
         args.append("--w");
         args.append("html-head");
         args.append(m_args.style_html_header);
      }

      if (! m_args.style_html_footer.isEmpty()) {
         args.append("--w");
         args.append("html-foot");
         args.append(m_args.style_html_footer);
      }

      if (! m_args.style_html_css.isEmpty()) {
         args.append("--w");
         args.append("html-style");
         args.append(m_args.style_html_css);
      }

      if (m_args.setDateTime) {
         args.append("--dt");
         args.append(m_args.dateTime);
      }

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
         m_ui->parameters_PB->setEnabled(false);
         m_ui->display_PB->setEnabled(false);
         m_ui->clear_PB->setEnabled(false);
         m_ui->find_PB->setEnabled(false);
         m_ui->save_log_PB->setEnabled(false);

         m_running = true;

         m_ui->run_PB->setText(tr("Stop DoxyPress"));
         m_ui->runStatus->setText(tr("DoxyPress is Running"));

//B      m_timer->start(1000);
      }

   } else {      
      // doxypress is running, stop it
      m_running = false;

      m_runProcess->kill();

      // m_ui->run_PB->setText(tr("Run DoxyPress"));
      // m_ui->runStatus->setText(tr("DoxyPress is Idle"));

//B   m_timer->stop();
   }
}

void MainWindow::readStdout()      
{   
   static int count = 0;

   if (m_running) {
      QByteArray tmp = m_runProcess->readAllStandardOutput();

      QString text = QString::fromLocal8Bit(tmp);

      if (! text.isEmpty()) {
         runText_Append(text);

         count += text.count('\n');
         if (count > 8) {
            QTextCursor cursor = m_ui->runText->textCursor();
            cursor.movePosition(QTextCursor::End);
            m_ui->runText->setTextCursor(cursor);

            m_ui->runText->ensureCursorVisible();
            count = 0;
         }
      }
   }
}

void MainWindow::runComplete()
{
   if (m_running) {

      if (m_runProcess->exitStatus() == QProcess::CrashExit) {
         runText_Append(tr("\n** DoxyPress aborted\n"));

      } else {
         runText_Append(tr("\n** DoxyPress completed\n"));
      }

   } else {
      runText_Append(tr("\n** DoxyPress cancelled\n"));

   }

   QTextCursor cursor = m_ui->runText->textCursor();
   cursor.movePosition(QTextCursor::End);
   m_ui->runText->setTextCursor(cursor);

   m_ui->runText->ensureCursorVisible();

   m_ui->run_PB->setText(tr("Ru&n DoxyPress"));
   m_ui->runStatus->setText(tr("DoxyPress is Idle"));

   m_running = false;

   updateRunButtons();
   m_syntaxParser->processSyntax();
}

//  **
void MainWindow::runText_Append(const QString &text)
{
   QTextCursor cursor(m_ui->runText->document());
   cursor.movePosition(QTextCursor::End);
   cursor.insertText(text);
}

void MainWindow::setArgs()
{
   Dialog_Args *dw = new Dialog_Args(this, m_args);
   int result = dw->exec();

   switch (result) {

      case QDialog::Rejected:
         break;

      case QDialog::Accepted:
         m_args = dw->get_Args();
         break;
   }
}

bool MainWindow::htmlOutputPresent() const
{
   bool generateHtml = m_ui->gen_html_CB1->isChecked();

   if (generateHtml) {

      QString dir = get_OutputDir() + "/" + m_ui->html_output->text();

      QString indexHtml = dir + "/index.html";
      QFileInfo fi(indexHtml);

      return (fi.exists() && fi.isFile());
   }

   return false;
}

void MainWindow::showHtmlOutput()
{   
   QString dir = get_OutputDir() + "/" + m_ui->html_output->text();

   QString indexHtml = dir + "/index.html";
   QFileInfo fileInfo(indexHtml);

   if (fileInfo.exists() && fileInfo.isFile()) {
      QString indexUrl = "file:///" + fileInfo.absoluteFilePath();
      bool ok = QDesktopServices::openUrl(QUrl(indexUrl));

      if (! ok)  {
         csError("Display HTML", "Unable to display generated documentation, " + indexUrl );
      }
   }
}

void MainWindow::clearOutput()
{
   m_ui->runText->document()->clear();
}

void MainWindow::find()
{
   Dialog_Find *dw = new Dialog_Find(this, m_findText);
   int result = dw->exec();

   if (result == QDialog::Accepted) {

      if (dw->get_SearchTop()) {
         QTextCursor cursor(m_ui->runText->textCursor());
         cursor.movePosition(QTextCursor::Start);
         m_ui->runText->setTextCursor(cursor);
      }

      m_findText = dw->get_findText();

      // get the flags
      m_flags = 0;

      m_direction  = dw->get_Direction();

      if (! m_direction) {
         m_flags |= QTextDocument::FindBackward;
      }

      m_case = dw->get_Case();
      if (m_case) {
         m_flags |= QTextDocument::FindCaseSensitively;
      }

      if (! m_findText.isEmpty())  {
         bool found = m_ui->runText->find(m_findText, m_flags);

         if (! found)  {
            // text not found, query if the user wants to search from top of file
            findNext();
         }
      }
   }

   delete dw;
}

void MainWindow::findNext()
{
   QTextDocument::FindFlags flags = QTextDocument::FindFlags(~QTextDocument::FindBackward & m_flags);
   bool found = m_ui->runText->find(m_findText, flags);

   if (! found)  {
      QString msg = "Not found: " + m_findText + "\n\n";
      msg += "Search from the beginning of this document?\n";

      QMessageBox msgFindNext(this);
      msgFindNext.setWindowTitle("Find");
      msgFindNext.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgFindNext.setDefaultButton(QMessageBox::Yes);
      msgFindNext.setText(msg);

      int result = msgFindNext.exec();

      if (result == QMessageBox::Yes) {

         // reset to the beginning of the document
         QTextCursor cursor(m_ui->runText->textCursor());
         cursor.movePosition(QTextCursor::Start);
         m_ui->runText->setTextCursor(cursor);

         // search again
         this->findNext();
      }
   }
}

void MainWindow::saveLog()
{
   QString projectDir = pathName(m_curFile);

   QString logName = QFileDialog::getSaveFileName(this, tr("Save log output"),
                  projectDir + "/doxypress_log.txt");

   if (! logName.isEmpty()) {
      QFile f(logName);

      if (f.open(QIODevice::WriteOnly)) {       

         QString data = m_ui->runText->toPlainText();
         f.write(data.toUtf8());

         setStatusBar(tr("Output log saved"));

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
      m_ui->find_PB->setEnabled(false);

   } else {  
      m_ui->save_log_PB->setEnabled(true);
      m_ui->clear_PB->setEnabled(true);
      m_ui->find_PB->setEnabled(true);

   }

   m_ui->parameters_PB->setEnabled(! m_running);

   bool isHtml = htmlOutputPresent();
   m_ui->display_PB->setEnabled(isHtml);
}

QString MainWindow::get_OutputDir() const
{
   QString outputDir  = m_ui->output_dir->text();
   QString projectDir = pathName(m_curFile);

   // test if outputdir is relative
   if (QDir::isAbsolutePath(outputDir)) {
      // do nothing

   } else if (outputDir.isEmpty()) {
      // outputdir was blank
      outputDir = projectDir;

   } else {
      // outputdir was relative
      outputDir = projectDir + "/" + outputDir;

   }

   return outputDir;
}

void MainWindow::deleteOutputFiles(const QString &path)
{
   QDir dir(path);

   if (dir.exists() && htmlOutputPresent()) {

      QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

      for (auto &fi : fileInfoList)  {

         if (fi.isDir() && ! fi.isSymLink()) {
            QDir temp(fi.filePath());
            temp.removeRecursively();

         } else {
            QFile::remove(fi.filePath());

         }
      }
   }
}

