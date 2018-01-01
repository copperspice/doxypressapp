/*************************************************************************
 *
 * Copyright (C) 2014-2018 Barbara Geller & Ansel Sermersheim
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
