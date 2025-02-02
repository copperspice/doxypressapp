/************************************************************************
*
* Copyright (c) 2014-2025 Barbara Geller & Ansel Sermersheim
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

#ifndef DIALOG_LOOKUP_H
#define DIALOG_LOOKUP_H

#include "mainwindow.h"
#include "ui_dialog_lookup.h"

#include <QDialog>
#include <QStandardItemModel>

class Dialog_LookUp : public QDialog
{
   CS_OBJECT(Dialog_LookUp)

 public:
   explicit Dialog_LookUp(MainWindow *parent, struct LookUpInfo lookUpInfo);
   ~Dialog_LookUp();

   QStringList getData();
   QSize sizeHint() const override;

 private:
   MainWindow *m_owner;

   Ui::Dialog_LookUp *m_ui;
   QStandardItemModel *m_model;
   struct LookUpInfo m_lookUpInfo;

   void moveItemUp();
   void moveItemDown();
   void getFile();
   void getFolder();
   void addItem();
   void deleteItem();
   void save();
   void cancel();
};

#endif
