/************************************************************************
*
* Copyright (c) 2014-2021 Barbara Geller & Ansel Sermersheim
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

#include "dialog_editcfg.h"

Dialog_EditCfg::Dialog_EditCfg(MainWindow *parent, QString fileName)
   : QDialog(parent), m_ui(new Ui::Dialog_EditCfg)
{
   m_owner = parent;

   m_ui->setupUi(this);
   this->setWindowIcon(QIcon("://resources/doxypress.png"));

   m_ui->doxyPressFile->setText(fileName);
   m_ui->doxyPressFile->setFocus();

   connect(m_ui->doxyPressFile_PB, &QPushButton::clicked, this, &Dialog_EditCfg::fileLookUp);
   connect(m_ui->save_PB,          &QPushButton::clicked, this, &Dialog_EditCfg::save);
   connect(m_ui->cancel_PB,        &QPushButton::clicked, this, &Dialog_EditCfg::cancel);
}

Dialog_EditCfg::~Dialog_EditCfg()
{
   delete m_ui;
}

QString Dialog_EditCfg::get_doxyPressFn()
{
   return m_ui->doxyPressFile->text();
}

void Dialog_EditCfg::fileLookUp()
{
   QString file = m_ui->doxyPressFile->text();
   file = m_owner->getSingleFile(tr("Select file"), file);

   m_ui->doxyPressFile->setText(file);
}

void Dialog_EditCfg::cancel()
{
   this->done(QDialog::Rejected);
}

void Dialog_EditCfg::save()
{
   this->done(QDialog::Accepted);
}
