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

#ifndef DIALOG_ARGS_H
#define DIALOG_ARGS_H

#include <QDialog>
#include <QString>

#include "mainwindow.h"
#include "ui_dialog_args.h"

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
