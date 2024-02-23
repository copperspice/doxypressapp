/************************************************************************
*
* Copyright (c) 2014-2024 Barbara Geller & Ansel Sermersheim
* Copyright (c) 1997-2014 by Dimitri van Heesch
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

#ifndef DIALOG_SELECTCFG_H
#define DIALOG_SELECTCFG_H

#include "mainwindow.h"
#include "ui_dialog_selectcfg.h"

#include <QDialog>

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

   void sysDefault();
   void pick();
   void existing();
   void cancel();
};

#endif
