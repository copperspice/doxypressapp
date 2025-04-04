/************************************************************************
*
* Copyright (c) 2014-2025 Barbara Geller & Ansel Sermersheim
*
* Copyright (c) 1997-2014 by Dimitri van Heesch
*
* DoxyPressApp is free software. You can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
* DoxyPressApp is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* Documents produced by DoxyPress are derivative works derived from the
* input used in their production; they are not affected by this license.
*
* https://www.gnu.org/licenses/
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

      if (! tempName.isEmpty() && ! QDir::isAbsolutePath(tempName) ) {
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

      if (! tempName.isEmpty() && ! QDir::isAbsolutePath(tempName) ) {
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->abbreviate_brief->toPlainText();

   lookUpData.title      = "Abbreviate Brief Description";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->abbreviate_brief->setPlainText(tmp);
   }
}

void MainWindow::strip_from_path_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->strip_from_path->toPlainText();

   lookUpData.title      = "Strip From Path";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->strip_from_path->setPlainText(tmp);
   }
}

void MainWindow::strip_from_inc_path_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->strip_from_inc_path->toPlainText();

   lookUpData.title      = "Strip From Include Path";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->strip_from_inc_path->setPlainText(tmp);
   }
}

void MainWindow::aliases_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->aliases->toPlainText();

   lookUpData.title      = "Aliases";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->aliases->setPlainText(tmp);
   }
}

void MainWindow::tcl_subst_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->tcl_subst->toPlainText();

   lookUpData.title      = "Tcl Substitute";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->tcl_subst->setPlainText(tmp);
   }
}

void MainWindow::language_mapping_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->language_mapping->toPlainText();

   lookUpData.title      = "Language Mapping";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->language_mapping->setPlainText(tmp);
   }
}

// tab 2- look up (build)
void MainWindow::enabled_sections_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->enabled_sections->toPlainText();

   lookUpData.title      = "Enabled Sections";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->enabled_sections->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->ns_alias->toPlainText();

   lookUpData.title      = "Namespace Alias";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->ns_alias->setPlainText(tmp);
   }
}

// tab 2- look up (messages)
void MainWindow::warn_sa_ignore_words_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->warn_sa_ignore_words->toPlainText();

   lookUpData.title      = "List of Words to Ignore in a See Also";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->warn_sa_ignore_words->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->input_source->toPlainText();

   lookUpData.title      = "Input Source";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->input_source->setPlainText(tmp);
   }
}

void MainWindow::input_patterns_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->input_patterns->toPlainText();

   lookUpData.title      = "Input Patterns";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->input_patterns->setPlainText(tmp);
   }
}

void MainWindow::exclude_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->exclude_files->toPlainText();

   lookUpData.title      = "Exclude Files or Folders";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->exclude_files->setPlainText(tmp);
   }
}

void MainWindow::exclude_patterns_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->exclude_patterns->toPlainText();

   lookUpData.title      = "Exclude Patterns";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->exclude_patterns->setPlainText(tmp);
   }
}

void MainWindow::exclude_symbols_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->exclude_symbols->toPlainText();

   lookUpData.title      = "Exclude Symbols";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->exclude_symbols->setPlainText(tmp);
   }
}

void MainWindow::example_source_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->example_source->toPlainText();

   lookUpData.title      = "Example Files or Directories";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->example_source->setPlainText(tmp);
   }
}

void MainWindow::example_patterns_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->example_patterns->toPlainText();

   lookUpData.title      = "Example Patterns";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->example_patterns->setPlainText(tmp);
   }
}

void MainWindow::image_path_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->image_path->toPlainText();

   lookUpData.title      = "Image Path";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->image_path->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->filter_patterns->toPlainText();

   lookUpData.title      = "Filter Patterns";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->filter_patterns->setPlainText(tmp);
   }
}

void MainWindow::filter_source_patterns_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->filter_source_patterns->toPlainText();

   lookUpData.title      = "Filter Source Patterns";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->filter_source_patterns->setPlainText(tmp);
   }
}

// tab 2- look up (source code)
void MainWindow::suffix_source_navtree_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->suffix_source_navtree->toPlainText();

   lookUpData.title      = "Extensions Source Navtree";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->suffix_source_navtree->setPlainText(tmp);
   }
}

void MainWindow::suffix_header_navtree_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->suffix_header_navtree->toPlainText();

   lookUpData.title      = "Extensions Header Navtree";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->suffix_header_navtree->setPlainText(tmp);
   }
}

void MainWindow::suffix_exclude_navtree_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->suffix_exclude_navtree->toPlainText();

   lookUpData.title      = "Extensions Exclude Navtree";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->suffix_exclude_navtree->setPlainText(tmp);
   }
}

void MainWindow::clang_options_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->clang_flags->toPlainText();

   lookUpData.title      = "Clang Options";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->clang_flags->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->ignore_prefix->toPlainText();

   lookUpData.title      = "Ignore Prefix";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->ignore_prefix->setPlainText(tmp);
   }
}

// tab 2- look up (preprocess)
void MainWindow::include_path_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->include_path->toPlainText();

   lookUpData.title      = "Include Path";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->include_path->setPlainText(tmp);
   }
}

void MainWindow::include_file_patterns_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->include_patterns->toPlainText();

   lookUpData.title      = "Inlcude File Patterns";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->include_patterns->setPlainText(tmp);
   }
}

void MainWindow::predefined_macros_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->predefined_macros->toPlainText();

   lookUpData.title      = "Predefined Macros";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->predefined_macros->setPlainText(tmp);
   }
}

void MainWindow::expand_as_defined_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->expand_as_defined->toPlainText();

   lookUpData.title      = "Expand As Defined";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->expand_as_defined->setPlainText(tmp);
   }
}

// tab 2- look up (external)
void MainWindow::tag_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->tag_files->toPlainText();

   lookUpData.title      = "Tag Files";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->tag_files->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->dot_file_dirs->toPlainText();

   lookUpData.title      = "Dot File Directory";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->dot_file_dirs->setPlainText(tmp);
   }
}

void MainWindow::msc_file_dirs_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->msc_file_dirs->toPlainText();

   lookUpData.title      = "MSC File Directory";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->msc_file_dirs->setPlainText(tmp);
   }
}

void MainWindow::dia_file_dirs_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->dia_file_dirs->toPlainText();

   lookUpData.title      = "Diafile Directories";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = OUTPUT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->dia_file_dirs->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->plantuml_inc_path->toPlainText();

   lookUpData.title      = "PlantUML Include Path";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = true;
   lookUpData.relativeTo = OUTPUT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->plantuml_inc_path->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->html_stylesheets->toPlainText();

   lookUpData.title      = "HTML Stylesheets";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->html_stylesheets->setPlainText(tmp);
   }
}

void MainWindow::html_extra_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->html_extra_files->toPlainText();

   lookUpData.title      = "HTML Extra Files";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->html_extra_files->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->qhp_cust_attrib->toPlainText();

   lookUpData.title      = "Qt Help Custom Attribute";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->qhp_cust_attrib->setPlainText(tmp);
   }
}

void MainWindow::qhp_sect_attrib_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->qhp_sect_attrib->toPlainText();

   lookUpData.title      = "Qt Help Section Attribute";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->qhp_sect_attrib->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->mathjax_extensions->toPlainText();

   lookUpData.title      = "Mathjax Extensions";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->mathjax_extensions->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->search_mappings->toPlainText();

   lookUpData.title      = "Extra Search Mappings";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->search_mappings->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->latex_extra_packages->toPlainText();

   lookUpData.title      = "Latex Extra Packages";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = false;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = ABSOLUTE_ONLY;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->latex_extra_packages->setPlainText(tmp);
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
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->latex_stylesheets->toPlainText();

   lookUpData.title      = "Latex Stylesheets";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->latex_stylesheets->setPlainText(tmp);
   }
}

void MainWindow::latex_extra_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->latex_stylesheets->toPlainText();

   lookUpData.title      = "Latex Extra Files";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->latex_extra_files->setPlainText(tmp);
   }
}

void MainWindow::cite_bib_files_PB()
{
   QRegularExpression regExp("\\s*, \\s*");
   struct LookUpInfo lookUpData;

   QString plainText = m_ui->cite_bib_files->toPlainText();

   lookUpData.title      = "Cite Bib Files";
   lookUpData.dataList   = plainText.split(regExp);
   lookUpData.isFilePB   = true;
   lookUpData.isFolderPB = false;
   lookUpData.relativeTo = PROJECT_DIR;

   Dialog_LookUp *dw = new Dialog_LookUp(this, lookUpData);
   int result = dw->exec();

   if (result == QDialog::Accepted) {
      QStringList dataList = dw->getData();

      QString tmp = dataList.join(", ");
      m_ui->cite_bib_files->setPlainText(tmp);
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
