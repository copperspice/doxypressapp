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

#include "mainwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QSpinBox>

static QString getFilePattens();
const QString MainWindow::m_filePatterns = getFilePattens();

QMap<QWidget *, HelpData> MainWindow::m_bigMap;

void MainWindow::clearAllFields()
{
   for (auto iter = m_bigMap.begin(); iter != m_bigMap.end(); ++iter) {

      QWidget *item = iter.key();

      //
      QCheckBox *temp_CB = dynamic_cast<QCheckBox *>(item);

      if (temp_CB) {
         temp_CB->setChecked(false);

         QLabel *label = getLabel(temp_CB);
         if (label) {
            setLabelColor(DEFAULT, label);
         }
      }

      QGroupBox *temp_GB = dynamic_cast<QGroupBox *>(item);

      if (temp_GB) {
         temp_GB->setChecked(false);

         // no label coloring
      }

      QComboBox *temp_CM = dynamic_cast<QComboBox *>(item);

      if (temp_CM) {
        temp_CM->setCurrentIndex(-1);

        QLabel *label = getLabel(temp_CM);
        if (label) {
           setLabelColor(DEFAULT, label);
        }
      }

      QLineEdit *temp_LE = dynamic_cast<QLineEdit *>(item);

      if (temp_LE) {
         temp_LE->setText("");

         QLabel *label = getLabel(temp_LE);
         if (label) {
            setLabelColor(DEFAULT, label);
         }
      }

      QPlainTextEdit *temp_PT = dynamic_cast<QPlainTextEdit *>(item);

      if (temp_PT) {
         temp_PT->setPlainText("");

         QLabel *label = getLabel(temp_PT);
         if (label) {
            setLabelColor(DEFAULT, label);
         }
      }

      QRadioButton *temp_RB = dynamic_cast<QRadioButton *>(item);

      if (temp_RB) {
         temp_RB->setChecked(false);

         // no label coloring
      }

      QSpinBox *temp_SB = dynamic_cast<QSpinBox *>(item);

      if (temp_SB) {
         temp_SB->setValue(0);

         QLabel *label = getLabel(temp_SB);
         if (label) {
            setLabelColor(DEFAULT, label);
         }
      }
   }

   int index;

   // reset initial default values
   m_ui->project_name->setText("My Project");
   m_project_logoFN = "";

   m_ui->optimize_cplus_CB->setChecked(true);
   m_ui->gen_html_CB1->setChecked(true);
   m_ui->html_search_CB1->setChecked(true);

   m_ui->html_colorstyle_hue->setValue(220);
   m_ui->html_colorstyle_sat->setValue(100);
   m_ui->html_colorstyle_gamma->setValue(80);

   m_ui->diagram_built_in_RB->setChecked(true);
   m_ui->dot_class_graph_CB1->setChecked(true);
   m_ui->dot_collaboration_CB1->setChecked(true);
   m_ui->dot_hierarchy_CB1->setChecked(true);
   m_ui->dot_include_CB1->setChecked(true);
   m_ui->dot_included_by_CB1->setChecked(true);

   // tab 2 - project
   m_ui->project_encoding->setText("UTF-8");

   index = m_ui->output_language_CM->findText("English");
   m_ui->output_language_CM->setCurrentIndex(index);

   m_ui->brief_member_desc_CB->setChecked(true);
   m_ui->repeat_brief_CB->setChecked(true);

   QStringList tempStr;
   tempStr.append("The $name class");
   tempStr.append("The $name widget");
   tempStr.append("The $name file");
   tempStr.append("is");
   tempStr.append("provides");
   tempStr.append("specifies");
   tempStr.append("contains");
   tempStr.append("represents");
   tempStr.append("a");
   tempStr.append("an");
   tempStr.append("the");

   QString tempText = tempStr.join(", ");
   m_ui->abbreviate_brief->setPlainText(tempText);

   m_ui->full_path_names_CB->setChecked(true);
   m_ui->inherit_docs_CB->setChecked(true);
   m_ui->tab_size_SB->setValue(4);
   m_ui->markdown_CB->setChecked(true);
   m_ui->auto_link_CB->setChecked(true);
   m_ui->idl_support_CB->setChecked(true);
   m_ui->allow_sub_grouping_CB->setChecked(true);

   // tab 2 - build
   m_ui->extract_local_classes_CB->setChecked(true);
   m_ui->case_sense_names_CB->setChecked(true);

   m_ui->show_include_files_CB->setChecked(true);
   m_ui->inline_info_CB->setChecked(true);   
   m_ui->sort_member_docs_CB->setChecked(true);   
   m_ui->gen_todo_list_CB->setChecked(true);
   m_ui->gen_test_list_CB->setChecked(true);
   m_ui->gen_bug_list_CB->setChecked(true);
   m_ui->gen_deprecate_list_CB->setChecked(true);

   m_ui->max_init_lines_SB->setValue(30);
   m_ui->show_used_files_CB->setChecked(true);
   m_ui->show_files_CB->setChecked(true);
   m_ui->show_namespaces_CB->setChecked(true);

   // tab 2 - messages
   m_ui->warnings_CB->setChecked(true);
   m_ui->warn_undoc_CB->setChecked(true);
   m_ui->warn_doc_error_CB->setChecked(true);

   m_ui->warn_format->setText("$file:$line: $text");

   // tab 2 -input
   m_ui->input_encoding->setText("UTF-8");
   m_ui->file_patterns->setPlainText(m_filePatterns);
   m_ui->example_patterns->setPlainText("*");

   // tab 2 -source browser
   m_ui->strip_code_comments_CB->setChecked(true);
   m_ui->ref_link_source_CB ->setChecked(true);
   m_ui->source_tooltips_CB->setChecked(true);
   m_ui->verbatim_headers_CB->setChecked(true);

   // tab 2 -index
   m_ui->alpha_index_CB->setChecked(true);
   m_ui->cols_in_index_SB->setValue(5);

   // tab 2 permod
   m_ui->perl_pretty_CB->setChecked(true);

   // tab 2 - preprocess
   m_ui->enable_preprocessing_CB->setChecked(true);
   m_ui->search_includes_CB->setChecked(true);
   m_ui->skip_function_macros_CB->setChecked(true);

   // tab 2 - external
   m_ui->external_groups_CB->setChecked(true);
   m_ui->external_pages_CB->setChecked(true);
   m_ui->perl_path->setText("/usr/bin/perl");

   // tab 2 - dot
   m_ui->class_diagrams_CB->setChecked(true);
   m_ui->hide_undoc_relations_CB->setChecked(true);
   m_ui->dot_font_name->setText("Helvetica");
   m_ui->dot_font_size_SB->setValue(10);
   m_ui->group_graphs_CB->setChecked(true);
   m_ui->uml_limit_num_fields_SB->setValue(10);
   m_ui->directory_graph_CB->setChecked(true);

   index = m_ui->dot_image_format_CM->findText("png");
   m_ui->dot_image_format_CM->setCurrentIndex(index);

   m_ui->dot_graph_max_nodes_SB->setValue(50);
   m_ui->gen_legend_CB->setChecked(true);
   m_ui->dot_cleanup_CB->setChecked(true);

   // tab 3 - html
   m_ui->html_output->setText("html");
   m_ui->html_file_extension->setText(".html");
   m_ui->html_timestamp_CB->setChecked(true);
   m_ui->html_index_num_entries_SB->setValue(100);
   m_ui->treeview_width_SB ->setValue(250);
   m_ui->enum_values_per_line_SB->setValue(4);
   m_ui->search_data_file->setText("searchdata.xml");

   m_ui->formula_fontsize_SB->setValue(10);
   m_ui->formula_transparent_CB->setChecked(true);

   index = m_ui->mathjax_format_CM->findText("HTML-CSS");
   m_ui->mathjax_format_CM->setCurrentIndex(index);

   m_ui->mathjax_relpath->setText("http://cdn.mathjax.org/mathjax/latest");

   // tab 3 - chm

   // tab 3 - docset
   m_ui->docset_feedname->setText("DoxyPress generated docs");
   m_ui->docset_bundle_id->setText("org.doxypress.Project");
   m_ui->docset_publisher_id->setText("org.doxypress.Publisher");
   m_ui->docset_publisher_name->setText("Publisher");

   // tab 3 - eclipse
   m_ui->eclipse_doc_id->setText("org.doxypress.Project");

   // tab 3 - latex
   m_ui->latex_output->setText("latex");
   m_ui->latex_cmd_name->setText("latex");
   m_ui->make_index_cmd_name->setText("makeindex");

   index = m_ui->latex_paper_type_CM->findText("a4");
   m_ui->latex_paper_type_CM->setCurrentIndex(index);

   m_ui->latex_hyper_pdf_CB->setChecked(true);
   m_ui->latex_pdf_CB->setChecked(true);
   m_ui->latex_bib_style->setText("plain");

   // tab 3 - man
   m_ui->man_output->setText("man");
   m_ui->man_extension->setText(".3");

   // tab 3 - qthelp
   m_ui->qhp_namespace->setText("org.doxypress.Project");
   m_ui->qhp_virtual_folder->setText("doc");

   // tab 3 - rtf
   m_ui->rtf_output->setText("rtf");

   // tab 3 - xml
   m_ui->xml_output->setText("xml");
   m_ui->xml_program_listing_CB->setChecked(true);

   // tab 3 - docbook
   m_ui->docbook_output->setText("docbook");

   // final step
   finalLoad();
}

