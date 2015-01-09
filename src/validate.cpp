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

#include "mainwindow.h"

#include <QCheckBox>
#include <QStringList>

enum Validate{
   MODIFIED,
   DEFAULT
};

struct HelpData {
   QLabel *label;
   QString defValue;
   QString title;
   QString body;
};

static QMap<QWidget *, HelpData> m_bigMap;

static QString getBody(QWidget *name);
static QString getDefault(QWidget *name);
static QString getTitle(QWidget *name);

void MainWindow::clearAllFields()
{
   // tab 1
   m_ui->project_name->setText("");
   m_ui->project_brief->setText("");
   m_ui->project_number->setText("");

   m_project_iconFN = "";
   m_ui->output_dir->setText("");

   m_ui->gen_html_CB1->setChecked(false);
   m_ui->gen_latex_CB1->setChecked(false);
   m_ui->gen_rtf_CB1->setChecked(false);
   m_ui->gen_man_CB1->setChecked(false);
   m_ui->gen_xml_CB1->setChecked(false);
   m_ui->gen_docbook_CB1->setChecked(false);

/*
   m_ui->html_RB1->setChecked(true);
   m_ui->html_RB2->setChecked(false);
   m_ui->html_RB3->setChecked(false);
   m_ui->searchEnabled_CB->setChecked(false);

   m_ui->latex_RB1->setChecked(true);
   m_ui->latex_RB2->setChecked(false);
   m_ui->latex_RB3->setChecked(false);

   m_ui->diagram_RB1->setChecked(true);
   m_ui->diagram_RB2->setChecked(false);
   m_ui->diagram_RB3->setChecked(false);
*/

   m_ui->dot_class_CB->setChecked(false);
   m_ui->dot_collaboration_CB->setChecked(false);
   m_ui->dot_overall_CB->setChecked(false);
   m_ui->dot_include_CB->setChecked(false);
   m_ui->dot_included_by_CB->setChecked(false);
   m_ui->dot_call_CB->setChecked(false);
   m_ui->dot_called_by_CB->setChecked(false);

   // tab 2
   m_ui->project_encoding->setText("");
   m_ui->source_recursive_CB->setChecked(false);
   m_ui->show_used_files_CB->setChecked(false);
   m_ui->file_version_filter->setText("");
   m_ui->layout_file->setText("");
   m_ui->warn_logfile->setText("");
   m_ui->warn_forrmat->setText("");
   m_ui->input_encoding->setText("");
   m_ui->mdfile_mainpage->setText("");
   m_ui->perlmod_prefix->setText("");
   m_ui->generate_tagfile->setText("");
   m_ui->perl_path->setText("");

   m_ui->exclude_symlinks_CB->setChecked(false);

   m_ui->input_source->setPlainText("");
   m_ui->file_patterns->setPlainText("");
   m_ui->exclude_files->setPlainText("");
   m_ui->exclude_patterns->setPlainText("");
   m_ui->exclude_symbols->setPlainText("");

   // tab 3
   m_ui->gen_html_CB2->setChecked(false);
   m_ui->gen_latex_CB2->setChecked(false);
   m_ui->gen_rtf_CB2->setChecked(false);
   m_ui->gen_man_CB2->setChecked(false);
   m_ui->gen_xml_CB2->setChecked(false);
   m_ui->gen_docbook_CB2->setChecked(false);


   // initial default values
   int index = m_ui->output_langugae_CM->findText("English");
   m_ui->output_langugae_CM->setCurrentIndex(index);

   m_ui->tab_size->setValue(4);

   m_ui->html_colorstyle_hue->setValue(220);
   m_ui->html_colorstyle_sat->setValue(100);
   m_ui->html_colorstyle_gamma->setValue(80);
}

