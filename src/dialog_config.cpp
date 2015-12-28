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

#include "dialog_config.h"
#include "util.h"

Dialog_Config::Dialog_Config(QString fileName)
   : m_ui(new Ui::Dialog_Config)
{
   m_ui->setupUi(this);
   this->setWindowIcon(QIcon("://resources/doxypress.png"));

   m_ui->currentName->setText(fileName);
   m_ui->newName->setText(fileName);

   connect(m_ui->create_PB, SIGNAL(clicked()),this, SLOT(createNew()));
   connect(m_ui->select_PB, SIGNAL(clicked()),this, SLOT(select()));
   connect(m_ui->rename_PB, SIGNAL(clicked()),this, SLOT(rename()));
   connect(m_ui->cancel_PB, SIGNAL(clicked()),this, SLOT(cancel()));
}

Dialog_Config::~Dialog_Config()
{
   delete m_ui;
}

void Dialog_Config::createNew()
{
   this->done(1);
}

void Dialog_Config::select()
{
   this->done(2);
}

void Dialog_Config::rename()
{
   QString name = m_ui->newName->text();

   if (name.isEmpty())  {
      csError("Rename Configuration File", "New file name can not be blank.");
      return;
   }

   this->done(3);
}

void Dialog_Config::cancel()
{
   this->done(0);
}

QString Dialog_Config::get_newName()
{
   return m_ui->newName->text();
}

