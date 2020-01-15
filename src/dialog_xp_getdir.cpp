/************************************************************************
*
* Copyright (C) 2014-2020 Barbara Geller & Ansel Sermersheim
* Copyright (C) 1997-2014 by Dimitri van Heesch
*
* DoxyPressApp is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* DoxyPressApp is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* Documents produced by DoxyPress are derivative works derived from the
* input used in their production; they are not affected by this license.
*
*************************************************************************/

#include <QAbstractItemView>
#include <QDir>
#include <QFileInfoList>

#include "dialog_xp_getdir.h"
#include "mainwindow.h"
#include "util.h"

#ifdef Q_OS_WIN
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <lm.h>
#endif

Dialog_XP_GetDir::Dialog_XP_GetDir(MainWindow *from, const QString title, const QString path, QFileDialog::Options options)
   : QDialog(from), m_ui(new Ui::Dialog_XP_GetDir)
{
   // the value for path is an absolute path
   m_path = path;

   m_ui->setupUi(this);
   setWindowTitle(title);

   // set the width of the panes
   QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);

   sizePolicy1.setHorizontalStretch(4);
   sizePolicy1.setVerticalStretch(0);
   sizePolicy1.setHeightForWidth(m_ui->drives_TV->sizePolicy().hasHeightForWidth());
   m_ui->drives_TV->setSizePolicy(sizePolicy1);

   QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);

   sizePolicy2.setHorizontalStretch(5);
   sizePolicy2.setVerticalStretch(0);
   sizePolicy2.setHeightForWidth(m_ui->folders_TV->sizePolicy().hasHeightForWidth());
   m_ui->folders_TV->setSizePolicy(sizePolicy2);

   // get drive list and starting drive
   QFileInfoList driveList = QDir::drives();

   QString drive_L = m_path;
   if (drive_L.isEmpty())  {
      drive_L = QCoreApplication::applicationDirPath();
   }

   drive_L = drive_L.left(3);

   // set up drive tree view - QList<QFileInfo>
   m_ui->drives_TV->setHeaderLabels(QStringList() << "Drive"  << "Drive Type");
   m_ui->drives_TV->setColumnCount(2);
   m_ui->drives_TV->setColumnWidth(15, 50);

   QString data;
   QTreeWidgetItem *item;

   for (auto k = driveList.begin(); k != driveList.end(); ++k) {
      data = k->path();

      if (data.endsWith("/")) {
         data = data.left(2);
      }

      //
      QString other = driveType(data);

      item = new QTreeWidgetItem(m_ui->drives_TV);
      item->setText(0, data);
      item->setText(1, other);

      if (drive_L.startsWith(data, Qt::CaseInsensitive)) {
         m_ui->drives_TV->setCurrentItem(item);
      }
   }

   // set up tree view right
   m_model_R = new QFileSystemModel;
   m_model_R->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
   m_model_R->setRootPath(drive_L);

   // reset in case there is a backslash issue
   drive_L = m_model_R->rootPath();

   m_ui->folders_TV->setModel(m_model_R);
   m_ui->folders_TV->setHeaderHidden(true);

   QModelIndex index = m_model_R->index(drive_L);
   m_ui->folders_TV->setRootIndex(index);

   for(int nCount = 1; nCount < m_model_R->columnCount(); nCount++) {
      m_ui->folders_TV->hideColumn(nCount);
   }

   // search for the current path
   m_index_R = m_model_R->index(m_path);
   m_ui->folders_TV->setCurrentIndex(m_index_R);

   // tree view triggers
   connect(m_ui->drives_TV,  &QTreeWidget::currentItemChanged, this, &Dialog_XP_GetDir::showDirectories);

   connect(m_ui->network_PB, &QPushButton::clicked, this, &Dialog_XP_GetDir::network);
   connect(m_ui->ok_PB,      &QPushButton::clicked, this, &Dialog_XP_GetDir::ok);
   connect(m_ui->cancel_PB,  &QPushButton::clicked, this, &Dialog_XP_GetDir::cancel);

   // update right treeview
   connect(m_model_R, &QFileSystemModel::directoryLoaded, this, &Dialog_XP_GetDir::showMe);

   // force call to sizeHint()
   adjustSize();

   m_ui->drives_TV->setFocus();
}

Dialog_XP_GetDir::~Dialog_XP_GetDir()
{
   delete m_ui;
}

void Dialog_XP_GetDir::cancel()
{
   this->done(QDialog::Rejected);
}