void MainWindow::setupLimits()
{
   QString temp = "Afrikaans, Arabic, Armenian, Brazilian, Catalan, Chinese, "
         "Chinese-Traditional, Croatian, Czech, Danish, Dutch, English, "
         "Esperanto, Farsi (Persian), Finnish, French, German, Greek, Hungarian, "
         "Indonesian, Italian, Japanese, Japanese-en (Japanese with English messages), "
         "Korean, Korean-en (Korean with English messages), Latvian, Lithuanian, "
         "Macedonian, Norwegian, Persian (Farsi), Polish, Portuguese, Romanian, Russian, "
         "Serbian, Serbian-Cyrillic, Slovak, Slovene, Spanish, Swedish, Turkish, Ukrainian, Vietnamese";

   QStringList lang = temp.split(", ");
   m_ui->output_langugae_CM->addItems(lang);

   m_ui->tab_size->setMinimum(1);
   m_ui->tab_size->setMaximum(16);

   // m_ui->look_up_cache->setMinimum(0);
   // m_ui->look_up_cache->setMaximum(9);

   // m_ui->max_init_lines->setMinimum(0);
   // m_ui->max_init_lines->setMaximum(10000);

   // file patterns

   // cols in alph index

   m_ui->html_colorstyle_hue->setMinimum(0);
   m_ui->html_colorstyle_hue->setMaximum(359);

   m_ui->html_colorstyle_sat->setMinimum(0);
   m_ui->html_colorstyle_sat->setMaximum(255);

   m_ui->html_colorstyle_gamma->setMinimum(0);
   m_ui->html_colorstyle_gamma->setMaximum(240);

}

void MainWindow::focusChanged(QWidget *old_Widget, QWidget *new_Widget)
{
   // entering
   setHelpText(new_Widget);

   // leaving
   QString defValue = getDefault(old_Widget);

   if (! defValue.isEmpty())

      if (old_Widget == m_ui->project_encoding) {

         if (m_ui->project_encoding->text() == defValue) {
            setHelpColor(DEFAULT, m_ui->label_t2_1);

          } else {
            setHelpColor(MODIFIED, m_ui->label_t2_1);

         }

      } else if (old_Widget == m_ui->output_langugae_CM) {

         if (m_ui->output_langugae_CM->currentText() == defValue) {
            setHelpColor(DEFAULT, m_ui->label_t2_4);

          } else {
            setHelpColor(MODIFIED, m_ui->label_t2_4);

         }

   } else {
      // all others

      QCheckBox *tempCB = dynamic_cast<QCheckBox *>(old_Widget);

      if (tempCB) {

         if (! tempCB->isChecked()) {
            setHelpColor(DEFAULT, m_ui->label_t2_2);

         } else {
            setHelpColor(MODIFIED, m_ui->label_t2_2);

         }
      }

      QLineEdit *tempLE = dynamic_cast<QLineEdit *>(old_Widget);

      if (tempLE) {

         if (tempLE->text().isEmpty()) {
            setHelpColor(DEFAULT, m_ui->label_190);

         } else {
            setHelpColor(MODIFIED, m_ui->label_190);

         }
      }
   }

}

void MainWindow::setHelpColor(int option, QWidget *label)
{
   if (option == DEFAULT) {
      QPalette temp = label->palette();
      temp.setColor(QPalette::WindowText, Qt::black);

      label->setPalette(temp);

    } else if (option == MODIFIED) {
      QPalette temp = label->palette();
      temp.setColor(QPalette::WindowText, Qt::red);

      label->setPalette(temp);
   }
}

void MainWindow::setHelpText(QWidget *name)
{
   QString title = getTitle(name);
   QString body  = getBody(name);;

   if (body.isEmpty())  {
      m_ui->help_text->setText("");

   } else {
      QString text = "<b>" + title + "</b><br><br>" + body;
      m_ui->help_text->setText(text);
   }
}

void MainWindow::createMap()
{
   HelpData temp;

   temp = HelpData{ m_ui->label_t2_1, "UTF-8", "Project Endcoding", "who cares about utf"};
   m_bigMap.insert(m_ui->project_encoding, temp);

   temp = HelpData{ m_ui->label_t2_4, "English", "Output Language", "learn another language"};
   m_bigMap.insert(m_ui->output_langugae_CM, temp);

}

static QString getBody(QWidget *name)
{
   QString retval;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->body;
   }

   return retval;
}

static QString getDefault(QWidget *name)
{
   QString retval;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->defValue;
   }

   return retval;
}

static QString getTitle(QWidget *name)
{
   QString retval;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->title;
   }

   return retval;
}
