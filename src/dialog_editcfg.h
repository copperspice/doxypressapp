/************************************************************************
*
* Copyright (c) 2014-2022 Barbara Geller & Ansel Sermersheim
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

      void fileLookUp();
      void save();
      void cancel();
};

#endif
