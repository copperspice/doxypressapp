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

#ifndef DIALOG_LOOKUP_H
#define DIALOG_LOOKUP_H

#include <QDialog>
#include <QStandardItemModel>

#include "mainwindow.h"
#include "ui_dialog_lookup.h"

class Dialog_LookUp : public QDialog
{
   CS_OBJECT(Dialog_LookUp)

   public:
      explicit Dialog_LookUp(MainWindow *parent, struct LookUpInfo data);
      ~Dialog_LookUp();

      QStringList getData();
      QSize sizeHint() const;

   private:
      MainWindow *m_owner;

      Ui::Dialog_LookUp *m_ui;
      QStandardItemModel *m_model;
      struct LookUpInfo m_data;

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