void MainWindow::finalLoad()
{
   getLogo("load");

   setDuplicates();
   validGet_html();
   validGet_latex();
   validGet_dot();

   valid_gen_html();
   valid_gen_chm();
   valid_gen_docset();
   valid_gen_eclipse();
   valid_gen_latex();
   valid_gen_man();
   valid_gen_qthelp();
   valid_gen_rtf();
   valid_gen_xml();
   valid_gen_docbook();

   valid_html_search();
}

void MainWindow::setDuplicates()
{
   // rb (tab 1), CB (tab 3)
   m_ui->html_chm_RB->setChecked(m_ui->gen_chm_CB->isChecked());
   m_ui->html_search_CB1->setChecked(m_ui->html_search_CB2->isChecked());

   //
   m_ui->gen_html_CB2->setChecked(m_ui->gen_html_CB1->isChecked());
   m_ui->gen_latex_CB2->setChecked(m_ui->gen_latex_CB1->isChecked());
   m_ui->gen_rtf_CB2->setChecked(m_ui->gen_rtf_CB1->isChecked());
   m_ui->gen_man_CB2->setChecked(m_ui->gen_man_CB1->isChecked());
   m_ui->gen_xml_CB2->setChecked(m_ui->gen_xml_CB1->isChecked());
   m_ui->gen_docbook_CB2->setChecked(m_ui->gen_docbook_CB1->isChecked());

   m_ui->dot_class_graph_CB2->setChecked(m_ui->dot_class_graph_CB1->isChecked());
   m_ui->dot_collaboration_CB2->setChecked(m_ui->dot_collaboration_CB1->isChecked());
   m_ui->dot_hierarchy_CB2->setChecked(m_ui->dot_hierarchy_CB1->isChecked());
   m_ui->dot_include_CB2->setChecked(m_ui->dot_include_CB1->isChecked());
   m_ui->dot_included_by_CB2->setChecked(m_ui->dot_included_by_CB1->isChecked());
   m_ui->dot_call_CB2->setChecked(m_ui->dot_call_CB1->isChecked());
   m_ui->dot_called_by_CB2->setChecked(m_ui->dot_called_by_CB1->isChecked());
}

