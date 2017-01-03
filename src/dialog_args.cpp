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

#include <QDateTimeEdit>

#include "dialog_args.h"
#include "util.h"

const QString dateTimeFormat = "yyyy/MM/dd HH:mm:ss";

Dialog_Args::Dialog_Args(MainWindow *m_owner, struct Args argsX)
   : m_ui(new Ui::Dialog_Args)
{
   m_ui->setupUi(this);
   this->setWindowIcon(QIcon("://resources/doxypress.png"));

   m_args = argsX;

   m_ui->set_dateTime->setChecked(m_args.setDateTime);

   if (m_args.dateTime.isEmpty()) {
      QDateTime dt = QDateTime::currentDateTime();     
      m_ui->dateTime->setDateTime(dt);

   } else {
      m_ui->dateTime->setDateTime(QDateTime::fromString(m_args.dateTime, dateTimeFormat) );
   } 

   m_ui->deleteHtml->setChecked(m_args.deleteHtml);

   m_ui->generate_layout->setChecked(m_args.blank_layout);

   if (! m_args.layout.isEmpty()) {
      m_ui->layout_fName->setText(m_args.layout);
   }

   m_ui->html_header->setText(m_args.style_html_header);
   m_ui->html_footer->setText(m_args.style_html_footer);
   m_ui->html_css->setText(m_args.style_html_css);

   connect(m_ui->ok_PB,     &QPushButton::clicked, this, &Dialog_Args::ok);
   connect(m_ui->cancel_PB, &QPushButton::clicked, this, &Dialog_Args::cancel);
}

Dialog_Args::~Dialog_Args()
{
   delete m_ui;
}

void Dialog_Args::ok()
{
   // update m_args
   m_args.setDateTime = m_ui->set_dateTime->isChecked();
   QDateTime dt       = m_ui->dateTime->dateTime();
   m_args.dateTime    = dt.toString(dateTimeFormat);

   m_args.deleteHtml  = m_ui->deleteHtml->isChecked();

   m_args.blank_layout = m_ui->generate_layout->isChecked();
   m_args.layout       = m_ui->layout_fName->text();

   m_args.style_html_header = m_ui->html_header->text();
   m_args.style_html_footer = m_ui->html_footer->text();
   m_args.style_html_css    = m_ui->html_css->text();

   // verify
   if (m_args.blank_layout && m_args.layout.isEmpty())    {
      csError("Passed Parameters", "No layout file name was provided");

   } else {
      this->done(1);
   }
}

void Dialog_Args::cancel()
{
   this->done(0);
}

struct Args Dialog_Args::get_Args()
{
   return m_args;
}

