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

#ifndef DIALOG_SELECTCFG_H
#define DIALOG_SELECTCFG_H

#include <QDialog>

#include "mainwindow.h"
#include "ui_dialog_selectcfg.h"

class Dialog_SelectCfg : public QDialog
{
   CS_OBJECT(Dialog_SelectCfg)

   public:
      explicit Dialog_SelectCfg(MainWindow *parent);
      ~Dialog_SelectCfg();

      enum Result {
         SysDefault,
         Pick,
         Existing,
         Cancel
      };

   private:
      Ui::Dialog_SelectCfg *m_ui;

      CS_SLOT_1(Private, void sysDefault())
      CS_SLOT_2(sysDefault)

      CS_SLOT_1(Private, void pick())
      CS_SLOT_2(pick)

      CS_SLOT_1(Private, void existing())
      CS_SLOT_2(existing)

      CS_SLOT_1(Private, void cancel())
      CS_SLOT_2(cancel)
};

#endif
