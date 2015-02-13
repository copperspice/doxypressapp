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
static QMap<QString, QString> m_fullHelp;

static QString getBody(QWidget *name);
static QString getDefault(QWidget *name);
static QLabel *getLabel(QWidget *name);
static QString getTitle(QWidget *name);

static void configFullHelp();
static QString lookUpHelpBody(QString name);

void MainWindow::clearAllFields()
{   
   for (auto iter = m_bigMap.begin(); iter != m_bigMap.end(); ++iter) {

      QWidget *item = iter.key();

      QCheckBox *temp_CB = dynamic_cast<QCheckBox *>(item);

      if (temp_CB) {
         temp_CB->setChecked(false);

         QLabel *label = getLabel(temp_CB);         
         if (label) {
            setHelpColor(DEFAULT, label);
         }
      }

      QGroupBox *temp_GB = dynamic_cast<QGroupBox *>(item);

      if (temp_GB) {
         temp_GB->setChecked(false);

         // no label coloring
      }

      QComboBox *temp_CM = dynamic_cast<QComboBox *>(item);

      if (temp_CM) {
        temp_CM->setCurrentIndex(-1);     // broom not sure

        QLabel *label = getLabel(temp_CM);
        if (label) {
           setHelpColor(DEFAULT, label);
        }
      }

      QLineEdit *temp_LE = dynamic_cast<QLineEdit *>(item);

      if (temp_LE) {
         temp_LE->setText("");

         QLabel *label = getLabel(temp_LE);
         if (label) {
            setHelpColor(DEFAULT, label);
         }
      }

      QPlainTextEdit *temp_PT = dynamic_cast<QPlainTextEdit *>(item);

      if (temp_PT) {
         temp_PT->setPlainText("");

         QLabel *label = getLabel(temp_PT);
         if (label) {
            setHelpColor(DEFAULT, label);
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
            setHelpColor(DEFAULT, label);
         }
      }
   }

   // reset initial default values
   m_project_iconFN = "";
   getIcon("load");

   m_ui->optimize_cplus_CB->setChecked(true);
   m_ui->gen_html_CB1->setChecked(true);

   m_ui->html_colorstyle_hue->setValue(220);
   m_ui->html_colorstyle_sat->setValue(100);
   m_ui->html_colorstyle_gamma->setValue(80);

   m_ui->diagram_built_in_RB->setChecked(true);
   m_ui->class_diagrams_CB->setChecked(true);
   m_ui->dot_collaboration_CB1->setChecked(true);
   m_ui->dot_hierarchy_CB1->setChecked(true);
   m_ui->dot_include_CB1->setChecked(true);
   m_ui->dot_included_by_CB1->setChecked(true);

   // tab 2 - project
   m_ui->project_encoding->setText("UTF-8");

   int index = m_ui->output_language_CM->findText("English");
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

   // tab 2 - messages

   // tab 2 -input

   // tab 2 -index

   // tab 2 - preprocess

   // tab 2 - external

   // tab 2 - dot

   // tab 3 - all of them


   // final step
   setDuplicates();  
   validGet_html();
   validGet_latex();
}

