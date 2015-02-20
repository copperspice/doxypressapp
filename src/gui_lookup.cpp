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
   QString path    = m_ui->output_dir->text();
   QString relPath = pathName(m_curFile);

   path = get_DirPath(tr("Select destination directory"), path, relPath, true);

   m_ui->output_dir->setText(path);
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

// tab 2- look up (general)
void MainWindow::abbreviate_brief_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->abbreviate_brief->toPlainText();

   data.title      = "Abbreviate Brief Description";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->abbreviate_brief->setPlainText(temp);
   }
}

void MainWindow::strip_from_path_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->strip_from_path->toPlainText();

   data.title      = "Strip From Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->strip_from_path->setPlainText(temp);
   }
}

void MainWindow::strip_from_inc_path_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->strip_from_inc_path->toPlainText();

   data.title      = "Strip From Include Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->strip_from_inc_path->setPlainText(temp);
   }
}

void MainWindow::aliases_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->aliases->toPlainText();

   data.title      = "Aliases";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->aliases->setPlainText(temp);
   }
}

void MainWindow::tcl_subst_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->tcl_subst->toPlainText();

   data.title      = "Tcl Substitue";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->tcl_subst->setPlainText(temp);
   }
}

void MainWindow::extension_mapping_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->extension_mapping->toPlainText();

   data.title      = "Extension Mapping";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->extension_mapping->setPlainText(temp);
   }
}

// tab 2- look up (build)
void MainWindow::enabled_sections_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->enabled_sections->toPlainText();

   data.title      = "Enabled Sections";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->enabled_sections->setPlainText(temp);
   }
}

void MainWindow::file_version_filter_PB()
{
   QString file = m_ui->file_version_filter->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->file_version_filter->setText(file);
}

void MainWindow::layout_file_PB()
{
   QString file = m_ui->layout_file->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->layout_file->setText(file);
}

void MainWindow::cite_bib_files_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->cite_bib_files->toPlainText();

   data.title      = "Cite Bib Files";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->cite_bib_files->setPlainText(temp);
   }
}

// tab 2- look up (messages)
void MainWindow::warn_logfile_PB()
{
   QString file = m_ui->warn_logfile->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->warn_logfile->setText(file);
}

// tab 2- look up (input)
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

void MainWindow::example_source_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->example_source->toPlainText();

   data.title      = "Example Source";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->example_source->setPlainText(temp);
   }
}

void MainWindow::example_patterns_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->example_patterns->toPlainText();

   data.title      = "Example Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->example_patterns->setPlainText(temp);
   }
}

void MainWindow::image_path_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->image_path->toPlainText();

   data.title      = "Image Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->image_path->setPlainText(temp);
   }
}

void MainWindow::input_filter_PB()
{
   QString file = m_ui->input_filter->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->input_filter->setText(file);
}

void MainWindow::filter_patterns_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->file_patterns->toPlainText();

   data.title      = "Filter Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->filter_patterns->setPlainText(temp);
   }
}

void MainWindow::filter_source_patterns_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->filter_source_patterns->toPlainText();

   data.title      = "Filter Source Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->filter_source_patterns->setPlainText(temp);
   }
}

// tab 2- look up (browser)
void MainWindow::clang_options_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->clang_options->toPlainText();

   data.title      = "Clang Options";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->clang_options->setPlainText(temp);
   }
}

// tab 2- look up (index)
void MainWindow::ignore_prefix_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->ignore_prefix->toPlainText();

   data.title      = "Ignore Prefix";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->ignore_prefix->setPlainText(temp);
   }
}

// tab 2- look up (preprocess)
void MainWindow::include_path_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->include_path->toPlainText();

   data.title      = "Include Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->include_path->setPlainText(temp);
   }
}

void MainWindow::include_file_patterns_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->include_file_patterns->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->include_file_patterns->setPlainText(temp);
   }
}

void MainWindow::predefined_macros_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->predefined_macros->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->predefined_macros->setPlainText(temp);
   }
}

void MainWindow::expand_as_defined_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->expand_as_defined->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->expand_as_defined->setPlainText(temp);
   }
}

// tab 2- look up (external)
void MainWindow::tag_files_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->tag_files->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->tag_files->setPlainText(temp);
   }
}

void MainWindow::gen_tagfile_PB()
{
   QString file = m_ui->gen_tagfile->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->gen_tagfile->setText(file);
}

void MainWindow::perl_path_PB()
{
   QString path    = m_ui->perl_path->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->perl_path->setText(path);
}

// tab 2- look up (dot)
void MainWindow::mscgen_path_PB()
{
   QString path    = m_ui->mscgen_path->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->mscgen_path->setText(path);
}

void MainWindow::dia_path_PB()
{
   QString path    = m_ui->dia_path->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->dia_path->setText(path);
}

void MainWindow::dot_font_name_PB()
{
   QString file = m_ui->dot_font_name->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->dot_font_name->setText(file);
}

