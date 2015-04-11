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
 * Documents produced by DoxyPress are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#include <dialog_lookup.h>

Dialog_LookUp::Dialog_LookUp(MainWindow *parent, struct LookUpInfo data)
   :  QDialog(parent), m_ui(new Ui::Dialog_LookUp)
{
   m_data  = data;
   m_owner = parent;

   m_ui->setupUi(this);
   setWindowTitle(m_data.title);

   m_model = new QStandardItemModel();

   m_ui->tableView->setModel(m_model);  
   m_ui->tableView->resizeColumnsToContents();

   QHeaderView *temp = m_ui->tableView->horizontalHeader();
   temp->setHidden(true);
   temp->setStretchLastSection(true);

   // existing data   
   for (auto s : m_data.dataList) {
      QStandardItem *item = new QStandardItem(s);
      m_model->appendRow(item);
   }

   // select first row
   QModelIndex index = m_model->index(0, 0);
   m_ui->tableView->setCurrentIndex(index);

   if (! data.isFilePB) {
      m_ui->files_PB->setEnabled(false);
   }

   if (! data.isFolderPB) {
      m_ui->folders_PB->setEnabled(false);
   }

   connect(m_ui->up_PB,           &QPushButton::clicked, this, [this](){ moveItemUp(); } );
   connect(m_ui->down_PB,         &QPushButton::clicked, this, [this](){ moveItemDown(); } );

   connect(m_ui->files_PB,        &QPushButton::clicked, this, [this](){ getFile(); } );
   connect(m_ui->folders_PB,      &QPushButton::clicked, this, [this](){ getFolder(); } );

   connect(m_ui->add_PB,          &QPushButton::clicked, this, [this](){ addItem(); } );
   connect(m_ui->delete_PB,       &QPushButton::clicked, this, [this](){ deleteItem(); } );
   connect(m_ui->save_PB,         &QPushButton::clicked, this, [this](){ save(); } );
   connect(m_ui->cancel_PB,       &QPushButton::clicked, this, [this](){ cancel(); } );

   // force call to sizeHint()
   adjustSize();
}

Dialog_LookUp::~Dialog_LookUp()
{
   delete m_ui;
}

void Dialog_LookUp::addItem()
{  
   QStandardItem *item = new QStandardItem("");
   m_model->appendRow(item);

   // add new row
   int row = m_model->rowCount() - 1;

   // select new row
   QModelIndex index = m_model->index(row, 0);
   m_ui->tableView->setCurrentIndex(index);
}

void Dialog_LookUp::deleteItem()
{
   QModelIndex index = m_ui->tableView->currentIndex();
   if (! index.isValid() ) {
      return;
   }

   int row = index.row();

   // change the row id to zero before the delete
   QModelIndex indexDel = m_model->index(row, 0);
   m_model->setData(indexDel, QVariant(0) );

   // hide current row from the view
   m_ui->tableView->setRowHidden(row, true);

   m_model->removeRow(row);    

   //
   int maxRow = m_model->rowCount();
   ++row;

   if (row < maxRow )  {    
      // move to next row
      index = m_model->index(row, 0);

   } else  {
      // move to row 0      
      index = m_model->index(0, 0);
   }

   m_ui->tableView->setCurrentIndex(index);   
}

void Dialog_LookUp::getFile()
{
   QModelIndex index = m_ui->tableView->currentIndex();
   if (! index.isValid() ) {
      return;
   }

   QString file = m_model->data(index).toString();
   file = m_owner->getSingleFile(tr("Select file"), file);

   // save new data
   m_model->setData(index, QVariant(file));
}

void Dialog_LookUp::getFolder()
{
   QModelIndex index = m_ui->tableView->currentIndex();
   if (! index.isValid() ) {
      return;
   }

   QString path    = m_model->data(index).toString();
   QString relPath = "marker";  // BROOM - resolve

   path = m_owner->get_DirPath(tr("Select destination directory"), path, relPath);

   // save new data
   m_model->setData(index, QVariant(path));
}

void Dialog_LookUp::moveItemUp()
{
   QModelIndex index = m_ui->tableView->currentIndex();
   if (! index.isValid() )
      return;

   // current row user wants to move up
   int rowUp = index.row();

   if (rowUp == 0) {
      // Current row can not be moved up
      return;
   }

   // calculate the prior visibale row
   int rowAbove = rowUp - 1;

   while (true) {

      if (m_ui->tableView->isRowHidden(rowAbove)) {

         if (rowAbove == 0) {
            // Top row can not be moved up -- nothing visable above current row
            return;
         }

         // decrement and try again
         rowAbove--;

      } else {
         // have a good value
         break;
      }
   }

   // swap the data for column 0
   setUpdatesEnabled(false);

   {
      // get data for rowUp
      QModelIndex indexUp = m_model->index(rowUp, 0);
      QVariant dataUp = m_model->data(indexUp, Qt::DisplayRole);

      // get data for rowAbove
      QModelIndex indexAbove = m_model->index(rowAbove, 0);
      QVariant dataAbove = m_model->data(indexAbove,Qt::DisplayRole);

      // save
      m_model->setData(indexAbove, QVariant(dataAbove), Qt::EditRole );
      m_model->setData(indexUp, QVariant(dataUp), Qt::EditRole );

      // stay on the row that was moved up
      m_ui->tableView->setCurrentIndex(indexAbove);
   }

   setUpdatesEnabled(true);
}

void Dialog_LookUp::moveItemDown()
{
   QModelIndex index = m_ui->tableView->currentIndex();
   if (! index.isValid() ) {
      return;
   }

   // current row user wants to move down
   int rowDown = index.row();

   int lastRow = m_model->rowCount() - 1;

   if (rowDown == lastRow) {
      // Current row can not be moved down
      return;
   }

   // calculate the next visibale row
   int rowBelow = rowDown + 1;

   while (true) {

      if (m_ui->tableView->isRowHidden(rowBelow)) {

         if (rowDown == lastRow) {
            // Bottom row can not be moved down -- nothing visable below current row
            return;
         }

         // decrement and try again
         rowBelow++;

      } else {
         // have a good value
         break;
      }
   }

   // swap the data for column 0
   setUpdatesEnabled(false);

   {
      // 2 get data for rowDown
      QModelIndex indexDown = m_model->index(rowDown, 0);
      QVariant dataDown = m_model->data(indexDown, Qt::DisplayRole);

      // get data for rowBelow
      QModelIndex indexBelow = m_model->index(rowBelow, 0);
      QVariant dataBelow = m_model->data(indexBelow,Qt::DisplayRole);

      // save
      m_model->setData(indexBelow, QVariant(dataDown), Qt::EditRole );
      m_model->setData(indexDown, QVariant(dataBelow), Qt::EditRole );

      // stay on the row that was moved up
      m_ui->tableView->setCurrentIndex(indexBelow);
   }

   setUpdatesEnabled(true);
}

void Dialog_LookUp::save()
{
   this->done(QDialog::Accepted);
}

QSize Dialog_LookUp::sizeHint() const
{
   return QSize(616, 600);
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

      if (! item->text().isEmpty()) {
         retval.append(item->text());
      }
   }

   return retval;
}
