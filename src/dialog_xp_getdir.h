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

#ifndef DIALOG_XP_GETDIR_H
#define DIALOG_XP_GETDIR_H

#include <QFileDialog>
#include <QFileSystemModel>
#include <QTreeWidgetItem>

#include "ui_dialog_xp_getdir.h"
#include "mainwindow.h"

struct netServers {
   QString serverName;   
   bool isAvailable;
};

struct netShares {
   QString serverName;
   QString shareName;
};

class Dialog_XP_GetDir  : public QDialog
{
   CS_OBJECT(Dialog_XP_GetDir)

   public:
      Dialog_XP_GetDir(MainWindow *from, const QString title, const QString path, QFileDialog::Options options);
      ~Dialog_XP_GetDir();

       QString getDirectory();

       void showMe(const QString &);
       QSize sizeHint() const;

   private:
      Ui::Dialog_XP_GetDir *m_ui;      
      QFileSystemModel  *m_model_R;

      QString m_path;
      QModelIndex m_index_R;

      QList<netServers> m_netServers;
      QList<netShares> m_netShares;

      QString driveType(QString drive);

      QList<netServers> getWin_NetServers();

      void showDirectories(QTreeWidgetItem * current,QTreeWidgetItem * previous);
      void network();
      void ok();
      void cancel();
};

#endif
