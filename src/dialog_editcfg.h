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

#ifndef DIALOG_EDITCFG_H
#define DIALOG_EDITCFG_H

#include <QDialog>

#include "mainwindow.h"
#include "ui_dialog_editcfg.h"

class Dialog_EditCfg : public QDialog
{
   CS_OBJECT(Dialog_EditCfg)

   public:
      explicit Dialog_EditCfg(MainWindow *parent, QString fileName);
      ~Dialog_EditCfg();

      QString get_doxyPressFn();

   private:
      MainWindow *m_owner;
      Ui::Dialog_EditCfg *m_ui;

      CS_SLOT_1(Private, void fileLookUp())
      CS_SLOT_2(fileLookUp)

      CS_SLOT_1(Private, void save())
      CS_SLOT_2(save)

      CS_SLOT_1(Private, void cancel())
      CS_SLOT_2(cancel)

};

#endif