QString Dialog_XP_GetDir::driveType(QString drive)
{
   QString retval;

   int driveType = 0;
   drive = drive + "\\";

#ifdef Q_OS_WIN
   // windows api call, pass drive value as UNICODE
   driveType = GetDriveType(drive.toStdWString().c_str());
#endif

   switch (driveType) {

      case 0:
      case 1:
         retval = "Unknown";
         break;

      case 2:
         retval = "Removable Drive";
         break;

      case 3:
         retval = "Local Drive";
         break;

      case 4:
         retval = "Network Drive";
         break;

      case 5:
         retval = "CD Drive";
         break;

      case 6:
         retval = "Ram Disk";
         break;
   }

   return retval;
}

QString Dialog_XP_GetDir::getDirectory()
{
   QModelIndex index = m_ui->folders_TV->currentIndex();
   QString retval = m_model_R->filePath(index);

   return retval;
}

void Dialog_XP_GetDir::ok()
{
   this->done(QDialog::Accepted);
}

void Dialog_XP_GetDir::network()
{
   QString data;
   QTreeWidgetItem *item;

   // add network servers to left treeview
   m_netServers = getWin_NetServers();

// QFileSystemModel finds the netShares, this method is not required
// m_netShares  = getWin_NetShares();

   for (auto k = m_netServers.begin(); k != m_netServers.end(); ++k) {
      data = k->serverName;

      QString other = "Network Share";

/*    if (k->isAvailable) {
         other = "Network Share";
      } else  {
         other = "UnAvailable";
      }
*/
      item = new QTreeWidgetItem(m_ui->drives_TV);
      item->setText(0, data);
      item->setText(1, other);
   }

   m_ui->network_PB->setDisabled(true);
}

void Dialog_XP_GetDir::showDirectories(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
   if (! current) {
      return;
   }

   QString drive;

   if (current->text(1) == "Network Share")  {

      drive = "//" + current->text(0).toLower() + "/";

      // 1
      m_model_R = new QFileSystemModel;
      m_model_R->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
      m_model_R->setRootPath(drive);

      // will hide printer folders
      QStringList filters;
      filters << "*[^$]";

      m_model_R->setNameFilters(filters);
      m_model_R->setNameFilterDisables(false);

   } else if (current->text(1) == "UnAvailable")  {
      // show an empty list

      drive = "//" + current->text(0).toLower() + "/";

      // 1
      m_model_R = new QFileSystemModel;
      m_model_R->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
      m_model_R->setRootPath(drive);

      // hide everthing for now
      QStringList filters;
      filters << "(?!E)E";

      m_model_R->setNameFilters(filters);
      m_model_R->setNameFilterDisables(false);

   } else {
      drive = current->text(0) + "/";

      // 1
      m_model_R = new QFileSystemModel;
      m_model_R->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
      m_model_R->setRootPath(drive);
   }

   // 2
   m_ui->folders_TV->setModel(m_model_R);

   // 3
   QModelIndex index = m_model_R->index(drive);
   m_ui->folders_TV->setRootIndex(index);
}

void Dialog_XP_GetDir::showMe(const QString &path)
{
   //  m_index_R = m_model_R->index(m_path);
   //  m_ui->folders_TV->setCurrentIndex(m_index_R);

   // needs additional functionality in CS
   m_ui->folders_TV->scrollTo(m_index_R, QAbstractItemView::PositionAtCenter);
}

QSize Dialog_XP_GetDir::sizeHint() const
{
   return QSize(625,600);
}

QList<netServers> Dialog_XP_GetDir::getWin_NetServers()
{
   QList<netServers> retval;

#ifdef Q_OS_WIN
   // local comptuer
   QString nameLocal;

   wchar_t nameBuffer[4096];
   long unsigned int  nameSize = 4096;

   if (GetComputerName(nameBuffer, &nameSize)) {
      nameLocal = QString::fromStdWString(nameBuffer);
   }

   // struct SERVER_INFO_100 declared in lm.h
   using Server100 = SERVER_INFO_100;

   Server100 *buffer;

   wchar_t *serverName           = NULL;
   long unsigned int maxLen      = MAX_PREFERRED_LENGTH;
   long unsigned int entriesRead = 0;
   long unsigned int count       = 0;
   long unsigned int serverType  = SV_TYPE_SERVER;        // all servers
   wchar_t *domainName           = NULL;
   long unsigned int resume      = 0;

   NET_API_STATUS nStatus;

   nStatus = NetServerEnum(serverName, 100, (LPBYTE *) &buffer, maxLen, &entriesRead, &count, serverType, domainName, &resume);

   if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {

      if (buffer != NULL) {

         for (uint32_t k = 0; k < entriesRead; k++)  {
            QString temp = QString::fromStdWString(buffer[k].sv100_name);

            if (temp != nameLocal ) {
               retval.append( netServers{temp, true} );
            }
         }
      }
   }

#endif

   return retval;
}
