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

#include "dialog_xp_getdir.h"
#include "mainwindow.h"

#include <QFileInfo>

void MainWindow::closeEvent(QCloseEvent *event)
{ 
   if (querySave()) {
      saveSettings();
      event->accept();

   } else {
      event->ignore();
   }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
   //  process events for help
   if (event->type() == QEvent::HoverEnter) {
      static QWidget *old_Widget = nullptr;

      QWidget *new_Widget = dynamic_cast<QWidget *>(object);

      if (new_Widget) {
         hoverChanged(old_Widget, new_Widget);

         // save for next time around
         old_Widget = new_Widget;
      }
   }

   return QObject::eventFilter(object, event);
}

QString MainWindow::get_DirPath(QString message, QString initialPath, QString relativePath, bool isOutputDir)
{
   QString path = initialPath;

   QString outputDir  = m_ui->output_dir->text();
   QString projectDir = pathName(m_curFile);

   if (path.startsWith("$output_dir"))  {
      path.replace("$output_dir", outputDir);

   } else if (path.startsWith("$project_dir"))  {
      path.replace("$project_dir", projectDir);

   } else if (path.isEmpty()) {
      path = relativePath;

   }

   // just text, if relative then append
   if (QDir::isRelativePath(path) )  {
      path = relativePath + "/" + path;
   }  

   QFileDialog::Options options;
   options |= QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks; 

#ifdef Q_OS_WIN

   if (QSysInfo::WindowsVersion < QSysInfo::WV_VISTA) {

      Dialog_XP_GetDir *dw = new Dialog_XP_GetDir(this, message, path, options);
      int result = dw->exec();

      if (result == QDialog::Accepted) {
         path = dw->getDirectory();
      } else {
         path = "";
      }

   } else {
      path = QFileDialog::getExistingDirectory(this, message, path, options);

   }

#else
   // on X11 the title bar may not be displayed
   path = QFileDialog::getExistingDirectory(this, message, path, options);

#endif

   if (path.isEmpty())  {
      // user canceled dialog
      return initialPath;
   }

   // adjust for platform slash issue
   if (! path.isEmpty()) {      
      QDir temp(path + "/");
      path = temp.canonicalPath() + "/";
   }     

   if (! isOutputDir) {
      // resolve relative path

      if (path.startsWith(outputDir))  {
         path.replace(outputDir, "$output_dir/");

      } else if (path.startsWith(projectDir))  {
         path.replace(projectDir,"$project_dir/");

      }
   }

   return path;
}

QString MainWindow::getSingleFile(QString title, QString fname, QString filter)
{
   QString retval = fname;

   // save last path used
   static QString path;

   if (! fname.isEmpty()) {
      path = fname;
   }

   QString selectedFilter;
   QFileDialog::Options options;

   // force windows 7 and 8 to honor initial path
   options = QFileDialog::ForceInitialDir_Win7;

   QString file = QFileDialog::getOpenFileName(this, title, path, filter, &selectedFilter, options);

   if (! file.isEmpty()) {
      retval = file;
   }

   return retval;
}

// **
void MainWindow::newDoxy()
{
   if (querySave()) {
      clearAllFields();
   }
}

void MainWindow::openDoxy()
{
   if (querySave()) {
      QString fname = QFileDialog::getOpenFileName(this, tr("Open DoxyPress Project"), m_struct.pathPrior);

      if (! fname.isEmpty()) {
         openDoxy_Internal(fname);
      }
   }
}

bool MainWindow::openDoxy_Internal(const QString fname)
{  
   QFile file(fname);

   if (! file.open(QIODevice::ReadOnly)) {
      QMessageBox::warning(this, tr("Error Opening: ") + fname, tr("Unable to open: ") + file.error());
      return false;
   }

   QByteArray data;

   data = file.readAll();
   file.close();

   //
   m_curFile = fname;
   m_struct.pathPrior = this->pathName(fname);

   json_Write(PATH_PRIOR);

   if (! m_rf_List.contains(m_curFile)) {
      rf_Update();
   }

   //
   json_OpenDoxy(data);   

   updateRunButtons();
   setDoxyTitle(false);

   return true;
}

QString MainWindow::pathName(QString fileName) const
{
   QString retval = "";

   if (! fileName.isEmpty())  {
      retval = QFileInfo(fileName).absolutePath();
   }

   return retval;
}

bool MainWindow::querySave()
{
   if (m_modified) {

      QMessageBox quest;
      quest.setWindowTitle(tr("DoxyPressApp"));
      quest.setText( tr("DoxyPress file has been modified. Save changes?"));
      quest.setStandardButtons(QMessageBox::Save | QMessageBox::Discard  | QMessageBox::Cancel );
      quest.setDefaultButton(QMessageBox::Cancel);

      int retval = quest.exec();

      if (retval == QMessageBox::Save) {
         saveDoxy();
         return true;

      } else if (retval == QMessageBox::Cancel) {
         return false;

      }
   }

   return true;
}

void MainWindow::reloadDoxy()
{
   clearAllFields();

   if (! m_curFile.isEmpty()) {
      openDoxy_Internal(m_curFile);
   }
}

void MainWindow::saveDoxy_Internal()
{
   QByteArray data = json_SaveDoxy();

   QFile file(m_curFile);

   if (! file.open(QIODevice::WriteOnly)) {
      QMessageBox::warning(this, tr("Error Saving: ") + m_curFile, tr("Unable to save: ") + file.error());
      return;
   }

   file.write(data);
   file.close();

   setDoxyTitle(false);
}

bool MainWindow::saveDoxyAs()
{
   m_curFile = QFileDialog::getSaveFileName(this, tr("DoxyPress project file"), m_struct.pathPrior,
                                            tr("Json Files (*.json)"));

   if (m_curFile.isEmpty()) {
      return false;

   } else {
      saveDoxy_Internal();

      json_Write(PATH_PRIOR);

      if (! m_rf_List.contains(m_curFile)) {
         rf_Update();
      }
   }

   return true;
}

void MainWindow::saveDoxy()
{
   if (m_curFile.isEmpty()) {
      saveDoxyAs();

   } else {
      saveDoxy_Internal();
   }
}

void MainWindow::saveSettings()
{
  json_Write(CLOSE);
}

void MainWindow::setDoxyTitle(bool isModified)
{
   m_modified = isModified;   
   setWindowModified(m_modified);

   // displays as: DoxyPressApp -- FileName[*]
   QString temp = QChar(0x02014);

   if (m_curFile.isEmpty())  {

      if (m_modified) {
         setWindowTitle(tr("DoxyPressApp ") + temp + " [*]" );
      } else {
         setWindowTitle(tr("DoxyPressApp ") );
      }

   } else {     

      if (m_modified) {
         setWindowTitle(tr("DoxyPressApp ") + temp + " " + m_curFile + " [*]" );
      } else {
         setWindowTitle(tr("DoxyPressApp ") + m_curFile );
      }
   }
}

void MainWindow::setStatusBar(QString msg)
{     
   statusBar()->showMessage(msg, 5000);
}

void MainWindow::setStatusBar(QString msg, int timeOut)
{
   statusBar()->showMessage(msg, timeOut);
}