void MainWindow::setupLimits()
{
   QString temp;
   QStringList data;

   // tab 2 - general
   temp = "Afrikaans, Arabic, Armenian, Brazilian, Catalan, Chinese, "
         "Chinese-Traditional, Croatian, Czech, Danish, Dutch, English, "
         "Esperanto, Farsi (Persian), Finnish, French, German, Greek, Hungarian, "
         "Indonesian, Italian, Japanese, Japanese-en (Japanese with English messages), "
         "Korean, Korean-en (Korean with English messages), Latvian, Lithuanian, "
         "Macedonian, Norwegian, Persian (Farsi), Polish, Portuguese, Romanian, Russian, "
         "Serbian, Serbian-Cyrillic, Slovak, Slovene, Spanish, Swedish, Turkish, Ukrainian, Vietnamese";

   data.clear();
   data = temp.split(", ");
   m_ui->output_language_CM->addItems(data);

   data.clear();
   data.append("png");
   data.append("jpg");
   data.append("gif");
   data.append("svg");
   m_ui->dot_image_format_CM->addItems(data);

   data.clear();
   data.append("HTML-CSS");
   data.append("NativeMML");
   data.append("SVG");
   m_ui->mathjax_format_CM->addItems(data);

   // tab 3 - latex
   data.clear();
   data.append("a4");
   data.append("letter");
   data.append("legal");
   data.append("executive");
   m_ui->latex_paper_type_CM->addItems(data);

   // tab 2 -
   m_ui->tab_size_SB->setMinimum(1);
   m_ui->tab_size_SB->setMaximum(16);

   m_ui->lookup_cache_size_SB->setMinimum(0);
   m_ui->lookup_cache_size_SB->setMaximum(9);

  // tab 2 - build
   m_ui->max_init_lines_SB->setMinimum(0);
   m_ui->max_init_lines_SB->setMaximum(10000);

   // tab 2 - index
   m_ui->cols_in_index_SB->setMinimum(1);
   m_ui->cols_in_index_SB->setMaximum(20);

   // tab 2 - dot
   m_ui->dot_num_threads_SB->setMinimum(0);
   m_ui->dot_num_threads_SB->setMaximum(10000);

   m_ui->dot_font_size_SB->setMinimum(4);
   m_ui->dot_font_size_SB->setMaximum(24);

   m_ui->uml_limit_num_fields_SB->setMinimum(0);
   m_ui->uml_limit_num_fields_SB->setMaximum(100);

   m_ui->dot_graph_max_nodes_SB->setMinimum(0);
   m_ui->dot_graph_max_nodes_SB->setMaximum(10000);

   m_ui->dot_graph_max_depth_SB->setMinimum(0);
   m_ui->dot_graph_max_depth_SB->setMaximum(1000);

   // tab 3 html
   m_ui->html_colorstyle_hue->setMinimum(0);
   m_ui->html_colorstyle_hue->setMaximum(359);

   m_ui->html_colorstyle_sat->setMinimum(0);
   m_ui->html_colorstyle_sat->setMaximum(255);

   m_ui->html_colorstyle_gamma->setMinimum(0);
   m_ui->html_colorstyle_gamma->setMaximum(240);

   m_ui->html_index_num_entries_SB->setMinimum(0);
   m_ui->html_index_num_entries_SB->setMaximum(9999);

   m_ui->treeview_width_SB ->setMinimum(0);
   m_ui->treeview_width_SB ->setMaximum(1500);

   m_ui->enum_values_per_line_SB->setMinimum(0);
   m_ui->enum_values_per_line_SB->setMaximum(20);

   m_ui->formula_fontsize_SB->setMinimum(8);
   m_ui->formula_fontsize_SB->setMaximum(50);
}

