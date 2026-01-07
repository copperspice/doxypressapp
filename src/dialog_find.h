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

#ifndef DIALOG_FIND_H
#define DIALOG_FIND_H

#include "ui_dialog_find.h"
#include "mainwindow.h"

#include <QDialog>
#include <QString>

class Dialog_Find : public QDialog
{
   CS_OBJECT(Dialog_Find)

 public:
   Dialog_Find(MainWindow *parent, QString text);
   ~Dialog_Find();

   QString get_findText();
   bool get_Direction();
   bool get_Case();
   bool get_SearchTop();

 private:
   Ui::Dialog_Find *m_ui;
   bool m_upd_Find;

   void find();
   void cancel();
};

#endif
