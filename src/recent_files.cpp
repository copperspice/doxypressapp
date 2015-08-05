/*************************************************************************
 *
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim
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

#include "mainwindow.h"

// ****  recent files
void MainWindow::rf_CreateMenus()
{
   int cnt = m_rf_List.count();

   QString tName;

   QMenu   *fileMenu = m_ui->menuFile;
   QAction *action   = fileMenu->insertSeparator(m_ui->actionExit);

   for (int i = 0; i < RECENT_FILES_MAX; ++i) {

      if (i < cnt)  {
         tName = m_rf_List[i];
      } else {
         tName = "file"+QString::number(i);
      }

      rf_Actions[i] = new QAction(tName, this);
      rf_Actions[i]->setData(QString("recent-file"));

      fileMenu->insertAction(action, rf_Actions[i]);

      if (i >= cnt)  {
         rf_Actions[i]->setVisible(false);
      }

      connect(rf_Actions[i], SIGNAL(triggered()), this, SLOT(rf_Open()));
   }
}

void MainWindow::rf_Open()
{
   QAction *action;
   action = (QAction *)sender();

   if (action) {
      bool ok = openDoxy_Internal(action->text());

      if (! ok) {
         // remove file from list since it did not load
         int index = m_rf_List.indexOf(action->text());

         if (index >= 0) {
            m_rf_List.removeAt(index);

            // save new list of files
            json_Write(RECENTFILE);

            // update actions
            rf_UpdateActions();
         }
      }
   }
}

void MainWindow::showContext_Files(const QPoint &pt)
{
   QAction *action = m_ui->menuFile->actionAt(pt);

   if (action)  {
      QString data = action->data().toString();

      if (data == "recent-file")  {
         QString fName = action->text();

         QMenu *menu = new QMenu(this);
         menu->addAction("Clear Recent file list", this, SLOT(rf_ClearList()) );

         QAction *rfAction = menu->addAction("Remove file:  " + fName, this, SLOT(rf_RemoveFName() ));
         rfAction->setData(fName);

         menu->exec(m_ui->menuFile->mapToGlobal(pt));
         delete menu;
      }
   }
}

void MainWindow::rf_ClearList()
{
   QAction *action;
   action = (QAction *)sender();

   if (action) {
      m_rf_List.clear();

      // save new list of files
      json_Write(RECENTFILE);

      // update actions
      rf_UpdateActions();
   }
}

void MainWindow::rf_RemoveFName()
{
   QAction *action;
   action = (QAction *)sender();

   if (action) {
      QString fName = action->data().toString();

      int index = m_rf_List.indexOf(fName);

      if (index >= 0) {
         m_rf_List.removeAt(index);

         // save new list of files
         json_Write(RECENTFILE);

         // update actions
         rf_UpdateActions();
      }
   }
}

void MainWindow::rf_DeleteName()
{
   QAction *action;
   action = (QAction *)sender();

   if (action) {
      csMsg( "File name " + action->whatsThis() );
   }
}

void MainWindow::rf_Update()
{
   int cnt = m_rf_List.count();

   if (cnt >= RECENT_FILES_MAX) {
      m_rf_List.removeFirst();
   }

   m_rf_List.append(m_curFile);

   // save new list of files
   json_Write(RECENTFILE);

   // update actions
   rf_UpdateActions();
}

void MainWindow::rf_UpdateActions()
{
   int cnt = m_rf_List.count();

   for (int i = 0; i < RECENT_FILES_MAX; ++i) {

     if (i < cnt)  {
        rf_Actions[i]->setText(m_rf_List[i]);
        rf_Actions[i]->setVisible(true);

     } else {
        rf_Actions[i]->setVisible(false);
     }

   }
}