// **
void MainWindow::valid_output_dir()
{
   if (m_ui->output_dir->text().contains("$output_dir")) {
      csError("Output Directory", "Value of '$output_dir' is not allowed, specify an absolute or relative path.");
      m_ui->output_dir->setText("");
   }
}

void MainWindow::valid_gen_html()
{
   // this line can be redundant
   m_ui->gen_html_CB1->setChecked(m_ui->gen_html_CB2->isChecked());

   if (m_ui->gen_html_CB2->isChecked()) {                 
      m_ui->html_output->setEnabled(true);
      m_ui->html_output_PB->setEnabled(true);
      m_ui->html_file_extension->setEnabled(true);
      m_ui->html_header->setEnabled(true);
      m_ui->html_header_PB->setEnabled(true);
      m_ui->html_footer->setEnabled(true);
      m_ui->html_footer_PB->setEnabled(true);
      m_ui->html_stylesheets->setEnabled(true);
      m_ui->html_stylesheets_PB->setEnabled(true);
      m_ui->html_extra_files->setEnabled(true);
      m_ui->html_extra_files_PB->setEnabled(true);
      m_ui->html_colorstyle_hue->setEnabled(true);
      m_ui->html_colorstyle_sat->setEnabled(true);
      m_ui->html_colorstyle_gamma->setEnabled(true);
      m_ui->html_timestamp_CB->setEnabled(true);
      m_ui->html_dynamic_sections_CB->setEnabled(true);
      m_ui->html_index_num_entries_SB->setEnabled(true);           

      m_ui->disable_index_CB->setEnabled(true);
      m_ui->gen_treeview_CB->setEnabled(true);
      m_ui->treeview_width_SB->setEnabled(true);
      m_ui->enum_values_per_line_SB->setEnabled(true);
      m_ui->external_links_in_window_CB->setEnabled(true);

      m_ui->formula_fontsize_SB->setEnabled(true);
      m_ui->formula_transparent_CB->setEnabled(true);
      m_ui->ghostscript->setEnabled(true);
      m_ui->use_mathjax_CB->setEnabled(true);
      m_ui->mathjax_format_CM->setEnabled(true);
      m_ui->mathjax_relpath->setEnabled(true);
      m_ui->mathjax_extensions->setEnabled(true);
      m_ui->mathjax_extensions_PB->setEnabled(true);
      m_ui->mathjax_codefile->setEnabled(true);

      m_ui->gen_chm_CB->setEnabled(true);
      m_ui->gen_docset_CB->setEnabled(true);
      m_ui->gen_eclipse_CB->setEnabled(true);;
      m_ui->gen_qthelp_CB->setEnabled(true);

      valid_gen_chm();
      valid_gen_docset();
      valid_gen_eclipse();
      valid_gen_qthelp();

      m_ui->html_search_CB2->setEnabled(true);
      valid_html_search();

   } else {
      m_ui->html_output->setEnabled(false);
      m_ui->html_output_PB->setEnabled(false);
      m_ui->html_file_extension->setEnabled(false);
      m_ui->html_header->setEnabled(false);
      m_ui->html_header_PB->setEnabled(false);
      m_ui->html_footer->setEnabled(false);
      m_ui->html_footer_PB->setEnabled(false);
      m_ui->html_stylesheets->setEnabled(false);
      m_ui->html_stylesheets_PB->setEnabled(false);
      m_ui->html_extra_files->setEnabled(false);
      m_ui->html_extra_files_PB->setEnabled(false);
      m_ui->html_colorstyle_hue->setEnabled(false);
      m_ui->html_colorstyle_sat->setEnabled(false);
      m_ui->html_colorstyle_gamma->setEnabled(false);
      m_ui->html_timestamp_CB->setEnabled(false);
      m_ui->html_dynamic_sections_CB->setEnabled(false);
      m_ui->html_index_num_entries_SB->setEnabled(false);

      m_ui->disable_index_CB->setEnabled(false);
      m_ui->gen_treeview_CB->setEnabled(false);
      m_ui->treeview_width_SB->setEnabled(false);
      m_ui->enum_values_per_line_SB->setEnabled(false);
      m_ui->external_links_in_window_CB->setEnabled(false);

      m_ui->formula_fontsize_SB->setEnabled(false);
      m_ui->formula_transparent_CB->setEnabled(false);
      m_ui->ghostscript->setEnabled(false);
      m_ui->use_mathjax_CB->setEnabled(false);
      m_ui->mathjax_format_CM->setEnabled(false);
      m_ui->mathjax_relpath->setEnabled(false);
      m_ui->mathjax_extensions->setEnabled(false);
      m_ui->mathjax_extensions_PB->setEnabled(false);
      m_ui->mathjax_codefile->setEnabled(false);

      m_ui->gen_chm_CB->setEnabled(false);
      m_ui->gen_docset_CB->setEnabled(false);
      m_ui->gen_eclipse_CB->setEnabled(false);
      m_ui->gen_qthelp_CB->setEnabled(false);

      valid_gen_chm();
      valid_gen_docset();
      valid_gen_eclipse();
      valid_gen_qthelp();

      m_ui->html_search_CB2->setEnabled(false);
      valid_html_search();

   }
}

