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

void MainWindow::closeEvent(QCloseEvent *event)
{ 
   if (querySave()) {
      saveSettings();
      event->accept();

   } else {
      event->ignore();
   }
}

//void MainWindow::configChanged()
//{
//   setDoxyTitle(true);
//}

QString MainWindow::getFile_CS(QString title, QString fname, QString filter)
{
   QString selectedFilter;
   QFileDialog::Options options;

   // force windows 7 and 8 to honor initial path
   options = QFileDialog::ForceInitialDir_Win7;

   fname = m_appPath + "/" + fname;

   QString file = QFileDialog::getOpenFileName(this, "Select " + title,
         fname, filter, &selectedFilter, options);

   return file;
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
   return QFileInfo(fileName).path();
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
   m_curFile = QFileDialog::getSaveFileName(this, tr("New DoxyPress project file"), m_struct.pathPrior,
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

   // displays as: DoxyPressApp --  ConfigFileName[*]
   if (m_curFile.isEmpty())   {

      setWindowTitle(tr("DoxyPressApp ") );

   } else {
      QString temp = QChar(0x02014);

      if (m_modified) {
         setWindowTitle(tr("DoxyPressApp ") + temp + " " + m_curFile + " [*]" );
      } else {
         setWindowTitle(tr("DoxyPressApp ") + temp + " " + m_curFile );
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