void MainWindow::dot_font_path_PB()
{
   QString path    = m_ui->dot_font_path->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->dot_font_path->setText(path);
}

void MainWindow::dot_path_PB()
{
   QString path    = m_ui->dot_path->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->dot_path->setText(path);
}

void MainWindow::dot_file_dirs_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->dot_file_dirs->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->dot_file_dirs->setPlainText(temp);
   }
}

void MainWindow::msc_file_dirs_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->msc_file_dirs->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->msc_file_dirs->setPlainText(temp);
   }
}

void MainWindow::dia_file_dirs_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->dia_file_dirs->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->dia_file_dirs->setPlainText(temp);
   }
}

void MainWindow::plantuml_jar_path_PB()
{
   QString path    = m_ui->plantuml_jar_path->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->plantuml_jar_path->setText(path);
}

// tab 3 look up (html)
void MainWindow::html_output_PB()
{
   QString path    = m_ui->html_output->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->html_output->setText(path);
}

void MainWindow::html_header_PB()
{
   QString file = m_ui->html_header->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->html_header->setText(file);
}

void MainWindow::html_footer_PB()
{
   QString file = m_ui->html_footer->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->html_footer->setText(file);
}

void MainWindow::html_extra_stylesheets_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->html_extra_stylesheets->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->html_extra_stylesheets->setPlainText(temp);
   }
}

void MainWindow::html_extra_files_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->html_extra_files->toPlainText();

   data.title      = "BROOM";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->html_extra_files->setPlainText(temp);
   }
}

void MainWindow::chm_file_PB()
{
   QString file = m_ui->chm_file->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->chm_file->setText(file);
}

void MainWindow::hhc_location_PB()
{
   QString file = m_ui->hhc_location->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->hhc_location->setText(file);
}

void MainWindow::qch_file_PB()
{
   QString file = m_ui->qch_file->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->qch_file->setText(file);
}

void MainWindow::qt_help_gen_path_PB()
{
   QString file = m_ui->qthelp_gen_path->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->qthelp_gen_path->setText(file);
}

void MainWindow::mathjax_extensions_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->mathjax_extensions->toPlainText();

   data.title      = "Mathjax Extensions";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->mathjax_extensions->setPlainText(temp);
   }
}

void MainWindow::search_data_file_PB()
{
   QString file = m_ui->search_data_file->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->search_data_file->setText(file);
}

void MainWindow::search_mappings_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->search_mappings->toPlainText();

   data.title      = "Extra Search Mappings";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->search_mappings->setPlainText(temp);
   }
}

// tab 3 look up (latex)
void MainWindow::latex_output_PB()
{
   QString path    = m_ui->latex_output->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->latex_output->setText(path);
}

void MainWindow::latex_cmd_name_PB()
{
   QString file = m_ui->latex_cmd_name->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->latex_cmd_name->setText(file);
}

void MainWindow::make_index_cmd_name_PB()
{
   QString file = m_ui->make_index_cmd_name->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->make_index_cmd_name->setText(file);
}

void MainWindow::latex_extra_packages_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->latex_extra_packages->toPlainText();

   data.title      = "Latex Extra Packages";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->latex_extra_packages->setPlainText(temp);
   }
}

void MainWindow::latex_header_PB()
{
   QString file = m_ui->latex_header->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->latex_header->setText(file);
}

void MainWindow::latex_footer_PB()
{
   QString file = m_ui->latex_footer->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->latex_footer->setText(file);
}

void MainWindow::latex_extra_files_PB()
{
   QRegExp regExp("\\s*,\\s*");
   struct LookUpInfo data;

   QString temp = m_ui->latex_extra_files->toPlainText();

   data.title      = "Latex Extra Files";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->latex_extra_files->setPlainText(temp);
   }
}

// tab 3 look up (rtf)
void MainWindow::rtf_output_PB()
{
   QString path    = m_ui->rtf_output->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->rtf_output->setText(path);
}

void MainWindow::rtf_stylesheet_PB()
{
   QString file = m_ui->rtf_stylesheet->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->rtf_stylesheet->setText(file);
}

void MainWindow::rtf_extension_PB()
{
   QString file = m_ui->rtf_extension->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->rtf_extension->setText(file);
}

// tab 3 look up (man)
void MainWindow::man_output_PB()
{
   QString path    = m_ui->man_output->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->man_output->setText(path);
}

// tab 3 look up (xml)
void MainWindow::xml_output_PB()
{
   QString path    = m_ui->xml_output->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->xml_output->setText(path);
}

// tab 3 look up (docbook)
void MainWindow::docbook_output_PB()
{   
   QString path    = m_ui->docbook_output->text();
   QString relPath = m_ui->output_dir->text();

   path = get_DirPath(tr("Select destination directory"), path, relPath);

   m_ui->docbook_output->setText(path);
}
