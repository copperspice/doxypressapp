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
   m_ui->gen_html_CB1->setChecked(true);

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

   //QCombo  - not sure

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

         if (temp_SB->value == defValue) {
            setHelpColor(DEFAULT, label);

         } else {
            setHelpColor(MODIFIED, label);

         }
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

   m_fullHelp.insert("PROJECT_ENCODING",
            "This tag specifies the encoding used for all characters in the config file\n"
            "that follow. The default is UTF-8 which is also the encoding used for all text\n"
            "before the first occurrence of this tag. Doxygen uses libiconv (or the iconv\n"
            "built into libc) for the transcoding. See http://www.gnu.org/software/libiconv\n"
            "for the list of possible encodings.\n"
            "The default value is: UTF-8.");

   m_fullHelp.insert("PROJECT_NAME",
            "The PROJECT_NAME tag is a single word (or a sequence of words surrounded by\n"
            "double-quotes, unless you are using Doxywizard) that should identify the\n"
            "project for which the documentation is generated. This name is used in the\n"
            "title of most generated pages and in a few other places.\n"
            "The default value is: My Project.");

   m_fullHelp.insert("PROJECT_NUMBER",
            "The PROJECT_NUMBER tag can be used to enter a project or revision number. This\n"
            "could be handy for archiving the generated documentation or if some version\n"
            "control system is used.");

   m_fullHelp.insert("PROJECT_BRIEF",
            "Using the PROJECT_BRIEF tag one can provide an optional one line description\n"
            "for a project that appears at the top of each page and should give viewer a\n"
            "quick idea about the purpose of the project. Keep the description short.");

   m_fullHelp.insert("PROJECT_ICON",
            "With the PROJECT_LOGO tag one can specify a logo or an icon that is included\n"
            "in the documentation. The maximum height of the logo should not exceed 55\n"
            "pixels and the maximum width should not exceed 200 pixels. Doxygen will copy\n"
            "the logo to the output directory.");

   m_fullHelp.insert("OUTPUT_DIRECTORY",
            "The OUTPUT_DIRECTORY tag is used to specify the (relative or absolute) path\n"
            "into which the generated documentation will be written. If a relative path is\n"
            "entered, it will be relative to the location where doxygen was started. If\n"
            "left blank the current directory will be used.");

   m_fullHelp.insert("CREATE_SUBDIRS",
            "If the CREATE_SUBDIRS tag is set to YES then doxygen will create 4096 sub-\n"
            "directories (in 2 levels) under the output directory of each output format and\n"
            "will distribute the generated files over these directories. Enabling this\n"
            "option can be useful when feeding doxygen a huge amount of source files, where\n"
            "putting all generated files in the same directory would otherwise causes\n"
            "performance problems for the file system.\n"
            "The default value is: NO.");

   m_fullHelp.insert("ALLOW_UNICODE_NAMES",
            "If the ALLOW_UNICODE_NAMES tag is set to YES, doxygen will allow non-ASCII\n"
            "characters to appear in the names of generated files. If set to NO, non-ASCII\n"
            "characters will be escaped, for example _xE3_x81_x84 will be used for Unicode\n"
            "U+3044.\n"
            "The default value is: NO.");

   m_fullHelp.insert("OUTPUT_LANGUAGE",
            "The OUTPUT_LANGUAGE tag is used to specify the language in which all\n"
            "documentation generated by doxygen is written. Doxygen will use this\n"
            "information to generate all constant output in the proper language.\n"
            "Possible values are: Afrikaans, Arabic, Armenian, Brazilian, Catalan, Chinese,\n"
            "Chinese-Traditional, Croatian, Czech, Danish, Dutch, English (United States),\n"
            "Esperanto, Farsi (Persian), Finnish, French, German, Greek, Hungarian,\n"
            "Indonesian, Italian, Japanese, Japanese-en (Japanese with English messages),\n"
            "Korean, Korean-en (Korean with English messages), Latvian, Lithuanian,\n"
            "Macedonian, Norwegian, Persian (Farsi), Polish, Portuguese, Romanian, Russian,\n"
            "Serbian, Serbian-Cyrillic, Slovak, Slovene, Spanish, Swedish, Turkish,\n"
            "Ukrainian and Vietnamese.\n"
            "The default value is: English.");

   m_fullHelp.insert("BRIEF_MEMBER_DESC",
            "If the BRIEF_MEMBER_DESC tag is set to YES, doxygen will include brief member\n"
            "descriptions after the members that are listed in the file and class\n"
            "documentation (similar to Javadoc). Set to NO to disable this.\n"
            "The default value is: YES.");

   m_fullHelp.insert("REPEAT_BRIEF",
            "If the REPEAT_BRIEF tag is set to YES, doxygen will prepend the brief\n"
            "description of a member or function before the detailed description\n"
            "\n"
            "Note: If both HIDE_UNDOC_MEMBERS and BRIEF_MEMBER_DESC are set to NO, the\n"
            "brief descriptions will be completely suppressed.\n"
            "The default value is: YES.");

   m_fullHelp.insert("ABBREVIATE_BRIEF",
            "This tag implements a quasi-intelligent brief description abbreviator that is\n"
            "used to form the text in various listings. Each string in this list, if found\n"
            "as the leading text of the brief description, will be stripped from the text\n"
            "and the result, after processing the whole list, is used as the annotated\n"
            "text. Otherwise, the brief description is used as-is. If left blank, the\n"
            "following values are used ($name is automatically replaced with the name of\n"
            "the entity):The $name class, The $name widget, The $name file, is, provides,\n"
            "specifies, contains, represents, a, an and the.");

   m_fullHelp.insert("ALWAYS_DETAILED_SEC",
            "If the ALWAYS_DETAILED_SEC and REPEAT_BRIEF tags are both set to YES then\n"
            "doxygen will generate a detailed section even if there is only a brief\n"
            "description.\n"
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_INHERITED_MEMB",
            "If the INLINE_INHERITED_MEMB tag is set to YES, doxygen will show all\n"
            "inherited members of a class in the documentation of that class as if those\n"
            "members were ordinary class members. Constructors, destructors and assignment\n"
            "operators of the base classes will not be shown.\n"
            "The default value is: NO.");

   m_fullHelp.insert("FULL_PATH_NAMES",
            "If the FULL_PATH_NAMES tag is set to YES, doxygen will prepend the full path\n"
            "before files name in the file list and in the header files. If set to NO the\n"
            "shortest path that makes the file name unique will be used\n"
            "The default value is: YES.");

   m_fullHelp.insert("STRIP_FROM_PATH",
            "The STRIP_FROM_PATH tag can be used to strip a user-defined part of the path.\n"
            "Stripping is only done if one of the specified strings matches the left-hand\n"
            "part of the path. The tag can be used to show relative paths in the file list.\n"
            "If left blank the directory from which doxygen is run is used as the path to\n"
            "strip.\n"
            "\n"
            "Note that you can specify absolute paths here, but also relative paths, which\n"
            "will be relative from the directory where doxygen is started.\n"
            "This tag requires that the tag FULL_PATH_NAMES is set to YES.");

   m_fullHelp.insert("STRIP_FROM_INC_PATH",
            "The STRIP_FROM_INC_PATH tag can be used to strip a user-defined part of the\n"
            "path mentioned in the documentation of a class, which tells the reader which\n"
            "header file to include in order to use a class. If left blank only the name of\n"
            "the header file containing the class definition is used. Otherwise one should\n"
            "specify the list of include paths that are normally passed to the compiler\n"
            "using the -I flag.");

   m_fullHelp.insert("SHORT_NAMES_CB",
            "If the SHORT_NAMES tag is set to YES, doxygen will generate much shorter (but\n"
            "less readable) file names. This can be useful is your file systems doesn't\n"
            "support long names like on DOS, Mac, or CD-ROM.\n"
            "The default value is: NO.");

   m_fullHelp.insert("JAVADOC_AUTOBRIEF",
            "If the JAVADOC_AUTOBRIEF tag is set to YES then doxygen will interpret the\n"
            "first line (until the first dot) of a Javadoc-style comment as the brief\n"
            "description. If set to NO, the Javadoc-style will behave just like regular Qt-\n"
            "style comments (thus requiring an explicit @brief command for a brief\n"
            "description.)\n"
            "The default value is: NO.");

   m_fullHelp.insert("QT_AUTOBRIEF",
            "If the QT_AUTOBRIEF tag is set to YES then doxygen will interpret the first\n"
            "line (until the first dot) of a Qt-style comment as the brief description. If\n"
            "set to NO, the Qt-style will behave just like regular Qt-style comments (thus\n"
            "requiring an explicit \\brief command for a brief description.)\n"
            "The default value is: NO.");

   m_fullHelp.insert("MULTILINE_CPP_IS_BRIEF",
            "The MULTILINE_CPP_IS_BRIEF tag can be set to YES to make doxygen treat a\n"
            "multi-line C++ special comment block (i.e. a block of //! or /// comments) as\n"
            "a brief description. This used to be the default behavior. The new default is\n"
            "to treat a multi-line C++ comment block as a detailed description. Set this\n"
            "tag to YES if you prefer the old behavior instead.\n"
            "\n"
            "Note that setting this tag to YES also means that rational rose comments are\n"
            "not recognized any more.\n"
            "The default value is: NO.");

   m_fullHelp.insert("INHERIT_DOCS",
            "If the INHERIT_DOCS tag is set to YES then an undocumented member inherits the\n"
            "documentation from any documented member that it re-implements.\n"
            "The default value is: YES.");

   m_fullHelp.insert("SEPARATE_MEMBER_PAGES",
            "If the SEPARATE_MEMBER_PAGES tag is set to YES then doxygen will produce a new\n"
            "page for each member. If set to NO, the documentation of a member will be part\n"
            "of the file/class/namespace that contains it.\n"
            "The default value is: NO.");

   m_fullHelp.insert("TAB_SIZE",
            "The TAB_SIZE tag can be used to set the number of spaces in a tab. Doxygen\n"
            "uses this value to replace tabs by spaces in code fragments.\n"
            "Minimum value: 1, maximum value: 16, default value: 4.");

   m_fullHelp.insert("ALIASES",
            "This tag can be used to specify a number of aliases that act as commands in\n"
            "the documentation. An alias has the form:\n"
            "name=value\n"
            "For example adding\n"
            "\"sideeffect=@par Side Effects:\\n\"\n"
            "will allow you to put the command \\sideeffect (or @sideeffect) in the\n"
            "documentation, which will result in a user-defined paragraph with heading\n"
            "\"Side Effects:\". You can put \\n's in the value part of an alias to insert\n"
            "newlines.");

   m_fullHelp.insert("TCL_SUBST",
            "This tag can be used to specify a number of word-keyword mappings (TCL only).\n"
            "A mapping has the form \"name=value\". For example adding \"class=itcl::class\"\n"
            "will allow you to use the command class in the itcl::class meaning.");

   m_fullHelp.insert("OPTIMIZE_OUTPUT_FOR_C",
            "Set the OPTIMIZE_OUTPUT_FOR_C tag to YES if your project consists of C sources\n"
            "only. Doxygen will then generate output that is more tailored for C. For\n"
            "instance, some of the names that are used will be different. The list of all\n"
            "members will be omitted, etc.\n"
            "The default value is: NO.");

   m_fullHelp.insert("OPTIMIZE_OUTPUT_JAVA",
            "Set the OPTIMIZE_OUTPUT_JAVA tag to YES if your project consists of Java or\n"
            "Python sources only. Doxygen will then generate output that is more tailored\n"
            "for that language. For instance, namespaces will be presented as packages,\n"
            "qualified scopes will look different, etc.\n"
            "The default value is: NO.");

   m_fullHelp.insert("OPTIMIZE_FOR_FORTRAN",
            "Set the OPTIMIZE_FOR_FORTRAN tag to YES if your project consists of Fortran\n"
            "sources. Doxygen will then generate output that is tailored for Fortran.\n"
            "The default value is: NO.");

   m_fullHelp.insert("OPTIMIZE_OUTPUT_VHDL",
            "Set the OPTIMIZE_OUTPUT_VHDL tag to YES if your project consists of VHDL\n"
            "sources. Doxygen will then generate output that is tailored for VHDL.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXTENSION_MAPPING",
            "Doxygen selects the parser to use depending on the extension of the files it\n"
            "parses. With this tag you can assign which parser to use for a given\n"
            "extension. Doxygen has a built-in mapping, but you can override or extend it\n"
            "using this tag. The format is ext=language, where ext is a file extension, and\n"
            "language is one of the parsers supported by doxygen: IDL, Java, Javascript,\n"
            "C#, C, C++, D, PHP, Objective-C, Python, Fortran (fixed format Fortran:\n"
            "FortranFixed, free formatted Fortran: FortranFree, unknown formatted Fortran:\n"
            "Fortran. In the later case the parser tries to guess whether the code is fixed\n"
            "or free formatted code, this is the default for Fortran type files), VHDL. For\n"
            "instance to make doxygen treat .inc files as Fortran files (default is PHP),\n"
            "and .f files as C (default is Fortran), use: inc=Fortran f=C.\n"
            "\n"
            "Note: For files without extension you can use no_extension as a placeholder.\n"
            "\n"
            "Note that for custom extensions you also need to set FILE_PATTERNS otherwise\n"
            "the files are not read by doxygen.");

   m_fullHelp.insert("MARKDOWN_SUPPORT",
            "If the MARKDOWN_SUPPORT tag is enabled then doxygen pre-processes all comments\n"
            "according to the Markdown format, which allows for more readable\n"
            "documentation. See http://daringfireball.net/projects/markdown/ for details.\n"
            "The output of markdown processing is further processed by doxygen, so you can\n"
            "mix doxygen, HTML, and XML commands with Markdown formatting. Disable only in\n"
            "case of backward compatibilities issues.\n"
            "The default value is: YES.");

   m_fullHelp.insert("AUTOLINK_SUPPORT",
            "When enabled doxygen tries to link words that correspond to documented\n"
            "classes, or namespaces to their corresponding documentation. Such a link can\n"
            "be prevented in individual cases by putting a % sign in front of the word or\n"
            "globally by setting AUTOLINK_SUPPORT to NO.\n"
            "The default value is: YES.");

   m_fullHelp.insert("BUILTIN_STL_SUPPORT",
            "If you use STL classes (i.e. std::string, std::vector, etc.) but do not want\n"
            "to include (a tag file for) the STL sources as input, then you should set this\n"
            "tag to YES in order to let doxygen match functions declarations and\n"
            "definitions whose arguments contain STL classes (e.g. func(std::string);\n"
            "versus func(std::string) {}). This also make the inheritance and collaboration\n"
            "diagrams that involve STL classes more complete and accurate.\n"
            "The default value is: NO.");

   m_fullHelp.insert("CPP_CLI_SUPPORT",
            "If you use Microsoft's C++/CLI language, you should set this option to YES to\n"
            "enable parsing support.\n"
            "The default value is: NO.");

   m_fullHelp.insert("SIP_SUPPORT",
            "Set the SIP_SUPPORT tag to YES if your project consists of sip (see:\n"
            "http://www.riverbankcomputing.co.uk/software/sip/intro) sources only. Doxygen\n"
            "will parse them like normal C++ but will assume all classes use public instead\n"
            "of private inheritance when no explicit protection keyword is present.\n"
            "The default value is: NO.");

   m_fullHelp.insert("IDL_PROPERTY_SUPPORT",
            "For Microsoft's IDL there are propget and propput attributes to indicate\n"
            "getter and setter methods for a property. Setting this option to YES will make\n"
            "doxygen to replace the get and set methods by a property in the documentation.\n"
            "This will only work if the methods are indeed getting or setting a simple\n"
            "type. If this is not the case, or you want to show the methods anyway, you\n"
            "should set this option to NO.\n"
            "The default value is: YES.");

   m_fullHelp.insert("DISTRIBUTE_GROUP_DOC",
            "If member grouping is used in the documentation and the DISTRIBUTE_GROUP_DOC\n"
            "tag is set to YES then doxygen will reuse the documentation of the first\n"
            "member in the group (if any) for the other members of the group. By default\n"
            "all members of a group must be documented explicitly.\n"
            "The default value is: NO.");

   m_fullHelp.insert("SUBGROUPING",
            "Set the SUBGROUPING tag to YES to allow class member groups of the same type\n"
            "(for instance a group of public functions) to be put as a subgroup of that\n"
            "type (e.g. under the Public Functions section). Set it to NO to prevent\n"
            "subgrouping. Alternatively, this can be done per class using the\n"
            "\\nosubgrouping command.\n"
            "The default value is: YES.");

   m_fullHelp.insert("INLINE_GROUPED_CLASSES",
            "When the INLINE_GROUPED_CLASSES tag is set to YES, classes, structs and unions\n"
            "are shown inside the group in which they are included (e.g. using \\ingroup)\n"
            "instead of on a separate page (for HTML and Man pages) or section (for LaTeX\n"
            "and RTF).\n"
            "\n"
            "Note that this feature does not work in combination with\n"
            "SEPARATE_MEMBER_PAGES.\n"
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_SIMPLE_STRUCTS",
            "When the INLINE_SIMPLE_STRUCTS tag is set to YES, structs, classes, and unions\n"
            "with only public data fields or simple typedef fields will be shown inline in\n"
            "the documentation of the scope in which they are defined (i.e. file,\n"
            "namespace, or group documentation), provided this scope is documented. If set\n"
            "to NO, structs, classes, and unions are shown on a separate page (for HTML and\n"
            "Man pages) or section (for LaTeX and RTF).\n"
            "The default value is: NO.");

   m_fullHelp.insert("TYPEDEF_HIDES_STRUCT",
            "When TYPEDEF_HIDES_STRUCT tag is enabled, a typedef of a struct, union, or\n"
            "enum is documented as struct, union, or enum with the name of the typedef. So\n"
            "typedef struct TypeS {} TypeT, will appear in the documentation as a struct\n"
            "with name TypeT. When disabled the typedef will appear as a member of a file,\n"
            "namespace, or class. And the struct will be named TypeS. This can typically be\n"
            "useful for C code in case the coding convention dictates that all compound\n"
            "types are typedef'ed and only the typedef is referenced, never the tag name.\n"
            "The default value is: NO.");

   m_fullHelp.insert("LOOK_UP_CACHE",
            "The size of the symbol lookup cache can be set using LOOKUP_CACHE_SIZE. This\n"
            "cache is used to resolve symbols given their name and scope. Since this can be\n"
            "an expensive process and often the same symbol appears multiple times in the\n"
            "code, doxygen keeps a cache of pre-resolved symbols. If the cache is too small\n"
            "doxygen will become slower. If the cache is too large, memory is wasted. The\n"
            "cache size is given by this formula: 2^(16+LOOKUP_CACHE_SIZE). The valid range\n"
            "is 0..9, the default is 0, corresponding to a cache size of 2^16=65536\n"
            "symbols. At the end of a run doxygen will report the cache usage and suggest\n"
            "the optimal cache size from a speed point of view.\n"
            "Minimum value: 0, maximum value: 9, default value: 0.");

   m_fullHelp.insert("EXTRACT_ALL",
            "If the EXTRACT_ALL tag is set to YES, doxygen will assume all entities in\n"
            "documentation are documented, even if no documentation was available. Private\n"
            "class members and static file members will be hidden unless the\n"
            "EXTRACT_PRIVATE respectively EXTRACT_STATIC tags are set to YES.\n"
            "Note: This will also disable the warnings about undocumented members that are\n"
            "normally produced when WARNINGS is set to YES.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_PRIVATE",
            "If the EXTRACT_PRIVATE tag is set to YES, all private members of a class will\n"
            "be included in the documentation.\n""The default value is: NO.");

   m_fullHelp.insert("EXTRACT_PACKAGE",
            "If the EXTRACT_PACKAGE tag is set to YES, all members with package or internal\n"
            "scope will be included in the documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_STATIC",
            "If the EXTRACT_STATIC tag is set to YES, all static members of a file will be\n"
            "included in the documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_LOCAL_CLASSES",
            "If the EXTRACT_LOCAL_CLASSES tag is set to YES, classes (and structs) defined\n"
            "locally in source files will be included in the documentation. If set to NO,\n"
            "only classes defined in header files are included. Does not have any effect\n"
            "for Java sources.\n"
            "The default value is: YES.");

   m_fullHelp.insert("EXTRACT_LOCAL_METHODS",
            "This flag is only useful for Objective-C code. If set to YES, local methods,\n"
            "which are defined in the implementation section but not in the interface are\n"
            "included in the documentation. If set to NO, only methods in the interface are\n"
            "included.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXTRACT_ANON_NSPACES",
            "If this flag is set to YES, the members of anonymous namespaces will be\n"
            "extracted and appear in the documentation as a namespace called\n"
            "'anonymous_namespace{file}', where file will be replaced with the base name of\n"
            "the file that contains the anonymous namespace. By default anonymous namespace\n"
            "are hidden.\n"
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_UNDOC_MEMBERS",
            "If the HIDE_UNDOC_MEMBERS tag is set to YES, doxygen will hide all\n"
            "undocumented members inside documented classes or files. If set to NO these\n"
            "members will be included in the various overviews, but no documentation\n"
            "section is generated. This option has no effect if EXTRACT_ALL is enabled.\n"
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_UNDOC_CLASSES",
            "If the HIDE_UNDOC_CLASSES tag is set to YES, doxygen will hide all\n"
            "undocumented classes that are normally visible in the class hierarchy. If set\n"
            "to NO, these classes will be included in the various overviews. This option\n"
            "has no effect if EXTRACT_ALL is enabled.\n"
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_FRIEND_COMPOUNDS",
            "If the HIDE_FRIEND_COMPOUNDS tag is set to YES, doxygen will hide all friend\n"
            "(class|struct|union) declarations. If set to NO, these declarations will be\n"
            "included in the documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_IN_BODY_DOCS",
            "If the HIDE_IN_BODY_DOCS tag is set to YES, doxygen will hide any\n"
            "documentation blocks found inside the body of a function. If set to NO, these\n"
            "blocks will be appended to the function's detailed documentation block.\n"
            "The default value is: NO.");

   m_fullHelp.insert("INTERNAL_DOCS",
            "The INTERNAL_DOCS tag determines if documentation that is typed after a\n"
            "\\internal command is included. If the tag is set to NO then the documentation\n"
            "will be excluded. Set it to YES to include the internal documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("CASE_SENSE_NAMES",
            "If the CASE_SENSE_NAMES tag is set to NO then doxygen will only generate file\n"
            "names in lower-case letters. If set to YES, upper-case letters are also\n"
            "allowed. This is useful if you have classes or files whose names only differ\n"
            "in case and if your file system supports case sensitive file names. Windows\n"
            "and Mac users are advised to set this option to NO.\n"
            "The default value is: system dependent.");

   m_fullHelp.insert("HIDE_SCOPE_NAMES",
            "If the HIDE_SCOPE_NAMES tag is set to NO then doxygen will show members with\n"
            "their full class and namespace scopes in the documentation. If set to YES, the\n"
            "scope will be hidden.\n"
            "The default value is: NO.");

   m_fullHelp.insert("HIDE_COMPOUND_REFERENCE",
            "If the HIDE_COMPOUND_REFERENCE tag is set to NO (default) then doxygen will\n"
            "append additional text to a page's title, such as Class Reference. If set to\n"
            "YES the compound reference will be hidden.\n"
            "The default value is: NO.");

   m_fullHelp.insert("SHOW_INCLUDE_FILES",
            "If the SHOW_INCLUDE_FILES tag is set to YES then doxygen will put a list of\n"
            "the files that are included by a file in the documentation of that file.\n"
            "The default value is: YES.");

   m_fullHelp.insert("SHOW_GROUPED_MEMB_INC",
            "If the SHOW_GROUPED_MEMB_INC tag is set to YES then Doxygen will add for each\n"
            "grouped member an include statement to the documentation, telling the reader\n"
            "which file to include in order to use the member.\n"
            "The default value is: NO.");

   m_fullHelp.insert("FORCE_LOCAL_INCLUDES",
            "If the FORCE_LOCAL_INCLUDES tag is set to YES then doxygen will list include\n"
            "files with double quotes in the documentation rather than with sharp brackets.\n"
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_INFO",
            "If the INLINE_INFO tag is set to YES then a tag [inline] is inserted in the\n"
            "documentation for inline members.\n"
            "The default value is: YES.");

   m_fullHelp.insert("SORT_MEMBER_DOCS",
            "If the SORT_MEMBER_DOCS tag is set to YES then doxygen will sort the\n"
            "(detailed) documentation of file and class members alphabetically by member\n"
            "name. If set to NO, the members will appear in declaration order.\n"
            "The default value is: YES.");

   m_fullHelp.insert("SORT_BRIEF_DOCS",
            "If the SORT_BRIEF_DOCS tag is set to YES then doxygen will sort the brief\n"
            "descriptions of file, namespace and class members alphabetically by member\n"
            "name. If set to NO, the members will appear in declaration order. Note that\n"
            "this will also influence the order of the classes in the class list.\n"
            "The default value is: NO.");

   m_fullHelp.insert("SORT_MEMBERS_CTORS_1ST",
            "If the SORT_MEMBERS_CTORS_1ST tag is set to YES then doxygen will sort the\n"
            "(brief and detailed) documentation of class members so that constructors and\n"
            "destructors are listed first. If set to NO the constructors will appear in the\n"
            "respective orders defined by SORT_BRIEF_DOCS and SORT_MEMBER_DOCS.\n"
            "Note: If SORT_BRIEF_DOCS is set to NO this option is ignored for sorting brief\n"
            "member documentation.\n"
            "Note: If SORT_MEMBER_DOCS is set to NO this option is ignored for sorting\n"
            "detailed member documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("SORT_GROUP_NAMES",
            "If the SORT_GROUP_NAMES tag is set to YES then doxygen will sort the hierarchy\n"
            "of group names into alphabetical order. If set to NO the group names will\n"
            "appear in their defined order.\n"
            "The default value is: NO.");

   m_fullHelp.insert("SORT_BY_SCOPE_NAME",
            "If the SORT_BY_SCOPE_NAME tag is set to YES, the class list will be sorted by\n"
            "fully-qualified names, including namespaces. If set to NO, the class list will\n"
            "be sorted only by class name, not including the namespace part.\n"
            "Note: This option is not very useful if HIDE_SCOPE_NAMES is set to YES.\n"
            "Note: This option applies only to the class list, not to the alphabetical\n"
            "list.\n"
            "The default value is: NO.");

   m_fullHelp.insert("STRICT_PROTO_MATCHING",
            "If the STRICT_PROTO_MATCHING option is enabled and doxygen fails to do proper\n"
            "type resolution of all parameters of a function it will reject a match between\n"
            "the prototype and the implementation of a member function even if there is\n"
            "only one candidate or it is obvious which candidate to choose by doing a\n"
            "simple string match. By disabling STRICT_PROTO_MATCHING doxygen will still\n"
            "accept a match between prototype and implementation in such cases.\n"
            "The default value is: NO.");

   m_fullHelp.insert("GENERATE_TODOLIST",
            "The GENERATE_TODOLIST tag can be used to enable (YES) or disable (NO) the todo\n"
            "list. This list is created by putting \\todo commands in the documentation.\n"
            "The default value is: YES.");

   m_fullHelp.insert("GENERATE_TESTLIST",
            "The GENERATE_TESTLIST tag can be used to enable (YES) or disable (NO) the test\n"
            "list. This list is created by putting \\test commands in the documentation.\n"
            "The default value is: YES.");

   m_fullHelp.insert("GENERATE_BUGLIST",
            "The GENERATE_BUGLIST tag can be used to enable (YES) or disable (NO) the bug\n"
            "list. This list is created by putting \\bug commands in the documentation.\n"
            "The default value is: YES.");

   m_fullHelp.insert("GENERATE_DEPRECATEDLIST",
            "The GENERATE_DEPRECATEDLIST tag can be used to enable (YES) or disable (NO)\n"
            "the deprecated list. This list is created by putting \\deprecated commands in\n"
            "the documentation.\n"
            "The default value is: YES.");

   m_fullHelp.insert("ENABLED_SECTIONS",
            "The ENABLED_SECTIONS tag can be used to enable conditional documentation\n"
            "sections, marked by \\if <section_label> ... \\endif and \\cond <section_label>\n"
            "... \\endcond blocks.");

   m_fullHelp.insert("MAX_INIT_LINES",
            "The MAX_INITIALIZER_LINES tag determines the maximum number of lines that the\n"
            "initial value of a variable or macro / define can have for it to appear in the\n"
            "documentation. If the initializer consists of more lines than specified here\n"
            "it will be hidden. Use a value of 0 to hide initializers completely. The\n"
            "appearance of the value of individual variables and macros / defines can be\n"
            "controlled using \\showinitializer or \\hideinitializer command in the\n"
            "documentation regardless of this setting.\n"
            "Minimum value: 0, maximum value: 10000, default value: 30.");

   m_fullHelp.insert("SHOW_USED_FILES",
            "Set the SHOW_USED_FILES tag to NO to disable the list of files generated at\n"
            "the bottom of the documentation of classes and structs. If set to YES, the\n"
            "list will mention the files that were used to generate the documentation.\n"
            "The default value is: YES.");

   m_fullHelp.insert("SHOW_FILES",
            "Set the SHOW_FILES tag to NO to disable the generation of the Files page. This\n"
            "will remove the Files entry from the Quick Index and from the Folder Tree View\n"
            "(if specified).\n"
            "The default value is: YES.");

   m_fullHelp.insert("SHOW_NAMESPACES",
            "Set the SHOW_NAMESPACES tag to NO to disable the generation of the Namespaces\n"
            "page. This will remove the Namespaces entry from the Quick Index and from the\n"
            "Folder Tree View (if specified).\n"
            "The default value is: YES.");

   m_fullHelp.insert("FILE_VERSION_FILTER",
            "The FILE_VERSION_FILTER tag can be used to specify a program or script that\n"
            "doxygen should invoke to get the current version for each file (typically from\n"
            "the version control system). Doxygen will invoke the program by executing (via\n"
            "popen()) the command command input-file, where command is the value of the\n"
            "FILE_VERSION_FILTER tag, and input-file is the name of an input file provided\n"
            "by doxygen. Whatever the program writes to standard output is used as the file\n"
            "version. For an example see the documentation.");

   m_fullHelp.insert("LAYOUT_FILE",
            "The LAYOUT_FILE tag can be used to specify a layout file which will be parsed\n"
            "by doxygen. The layout file controls the global structure of the generated\n"
            "output files in an output format independent way. To create the layout file\n"
            "that represents doxygen's defaults, run doxygen with the -l option. You can\n"
            "optionally specify a file name after the option, if omitted DoxygenLayout.xml\n"
            "will be used as the name of the layout file.\n"
            "\n"
            "Note that if you run doxygen from a directory containing a file called\n"
            "DoxygenLayout.xml, doxygen will parse it automatically even if the LAYOUT_FILE\n"
            "tag is left empty.");

   m_fullHelp.insert("CITE_BIB_FILES",
            "The CITE_BIB_FILES tag can be used to specify one or more bib files containing\n"
            "the reference definitions. This must be a list of .bib files. The .bib\n"
            "extension is automatically appended if omitted. This requires the bibtex tool\n"
            "to be installed. See also http://en.wikipedia.org/wiki/BibTeX for more info.\n"
            "For LaTeX the style of the bibliography can be controlled using\n"
            "LATEX_BIB_STYLE. To use this feature you need bibtex and perl available in the\n"
            "search path. See also \\cite for info how to create references.");

   m_fullHelp.insert("QUIET",
            "The QUIET tag can be used to turn on/off the messages that are generated to\n"
            "standard output by doxygen. If QUIET is set to YES this implies that the\n"
            "messages are off.\n"
            "The default value is: NO.");

   m_fullHelp.insert("WARNINGS",
            "The WARNINGS tag can be used to turn on/off the warning messages that are\n"
            "generated to standard error (stderr) by doxygen. If WARNINGS is set to YES\n"
            "this implies that the warnings are on.\n"
            "\n"
            "Tip: Turn warnings on while writing the documentation.\n"
            "The default value is: YES.");

   m_fullHelp.insert("WARN_IF_UNDOCUMENTED",
            "If the WARN_IF_UNDOCUMENTED tag is set to YES then doxygen will generate\n"
            "warnings for undocumented members. If EXTRACT_ALL is set to YES then this flag\n"
            "will automatically be disabled.\n"
            "The default value is: YES.");

   m_fullHelp.insert("WARN_IF_DOC_ERROR",
            "If the WARN_IF_DOC_ERROR tag is set to YES, doxygen will generate warnings for\n"
            "potential errors in the documentation, such as not documenting some parameters\n"
            "in a documented function, or documenting parameters that don't exist or using\n"
            "markup commands wrongly.\n"
            "The default value is: YES.");

   m_fullHelp.insert("WARN_NO_PARAMDOC",
            "This WARN_NO_PARAMDOC option can be enabled to get warnings for functions that\n"
            "are documented, but have no documentation for their parameters or return\n"
            "value. If set to NO, doxygen will only warn about wrong or incomplete\n"
            "parameter documentation, but not about the absence of documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("WARN_FORMAT",
            "The WARN_FORMAT tag determines the format of the warning messages that doxygen\n"
            "can produce. The string should contain the $file, $line, and $text tags, which\n"
            "will be replaced by the file and line number from which the warning originated\n"
            "and the warning text. Optionally the format may contain $version, which will\n"
            "be replaced by the version of the file (if it could be obtained via\n"
            "FILE_VERSION_FILTER)\n"
            "The default value is: $file:$line: $text.");

   m_fullHelp.insert("WARN_LOGFILE",
            "The WARN_LOGFILE tag can be used to specify a file to which warning and error\n"
            "messages should be written. If left blank the output is written to standard\n"
            "error (stderr).");

   m_fullHelp.insert("INPUT",
            "The INPUT tag is used to specify the files and/or directories that contain\n"
            "documented source files. You may enter file names like myfile.cpp or\n"
            "directories like /usr/src/myproject. Separate the files or directories with\n"
            "spaces.\n"
            "Note: If this tag is empty the current directory is searched.");

   m_fullHelp.insert("INPUT_ENCODING",
            "This tag can be used to specify the character encoding of the source files\n"
            "that doxygen parses. Internally doxygen uses the UTF-8 encoding. Doxygen uses\n"
            "libiconv (or the iconv built into libc) for the transcoding. See the libiconv\n"
            "documentation (see: http://www.gnu.org/software/libiconv) for the list of\n"
            "possible encodings.\n"
            "The default value is: UTF-8.");

   m_fullHelp.insert("FILE_PATTERNS",
            "If the value of the INPUT tag contains directories, you can use the\n"
            "FILE_PATTERNS tag to specify one or more wildcard patterns (like *.cpp and\n"
            "*.h) to filter out the source-files in the directories. If left blank the\n"
            "following patterns are tested:*.c, *.cc, *.cxx, *.cpp, *.c++, *.java, *.ii,\n"
            "*.ixx, *.ipp, *.i++, *.inl, *.idl, *.ddl, *.odl, *.h, *.hh, *.hxx, *.hpp,\n"
            "*.h++, *.cs, *.d, *.php, *.php4, *.php5, *.phtml, *.inc, *.m, *.markdown,\n"
            "*.md, *.mm, *.dox, *.py, *.f90, *.f, *.for, *.tcl, *.vhd, *.vhdl, *.ucf,\n"
            "*.qsf, *.as and *.js.");

   m_fullHelp.insert("RECURSIVE",
            "The RECURSIVE tag can be used to specify whether or not subdirectories should\n"
            "be searched for input files as well.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXCLUDE",
            "The EXCLUDE tag can be used to specify files and/or directories that should be\n"
            "excluded from the INPUT source files. This way you can easily exclude a\n"
            "subdirectory from a directory tree whose root is specified with the INPUT tag.\n"
            "\n"
            "Note that relative paths are relative to the directory from which doxygen is\n"
            "run.");

   m_fullHelp.insert("EXCLUDE_SYMLINKS",
            "The EXCLUDE_SYMLINKS tag can be used to select whether or not files or\n"
            "directories that are symbolic links (a Unix file system feature) are excluded\n"
            "from the input.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXCLUDE_PATTERNS",
            "If the value of the INPUT tag contains directories, you can use the\n"
            "EXCLUDE_PATTERNS tag to specify one or more wildcard patterns to exclude\n"
            "certain files from those directories.\n"
            "\n"
            "Note that the wildcards are matched against the file with absolute path, so to\n"
            "exclude all test directories for example use the pattern */test/*");   

   m_fullHelp.insert("EXCLUDE_SYMBOLS",
            "The EXCLUDE_SYMBOLS tag can be used to specify one or more symbol names\n"
            "(namespaces, classes, functions, etc.) that should be excluded from the\n"
            "output. The symbol name can be a fully qualified name, a word, or if the\n"
            "wildcard * is used, a substring. Examples: ANamespace, AClass,\n"
            "AClass::ANamespace, ANamespace::*Test\n"
            "\n"
            "Note that the wildcards are matched against the file with absolute path, so to\n"
            "exclude all test directories use the pattern */test/*");     

   m_fullHelp.insert("EXAMPLE_PATH",
            "The EXAMPLE_PATH tag can be used to specify one or more files or directories\n"
            "that contain example code fragments that are included (see the \\include\n"
            "command).");

   m_fullHelp.insert("EXAMPLE_PATTERNS",
            "If the value of the EXAMPLE_PATH tag contains directories, you can use the\n"
            "EXAMPLE_PATTERNS tag to specify one or more wildcard pattern (like *.cpp and\n"
            "*.h) to filter out the source-files in the directories. If left blank all\n"
            "files are included.");

   m_fullHelp.insert("EXAMPLE_RECURSIVE",
            "If the EXAMPLE_RECURSIVE tag is set to YES then subdirectories will be\n"
            "searched for input files to be used with the \\include or \\dontinclude commands\n"
            "irrespective of the value of the RECURSIVE tag.\n"
            "The default value is: NO.");

   m_fullHelp.insert("IMAGE_PATH",
            "The IMAGE_PATH tag can be used to specify one or more files or directories\n"
            "that contain images that are to be included in the documentation (see the\n"
            "\\image command).");

   m_fullHelp.insert("INPUT_FILTER",
            "The INPUT_FILTER tag can be used to specify a program that doxygen should\n"
            "invoke to filter for each input file. Doxygen will invoke the filter program\n"
            "by executing (via popen()) the command:\n"
            "\n"
            "<filter> <input-file>\n"
            "\n"
            "where <filter> is the value of the INPUT_FILTER tag, and <input-file> is the\n"
            "name of an input file. Doxygen will then use the output that the filter\n"
            "program writes to standard output. If FILTER_PATTERNS is specified, this tag\n"
            "will be ignored.\n"
            "\n"
            "Note that the filter must not add or remove lines; it is applied before the\n"
            "code is scanned, but not when the output code is generated. If lines are added\n"
            "or removed, the anchors will not be placed correctly.");

   m_fullHelp.insert("FILTER_PATTERNS",
            "The FILTER_PATTERNS tag can be used to specify filters on a per file pattern\n"
            "basis. Doxygen will compare the file name with each pattern and apply the\n"
            "filter if there is a match. The filters are a list of the form: pattern=filter\n"
            "(like *.cpp=my_cpp_filter). See INPUT_FILTER for further information on how\n"
            "filters are used. If the FILTER_PATTERNS tag is empty or if none of the\n"
            "patterns match the file name, INPUT_FILTER is applied.");

   m_fullHelp.insert("FILTER_SOURCE_FILES",
            "If the FILTER_SOURCE_FILES tag is set to YES, the input filter (if set using\n"
            "INPUT_FILTER) will also be used to filter the input files that are used for\n"
            "producing the source files to browse (i.e. when SOURCE_BROWSER is set to YES).\n"
            "The default value is: NO.");

   m_fullHelp.insert("FILTER_SOURCE_PATTERNS",
            "The FILTER_SOURCE_PATTERNS tag can be used to specify source filters per file\n"
            "pattern. A pattern will override the setting for FILTER_PATTERN (if any) and\n"
            "it is also possible to disable source filtering for a specific pattern using\n"
            "*.ext= (so without naming a filter).\n"
            "This tag requires that the tag FILTER_SOURCE_FILES is set to YES.");

   m_fullHelp.insert("USE_MDFILE_AS_MAINPAGE",
            "If the USE_MDFILE_AS_MAINPAGE tag refers to the name of a markdown file that\n"
            "is part of the input, its contents will be placed on the main page\n"
            "(index.html). This can be useful if you have a project on for instance GitHub\n"
            "and want to reuse the introduction page also for the doxygen output.");

   m_fullHelp.insert("SOURCE_BROWSER",
            "If the SOURCE_BROWSER tag is set to YES then a list of source files will be\n"
            "generated. Documented entities will be cross-referenced with these sources.\n"
            "\n"
            "Note: To get rid of all source code in the generated output, make sure that\n"
            "also VERBATIM_HEADERS is set to NO.\n"
            "The default value is: NO.");

   m_fullHelp.insert("INLINE_SOURCES",
            "Setting the INLINE_SOURCES tag to YES will include the body of functions,\n"
            "classes and enums directly into the documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("STRIP_CODE_COMMENTS",
            "Setting the STRIP_CODE_COMMENTS tag to YES will instruct doxygen to hide any\n"
            "special comment blocks from generated source code fragments. Normal C, C++ and\n"
            "Fortran comments will always remain visible.\n"
            "The default value is: YES.");

   m_fullHelp.insert("REFERENCED_BY_RELATION",
            "If the REFERENCED_BY_RELATION tag is set to YES then for each documented\n"
            "function all documented functions referencing it will be listed.\n"
            "The default value is: NO.");

   m_fullHelp.insert("REFERENCES_RELATION",
            "If the REFERENCES_RELATION tag is set to YES then for each documented function\n"
            "all documented entities called/used by that function will be listed.\n"
            "The default value is: NO.");

   m_fullHelp.insert("REFERENCES_LINK_SOURCE",
            "If the REFERENCES_LINK_SOURCE tag is set to YES and SOURCE_BROWSER tag is set\n"
            "to YES then the hyperlinks from functions in REFERENCES_RELATION and\n"
            "REFERENCED_BY_RELATION lists will link to the source code. Otherwise they will\n"
            "link to the documentation.\n"
            "The default value is: YES.");

   m_fullHelp.insert("SOURCE_TOOLTIPS",
            "If SOURCE_TOOLTIPS is enabled (the default) then hovering a hyperlink in the\n"
            "source code will show a tooltip with additional information such as prototype,\n"
            "brief description and links to the definition and documentation. Since this\n"
            "will make the HTML file larger and loading of large files a bit slower, you\n"
            "can opt to disable this feature.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag SOURCE_BROWSER is set to YES.");

   m_fullHelp.insert("USE_HTAGS",
            "If the USE_HTAGS tag is set to YES then the references to source code will\n"
            "point to the HTML generated by the htags(1) tool instead of doxygen built-in\n"
            "source browser. The htags tool is part of GNU's global source tagging system\n"
            "(see http://www.gnu.org/software/global/global.html). You will need version\n"
            "4.8.6 or higher.\n"
            "\n"
            "To use it do the following:\n"
            "- Install the latest version of global\n"
            "- Enable SOURCE_BROWSER and USE_HTAGS in the config file\n"
            "- Make sure the INPUT points to the root of the source tree\n"
            "- Run doxygen as normal\n"
            "\n"
            "Doxygen will invoke htags (and that will in turn invoke gtags), so these\n"
            "tools must be available from the command line (i.e. in the search path).\n"
            "\n"
            "The result: instead of the source browser generated by doxygen, the links to\n"
            "source code will now point to the output of htags.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag SOURCE_BROWSER is set to YES.");

   m_fullHelp.insert("VERBATIM_HEADERS",
            "If the VERBATIM_HEADERS tag is set the YES then doxygen will generate a\n"
            "verbatim copy of the header file for each class for which an include is\n"
            "specified. Set to NO to disable this.\n"
            "See also: Section \\class.\n"
            "The default value is: YES.");

   m_fullHelp.insert("CLANG_ASSISTED_PARSING",
            "If the CLANG_ASSISTED_PARSING tag is set to YES then doxygen will use the\n"
            "clang parser (see: http://clang.llvm.org/) for more accurate parsing at the\n"
            "cost of reduced performance. This can be particularly helpful with template\n"
            "rich C++ code for which doxygen's built-in parser lacks the necessary type\n"
            "information.\n"
            "Note: The availability of this option depends on whether or not doxygen was\n"
            "compiled with the --with-libclang option.\n"
            "The default value is: NO.");

   m_fullHelp.insert("CLANG_OPTIONS",
            "If clang assisted parsing is enabled you can provide the compiler with command\n"
            "line options that you would normally use when invoking the compiler. Note that\n"
            "the include paths will already be set by doxygen for the files and directories\n"
            "specified with INPUT and INCLUDE_PATH.\n"
            "This tag requires that the tag CLANG_ASSISTED_PARSING is set to YES.");

   m_fullHelp.insert("ALPHABETICAL_INDEX",
            "If the ALPHABETICAL_INDEX tag is set to YES, an alphabetical index of all\n"
            "compounds will be generated. Enable this if the project contains a lot of\n"
            "classes, structs, unions or interfaces.\n"
            "The default value is: YES.");

   m_fullHelp.insert("COLS_IN_ALPHA_INDEX",
            "The COLS_IN_ALPHA_INDEX tag can be used to specify the number of columns in\n"
            "which the alphabetical index list will be split.\n"
            "Minimum value: 1, maximum value: 20, default value: 5.\n"
            "This tag requires that the tag ALPHABETICAL_INDEX is set to YES.");

   m_fullHelp.insert("IGNORE_PREFIX",
            "In case all classes in a project start with a common prefix, all classes will\n"
            "be put under the same header in the alphabetical index. The IGNORE_PREFIX tag\n"
            "can be used to specify a prefix (or a list of prefixes) that should be ignored\n"
            "while generating the index headers.\n"
            "This tag requires that the tag ALPHABETICAL_INDEX is set to YES.");

   m_fullHelp.insert("GENERATE_HTML",
            "If the GENERATE_HTML tag is set to YES, doxygen will generate HTML output\n"
            "The default value is: YES.");

   m_fullHelp.insert("HTML_OUTPUT",
            "The HTML_OUTPUT tag is used to specify where the HTML docs will be put. If a\n"
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of\n"
            "it.\n"
            "The default directory is: html.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_FILE_EXTENSION",
            "The HTML_FILE_EXTENSION tag can be used to specify the file extension for each\n"
            "generated HTML page (for example: .htm, .php, .asp).\n"
            "The default value is: .html.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_HEADER",
            "The HTML_HEADER tag can be used to specify a user-defined HTML header file for\n"
            "each generated HTML page. If the tag is left blank doxygen will generate a\n"
            "standard header.\n"
            "\n"
            "To get valid HTML the header file that includes any scripts and style sheets\n"
            "that doxygen needs, which is dependent on the configuration options used (e.g.\n"
            "the setting GENERATE_TREEVIEW). It is highly recommended to start with a\n"
            "default header using\n"
            "doxygen -w html new_header.html new_footer.html new_stylesheet.css\n"
            "YourConfigFile\n"
            "and then modify the file new_header.html. See also section \"Doxygen usage\"\n"
            "for information on how to generate the default header that doxygen normally\n"
            "uses.\n"
            "Note: The header is subject to change so you typically have to regenerate the\n"
            "default header when upgrading to a newer version of doxygen. For a description\n"
            "of the possible markers and block names see the documentation.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_FOOTER",
            "The HTML_FOOTER tag can be used to specify a user-defined HTML footer for each\n"
            "generated HTML page. If the tag is left blank doxygen will generate a standard\n"
            "footer. See HTML_HEADER for more information on how to generate a default\n"
            "footer and what special commands can be used inside the footer. See also\n"
            "section \"Doxygen usage\" for information on how to generate the default footer\n"
            "that doxygen normally uses.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_STYLESHEET",
            "The HTML_STYLESHEET tag can be used to specify a user-defined cascading style\n"
            "sheet that is used by each HTML page. It can be used to fine-tune the look of\n"
            "the HTML output. If left blank doxygen will generate a default style sheet.\n"
            "See also section \"Doxygen usage\" for information on how to generate the style\n"
            "sheet that doxygen normally uses.\n"
            "Note: It is recommended to use HTML_EXTRA_STYLESHEET instead of this tag, as\n"
            "it is more robust and this tag (HTML_STYLESHEET) will in the future become\n"
            "obsolete.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_EXTRA_STYLESHEET",
            "The HTML_EXTRA_STYLESHEET tag can be used to specify additional user-defined\n"
            "cascading style sheets that are included after the standard style sheets\n"
            "created by doxygen. Using this option one can overrule certain style aspects.\n"
            "This is preferred over using HTML_STYLESHEET since it does not replace the\n"
            "standard style sheet and is therefore more robust against future updates.\n"
            "Doxygen will copy the style sheet files to the output directory.\n"
            "Note: The order of the extra stylesheet files is of importance (e.g. the last\n"
            "stylesheet in the list overrules the setting of the previous ones in the\n"
            "list). For an example see the documentation.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_EXTRA_FILES",
            "The HTML_EXTRA_FILES tag can be used to specify one or more extra images or\n"
            "other source files which should be copied to the HTML output directory. Note\n"
            "that these files will be copied to the base HTML output directory. Use the\n"
            "$relpath^ marker in the HTML_HEADER and/or HTML_FOOTER files to load these\n"
            "files. In the HTML_STYLESHEET file, use the file name only. Also note that the\n"
            "files will be copied as-is; there are no commands or markers available.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_COLORSTYLE_HUE",
            "The HTML_COLORSTYLE_HUE tag controls the color of the HTML output. Doxygen\n"
            "will adjust the colors in the stylesheet and background images according to\n"
            "this color. Hue is specified as an angle on a colorwheel, see\n"
            "http://en.wikipedia.org/wiki/Hue for more information. For instance the value\n"
            "0 represents red, 60 is yellow, 120 is green, 180 is cyan, 240 is blue, 300\n"
            "purple, and 360 is red again.\n"
            "Minimum value: 0, maximum value: 359, default value: 220.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_COLORSTYLE_SAT",
            "The HTML_COLORSTYLE_SAT tag controls the purity (or saturation) of the colors\n"
            "in the HTML output. For a value of 0 the output will use grayscales only. A\n"
            "value of 255 will produce the most vivid colors.\n"
            "Minimum value: 0, maximum value: 255, default value: 100.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_COLORSTYLE_GAMMA",
            "The HTML_COLORSTYLE_GAMMA tag controls the gamma correction applied to the\n"
            "luminance component of the colors in the HTML output. Values below 100\n"
            "gradually make the output lighter, whereas values above 100 make the output\n"
            "darker. The value divided by 100 is the actual gamma applied, so 80 represents\n"
            "a gamma of 0.8, The value 220 represents a gamma of 2.2, and 100 does not\n"
            "change the gamma.\n"
            "Minimum value: 40, maximum value: 240, default value: 80.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_TIMESTAMP",
            "If the HTML_TIMESTAMP tag is set to YES then the footer of each generated HTML\n"
            "page will contain the date and time when the page was generated. Setting this\n"
            "to NO can help when comparing the output of multiple runs.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_DYNAMIC_SECTIONS",
            "If the HTML_DYNAMIC_SECTIONS tag is set to YES then the generated HTML\n"
            "documentation will contain sections that can be hidden and shown after the\n"
            "page has loaded.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("HTML_INDEX_NUM_ENTRIES",
            "With HTML_INDEX_NUM_ENTRIES one can control the preferred number of entries\n"
            "shown in the various tree structured indices initially; the user can expand\n"
            "and collapse entries dynamically later on. Doxygen will expand the tree to\n"
            "such a level that at most the specified number of entries are visible (unless\n"
            "a fully collapsed tree already exceeds this amount). So setting the number of\n"
            "entries 1 will produce a full collapsed tree by default. 0 is a special value\n"
            "representing an infinite number of entries and will result in a full expanded\n"
            "tree by default.\n"
            "Minimum value: 0, maximum value: 9999, default value: 100.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("GENERATE_DOCSET",
            "If the GENERATE_DOCSET tag is set to YES, additional index files will be\n"
            "generated that can be used as input for Apple's Xcode 3 integrated development\n"
            "environment (see: http://developer.apple.com/tools/xcode/), introduced with\n"
            "OSX 10.5 (Leopard). To create a documentation set, doxygen will generate a\n"
            "Makefile in the HTML output directory. Running make will produce the docset in\n"
            "that directory and running make install will install the docset in\n"
            "~/Library/Developer/Shared/Documentation/DocSets so that Xcode will find it at\n"
            "startup. See http://developer.apple.com/tools/creatingdocsetswithdoxygen.html\n"
            "for more information.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("DOCSET_FEEDNAME",
            "This tag determines the name of the docset feed. A documentation feed provides\n"
            "an umbrella under which multiple documentation sets from a single provider\n"
            "(such as a company or product suite) can be grouped.\n"
            "The default value is: Doxygen generated docs.\n"
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("DOCSET_BUNDLE_ID",
            "This tag specifies a string that should uniquely identify the documentation\n"
            "set bundle. This should be a reverse domain-name style string, e.g.\n"
            "com.mycompany.MyDocSet. Doxygen will append .docset to the name.\n"
            "The default value is: org.doxygen.Project.\n"
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("DOCSET_PUBLISHER_ID",
            "The DOCSET_PUBLISHER_ID tag specifies a string that should uniquely identify\n"
            "the documentation publisher. This should be a reverse domain-name style\n"
            "string, e.g. com.mycompany.MyDocSet.documentation.\n"
            "The default value is: org.doxygen.Publisher.\n"
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("DOCSET_PUBLISHER_NAME",
            "The DOCSET_PUBLISHER_NAME tag identifies the documentation publisher.\n"
            "The default value is: Publisher.\n"
            "This tag requires that the tag GENERATE_DOCSET is set to YES.");

   m_fullHelp.insert("GENERATE_HTMLHELP",
            "If the GENERATE_HTMLHELP tag is set to YES then doxygen generates three\n"
            "additional HTML index files: index.hhp, index.hhc, and index.hhk. The\n"
            "index.hhp is a project file that can be read by Microsoft's HTML Help Workshop\n"
            "(see: http://www.microsoft.com/en-us/download/details.aspx?id=21138) on\n"
            "Windows.\n"
            "\n"
            "The HTML Help Workshop contains a compiler that can convert all HTML output\n"
            "generated by doxygen into a single compiled HTML file (.chm). Compiled HTML\n"
            "files are now used as the Windows 98 help format, and will replace the old\n"
            "Windows help format (.hlp) on all Windows platforms in the future. Compressed\n"
            "HTML files also contain an index, a table of contents, and you can search for\n"
            "words in the documentation. The HTML workshop also contains a viewer for\n"
            "compressed HTML files.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("CHM_FILE",
            "The CHM_FILE tag can be used to specify the file name of the resulting .chm\n"
            "file. You can add a path in front of the file if the result should not be\n"
            "written to the html output directory.\n"
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("HHC_LOCATION",
            "The HHC_LOCATION tag can be used to specify the location (absolute path\n"
            "including file name) of the HTML help compiler (hhc.exe). If non-empty,\n"
            "doxygen will try to run the HTML help compiler on the generated index.hhp.\n"
            "The file has to be specified with full path.\n"
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("GENERATE_CHI",
            "The GENERATE_CHI flag controls if a separate .chi index file is generated\n"
            "(YES) or that it should be included in the master .chm file (NO).\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("CHM_INDEX_ENCODING",
            "The CHM_INDEX_ENCODING is used to encode HtmlHelp index (hhk), content (hhc)\n"
            "and project file content.\n"
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("BINARY_TOC",
            "The BINARY_TOC flag controls whether a binary table of contents is generated\n"
            "(YES) or a normal table of contents (NO) in the .chm file. Furthermore it\n"
            "enables the Previous and Next buttons.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("TOC_EXPAND",
            "The TOC_EXPAND flag can be set to YES to add extra items for group members to\n"
            "the table of contents of the HTML help documentation and to the tree view.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTMLHELP is set to YES.");

   m_fullHelp.insert("GENERATE_QHP",
            "If the GENERATE_QHP tag is set to YES and both QHP_NAMESPACE and\n"
            "QHP_VIRTUAL_FOLDER are set, an additional index file will be generated that\n"
            "can be used as input for Qt's qhelpgenerator to generate a Qt Compressed Help\n"
            "(.qch) of the generated HTML documentation.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("QCH_FILE",
            "If the QHG_LOCATION tag is specified, the QCH_FILE tag can be used to specify\n"
            "the file name of the resulting .qch file. The path specified is relative to\n"
            "the HTML output folder.\n"
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_NAMESPACE",
            "The QHP_NAMESPACE tag specifies the namespace to use when generating Qt Help\n"
            "Project output. For more information please see Qt Help Project / Namespace\n"
            "(see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#namespace).\n"
            "The default value is: org.doxygen.Project.\n"
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_VIRTUAL_FOLDER",
            "The QHP_VIRTUAL_FOLDER tag specifies the namespace to use when generating Qt\n"
            "Help Project output. For more information please see Qt Help Project / Virtual\n"
            "Folders (see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#virtual-\n"
            "folders).\n"
            "The default value is: doc.\n"
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_CUST_FILTER_NAME",
            "If the QHP_CUST_FILTER_NAME tag is set, it specifies the name of a custom\n"
            "filter to add. For more information please see Qt Help Project / Custom\n"
            "Filters (see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#custom-\n"
            "filters).\n"
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_CUST_FILTER_ATTRS",
            "The QHP_CUST_FILTER_ATTRS tag specifies the list of the attributes of the\n"
            "custom filter to add. For more information please see Qt Help Project / Custom\n"
            "Filters (see: http://qt-project.org/doc/qt-4.8/qthelpproject.html#custom-\n"
            "filters).\n"
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHP_SECT_FILTER_ATTRS",
            "The QHP_SECT_FILTER_ATTRS tag specifies the list of the attributes this\n"
            "project's filter section matches. Qt Help Project / Filter Attributes (see:\n"
            "http://qt-project.org/doc/qt-4.8/qthelpproject.html#filter-attributes).\n"
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("QHG_LOCATION",
            "The QHG_LOCATION tag can be used to specify the location of Qt's\n"
            "qhelpgenerator. If non-empty doxygen will try to run qhelpgenerator on the\n"
            "generated .qhp file.\n"
            "This tag requires that the tag GENERATE_QHP is set to YES.");

   m_fullHelp.insert("GENERATE_ECLIPSEHELP",
            "If the GENERATE_ECLIPSEHELP tag is set to YES, additional index files will be\n"
            "generated, together with the HTML files, they form an Eclipse help plugin. To\n"
            "install this plugin and make it available under the help contents menu in\n"
            "Eclipse, the contents of the directory containing the HTML and XML files needs\n"
            "to be copied into the plugins directory of eclipse. The name of the directory\n"
            "within the plugins directory should be the same as the ECLIPSE_DOC_ID value.\n"
            "After copying Eclipse needs to be restarted before the help appears.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("ECLIPSE_DOC_ID",
            "A unique identifier for the Eclipse help plugin. When installing the plugin\n"
            "the directory name containing the HTML and XML files should also have this\n"
            "name. Each documentation set should have its own identifier.\n"
            "The default value is: org.doxygen.Project.\n"
            "This tag requires that the tag GENERATE_ECLIPSEHELP is set to YES.");

   m_fullHelp.insert("DISABLE_INDEX",
            "If you want full control over the layout of the generated HTML pages it might\n"
            "be necessary to disable the index and replace it with your own. The\n"
            "DISABLE_INDEX tag can be used to turn on/off the condensed index (tabs) at top\n"
            "of each HTML page. A value of NO enables the index and the value YES disables\n"
            "it. Since the tabs in the index contain the same information as the navigation\n"
            "tree, you can set this option to YES if you also set GENERATE_TREEVIEW to YES.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("GENERATE_TREEVIEW",
            "The GENERATE_TREEVIEW tag is used to specify whether a tree-like index\n"
            "structure should be generated to display hierarchical information. If the tag\n"
            "value is set to YES, a side panel will be generated containing a tree-like\n"
            "index structure (just like the one that is generated for HTML Help). For this\n"
            "to work a browser that supports JavaScript, DHTML, CSS and frames is required\n"
            "(i.e. any modern browser). Windows users are probably better off using the\n"
            "HTML help feature. Via custom stylesheets (see HTML_EXTRA_STYLESHEET) one can\n"
            "further fine-tune the look of the index. As an example, the default style\n"
            "sheet generated by doxygen has an example that shows how to put an image at\n"
            "the root of the tree instead of the PROJECT_NAME. Since the tree basically has\n"
            "the same information as the tab index, you could consider setting\n"
            "DISABLE_INDEX to YES when enabling this option.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("ENUM_VALUES_PER_LINE",
            "The ENUM_VALUES_PER_LINE tag can be used to set the number of enum values that\n"
            "doxygen will group on one line in the generated HTML documentation.\n"
            "\n"
            "Note that a value of 0 will completely suppress the enum values from appearing\n"
            "in the overview section.\n"
            "Minimum value: 0, maximum value: 20, default value: 4.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("TREEVIEW_WIDTH",
            "If the treeview is enabled (see GENERATE_TREEVIEW) then this tag can be used\n"
            "to set the initial width (in pixels) of the frame in which the tree is shown.\n"
            "Minimum value: 0, maximum value: 1500, default value: 250.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("EXT_LINKS_IN_WINDOW",
            "If the EXT_LINKS_IN_WINDOW option is set to YES, doxygen will open links to\n"
            "external symbols imported via tag files in a separate window.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("FORMULA_FONTSIZE",
            "Use this tag to change the font size of LaTeX formulas included as images in\n"
            "the HTML documentation. When you change the font size after a successful\n"
            "doxygen run you need to manually remove any form_*.png images from the HTML\n"
            "output directory to force them to be regenerated.\n"
            "Minimum value: 8, maximum value: 50, default value: 10.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("FORMULA_TRANSPARENT",
            "Use the FORMULA_TRANPARENT tag to determine whether or not the images\n"
            "generated for formulas are transparent PNGs. Transparent PNGs are not\n"
            "supported properly for IE 6.0, but are supported on all modern browsers.\n"
            "\n"
            "Note that when changing this option you need to delete any form_*.png files in\n"
            "the HTML output directory before the changes have effect.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("USE_MATHJAX",
            "Enable the USE_MATHJAX option to render LaTeX formulas using MathJax (see\n"
            "http://www.mathjax.org) which uses client side Javascript for the rendering\n"
            "instead of using pre-rendered bitmaps. Use this if you do not have LaTeX\n"
            "installed or if you want to formulas look prettier in the HTML output. When\n"
            "enabled you may also need to install MathJax separately and configure the path\n"
            "to it using the MATHJAX_RELPATH option.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("MATHJAX_FORMAT",
            "When MathJax is enabled you can set the default output format to be used for\n"
            "the MathJax output. See the MathJax site (see:\n"
            "http://docs.mathjax.org/en/latest/output.html) for more details.\n"
            "Possible values are: HTML-CSS (which is slower, but has the best\n"
            "compatibility), NativeMML (i.e. MathML) and SVG.\n"
            "The default value is: HTML-CSS.\n"
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("MATHJAX_RELPATH",
            "When MathJax is enabled you need to specify the location relative to the HTML\n"
            "output directory using the MATHJAX_RELPATH option. The destination directory\n"
            "should contain the MathJax.js script. For instance, if the mathjax directory\n"
            "is located at the same level as the HTML output directory, then\n"
            "MATHJAX_RELPATH should be ../mathjax. The default value points to the MathJax\n"
            "Content Delivery Network so you can quickly see the result without installing\n"
            "MathJax. However, it is strongly recommended to install a local copy of\n"
            "MathJax from http://www.mathjax.org before deployment.\n"
            "The default value is: http://cdn.mathjax.org/mathjax/latest.\n"
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("MATHJAX_EXTENSIONS",
            "The MATHJAX_EXTENSIONS tag can be used to specify one or more MathJax\n"
            "extension names that should be enabled during MathJax rendering. For example\n"
            "MATHJAX_EXTENSIONS = TeX/AMSmath TeX/AMSsymbols\n"
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("MATHJAX_CODEFILE",
            "The MATHJAX_CODEFILE tag can be used to specify a file with javascript pieces\n"
            "of code that will be used on startup of the MathJax code. See the MathJax site\n"
            "(see: http://docs.mathjax.org/en/latest/output.html) for more details. For an\n"
            "example see the documentation.\n"
            "This tag requires that the tag USE_MATHJAX is set to YES.");

   m_fullHelp.insert("SEARCHENGINE",
            "When the SEARCHENGINE tag is enabled doxygen will generate a search box for\n"
            "the HTML output. The underlying search engine uses javascript and DHTML and\n"
            "should work on any modern browser. Note that when using HTML help\n"
            "(GENERATE_HTMLHELP), Qt help (GENERATE_QHP), or docsets (GENERATE_DOCSET)\n"
            "there is already a search function so this one should typically be disabled.\n"
            "For large projects the javascript based search engine can be slow, then\n"
            "enabling SERVER_BASED_SEARCH may provide a better solution. It is possible to\n"
            "search using the keyboard; to jump to the search box use <access key> + S\n"
            "(what the <access key> is depends on the OS and browser, but it is typically\n"
            "<CTRL>, <ALT>/<option>, or both). Inside the search box use the <cursor down\n"
            "key> to jump into the search results window, the results can be navigated\n"
            "using the <cursor keys>. Press <Enter> to select an item or <escape> to cancel\n"
            "the search. The filter options can be selected when the cursor is inside the\n"
            "search box by pressing <Shift>+<cursor down>. Also here use the <cursor keys>\n"
            "to select a filter and <Enter> or <escape> to activate or cancel the filter\n"
            "option.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag GENERATE_HTML is set to YES.");

   m_fullHelp.insert("SERVER_BASED_SEARCH",
            "When the SERVER_BASED_SEARCH tag is enabled the search engine will be\n"
            "implemented using a web server instead of a web client using Javascript. There\n"
            "are two flavors of web server based searching depending on the EXTERNAL_SEARCH\n"
            "setting. When disabled, doxygen will generate a PHP script for searching and\n"
            "an index file used by the script. When EXTERNAL_SEARCH is enabled the indexing\n"
            "and searching needs to be provided by external tools. See the section\n"
            "\"External Indexing and Searching\" for details.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("EXTERNAL_SEARCH",
            "When EXTERNAL_SEARCH tag is enabled doxygen will no longer generate the PHP\n"
            "script for searching. Instead the search results are written to an XML file\n"
            "which needs to be processed by an external indexer. Doxygen will invoke an\n"
            "external search engine pointed to by the SEARCHENGINE_URL option to obtain the\n"
            "search results.\n"
            "\n"
            "Doxygen ships with an example indexer (doxyindexer) and search engine\n"
            "(doxysearch.cgi) which are based on the open source search engine library\n"
            "Xapian (see: http://xapian.org/).\n"
            "\n"
            "See the section \"External Indexing and Searching\" for details.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("SEARCHENGINE_URL",
            "The SEARCHENGINE_URL should point to a search engine hosted by a web server\n"
            "which will return the search results when EXTERNAL_SEARCH is enabled.\n"
            "\n"
            "Doxygen ships with an example indexer (doxyindexer) and search engine\n"
            "(doxysearch.cgi) which are based on the open source search engine library\n"
            "Xapian (see: http://xapian.org/). See the section \"External Indexing and\n"
            "Searching\" for details.\n"
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("SEARCHDATA_FILE",
            "When SERVER_BASED_SEARCH and EXTERNAL_SEARCH are both enabled the unindexed\n"
            "search data is written to a file for indexing by an external tool. With the\n"
            "SEARCHDATA_FILE tag the name of this file can be specified.\n"
            "The default file is: searchdata.xml.\n"
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("EXTERNAL_SEARCH_ID",
            "When SERVER_BASED_SEARCH and EXTERNAL_SEARCH are both enabled the\n"
            "EXTERNAL_SEARCH_ID tag can be used as an identifier for the project. This is\n"
            "useful in combination with EXTRA_SEARCH_MAPPINGS to search through multiple\n"
            "projects and redirect the results back to the right project.\n"
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("EXTRA_SEARCH_MAPPINGS",
            "The EXTRA_SEARCH_MAPPINGS tag can be used to enable searching through doxygen\n"
            "projects other than the one defined by this configuration file, but that are\n"
            "all added to the same external search index. Each project needs to have a\n"
            "unique id set via EXTERNAL_SEARCH_ID. The search mapping then maps the id of\n"
            "to a relative location where the documentation can be found. The format is:\n"
            "EXTRA_SEARCH_MAPPINGS = tagname1=loc1 tagname2=loc2 ...\n"
            "This tag requires that the tag SEARCHENGINE is set to YES.");

   m_fullHelp.insert("GENERATE_LATEX",
            "If the GENERATE_LATEX tag is set to YES, doxygen will generate LaTeX output.\n"
            "The default value is: YES.");

   m_fullHelp.insert("LATEX_OUTPUT",
            "The LATEX_OUTPUT tag is used to specify where the LaTeX docs will be put. If a\n"
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of\n"
            "it.\n"
            "The default directory is: latex.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_CMD_NAME",
            "The LATEX_CMD_NAME tag can be used to specify the LaTeX command name to be\n"
            "invoked.\n"
            "\n"
            "Note that when enabling USE_PDFLATEX this option is only used for generating\n"
            "bitmaps for formulas in the HTML output, but not in the Makefile that is\n"
            "written to the output directory.\n"
            "The default file is: latex.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("MAKEINDEX_CMD_NAME",
            "The MAKEINDEX_CMD_NAME tag can be used to specify the command name to generate\n"
            "index for LaTeX.\n"
            "The default file is: makeindex.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("COMPACT_LATEX",
            "If the COMPACT_LATEX tag is set to YES, doxygen generates more compact LaTeX\n"
            "documents. This may be useful for small projects and may help to save some\n"
            "trees in general.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("PAPER_TYPE",
            "The PAPER_TYPE tag can be used to set the paper type that is used by the\n"
            "printer.\n"
            "Possible values are: a4 (210 x 297 mm), letter (8.5 x 11 inches), legal (8.5 x\n"
            "14 inches) and executive (7.25 x 10.5 inches).\n"
            "The default value is: a4.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("EXTRA_PACKAGES",
            "The EXTRA_PACKAGES tag can be used to specify one or more LaTeX package names\n"
            "that should be included in the LaTeX output. To get the times font for\n"
            "instance you can specify\n"
            "EXTRA_PACKAGES=times\n"
            "If left blank no extra packages will be included.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_HEADER",
            "The LATEX_HEADER tag can be used to specify a personal LaTeX header for the\n"
            "generated LaTeX document. The header should contain everything until the first\n"
            "chapter. If it is left blank doxygen will generate a standard header. See\n"
            "section \"Doxygen usage\" for information on how to let doxygen write the\n"
            "default header to a separate file.\n"
            "\n"
            "Note: Only use a user-defined header if you know what you are doing! The\n"
            "following commands have a special meaning inside the header: $title,\n"
            "$datetime, $date, $doxygenversion, $projectname, $projectnumber,\n"
            "$projectbrief, $projectlogo. Doxygen will replace $title with the empty\n"
            "string, for the replacement values of the other commands the user is referred\n"
            "to HTML_HEADER.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_FOOTER",
            "The LATEX_FOOTER tag can be used to specify a personal LaTeX footer for the\n"
            "generated LaTeX document. The footer should contain everything after the last\n"
            "chapter. If it is left blank doxygen will generate a standard footer. See\n"
            "LATEX_HEADER for more information on how to generate a default footer and what\n"
            "special commands can be used inside the footer.\n"
            "\n"
            "Note: Only use a user-defined footer if you know what you are doing!\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_EXTRA_FILES",
            "The LATEX_EXTRA_FILES tag can be used to specify one or more extra images or\n"
            "other source files which should be copied to the LATEX_OUTPUT output\n"
            "directory. Note that the files will be copied as-is; there are no commands or\n"
            "markers available.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("PDF_HYPERLINKS",
            "If the PDF_HYPERLINKS tag is set to YES, the LaTeX that is generated is\n"
            "prepared for conversion to PDF (using ps2pdf or pdflatex). The PDF file will\n"
            "contain links (just like the HTML output) instead of page references. This\n"
            "makes the output suitable for online browsing using a PDF viewer.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("USE_PDFLATEX",
            "If the USE_PDFLATEX tag is set to YES, doxygen will use pdflatex to generate\n"
            "the PDF file directly from the LaTeX files. Set this option to YES, to get a\n"
            "higher quality PDF documentation.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_BATCHMODE",
            "If the LATEX_BATCHMODE tag is set to YES, doxygen will add the \\batchmode\n"
            "command to the generated LaTeX files. This will instruct LaTeX to keep running\n"
            "if errors occur, instead of asking the user for help. This option is also used\n"
            "when generating formulas in HTML.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_HIDE_INDICES",
            "If the LATEX_HIDE_INDICES tag is set to YES then doxygen will not include the\n"
            "index chapters (such as File Index, Compound Index, etc.) in the output.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_SOURCE_CODE",
            "If the LATEX_SOURCE_CODE tag is set to YES then doxygen will include source\n"
            "code with syntax highlighting in the LaTeX output.\n"
            "\n"
            "Note that which sources are shown also depends on other settings such as\n"
            "SOURCE_BROWSER.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("LATEX_BIB_STYLE",
            "The LATEX_BIB_STYLE tag can be used to specify the style to use for the\n"
            "bibliography, e.g. plainnat, or ieeetr. See\n"
            "http://en.wikipedia.org/wiki/BibTeX and \\cite for more info.\n"
            "The default value is: plain.\n"
            "This tag requires that the tag GENERATE_LATEX is set to YES.");

   m_fullHelp.insert("GENERATE_RTF",
            "If the GENERATE_RTF tag is set to YES, doxygen will generate RTF output. The\n"
            "RTF output is optimized for Word 97 and may not look too pretty with other RTF\n"
            "readers/editors.\n"
            "The default value is: NO.");

   m_fullHelp.insert("RTF_OUTPUT",
            "The RTF_OUTPUT tag is used to specify where the RTF docs will be put. If a\n"
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of\n"
            "it.\n"
            "The default directory is: rtf.\n"
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("COMPACT_RTF",
            "If the COMPACT_RTF tag is set to YES, doxygen generates more compact RTF\n"
            "documents. This may be useful for small projects and may help to save some\n"
            "trees in general.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("RTF_HYPERLINKS",
            "If the RTF_HYPERLINKS tag is set to YES, the RTF that is generated will\n"
            "contain hyperlink fields. The RTF file will contain links (just like the HTML\n"
            "output) instead of page references. This makes the output suitable for online\n"
            "browsing using Word or some other Word compatible readers that support those\n"
            "fields.\n"
            "\n"
            "Note: WordPad (write) and others do not support links.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("RTF_STYLESHEET_FILE",
            "Load stylesheet definitions from file. Syntax is similar to doxygen's config\n"
            "file, i.e. a series of assignments. You only have to provide replacements,\n"
            "missing definitions are set to their default value.\n"
            "\n"
            "See also section \"Doxygen usage\" for information on how to generate the\n"
            "default style sheet that doxygen normally uses.\n"
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("RTF_EXTENSIONS_FILE",
            "Set optional variables used in the generation of an RTF document. Syntax is\n"
            "similar to doxygen's config file. A template extensions file can be generated\n"
            "using doxygen -e rtf extensionFile.\n"
            "This tag requires that the tag GENERATE_RTF is set to YES.");

   m_fullHelp.insert("GENERATE_MAN",
            "If the GENERATE_MAN tag is set to YES, doxygen will generate man pages for\n"
            "classes and files.\n"
            "The default value is: NO.");

   m_fullHelp.insert("MAN_OUTPUT",
            "The MAN_OUTPUT tag is used to specify where the man pages will be put. If a\n"
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of\n"
            "it. A directory man3 will be created inside the directory specified by\n"
            "MAN_OUTPUT.\n"
            "The default directory is: man.\n"
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("MAN_EXTENSION",
            "The MAN_EXTENSION tag determines the extension that is added to the generated\n"
            "man pages. In case the manual section does not start with a number, the number\n"
            "3 is prepended. The dot (.) at the beginning of the MAN_EXTENSION tag is\n"
            "optional.\n"
            "The default value is: .3.\n"
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("MAN_SUBDIR",
            "The MAN_SUBDIR tag determines the name of the directory created within\n"
            "MAN_OUTPUT in which the man pages are placed. If defaults to man followed by\n"
            "MAN_EXTENSION with the initial . removed.\n"
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("MAN_LINKS",
            "If the MAN_LINKS tag is set to YES and doxygen generates man output, then it\n"
            "will generate one additional man file for each entity documented in the real\n"
            "man page(s). These additional files only source the real man page, but without\n"
            "them the man command would be unable to find the correct page.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_MAN is set to YES.");

   m_fullHelp.insert("GENERATE_XML",
            "If the GENERATE_XML tag is set to YES, doxygen will generate an XML file that\n"
            "captures the structure of the code including all documentation.\n"
            "The default value is: NO.");

   m_fullHelp.insert("XML_OUTPUT",
            "The XML_OUTPUT tag is used to specify where the XML pages will be put. If a\n"
            "relative path is entered the value of OUTPUT_DIRECTORY will be put in front of\n"
            "it.\n"
            "The default directory is: xml.\n"
            "This tag requires that the tag GENERATE_XML is set to YES.");

   m_fullHelp.insert("XML_PROGRAMLISTING",
            "If the XML_PROGRAMLISTING tag is set to YES, doxygen will dump the program\n"
            "listings (including syntax highlighting and cross-referencing information) to\n"
            "the XML output. Note that enabling this will significantly increase the size\n"
            "of the XML output.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag GENERATE_XML is set to YES.");

   m_fullHelp.insert("GENERATE_DOCBOOK",
            "If the GENERATE_DOCBOOK tag is set to YES, doxygen will generate Docbook files\n"
            "that can be used to generate PDF.\n"
            "The default value is: NO.");

   m_fullHelp.insert("DOCBOOK_OUTPUT",
            "The DOCBOOK_OUTPUT tag is used to specify where the Docbook pages will be put.\n"
            "If a relative path is entered the value of OUTPUT_DIRECTORY will be put in\n"
            "front of it.\n"
            "The default directory is: docbook.\n"
            "This tag requires that the tag GENERATE_DOCBOOK is set to YES.");

   m_fullHelp.insert("DOCBOOK_PROGRAMLISTING",
            "If the DOCBOOK_PROGRAMLISTING tag is set to YES, doxygen will include the\n"
            "program listings (including syntax highlighting and cross-referencing\n"
            "information) to the DOCBOOK output. Note that enabling this will significantly\n"
            "increase the size of the DOCBOOK output.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_DOCBOOK is set to YES.");

   m_fullHelp.insert("GENERATE_AUTOGEN_DEF",
            "If the GENERATE_AUTOGEN_DEF tag is set to YES, doxygen will generate an\n"
            "AutoGen Definitions (see http://autogen.sf.net) file that captures the\n"
            "structure of the code including all documentation. Note that this feature is\n"
            "still experimental and incomplete at the moment.\n"
            "The default value is: NO.");

   m_fullHelp.insert("GENERATE_PERLMOD",
            "If the GENERATE_PERLMOD tag is set to YES, doxygen will generate a Perl module\n"
            "file that captures the structure of the code including all documentation.\n"
            "\n"
            "Note that this feature is still experimental and incomplete at the moment.\n"
            "The default value is: NO.");

   m_fullHelp.insert("PERLMOD_LATEX",
            "If the PERLMOD_LATEX tag is set to YES, doxygen will generate the necessary\n"
            "Makefile rules, Perl scripts and LaTeX code to be able to generate PDF and DVI\n"
            "output from the Perl module output.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag GENERATE_PERLMOD is set to YES.");

   m_fullHelp.insert("PERLMOD_PRETTY",
            "If the PERLMOD_PRETTY tag is set to YES, the Perl module output will be nicely\n"
            "formatted so it can be parsed by a human reader. This is useful if you want to\n"
            "understand what is going on. On the other hand, if this tag is set to NO, the\n"
            "size of the Perl module output will be much smaller and Perl will parse it\n"
            "just the same.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag GENERATE_PERLMOD is set to YES.");

   m_fullHelp.insert("PERLMOD_MAKEVAR_PREFIX",
            "The names of the make variables in the generated doxyrules.make file are\n"
            "prefixed with the string contained in PERLMOD_MAKEVAR_PREFIX. This is useful\n"
            "so different doxyrules.make files included by the same Makefile don't\n"
            "overwrite each other's variables.\n"
            "This tag requires that the tag GENERATE_PERLMOD is set to YES.");

   m_fullHelp.insert("ENABLE_PREPROCESSING",
            "If the ENABLE_PREPROCESSING tag is set to YES, doxygen will evaluate all\n"
            "C-preprocessor directives found in the sources and include files.\n"
            "The default value is: YES.");

   m_fullHelp.insert("MACRO_EXPANSION",
            "If the MACRO_EXPANSION tag is set to YES, doxygen will expand all macro names\n"
            "in the source code. If set to NO, only conditional compilation will be\n"
            "performed. Macro expansion can be done in a controlled way by setting\n"
            "EXPAND_ONLY_PREDEF to YES.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("EXPAND_ONLY_PREDEF",
            "If the EXPAND_ONLY_PREDEF and MACRO_EXPANSION tags are both set to YES then\n"
            "the macro expansion is limited to the macros specified with the PREDEFINED and\n"
            "EXPAND_AS_DEFINED tags.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("SEARCH_INCLUDES",
            "If the SEARCH_INCLUDES tag is set to YES, the include files in the\n"
            "INCLUDE_PATH will be searched if a #include is found.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("INCLUDE_PATH",
            "The INCLUDE_PATH tag can be used to specify one or more directories that\n"
            "contain include files that are not input files but should be processed by the\n"
            "preprocessor.\n"
            "This tag requires that the tag SEARCH_INCLUDES is set to YES.");

   m_fullHelp.insert("INCLUDE_FILE_PATTERNS",
            "You can use the INCLUDE_FILE_PATTERNS tag to specify one or more wildcard\n"
            "patterns (like *.h and *.hpp) to filter out the header-files in the\n"
            "directories. If left blank, the patterns specified with FILE_PATTERNS will be\n"
            "used.\n"
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("PREDEFINED",
            "The PREDEFINED tag can be used to specify one or more macro names that are\n"
            "defined before the preprocessor is started (similar to the -D option of e.g.\n"
            "gcc). The argument of the tag is a list of macros of the form: name or\n"
            "name=definition (no spaces). If the definition and the \"=\" are omitted, \"=1\"\n"
            "is assumed. To prevent a macro definition from being undefined via #undef or\n"
            "recursively expanded use the := operator instead of the = operator.\n"
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("EXPAND_AS_DEFINED",
            "If the MACRO_EXPANSION and EXPAND_ONLY_PREDEF tags are set to YES then this\n"
            "tag can be used to specify a list of macro names that should be expanded. The\n"
            "macro definition that is found in the sources will be used. Use the PREDEFINED\n"
            "tag if you want to use a different macro definition that overrules the\n"
            "definition found in the source code.\n"
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("SKIP_FUNCTION_MACROS",
            "If the SKIP_FUNCTION_MACROS tag is set to YES then doxygen's preprocessor will\n"
            "remove all references to function-like macros that are alone on a line, have\n"
            "an all uppercase name, and do not end with a semicolon. Such function macros\n"
            "are typically used for boiler-plate code, and will confuse the parser if not\n"
            "removed.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag ENABLE_PREPROCESSING is set to YES.");

   m_fullHelp.insert("TAGFILES",
            "The TAGFILES tag can be used to specify one or more tag files. For each tag\n"
            "file the location of the external documentation should be added. The format of\n"
            "a tag file without this location is as follows:\n"
            "TAGFILES = file1 file2 ...\n"
            "Adding location for the tag files is done as follows:\n"
            "TAGFILES = file1=loc1 \"file2 = loc2\" ...\n"
            "where loc1 and loc2 can be relative or absolute paths or URLs. See the\n"
            "section \"Linking to external documentation\" for more information about the use\n"
            "of tag files.\n"
            "Note: Each tag file must have a unique name (where the name does NOT include\n"
            "the path). If a tag file is not located in the directory in which doxygen is\n"
            "run, you must also specify the path to the tagfile here.");

   m_fullHelp.insert("GENERATE_TAGFILE",
            "When a file name is specified after GENERATE_TAGFILE, doxygen will create a\n"
            "tag file that is based on the input files it reads. See section \"Linking to\n"
            "external documentation\" for more information about the usage of tag files.");

   m_fullHelp.insert("ALLEXTERNALS",
            "If the ALLEXTERNALS tag is set to YES, all external class will be listed in\n"
            "the class index. If set to NO, only the inherited external classes will be\n"
            "listed.\n"
            "The default value is: NO.");

   m_fullHelp.insert("EXTERNAL_GROUPS",
            "If the EXTERNAL_GROUPS tag is set to YES, all external groups will be listed\n"
            "in the modules index. If set to NO, only the current project's groups will be\n"
            "listed.\n"
            "The default value is: YES.");

   m_fullHelp.insert("EXTERNAL_PAGES",
            "If the EXTERNAL_PAGES tag is set to YES, all external pages will be listed in\n"
            "the related pages index. If set to NO, only the current project's pages will\n"
            "be listed.\n"
            "The default value is: YES.");

   m_fullHelp.insert("PERL_PATH",
            "The PERL_PATH should be the absolute path and name of the perl script\n"
            "interpreter (i.e. the result of 'which perl').\n"
            "The default file (with absolute path) is: /usr/bin/perl.");

   m_fullHelp.insert("CLASS_DIAGRAMS",
            "If the CLASS_DIAGRAMS tag is set to YES, doxygen will generate a class diagram\n"
            "(in HTML and LaTeX) for classes with base or super classes. Setting the tag to\n"
            "NO turns the diagrams off. Note that this option also works with HAVE_DOT\n"
            "disabled, but it is recommended to install and use dot, since it yields more\n"
            "powerful graphs.\n"
            "The default value is: YES.");

   m_fullHelp.insert("MSCGEN_PATH",
            "You can define message sequence charts within doxygen comments using the \\msc\n"
            "command. Doxygen will then run the mscgen tool (see:\n"
            "http://www.mcternan.me.uk/mscgen/)) to produce the chart and insert it in the\n"
            "documentation. The MSCGEN_PATH tag allows you to specify the directory where\n"
            "the mscgen tool resides. If left empty the tool is assumed to be found in the\n"
            "default search path.");

   m_fullHelp.insert("DIA_PATH",
            "You can include diagrams made with dia in doxygen documentation. Doxygen will\n"
            "then run dia to produce the diagram and insert it in the documentation. The\n"
            "DIA_PATH tag allows you to specify the directory where the dia binary resides.\n"
            "If left empty dia is assumed to be found in the default search path.");

   m_fullHelp.insert("HIDE_UNDOC_RELATIONS",
            "If set to YES the inheritance and collaboration graphs will hide inheritance\n"
            "and usage relations if the target is undocumented or is not a class.\n"
            "The default value is: YES.");

   m_fullHelp.insert("HAVE_DOT",
            "If you set the HAVE_DOT tag to YES then doxygen will assume the dot tool is\n"
            "available from the path. This tool is part of Graphviz (see:\n"
            "http://www.graphviz.org/), a graph visualization toolkit from AT&T and Lucent\n"
            "Bell Labs. The other options in this section have no effect if this option is\n"
            "set to NO\n"
            "The default value is: NO.");

   m_fullHelp.insert("DOT_NUM_THREADS",
            "The DOT_NUM_THREADS specifies the number of dot invocations doxygen is allowed\n"
            "to run in parallel. When set to 0 doxygen will base this on the number of\n"
            "processors available in the system. You can set it explicitly to a value\n"
            "larger than 0 to get control over the balance between CPU load and processing\n"
            "speed.\n"
            "Minimum value: 0, maximum value: 32, default value: 0.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_FONTNAME",
            "When you want a differently looking font in the dot files that doxygen\n"
            "generates you can specify the font name using DOT_FONTNAME. You need to make\n"
            "sure dot is able to find the font, which can be done by putting it in a\n"
            "standard location or by setting the DOTFONTPATH environment variable or by\n"
            "setting DOT_FONTPATH to the directory containing the font.\n"
            "The default value is: Helvetica.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_FONTSIZE",
            "The DOT_FONTSIZE tag can be used to set the size (in points) of the font of\n"
            "dot graphs.\n"
            "Minimum value: 4, maximum value: 24, default value: 10.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_FONTPATH",
            "By default doxygen will tell dot to use the default font as specified with\n"
            "DOT_FONTNAME. If you specify a different font using DOT_FONTNAME you can set\n"
            "the path where dot can find it using this tag.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("CLASS_GRAPH",
            "If the CLASS_GRAPH tag is set to YES then doxygen will generate a graph for\n"
            "each documented class showing the direct and indirect inheritance relations.\n"
            "Setting this tag to YES will force the CLASS_DIAGRAMS tag to NO.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("COLLABORATION_GRAPH",
            "If the COLLABORATION_GRAPH tag is set to YES then doxygen will generate a\n"
            "graph for each documented class showing the direct and indirect implementation\n"
            "dependencies (inheritance, containment, and class references variables) of the\n"
            "class with other documented classes.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("GROUP_GRAPHS",
            "If the GROUP_GRAPHS tag is set to YES then doxygen will generate a graph for\n"
            "groups, showing the direct groups dependencies.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("UML_LOOK",
            "If the UML_LOOK tag is set to YES, doxygen will generate inheritance and\n"
            "collaboration diagrams in a style similar to the OMG's Unified Modeling\n"
            "Language.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("UML_LIMIT_NUM_FIELDS",
            "If the UML_LOOK tag is enabled, the fields and methods are shown inside the\n"
            "class node. If there are many fields or methods and many nodes the graph may\n"
            "become too big to be useful. The UML_LIMIT_NUM_FIELDS threshold limits the\n"
            "number of items for each type to make the size more manageable. Set this to 0\n"
            "for no limit. Note that the threshold may be exceeded by 50% before the limit\n"
            "is enforced. So when you set the threshold to 10, up to 15 fields may appear,\n"
            "but if the number exceeds 15, the total amount of fields shown is limited to\n"
            "10.\n"
            "Minimum value: 0, maximum value: 100, default value: 10.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("TEMPLATE_RELATIONS",
            "If the TEMPLATE_RELATIONS tag is set to YES then the inheritance and\n"
            "collaboration graphs will show the relations between templates and their\n"
            "instances.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("INCLUDE_GRAPH",
            "If the INCLUDE_GRAPH, ENABLE_PREPROCESSING and SEARCH_INCLUDES tags are set to\n"
            "YES then doxygen will generate a graph for each documented file showing the\n"
            "direct and indirect include dependencies of the file with other documented\n"
            "files.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("INCLUDED_BY_GRAPH",
            "If the INCLUDED_BY_GRAPH, ENABLE_PREPROCESSING and SEARCH_INCLUDES tags are\n"
            "set to YES then doxygen will generate a graph for each documented file showing\n"
            "the direct and indirect include dependencies of the file with other documented\n"
            "files.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("CALL_GRAPH",
            "If the CALL_GRAPH tag is set to YES then doxygen will generate a call\n"
            "dependency graph for every global function or class method.\n"
            "\n"
            "Note that enabling this option will significantly increase the time of a run.\n"
            "So in most cases it will be better to enable call graphs for selected\n"
            "functions only using the \\callgraph command.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("CALLER_GRAPH",
            "If the CALLER_GRAPH tag is set to YES then doxygen will generate a caller\n"
            "dependency graph for every global function or class method.\n"
            "\n"
            "Note that enabling this option will significantly increase the time of a run.\n"
            "So in most cases it will be better to enable caller graphs for selected\n"
            "functions only using the \\callergraph command.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("GRAPHICAL_HIERARCHY",
            "If the GRAPHICAL_HIERARCHY tag is set to YES then doxygen will graphical\n"
            "hierarchy of all classes instead of a textual one.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DIRECTORY_GRAPH",
            "If the DIRECTORY_GRAPH tag is set to YES then doxygen will show the\n"
            "dependencies a directory has on other directories in a graphical way. The\n"
            "dependency relations are determined by the #include relations between the\n"
            "files in the directories.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_IMAGE_FORMAT",
            "The DOT_IMAGE_FORMAT tag can be used to set the image format of the images\n"
            "generated by dot.\n"
            "Note: If you choose svg you need to set HTML_FILE_EXTENSION to xhtml in order\n"
            "to make the SVG files visible in IE 9+ (other browsers do not have this\n"
            "requirement).\n"
            "Possible values are: png, jpg, gif and svg.\n"
            "The default value is: png.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("INTERACTIVE_SVG",
            "If DOT_IMAGE_FORMAT is set to svg, then this option can be set to YES to\n"
            "enable generation of interactive SVG images that allow zooming and panning.\n"
            "\n"
            "Note that this requires a modern browser other than Internet Explorer. Tested\n"
            "and working are Firefox, Chrome, Safari, and Opera.\n"
            "Note: For IE 9+ you need to set HTML_FILE_EXTENSION to xhtml in order to make\n"
            "the SVG files visible. Older versions of IE do not have SVG support.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_PATH",
            "The DOT_PATH tag can be used to specify the path where the dot tool can be\n"
            "found. If left blank, it is assumed the dot tool can be found in the path.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOTFILE_DIRS",
            "The DOTFILE_DIRS tag can be used to specify one or more directories that\n"
            "contain dot files that are included in the documentation (see the \\dotfile\n"
            "command).\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("MSCFILE_DIRS",
            "The MSCFILE_DIRS tag can be used to specify one or more directories that\n"
            "contain msc files that are included in the documentation (see the \\mscfile\n"
            "command).");

   m_fullHelp.insert("DIAFILE_DIRS",
            "The DIAFILE_DIRS tag can be used to specify one or more directories that\n"
            "contain dia files that are included in the documentation (see the \\diafile\n"
            "command).");

   m_fullHelp.insert("PLANTUML_JAR_PATH",
            "When using plantuml, the PLANTUML_JAR_PATH tag should be used to specify the\n"
            "path where java can find the plantuml.jar file. If left blank, it is assumed\n"
            "PlantUML is not used or called during a preprocessing step. Doxygen will\n"
            "generate a warning when it encounters a \\startuml command in this case and\n"
            "will not generate output for the diagram.");

   m_fullHelp.insert("PLANTUML_INCLUDE_PATH",
            "When using plantuml, the specified paths are searched for files specified by\n"
            "the !include statement in a plantuml block.");

   m_fullHelp.insert("DOT_GRAPH_MAX_NODES",
            "The DOT_GRAPH_MAX_NODES tag can be used to set the maximum number of nodes\n"
            "that will be shown in the graph. If the number of nodes in a graph becomes\n"
            "larger than this value, doxygen will truncate the graph, which is visualized\n"
            "by representing a node as a red box. Note that doxygen if the number of direct\n"
            "children of the root node in a graph is already larger than\n"
            "DOT_GRAPH_MAX_NODES then the graph will not be shown at all. Also note that\n"
            "the size of a graph can be further restricted by MAX_DOT_GRAPH_DEPTH.\n"
            "Minimum value: 0, maximum value: 10000, default value: 50.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("MAX_DOT_GRAPH_DEPTH",
            "The MAX_DOT_GRAPH_DEPTH tag can be used to set the maximum depth of the graphs\n"
            "generated by dot. A depth value of 3 means that only nodes reachable from the\n"
            "root by following a path via at most 3 edges will be shown. Nodes that lay\n"
            "further from the root node will be omitted. Note that setting this option to 1\n"
            "or 2 may greatly reduce the computation time needed for large code bases. Also\n"
            "note that the size of a graph can be further restricted by\n"
            "DOT_GRAPH_MAX_NODES. Using a depth of 0 means no depth restriction.\n"
            "Minimum value: 0, maximum value: 1000, default value: 0.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_TRANSPARENT",
            "Set the DOT_TRANSPARENT tag to YES to generate images with a transparent\n"
            "background. This is disabled by default, because dot on Windows does not seem\n"
            "to support this out of the box.\n"
            "\n"
            "Warning: Depending on the platform used, enabling this option may lead to\n"
            "badly anti-aliased labels on the edges of a graph (i.e. they become hard to\n"
            "read).\n"
            "The default value is: NO.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_MULTI_TARGETS",
            "Set the DOT_MULTI_TARGETS tag to YES to allow dot to generate multiple output\n"
            "files in one run (i.e. multiple -o and -T options on the command line). This\n"
            "makes dot run faster, but since only newer versions of dot (>1.8.10) support\n"
            "this, this feature is disabled by default.\n"
            "The default value is: NO.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("GENERATE_LEGEND",
            "If the GENERATE_LEGEND tag is set to YES doxygen will generate a legend page\n"
            "explaining the meaning of the various boxes and arrows in the dot generated\n"
            "graphs.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");

   m_fullHelp.insert("DOT_CLEANUP",
            "If the DOT_CLEANUP tag is set to YES, doxygen will remove the intermediate dot\n"
            "files that are used to generate the various graphs.\n"
            "The default value is: YES.\n"
            "This tag requires that the tag HAVE_DOT is set to YES.");
}