void MainWindow::valid_gen_chm()
{
   // this line can be redundant
   m_ui->html_chm_RB->setChecked(m_ui->gen_chm_CB->isChecked());

   if (m_ui->gen_chm_CB->isChecked() && m_ui->gen_html_CB2->isChecked()) {
      m_ui->chm_file->setEnabled(true);
      m_ui->chm_file_PB->setEnabled(true);
      m_ui->hhc_location->setEnabled(true);
      m_ui->hhc_location_PB->setEnabled(true);
      m_ui->gen_chi_CB->setEnabled(true);
      m_ui->chm_index_encoding->setEnabled(true);
      m_ui->binary_toc_CB->setEnabled(true);
      m_ui->toc_expanded_CB->setEnabled(true);

   } else {
      m_ui->chm_file->setEnabled(false);
      m_ui->chm_file_PB->setEnabled(false);
      m_ui->hhc_location->setEnabled(false);
      m_ui->hhc_location_PB->setEnabled(false);
      m_ui->gen_chi_CB->setEnabled(false);
      m_ui->chm_index_encoding->setEnabled(false);
      m_ui->binary_toc_CB->setEnabled(false);
      m_ui->toc_expanded_CB->setEnabled(false);

   }
}

void MainWindow::valid_gen_docset()
{
   if (m_ui->gen_docset_CB->isChecked() && m_ui->gen_html_CB2->isChecked()) {
      m_ui->docset_feedname->setEnabled(true);
      m_ui->docset_bundle_id->setEnabled(true);
      m_ui->docset_publisher_id->setEnabled(true);
      m_ui->docset_publisher_name->setEnabled(true);

   } else {
      m_ui->docset_feedname->setEnabled(false);
      m_ui->docset_bundle_id->setEnabled(false);
      m_ui->docset_publisher_id->setEnabled(false);
      m_ui->docset_publisher_name->setEnabled(false);
   }
}

void MainWindow::valid_gen_eclipse()
{
   if (m_ui->gen_eclipse_CB->isChecked() && m_ui->gen_html_CB2->isChecked()) {
      m_ui->eclipse_doc_id->setEnabled(true);

   } else {
      m_ui->eclipse_doc_id->setEnabled(false);

   }
}

