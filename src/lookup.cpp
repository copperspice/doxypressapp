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

#include "dialog_lookup.h"
#include "mainwindow.h"

// tab 1
void MainWindow::icon_PB(const QString route)
{
   QString iconName;

   if (route == "load") {
      iconName = m_project_iconFN;

   } else {
      QString path = pathName(m_ConfigFile);
      iconName = QFileDialog::getOpenFileName(this, tr("Select Project Icon"), path);
   }

   if (iconName.isEmpty()) {
      m_ui->project_icon->setText(tr("No Logo was selected"));

   } else {
      QFile fout(iconName);

      if (! fout.exists()) {
         m_ui->project_icon->setText(tr("Unable to find file: ") + iconName);

      } else {
         QPixmap pm(iconName);

         if (! pm.isNull()) {
            m_ui->project_icon->setPixmap(pm.scaledToHeight(55, Qt::SmoothTransformation));

         } else {
            m_ui->project_icon->setText(tr("No preview is available for: ") + iconName);
         }
      }

      m_project_iconFN = iconName;
   }

}

void MainWindow::output_PB()
{
   QString path    = pathName(m_ConfigFile);
   QString dirName = QFileDialog::getExistingDirectory(this, tr("Select destination directory"), path);
   QDir dir(path);

   if (! m_ConfigFile.isEmpty() && dir.exists()) {
      dirName = dir.relativeFilePath(dirName);
   }

   if (dirName.isEmpty()) {
      dirName = QString::fromAscii(".");
   }

   m_ui->source_output->setText(dirName);
}

// tab 2
void MainWindow::source_input_PB()
{
   QStringList dataList;

   Dialog_LookUp *dw = new Dialog_LookUp(this, dataList);
   int result = dw->exec();

   if (result == QDialog::Accepted) {

      dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->source_input->setPlainText(temp);

/*
      if (! m_ConfigFile.isEmpty() && dir.exists()) {
         dirName = dir.relativeFilePath(dirName);
      }

      if (dirName.isEmpty()) {
         dirName = QString::fromAscii(".");
      }

      m_ui->source_input->setText(dirName);
*/

   }
}
