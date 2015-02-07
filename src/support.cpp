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
//   setDoxygenTitle(true);
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
void MainWindow::initTabs()
{
   icon_PB("load");

/*
   if (getBoolOption(m_modelData, STR_HAVE_DOT)) {
      // Dot
      m_diagramModeGroup->button(2)->setChecked(true);

   } else if (getBoolOption(m_modelData, STR_CLASS_DIAGRAMS)) {
      // Builtin diagrams
      m_diagramModeGroup->button(1)->setChecked(true);

   } else {
      // no diagrams
      m_diagramModeGroup->button(0)->setChecked(true);
   }

   m_dotClass->setChecked(getBoolOption(m_modelData, STR_CLASS_GRAPH));
   m_dotCollaboration->setChecked(getBoolOption(m_modelData, STR_COLLABORATION_GRAPH));
   m_dotInheritance->setChecked(getBoolOption(m_modelData, STR_GRAPHICAL_HIERARCHY));
   m_dotInclude->setChecked(getBoolOption(m_modelData, STR_INCLUDE_GRAPH));
   m_dotIncludedBy->setChecked(getBoolOption(m_modelData, STR_INCLUDED_BY_GRAPH));
   m_dotCall->setChecked(getBoolOption(m_modelData, STR_CALL_GRAPH));
   m_dotCaller->setChecked(getBoolOption(m_modelData, STR_CALLER_GRAPH));

*/
}

void MainWindow::newDoxy()
{
   if (querySave()) {
      clearAllFields();
   }
}

void MainWindow::openDoxy()
{
   if (querySave()) {
      QString fname = QFileDialog::getOpenFileName(this, tr("Open CS Doxygen Config"), m_struct.pathPrior);

      if (! fname.isEmpty()) {
         openDoxy_Internal(fname);
      }
   }
}

void MainWindow::openDoxy_Internal(const QString fname)
{
   m_ConfigFile = fname;

   QFile file(m_ConfigFile);

   if (! file.open(QIODevice::ReadOnly)) {
      QMessageBox::warning(this, tr("Error Opening: ") + m_ConfigFile, tr("Unable to open: ") + file.error());
      return;
   }

   QByteArray data;

   data = file.readAll();
   file.close();

   json_Write(PATH_PRIOR);
//   if (! m_struct.XXX.contains(m_ConfigFile)) {
//      addRecentFile(m_ConfigFile);
//   }

   json_OpenDoxy(data);

   initTabs();
   updateLaunchButtonState();

   setDoxygenTitle(false);
}

QString MainWindow::pathName(QString fileName) const
{
   return QFileInfo(fileName).path();
}

bool MainWindow::querySave()
{
   if (m_modified) {

      QMessageBox quest;
      quest.setWindowTitle(tr("CS Doxygen"));
      quest.setText( tr("Configuration file has been modified. Save changes?"));
      quest.setStandardButtons(QMessageBox::Save | QMessageBox::Discard  | QMessageBox::Cancel );
      quest.setDefaultButton(QMessageBox::Cancel);

      int retval = quest.exec();

      if (retval == QMessageBox::Save) {
         // saveDoxCfgFile();
         csMsg("Do the save now");

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

   if (! m_ConfigFile.isEmpty()) {
      openDoxy_Internal(m_ConfigFile);
   }
}

void MainWindow::saveDoxy_Internal()
{
   QByteArray data = json_SaveDoxy();

   QFile file(m_ConfigFile);

   if (! file.open(QIODevice::WriteOnly)) {
      QMessageBox::warning(this, tr("Error Saving: ") + m_ConfigFile, tr("Unable to save: ") + file.error());
      return;
   }

   file.write(data);
   file.close();

   setDoxygenTitle(false);
}

bool MainWindow::saveDoxyAs()
{
   m_ConfigFile = QFileDialog::getSaveFileName(this, tr("Select name for CS Doxygen Config"), m_struct.pathPrior);

   if (m_ConfigFile.isEmpty()) {
      return false;

   } else {
      saveDoxy_Internal();

      json_Write(PATH_PRIOR);
 //   if (! m_struct.XXX.contains(m_ConfigFile)) {
 //      addRecentFile(m_ConfigFile);
 //   }

   }

   return true;
}

void MainWindow::saveDoxy()
{
   if (m_ConfigFile.isEmpty()) {
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

   // displays as: CS Doxygen --  ConfigFileName[*]
   if (m_ConfigFile.isEmpty())   {

      setWindowTitle(tr("CS Doxygen ") );

   } else {
      QString temp = QChar(0x02014);

      if (m_modified) {
         setWindowTitle(tr("CS Doxygen ") + temp + " " + m_ConfigFile + " [*]" );
      } else {
         setWindowTitle(tr("CS Doxygen ") + temp + " " + m_ConfigFile );
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

void MainWindow::updateLaunchButtonState()
{
   bool ok = false;           //  BROOM m_expert->htmlOutputPresent(m_ui->output->text());
   m_ui->display_PB->setEnabled(ok);
}
