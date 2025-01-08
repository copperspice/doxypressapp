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

#ifndef DIALOG_ARGS_H
#define DIALOG_ARGS_H

#include "mainwindow.h"
#include "ui_dialog_args.h"

#include <QDialog>
#include <QString>

class Dialog_Args: public QDialog
{
   CS_OBJECT(Dialog_Args)

 public:
   Dialog_Args(MainWindow *parent, struct Args argsX);
   ~Dialog_Args();

   struct Args get_Args();

 private:
   MainWindow *m_owner;
   Ui::Dialog_Args *m_ui;

   struct Args m_args;

   void ok();
   void cancel();
};

#endif
