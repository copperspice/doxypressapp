/************************************************************************
*
* Copyright (c) 2014-2026 Barbara Geller & Ansel Sermersheim
*
* Copyright (c) 1997-2014 by Dimitri van Heesch
*
* DoxyPressApp is free software. You can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
* DoxyPressApp is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* Documents produced by DoxyPress are derivative works derived from the
* input used in their production; they are not affected by this license.
*
* https://www.gnu.org/licenses/
*
*************************************************************************/

#include "dialog_xp_getdir.h"
#include "mainwindow.h"
#include "util.h"

#include <QAbstractItemView>
#include <QDir>
#include <QFileInfoList>

#ifdef Q_OS_WIN
#include <windows.h>
#include <lm.h>
#endif

Dialog_XP_GetDir::Dialog_XP_GetDir(MainWindow *from, const QString title, const QString absolutePath,
      QFileDialog::FileDialogOptions options)
   : QDialog(from), m_ui(new Ui::Dialog_XP_GetDir)
{
   (void) options;

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

   QString driveLetter = absolutePath;

   if (driveLetter.isEmpty())  {
      driveLetter = QCoreApplication::applicationDirPath();
   }

   driveLetter = driveLetter.left(3);

   // set up drive tree view - QList<QFileInfo>
   m_ui->drives_TV->setHeaderLabels(QStringList() << "Drive"  << "Drive Type");
   m_ui->drives_TV->setColumnCount(2);
   m_ui->drives_TV->setColumnWidth(15, 50);

   QString path;
   QTreeWidgetItem *treeItem;

   for (auto item : driveList) {
      path = item.path();

      if (path.endsWith("/")) {
         path = path.left(2);
      }

      QString other = driveType(path);

      treeItem = new QTreeWidgetItem(m_ui->drives_TV);
      treeItem->setText(0, path);
      treeItem->setText(1, other);

      if (driveLetter.startsWith(path, Qt::CaseInsensitive)) {
         m_ui->drives_TV->setCurrentItem(treeItem);
      }
   }

   // set up tree view right
   m_model_R = new QFileSystemModel;
   m_model_R->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
   m_model_R->setRootPath(driveLetter);

   // reset in case there is a backslash issue
   driveLetter = m_model_R->rootPath();

   m_ui->folders_TV->setModel(m_model_R);
   m_ui->folders_TV->setHeaderHidden(true);

   QModelIndex index = m_model_R->index(driveLetter);
   m_ui->folders_TV->setRootIndex(index);

   for (int nCount = 1; nCount < m_model_R->columnCount(); nCount++) {
      m_ui->folders_TV->hideColumn(nCount);
   }

   // search for the current path
   m_index_R = m_model_R->index(absolutePath);
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
   driveType = GetDriveTypeW(drive.toStdWString().c_str());
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
   QString networkName;
   QTreeWidgetItem *treeItem;

   // add network servers to left treeview
   m_netServers = getWin_NetServers();

   for (auto item : m_netServers) {
      networkName = item.serverName;

      QString other = "Network Share";

      treeItem = new QTreeWidgetItem(m_ui->drives_TV);
      treeItem->setText(0, networkName);
      treeItem->setText(1, other);
   }

   m_ui->network_PB->setDisabled(true);
}

void Dialog_XP_GetDir::showDirectories(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
   (void) previous;

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

void Dialog_XP_GetDir::showMe()
{
   // needs additional functionality in CS
   m_ui->folders_TV->scrollTo(m_index_R, QAbstractItemView::PositionAtCenter);
}

QSize Dialog_XP_GetDir::sizeHint() const
{
   return QSize(625, 600);
}

QList<netServers> Dialog_XP_GetDir::getWin_NetServers()
{
   QList<netServers> retval;

#ifdef Q_OS_WIN
   // local comptuer
   QString nameLocal;

   wchar_t nameBuffer[4096];
   long unsigned int  nameSize = 4096;

   if (GetComputerNameW(nameBuffer, &nameSize)) {
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

         for (uint32_t i = 0; i < entriesRead; i++)  {
            QString temp = QString::fromStdWString(buffer[i].sv100_name);

            if (temp != nameLocal ) {
               retval.append( netServers{temp, true} );
            }
         }
      }
   }
#endif

   return retval;
}
