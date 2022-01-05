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

#include "dialog_config.h"
#include "util.h"

Dialog_Config::Dialog_Config(QString fileName)
   : m_ui(new Ui::Dialog_Config)
{
   m_ui->setupUi(this);
   this->setWindowIcon(QIcon("://resources/doxypress.png"));

   m_ui->currentName->setText(fileName);
   m_ui->newName->setText(fileName);

   connect(m_ui->create_PB, &QPushButton::clicked, this, &Dialog_Config::createNew);
   connect(m_ui->select_PB, &QPushButton::clicked, this, &Dialog_Config::select);
   connect(m_ui->rename_PB, &QPushButton::clicked, this, &Dialog_Config::rename);
   connect(m_ui->cancel_PB, &QPushButton::clicked, this, &Dialog_Config::cancel);
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

