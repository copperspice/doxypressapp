/************************************************************************
*
* Copyright (c) 2014-2024 Barbara Geller & Ansel Sermersheim
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

#include "colors.h"
#include "dialog_lookup.h"
#include "mainwindow.h"

#include <QRegularExpression>

// tab 1
void MainWindow::getLogo(const QString route)
{
   QString projectDir = pathName(m_curFile);
   QString logoName   = m_ui->project_logo->text();

   if (route == "load") {
      QString tempName = logoName;

      if (! tempName.isEmpty() && ! QDir::isAbsolutePath(tempName) )  {
         tempName = projectDir + "/" + tempName;
      }

   } else if (route == "select") {
      logoName = getSingleFile(tr("Select Project Logo"), logoName);
      m_ui->project_logo->setText(logoName);
   }

   if (logoName.isEmpty()) {
      m_ui->project_logo_image->setText(tr("No Logo was selected"));

   } else {
      QString tempName = logoName;

      if (! tempName.isEmpty() && ! QDir::isAbsolutePath(tempName) )  {
         tempName = projectDir + "/" + tempName;
      }

      QFile fout(tempName);

      if (! fout.exists()) {
         m_ui->project_logo_image->setText(tr("Unable to find file: ") + tempName);

      } else {
         QPixmap pm(tempName);

         if (! pm.isNull()) {
            m_ui->project_logo_image->setPixmap(pm.scaledToHeight(55, Qt::SmoothTransformation));

         } else {
            m_ui->project_logo_image->setText(tr("No preview is available for: ") + tempName);
         }
      }
   }
}

void MainWindow::output_dir_PB()
{
   QString path = m_ui->output_dir->text();
   path = get_DirPath(tr("Select destination directory"), path, PROJECT_DIR);
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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->abbreviate_brief->toPlainText();

   data.title      = "Abbreviate Brief Description";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->strip_from_path->toPlainText();

   data.title      = "Strip From Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->strip_from_inc_path->toPlainText();

   data.title      = "Strip From Include Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->aliases->toPlainText();

   data.title      = "Aliases";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->tcl_subst->toPlainText();

   data.title      = "Tcl Substitute";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->tcl_subst->setPlainText(temp);
   }
}

void MainWindow::language_mapping_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->language_mapping->toPlainText();

   data.title      = "Language Mapping";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->language_mapping->setPlainText(temp);
   }
}

// tab 2- look up (build)
void MainWindow::enabled_sections_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->enabled_sections->toPlainText();

   data.title      = "Enabled Sections";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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

void MainWindow::main_page_name_PB()
{
   QString file = m_ui->main_page_name->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->main_page_name->setText(file);
}

void MainWindow::layout_file_PB()
{
   QString file = m_ui->layout_file->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->layout_file->setText(file);
}

void MainWindow::ns_alias_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->ns_alias->toPlainText();

   data.title      = "Namespace Alias";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->ns_alias->setPlainText(temp);
   }
}

// tab 2- look up (messages)
void MainWindow::warn_sa_ignore_words_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->warn_sa_ignore_words->toPlainText();

   data.title      = "List of Words to Ignore in a See Also";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->warn_sa_ignore_words->setPlainText(temp);
   }
}

void MainWindow::warn_logfile_PB()
{
   QString file = m_ui->warn_logfile->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->warn_logfile->setText(file);
}

// tab 2- look up (input)
void MainWindow::input_source_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->input_source->toPlainText();

   data.title      = "Input Source";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->input_source->setPlainText(temp);
   }
}

void MainWindow::input_patterns_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->input_patterns->toPlainText();

   data.title      = "Input Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->input_patterns->setPlainText(temp);
   }
}

void MainWindow::exclude_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp    = m_ui->exclude_files->toPlainText();

   data.title      = "Exclude Files or Folders";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp    = m_ui->exclude_patterns->toPlainText();

   data.title      = "Exclude Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp    = m_ui->exclude_symbols->toPlainText();

   data.title      = "Exclude Symbols";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->example_source->toPlainText();

   data.title      = "Example Files or Directories";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->example_patterns->toPlainText();

   data.title      = "Example Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->image_path->toPlainText();

   data.title      = "Image Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->image_path->setPlainText(temp);
   }
}

void MainWindow::mdfile_mainpage_PB()
{
   QString file = m_ui->mdfile_mainpage->text();
   file = getSingleFile(tr("Select file"), file, "*.md");
   m_ui->mdfile_mainpage->setText(file);
}

void MainWindow::filter_program_PB()
{
   QString file = m_ui->filter_program->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->filter_program->setText(file);
}

void MainWindow::filter_patterns_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->filter_patterns->toPlainText();

   data.title      = "Filter Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->filter_source_patterns->toPlainText();

   data.title      = "Filter Source Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->filter_source_patterns->setPlainText(temp);
   }
}

// tab 2- look up (source code)
void MainWindow::suffix_source_navtree_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->suffix_source_navtree->toPlainText();

   data.title      = "Extensions Source Navtree";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->suffix_source_navtree->setPlainText(temp);
   }
}

void MainWindow::suffix_header_navtree_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->suffix_header_navtree->toPlainText();

   data.title      = "Extensions Header Navtree";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->suffix_header_navtree->setPlainText(temp);
   }
}

void MainWindow::suffix_exclude_navtree_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->suffix_exclude_navtree->toPlainText();

   data.title      = "Extensions Exclude Navtree";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->suffix_exclude_navtree->setPlainText(temp);
   }
}

void MainWindow::clang_options_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->clang_flags->toPlainText();

   data.title      = "Clang Options";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->clang_flags->setPlainText(temp);
   }
}

void MainWindow::clang_compilation_path_PB()
{
   QString path = m_ui->clang_compilation_path->text();
   path = get_DirPath(tr("Select compilation path"), path, PROJECT_DIR);
   m_ui->clang_compilation_path->setText(path);
}

// tab 2- look up (index)
void MainWindow::ignore_prefix_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->ignore_prefix->toPlainText();

   data.title      = "Ignore Prefix";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->include_path->toPlainText();

   data.title      = "Include Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->include_patterns->toPlainText();

   data.title      = "Inlcude File Patterns";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->include_patterns->setPlainText(temp);
   }
}

void MainWindow::predefined_macros_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->predefined_macros->toPlainText();

   data.title      = "Predefined Macros";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->expand_as_defined->toPlainText();

   data.title      = "Expand As Defined";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->tag_files->toPlainText();

   data.title      = "Tag Files";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = false;
   data.relativeTo = PROJECT_DIR;

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

// tab 2- look up (dot)
void MainWindow::mscgen_path_PB()
{
   QString path = m_ui->mscgen_path->text();
   path = get_DirPath(tr("Select mscgen path"), path, ABSOLUTE_ONLY);

   m_ui->mscgen_path->setText(path);
}

void MainWindow::dia_path_PB()
{
   QString path = m_ui->dia_path->text();
   path = get_DirPath(tr("Select dia path"), path, ABSOLUTE_ONLY);

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
   QString path = m_ui->dot_font_path->text();
   path = get_DirPath(tr("Select font path"), path, PROJECT_DIR);

   m_ui->dot_font_path->setText(path);
}

void MainWindow::dot_path_PB()
{
   QString path = m_ui->dot_path->text();
   path = get_DirPath(tr("Select dot path"), path, ABSOLUTE_ONLY);

   m_ui->dot_path->setText(path);
}

void MainWindow::dot_file_dirs_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->dot_file_dirs->toPlainText();

   data.title      = "Dot File Directory";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->msc_file_dirs->toPlainText();

   data.title      = "MSC File Directory";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = PROJECT_DIR;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->dia_file_dirs->toPlainText();

   data.title      = "Diafile Directories";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = OUTPUT_DIR;

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
   QString path = m_ui->plantuml_jar_path->text();
   path = get_DirPath(tr("Select jar path"), path, PROJECT_DIR);

   m_ui->plantuml_jar_path->setText(path);
}

void MainWindow::plantuml_cfg_file_PB()
{
   QString path = m_ui->plantuml_cfg_file->text();
   path = get_DirPath(tr("Select plantuml cfg path"), path, PROJECT_DIR);

   m_ui->plantuml_cfg_file->setText(path);
}

void MainWindow::plantuml_inc_path_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->plantuml_inc_path->toPlainText();

   data.title      = "PlantUML Include Path";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = true;
   data.relativeTo = OUTPUT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->plantuml_inc_path->setPlainText(temp);
   }
}


// tab 3 look up (html)
void MainWindow::html_output_PB()
{
   QString path = m_ui->html_output->text();
   path = get_DirPath(tr("Select html output directory"), path, OUTPUT_DIR);

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

void MainWindow::html_stylesheets_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->html_stylesheets->toPlainText();

   data.title      = "HTML Stylesheets";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = false;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->html_stylesheets->setPlainText(temp);
   }
}

void MainWindow::html_extra_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->html_extra_files->toPlainText();

   data.title      = "HTML Extra Files";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = false;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->html_extra_files->setPlainText(temp);
   }
}

void MainWindow::formula_macrofile_PB()
{
   QString file = m_ui->formula_macrofile->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->formula_macrofile->setText(file);
}

void MainWindow::ghostscript_PB()
{
   QString file = m_ui->ghostscript->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->ghostscript->setText(file);
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

void MainWindow::qhp_cust_attrib_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->qhp_cust_attrib->toPlainText();

   data.title      = "Qt Help Custom Attribute";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->qhp_cust_attrib->setPlainText(temp);
   }
}

void MainWindow::qhp_sect_attrib_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->qhp_sect_attrib->toPlainText();

   data.title      = "Qt Help Section Attribute";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->qhp_sect_attrib->setPlainText(temp);
   }
}

void MainWindow::qthelp_gen_path_PB()
{
   QString file = m_ui->qthelp_gen_path->text();
   file = getSingleFile(tr("Select file"), file);
   m_ui->qthelp_gen_path->setText(file);
}

void MainWindow::mathjax_extensions_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->mathjax_extensions->toPlainText();

   data.title      = "Mathjax Extensions";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->search_mappings->toPlainText();

   data.title      = "Extra Search Mappings";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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
   QString path = m_ui->latex_output->text();
   path = get_DirPath(tr("Select latex output directory"), path, OUTPUT_DIR);

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
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->latex_extra_packages->toPlainText();

   data.title      = "Latex Extra Packages";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = false;
   data.isFolderPB = false;
   data.relativeTo = ABSOLUTE_ONLY;

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

void MainWindow::latex_stylesheets_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->latex_stylesheets->toPlainText();

   data.title      = "Latex Stylesheets";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = false;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->latex_stylesheets->setPlainText(temp);
   }
}

void MainWindow::latex_extra_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->latex_stylesheets->toPlainText();

   data.title      = "Latex Extra Files";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = false;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->latex_extra_files->setPlainText(temp);
   }
}

void MainWindow::cite_bib_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo data;

   QString temp = m_ui->cite_bib_files->toPlainText();

   data.title      = "Cite Bib Files";
   data.dataList   = temp.split(regExp);
   data.isFilePB   = true;
   data.isFolderPB = false;
   data.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, data);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString temp = dataList.join(", ");
      m_ui->cite_bib_files->setPlainText(temp);
   }
}

// tab 3 look up (rtf)
void MainWindow::rtf_output_PB()
{
   QString path = m_ui->rtf_output->text();
   path = get_DirPath(tr("Select rtf output directory"), path, OUTPUT_DIR);

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
   QString path = m_ui->man_output->text();
   path = get_DirPath(tr("Select man output directory"), path, OUTPUT_DIR);

   m_ui->man_output->setText(path);
}

// tab 3 look up (xml)
void MainWindow::xml_output_PB()
{
   QString path = m_ui->xml_output->text();
   path = get_DirPath(tr("Select xml output directory"), path, OUTPUT_DIR);

   m_ui->xml_output->setText(path);
}

// tab 3 look up (docbook)
void MainWindow::docbook_output_PB()
{
   QString path = m_ui->docbook_output->text();
   path = get_DirPath(tr("Select docbook output directory"), path, OUTPUT_DIR);

   m_ui->docbook_output->setText(path);
}
