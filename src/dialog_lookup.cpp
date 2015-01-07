/*************************************************************************
 *
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#include <dialog_lookup.h>

Dialog_LookUp::Dialog_LookUp(MainWindow *parent, QStringList data)
   :  QDialog(parent), m_ui(new Ui::Dialog_LookUp)
{
   m_ui->setupUi(this);
   m_data = data;

// setWindowTitle("Select Entries");
// m_ui->topMsg->setText("something");

   m_model = new QStandardItemModel();

   m_ui->tableView->setModel(m_model);  
   m_ui->tableView->resizeColumnsToContents();

   QHeaderView *temp = m_ui->tableView->horizontalHeader();
   temp->setHidden(true);
   temp->setStretchLastSection(true);

   // existing data
   for (int row = 0; row < 4; ++row) {
      QStandardItem *item = new QStandardItem( QString("information in row %0").arg(row) );
      m_model->appendRow(item);
   }

// connect(m_ui->delete_PB,       SIGNAL( clicked()),this,SLOT(deleteItem()));
// connect(m_ui->up_PB,           SIGNAL( clicked()),this,SLOT(moveItemUp()));
// connect(m_ui->down_PB,         SIGNAL( clicked()),this,SLOT(moveItemDown()));

   connect(m_ui->add_PB,          SIGNAL( clicked()),this,SLOT(add()));
   connect(m_ui->save_PB,         SIGNAL( clicked()),this,SLOT(save()));
   connect(m_ui->cancel_PB,       SIGNAL( clicked()),this,SLOT(cancel()));

};

Dialog_LookUp::~Dialog_LookUp()
{
   delete m_ui;
}

void Dialog_LookUp::add()
{  
   QStandardItem *item = new QStandardItem("");
   m_model->appendRow(item);
}

void Dialog_LookUp::save()
{
   this->done(QDialog::Accepted);
}

void Dialog_LookUp::cancel()
{ 
   this->done(QDialog::Rejected);
}

QStringList Dialog_LookUp::getData()
{
   QStringList retval;

   int max = m_model->rowCount();

   for (int row = 0; row < max; ++row) {
      QStandardItem *item = m_model->item(row,0);
      retval.append(item->text());
   }

   return retval;
}