void MainWindow::setDuplicates()
{
   m_ui->html_search_CB->setChecked(m_ui->search_engine_CB->isChecked());

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

void MainWindow::validSet_html(QAbstractButton *button)
{
   if (button == m_ui->html_plain_RB) {
      // plain HTML

      m_ui->gen_html_help_CB->setChecked(false);
      m_ui->gen_treeview_CB->setChecked(false);

   } else if (button == m_ui->html_nav_RB) {
      // with navigation tree

      m_ui->gen_html_help_CB->setChecked(false);
      m_ui->gen_treeview_CB->setChecked(true);

   } else if (button == m_ui->html_chm_RB ) {
      // with compiled help

      m_ui->gen_html_help_CB->setChecked(true);
      m_ui->gen_treeview_CB->setChecked(false);
   }
}

void MainWindow::validGet_html()
{
   if (m_ui->gen_html_help_CB->isChecked()) {
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

      m_ui->pdf_latex_CB->setChecked(true);
      m_ui->pdf_hyperlinks_CB->setChecked(true);

   } else if (button == m_ui->latex_pdf_RB) {
      // pdf

      m_ui->pdf_latex_CB->setChecked(true);
      m_ui->pdf_hyperlinks_CB->setChecked(false);

   } else if (button == m_ui->latex_ps_RB) {
      // PostScript

      m_ui->pdf_latex_CB->setChecked(false);
      m_ui->pdf_hyperlinks_CB->setChecked(false);
   }
}

void MainWindow::validGet_latex()
{
   if (m_ui->pdf_latex_CB->isChecked()) {
     // postScript
     m_ui->latex_ps_RB->setChecked(true);

   } else if (m_ui->pdf_hyperlinks_CB->isChecked()) {
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

void MainWindow::setupLimits()
{
   QStringList data;

   QString temp = "Afrikaans, Arabic, Armenian, Brazilian, Catalan, Chinese, "
         "Chinese-Traditional, Croatian, Czech, Danish, Dutch, English, "
         "Esperanto, Farsi (Persian), Finnish, French, German, Greek, Hungarian, "
         "Indonesian, Italian, Japanese, Japanese-en (Japanese with English messages), "
         "Korean, Korean-en (Korean with English messages), Latvian, Lithuanian, "
         "Macedonian, Norwegian, Persian (Farsi), Polish, Portuguese, Romanian, Russian, "
         "Serbian, Serbian-Cyrillic, Slovak, Slovene, Spanish, Swedish, Turkish, Ukrainian, Vietnamese";

   data = temp.split(", ");
   m_ui->output_language_CM->addItems(data);

   data.clear();
   data.append("HTML-CSS");
   data.append("NativeMML");
   data.append("SVG");
   m_ui->mathjax_format_CM->addItems(data);

   data.clear();
   data.append("png");
   data.append("jpg");
   data.append("gif");
   data.append("svg");
   m_ui->dot_image_format_CM->addItems(data);

   data.clear();
   data.append("a4");
   data.append("letter");
   data.append("legal");
   data.append("executive");
   m_ui->paper_type_CM->addItems(data);

   //
   m_ui->tab_size_SB->setMinimum(1);
   m_ui->tab_size_SB->setMaximum(16);

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
   // entering - adjust help message for new_Widget
   setHelpText(new_Widget);

   // leaving - adjust color on old_Widget
   QString defValue = getDefault(old_Widget);

   QCheckBox *temp_CB = dynamic_cast<QCheckBox *>(old_Widget);

   if (temp_CB) {
      QLabel *label = getLabel(temp_CB);

      if (label) {
         // broom - incorrect -->  need to add a way to test for the defValue

         if (! temp_CB->isChecked()) {
            setHelpColor(DEFAULT, label);

         } else {
            setHelpColor(MODIFIED, label);

         }
      }
   }

   // QGroupBox - do nothing

   // QCombo  - not sure

   QLineEdit *temp_LE = dynamic_cast<QLineEdit *>(old_Widget);

   if (temp_LE) {
      QLabel *label = getLabel(temp_LE);

      if (label) {

         if (temp_LE->text().trimmed() == defValue ) {
            setHelpColor(DEFAULT, label);

         } else {
            setHelpColor(MODIFIED, label);

         }
      }
   }

   QPlainTextEdit *temp_PT = dynamic_cast<QPlainTextEdit *>(old_Widget);

   if (temp_PT) {
      QLabel *label = getLabel(temp_PT);

      if (label) {

         if (temp_PT->toPlainText().trimmed() == defValue) {
            setHelpColor(DEFAULT, label);

         } else {
            setHelpColor(MODIFIED, label);

         }
      }
   }

   // QRadioButton - do nothing

   QSpinBox *temp_SB = dynamic_cast<QSpinBox *>(old_Widget);

   if (temp_SB) {
      QLabel *label = getLabel(temp_SB);

      if (label) {         
/*       QString defValue_SB = getDefault(old_Widget);

         if (temp_SB->value == defValue_SB) {
            setHelpColor(DEFAULT, label);

         } else {
            setHelpColor(MODIFIED, label);

         }
*/

      }
   }
}

void MainWindow::createMap()
{
   // 1  load help data
   configFullHelp();

   // 2  load bigMap
   HelpData temp;

   QList<QCheckBox *> temp_CB = this->findChildren<QCheckBox *>();

   for (auto ptr : temp_CB)  {
      temp = HelpData{nullptr, "", lookUpHelpBody(ptr->objectName()), ""};
      m_bigMap.insert(ptr, temp);
   }

   QList<QGroupBox *> temp_GB = this->findChildren<QGroupBox *>();

   for (auto ptr : temp_GB)  {
      temp = HelpData{nullptr, "", "", lookUpHelpBody(ptr->objectName()) };
      m_bigMap.insert(ptr, temp);
   }

   QList<QComboBox *> temp_CM = this->findChildren<QComboBox *>();

   for (auto ptr : temp_CM)  {
      temp = HelpData{nullptr, "", "", lookUpHelpBody(ptr->objectName()) };
      m_bigMap.insert(ptr, temp);
   }

   QList<QLineEdit *> temp_LE = this->findChildren<QLineEdit *>();

   for (auto ptr : temp_LE)  {
      temp = HelpData{nullptr, "", "", lookUpHelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);
   }

   QList<QPlainTextEdit *> temp_PT = this->findChildren<QPlainTextEdit *>();

   for (auto ptr : temp_PT)  {
      temp = HelpData{nullptr, "", "", lookUpHelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);
   }

   QList<QRadioButton *> temp_RB = this->findChildren<QRadioButton *>();

   for (auto ptr : temp_RB)  {
      temp = HelpData{nullptr, "", "", lookUpHelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);
   }

   QList<QSpinBox *> temp_SB = this->findChildren<QSpinBox *>();

   for (auto ptr : temp_SB)  {
      temp = HelpData{nullptr, "", "", lookUpHelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);
   }

   // get the buddy list
   QList<QLabel *> list_Labels = this->findChildren<QLabel *>();

   for (auto ptr : list_Labels)  {

      QWidget *buddy = ptr->buddy();

      if (buddy) {
         auto index = m_bigMap.find(buddy);

         if (index != m_bigMap.end()) {
            index->label = ptr;
            index->title = ptr->text().left( ptr->text().length() - 1);
         }
      }
   }

   //
   setupLimits();
   clearAllFields();
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

static QLabel *getLabel(QWidget *name)
{
   QLabel *retval = nullptr;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->label;
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
   QString body  = getBody(name);

   QString text = "<b>" + title + "</b><br><br>" + body;

   m_ui->help_t1->setText(text);
   m_ui->help_t2->setText(text);
   m_ui->help_t3->setText(text);
}

static QString lookUpHelpBody(QString name)
{
   QString retval;

   auto index = m_fullHelp.find(name.toUpper());

   if (index != m_fullHelp.end()) {
      retval = index.value();

   } else {
      retval = name + " (no help value)";

   }

   return retval;
}

static void configFullHelp()
{
   m_fullHelp.clear(); 

   m_fullHelp.insert("PROJECT_NAME",
            "The PROJECT_NAME tag is a single word (or a sequence of words surrounded by "
            "double-quotes, unless you are using DoxyPressApp) which should identify the "
            "project. This name is used in the "
            "title of most generated pages and in a few other places. ");

   m_fullHelp.insert("PROJECT_BRIEF",
            "Using the PROJECT_BRIEF tag one can provide an optional one line description "
            "for a project that appears at the top of each page and should give viewer a "
            "quick idea about the purpose of the project. Keep the description short.");

   m_fullHelp.insert("PROJECT_NUMBER",
            "The PROJECT_NUMBER tag can be used to enter a project or revision number. This "
            "can be usefull for archiving the generated documentation or if a version "
            "control system is used.");

   m_fullHelp.insert("PROJECT_ICON",
            "With the PROJECT_LOGO tag one can specify a logo or an icon that is included "
            "in the documentation. The maximum height of the logo should not exceed 55 "
            "pixels and the maximum width should not exceed 200 pixels. DoxyPress will copy "
            "the logo to the output directory.");

   m_fullHelp.insert("OUTPUT_DIR",
            "The OUTPUT_DIRECTORY tag is used to specify the (relative or absolute) path "
            "into which the generated documentation will be written. If a relative path is "
            "entered, it will be relative to the location where DoxyPress was started. If "
            "left blank the current directory will be used.");

   m_fullHelp.insert("CREATE_SUBDIRS",
            "If the CREATE_SUBDIRS tag is set to YES then DoxyPress will create 4096 sub- "
            "directories (in 2 levels) under the output directory of each output format and "
            "will distribute the generated files over these directories. Enabling this "
            "option can be useful when feeding DoxyPress a huge amount of source files, where "
            "putting all generated files in the same directory would otherwise causes "
            "performance problems for the file system. "
            "The default value is: NO.");

   m_fullHelp.insert("ALLOW_UNICODE_NAMES",
            "If the ALLOW_UNICODE_NAMES tag is set to YES, DoxyPress will allow non-ASCII "
            "characters to appear in the names of generated files. If set to NO, non-ASCII "
            "characters will be escaped, for example _xE3_x81_x84 will be used for Unicode "
            "U+3044. "
            "The default value is: NO.");

   m_fullHelp.insert("OUTPUT_LANGUAGE",
            "The OUTPUT_LANGUAGE tag is used to specify the language in which all "
            "documentation generated by DoxyPress is written. DoxyPress will use this "
            "information to generate output in the proper language. "
            "Possible values are: Afrikaans, Arabic, Armenian, Brazilian, Catalan, Chinese, "
            "Chinese-Traditional, Croatian, Czech, Danish, Dutch, English (United States), "
            "Esperanto, Farsi (Persian), Finnish, French, German, Greek, Hungarian, "
            "Indonesian, Italian, Japanese, Japanese-en (Japanese with English messages), "
            "Korean, Korean-en (Korean with English messages), Latvian, Lithuanian, "
            "Macedonian, Norwegian, Persian (Farsi), Polish, Portuguese, Romanian, Russian, "
            "Serbian, Serbian-Cyrillic, Slovak, Slovene, Spanish, Swedish, Turkish, "
            "Ukrainian and Vietnamese. "
            "The default value is: English.");

   m_fullHelp.insert("BRIEF_MEMBER_DESC",
            "If the BRIEF_MEMBER_DESC tag is set to YES, DoxyPress will include brief member "
            "descriptions after the members that are listed in the file and class "
            "documentation (similar to Javadoc). Set to NO to disable this. "
            "The default value is: YES.");

   m_fullHelp.insert("REPEAT_BRIEF",
            "If the REPEAT_BRIEF tag is set to YES, DoxyPress will prepend the brief "
            "description of a member or function before the detailed description "
            " "
            "Note: If both HIDE_UNDOC_MEMBERS and BRIEF_MEMBER_DESC are set to NO, the "
            "brief descriptions will be completely suppressed. "
            "The default value is: YES.");

   m_fullHelp.insert("ABBREVIATE_BRIEF",
            "This tag implements a quasi-intelligent brief description abbreviator that is "
            "used to form the text in various listings. Each string in this list, if found "
            "as the leading text of the brief description, will be stripped from the text "
            "and the result, after processing the whole list, is used as the annotated "
            "text. Otherwise, the brief description is used as-is. If left blank, the "
            "following values are used ($name is automatically replaced with the name of "
            "the entity):The $name class, The $name widget, The $name file, is, provides, "
            "specifies, contains, represents, a, an and the.");

   m_fullHelp.insert("ALWAYS_DETAILED_SEC",
            "If the ALWAYS_DETAILED_SEC and REPEAT_BRIEF tags are both set to YES then "
            "DoxyPress will generate a detailed section even if there is only a brief "
            "description. "
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_INHERITED_MEMB",
            "If the INLINE_INHERITED_MEMB tag is set to YES, DoxyPress will show all "
            "inherited members of a class in the documentation of that class as if those "
            "members were ordinary class members. Constructors, destructors and assignment "
            "operators of the base classes will not be shown. "
            "The default value is: NO.");

   m_fullHelp.insert("FULL_PATH_NAMES",
            "If the FULL_PATH_NAMES tag is set to YES, DoxyPress will prepend the full path "
            "before files name in the file list and in the header files. If set to NO the "
            "shortest path that makes the file name unique will be used "
            "The default value is: YES.");

   m_fullHelp.insert("STRIP_FROM_PATH",
            "The STRIP_FROM_PATH tag can be used to strip a user-defined part of the path. "
            "Stripping is only done if one of the specified strings matches the left-hand "
            "part of the path. The tag can be used to show relative paths in the file list. "
            "If left blank the directory from which DoxyPress is run is used as the path to "
            "strip. "
            " "
            "Specify an absolute path or a relative path, which "
            "will be relative from the directory where DoxyPress is started. "
            "This tag requires the tag FULL_PATH_NAMES is set to YES.");

   m_fullHelp.insert("STRIP_FROM_INC_PATH",
            "The STRIP_FROM_INC_PATH tag can be used to strip a user-defined part of the "
            "path mentioned in the documentation of a class, which tells the reader which "
            "header file to include in order to use a class. If left blank only the name of "
            "the header file containing the class definition is used. Otherwise one should "
            "specify the list of include paths that are normally passed to the compiler "
            "using the -I flag.");

   m_fullHelp.insert("SHORT_NAMES_CB",
            "If the SHORT_NAMES tag is set to YES, DoxyPress will generate much shorter (but "
            "less readable) file names. This can be useful is your file systems doesn't "
            "support long names like on DOS, Mac, or CD-ROM. "
            "The default value is: NO.");

   m_fullHelp.insert("JAVADOC_AUTOBRIEF",
            "If the JAVADOC_AUTOBRIEF tag is set to YES then DoxyPress will interpret the "
            "first line (until the first dot) of a Javadoc-style comment as the brief "
            "description. If set to NO, the Javadoc-style will behave just like regular Qt- "
            "style comments (thus requiring an explicit @brief command for a brief "
            "description.) "
            "The default value is: NO.");

   m_fullHelp.insert("QT_AUTOBRIEF",
            "If the QT_AUTOBRIEF tag is set to YES then DoxyPress will interpret the first "
            "line (until the first dot) of a Qt-style comment as the brief description. If "
            "set to NO, the Qt-style will behave just like regular Qt-style comments (thus "
            "requiring an explicit \\brief command for a brief description.) "
            "The default value is: NO.");

   m_fullHelp.insert("MULTILINE_CPP_IS_BRIEF",
            "The MULTILINE_CPP_IS_BRIEF tag can be set to YES to make DoxyPress treat a "
            "multi-line C++ special comment block (i.e. a block of //! or /// comments) as "
            "a brief description. This used to be the default behavior. The new default is "
            "to treat a multi-line C++ comment block as a detailed description. Set this "
            "tag to YES if you prefer the old behavior instead. "
            " "
            "Note that setting this tag to YES also means that rational rose comments are "
            "not recognized any more. "
            "The default value is: NO.");

   m_fullHelp.insert("INHERIT_DOCS",
            "If the INHERIT_DOCS tag is set to YES then an undocumented member inherits the "
            "documentation from any documented member that it re-implements. "
            "The default value is: YES.");

   m_fullHelp.insert("SEPARATE_MEMBER_PAGES",
            "If the SEPARATE_MEMBER_PAGES tag is set to YES then DoxyPress will produce a new "
            "page for each member. If set to NO, the documentation of a member will be part "
            "of the file/class/namespace that contains it. "
            "The default value is: NO.");

   m_fullHelp.insert("TAB_SIZE",
            "The TAB_SIZE tag can be used to set the number of spaces in a tab. DoxyPress "
            "uses this value to replace tabs by spaces in code fragments. "
            "Minimum value: 1, maximum value: 16, default value: 4.");

   m_fullHelp.insert("ALIASES",
            "This tag can be used to specify a number of aliases which act as commands in "
            "the documentation. An alias has the form: \n"
            "name=value \n"
            "Adding \"sideeffect=@par Side Effects:\\n\" "
            "defines the command \n"
            "\\sideeffect (or @sideeffect) \n"
            "in the documentation. This will result in a user-defined paragraph with a heading of \n"
            "\"Side Effects:\" ");

   m_fullHelp.insert("TCL_SUBST",
            "This tag can be used to specify a number of word-keyword mappings (TCL only). "
            "A mapping has the form \"name=value\". For example adding \"class=itcl::class\" "
            "will allow you to use the command class in the itcl::class meaning.");

   m_fullHelp.insert("OPTIMIZE_OUTPUT_FOR_C",
            "Set the OPTIMIZE_OUTPUT_FOR_C tag to YES if your project consists of C sources "
            "only. DoxyPress will then generate output that is more tailored for C. For "
            "instance, some of the names that are used will be different. The list of all "
            "members will be omitted, etc. "
            "The default value is: NO.");

   m_fullHelp.insert("OPTIMIZE_OUTPUT_JAVA",
            "Set the OPTIMIZE_OUTPUT_JAVA tag to YES if your project consists of Java or "
            "Python sources only. DoxyPress will then generate output that is more tailored "
            "for that language. For instance, namespaces will be presented as packages, "
            "qualified scopes will look different, etc. "
            "The default value is: NO.");

   m_fullHelp.insert("OPTIMIZE_FOR_FORTRAN",
            "Set the OPTIMIZE_FOR_FORTRAN tag to YES if your project consists of Fortran "
            "sources. DoxyPress will then generate output that is tailored for Fortran. "
            "The default value is: NO.");


   m_fullHelp.insert("PROJECT_ENCODING",
            "This tag specifies the encoding used for all characters in the config file "
            "that follow. The default is UTF-8 which is also the encoding used for all text "
            "before the first occurrence of this tag. DoxyPress uses libiconv (or the iconv "
            "built into libc) for the transcoding. See http://www.gnu.org/software/libiconv "
            "for the list of possible encodings. "
            "The default value is: UTF-8.");

   m_fullHelp.insert("EXTENSION_MAPPING",
            "DoxyPress selects the parser to use depending on the extension of the files it "
            "parses. With this tag you can assign which parser to use for a given "
            "extension. DoxyPress has a built-in mapping, but you can override or extend it "
            "using this tag. The format is ext=language, where ext is a file extension, and "
            "language is one of the parsers supported by DoxyPress: IDL, Java, Javascript, "
            "C#, C, C++, D, PHP, Objective-C, Python, Fortran (fixed format Fortran: "
            "FortranFixed, free formatted Fortran: FortranFree, unknown formatted Fortran: "
            "Fortran. In the later case the parser tries to guess whether the code is fixed "
            "or free formatted code, this is the default for Fortran type files), VHDL. For "
            "instance to make DoxyPress treat .inc files as Fortran files (default is PHP), "
            "and .f files as C (default is Fortran), use: inc=Fortran f=C. "
            " "
            "Note: For files without extension you can use no_extension as a placeholder. "
            " "
            "Note that for custom extensions you also need to set FILE_PATTERNS otherwise "
            "the files are not read by DoxyPress.");

   m_fullHelp.insert("MARKDOWN_SUPPORT",
            "If the MARKDOWN_SUPPORT tag is enabled then DoxyPress pre-processes all comments "
            "according to the Markdown format, which allows for more readable "
            "documentation. See http://daringfireball.net/projects/markdown/ for details. "
            "The output of markdown processing is further processed by DoxyPress, so you can "
            "mix DoxyPress, HTML, and XML commands with Markdown formatting. Disable only in "
            "case of backward compatibilities issues. "
            "The default value is: YES.");

   m_fullHelp.insert("AUTOLINK_SUPPORT",
            "When enabled DoxyPress tries to link words that correspond to documented "
            "classes, or namespaces to their corresponding documentation. Such a link can "
            "be prevented in individual cases by putting a % sign in front of the word or "
            "globally by setting AUTOLINK_SUPPORT to NO. "
            "The default value is: YES.");

   m_fullHelp.insert("BUILTIN_STL_SUPPORT",
            "If you use STL classes (i.e. std::string, std::vector, etc.) but do not want "
            "to include (a tag file for) the STL sources as input, then you should set this "
            "tag to YES in order to let DoxyPress match functions declarations and "
            "definitions whose arguments contain STL classes (e.g. func(std::string); "
            "versus func(std::string) {}). This also make the inheritance and collaboration "
            "diagrams that involve STL classes more complete and accurate. "
            "The default value is: NO.");

   m_fullHelp.insert("CPP_CLI_SUPPORT",
            "If you use Microsoft's C++/CLI language, you should set this option to YES to "
            "enable parsing support. "
            "The default value is: NO.");

   m_fullHelp.insert("SIP_SUPPORT",
            "Set the SIP_SUPPORT tag to YES if your project consists of sip (see: "
            "http://www.riverbankcomputing.co.uk/software/sip/intro) sources only. DoxyPress "
            "will parse them like normal C++ but will assume all classes use public instead "
            "of private inheritance when no explicit protection keyword is present. "
            "The default value is: NO.");

   m_fullHelp.insert("IDL_PROPERTY_SUPPORT",
            "For Microsoft's IDL there are propget and propput attributes to indicate "
            "getter and setter methods for a property. Setting this option to YES will make "
            "DoxyPress to replace the get and set methods by a property in the documentation. "
            "This will only work if the methods are indeed getting or setting a simple "
            "type. If this is not the case, or you want to show the methods anyway, you "
            "should set this option to NO. "
            "The default value is: YES.");

   m_fullHelp.insert("DISTRIBUTE_GROUP_DOC",
            "If member grouping is used in the documentation and the DISTRIBUTE_GROUP_DOC "
            "tag is set to YES then DoxyPress will reuse the documentation of the first "
            "member in the group (if any) for the other members of the group. By default "
            "all members of a group must be documented explicitly. "
            "The default value is: NO.");

   m_fullHelp.insert("SUBGROUPING",
            "Set the SUBGROUPING tag to YES to allow class member groups of the same type "
            "(for instance a group of public functions) to be put as a subgroup of that "
            "type (e.g. under the Public Functions section). Set it to NO to prevent "
            "subgrouping. Alternatively, this can be done per class using the "
            "\\nosubgrouping command. The default value is: YES.");

   m_fullHelp.insert("INLINE_GROUPED_CLASSES",
            "When the INLINE_GROUPED_CLASSES tag is set to YES, classes, structs and unions "
            "are shown inside the group in which they are included (e.g. using \\ingroup) "
            "instead of on a separate page (for HTML and Man pages) or section (for LaTeX "
            "and RTF). Note: this feature does not work in combination with SEPARATE_MEMBER_PAGES. "
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_SIMPLE_STRUCTS",
            "When the INLINE_SIMPLE_STRUCTS tag is set to YES, structs, classes, and unions "
            "with only public data fields or simple typedef fields will be shown inline in "
            "the documentation of the scope in which they are defined (i.e. file, "
            "namespace, or group documentation), provided this scope is documented. If set "
            "to NO, structs, classes, and unions are shown on a separate page (for HTML and "
            "Man pages) or section (for LaTeX and RTF). "
            "The default value is: NO.");

   m_fullHelp.insert("TYPEDEF_HIDES_STRUCT",
            "When TYPEDEF_HIDES_STRUCT tag is enabled, a typedef of a struct, union, or "
            "enum is documented as struct, union, or enum with the name of the typedef. So "
            "typedef struct TypeS {} TypeT, will appear in the documentation as a struct "
            "with name TypeT. When disabled the typedef will appear as a member of a file, "
            "namespace, or class. And the struct will be named TypeS. This can typically be "
            "useful for C code in case the coding convention dictates that all compound "
            "types are typedef'ed and only the typedef is referenced, never the tag name. "
            "The default value is: NO.");

   m_fullHelp.insert("LOOK_UP_CACHE",
            "The size of the symbol lookup cache can be set using LOOKUP_CACHE_SIZE. This "
            "cache is used to resolve symbols given their name and scope. Since this can be "
            "an expensive process and often the same symbol appears multiple times in the "
            "code, DoxyPress keeps a cache of pre-resolved symbols. If the cache is too small "
            "DoxyPress will become slower. If the cache is too large, memory is wasted. The "
            "cache size is given by this formula: 2^(16+LOOKUP_CACHE_SIZE). The valid range "
            "is 0..9, the default is 0, corresponding to a cache size of 2^16=65536 "
            "symbols. At the end of a run DoxyPress will report the cache usage and suggest "
            "the optimal cache size from a speed point of view. "
            "Minimum value: 0, maximum value: 9, default value: 0.");

   m_fullHelp.insert("EXTRACT_ALL",
            "If the EXTRACT_ALL tag is set to YES, DoxyPress will assume all entities in "
            "documentation are documented, even if no documentation was available. Private "
            "class members and static file members will be hidden unless the "
            "EXTRACT_PRIVATE respectively EXTRACT_STATIC tags are set to YES. "
            "Note: This will also disable the warnings about undocumented members that are "
            "normally produced when WARNINGS is set to YES. "
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_PRIVATE",
            "If the EXTRACT_PRIVATE tag is set to YES, all private members of a class will "
            "be included in the documentation. ""The default value is: NO.");

   m_fullHelp.insert("EXTRACT_PACKAGE",
            "If the EXTRACT_PACKAGE tag is set to YES, all members with package or internal "
            "scope will be included in the documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_STATIC",
            "If the EXTRACT_STATIC tag is set to YES, all static members of a file will be "
            "included in the documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_LOCAL_CLASSES",
            "If the EXTRACT_LOCAL_CLASSES tag is set to YES, classes (and structs) defined "
            "locally in source files will be included in the documentation. If set to NO, "
            "only classes defined in header files are included. Does not have any effect "
            "for Java sources. "
            "The default value is: YES.");

   m_fullHelp.insert("EXTRACT_LOCAL_METHODS",
            "This flag is only useful for Objective-C code. If set to YES, local methods, "
            "which are defined in the implementation section but not in the interface are "
            "included in the documentation. If set to NO, only methods in the interface are "
            "included. "
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_ANON_NSPACES",
            "If this flag is set to YES, the members of anonymous namespaces will be "
            "extracted and appear in the documentation as a namespace called "
            "'anonymous_namespace{file}', where file will be replaced with the base name of "
            "the file that contains the anonymous namespace. By default anonymous namespace "
            "are hidden. "
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_UNDOC_MEMBERS",
            "If the HIDE_UNDOC_MEMBERS tag is set to YES, DoxyPress will hide all "
            "undocumented members inside documented classes or files. If set to NO these "
            "members will be included in the various overviews, but no documentation "
            "section is generated. This option has no effect if EXTRACT_ALL is enabled. "
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_UNDOC_CLASSES",
            "If the HIDE_UNDOC_CLASSES tag is set to YES, DoxyPress will hide all "
            "undocumented classes that are normally visible in the class hierarchy. If set "
            "to NO, these classes will be included in the various overviews. This option "
            "has no effect if EXTRACT_ALL is enabled. "
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_FRIEND_COMPOUNDS",
            "If the HIDE_FRIEND_COMPOUNDS tag is set to YES, DoxyPress will hide all friend "
            "(class|struct|union) declarations. If set to NO, these declarations will be "
            "included in the documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_IN_BODY_DOCS",
            "If the HIDE_IN_BODY_DOCS tag is set to YES, DoxyPress will hide any "
            "documentation blocks found inside the body of a function. If set to NO, these "
            "blocks will be appended to the function's detailed documentation block. "
            "The default value is: NO.");

   m_fullHelp.insert("INTERNAL_DOCS",
            "The INTERNAL_DOCS tag determines if documentation that is typed after a "
            "\\internal command is included. If the tag is set to NO then the documentation "
            "will be excluded. Set it to YES to include the internal documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("CASE_SENSE_NAMES",
            "If the CASE_SENSE_NAMES tag is set to NO then DoxyPress will only generate file "
            "names in lower-case letters. If set to YES, upper-case letters are also "
            "allowed. This is useful if you have classes or files whose names only differ "
            "in case and if your file system supports case sensitive file names. Windows "
            "and Mac users are advised to set this option to NO. "
            "The default value is: system dependent.");

   m_fullHelp.insert("HIDE_SCOPE_NAMES",
            "If the HIDE_SCOPE_NAMES tag is set to NO then DoxyPress will show members with "
            "their full class and namespace scopes in the documentation. If set to YES, the "
            "scope will be hidden. "
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_COMPOUND_REFERENCE",
            "If the HIDE_COMPOUND_REFERENCE tag is set to NO (default) then DoxyPress will "
            "append additional text to a page's title, such as Class Reference. If set to "
            "YES the compound reference will be hidden. "
            "The default value is: NO.");

   m_fullHelp.insert("SHOW_INCLUDE_FILES",
            "If the SHOW_INCLUDE_FILES tag is set to YES then DoxyPress will put a list of "
            "the files that are included by a file in the documentation of that file. "
            "The default value is: YES.");

   m_fullHelp.insert("SHOW_GROUPED_MEMB_INC",
            "If the SHOW_GROUPED_MEMB_INC tag is set to YES then DoxyPress will add for each "
            "grouped member an include statement to the documentation, telling the reader "
            "which file to include in order to use the member. "
            "The default value is: NO.");

   m_fullHelp.insert("FORCE_LOCAL_INCLUDES",
            "If the FORCE_LOCAL_INCLUDES tag is set to YES then DoxyPress will list include "
            "files with double quotes in the documentation rather than with sharp brackets. "
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_INFO",
            "If the INLINE_INFO tag is set to YES then a tag [inline] is inserted in the "
            "documentation for inline members. "
            "The default value is: YES.");

   m_fullHelp.insert("SORT_MEMBER_DOCS",
            "If the SORT_MEMBER_DOCS tag is set to YES then DoxyPress will sort the "
            "(detailed) documentation of file and class members alphabetically by member "
            "name. If set to NO, the members will appear in declaration order. "
            "The default value is: YES.");

   m_fullHelp.insert("SORT_BRIEF_DOCS",
            "If the SORT_BRIEF_DOCS tag is set to YES then DoxyPress will sort the brief "
            "descriptions of file, namespace and class members alphabetically by member "
            "name. If set to NO, the members will appear in declaration order. Note that "
            "this will also influence the order of the classes in the class list. "
            "The default value is: NO.");

   m_fullHelp.insert("SORT_MEMBERS_CTORS_FIRST",
            "If the SORT_MEMBERS_CTORS_1ST tag is set to YES then DoxyPress will sort the "
            "(brief and detailed) documentation of class members so that constructors and "
            "destructors are listed first. If set to NO the constructors will appear in the "
            "respective orders defined by SORT_BRIEF_DOCS and SORT_MEMBER_DOCS. "
            "Note: If SORT_BRIEF_DOCS is set to NO this option is ignored for sorting brief "
            "member documentation. "
            "Note: If SORT_MEMBER_DOCS is set to NO this option is ignored for sorting "
            "detailed member documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("SORT_GROUP_NAMES",
            "If the SORT_GROUP_NAMES tag is set to YES then DoxyPress will sort the hierarchy "
            "of group names into alphabetical order. If set to NO the group names will "
            "appear in their defined order. "
            "The default value is: NO.");

   m_fullHelp.insert("SORT_BY_SCOPE_NAME",
            "If the SORT_BY_SCOPE_NAME tag is set to YES, the class list will be sorted by "
            "fully-qualified names, including namespaces. If set to NO, the class list will "
            "be sorted only by class name, not including the namespace part. "
            "Note: This option is not very useful if HIDE_SCOPE_NAMES is set to YES. "
            "Note: This option applies only to the class list, not to the alphabetical "
            "list. "
            "The default value is: NO.");

   m_fullHelp.insert("STRICT_PROTO_MATCHING",
            "If the STRICT_PROTO_MATCHING option is enabled and DoxyPress fails to do proper "
            "type resolution of all parameters of a function it will reject a match between "
            "the prototype and the implementation of a member function even if there is "
            "only one candidate or it is obvious which candidate to choose by doing a "
            "simple string match. By disabling STRICT_PROTO_MATCHING DoxyPress will still "
            "accept a match between prototype and implementation in such cases. "
            "The default value is: NO.");

   m_fullHelp.insert("GEN_TODO_LIST",
            "The GENERATE_TODOLIST tag can be used to enable (YES) or disable (NO) the todo "
            "list. This list is created by putting \\todo commands in the documentation. "
            "The default value is: YES.");

   m_fullHelp.insert("GEN_TEST_LIST",
            "The GENERATE_TESTLIST tag can be used to enable (YES) or disable (NO) the test "
            "list. This list is created by putting \\test commands in the documentation. "
            "The default value is: YES.");

   m_fullHelp.insert("GEN_BUG_LIST",
            "The GENERATE_BUGLIST tag can be used to enable (YES) or disable (NO) the bug "
            "list. This list is created by putting \\bug commands in the documentation. "
            "The default value is: YES.");

   m_fullHelp.insert("GENE_DEPRECATE_LIST",
            "The GENERATE_DEPRECATEDLIST tag can be used to enable (YES) or disable (NO) "
            "the deprecated list. This list is created by putting \\deprecated commands in "
            "the documentation. "
            "The default value is: YES.");

   m_fullHelp.insert("ENABLED_SECTIONS",
            "The ENABLED_SECTIONS tag can be used to enable conditional documentation "
            "sections, marked by \\if <section_label> ... \\endif and \\cond <section_label> "
            "... \\endcond blocks.");

   m_fullHelp.insert("MAX_INIT_LINES",
            "The MAX_INITIALIZER_LINES tag determines the maximum number of lines that the "
            "initial value of a variable or macro / define can have for it to appear in the "
            "documentation. If the initializer consists of more lines than specified here "
            "it will be hidden. Use a value of 0 to hide initializers completely. The "
            "appearance of the value of individual variables and macros / defines can be "
            "controlled using \\showinitializer or \\hideinitializer command in the "
            "documentation regardless of this setting. "
            "Minimum value: 0, maximum value: 10000, default value: 30.");

   m_fullHelp.insert("SHOW_USED_FILES",
            "Set the SHOW_USED_FILES tag to NO to disable the list of files generated at "
            "the bottom of the documentation of classes and structs. If set to YES, the "
            "list will mention the files that were used to generate the documentation. "
            "The default value is: YES.");

   m_fullHelp.insert("SHOW_FILES",
            "Set the SHOW_FILES tag to NO to disable the generation of the Files page. This "
            "will remove the Files entry from the Quick Index and from the Folder Tree View "
            "(if specified). "
            "The default value is: YES.");

   m_fullHelp.insert("SHOW_NAMESPACES",
            "Set the SHOW_NAMESPACES tag to NO to disable the generation of the Namespaces "
            "page. This will remove the Namespaces entry from the Quick Index and from the "
            "Folder Tree View (if specified). "
            "The default value is: YES.");

   m_fullHelp.insert("FILE_VERSION_FILTER",
            "The FILE_VERSION_FILTER tag can be used to specify a program or script that "
            "DoxyPress should invoke to get the current version for each file (typically from "
            "the version control system). DoxyPress will invoke the program by executing (via "
            "popen()) the command command input-file, where command is the value of the "
            "FILE_VERSION_FILTER tag, and input-file is the name of an input file provided "
            "by DoxyPress. Whatever the program writes to standard output is used as the file "
            "version. For an example see the documentation.");

   m_fullHelp.insert("LAYOUT_FILE",
            "The LAYOUT_FILE tag can be used to specify a layout file which will be parsed "
            "by DoxyPress. The layout file controls the global structure of the generated "
            "output files. To create the layout file run DoxyPress with the --l option. A "
            "file name can be specified after the option, if omitted DoxyPress_Layout.xml "
            "will be the default. "
            " "
            "Note that if you run DoxyPress from a directory containing a file called "
            "DoxyPressLayout.xml, DoxyPress will parse it automatically even if the LAYOUT_FILE "
            "tag is left empty.");

   m_fullHelp.insert("CITE_BIB_FILES",
            "The CITE_BIB_FILES tag can be used to specify one or more bib files containing "
            "the reference definitions. This must be a list of .bib files. The .bib "
            "extension is automatically appended if omitted. This requires the bibtex tool "
            "to be installed. See also http://en.wikipedia.org/wiki/BibTeX for more info. "
            "For LaTeX the style of the bibliography can be controlled using "
            "LATEX_BIB_STYLE. To use this feature you need bibtex and perl available in the "
            "search path. See also \\cite for info how to create references.");

   m_fullHelp.insert("QUIET",
            "The QUIET tag can be used to turn on/off the messages that are generated to "
            "standard output by DoxyPress. If QUIET is set to YES this implies that the "
            "messages are off. "
            "The default value is: NO.");

   m_fullHelp.insert("WARNINGS",
            "The WARNINGS tag can be used to turn on/off the warning messages that are "
            "generated to standard error (stderr) by DoxyPress. If WARNINGS is set to YES "
            "this implies that the warnings are on. "
            " "
            "Tip: Turn warnings on while writing the documentation. "
            "The default value is: YES.");

   m_fullHelp.insert("WARN_IF_UNDOCUMENTED",
            "If the WARN_IF_UNDOCUMENTED tag is set to YES then DoxyPress will generate "
            "warnings for undocumented members. If EXTRACT_ALL is set to YES then this flag "
            "will automatically be disabled. "
            "The default value is: YES.");

   m_fullHelp.insert("WARN_IF_DOC_ERROR",
            "If the WARN_IF_DOC_ERROR tag is set to YES, DoxyPress will generate warnings for "
            "potential errors in the documentation, such as not documenting some parameters "
            "in a documented function, or documenting parameters that don't exist or using "
            "markup commands wrongly. "
            "The default value is: YES.");

   m_fullHelp.insert("WARN_NO_PARAMDOC",
            "This WARN_NO_PARAMDOC option can be enabled to get warnings for functions that "
            "are documented, but have no documentation for their parameters or return "
            "value. If set to NO, DoxyPress will only warn about wrong or incomplete "
            "parameter documentation, but not about the absence of documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("WARN_FORMAT",
            "The WARN_FORMAT tag determines the format of the warning messages that DoxyPress "
            "can produce. The string should contain the $file, $line, and $text tags, which "
            "will be replaced by the file and line number from which the warning originated "
            "and the warning text. Optionally the format may contain $version, which will "
            "be replaced by the version of the file (if it could be obtained via "
            "FILE_VERSION_FILTER) "
            "The default value is: $file:$line: $text.");

   m_fullHelp.insert("WARN_LOGFILE",
            "The WARN_LOGFILE tag can be used to specify a file to which warning and error "
            "messages should be written. If left blank the output is written to standard "
            "error (stderr).");

   m_fullHelp.insert("INPUT_SOURCE",
            "The INPUT tag is used to specify the files and/or directories that contain "
            "documented source files. You may enter file names like myfile.cpp or "
            "directories like /usr/src/myproject. Separate the files or directories with "
            "spaces. "
            "Note: If this tag is empty the current directory is searched.");

   m_fullHelp.insert("INPUT_ENCODING",
            "This tag can be used to specify the character encoding of the source files "
            "that DoxyPress parses. Internally DoxyPress uses the UTF-8 encoding. DoxyPress uses "
            "libiconv (or the iconv built into libc) for the transcoding. See the libiconv "
            "documentation (see: http://www.gnu.org/software/libiconv) for the list of "
            "possible encodings. "
            "The default value is: UTF-8.");

   m_fullHelp.insert("FILE_PATTERNS",
            "If the value of the INPUT tag contains directories, you can use the "
            "FILE_PATTERNS tag to specify one or more wildcard patterns (like *.cpp and "
            "*.h) to filter out the source-files in the directories. If left blank the "
            "following patterns are tested:*.c, *.cc, *.cxx, *.cpp, *.c++, *.java, *.ii, "
            "*.ixx, *.ipp, *.i++, *.inl, *.idl, *.ddl, *.odl, *.h, *.hh, *.hxx, *.hpp, "
            "*.h++, *.cs, *.d, *.php, *.php4, *.php5, *.phtml, *.inc, *.m, *.markdown, "
            "*.md, *.mm, *.dox, *.py, *.f90, *.f, *.for, *.tcl, *.vhd, *.vhdl, *.ucf, "
            "*.qsf, *.as and *.js.");

   m_fullHelp.insert("RECURSIVE",
            "The RECURSIVE tag can be used to specify whether or not subdirectories should "
            "be searched for input files as well. "
            "The default value is: NO.");

   m_fullHelp.insert("EXCLUDE_FILES",
            "The EXCLUDE tag can be used to specify files and/or directories that should be "
            "excluded from the INPUT source files. This way you can easily exclude a "
            "subdirectory from a directory tree whose root is specified with the INPUT tag. "
            " "
            "Note that relative paths are relative to the directory from which DoxyPress is "
            "run.");

   m_fullHelp.insert("EXCLUDE_SYMLINKS",
            "The EXCLUDE_SYMLINKS tag can be used to select whether or not files or "
            "directories that are symbolic links (a Unix file system feature) are excluded "
            "from the input. "
            "The default value is: NO.");

   m_fullHelp.insert("EXCLUDE_PATTERNS",
            "If the value of the INPUT tag contains directories, you can use the "
            "EXCLUDE_PATTERNS tag to specify one or more wildcard patterns to exclude "
            "certain files from those directories. "
            " "
            "Note that the wildcards are matched against the file with absolute path, so to "
            "exclude all test directories for example use the pattern */test/*");

   m_fullHelp.insert("EXCLUDE_SYMBOLS",
            "The EXCLUDE_SYMBOLS tag can be used to specify one or more symbol names "
            "(namespaces, classes, functions, etc.) that should be excluded from the "
            "output. The symbol name can be a fully qualified name, a word, or if the "
            "wildcard * is used, a substring. Examples: ANamespace, AClass, "
            "AClass::ANamespace, ANamespace::*Test "
            " "
            "Note that the wildcards are matched against the file with absolute path, so to "
            "exclude all test directories use the pattern */test/*");

   m_fullHelp.insert("EXAMPLE_SOURCE",
            "Used  to specify one or more files or directories "
            "which contain example code fragments that are included (see the \\include command).");

   m_fullHelp.insert("EXAMPLE_PATTERNS",
            "If the value of the EXAMPLE_PATH tag contains directories, you can use the "
            "EXAMPLE_PATTERNS tag to specify one or more wildcard pattern (like *.cpp and "
            "*.h) to filter out the source-files in the directories. If left blank all "
            "files are included.");

   m_fullHelp.insert("EXAMPLE_RECURSIVE",
            "If the EXAMPLE_RECURSIVE tag is set to YES then subdirectories will be "
            "searched for input files to be used with the \\include or \\dontinclude commands "
            "irrespective of the value of the RECURSIVE tag. "
            "The default value is: NO.");

   m_fullHelp.insert("IMAGE_PATH",
            "The IMAGE_PATH tag can be used to specify one or more files or directories "
            "that contain images that are to be included in the documentation (see the "
            "\\image command).");

   m_fullHelp.insert("INPUT_FILTER",
            "The INPUT_FILTER tag can be used to specify a program that DoxyPress should "
            "invoke to filter for each input file. DoxyPress will invoke the filter program "
            "by executing (via popen()) the command: "
            " "
            "<filter> <input-file> "
            " "
            "where <filter> is the value of the INPUT_FILTER tag, and <input-file> is the "
            "name of an input file. DoxyPress will then use the output that the filter "
            "program writes to standard output. If FILTER_PATTERNS is specified, this tag "
            "will be ignored. "
            " "
            "Note that the filter must not add or remove lines; it is applied before the "
            "code is scanned, but not when the output code is generated. If lines are added "
            "or removed, the anchors will not be placed correctly.");

   m_fullHelp.insert("FILTER_PATTERNS",
            "The FILTER_PATTERNS tag can be used to specify filters on a per file pattern "
            "basis. DoxyPress will compare the file name with each pattern and apply the "
            "filter if there is a match. The filters are a list of the form: pattern=filter "
            "(like *.cpp=my_cpp_filter). See INPUT_FILTER for further information on how "
            "filters are used. If the FILTER_PATTERNS tag is empty or if none of the "
            "patterns match the file name, INPUT_FILTER is applied.");

   m_fullHelp.insert("FILTER_SOURCE_FILES",
            "If the FILTER_SOURCE_FILES tag is set to YES, the input filter (if set using "
            "INPUT_FILTER) will also be used to filter the input files that are used for "
            "producing the source files to browse (i.e. when SOURCE_BROWSER is set to YES). "
            "The default value is: NO.");

   m_fullHelp.insert("FILTER_SOURCE_PATTERNS",
            "The FILTER_SOURCE_PATTERNS tag can be used to specify source filters per file "
            "pattern. A pattern will override the setting for FILTER_PATTERN (if any) and "
            "it is also possible to disable source filtering for a specific pattern using "
            "*.ext= (so without naming a filter). "
            "This tag requires that the tag FILTER_SOURCE_FILES is set to YES.");

   m_fullHelp.insert("MDFILE_MAINPAGE",
            "If the USE_MDFILE_AS_MAINPAGE tag refers to the name of a markdown file that "
            "is part of the input, its contents will be placed on the main page "
            "(index.html). This can be useful if you have a project on for instance GitHub "
            "and want to reuse the introduction page also for the DoxyPress output.");

   m_fullHelp.insert("SOURCE_BROWSER",
            "If the SOURCE_BROWSER tag is set to YES then a list of source files will be "
            "generated. Documented entities will be cross-referenced with these sources. "
            " "
            "Note: To get rid of all source code in the generated output, make sure that "
            "also VERBATIM_HEADERS is set to NO. "
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_SOURCES",
            "Setting the INLINE_SOURCES tag to YES will include the body of functions, "
            "classes and enums directly into the documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("STRIP_CODE_COMMENTS",
            "Setting the STRIP_CODE_COMMENTS tag to YES will instruct DoxyPress to hide any "
            "special comment blocks from generated source code fragments. Normal C, C++ and "
            "Fortran comments will always remain visible. "
            "The default value is: YES.");

   m_fullHelp.insert("REFERENCED_BY_RELATION",
            "If the REFERENCED_BY_RELATION tag is set to YES then for each documented "
            "function all documented functions referencing it will be listed. "
            "The default value is: NO.");

   m_fullHelp.insert("REFERENCES_RELATION",
            "If the REFERENCES_RELATION tag is set to YES then for each documented function "
            "all documented entities called/used by that function will be listed. "
            "The default value is: NO.");

   m_fullHelp.insert("REFERENCES_LINK_SOURCE",
            "If the REFERENCES_LINK_SOURCE tag is set to YES and SOURCE_BROWSER tag is set "
            "to YES then the hyperlinks from functions in REFERENCES_RELATION and "
            "REFERENCED_BY_RELATION lists will link to the source code. Otherwise they will "
            "link to the documentation. "
            "The default value is: YES.");

   m_fullHelp.insert("SOURCE_TOOLTIPS",
            "If SOURCE_TOOLTIPS is enabled (the default) then hovering a hyperlink in the "
            "source code will show a tooltip with additional information such as prototype, "
            "brief description and links to the definition and documentation. Since this "
            "will make the HTML file larger and loading of large files a bit slower, you "
            "can opt to disable this feature. "
            "The default value is: YES. "
            "This tag requires that the tag SOURCE_BROWSER is set to YES.");

   m_fullHelp.insert("USE_HTAGS",
            "If the USE_HTAGS tag is set to YES then the references to source code will "
            "point to the HTML generated by the htags(1) tool instead of DoxyPress built-in "
            "source browser. The htags tool is part of GNU's global source tagging system "
            "(see http://www.gnu.org/software/global/global.html). You will need version "
            "4.8.6 or higher. "
            " "
            "To use it do the following: "
            "- Install the latest version of global "
            "- Enable SOURCE_BROWSER and USE_HTAGS in the config file "
            "- Make sure the INPUT points to the root of the source tree "
            "- Run DoxyPress as normal "
            " "
            "DoxyPress will invoke htags (and that will in turn invoke gtags), so these "
            "tools must be available from the command line (i.e. in the search path). "
            " "
            "The result: instead of the source browser generated by DoxyPress, the links to "
            "source code will now point to the output of htags. "
            "The default value is: NO. "
            "This tag requires that the tag SOURCE_BROWSER is set to YES.");

   m_fullHelp.insert("VERBATIM_HEADERS",
            "If the VERBATIM_HEADERS tag is set the YES then DoxyPress will generate a "
            "verbatim copy of the header file for each class for which an include is "
            "specified. Set to NO to disable this. "
            "See also: Section \\class. "
            "The default value is: YES.");

   m_fullHelp.insert("CLANG_ASSISTED_PARSING",
            "If the CLANG_ASSISTED_PARSING tag is set to YES then DoxyPress will use the "
            "clang parser (see: http://clang.llvm.org/) for more accurate parsing at the "
            "cost of reduced performance. This can be particularly helpful with template "
            "rich C++ code for which DoxyPress's built-in parser lacks the necessary type "
            "information. "
            "Note: The availability of this option depends on whether or not DoxyPress was "
            "compiled with the --with-libclang option. "
            "The default value is: NO.");

   m_fullHelp.insert("CLANG_OPTIONS",
            "If clang assisted parsing is enabled you can provide the compiler with command "
            "line options that you would normally use when invoking the compiler. Note that "
            "the include paths will already be set by DoxyPress for the files and directories "
            "specified with INPUT and INCLUDE_PATH. "
            "This tag requires that the tag CLANG_ASSISTED_PARSING is set to YES.");

   m_fullHelp.insert("ALPHABETICAL_INDEX",
            "If the ALPHABETICAL_INDEX tag is set to YES, an alphabetical index of all "
            "compounds will be generated. Enable this if the project contains a lot of "
            "classes, structs, unions or interfaces. "
            "The default value is: YES.");

   m_fullHelp.insert("COLS_IN_ALPHA_INDEX",
            "The COLS_IN_ALPHA_INDEX tag can be used to specify the number of columns in "
            "which the alphabetical index list will be split. "
            "Minimum value: 1, maximum value: 20, default value: 5. "
            "This tag requires that the tag ALPHABETICAL_INDEX is set to YES.");

   m_fullHelp.insert("IGNORE_PREFIX",
            "In case all classes in a project start with a common prefix, all classes will "
            "be put under the same header in the alphabetical index. The IGNORE_PREFIX tag "
            "can be used to specify a prefix (or a list of prefixes) that should be ignored "
            "while generating the index headers. "
            "This tag requires that the tag ALPHABETICAL_INDEX is set to YES.");

   m_fullHelp.insert("GENERATE_HTML",
            "If the GENERATE_HTML tag is set to YES, DoxyPress will generate HTML output "
            "The default value is: YES.");

   m_fullHelp.insert("HTML_OUTPUT",
            "The HTML_OUTPUT tag is used to specify where the HTML docs will be put. If a "
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of "
            "it. "
            "The default directory is: html. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_FILE_EXTENSION",
            "The HTML_FILE_EXTENSION tag can be used to specify the file extension for each "
            "generated HTML page (for example: .htm, .php, .asp). "
            "The default value is: .html. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_HEADER",
            "The HTML_HEADER tag can be used to specify a user-defined HTML header file for "
            "each generated HTML page. If the tag is left blank DoxyPress will generate a "
            "standard header. "
            " "
            "To get valid HTML the header file that includes any scripts and style sheets "
            "that DoxyPress needs, which is dependent on the configuration options used (e.g. "
            "the setting GENERATE_TREEVIEW). It is highly recommended to start with a "
            "default header using "
            "DoxyPress -w html new_header.html new_footer.html new_stylesheet.css "
            "YourConfigFile "
            "and then modify the file new_header.html. See also section \"DoxyPress usage\" "
            "for information on how to generate the default header that DoxyPress normally "
            "uses. "
            "Note: The header is subject to change so you typically have to regenerate the "
            "default header when upgrading to a newer version of DoxyPress. For a description "
            "of the possible markers and block names see the documentation. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_FOOTER",
            "The HTML_FOOTER tag can be used to specify a user-defined HTML footer for each "
            "generated HTML page. If the tag is left blank DoxyPress will generate a standard "
            "footer. See HTML_HEADER for more information on how to generate a default "
            "footer and what special commands can be used inside the footer. See also "
            "section \"DoxyPress usage\" for information on how to generate the default footer "
            "that DoxyPress normally uses. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_STYLESHEET",
            "The HTML_STYLESHEET tag can be used to specify a user-defined cascading style "
            "sheet that is used by each HTML page. It can be used to fine-tune the look of "
            "the HTML output. If left blank DoxyPress will generate a default style sheet. "
            "See also section \"DoxyPress usage\" for information on how to generate the style "
            "sheet that DoxyPress normally uses. "
            "Note: It is recommended to use HTML_EXTRA_STYLESHEET instead of this tag, as "
            "it is more robust and this tag (HTML_STYLESHEET) will in the future become "
            "obsolete. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_EXTRA_STYLESHEET",
            "The HTML_EXTRA_STYLESHEET tag can be used to specify additional user-defined "
            "cascading style sheets that are included after the standard style sheets "
            "created by DoxyPress. Using this option one can overrule certain style aspects. "
            "This is preferred over using HTML_STYLESHEET since it does not replace the "
            "standard style sheet and is therefore more robust against future updates. "
            "DoxyPress will copy the style sheet files to the output directory. "
            "Note: The order of the extra stylesheet files is of importance (e.g. the last "
            "stylesheet in the list overrules the setting of the previous ones in the "
            "list). For an example see the documentation. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_EXTRA_FILES",
            "The HTML_EXTRA_FILES tag can be used to specify one or more extra images or "
            "other source files which should be copied to the HTML output directory. Note "
            "that these files will be copied to the base HTML output directory. Use the "
            "$relpath^ marker in the HTML_HEADER and/or HTML_FOOTER files to load these "
            "files. In the HTML_STYLESHEET file, use the file name only. Also note that the "
            "files will be copied as-is; there are no commands or markers available. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_COLORSTYLE_HUE",
            "The HTML_COLORSTYLE_HUE tag controls the color of the HTML output. DoxyPress "
            "will adjust the colors in the stylesheet and background images according to "
            "this color. Hue is specified as an angle on a colorwheel, see "
            "http://en.wikipedia.org/wiki/Hue for more information. For instance the value "
            "0 represents red, 60 is yellow, 120 is green, 180 is cyan, 240 is blue, 300 "
            "purple, and 360 is red again. "
            "Minimum value: 0, maximum value: 359, default value: 220. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_COLORSTYLE_SAT",
            "The HTML_COLORSTYLE_SAT tag controls the purity (or saturation) of the colors "
            "in the HTML output. For a value of 0 the output will use grayscales only. A "
            "value of 255 will produce the most vivid colors. "
            "Minimum value: 0, maximum value: 255, default value: 100. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_COLORSTYLE_GAMMA",
            "The HTML_COLORSTYLE_GAMMA tag controls the gamma correction applied to the "
            "luminance component of the colors in the HTML output. Values below 100 "
            "gradually make the output lighter, whereas values above 100 make the output "
            "darker. The value divided by 100 is the actual gamma applied, so 80 represents "
            "a gamma of 0.8, The value 220 represents a gamma of 2.2, and 100 does not "
            "change the gamma. "
            "Minimum value: 40, maximum value: 240, default value: 80. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_TIMESTAMP",
            "If the HTML_TIMESTAMP tag is set to YES then the footer of each generated HTML "
            "page will contain the date and time when the page was generated. Setting this "
            "to NO can help when comparing the output of multiple runs. "
            "The default value is: YES. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_DYNAMIC_SECTIONS",
            "If the HTML_DYNAMIC_SECTIONS tag is set to YES then the generated HTML "
            "documentation will contain sections that can be hidden and shown after the "
            "page has loaded. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_INDEX_NUM_ENTRIES",
            "With HTML_INDEX_NUM_ENTRIES one can control the preferred number of entries "
            "shown in the various tree structured indices initially; the user can expand "
            "and collapse entries dynamically later on. DoxyPress will expand the tree to "
            "such a level that at most the specified number of entries are visible (unless "
            "a fully collapsed tree already exceeds this amount). So setting the number of "
            "entries 1 will produce a full collapsed tree by default. 0 is a special value "
            "representing an infinite number of entries and will result in a full expanded "
            "tree by default. "
            "Minimum value: 0, maximum value: 9999, default value: 100. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("GENERATE_DOCSET",
            "If the GENERATE_DOCSET tag is set to YES, additional index files will be "
            "generated that can be used as input for Apple's Xcode 3 integrated development "
            "environment (see: http://developer.apple.com/tools/xcode/), introduced with "
            "OSX 10.5 (Leopard). To create a documentation set, DoxyPress will generate a "
            "Makefile in the HTML output directory. Running make will produce the docset in "
            "that directory and running make install will install the docset in "
            "~/Library/Developer/Shared/Documentation/DocSets so that Xcode will find it at "
            "startup. See http://developer.apple.com/tools/creatingdocsetswithDoxyPress.html "
            "for more information. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("DOCSET_FEEDNAME",
            "This tag determines the name of the docset feed. A documentation feed provides "
            "an umbrella under which multiple documentation sets from a single provider "
            "(such as a company or product suite) can be grouped. "
            "The default value is: DoxyPress generated docs. "
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("DOCSET_BUNDLE_ID",
            "This tag specifies a string that should uniquely identify the documentation "
            "set bundle. This should be a reverse domain-name style string, e.g. "
            "com.mycompany.MyDocSet. DoxyPress will append .docset to the name. "
            "The default value is: org.DoxyPress.Project. "
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("DOCSET_PUBLISHER_ID",
            "The DOCSET_PUBLISHER_ID tag specifies a string that should uniquely identify "
            "the documentation publisher. This should be a reverse domain-name style "
            "string, e.g. com.mycompany.MyDocSet.documentation. "
            "The default value is: org.DoxyPress.Publisher. "
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("DOCSET_PUBLISHER_NAME",
            "The DOCSET_PUBLISHER_NAME tag identifies the documentation publisher. "
            "The default value is: Publisher. "
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("GENERATE_HTMLHELP",
            "If the GENERATE_HTMLHELP tag is set to YES then DoxyPress generates three "
            "additional HTML index files: index.hhp, index.hhc, and index.hhk. The "
            "index.hhp is a project file that can be read by Microsoft's HTML Help Workshop "
            "(see: http://www.microsoft.com/en-us/download/details.aspx?id=21138) on "
            "Windows. "
            " "
            "The HTML Help Workshop contains a compiler that can convert all HTML output "
            "generated by DoxyPress into a single compiled HTML file (.chm). Compiled HTML "
            "files are now used as the Windows 98 help format, and will replace the old "
            "Windows help format (.hlp) on all Windows platforms in the future. Compressed "
            "HTML files also contain an index, a table of contents, and you can search for "
            "words in the documentation. The HTML workshop also contains a viewer for "
            "compressed HTML files. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("CHM_FILE",
            "The CHM_FILE tag can be used to specify the file name of the resulting .chm "
            "file. You can add a path in front of the file if the result should not be "
            "written to the html output directory. "
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("HHC_LOCATION",
            "The HHC_LOCATION tag can be used to specify the location (absolute path "
            "including file name) of the HTML help compiler (hhc.exe). If non-empty, "
            "DoxyPress will try to run the HTML help compiler on the generated index.hhp. "
            "The file has to be specified with full path. "
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("GENERATE_CHI",
            "The GENERATE_CHI flag controls if a separate .chi index file is generated "
            "(YES) or that it should be included in the master .chm file (NO). "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("CHM_INDEX_ENCODING",
            "The CHM_INDEX_ENCODING is used to encode HtmlHelp index (hhk), content (hhc) "
            "and project file content. "
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("BINARY_TOC",
            "The BINARY_TOC flag controls whether a binary table of contents is generated "
            "(YES) or a normal table of contents (NO) in the .chm file. Furthermore it "
            "enables the Previous and Next buttons. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("TOC_EXPAND",
            "The TOC_EXPAND flag can be set to YES to add extra items for group members to "
            "the table of contents of the HTML help documentation and to the tree view. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("GENERATE_QHP",
            "If the GENERATE_QHP tag is set to YES and both QHP_NAMESPACE and "
            "QHP_VIRTUAL_FOLDER are set, an additional index file will be generated that "
            "can be used as input for Qt's qhelpgenerator to generate a Qt Compressed Help "
            "(.qch) of the generated HTML documentation. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("QCH_FILE",
            "If the QHG_LOCATION tag is specified, the QCH_FILE tag can be used to specify "
            "the file name of the resulting .qch file. The path specified is relative to "
            "the HTML output folder. "
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_NAMESPACE",
            "The QHP_NAMESPACE tag specifies the namespace to use when generating Qt Help "
            "Project output. For more information please see Qt Help Project / Namespace "
            "(see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#namespace). "
            "The default value is: org.DoxyPress.Project. "
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_VIRTUAL_FOLDER",
            "The QHP_VIRTUAL_FOLDER tag specifies the namespace to use when generating Qt "
            "Help Project output. For more information please see Qt Help Project / Virtual "
            "Folders (see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#virtual- "
            "folders). "
            "The default value is: doc. "
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_CUST_FILTER_NAME",
            "If the QHP_CUST_FILTER_NAME tag is set, it specifies the name of a custom "
            "filter to add. For more information please see Qt Help Project / Custom "
            "Filters (see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#custom- "
            "filters). "
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_CUST_FILTER_ATTRS",
            "The QHP_CUST_FILTER_ATTRS tag specifies the list of the attributes of the "
            "custom filter to add. For more information please see Qt Help Project / Custom "
            "Filters (see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#custom- "
            "filters). "
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_SECT_FILTER_ATTRS",
            "The QHP_SECT_FILTER_ATTRS tag specifies the list of the attributes this "
            "project's filter section matches. Qt Help Project / Filter Attributes (see: "
            "http://qt-project.org/doc/qt-4.8/qthelpproject.html#filter-attributes). "
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHG_LOCATION",
            "The QHG_LOCATION tag can be used to specify the location of Qt's "
            "qhelpgenerator. If non-empty DoxyPress will try to run qhelpgenerator on the "
            "generated .qhp file. "
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("GENERATE_ECLIPSEHELP",
            "If the GENERATE_ECLIPSEHELP tag is set to YES, additional index files will be "
            "generated, together with the HTML files, they form an Eclipse help plugin. To "
            "install this plugin and make it available under the help contents menu in "
            "Eclipse, the contents of the directory containing the HTML and XML files needs "
            "to be copied into the plugins directory of eclipse. The name of the directory "
            "within the plugins directory should be the same as the ECLIPSE_DOC_ID value. "
            "After copying Eclipse needs to be restarted before the help appears. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("ECLIPSE_DOC_ID",
            "A unique identifier for the Eclipse help plugin. When installing the plugin "
            "the directory name containing the HTML and XML files should also have this "
            "name. Each documentation set should have its own identifier. "
            "The default value is: org.DoxyPress.Project. "
            "This tag requires that the tag GENERATE_ECLIPSEHELP is set to YES.");

   m_fullHelp.insert("DISABLE_INDEX",
            "If you want full control over the layout of the generated HTML pages it might "
            "be necessary to disable the index and replace it with your own. The "
            "DISABLE_INDEX tag can be used to turn on/off the condensed index (tabs) at top "
            "of each HTML page. A value of NO enables the index and the value YES disables "
            "it. Since the tabs in the index contain the same information as the navigation "
            "tree, you can set this option to YES if you also set GENERATE_TREEVIEW to YES. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("GENERATE_TREEVIEW",
            "The GENERATE_TREEVIEW tag is used to specify whether a tree-like index "
            "structure should be generated to display hierarchical information. If the tag "
            "value is set to YES, a side panel will be generated containing a tree-like "
            "index structure (just like the one that is generated for HTML Help). For this "
            "to work a browser that supports JavaScript, DHTML, CSS and frames is required "
            "(i.e. any modern browser). Windows users are probably better off using the "
            "HTML help feature. Via custom stylesheets (see HTML_EXTRA_STYLESHEET) one can "
            "further fine-tune the look of the index. As an example, the default style "
            "sheet generated by DoxyPress has an example that shows how to put an image at "
            "the root of the tree instead of the PROJECT_NAME. Since the tree basically has "
            "the same information as the tab index, you could consider setting "
            "DISABLE_INDEX to YES when enabling this option. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("ENUM_VALUES_PER_LINE",
            "The ENUM_VALUES_PER_LINE tag can be used to set the number of enum values that "
            "DoxyPress will group on one line in the generated HTML documentation. "
            " "
            "Note that a value of 0 will completely suppress the enum values from appearing "
            "in the overview section. "
            "Minimum value: 0, maximum value: 20, default value: 4. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("TREEVIEW_WIDTH",
            "If the treeview is enabled (see GENERATE_TREEVIEW) then this tag can be used "
            "to set the initial width (in pixels) of the frame in which the tree is shown. "
            "Minimum value: 0, maximum value: 1500, default value: 250. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("EXT_LINKS_IN_WINDOW",
            "If the EXT_LINKS_IN_WINDOW option is set to YES, DoxyPress will open links to "
            "external symbols imported via tag files in a separate window. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("FORMULA_FONTSIZE",
            "Use this tag to change the font size of LaTeX formulas included as images in "
            "the HTML documentation. When you change the font size after a successful "
            "DoxyPress run you need to manually remove any form_*.png images from the HTML "
            "output directory to force them to be regenerated. "
            "Minimum value: 8, maximum value: 50, default value: 10. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("FORMULA_TRANSPARENT",
            "Use the FORMULA_TRANPARENT tag to determine whether or not the images "
            "generated for formulas are transparent PNGs. Transparent PNGs are not "
            "supported properly for IE 6.0, but are supported on all modern browsers. "
            " "
            "Note that when changing this option you need to delete any form_*.png files in "
            "the HTML output directory before the changes have effect. "
            "The default value is: YES. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("USE_MATHJAX",
            "Enable the USE_MATHJAX option to render LaTeX formulas using MathJax (see "
            "http://www.mathjax.org) which uses client side Javascript for the rendering "
            "instead of using pre-rendered bitmaps. Use this if you do not have LaTeX "
            "installed or if you want to formulas look prettier in the HTML output. When "
            "enabled you may also need to install MathJax separately and configure the path "
            "to it using the MATHJAX_RELPATH option. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("MATHJAX_FORMAT",
            "When MathJax is enabled you can set the default output format to be used for "
            "the MathJax output. See the MathJax site (see: "
            "http://docs.mathjax.org/en/latest/output.html) for more details. "
            "Possible values are: HTML-CSS (which is slower, but has the best "
            "compatibility), NativeMML (i.e. MathML) and SVG. "
            "The default value is: HTML-CSS. "
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("MATHJAX_RELPATH",
            "When MathJax is enabled you need to specify the location relative to the HTML "
            "output directory using the MATHJAX_RELPATH option. The destination directory "
            "should contain the MathJax.js script. For instance, if the mathjax directory "
            "is located at the same level as the HTML output directory, then "
            "MATHJAX_RELPATH should be ../mathjax. The default value points to the MathJax "
            "Content Delivery Network so you can quickly see the result without installing "
            "MathJax. However, it is strongly recommended to install a local copy of "
            "MathJax from http://www.mathjax.org before deployment. "
            "The default value is: http://cdn.mathjax.org/mathjax/latest. "
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("MATHJAX_EXTENSIONS",
            "The MATHJAX_EXTENSIONS tag can be used to specify one or more MathJax "
            "extension names that should be enabled during MathJax rendering. For example "
            "MATHJAX_EXTENSIONS = TeX/AMSmath TeX/AMSsymbols "
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("MATHJAX_CODEFILE",
            "The MATHJAX_CODEFILE tag can be used to specify a file with javascript pieces "
            "of code that will be used on startup of the MathJax code. See the MathJax site "
            "(see: http://docs.mathjax.org/en/latest/output.html) for more details. For an "
            "example see the documentation. "
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("SEARCHENGINE",
            "When the SEARCHENGINE tag is enabled DoxyPress will generate a search box for "
            "the HTML output. The underlying search engine uses javascript and DHTML and "
            "should work on any modern browser. Note that when using HTML help "
            "(GENERATE_HTMLHELP), Qt help (GENERATE_QHP), or docsets (GENERATE_DOCSET) "
            "there is already a search function so this one should typically be disabled. "
            "For large projects the javascript based search engine can be slow, then "
            "enabling SERVER_BASED_SEARCH may provide a better solution. It is possible to "
            "search using the keyboard; to jump to the search box use <access key> + S "
            "(what the <access key> is depends on the OS and browser, but it is typically "
            "<CTRL>, <ALT>/<option>, or both). Inside the search box use the <cursor down "
            "key> to jump into the search results window, the results can be navigated "
            "using the <cursor keys>. Press <Enter> to select an item or <escape> to cancel "
            "the search. The filter options can be selected when the cursor is inside the "
            "search box by pressing <Shift>+<cursor down>. Also here use the <cursor keys> "
            "to select a filter and <Enter> or <escape> to activate or cancel the filter "
            "option. "
            "The default value is: YES. "
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("SERVER_BASED_SEARCH",
            "When the SERVER_BASED_SEARCH tag is enabled the search engine will be "
            "implemented using a web server instead of a web client using Javascript. There "
            "are two flavors of web server based searching depending on the EXTERNAL_SEARCH "
            "setting. When disabled, DoxyPress will generate a PHP script for searching and "
            "an index file used by the script. When EXTERNAL_SEARCH is enabled the indexing "
            "and searching needs to be provided by external tools. See the section "
            "\"External Indexing and Searching\" for details. "
            "The default value is: NO. "
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("EXTERNAL_SEARCH",
            "When EXTERNAL_SEARCH tag is enabled DoxyPress will no longer generate the PHP "
            "script for searching. Instead the search results are written to an XML file "
            "which needs to be processed by an external indexer. DoxyPress will invoke an "
            "external search engine pointed to by the SEARCHENGINE_URL option to obtain the "
            "search results. "
            " "
            "DoxyPress ships with an example indexer (doxyindexer) and search engine "
            "(doxysearch.cgi) which are based on the open source search engine library "
            "Xapian (see: http://xapian.org/). "
            " "
            "See the section \"External Indexing and Searching\" for details. "
            "The default value is: NO. "
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("SEARCHENGINE_URL",
            "The SEARCHENGINE_URL should point to a search engine hosted by a web server "
            "which will return the search results when EXTERNAL_SEARCH is enabled. "
            " "
            "DoxyPress ships with an example indexer (doxyindexer) and search engine "
            "(doxysearch.cgi) which are based on the open source search engine library "
            "Xapian (see: http://xapian.org/). See the section \"External Indexing and "
            "Searching\" for details. "
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("SEARCHDATA_FILE",
            "When SERVER_BASED_SEARCH and EXTERNAL_SEARCH are both enabled the unindexed "
            "search data is written to a file for indexing by an external tool. With the "
            "SEARCHDATA_FILE tag the name of this file can be specified. "
            "The default file is: searchdata.xml. "
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("EXTERNAL_SEARCH_ID",
            "When SERVER_BASED_SEARCH and EXTERNAL_SEARCH are both enabled the "
            "EXTERNAL_SEARCH_ID tag can be used as an identifier for the project. This is "
            "useful in combination with EXTRA_SEARCH_MAPPINGS to search through multiple "
            "projects and redirect the results back to the right project. "
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("EXTRA_SEARCH_MAPPINGS",
            "The EXTRA_SEARCH_MAPPINGS tag can be used to enable searching through DoxyPress "
            "projects other than the one defined by this configuration file, but that are "
            "all added to the same external search index. Each project needs to have a "
            "unique id set via EXTERNAL_SEARCH_ID. The search mapping then maps the id of "
            "to a relative location where the documentation can be found. The format is: "
            "EXTRA_SEARCH_MAPPINGS = tagname1=loc1 tagname2=loc2 ... "
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("GENERATE_LATEX",
            "If the GENERATE_LATEX tag is set to YES, DoxyPress will generate LaTeX output. "
            "The default value is: YES.");

   m_fullHelp.insert("LATEX_OUTPUT",
            "The LATEX_OUTPUT tag is used to specify where the LaTeX docs will be put. If a "
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of "
            "it. "
            "The default directory is: latex. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_CMD_NAME",
            "The LATEX_CMD_NAME tag can be used to specify the LaTeX command name to be "
            "invoked. "
            " "
            "Note that when enabling USE_PDFLATEX this option is only used for generating "
            "bitmaps for formulas in the HTML output, but not in the Makefile that is "
            "written to the output directory. "
            "The default file is: latex. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("MAKEINDEX_CMD_NAME",
            "The MAKEINDEX_CMD_NAME tag can be used to specify the command name to generate "
            "index for LaTeX. "
            "The default file is: makeindex. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("COMPACT_LATEX",
            "If the COMPACT_LATEX tag is set to YES, DoxyPress generates more compact LaTeX "
            "documents. This may be useful for small projects and may help to save some "
            "trees in general. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("PAPER_TYPE",
            "The PAPER_TYPE tag can be used to set the paper type that is used by the "
            "printer. "
            "Possible values are: a4 (210 x 297 mm), letter (8.5 x 11 inches), legal (8.5 x "
            "14 inches) and executive (7.25 x 10.5 inches). "
            "The default value is: a4. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("EXTRA_PACKAGES",
            "The EXTRA_PACKAGES tag can be used to specify one or more LaTeX package names "
            "that should be included in the LaTeX output. To get the times font for "
            "instance you can specify "
            "EXTRA_PACKAGES=times "
            "If left blank no extra packages will be included. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_HEADER",
            "The LATEX_HEADER tag can be used to specify a personal LaTeX header for the "
            "generated LaTeX document. The header should contain everything until the first "
            "chapter. If it is left blank DoxyPress will generate a standard header. See "
            "section \"DoxyPress usage\" for information on how to let DoxyPress write the "
            "default header to a separate file. "
            " "
            "Note: Only use a user-defined header if you know what you are doing! The "
            "following commands have a special meaning inside the header: $title, "
            "$datetime, $date, $DoxyPressversion, $projectname, $projectnumber, "
            "$projectbrief, $projectlogo. DoxyPress will replace $title with the empty "
            "string, for the replacement values of the other commands the user is referred "
            "to HTML_HEADER. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_FOOTER",
            "The LATEX_FOOTER tag can be used to specify a personal LaTeX footer for the "
            "generated LaTeX document. The footer should contain everything after the last "
            "chapter. If it is left blank DoxyPress will generate a standard footer. See "
            "LATEX_HEADER for more information on how to generate a default footer and what "
            "special commands can be used inside the footer. "
            " "
            "Note: Only use a user-defined footer if you know what you are doing! "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_EXTRA_FILES",
            "The LATEX_EXTRA_FILES tag can be used to specify one or more extra images or "
            "other source files which should be copied to the LATEX_OUTPUT output "
            "directory. Note that the files will be copied as-is; there are no commands or "
            "markers available. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("PDF_HYPERLINKS",
            "If the PDF_HYPERLINKS tag is set to YES, the LaTeX that is generated is "
            "prepared for conversion to PDF (using ps2pdf or pdflatex). The PDF file will "
            "contain links (just like the HTML output) instead of page references. This "
            "makes the output suitable for online browsing using a PDF viewer. "
            "The default value is: YES. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("USE_PDFLATEX",
            "If the USE_PDFLATEX tag is set to YES, DoxyPress will use pdflatex to generate "
            "the PDF file directly from the LaTeX files. Set this option to YES, to get a "
            "higher quality PDF documentation. "
            "The default value is: YES. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_BATCHMODE",
            "If the LATEX_BATCHMODE tag is set to YES, DoxyPress will add the \\batchmode "
            "command to the generated LaTeX files. This will instruct LaTeX to keep running "
            "if errors occur, instead of asking the user for help. This option is also used "
            "when generating formulas in HTML. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_HIDE_INDICES",
            "If the LATEX_HIDE_INDICES tag is set to YES then DoxyPress will not include the "
            "index chapters (such as File Index, Compound Index, etc.) in the output. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_SOURCE_CODE",
            "If the LATEX_SOURCE_CODE tag is set to YES then DoxyPress will include source "
            "code with syntax highlighting in the LaTeX output. "
            " "
            "Note that which sources are shown also depends on other settings such as "
            "SOURCE_BROWSER. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_BIB_STYLE",
            "The LATEX_BIB_STYLE tag can be used to specify the style to use for the "
            "bibliography, e.g. plainnat, or ieeetr. See "
            "http://en.wikipedia.org/wiki/BibTeX and \\cite for more info. "
            "The default value is: plain. "
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("GENERATE_RTF",
            "If the GENERATE_RTF tag is set to YES, DoxyPress will generate RTF output. The "
            "RTF output is optimized for Word 97 and may not look too pretty with other RTF "
            "readers/editors. "
            "The default value is: NO.");

   m_fullHelp.insert("RTF_OUTPUT",
            "The RTF_OUTPUT tag is used to specify where the RTF docs will be put. If a "
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of "
            "it. "
            "The default directory is: rtf. "
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("COMPACT_RTF",
            "If the COMPACT_RTF tag is set to YES, DoxyPress generates more compact RTF "
            "documents. This may be useful for small projects and may help to save some "
            "trees in general. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("RTF_HYPERLINKS",
            "If the RTF_HYPERLINKS tag is set to YES, the RTF that is generated will "
            "contain hyperlink fields. The RTF file will contain links (just like the HTML "
            "output) instead of page references. This makes the output suitable for online "
            "browsing using Word or some other Word compatible readers that support those "
            "fields. "
            " "
            "Note: WordPad (write) and others do not support links. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("RTF_STYLESHEET_FILE",
            "Load stylesheet definitions from file. Syntax is similar to DoxyPress's config "
            "file, i.e. a series of assignments. You only have to provide replacements, "
            "missing definitions are set to their default value. "
            " "
            "See also section \"DoxyPress usage\" for information on how to generate the "
            "default style sheet that DoxyPress normally uses. "
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("RTF_EXTENSIONS_FILE",
            "Set optional variables used in the generation of an RTF document. Syntax is "
            "similar to DoxyPress's config file. A template extensions file can be generated "
            "using DoxyPress -e rtf extensionFile. "
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("GENERATE_MAN",
            "If the GENERATE_MAN tag is set to YES, DoxyPress will generate man pages for "
            "classes and files. "
            "The default value is: NO.");

   m_fullHelp.insert("MAN_OUTPUT",
            "The MAN_OUTPUT tag is used to specify where the man pages will be put. If a "
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of "
            "it. A directory man3 will be created inside the directory specified by "
            "MAN_OUTPUT. "
            "The default directory is: man. "
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("MAN_EXTENSION",
            "The MAN_EXTENSION tag determines the extension that is added to the generated "
            "man pages. In case the manual section does not start with a number, the number "
            "3 is prepended. The dot (.) at the beginning of the MAN_EXTENSION tag is "
            "optional. "
            "The default value is: .3. "
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("MAN_SUBDIR",
            "The MAN_SUBDIR tag determines the name of the directory created within "
            "MAN_OUTPUT in which the man pages are placed. If defaults to man followed by "
            "MAN_EXTENSION with the initial . removed. "
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("MAN_LINKS",
            "If the MAN_LINKS tag is set to YES and DoxyPress generates man output, then it "
            "will generate one additional man file for each entity documented in the real "
            "man page(s). These additional files only source the real man page, but without "
            "them the man command would be unable to find the correct page. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("GENERATE_XML",
            "If the GENERATE_XML tag is set to YES, DoxyPress will generate an XML file that "
            "captures the structure of the code including all documentation. "
            "The default value is: NO.");

   m_fullHelp.insert("XML_OUTPUT",
            "The XML_OUTPUT tag is used to specify where the XML pages will be put. If a "
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of "
            "it. "
            "The default directory is: xml. "
            "This tag requires that the tag GENERATE_XML is set to YES.");

   m_fullHelp.insert("XML_PROGRAMLISTING",
            "If the XML_PROGRAMLISTING tag is set to YES, DoxyPress will dump the program "
            "listings (including syntax highlighting and cross-referencing information) to "
            "the XML output. Note that enabling this will significantly increase the size "
            "of the XML output. "
            "The default value is: YES. "
            "This tag requires that the tag GENERATE_XML is set to YES.");

   m_fullHelp.insert("GENERATE_DOCBOOK",
            "If the GENERATE_DOCBOOK tag is set to YES, DoxyPress will generate Docbook files "
            "that can be used to generate PDF. "
            "The default value is: NO.");

   m_fullHelp.insert("DOCBOOK_OUTPUT",
            "The DOCBOOK_OUTPUT tag is used to specify where the Docbook pages will be put. "
            "If a relative path is entered the value of OUTPUT_DIRECTORY will be put in "
            "front of it. "
            "The default directory is: docbook. "
            "This tag requires that the tag GENERATE_DOCBOOK is set to YES.");

   m_fullHelp.insert("DOCBOOK_PROGRAMLISTING",
            "If the DOCBOOK_PROGRAMLISTING tag is set to YES, DoxyPress will include the "
            "program listings (including syntax highlighting and cross-referencing "
            "information) to the DOCBOOK output. Note that enabling this will significantly "
            "increase the size of the DOCBOOK output. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_DOCBOOK is set to YES.");

   m_fullHelp.insert("GENERATE_AUTOGEN_DEF",
            "If the GENERATE_AUTOGEN_DEF tag is set to YES, DoxyPress will generate an "
            "AutoGen Definitions (see http://autogen.sf.net) file that captures the "
            "structure of the code including all documentation. Note that this feature is "
            "still experimental and incomplete at the moment. "
            "The default value is: NO.");

   m_fullHelp.insert("GENERATE_PERLMOD",
            "If the GENERATE_PERLMOD tag is set to YES, DoxyPress will generate a Perl module "
            "file that captures the structure of the code including all documentation. "
            " "
            "Note that this feature is still experimental and incomplete at the moment. "
            "The default value is: NO.");

   m_fullHelp.insert("PERLMOD_LATEX",
            "If the PERLMOD_LATEX tag is set to YES, DoxyPress will generate the necessary "
            "Makefile rules, Perl scripts and LaTeX code to be able to generate PDF and DVI "
            "output from the Perl module output. "
            "The default value is: NO. "
            "This tag requires that the tag GENERATE_PERLMOD is set to YES.");

   m_fullHelp.insert("PERLMOD_PRETTY",
            "If the PERLMOD_PRETTY tag is set to YES, the Perl module output will be nicely "
            "formatted so it can be parsed by a human reader. This is useful if you want to "
            "understand what is going on. On the other hand, if this tag is set to NO, the "
            "size of the Perl module output will be much smaller and Perl will parse it "
            "just the same. "
            "The default value is: YES. "
            "This tag requires that the tag GENERATE_PERLMOD is set to YES.");

   m_fullHelp.insert("PERLMOD_MAKEVAR_PREFIX",
            "The names of the make variables in the generated doxyrules.make file are "
            "prefixed with the string contained in PERLMOD_MAKEVAR_PREFIX. This is useful "
            "so different doxyrules.make files included by the same Makefile don't "
            "overwrite each other's variables. "
            "This tag requires that the tag GENERATE_PERLMOD is set to YES.");

   m_fullHelp.insert("ENABLE_PREPROCESSING",
            "If the ENABLE_PREPROCESSING tag is set to YES, DoxyPress will evaluate all "
            "C-preprocessor directives found in the sources and include files. "
            "The default value is: YES.");

   m_fullHelp.insert("MACRO_EXPANSION",
            "If the MACRO_EXPANSION tag is set to YES, DoxyPress will expand all macro names "
            "in the source code. If set to NO, only conditional compilation will be "
            "performed. Macro expansion can be done in a controlled way by setting "
            "EXPAND_ONLY_PREDEF to YES. "
            "The default value is: NO. "
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("EXPAND_ONLY_PREDEF",
            "If the EXPAND_ONLY_PREDEF and MACRO_EXPANSION tags are both set to YES then "
            "the macro expansion is limited to the macros specified with the PREDEFINED and "
            "EXPAND_AS_DEFINED tags. "
            "The default value is: NO. "
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("SEARCH_INCLUDES",
            "If the SEARCH_INCLUDES tag is set to YES, the include files in the "
            "INCLUDE_PATH will be searched if a #include is found. "
            "The default value is: YES. "
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("INCLUDE_PATH",
            "The INCLUDE_PATH tag can be used to specify one or more directories that "
            "contain include files that are not input files but should be processed by the "
            "preprocessor. "
            "This tag requires that the tag SEARCH_INCLUDES is set to YES.");

   m_fullHelp.insert("INCLUDE_FILE_PATTERNS",
            "You can use the INCLUDE_FILE_PATTERNS tag to specify one or more wildcard "
            "patterns (like *.h and *.hpp) to filter out the header-files in the "
            "directories. If left blank, the patterns specified with FILE_PATTERNS will be "
            "used. "
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("PREDEFINED",
            "The PREDEFINED tag can be used to specify one or more macro names that are "
            "defined before the preprocessor is started (similar to the -D option of e.g. "
            "gcc). The argument of the tag is a list of macros of the form: name or "
            "name=definition (no spaces). If the definition and the \"=\" are omitted, \"=1\" "
            "is assumed. To prevent a macro definition from being undefined via #undef or "
            "recursively expanded use the := operator instead of the = operator. "
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("EXPAND_AS_DEFINED",
            "If the MACRO_EXPANSION and EXPAND_ONLY_PREDEF tags are set to YES then this "
            "tag can be used to specify a list of macro names that should be expanded. The "
            "macro definition that is found in the sources will be used. Use the PREDEFINED "
            "tag if you want to use a different macro definition that overrules the "
            "definition found in the source code. "
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("SKIP_FUNCTION_MACROS",
            "If the SKIP_FUNCTION_MACROS tag is set to YES then DoxyPress's preprocessor will "
            "remove all references to function-like macros that are alone on a line, have "
            "an all uppercase name, and do not end with a semicolon. Such function macros "
            "are typically used for boiler-plate code, and will confuse the parser if not "
            "removed. "
            "The default value is: YES. "
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("TAGFILES",
            "The TAGFILES tag can be used to specify one or more tag files. For each tag "
            "file the location of the external documentation should be added. The format of "
            "a tag file without this location is as follows: "
            "TAGFILES = file1 file2 ... "
            "Adding location for the tag files is done as follows: "
            "TAGFILES = file1=loc1 \"file2 = loc2\" ... "
            "where loc1 and loc2 can be relative or absolute paths or URLs. See the "
            "section \"Linking to external documentation\" for more information about the use "
            "of tag files. "
            "Note: Each tag file must have a unique name (where the name does NOT include "
            "the path). If a tag file is not located in the directory in which DoxyPress is "
            "run, you must also specify the path to the tagfile here.");

   m_fullHelp.insert("GENERATE_TAGFILE",
            "When a file name is specified after GENERATE_TAGFILE, DoxyPress will create a "
            "tag file that is based on the input files it reads. See section \"Linking to "
            "external documentation\" for more information about the usage of tag files.");

   m_fullHelp.insert("ALLEXTERNALS",
            "If the ALLEXTERNALS tag is set to YES, all external class will be listed in "
            "the class index. If set to NO, only the inherited external classes will be "
            "listed. "
            "The default value is: NO.");

   m_fullHelp.insert("EXTERNAL_GROUPS",
            "If the EXTERNAL_GROUPS tag is set to YES, all external groups will be listed "
            "in the modules index. If set to NO, only the current project's groups will be "
            "listed. "
            "The default value is: YES.");

   m_fullHelp.insert("EXTERNAL_PAGES",
            "If the EXTERNAL_PAGES tag is set to YES, all external pages will be listed in "
            "the related pages index. If set to NO, only the current project's pages will "
            "be listed. "
            "The default value is: YES.");

   m_fullHelp.insert("PERL_PATH",
            "The PERL_PATH should be the absolute path and name of the perl script "
            "interpreter (i.e. the result of 'which perl'). "
            "The default file (with absolute path) is: /usr/bin/perl.");

   m_fullHelp.insert("CLASS_DIAGRAMS",
            "If the CLASS_DIAGRAMS tag is set to YES, DoxyPress will generate a class diagram "
            "(in HTML and LaTeX) for classes with base or super classes. Setting the tag to "
            "NO turns the diagrams off. Note that this option also works with HAVE_DOT "
            "disabled, but it is recommended to install and use dot, since it yields more "
            "powerful graphs. "
            "The default value is: YES.");

   m_fullHelp.insert("MSCGEN_PATH",
            "You can define message sequence charts within DoxyPress comments using the \\msc "
            "command. DoxyPress will then run the mscgen tool (see: "
            "http://www.mcternan.me.uk/mscgen/)) to produce the chart and insert it in the "
            "documentation. The MSCGEN_PATH tag allows you to specify the directory where "
            "the mscgen tool resides. If left empty the tool is assumed to be found in the "
            "default search path.");

   m_fullHelp.insert("DIA_PATH",
            "You can include diagrams made with dia in DoxyPress documentation. DoxyPress will "
            "then run dia to produce the diagram and insert it in the documentation. The "
            "DIA_PATH tag allows you to specify the directory where the dia binary resides. "
            "If left empty dia is assumed to be found in the default search path.");

   m_fullHelp.insert("HIDE_UNDOC_RELATIONS",
            "If set to YES the inheritance and collaboration graphs will hide inheritance "
            "and usage relations if the target is undocumented or is not a class. "
            "The default value is: YES.");

   m_fullHelp.insert("HAVE_DOT",
            "If you set the HAVE_DOT tag to YES then DoxyPress will assume the dot tool is "
            "available from the path. This tool is part of Graphviz (see: "
            "http://www.graphviz.org/), a graph visualization toolkit from AT&T and Lucent "
            "Bell Labs. The other options in this section have no effect if this option is "
            "set to NO "
            "The default value is: NO.");

   m_fullHelp.insert("DOT_NUM_THREADS",
            "The DOT_NUM_THREADS specifies the number of dot invocations DoxyPress is allowed "
            "to run in parallel. When set to 0 DoxyPress will base this on the number of "
            "processors available in the system. You can set it explicitly to a value "
            "larger than 0 to get control over the balance between CPU load and processing "
            "speed. "
            "Minimum value: 0, maximum value: 32, default value: 0. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_FONTNAME",
            "When you want a differently looking font in the dot files that DoxyPress "
            "generates you can specify the font name using DOT_FONTNAME. You need to make "
            "sure dot is able to find the font, which can be done by putting it in a "
            "standard location or by setting the DOTFONTPATH environment variable or by "
            "setting DOT_FONTPATH to the directory containing the font. "
            "The default value is: Helvetica. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_FONTSIZE",
            "The DOT_FONTSIZE tag can be used to set the size (in points) of the font of "
            "dot graphs. "
            "Minimum value: 4, maximum value: 24, default value: 10. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_FONTPATH",
            "By default DoxyPress will tell dot to use the default font as specified with "
            "DOT_FONTNAME. If you specify a different font using DOT_FONTNAME you can set "
            "the path where dot can find it using this tag. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("CLASS_GRAPH",
            "If the CLASS_GRAPH tag is set to YES then DoxyPress will generate a graph for "
            "each documented class showing the direct and indirect inheritance relations. "
            "Setting this tag to YES will force the CLASS_DIAGRAMS tag to NO. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("COLLABORATION_GRAPH",
            "If the COLLABORATION_GRAPH tag is set to YES then DoxyPress will generate a "
            "graph for each documented class showing the direct and indirect implementation "
            "dependencies (inheritance, containment, and class references variables) of the "
            "class with other documented classes. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("GROUP_GRAPHS",
            "If the GROUP_GRAPHS tag is set to YES then DoxyPress will generate a graph for "
            "groups, showing the direct groups dependencies. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("UML_LOOK",
            "If the UML_LOOK tag is set to YES, DoxyPress will generate inheritance and "
            "collaboration diagrams in a style similar to the OMG's Unified Modeling "
            "Language. "
            "The default value is: NO. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("UML_LIMIT_NUM_FIELDS",
            "If the UML_LOOK tag is enabled, the fields and methods are shown inside the "
            "class node. If there are many fields or methods and many nodes the graph may "
            "become too big to be useful. The UML_LIMIT_NUM_FIELDS threshold limits the "
            "number of items for each type to make the size more manageable. Set this to 0 "
            "for no limit. Note that the threshold may be exceeded by 50% before the limit "
            "is enforced. So when you set the threshold to 10, up to 15 fields may appear, "
            "but if the number exceeds 15, the total amount of fields shown is limited to "
            "10. "
            "Minimum value: 0, maximum value: 100, default value: 10. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("TEMPLATE_RELATIONS",
            "If the TEMPLATE_RELATIONS tag is set to YES then the inheritance and "
            "collaboration graphs will show the relations between templates and their "
            "instances. "
            "The default value is: NO. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("INCLUDE_GRAPH",
            "If the INCLUDE_GRAPH, ENABLE_PREPROCESSING and SEARCH_INCLUDES tags are set to "
            "YES then DoxyPress will generate a graph for each documented file showing the "
            "direct and indirect include dependencies of the file with other documented "
            "files. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("INCLUDED_BY_GRAPH",
            "If the INCLUDED_BY_GRAPH, ENABLE_PREPROCESSING and SEARCH_INCLUDES tags are "
            "set to YES then DoxyPress will generate a graph for each documented file showing "
            "the direct and indirect include dependencies of the file with other documented "
            "files. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("CALL_GRAPH",
            "If the CALL_GRAPH tag is set to YES then DoxyPress will generate a call "
            "dependency graph for every global function or class method. "
            " "
            "Note that enabling this option will significantly increase the time of a run. "
            "So in most cases it will be better to enable call graphs for selected "
            "functions only using the \\callgraph command. "
            "The default value is: NO. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("CALLER_GRAPH",
            "If the CALLER_GRAPH tag is set to YES then DoxyPress will generate a caller "
            "dependency graph for every global function or class method. "
            " "
            "Note that enabling this option will significantly increase the time of a run. "
            "So in most cases it will be better to enable caller graphs for selected "
            "functions only using the \\callergraph command. "
            "The default value is: NO. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("GRAPHICAL_HIERARCHY",
            "If the GRAPHICAL_HIERARCHY tag is set to YES then DoxyPress will graphical "
            "hierarchy of all classes instead of a textual one. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DIRECTORY_GRAPH",
            "If the DIRECTORY_GRAPH tag is set to YES then DoxyPress will show the "
            "dependencies a directory has on other directories in a graphical way. The "
            "dependency relations are determined by the #include relations between the "
            "files in the directories. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_IMAGE_FORMAT",
            "The DOT_IMAGE_FORMAT tag can be used to set the image format of the images "
            "generated by dot. "
            "Note: If you choose svg you need to set HTML_FILE_EXTENSION to xhtml in order "
            "to make the SVG files visible in IE 9+ (other browsers do not have this "
            "requirement). "
            "Possible values are: png, jpg, gif and svg. "
            "The default value is: png. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("INTERACTIVE_SVG",
            "If DOT_IMAGE_FORMAT is set to svg, then this option can be set to YES to "
            "enable generation of interactive SVG images that allow zooming and panning. "
            " "
            "Note that this requires a modern browser other than Internet Explorer. Tested "
            "and working are Firefox, Chrome, Safari, and Opera. "
            "Note: For IE 9+ you need to set HTML_FILE_EXTENSION to xhtml in order to make "
            "the SVG files visible. Older versions of IE do not have SVG support. "
            "The default value is: NO. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_PATH",
            "The DOT_PATH tag can be used to specify the path where the dot tool can be "
            "found. If left blank, it is assumed the dot tool can be found in the path. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOTFILE_DIRS",
            "The DOTFILE_DIRS tag can be used to specify one or more directories that "
            "contain dot files that are included in the documentation (see the \\dotfile "
            "command). "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("MSCFILE_DIRS",
            "The MSCFILE_DIRS tag can be used to specify one or more directories that "
            "contain msc files that are included in the documentation (see the \\mscfile "
            "command).");

   m_fullHelp.insert("DIAFILE_DIRS",
            "The DIAFILE_DIRS tag can be used to specify one or more directories that "
            "contain dia files that are included in the documentation (see the \\diafile "
            "command).");

   m_fullHelp.insert("PLANTUML_JAR_PATH",
            "When using plantuml, the PLANTUML_JAR_PATH tag should be used to specify the "
            "path where java can find the plantuml.jar file. If left blank, it is assumed "
            "PlantUML is not used or called during a preprocessing step. DoxyPress will "
            "generate a warning when it encounters a \\startuml command in this case and "
            "will not generate output for the diagram.");

   m_fullHelp.insert("PLANTUML_INCLUDE_PATH",
            "When using plantuml, the specified paths are searched for files specified by "
            "the !include statement in a plantuml block.");

   m_fullHelp.insert("DOT_GRAPH_MAX_NODES",
            "The DOT_GRAPH_MAX_NODES tag can be used to set the maximum number of nodes "
            "that will be shown in the graph. If the number of nodes in a graph becomes "
            "larger than this value, DoxyPress will truncate the graph, which is visualized "
            "by representing a node as a red box. Note that DoxyPress if the number of direct "
            "children of the root node in a graph is already larger than "
            "DOT_GRAPH_MAX_NODES then the graph will not be shown at all. Also note that "
            "the size of a graph can be further restricted by MAX_DOT_GRAPH_DEPTH. "
            "Minimum value: 0, maximum value: 10000, default value: 50. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("MAX_DOT_GRAPH_DEPTH",
            "The MAX_DOT_GRAPH_DEPTH tag can be used to set the maximum depth of the graphs "
            "generated by dot. A depth value of 3 means that only nodes reachable from the "
            "root by following a path via at most 3 edges will be shown. Nodes that lay "
            "further from the root node will be omitted. Note that setting this option to 1 "
            "or 2 may greatly reduce the computation time needed for large code bases. Also "
            "note that the size of a graph can be further restricted by "
            "DOT_GRAPH_MAX_NODES. Using a depth of 0 means no depth restriction. "
            "Minimum value: 0, maximum value: 1000, default value: 0. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_TRANSPARENT",
            "Set the DOT_TRANSPARENT tag to YES to generate images with a transparent "
            "background. This is disabled by default, because dot on Windows does not seem "
            "to support this out of the box. "
            " "
            "Warning: Depending on the platform used, enabling this option may lead to "
            "badly anti-aliased labels on the edges of a graph (i.e. they become hard to "
            "read). "
            "The default value is: NO. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_MULTI_TARGETS",
            "Set the DOT_MULTI_TARGETS tag to YES to allow dot to generate multiple output "
            "files in one run (i.e. multiple -o and -T options on the command line). This "
            "makes dot run faster, but since only newer versions of dot (>1.8.10) support "
            "this, this feature is disabled by default. "
            "The default value is: NO. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("GENERATE_LEGEND",
            "If the GENERATE_LEGEND tag is set to YES DoxyPress will generate a legend page "
            "explaining the meaning of the various boxes and arrows in the dot generated "
            "graphs. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_CLEANUP",
            "If the DOT_CLEANUP tag is set to YES, DoxyPress will remove the intermediate dot "
            "files that are used to generate the various graphs. "
            "The default value is: YES. "
            "This tag requires that the tag HAVE_DOT is set to YES.");
   }
