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

   private:
      Ui::Dialog_LookUp *m_ui;
      QStandardItemModel *m_model;
      struct LookUpInfo m_data;

      CS_SLOT_1(Private, void moveItemUp())
      CS_SLOT_2(moveItemUp)

      CS_SLOT_1(Private, void moveItemDown())
      CS_SLOT_2(moveItemDown)

      CS_SLOT_1(Private, void addItem())
      CS_SLOT_2(addItem)

      CS_SLOT_1(Private, void deleteItem())
      CS_SLOT_2(deleteItem)

      CS_SLOT_1(Private, void save())
      CS_SLOT_2(save)

      CS_SLOT_1(Private, void cancel())
      CS_SLOT_2(cancel)
};

#endif
