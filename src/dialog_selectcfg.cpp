/*************************************************************************
 *
 * Copyright (C) 2014-2018 Barbara Geller & Ansel Sermersheim
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

#include "dialog_selectcfg.h"

Dialog_SelectCfg::Dialog_SelectCfg(MainWindow *parent)
   : QDialog(parent), m_ui(new Ui::Dialog_SelectCfg)
{
   m_ui->setupUi(this);

   connect(m_ui->sysDefault_PB, &QPushButton::clicked, this, &Dialog_SelectCfg::sysDefault);
   connect(m_ui->pick_PB,       &QPushButton::clicked, this, &Dialog_SelectCfg::pick);
   connect(m_ui->existing_PB,   &QPushButton::clicked, this, &Dialog_SelectCfg::existing);
   connect(m_ui->cancel_PB,     &QPushButton::clicked, this, &Dialog_SelectCfg::cancel);
}

Dialog_SelectCfg::~Dialog_SelectCfg()
{
   delete m_ui;
}

void Dialog_SelectCfg::sysDefault()
{
   this->done(Result::SysDefault);
}

void Dialog_SelectCfg::pick()
{
   this->done(Result::Pick);
}

void Dialog_SelectCfg::existing()
{
   this->done(Result::Existing);
}

void Dialog_SelectCfg::cancel()
{
   this->done(Result::Cancel);
}