void MainWindow::valid_gen_latex()
{
   // this line can be redundant
   m_ui->gen_latex_CB1->setChecked(m_ui->gen_latex_CB2->isChecked());

   if (m_ui->gen_latex_CB2->isChecked()) {
      m_ui->latex_output->setEnabled(true);
      m_ui->latex_output_PB->setEnabled(true);
      m_ui->latex_cmd_name->setEnabled(true);
      m_ui->latex_cmd_name_PB->setEnabled(true);
      m_ui->make_index_cmd_name->setEnabled(true);
      m_ui->make_index_cmd_name_PB->setEnabled(true);
      m_ui->latex_compact_CB->setEnabled(true);
      m_ui->latex_paper_type_CM->setEnabled(true);
      m_ui->latex_extra_packages->setEnabled(true);
      m_ui->latex_extra_packages_PB->setEnabled(true);
      m_ui->latex_header->setEnabled(true);
      m_ui->latex_header_PB->setEnabled(true);
      m_ui->latex_footer->setEnabled(true);
      m_ui->latex_footer_PB->setEnabled(true);
      m_ui->latex_extra_files->setEnabled(true);
      m_ui->latex_extra_files_PB->setEnabled(true);
      m_ui->latex_hyper_pdf_CB->setEnabled(true);
      m_ui->latex_pdf_CB->setEnabled(true);
      m_ui->latex_batch_mode_CB->setEnabled(true);
      m_ui->latex_hide_indices_CB->setEnabled(true);
      m_ui->latex_source_code_CB->setEnabled(true);
      m_ui->latex_bib_style->setEnabled(true);

   } else {
      m_ui->latex_output->setEnabled(false);
      m_ui->latex_output_PB->setEnabled(false);
      m_ui->latex_cmd_name->setEnabled(false);
      m_ui->latex_cmd_name_PB->setEnabled(false);
      m_ui->make_index_cmd_name->setEnabled(false);
      m_ui->make_index_cmd_name_PB->setEnabled(false);
      m_ui->latex_compact_CB->setEnabled(false);
      m_ui->latex_paper_type_CM->setEnabled(false);
      m_ui->latex_extra_packages->setEnabled(false);
      m_ui->latex_extra_packages_PB->setEnabled(false);
      m_ui->latex_header->setEnabled(false);
      m_ui->latex_header_PB->setEnabled(false);
      m_ui->latex_footer->setEnabled(false);
      m_ui->latex_footer_PB->setEnabled(false);
      m_ui->latex_extra_files->setEnabled(false);
      m_ui->latex_extra_files_PB->setEnabled(false);
      m_ui->latex_hyper_pdf_CB->setEnabled(false);
      m_ui->latex_pdf_CB->setEnabled(false);
      m_ui->latex_batch_mode_CB->setEnabled(false);
      m_ui->latex_hide_indices_CB->setEnabled(false);
      m_ui->latex_source_code_CB->setEnabled(false);
      m_ui->latex_bib_style->setEnabled(false);

   }
}

void MainWindow::valid_gen_man()
{
   // this line can be redundant
   m_ui->gen_man_CB1->setChecked(m_ui->gen_man_CB2->isChecked());

   if (m_ui->gen_man_CB2->isChecked()) {
      m_ui->man_output->setEnabled(true);
      m_ui->man_output_PB->setEnabled(true);
      m_ui->man_extension->setEnabled(true);
      m_ui->man_subdir->setEnabled(true);
      m_ui->man_links_CB->setEnabled(true);

   } else {
      m_ui->man_output->setEnabled(false);
      m_ui->man_output_PB->setEnabled(false);
      m_ui->man_extension->setEnabled(false);
      m_ui->man_subdir->setEnabled(false);
      m_ui->man_links_CB->setEnabled(false);
   }
}

void MainWindow::valid_gen_qthelp()
{
   if (m_ui->gen_qthelp_CB->isChecked() && m_ui->gen_html_CB2->isChecked()) {
      m_ui->qch_file->setEnabled(true);
      m_ui->qch_file_PB->setEnabled(true);
      m_ui->qhp_namespace->setEnabled(true);
      m_ui->qhp_virtual_folder->setEnabled(true);
      m_ui->qhp_cust_filter_name->setEnabled(true);
      m_ui->qhp_cust_attrib->setEnabled(true);
      m_ui->qhp_sect_attrib->setEnabled(true);
      m_ui->qthelp_gen_path->setEnabled(true);
      m_ui->qthelp_gen_path_PB->setEnabled(true);

   } else {
      m_ui->qch_file->setEnabled(false);
      m_ui->qch_file_PB->setEnabled(false);
      m_ui->qhp_namespace->setEnabled(false);
      m_ui->qhp_virtual_folder->setEnabled(false);
      m_ui->qhp_cust_filter_name->setEnabled(false);
      m_ui->qhp_cust_attrib->setEnabled(false);
      m_ui->qhp_sect_attrib->setEnabled(false);
      m_ui->qthelp_gen_path->setEnabled(false);
      m_ui->qthelp_gen_path_PB->setEnabled(false);
   }
}

