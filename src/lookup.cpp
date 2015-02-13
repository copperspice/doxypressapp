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

#include "colors.h"
#include "dialog_lookup.h"
#include "mainwindow.h"

#include <QRegExp>

// tab 1
void MainWindow::getIcon(const QString route)
{
   QString iconName;

   if (route == "load") {
      iconName = m_project_iconFN;

   } else {
      QString path = pathName(m_curFile);
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

void MainWindow::output_dir_PB()
{
   QString path    = pathName(m_curFile);
   QString dirName = QFileDialog::getExistingDirectory(this, tr("Select destination directory"), path);
   QDir dir(path);

   if (! m_curFile.isEmpty() && dir.exists()) {
      dirName = dir.relativeFilePath(dirName);
   }

   if (dirName.isEmpty()) {
      dirName = QString::fromAscii(".");
   }

   m_ui->output_dir->setText(dirName);
}

void MainWindow::tune_colors_PB()
{
   int hue   = m_ui->html_colorstyle_hue->value();
   int sat   = m_ui->html_colorstyle_sat->value();
   int gamma = m_ui->html_colorstyle_gamma->value();

   TuneColorDialog tuneColor(hue, sat, gamma, this);

   if (tuneColor.exec() == QDialog::Accepted) {
      m_ui->html_colorstyle_hue->setValue( tuneColor.getHue());
      m_ui->html_colorstyle_sat->setValue( tuneColor.getSaturation());
      m_ui->html_colorstyle_gamma->setValue(tuneColor.getGamma());
   }
}

// tab 2
void MainWindow::input_source_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->input_source->toPlainText();

   data.title      = "Input Source";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->input_source->setPlainText(temp);
   }
}

void MainWindow::file_patterns_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->file_patterns->toPlainText();

   data.title      = "File Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {

      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->file_patterns->setPlainText(temp);
   }
}

void MainWindow::exclude_files_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp    = m_ui->exclude_files->toPlainText();

   data.title      = "Exclude Files";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->exclude_files->setPlainText(temp);
   }
}

void MainWindow::exclude_patterns_PB()
{   
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp    = m_ui->exclude_patterns->toPlainText();

   data.title      = "Exclude Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->exclude_patterns->setPlainText(temp);
   }
}

void MainWindow::exclude_symbols_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp    = m_ui->exclude_symbols->toPlainText();

   data.title      = "Exclude Symbols";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->exclude_symbols->setPlainText(temp);
   }
}
