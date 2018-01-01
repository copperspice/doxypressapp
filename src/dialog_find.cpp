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

#include "dialog_find.h"
#include "util.h"

#include <QCompleter>
#include <QCursor>
#include <QLineEdit>
#include <QMenu>
#include <QTextCursor>

Dialog_Find::Dialog_Find(MainWindow *parent, QString findText)
   : QDialog(parent), m_ui(new Ui::Dialog_Find)
{
   m_upd_Find = false;

   m_ui->setupUi(this);   

   m_ui->down_RB->setChecked(true);
   m_ui->searchTop_CKB->setChecked(true);

   // display last edit value
   m_ui->find->setText(findText);
   m_ui->find->setFocus();

   // any key deletes first, right arrow to continue typing
   m_ui->find->selectAll();

   connect(m_ui->find_PB,   &QPushButton::clicked, this, &Dialog_Find::find);
   connect(m_ui->cancel_PB, &QPushButton::clicked, this, &Dialog_Find::cancel);
}

Dialog_Find::~Dialog_Find()
{
   delete m_ui;
}

void Dialog_Find::find()
{
   this->done(1);
}

void Dialog_Find::cancel()
{
   this->done(0);
}

QString Dialog_Find::get_findText()
{
   return m_ui->find->text();
}

bool Dialog_Find::get_Direction()
{
   if (m_ui->down_RB->isChecked()) {
      return true;
   } else {
      return false;
   }
}

bool Dialog_Find::get_Case()
{
   return m_ui->case_CKB->isChecked();
}

bool Dialog_Find::get_SearchTop()
{
   return m_ui->searchTop_CKB->isChecked();
}