void MainWindow::valid_gen_rtf()
{
   // this line can be redundant
   m_ui->gen_rtf_CB1->setChecked(m_ui->gen_rtf_CB2->isChecked());

   if (m_ui->gen_rtf_CB2->isChecked()) {
      m_ui->rtf_output->setEnabled(true);
      m_ui->rtf_output_PB->setEnabled(true);
      m_ui->rtf_compact_CB->setEnabled(true);
      m_ui->rtf_hyperlinks_CB->setEnabled(true);
      m_ui->rtf_stylesheet->setEnabled(true);
      m_ui->rtf_stylesheet_PB->setEnabled(true);
      m_ui->rtf_extension->setEnabled(true);
      m_ui->rtf_extension_PB->setEnabled(true);

   } else {
      m_ui->rtf_output->setEnabled(false);
      m_ui->rtf_output_PB->setEnabled(false);
      m_ui->rtf_compact_CB->setEnabled(false);
      m_ui->rtf_hyperlinks_CB->setEnabled(false);
      m_ui->rtf_stylesheet->setEnabled(false);
      m_ui->rtf_stylesheet_PB->setEnabled(false);
      m_ui->rtf_extension->setEnabled(false);
      m_ui->rtf_extension_PB->setEnabled(false);
   }
}

void MainWindow::valid_gen_xml()
{
   m_ui->gen_xml_CB1->setChecked(m_ui->gen_xml_CB2->isChecked());

   if (m_ui->gen_xml_CB2->isChecked()) {
      m_ui->xml_output->setEnabled(true);
      m_ui->xml_output_PB->setEnabled(true);
      m_ui->xml_program_listing_CB->setEnabled(true);

   } else {
      m_ui->xml_output->setEnabled(false);
      m_ui->xml_output_PB->setEnabled(false);
      m_ui->xml_program_listing_CB->setEnabled(false);
   }
}

void MainWindow::valid_gen_docbook()
{
   // this line can be redundant
   m_ui->gen_docbook_CB1->setChecked(m_ui->gen_docbook_CB2->isChecked());

   if (m_ui->gen_docbook_CB2->isChecked()) {
      m_ui->docbook_output->setEnabled(true);
      m_ui->docbook_output_PB->setEnabled(true);
      m_ui->docbook_program_listing_CB->setEnabled(true);

   } else {
      m_ui->docbook_output->setEnabled(false);
      m_ui->docbook_output_PB->setEnabled(false);
      m_ui->docbook_program_listing_CB->setEnabled(false);
   }
}

void MainWindow::valid_html_search()
{
   // this line can be redundant
   m_ui->html_search_CB1->setChecked(m_ui->html_search_CB2->isChecked());

   if (m_ui->html_search_CB2->isChecked()) {
      m_ui->server_based_search_CB->setEnabled(true);
      m_ui->external_search_CB->setEnabled(true);
      m_ui->search_engine_url->setEnabled(true);
      m_ui->search_data_file->setEnabled(true);
      m_ui->search_data_file_PB->setEnabled(true);
      m_ui->search_external_id->setEnabled(true);
      m_ui->search_mappings->setEnabled(true);
      m_ui->search_mappings_PB ->setEnabled(true);

   } else {
      m_ui->server_based_search_CB->setEnabled(false);
      m_ui->external_search_CB->setEnabled(false);
      m_ui->search_engine_url->setEnabled(false);
      m_ui->search_data_file->setEnabled(false);
      m_ui->search_data_file_PB->setEnabled(false);
      m_ui->search_external_id->setEnabled(false);
      m_ui->search_mappings->setEnabled(false);
      m_ui->search_mappings_PB ->setEnabled(false);
   }
}


// **
void MainWindow::valid_gen_html_1(bool checked)
{
   m_ui->gen_html_CB1->setChecked(checked);
   m_ui->gen_html_CB2->setChecked(checked);

   valid_gen_html();
}

void MainWindow::valid_gen_latex_1(bool checked)
{
   m_ui->gen_latex_CB1->setChecked(checked);
   m_ui->gen_latex_CB2->setChecked(checked);

   valid_gen_latex();
}

void MainWindow::valid_gen_docbook_1(bool checked)
{
   m_ui->gen_docbook_CB1->setChecked(checked);
   m_ui->gen_docbook_CB2->setChecked(checked);

   valid_gen_docbook();
}

void MainWindow::valid_gen_man_1(bool checked)
{
   m_ui->gen_man_CB1->setChecked(checked);
   m_ui->gen_man_CB2->setChecked(checked);

   valid_gen_man();
}

void MainWindow::valid_gen_rtf_1(bool checked)
{
   m_ui->gen_rtf_CB1->setChecked(checked);
   m_ui->gen_rtf_CB2->setChecked(checked);

   valid_gen_rtf();
}

void MainWindow::valid_gen_xml_1(bool checked)
{
   m_ui->gen_xml_CB1->setChecked(checked);
   m_ui->gen_xml_CB2->setChecked(checked);

   valid_gen_xml();
}

