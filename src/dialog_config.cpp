/**************************************************************************
*
* Copyright (c) 2012-2015 Barbara Geller
* All rights reserved.
*
* This file is part of Diamond Editor.
*
* Diamond Editor is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License version 3
* as published by the Free Software Foundation.
*
* Diamond Editor is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Diamond Editor.  If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#include "dialog_config.h"
#include "util.h"

Dialog_Config::Dialog_Config(QString fileName)
   : m_ui(new Ui::Dialog_Config)
{
   m_ui->setupUi(this);
   this->setWindowIcon(QIcon("://resources/diamond.png"));

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

