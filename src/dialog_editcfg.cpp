/*************************************************************************
 *
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim
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

#include <dialog_editcfg.h>

Dialog_EditCfg::Dialog_EditCfg(MainWindow *parent, QString fileName)
   : QDialog(parent), m_ui(new Ui::Dialog_EditCfg)
{
   m_owner = parent;

   m_ui->setupUi(this);
   this->setWindowIcon(QIcon("://resources/doxypress.png"));

   m_ui->doxyPressFile->setText(fileName);
   m_ui->doxyPressFile->setFocus();

   connect(m_ui->doxyPressFile_PB, SIGNAL(clicked()), this, SLOT(fileLookUp()));
   connect(m_ui->save_PB,          SIGNAL(clicked()), this, SLOT(save()));
   connect(m_ui->cancel_PB,        SIGNAL(clicked()), this, SLOT(cancel()));
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