void MainWindow::validSet_html(QAbstractButton *button)
{
    if (button == m_ui->html_plain_RB) {
      // plain HTML

      m_ui->gen_chm_CB->setChecked(false);
      m_ui->gen_treeview_CB->setChecked(false);

   } else if (button == m_ui->html_nav_RB) {
      // with navigation tree

      m_ui->gen_chm_CB->setChecked(false);
      m_ui->gen_treeview_CB->setChecked(true);

   } else if (button == m_ui->html_chm_RB ) {
      // with compiled help

      m_ui->gen_chm_CB->setChecked(true);
      m_ui->gen_treeview_CB->setChecked(false);
   }

   valid_gen_chm();
}

void MainWindow::validGet_html()
{
   if (m_ui->gen_chm_CB->isChecked()) {
     // compiled help
     m_ui->html_chm_RB->setChecked(true);

   } else if (m_ui->gen_treeview_CB->isChecked()) {
     // navigation tree
     m_ui->html_nav_RB->setChecked(true);

   } else  {
     // plain HTML
     m_ui->html_plain_RB->setChecked(true);

   }
}

void MainWindow::validSet_latex(QAbstractButton *button)
{
   if (button == m_ui->latex_hyper_pdf_RB) {
      // hyperlinked PDF

      m_ui->latex_pdf_CB->setChecked(true);
      m_ui->latex_hyper_pdf_CB->setChecked(true);

   } else if (button == m_ui->latex_pdf_RB) {
      // pdf

      m_ui->latex_pdf_CB->setChecked(true);
      m_ui->latex_hyper_pdf_CB->setChecked(false);

   } else if (button == m_ui->latex_ps_RB) {
      // PostScript

      m_ui->latex_pdf_CB->setChecked(false);
      m_ui->latex_hyper_pdf_CB->setChecked(false);
   }
}

void MainWindow::validGet_latex()
{
   if (m_ui->latex_pdf_CB->isChecked()) {
     // postScript
     m_ui->latex_ps_RB->setChecked(true);

   } else if (m_ui->latex_hyper_pdf_CB->isChecked()) {
     // pdf
     m_ui->latex_pdf_RB->setChecked(true);

   } else  {
     // hyperlinked PDF
     m_ui->latex_hyper_pdf_RB->setChecked(true);

   }
}

void MainWindow::validSet_dot(QAbstractButton *button)
{
   if (button == m_ui->diagram_none_RB) {
      // no diagrams
      m_ui->have_dot_CB->setChecked(false);
      m_ui->class_diagrams_CB->setChecked(false);

   } else if (button == m_ui->diagram_built_in_RB) {
      // builtin diagrams
      m_ui->have_dot_CB->setChecked(false);
      m_ui->class_diagrams_CB->setChecked(true);

   } else if (button == m_ui->diagram_dot_RB) {
      // dot diagrams
      m_ui->have_dot_CB->setChecked(true);
      m_ui->class_diagrams_CB->setChecked(false);

   }
}

void MainWindow::validGet_dot()
{
   if (m_ui->have_dot_CB->isChecked()) {
     // dot
     m_ui->diagram_dot_RB->setChecked(true);

   } else if (m_ui->class_diagrams_CB->isChecked()) {
     // builtin diagrams
      m_ui->diagram_built_in_RB->setChecked(true);

   } else  {
     // no diagrams
     m_ui->diagram_none_RB->setChecked(true);

   }
}

static QString getFilePattens()
{
   QStringList list;
   list.append("*.as");
   list.append("*.c");
   list.append("*.cc");
   list.append("*.cpp");
   list.append("*.cxx");
   list.append("*.c++");
   list.append("*.cs");
   list.append("*.d");
   list.append("*.ddl");
   list.append("*.dox");
   list.append("*.for");
   list.append("*.f");
   list.append("*.f90");
   list.append("*.h");
   list.append("*.hh");
   list.append("*.hxx");
   list.append("*.hpp");
   list.append("*.h++");
   list.append("*.ii");
   list.append("*.ixx");
   list.append("*.ipp");
   list.append("*.i++");
   list.append("*.inc");
   list.append("*.inl");
   list.append("*.idl");
   list.append("*.java");
   list.append("*.js");
   list.append("*.m");
   list.append("*.md");
   list.append("*.mm");
   list.append("*.markdown");
   list.append("*.odl");
   list.append("*.php");
   list.append("*.php4");
   list.append("*.php5");
   list.append("*.phtml");
   list.append("*.py");
   list.append("*.qsf");
   list.append("*.tcl");
   list.append("*.ucf");

   QString retval = list.join(", ");

   return retval;
}


