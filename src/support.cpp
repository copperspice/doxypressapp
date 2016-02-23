/*************************************************************************
 * 
 * Copyright (C) 2014-2016 Barbara Geller & Ansel Sermersheim
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

QString MainWindow::get_DirPath(QString message, QString initialPath, enum RelativeX relativeTo)
{
   QString retval = initialPath;

   QString outputDir;
   QString projectDir;

   if (relativeTo != ABSOLUTE_ONLY) {
      // part 1 - find the real outputDir

      // a
      projectDir = pathName(m_curFile);

      // test if outputdir is relative
      outputDir = m_ui->output_dir->text();

      if (QDir::isAbsolutePath(outputDir)) {
         // do nothing

      } else if (outputDir.isEmpty()) {
         // outputdir was blank
         outputDir = projectDir;

      } else {
         // outputdir was relative
         outputDir = projectDir + "/" + outputDir;

      }

      // b
      if (relativeTo == PROJECT_DIR) {

         if (QDir::isAbsolutePath(initialPath) )  {
            // do nothing

         } else if (initialPath.isEmpty())  {
            retval = projectDir;

         } else {
            retval = projectDir + "/" + initialPath;

         }

      } else {

         if (QDir::isAbsolutePath(initialPath) )  {
            // do nothing

         } else if (initialPath.isEmpty())  {
            retval = outputDir;

         } else {
            retval = outputDir + "/" + initialPath;

         }
      }
   }

   // part 2 - user can select a new path
   QFileDialog::Options options;
   options |= QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks; 

#ifdef Q_OS_WIN

   if (QSysInfo::WindowsVersion < QSysInfo::WV_VISTA) {

      Dialog_XP_GetDir *dw = new Dialog_XP_GetDir(this, message, retval, options);
      int result = dw->exec();

      if (result == QDialog::Accepted) {
         retval = dw->getDirectory();
      } else {
         retval = "";
      }

   } else {
      retval = QFileDialog::getExistingDirectory(this, message, retval, options);

   }

#else
   // on X11 the title bar may not be displayed
   retval = QFileDialog::getExistingDirectory(this, message, retval, options);

#endif

   if (retval.isEmpty())  {
      // user canceled dialog
      return initialPath;

   } else if (relativeTo != ABSOLUTE_ONLY) {
      // turn absolute path into a relative path if possible

      QDir temp(retval + "/");
      retval = temp.canonicalPath();

      if (relativeTo == PROJECT_DIR) {
         projectDir = QDir(projectDir).canonicalPath();

         if (retval.startsWith(projectDir)) {
            retval = retval.mid(projectDir.length() + 1);
         }

      } else {
         outputDir = QDir(outputDir).canonicalPath();

         if (retval.startsWith(outputDir)) {
            retval = retval.mid(outputDir.length() + 1 );
         }

      }

      // show user something
      if (retval.isEmpty()) {
         retval = ".";
      }
   }

   return retval;
}

QString MainWindow::getSingleFile(QString title, QString fname, QString filter)
{
   QString retval      = fname;
   QString projectDir  = pathName(m_curFile);

   if (! QDir::isAbsolutePath(retval) )  {
      retval = projectDir + "/" + retval;
   }

   QString initialPath = pathName(retval);

   QString selectedFilter;
   QFileDialog::Options options;

   // force windows 7 and 8 to honor initial path
   options = QFileDialog::ForceInitialDir_Win7;

   QString newFile = QFileDialog::getOpenFileName(this, title, initialPath, filter, &selectedFilter, options);

   if (newFile.isEmpty()) {
      // fname might have been relative
      retval = fname;

    } else {
      retval = newFile;

      // turn absolute path into a relative path if possible
      QDir temp(retval + "/");
      retval = temp.canonicalPath();

      projectDir = QDir(projectDir).canonicalPath();

      if (retval.startsWith(projectDir)) {
         retval = retval.mid(projectDir.length() + 1);
      }
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
      QString fname = QFileDialog::getOpenFileName(this, tr("Open DoxyPress Project"), m_settings.pathPrior);

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
   m_settings.pathPrior = this->pathName(fname);

   json_Write(PATH_PRIOR);

   if (! m_rf_List.contains(m_curFile)) {
      rf_Update();
   }

   //
   json_OpenDoxy(data);   

   updateRunButtons();
   clearOutput();

   setDoxyTitle(false);

   return true;
}

QString MainWindow::pathName(QString fileName) const
{
   QString retval = "";

   if (! fileName.isEmpty())  {      
      QFileInfo temp(fileName);

      if (temp.isAbsolute()) {
         retval = temp.absolutePath();
      }
   }

   return retval;
}

bool MainWindow::querySave()
{
   bool retval = true;

   if (m_modified) {

      QMessageBox quest;
      quest.setWindowTitle(tr("DoxyPressApp"));
      quest.setWindowIcon(QIcon("://resources/doxypress.png"));

      quest.setText( tr("DoxyPress file has been modified. Save changes?"));
      quest.setStandardButtons(QMessageBox::Save | QMessageBox::Discard  | QMessageBox::Cancel );
      quest.setDefaultButton(QMessageBox::Cancel);

      int retval = quest.exec();

      if (retval == QMessageBox::Save) {
         retval = saveDoxy();

      } else if (retval == QMessageBox::Cancel) {
         retval = false;

      }
   }

   return retval;
}

void MainWindow::reloadDoxy()
{
   clearAllFields();

   if (! m_curFile.isEmpty()) {
      openDoxy_Internal(m_curFile);
   }
}

bool MainWindow::saveDoxy_Internal()
{
   QByteArray data = json_SaveDoxy();
   QFile file(m_curFile);

   if (! file.open(QIODevice::WriteOnly)) {
      QMessageBox::warning(this, tr("Error Saving: ") + m_curFile, tr("Unable to save: ") + file.error());
      return false;
   }

   file.write(data);
   file.close();

   setDoxyTitle(false);

   return true;
}

bool MainWindow::saveDoxyAs()
{
   bool retval;

   m_curFile = QFileDialog::getSaveFileName(this, tr("DoxyPress project file"), m_settings.pathPrior,
                                            tr("Json Files (*.json)"));

   if (m_curFile.isEmpty()) {
      retval = false;

   } else {
      retval = saveDoxy_Internal();

      if (retval) {
         json_Write(PATH_PRIOR);

         if (! m_rf_List.contains(m_curFile)) {
            rf_Update();
         }
      }
   }

   return retval;
}

bool MainWindow::saveDoxy()
{
   bool retval;     

   // test json format
   if (m_doxypressFormat != DOXYPRESS_FORMAT)  {

      QString msg = "The current project file is using an older format. Saving will automatically "
            "update to the new format.";

      QMessageBox msgB;
      msgB.setWindowTitle("Save Project File");
      msgB.setWindowIcon(QIcon("://resources/doxypress.png"));

      msgB.setText(msg);
      msgB.setInformativeText("Continue?");

      msgB.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
      int retval = msgB.exec();

      if (retval != QMessageBox::Save) {
         return false;
      }
   }

   if (m_curFile.isEmpty()) {
      retval = saveDoxyAs();

   } else {
      retval = saveDoxy_Internal();
   }

   return retval;
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
   QString temp;

   if (m_modified) {
      temp = QChar(0x02014);
   }

   if (m_curFile.isEmpty())  {
      setWindowTitle(tr("DoxyPressApp ") + " [*]" );

   } else {
      setWindowTitle(tr("DoxyPressApp ") + temp + " " + m_curFile + " [*]" );

   }
}

void MainWindow::setStatusBar(QString msg)
{     
   statusBar()->showMessage(msg, 5000);
}
