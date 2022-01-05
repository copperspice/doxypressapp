/************************************************************************
*
* Copyright (c) 2014-2022 Barbara Geller & Ansel Sermersheim
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

#include "mainwindow.h"

static QString find_HelpBody(QString name);
static QMap<QString, QString> s_fullHelp;

void MainWindow::createMap()
{
   // 1  load help data
   this->configFullHelp();

   // 2  load bigMap
   QList<QCheckBox *> temp_CB = this->findChildren<QCheckBox *>();

   for (auto ptr : temp_CB)  {
      HelpData temp = HelpData{HelpData::BOOL, nullptr, find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QGroupBox *> temp_GB = this->findChildren<QGroupBox *>();

   for (auto ptr : temp_GB)  {
      HelpData temp = HelpData{HelpData::STRING, nullptr, find_HelpBody(ptr->objectName()) };
      m_bigMap.insert(ptr, temp);
   }

   QList<QComboBox *> temp_CM = this->findChildren<QComboBox *>();

   for (auto ptr : temp_CM)  {
      HelpData temp = HelpData{HelpData::STRING, nullptr, find_HelpBody(ptr->objectName()) };
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->installEventFilter(this);
   }

   QList<QLineEdit *> temp_LE = this->findChildren<QLineEdit *>();

   for (auto ptr : temp_LE)  {
      HelpData temp = HelpData{HelpData::STRING, nullptr, find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QPlainTextEdit *> temp_PT = this->findChildren<QPlainTextEdit *>();

   for (auto ptr : temp_PT)  {
      HelpData temp = HelpData{HelpData::STRING, nullptr, find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QRadioButton *> temp_RB = this->findChildren<QRadioButton *>();

   for (auto ptr : temp_RB)  {
      HelpData temp = HelpData{HelpData::BOOL, nullptr, find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QSpinBox *> temp_SB = this->findChildren<QSpinBox *>();

   for (auto ptr : temp_SB)  {
      HelpData temp = HelpData{HelpData::INT, nullptr, find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->installEventFilter(this);
   }

   // get the buddy list
   QList<QLabel *> list_Labels = this->findChildren<QLabel *>();

   for (auto ptr : list_Labels)  {

      QWidget *buddy = ptr->buddy();

      if (buddy) {
         auto index = m_bigMap.find(buddy);

         if (index != m_bigMap.end()) {
            index->label = ptr;
            index->title = ptr->text();

            // remove the colon
            index->title.replace(":", "");
         }
      }
   }

   //
   setupLimits();
   clearAllFields();
}

bool MainWindow::getDefault(QCheckBox *name)
{
   bool retval = false;
   auto index  = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->defBool;
   }

   return retval;
}

QString MainWindow::getDefault(QWidget *name)
{
   QString retval;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->defValue;
   }

   return retval;
}

int MainWindow::getDefault(QSpinBox *name)
{
   int retval = 0;
   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->defInt;
   }

   return retval;
}

QLabel *MainWindow::getLabel(QWidget *name)
{
   QLabel *retval = nullptr;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->label;
   }

   return retval;
}

void MainWindow::setDefault(QCheckBox *name)
{
   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      // update default value

      if (name->isChecked()) {
         index->defBool = true;
      }
   }
}

void MainWindow::setDefault(QComboBox *name)
{
   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      // update default value
      index->defValue = name->currentText();
   }
}

void MainWindow::setDefault(QLineEdit *name)
{
   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      // update default value
      index->defValue = name->text();
   }
}

void MainWindow::setDefault(QPlainTextEdit *name)
{
   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      // update default value
      index->defValue = name->toPlainText();
   }
}

void MainWindow::setDefault(QSpinBox *name)
{
   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      // update default value
      index->defInt = name->value();
   }
}

void MainWindow::setLabelColor(int option, QWidget *label)
{
   if (option == DEFAULT) {
      QPalette temp = label->palette();
      temp.setColor(QPalette::WindowText, Qt::black);

      label->setPalette(temp);

    } else if (option == MODIFIED) {
      QPalette temp = label->palette();
      temp.setColor(QPalette::WindowText, QColor(160, 0, 160) );

      label->setPalette(temp);
   }
}

void MainWindow::hoverChanged(QWidget *old_Widget, QWidget *new_Widget)
{
   bool isModified = false;

   if (new_Widget == m_ui->runText) {
      return;
   }

   // adjust help message for new_Widget
   setHelpText(new_Widget);

   while (true) {
      // adjust color on old_Widget if updated

      QCheckBox *temp_CB = dynamic_cast<QCheckBox *>(old_Widget);

      if (temp_CB) {
         bool defValue = getDefault(temp_CB);
         QLabel *label = getLabel(temp_CB);

         if (label) {
            if (temp_CB->isChecked() == defValue) {
               setLabelColor(DEFAULT, label);

            } else {
               setLabelColor(MODIFIED, label);
               isModified = true;

            }
         }

         break;
      }

      // QGroupBox - do nothing

      QComboBox *temp_CM = dynamic_cast<QComboBox *>(old_Widget);

      if (temp_CM) {
         QString defValue = getDefault(temp_CM);
         QLabel *label    = getLabel(temp_CM);

         if (label) {
            if (temp_CM->currentText().trimmed() == defValue) {
               setLabelColor(DEFAULT, label);

            } else {
               setLabelColor(MODIFIED, label);
               isModified = true;

            }
         }

         break;
      }

      QLineEdit *temp_LE = dynamic_cast<QLineEdit *>(old_Widget);

      if (temp_LE) {
         QString defValue = getDefault(temp_LE);
         QLabel *label    = getLabel(temp_LE);

         if (label) {
            if (temp_LE->text().trimmed() == defValue ) {
               setLabelColor(DEFAULT, label);

            } else {
               setLabelColor(MODIFIED, label);
               isModified = true;
            }
         }

         break;
      }

      QPlainTextEdit *temp_PT = dynamic_cast<QPlainTextEdit *>(old_Widget);

      if (temp_PT) {
         QString defValue = getDefault(temp_PT);
         QLabel *label    = getLabel(temp_PT);

         if (label) {

            if (temp_PT->toPlainText().trimmed() == defValue) {
               setLabelColor(DEFAULT, label);

            } else {
               setLabelColor(MODIFIED, label);
               isModified = true;

            }
         }

         break;
      }

      // QRadioButton - do nothing

      QSpinBox *temp_SB = dynamic_cast<QSpinBox *>(old_Widget);

      if (temp_SB) {
         int defValue  = getDefault(temp_SB);
         QLabel *label = getLabel(temp_SB);

         if (label) {

            if (temp_SB->value() == defValue) {
               setLabelColor(DEFAULT, label);

            } else {
               setLabelColor(MODIFIED, label);
               isModified = true;

            }
         }
      }

      // just one time through
      break;
   }

   if (isModified && ! m_modified)  {
      setDoxyTitle(true);
   }
}

void MainWindow::setHelpText(QWidget *name)
{
   if (name->objectName() == "qt_spinbox_lineedit")  {
      name = name->parentWidget();
   }

   QString title = getHelpTitle(name);
   QString body  = getHelpBody(name);

   QString text = "<b>" + title + "</b><br><br>" + body;

   int index = m_ui->tabWidget->currentIndex();

   if (index == 0) {
      m_ui->help_t1->setText(text);

   } else if (index == 1) {
      m_ui->help_t2->setText(text);

   } else {
      m_ui->help_t3->setText(text);

   }
}

QString MainWindow::getHelpTitle(QWidget *name)
{
   QString retval;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->title;
   }

   if (retval.isEmpty()) {
      retval = "Help Information";
   }

   return retval;
}

QString MainWindow::getHelpBody(QWidget *name)
{
   QString retval;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->body;
   }

   if (retval.isEmpty()) {
      retval = "Missing help text";
   }

   return retval;
}

static QString find_HelpBody(QString name)
{
   QString retval;

   if (name.endsWith("_CB") || name.endsWith("_SB") || name.endsWith("_CM"))  {
      name = name.left(name.length() - 3);

   } else if (name.endsWith("_CB1") || name.endsWith("_CB2")  )  {
      name = name.left(name.length() - 4);

   }

   auto index = s_fullHelp.find(name.toUpper());

   if (index != s_fullHelp.end()) {
      retval = index.value();

   } else {
      retval = name + " (no help value)";

   }

   return retval;
}

void MainWindow::configFullHelp()
{
   s_fullHelp.clear();

   s_fullHelp.insert("PROJECT_NAME",
      "This tag is a single word or a sequence of words surrounded by double quotes "
      "which identifies your project. The project name is used in the title of the generated documentation.");

   s_fullHelp.insert("PROJECT_BRIEF",
      "This tag is used to specify a single line description which will "
      "appear at the top of each generated documentation page.");

   s_fullHelp.insert("PROJECT_VERSION",
      "This tag is used to enter a project number or revision number.");

   s_fullHelp.insert("PROJECT_LOGO",
      "This tag specifies a logo or icon which is included in the documentation. "
      "The height of the logo should not exceed 55 "
      "pixels and the width should not exceed 200 pixels.");

   s_fullHelp.insert("OUTPUT_DIR",
      "This tag is used to specify the relative or absolute path where the generated "
      "documentation will be written. If a relative path is entered it will be relative to the "
      "location where DoxyPress was started. If left blank the current directory will be used.");

   s_fullHelp.insert("OPTIMIZE_CPLUS_RB",
      "Set this tag to optimize for C++."
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("OPTIMIZE_JAVA_RB",
      "Set this tag if your project consists of Java or C# source code. "
      "DoxyPress will generate documentation tailored for these languages. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("OPTIMIZE_C_RB",
      "Set this tag if your project consists of PHP or C source code. "
      "DoxyPress will generate documentation tailored for these languages. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("OPTIMIZE_FORTRAN_RB",
      "Set this tag if your project consists of Fortran source code. "
      "DoxyPress will generate documentation tailored for Fortran. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("OPTIMIZE_PYTHON_RB",
      "Set this tag if your project consists of Python source code. "
      "DoxyPress will generate documentation tailored for Python. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HTML_PLAIN_RB",
      "This option is set if 'GENERATE HTML HELP' and 'GENERATE NAVIGATION TREEVIEW' are both disabled. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("HTML_NAV_RB",
      "Set this option to enable the 'GENERATE NAVIGATION TREEVIEW' tag."
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HTML_CHM_RB",
      "Set this option to enable the 'GENERATE HTML HELP' tag. If this option is set three "
      "additional files will be generated: index.hhp, index.hhc, and index.hhk. The index.hhp is used by "
      "Microsoft's HTML Help Workshop to convert HTML output into a single compiled HTML .chm file. "
      "<br><br>"
      "The default value is: NO"
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("LATEX_HYPER_PDF_RB",
      "If this tag is set the generated LaTeX is converted to a PDF using ps2pdf or pdflatex. "
      "The PDF file will contain hyperlinks instead of page references. "
      "<br><br>"
      "The default value is: YES"
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_PDF_RB",
      "If this tag is set DoxyPress will use the pdflatex program to generate the PDF. "
      "Set this option to YES, to obtain higher quality PDF documentation. "
      "<br><br>"
      "The default value is: YES"
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_PS_RB",
      "Setting this option will disable latex PDF output. "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   //
   s_fullHelp.insert("DIAGRAM_NONE_RB",
      "If this tag is set no Diagrams will be generated. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("DIAGRAM_BUILT_IN_RB",
      "If this tag is set the built in class diagram generator will be used. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("DIAGRAM_DOT_RB",
      "If this tag is set the dot program will be used to generate diagrams. "
      "<br><br>"
      "The default value is: NO");

   // project config
   s_fullHelp.insert("OUTPUT_LANGUAGE",
      "This tag is used to specify the language for the generated documentation. "
      "<br><br>"
      "The default value is: English");

   s_fullHelp.insert("ALIASES",
      "This tag is used to specify a number of aliases which act as commands in "
      "the documentation. An alias has the format: name=value"
      "<br><br>"
      "\"sideeffect=@par Side Effects:\\n\" "
      "<br><br>"
      "This example defines the command \\sideeffect (or @sideeffect). "
      "The result is a user-defined paragraph with a heading of \"Side Effects:\" ");

  s_fullHelp.insert("ABBREVIATE_BRIEF",
      "This tag is a list of phrases which are removed from the beginning of a brief description. "
      "<br><br>"
      "The default values are: The $name class, The $name widget, The $name file, is, provides, "
      "specifies, contains, represents, a, an, the");

   s_fullHelp.insert("STRIP_FROM_PATH",
      "This tag is used to specify what portion of the path to remove from the source files. "
      "The path is only stripped if one of the specified strings matches the beginning  "
      "part of the path. This tag can be used to show relative paths in the file list. "
      "If left blank the directory where DoxyPress is run is used as the path to strip. "
      "<br><br>"
      "Requires the tag 'FULL PATH NAME' is set to YES");

   s_fullHelp.insert("STRIP_FROM_INC_PATH",
      "This tag is used to specify what portion of the path to remove for include files. "
      "The include file shown in the documentation will indicate which header file to include, "
      "in order to use a particular class. If this tag is left blank the full path will be removed. ");

   s_fullHelp.insert("FULL_PATH_NAMES",
      "If this tag is set DoxyPress will prepend the full path "
      "before file names in the file list and in the header files. If set to NO, the "
      "shortest path which makes the file name unique will be used. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("BRIEF_MEMBER_DESC",
      "If this tag is set a brief description will be included after each method, function, or other member "
      "in the documentation of a class or file. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("REPEAT_BRIEF",
      "If this tag is set DoxyPress will prepend the brief description of a member or "
      "function before the detailed description. "
      "<br><br>"
      "If 'BRIEF DESCRIPTION' and 'HIDE UNDOCUMENTED MEMBERS' are set to NO, the "
      "brief descriptions will be completely suppressed. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("ALWAYS_DETAILED_SEC",
      "If this tag and 'REPEAT BRIEF DESCRIPTION' is set, a detailed section will be "
      "generated even if there is only a brief description. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("JAVADOC_AUTO_BRIEF",
      "If this tag is set DoxyPress will interpret the first sentence of a JavaDoc style comment "
      "as the brief description. If set to NO an explicit @brief command is required. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("QT_AUTO_BRIEF",
      "If this tag is set DoxyPress will interpret the first sentence of a Qt style comment "
      "as the brief description. If set to NO, an explicit @brief command is required. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("MULTILINE_CPP_BRIEF",
      "If this tag is set DoxyPress will treat a multi-line C++ special comment "
      "block (a block of //! or /// comments) as a brief description. "
      "The default is to treat a multi-line C++ special comment block as a detailed description. "
      "Setting this tag to YES, means Rational Rose comments are not recognized. "
      "<br><br>"
      "The default value is: NO");

  s_fullHelp.insert("TAB_SIZE",
      "This tag is used to set the number of spaces in a tab. This value is used to "
      "replace tabs with spaces in code fragments. "
      "<br><br>"
      "Minimum: 1, Maximum: 16, Default: 4");

  s_fullHelp.insert("LOOKUP_CACHE_SIZE",
      "The size of the symbol lookup cache can be set using this tag. If the cache is too small "
      "DoxyPress may run slower. At the end of a run DoxyPress will report the cache usage and "
      " suggest the optimal cache size. "
      "<br><br>"
      "Minimum: 0, Maximum: 9, Default: 0");

   // build config
   s_fullHelp.insert("SHORT_NAMES",
      "If this tag is set shorter file names will be generated which may be less readable. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("ALLOW_UNICODE_NAMES",
      "If this tag is set DoxyPress will allow non-ASCII characters to appear "
      "in the names of generated files. If set to NO, non-ASCII characters will be "
      "escaped. For example: _xE3_x81_x84 will be used for Unicode U+3044. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("CREATE_SUBDIRS",
      "If this tag is set, 4096 sub-directories (in 2 levels) will be created "
      "under the output directory for each output format type. "
      "The output files will be distributed in these directories. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INLINE_INHERITED_MEMBER",
      "If this tag is set DoxyPress will show all inherited members of a class in the "
      "documentation of that class as if those members were ordinary class members. "
      "Constructors, destructors and assignment operators of the base classes will not be shown. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("MARKDOWN",
      "If this tag is set DoxyPress will preprocess all comments according to the Markdown format. "
      "You can mix Markdown, HTML, and XML commands. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("AUTO_LINK",
      "When this tag is set DoxyPress tries to link words which correspond to documented "
      "classes, functions, or namespaces to their corresponding documentation. An auto link can "
      "be prevented for individual cases by adding a % character in front of the word or "
      "globally by setting this tag to NO. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("PYTHON_DOCSTRING",
      "If this tag is set Python docstrings will be parsed for DoxyPress commands. If this tag is not set "
      "docstrings will be treated as verbatim preformatted text.");

   s_fullHelp.insert("STRICT_SIG_MATCHING",
      "If this tag is set then an exact match of the function signature is required to document "
      "a function. An exact match means the function name and parameters must all match. "
      "If this tag is set to NO DoxyPress will do an approximate match. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INTERNAL_DOCS",
      "This tag determines if documentation which is typed after an \\internal command "
      "is included in the documentation. Set to YES to include the internal documentation, "
      "set to NO, to exclude the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("FORCE_LOCAL_INCLUDES",
      "If this tag is set DoxyPress will list include files with double quotes "
      "instead of less than and greater than symbols. "
      "<br><br>"
      "The default value is: NO");

    s_fullHelp.insert("INHERIT_DOCS",
      "If this tag is set then an undocumented member inherits the "
      "documentation from any documented member which it re-implements. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SEPARATE_MEMBER_PAGES",
      "If this tag is set DoxyPress will produce a new page for each member. If set to NO "
      "the documentation of a member will be part of the file/class/namespace which contains the member. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("ALLOW_SUB_GROUPING",
      "Set this tag to allow class member groups of the same type. "
      "For instance, a group of public functions will be grouped with other functions of the same type. "
      "Set this tag to NO to prevent subgrouping. Alternatively, sub-grouping can be done per "
      "class using the \\nosubgrouping command. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("DUPLICATE_DOCS",
       "If member grouping is used in the documentation and this tag "
       "is set, DoxyPress will reuse the documentation of the first "
       "member in the group (if any) for the other members of the group. By default "
       "all members of a group must be documented explicitly. "
       "<br><br>"
       "The default value is: NO");

   s_fullHelp.insert("GROUP_NESTED_COMPOUNDS",
       "If this tag is enabled then nested classes or structures will be in the same group "
       "the parent class or structure is located. If disabled nested classes or structures "
       "can be added explicitly using the \\ingroup command. "
       "<br><br>"
       "The default value is: NO");

   s_fullHelp.insert("INLINE_GROUPED_CLASSES",
      "When this tag is set classes, structs, and unions are shown inside the group "
      "in which they are included. Use the \\ingroup command instead of a "
      "separate page (for HTML and Man pages) or section (for LaTeX and RTF). "
      "<br><br>"
      "Note: This feature does not work in combination with 'SEPARATE MEMBER PAGES' "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INLINE_SIMPLE_STRUCT",
      "When this tag is set structs, classes, and unions with only public data fields or simple "
      "typedef fields, will be shown inline in the documentation of the scope in which they are "
      "defined. If this tag is set to NO then structs, classes, and unions are shown on a separate page "
      "(for HTML and Man pages) or section (for LaTeX and RTF). "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("USE_TYPEDEF_NAME",
      "When the tag is set a typedef of a struct, union, or enum is documented as struct, union, or "
      "enum with the name of the typedef. For example, Typedef struct Type_S {} Type_T, will appear "
      "as a struct with name Type_T. When disabled, the typedef will appear as a member of a file, "
      "namespace, or class and the struct will be named Type_S. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_ALL",
      "If this tag is set DoxyPress will assume all entities are documented. "
      "Private class members and static file members will be hidden unless the "
      "'EXTRACT PRIVATE' or the 'EXTRACT STATIC' tags are set to YES. "
      "<br><br>"
      "Setting this tag will disable warnings about undocumented members. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_PRIVATE",
      "If this tag is set then all private members of a class will be included in the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_PRIVATE_VIRTUAL",
      "If this tag is set private virtual methods of a class will be included in the documentation even "
      " when extract private is off."
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_PACKAGE",
      "If this tag is set then all members with package or internal scope will be included in the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_STATIC",
      "If this tag is set then all static members of a file will be included in the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_LOCAL_CLASSES",
      "If this tag is set then classes and structs defined locally in source files will "
      "be included in the documentation. If set to NO, only classes defined in "
      "header files are included. This tag has no effect on Java source code."
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("EXTRACT_LOCAL_METHODS",
      "This tag is only useful for Objective-C source code. If set then local methods, "
      "which are defined in the implementation section but not in the interface, are "
      "included in the documentation. If set to NO only methods in the interface are included. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_ANON_NAMESPACES",
      "If this tag is set the members of anonymous namespaces will be extracted and appear "
      "as a namespace called 'anonymous_namespace{file}'. The value for 'file' will be "
      "replaced with the base name of the file which contains the anonymous namespace. "
      "By default anonymous namespaces are hidden. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_UNDOC_MEMBERS",
      "If this tag is set DoxyPress will hide all undocumented members inside "
      "documented classes or files. If set to NO, these members will be included in the "
      "various overviews, but no documentation section is generated. "
      "<br><br>"
      "This tag has no effect if 'EXTRACT ALL' is set to YES. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_UNDOC_CLASSES",
      "If this tag is set DoxyPress will hide all undocumented classes which are "
      "normally visible in the class hierarchy. If set to NO, these classes will be "
      "included in the various overviews. "
      "<br><br>"
      "This tag has no effect if 'EXTRACT ALL' is set to YES. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_FRIEND_COMPOUNDS",
      "If this tag is set DoxyPress will hide all friend class, struct, or union declarations. "
      "If set to NO these declarations will be included in the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_IN_BODY_DOCS",
      "If this tag is set DoxyPress will hide any documentation blocks found inside the body "
      "of a function. If set to NO, these blocks will be appended to the function's detailed "
      "documentation block. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_SCOPE_NAMES",
      "If this tag is set to NO then members will be shown with their full class "
      "and namespace scopes in the documentation. If set to YES, the scope will be hidden. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_COMPOUND_REF",
      "If this tag is set to NO, DoxyPress will append additional text to a page's title, "
      "such as Class Reference. If set to YES the compound reference will be hidden. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_NAVTREE_MEMBERS",
      "If this tag is set to YES, then members will not be shown in the navigation treeview. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SHOW_INCLUDE_FILES",
      "If this tag is set DoxyPress will put a list of the files which are "
      "included by a file, in the documentation of that file. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SHOW_GROUPED_MEMBERS_INC",
      "If this tag is set DoxyPress will add for each grouped member, an include statement "
      "indicating which file to include in order to use the member. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SHOW_USED_FILES",
      "When this tag is set a list will be generated at the bottom of classes and struct "
      "documentation. The list will contain all file names which were used to generate the "
      "documentation. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SHOW_FILE_PAGE",
      "Set this tag to NO to disable the generation of the Files page. "
      "This will remove the Files entry from the Quick Index and from the Folder Tree View if specified. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SHOW_NAMESPACE_PAGE",
      "Set this tag to NO to disable the generation of the Namespaces page. "
      "This will remove the Namespaces entry from the Quick Index and from the Folder Tree "
      "View if specified. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("INLINE_INFO",
      "If this tag is set then '[inline]' is displayed for inline members. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SORT_MEMBER_DOCS",
      "If this tag is set DoxyPress will sort the detailed documentation alphabetically. "
      "If set to NO, members will appear in the order they are defined in the source code. "
      "To sort pages manually refer to the DoxyPress manual regarding the \\sortid command."
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SORT_BRIEF_DOCS",
      "If this tag is set DoxyPress will sort the brief descriptions of file, namespace, "
      "and class members alphabetically by member name. "
      "If set to NO, members will appear in the order they are defined in the source code. "
      "This tag affects the order of entries in the class list. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SORT_CONSTRUCTORS_FIRST",
      "If this tag is set DoxyPress will sort class members with constructors and destructors listed first. "
      "If this tag is set to NO they will appear in the order defined by 'SORT BRIEF DOCUMENTATION' "
      "and 'SORT MEMBER DOCUMENTATION'. "
      "<br><br>"
      "If 'SORT BRIEF DOCUMENTATION' is set to NO, this tag is ignored when sorting brief member documentation. "
      "<br><br>"
      "If 'SORT MEMBER DOCUMENTATION' is set to NO, this tag is ignored when sorting detailed member documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SORT_GROUP_NAMES",
      "If this tag is set DoxyPress will sort the groups alphabetically, as defined by the \\defgroup command. "
      "If set to NO, groups will appear in the order they are defined in the source code. Groups will appear "
      "on a page called modules. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SORT_BY_SCOPE_NAME",
      "If this tag is set the class list will be sorted by fully qualified names "
      "including namespaces. If set to NO, the class list will be sorted only by "
      "class name, not including the namespace part. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SORT_CLASS_CASE_SENSITIVE",
      "If this tag is set to YES then sorting of classes in the index will be case sensitive. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("CASE_SENSITIVE_FNAME",
      "If this tag is set to NO all file names will be generated in lower case. "
      "If set to YES, case is preserved. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("GEN_TODO_LIST",
      "This tag is used to enable (YES) or disable (NO) the todo list. "
      "This list is created by putting \\todo commands in the documentation. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("GEN_TEST_LIST",
      "This tag is used to enable (YES) or disable (NO) the test list. "
      "This list is created by putting \\test commands in the documentation. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("GEN_BUG_LIST",
      "This tag is used to enable (YES) or disable (NO) the bug list. "
      "This list is created by putting \\bug commands in the documentation. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("GEN_DEPRECATE_LIST",
      "This tag is used to enable (YES) or disable (NO) the deprecated list. This list is "
      "created by putting \\deprecated commands in the documentation. "
      "<br><br>"
      "The default value is: YES");

   // build output
   s_fullHelp.insert("MAX_INIT_LINES",
      "This tag specifies the maximum number of lines which are displayed in the documentation for the "
      "definition of a variable or a macro. If the variable or macro consists of more lines than specified in "
      "this tag, the entire initializer will be hidden. "
      "This tag can be overridden on a per variable or macro basis by using the \\showinitializer or "
      "\\hideinitializer command. "
      "<br><br>"
      "A value of 0 will hide initializers completely. "
      "<br><br>"
      "Minimum: 0, Maximum: 10000, Default: 30");

   s_fullHelp.insert("ENABLED_SECTIONS",
      "This tag is used to enable conditional documentation by marking a section with one of the following: "
      "<br><br>"
      "\\if <section_label> ... \\endif "
      "<br><br>"
      "\\cond <section_label> ... \\endcond");

   s_fullHelp.insert("FILE_VERSION_FILTER",
      "This tag is used to specify a program which DoxyPress should invoke to obtain the current version "
      "for each file. DoxyPress will invoke this program and pass a file name as the first command line parameter. "
      "The value written to standard output is used as the file version.");

   s_fullHelp.insert("MAIN_PAGE_NAME",
      "This tag is used to specify an input file name which should be used for the index page. "
      "<br><br>"
      "The file name will not be included in the navigation treeview file listing if FILES is enabled.");

   s_fullHelp.insert("MAIN_PAGE_OMIT",
      "If this tag is set the index page will not be included in the navigation treeview. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("LAYOUT_FILE",
      "This tag is used to specify a layout file which controls the global structure of the "
      "generated output. "
      "<br><br>"
      "If DoxyPress is run from a directory containing a file called DoxyPress_Layout.xml, "
      "DoxyPress will parse it automatically even if this tag is left empty."
      "<br><br>"
      "To create a new layout file run 'DoxyPress --l [layout file name]' ");

   s_fullHelp.insert("NS_ALIAS",
      "This tag is used to specify a list of namespaces which should be renamed in the documentation. "
      "The format for this tag is: namespace=alias"
      "<br><br>"
      "As an example boost::thread=bt will replace the boost::thread everywhere it occurs in your documentation "
      "with the value of 'bt'.");

   s_fullHelp.insert("TOC_INCLUDE_HEADERS",
      "In markdown syntax a line of documentation which starts with a # sign is called a header. "
      "In the output this 'markdown header' will look like an html H1 element. Multiple # signs can "
      "be used up to the standard header limit of 6, which corresponds to H6."
      "<br><br>"
      "When this tag is set to a value greater than 0 all markdown headers with the same or "
      "greater level as this tag will automatically be included in the table of contents."
      "This will occur even if the header does not have an id attribute."
      "<br><br>"
      "Requires the tag 'MARKDOWN SUPPORT' is set to YES");

/*
   s_fullHelp.insert("BB_STYLE",
      "This tag is used to enable the special comment style of documenting source code. Comments in the "
      "source must be below the source code. (Experimental)");
*/

   // language
   s_fullHelp.insert("TCL_SUBST",
      "This tag is used to specify a number of word-keyword mappings. "
      "A mapping has the form \"name=value\". For example, adding \"class=itcl::class\" "
      "allows you to use the command class in the itcl::class meaning."
      "<br><br>"
      "This tag is only used when parsing a TCL file.");

   s_fullHelp.insert("LANGUAGE_MAPPING",
      "DoxyPress selects the language parser depending on the extension of the files it is parsing. "
      "Use this tag to assign a specific parser to use for a given file extension. "
      "The format for this tag is ext=language, where ext is a file extension and language "
      "is one of the parsers supported by DoxyPress. "
      "<br><br>"
      "The supported parsers are: C#, C, C++, IDL, Java, JavaScript, Objective-C, "
      "PHP, Python, Fortran, FortranFixed, FortranFree. "
      "<br><br>"
      "As an example, to treat .f files as C (default is Fortran), use: f=C "
      "<br><br>"
      "Note: For files with no extensions use no_extension as a placeholder. "
      "For custom extensions modify 'INPUT FILE PATTERNS', otherwise these files will not be read by DoxyPress.");

   s_fullHelp.insert("BUILT_IN_STL_SUPPORT",
      "If you use STL classes (std::string, std::vector, etc.) but do not want "
      "to include a tag file for the STL sources as input, then set this tag to YES. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("CPP_CLI_SUPPORT",
      "If you use Microsoft's C++/CLI language this tag should set set to enable parsing support. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SIP_SUPPORT",
      "Set this tag if your project consists of sip sources only. DoxyPress "
      "will parse them like normal C++ but will assume all classes use public instead "
      "of private inheritance when no explicit protection keyword is present. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("IDL_SUPPORT",
      "For Microsoft's IDL there are propget and propput attributes to indicate "
      "getter and setter methods for a property. If this option is set "
      "DoxyPress will replace the get and set methods by a property in the documentation. "
      "This only works if the methods are getting or setting a simple type. "
      "If you want to show the methods anyway, set this option to NO. "
      "<br><br>"
      "The default value is: YES");

   // messages
   s_fullHelp.insert("QUIET",
      "This tag is used to enable (YES) or disable (NO) messages which are generated to "
      "standard output. If this tag is set then standard messages will be generated. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("WARNINGS",
      "This tag is used to enable (YES) or disable (NO) warning messages which are generated to "
      "standard error (stderr). If this tag is set then standard error messages will be generated. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("WARN_UNDOC",
      "If this tag is set DoxyPress will generate warnings for undocumented members. "
      "If the tag 'EXTRACT ALL' is set, then this tag will automatically be disabled. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("WARN_DOC_ERROR",
      "If this tag is set DoxyPress will generate warnings for potential errors "
      "such as not documenting some parameters in a documented function, documenting parameters "
      "which do not exist, or using markup commands incorrectly. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("WARN_UNDOC_PARAM",
      "This tag is used to warn about functions which are documented but have no "
      "documentation for their parameters or return value. If set to NO, only warnings "
      "about wrong or incomplete parameter documentation, but not about the absence of documentation, "
      "will be generated. "
      "If the tag 'EXTRACT ALL' is set, then this tag will automatically be disabled. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("WARN_PARAM_MISMATCH",
      "Setting this tag will generate a message when the parameter variable name in the declaration does "
      "not match the name in the documentation."
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("WARN_FORMAT",
      "This tag determines the format of the warning messages. The macro '$file'' will be "
      "replaced by the file name, '$line' is replaced by the line number where the warning originated, "
      "and '$text' is replaced with warning text. This tag can also include '$version' which will "
      "be replaced by the version of the file if the text can be obtained from the 'FILE VERSION FILTER'. "
      "<br><br>"
      "The default value is: $file:$line: $text");

   s_fullHelp.insert("WARN_LOGFILE",
      "This tag is used to specify a file where warning and error "
      "messages should be written. If left blank the output is written to standard error (stderr).");

   // input source files
   s_fullHelp.insert("INPUT_SOURCE",
      "This tag is used to specify full file names or directories containing source files to be processed. "
      "Wildcards are not permitted. "
      "<br><br>"
      "Note: If this tag is empty only the current directory is searched.");

   s_fullHelp.insert("INPUT_ENCODING",
      "This tag is used to specify the character encoding of the source files which are parsed. "
      "<br><br>"
      "The default value is: UTF-8");

   s_fullHelp.insert("INPUT_PATTERNS",
      "If the 'INPUT SOURCE' tag contains directories then this tag is used to "
      "specify wildcard patterns. Files matching these patterns will be processed by DoxyPress as source. "
      "Custom file extensions need to be defined in the 'LANGUAGE MAPPING' tag.  "
      "<br><br>"
      "If this tag is empty the default will be: " + m_filePatterns);

   s_fullHelp.insert("INPUT_RECURSIVE",
      "This tag is used to specify whether or not subdirectories should be searched for input files. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXCLUDE_FILES",
      "This tag is used to specify files or directories which should be excluded from "
      "the 'INPUT SOURCE' tag. "
      "<br><br>"
      "Note: The relative paths are relative to the directory from which DoxyPress is run.");

   s_fullHelp.insert("EXCLUDE_SYMLINKS",
      "This tag is used to select if symbolic links are excluded from the input. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXCLUDE_PATTERNS",
      "If the 'INPUT SOURCE' tag contains directories, this tag will be used to "
      "specify wildcard patterns to exclude certain files from those directories. "
      "<br><br>"
      "Note: Wildcards are matched using the absolute path. As an example, to "
      "exclude all test directories use the pattern \x2a/test/\x2a ");

   s_fullHelp.insert("EXCLUDE_SYMBOLS",
      "This tag is used to specify symbol names, namespaces, classes, functions, etc. "
      "which should be excluded from the documentation. "
      "<br><br>"
      "Examples: FooNamespace, BarClass, FooNamespace::BarClass, FooNamespace::*Test ");

   // input other
   s_fullHelp.insert("EXAMPLE_SOURCE",
      "This tag is used to specify full file names or directories which contain example code fragments. "
      "Refer to the DoxyPress manual regarding the \\include command.");

   s_fullHelp.insert("EXAMPLE_PATTERNS",
      "If the value of the 'EXAMPLE FILES OR DIRECTORIES' tag contains directories then this tag is used "
      "to specify one or more wildcard patterns like *.cpp or *.h to filter out the "
      "source files in the example directories. "
      "<br><br>"
      "The default value is: *");

   s_fullHelp.insert("EXAMPLE_RECURSIVE",
      "If this tag is set then example source code will be searched recursively for "
      "the \\include or \\dontinclude commands. This search will occur even if the 'INPUT, SCAN RECURSIVELY' "
      "tag is set to NO. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("IMAGE_PATH",
      "This tag can be used to specify one or more files or directories "
      "which contain images to be included in the documentation. "
      "<br><br>"
      "Refer to the DoxyPress manual regarding the \\image command.");

   s_fullHelp.insert("MDFILE_MAINPAGE",
      "This tag refers to the name of a markdown file which is part of the input. "
      "The contents of the file will be placed on the main index.html page.");


   // input filters
   s_fullHelp.insert("FILTER_PROGRAM",
      "This tag is used to specify a program which is called to filter each input file. "
      "DoxyPress will invoke the filter program and pass the input file name as the first parameter. "
      "The filter program must not add or remove lines and is run before the "
      "source code is scanned, not when the documentation output code is generated. "
      "<br><br>"
      "The output of the filter program will be used instead of your original source code."
      "<br><br>"
      "Custom file extensions need to be defined in the 'LANGUAGE MAPPING' tag.");

   s_fullHelp.insert("FILTER_PATTERNS",
      "This tag is used to specify filter programs based on a wildcard match. "
      "DoxyPress will compare source file names with the wildcards and apply the "
      "filter program if there is a match. "
      "If this tag is empty or none of the wildcards match then the program specified by the 'FILTER PROGRAM NAME' "
      "will be used. "
      "<br><br>"
      "Custom file extensions need to be defined in the 'LANGUAGE MAPPING' tag. "
      "<br><br>"
      "The filter programs are listed in the format: wildcard=filter program "
      "<br><br>"
      "Example: *.cpp=my_cpp_filter");

   s_fullHelp.insert("FILTER_SOURCE_FILES",
      "If this tag is set the program specified in the 'FILTER PROGRAM NAME' tag will be used to filter the input "
      "source files when generating source documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("FILTER_SOURCE_PATTERNS",
      "This tag is used to specify filter programs based on a wildcard match. "
      "DoxyPress will compare each source file name with the list of wildcards and apply the "
      "filter program which matches. This tag will override the 'SELECT FILTER PROGRAM BY WILDCARD'."
      "<br><br>"
      "Example: *.c=some_c_filter, *.h="
      "<br><br>"
      "The first pattern specifies which filter program to use for every *.c file. The second patter specifies "
      "not to use a filter program for any header file. "
      "<br><br>"
      "Requires the tag 'FILTER SOURCE CODE FILES' is set to YES");


   // index page
   s_fullHelp.insert("ALPHA_INDEX",
      "If this tag is set an alphabetical index of all compounds will be generated. "
      "This tag should be enabled if the project contains numerous "
      "classes, structs, unions or interfaces. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("COLS_IN_INDEX",
      "This tag is used to specify the number of columns to use for the alphabetical index. "
      "<br><br>"
      "Minimum: 1, Maximum: 20, Default: 5 "
      "<br><br>"
      "Requires the tag 'ALPHABETICAL INDEX' is set to YES");

   s_fullHelp.insert("IGNORE_PREFIX",
      "This tag is used to define a prefix or a list of prefixes, which should be ignored "
      "while generating the index headers. "
      "This is useful when several classes start with a common prefix as too many entries "
      "would appear under the same letter in the alphabetical index. "
      "<br><br>"
      "Requires the tag 'ALPHABETICAL INDEX' is set to YES");

   // preprocessing
   s_fullHelp.insert("ENABLE_PREPROCESSING",
      "If this tag is set DoxyPress will evaluate all "
      "C preprocessor directives found in the source and include files. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SEARCH_INCLUDES",
      "If this tag is set then files specified in the 'INCLUDE PATH' tag will be "
      "searched if the #include directive is found in the source code. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("INCLUDE_PATH",
      "This tag is used to specify one or more directories which contain include files."
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' and 'SEARCH INCLUDES' are set to YES");

   s_fullHelp.insert("INCLUDE_PATTERNS",
      "This tag is used to specify wildcard patterns (like *.h) "
      "to filter out the header files in the directories. If left blank, the patterns "
      "specified with the 'INPUT FILE PATTERNS' tag will be used. "
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("MACRO_EXPANSION",
      "If this tag is set DoxyPress will expand all macro names in the source code. "
      "If set to NO, only conditional compilation will be performed. "
      "Macro expansion can be done in a controlled way by setting 'EXPAND ONLY PREDEFINED MACROS' to YES. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("EXPAND_ONLY_PREDEFINED",
      "If this tag and 'MACRO EXPANSION' are both set to YES, the macro expansion is "
      "limited to the macros specified by 'PREDEFINED MACROS' and 'EXPAND AS DEFINED' tags. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("SKIP_FUNCTION_MACROS",
      "If this tag is set DoxyPress will preprocess and "
      "remove all references to function-like macros which are alone on a line, are in "
      "uppercase, and do not end with a semicolon. Such function macros "
      "are typically used for boiler-plate code and will confuse the parser if not removed. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("PREDEFINED_MACROS",
      "This tag is used to specify one or more macro names which are defined during"
      "DoxyPress preprocessor. To prevent a macro definition from being undefined via #undef or "
      "recursively expanded, use := instead of the = operator. "
      "If the definition and the \"=\" are omitted, \"=1\" is assumed. "
      "<br><br>"
      "The format for this tag is: name=definition, with no spaces."
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("EXPAND_AS_DEFINED",
      "If the 'MACRO EXPANSION' and 'EXPAND ONLY PREDEFINED MACROS' tags are set to YES, this "
      "tag is used to specify a list of macro names which should be expanded. "
      "DoxyPress will use the macro definitions found in your source code. Use the 'PREDEFINED MACROS' "
      "tag to specify a different macro definition. "
      "<br><br>"
      "Requires the tag 'ENABLE PREPROCESSING' is set to YES");


   // clang
   s_fullHelp.insert("CLANG_PARSING",
      "If this tag is set DoxyPress will use the clang parser for parsing C, C++, and "
      "Objective-C source code. Using clang might be slightly slower, however it will be more accurate."
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("CLANG_COMPILATION_PATH",
      "This tag is used to set the path to a JSON file previously created by a build tool like "
      "CMake or Visual Studio. This file is referred to as the 'compilation database'. If this tag is "
      "used then the compiler options do not need to be specified in the 'CLANG FLAGS'."
      "<br><br>"
      "Requires the tag 'CLANG PARSING' is set to YES");

   s_fullHelp.insert("CLANG_DIALECT",
      "This tag specifies which C++ standard should be used when parsing source code with clang and a compilation "
      "JSON file is not provided."
      "<br><br>"
      "The default value is: --std=c++14"
      "<br><br>"
      "Requires the tag 'CLANG PARSING' is set to YES");

   s_fullHelp.insert("CLANG_USE_HEADERS",
      "At run time DoxyPress will need access to the clang header files. Checking this tag will use "
      "the clang headers located in the installed 'doxypress/include' folder."
      "<br><br>"
      "To use a different set of clang headers ensure this tag is not set. You will need to specify the location "
      "of the clang headers using '-I pathToClang' in the 'CLANG FLAGS'."
      "<br><br>"
      "Requires the tag 'CLANG PARSING' is set to YES");

   s_fullHelp.insert("CLANG_INCLUDE_INPUT_SOURCE",
      "If this tag is set all directories listed in 'INPUT SOURCE' will be added to the "
      "clang flags using the '-I' option."
      "<br><br>"
      "Requires the tag 'CLANG PARSING' is set to YES");

   s_fullHelp.insert("CLANG_FLAGS",
      "This tag is used to pass flags to clang when parsing source files. "
      "<br><br>"
      "Requires the tag 'CLANG PARSING' is set to YES");

   // source listing
   s_fullHelp.insert("SOURCE_CODE",
      "If this tag is set then a list of source files will be generated. "
      "Documented entities will be cross-referenced with these sources. "
      "To remove all source code in the generated output set 'VERBATIM HEADERS' to NO "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INLINE_SOURCE",
      "Setting this tag will include the body of functions, classes and enums "
      "directly into the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("STRIP_CODE_COMMENTS",
      "If this tag is set DoxyPress will hide any special comment blocks from generated "
      "source code fragments. Normal C, C++, and Fortran comments will always remain visible. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("VERBATIM_HEADERS",
      "If this tag is set DoxyPress will generate a verbatim copy of the header "
      "file for each class for which an include is specified. Set to NO to disable this feature. "
      "Refer to the DoxyPress manual regarding the \\class command."
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("REF_BY_RELATION",
      "If this tag is set then for each documented entity all documented functions referencing it will be listed. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("REF_RELATION",
      "If this tag then for each documented function "
      "all documented entities called/used by that function will be listed. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("REF_LINK_SOURCE",
      "If this tag is set and 'SOURCE CODE' is set, the hyperlinks from functions "
      "in 'REFERENCED BY RELATION' and 'REFERENCES RELATION' will link to the source code. "
      "Otherwise, the link will refer to the generated documentation. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SOURCE_TOOLTIPS",
      "If this tag is set then hovering on a hyperlink in the source code will show a "
      "tooltip with additional information such as prototype, "
      "brief description, or links to the definition and documentation. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'SOURCE CODE' is set to YES");

   s_fullHelp.insert("USE_HTAGS",
      "If this tag is set then all references to source code will point to the HTML generated by "
      "the htags(1) program. Instead of the source browser generated by DoxyPress, the links to "
      "the source code will point to the output of htags. "
      "<br><br>"
      "The htags tool is part of GNU's global source tagging system. "
      "DoxyPress will invoke htags which will invoke gtags. Both programs must be available "
      "in your search path. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'SOURCE CODE' is set to YES");

   s_fullHelp.insert("SUFFIX_SOURCE_NAVTREE",
      "This tag is used to specify a list of file extensions which are treated as source. All entities "
      "in source files are considered to be 'local' rather than 'global'. Local entities will only be "
      "documented if the tag 'EXTRACT_LOCAL_CLASSES' is set. The 'dot' for the extension should not be specified "
      "in this tag."
      "<br><br>"
      "If this tag is empty the default will be: " + m_suffixSource );

   s_fullHelp.insert("SUFFIX_HEADER_NAVTREE",
      "This tag is used to specify a list of file extensions which will be treated as headers when building the "
      "Files and File List of the navigation treeview. The 'dot' for the extension should not be specified "
      "in this tag."
      "<br><br>"
      "If this tag is empty the default will be: " + m_suffixHeader);

   s_fullHelp.insert("SUFFIX_EXCLUDE_NAVTREE",
      "This tag is used to specify a list of file extensions which should not appear in the "
      "Files and File List of the navigation treeview. The 'dot' for the extension should not be specified "
      "in this tag."
      "<br><br>"
      "If this tag is empty the default will be: " + m_suffixExclude);

   // external
   s_fullHelp.insert("TAG_FILES",
      "Use this tag is used to specify one or more tag files which were generated from another project. "
      "For each entry specify the tag filename and the URL to the external documentation. "
      "The syntax for the URL can be relative or absolute. "
      "<br><br>"
      "tagFile_1=url_1 tagFile_2=url_2 ... "
      "<br><br>"
      "Refer to the DoxyPress documentation regarding 'Linking to external documentation' ");

   s_fullHelp.insert("GEN_TAGFILE",
      "When a file name is specified DoxyPress will create a tag file based on the input files. "
      "<br><br>"
      "Refer to the DoxyPress documentation regarding 'Linking to external documentation'.");

   s_fullHelp.insert("ALL_EXTERNALS",
      "If this tag is set all external classes will be listed in the class index. "
      "If set to NO only the inherited external classes will be listed. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTERNAL_GROUPS",
      "If this tag is set all external groups will be listed in the modules index. "
      "If set to NO only the current project's groups will be listed. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("EXTERNAL_PAGES",
      "If this tag is set all external pages will be listed in the related pages index. "
      "If set to NO only the current project's pages will be listed. "
      "<br><br>"
      "The default value is: YES");

   // dot
   s_fullHelp.insert("CLASS_DIAGRAMS",
      "If this tag is set DoxyPress will generate a class diagram in HTML and LaTeX "
      "for classes with base or super classes. Setting the tag to NO turns the diagrams off. "
      "It is recommended you install and use the dot program since it yields more powerful graphs. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("MSCGEN_PATH",
      "Message sequence charts can be defined within DoxyPress comments using the \\msc command. "
      "DoxyPress will run the mscgen program to produce the chart and insert it in the documentation. "
      "This tag is used to specify the directory where the mscgen program is located. "
      "If left empty the default path will be searched.");

   s_fullHelp.insert("DIA_PATH",
      "Diagrams made with Dia can be included in DoxyPress documentation. DoxyPress will "
      "run Dia to produce the diagram and insert it in the documentation. This tag is used to "
      "specify the directory where the Dia program is located. "
      "If left empty Dia is assumed to be found in the search path.");

   s_fullHelp.insert("HIDE_UNDOC_RELATIONS",
      "If this tag is set the inheritance and collaboration graphs will hide inheritance "
      "and usage relations if the target is undocumented or is not a class. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("HAVE_DOT",
      "If this tag is set DoxyPress will expect the dot program is available from your path. "
      "This program is part of Graphviz which is a graph visualization toolkit from AT&T "
      "and Lucent Bell Labs.   "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("DOT_NUM_THREADS",
      "This tag specifies the number of dot invocations DoxyPress is allowed to run in parallel. "
      "If the value is zero DoxyPress will select a value based on the number of processors available "
      "in the system. "
      "<br><br>"
      "Minimum: 0, Maximum: 32, Default: 0 "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FONT_NAME",
      "This tag specifies a different font for the dot program. The path can be set using the 'DOT FONT PATH' tag. "
      "<br><br>"
      "The default value is: Helvetica "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FONT_SIZE",
      "This tag can be used to set the size (in points) of the font of dot graphs. "
      "<br><br>"
      "Minimum: 4, Maximum: 24, Default: 10 "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FONT_PATH",
      "The dot program will use the default font as specified by 'DOT FONT NAME'. "
      "If a font name is specified then this tag will be used as the path to locate the font file. "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CLASS_GRAPH",
      "If this tag is set DoxyPress will generate a graph for each documented class showing the "
      "direct and indirect inheritance relations. "
      "Setting this tag to YES will force the 'CLASS DIAGRAMS' tag to NO. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_COLLABORATION",
      "If this tag is set DoxyPress will generate a graph for each documented class showing the "
      "direct and indirect implementation dependencies (inheritance, containment, and class references variables) "
      "of the class with other documented classes. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("GROUP_GRAPHS",
      "If this tag is set DoxyPress will generate a graph for groups, showing the direct groups dependencies. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("UML_LOOK",
      "If this tag is set DoxyPress will generate inheritance and "
      "collaboration diagrams in a style similar to the OMG's Unified Modeling Language. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("UML_LIMIT_NUM_FIELDS",
      "If the 'UML LOOK' tag is enabled, the fields and methods are shown inside the class node. "
      "If there are many fields or methods and many nodes the graph may become too big to be useful. "
      "This tag limits the number of items for each type to make the size more manageable. "
      "Set this tag to 0 for no limit. "
      "<br><br>"
      "Note: The threshold may be exceeded by 50% before the limit is enforced. When the threshold is 10, "
      "up to 15 fields may appear, but if the number exceeds 15, the total amount of fields shown is limited to 10. "
      "<br><br>"
      "Minimum: 0, Maximum: 100, Default: 10 "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("TEMPLATE_RELATIONS",
      "If this tag is set the inheritance and collaboration graphs will show the relations "
      "between templates and their instances. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_INCLUDE",
      "If this tag plus 'ENABLE PREPROCESSING' and 'SEARCH INCLUDES' are set to "
      "YES, DoxyPress will generate a graph for each documented file showing the "
      "direct and indirect include dependencies of the file with other documented files. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_INCLUDED_BY",
      "If this tag plus 'ENABLE PREPROCESSING' and 'SEARCH INCLUDES' are set  to "
      "YES, DoxyPress will generate a graph for each documented file showing "
      "the direct and indirect include dependencies of the file with other documented files. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CALL",
      "If this tag is set a call dependency graph for every global function or class method "
      "will be generated. "
      "Call graphs can be enabled for selected functions by using the \\callgraph command. "
      "If this tag is enabled use the \\hidecallgraph command to prevent a call graph from being generated. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CALLED_BY",
      "If this tag is set a caller dependency graph will be generated for every global function or class method. "
      "Caller graphs can be enabled for selected functions by using the \\callergraph command. "
      "If this tag is enabled use the \\hidecallergraph command to prevent a caller graph from being generated. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_HIERARCHY",
      "If this tag is set a graphical hierarchy of all classes will be shown instead of a textual one. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DIRECTORY_GRAPH",
      "This tag is set the dependencies a directory has on other directories, will be shown. "
      "The dependency relations are determined by the #include relations between the "
      "files in the directories. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_IMAGE_FORMAT",
      "This tag is used to specify the image format generated by the dot program. "
      "If svg is selected 'HTML FILE EXTENSION' should be set to 'xhtml' in order "
      "to ensure the images files are visible in all browsers. "
      "<br><br>"
      "The default format value is png."
      "<br><br>"
      "Requires he tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("INTERACTIVE_SVG",
      "If 'DOT IMAGE FORMAT' is set to svg, this option can be set to YES to "
      "enable the generation of interactive svg images to allow zooming and panning. "
      "If svg is selected 'HTML FILE EXTENSION' should be set to 'xhtml' in order "
      "to ensure the images files are visible in all browsers. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_PATH",
      "This tag is used to specify the path where the dot program is located. "
      "If left blank DoxyPress will search the users path. "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FILE_DIRS",
      "This tag is used to specify one or more directories containing "
      "dot files which are included in the documentation. "
      "Refer to the DoxyPress manual regarding the \\dotfile command."
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("MSC_FILE_DIRS",
      "This tag is used to specify one or more directories containing "
      "msc files included in the documentation. "
      "Refer to the DoxyPress manual regarding the \\mscfile command.");

   s_fullHelp.insert("DIA_FILE_DIRS",
      "This tag is used to specify one or more directories containing "
      "dia files which are included in the documentation. "
      "Refer to the DoxyPress manual regarding the \\diafile command.");

   s_fullHelp.insert("PLANTUML_JAR_PATH",
      "This tag specifies the path where java can find the plantuml.jar file. If left blank "
      "it is assumed PlantUML is not used or called during a preprocessing step. DoxyPress will "
      "generate a warning when it encounters a \\startuml command in this case and "
      "will not generate output for the diagram.");

   s_fullHelp.insert("PLANTUML_CFG_FILE",
      "When using PlantUML this tag can be used to specify a configuration file.");

   s_fullHelp.insert("PLANTUML_INC_PATH",
      "When using PlantUML the specified paths are searched for files specified by "
      "the <code>!include</code> statement in a plantuml block.");

   s_fullHelp.insert("DOT_GRAPH_MAX_NODES",
      "This tag sets the maximum number of nodes shown in the graph. If the number of nodes is larger "
      "than this tag value the graph will be truncated. If the number of direct children of the root node "
      "is already larger than this tag value, the graph will not be shown at all. "
      "The size of the graph can be further restricted by 'DOT MAX GRAPH DEPTH'."
      "<br><br>"
      "Minimum: 0, Maximum: 10000, Default: 50 "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_GRAPH_MAX_DEPTH",
      "This tag is used to set the maximum depth of the graphs generated by the dot program. A depth value "
      "of 3 means only nodes reachable from the root in no more than 3 steps will be shown. "
      "Nodes which are further from the root node will be omitted. Setting this option to 1 "
      "or 2 may reduce the computation time needed for large code bases. "
      "The size of a graph can be further restricted by 'DOT GRAPH MAX NODES'. "
      "Using a depth of 0 means no depth restriction. "
      "<br><br>"
      "Minimum: 0, Maximum: 1000, Default: 0 "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_TRANSPARENT",
      "Set this tag to generate images with a transparent background. "
      "Depending on your platform, enabling this option may lead to "
      "unreadable anti-aliased labels on the edges of a graph. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_MULTIPLE_TARGETS",
      "Set this tag to allow the dot program to generate multiple output files in one run. Setting this tag to YES "
      "makes dot run faster. Only versions of dot greater than 1.8.10 support this feature. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("GEN_LEGEND",
      "If this tag is set DoxyPress will generate a legend page "
      "explaining the meaning of the various boxes and arrows in the dot generated graphs. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CLEANUP",
      "If this tag is set DoxyPress will remove the intermediate dot files which are used to generate "
      "the various graphs."
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'HAVE DOT' is set to YES");

   // html
   s_fullHelp.insert("GEN_HTML",
      "If this tag is set HTML output will be generated."
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("HTML_OUTPUT",
      "This tag is used to specify the location for the HTML output. If a "
      "relative path is entered the value of 'OUTPUT DIRECTORY' will be prepended. "
      "<br><br>"
      "The default directory is: html "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_FILE_EXTENSION",
      "This tag is used to specify the file extension for each generated HTML page. For example: .html, .php, .asp. "
      "<br><br>"
      "The default value is: .html "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_HEADER",
      "This tag is used to specify a user-defined HTML header file for each generated HTML page. "
      "If left blank DoxyPress will generate a standard header. "
      "<br><br>"
      "To create a new header file run 'DoxyPress --w html-head [header file name]' "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_FOOTER",
      "This tag is used to specify an user defined HTML footer for each generated HTML page. "
      "If left blank DoxyPress will use a standard footer. "
      "<br><br>"
      "To create a new footer file run 'DoxyPress --w html-foot [footer file name]' "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_STYLESHEETS",
      "This tag is used to specify additional custom css files which will be included "
      "after the standard 'doxy_style.css' file. DoxyPress will automatically copy  "
      "any css files listed in this tag to the 'HTML OUTPUT DIRECTORY'. "
      "<br><br>"
      "The order of css files is important. The last stylesheet will override the "
      "settings of any previous stylesheets. "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_EXTRA_FILES",
      "This tag is used to specify one or more extra images or source files which should be copied to "
      "the 'HTML OUTPUT DIRECTORY'. To acccess the extra files in the 'HTML HEADER FILE' or "
      "'HTML FOOTER FILE', prepend $relpath^ to the file name. In a CSS stylesheet specify only the file name. "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_COLORSTYLE_HUE",
      "This tag controls the color of the HTML output. DoxyPress will adjust the colors in "
      "the stylesheet and background images according to this color. "
      "<br><br>"
      "Minimum: 0, Maximum: 359, Default: 220 "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_COLORSTYLE_SAT",
      "This tag controls the purity (or saturation) of the colors in the HTML output. "
      "<br><br>"
      "Minimum: 0, Maximum: 255, Default: 100 "
      "<br><br>"
      "TRequires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_COLORSTYLE_GAMMA",
      "This controls the gamma correction applied to the luminance component of the colors "
      "in the HTML output. "
      "<br><br>"
      "Minimum: 40, Maximum: 240, Default: 80 "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_TIMESTAMP",
      "If this tag is set to YES, the footer of each generated HTML "
      "page will contain the date and time when the page was generated. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_DYNAMIC_SECTIONS",
      "If this tag is set then the generated HTML documentation will contain sections which "
      "can be hidden and shown after the page has loaded. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_INDEX_NUM_ENTRIES",
      "This tag defines the preferred number of entries shown in the various tree "
      "structured indices. DoxyPress will expand the tree until the specified "
      "number of entries are visible. Setting this value to 1 will "
      "produce a fully collapsed tree. A value of 0 will result in a fully expanded tree. "
      "<br><br>"
      "Minimum: 0, Maximum: 9999, Default: 100 "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   // chm
   s_fullHelp.insert("GEN_CHM",
      "If this tag is set three additional HTML index files will be generated: "
      "index.hhp, index.hhc, and index.hhk. The index.hhp is a project file which can be read "
      "by Microsoft's HTML Help Workshop on Windows. "
      "<br><br>"
      "The HTML Help Workshop contains a compiler to convert HTML output "
      "into a single compiled HTML .chm file. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("CHM_FILE",
      "This tag is used to specify the file name of the resulting .chm file. "
      "If a path is specified the file will be written to this path instead of the html output directory. "
      "<br><br>"
      "Requires the tag 'GENERATE HTML HELP' is set to YES");

   s_fullHelp.insert("HHC_LOCATION",
      "This tag is used to specify the location (absolute path including file name) of the HTML help "
      "compiler (hhc.exe). If non-empty, DoxyPress will try to run the HTML help compiler on the "
      "generated index.hhp. The file has to be specified with full path. "
      "<br><br>"
      "Requires the tag 'GENERATE HTML HELP' is set to YES");

   s_fullHelp.insert("GEN_CHI",
      "This tag controls if a separate .chi index file is generated "
      "(YES) or if the index should be included in the master .chm file (NO). "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML HELP' is set to YES");

   s_fullHelp.insert("CHM_INDEX_ENCODING",
      "This tag is used to encode HtmlHelp index (hhk), content (hhc) "
      "and project file content. "
      "<br><br>"
      "Requires the tag 'GENERATE HTML HELP' is set to YES");

   s_fullHelp.insert("BINARY_TOC",
      "This tag controls whether the chm will contain a binary table of contents "
      "(YES) or a normal table of contents (NO). It also enables the Previous and Next buttons. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML HELP' is set to YES");

   s_fullHelp.insert("TOC_EXPANDED",
      "If this tag is set then group members will be added to  "
      "the table of contents of the HTML help documentation and to the tree view. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML HELP' is set to YES");

   // docbook
   s_fullHelp.insert("GEN_DOCBOOK",
      "If this tag is set Docbook files will be generated. These files can be used to generate a PDF. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("DOCBOOK_OUTPUT",
      "This tag is used to specify the location for the DocBook output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will be prepended. "
      "<br><br>"
      "The default directory is: docbook "
      "<br><br>"
      "Requires the tag 'GENERATE DOCBOOK' is set to YES");

   s_fullHelp.insert("DOCBOOK_PROGRAM_LISTING",
      "If this tag is set DoxyPress will include the "
      "program listings, including syntax highlighting and cross-referencing "
      "information, to the Docbook output. "
      "<br><br>"
      "Enabling this tag will significantly increase the size of the documentation. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE DOCBOOK' is set to YES");

   // docset
   s_fullHelp.insert("GEN_DOCSET",
      "If this tag is set, an additional index files will be "
      "generated which can be used as input for Apple's Xcode IDE. DoxyPress will generate a "
      "Makefile in the HTML output directory. Running make will produce the docset "
      "and running make install will install the docset. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("DOCSET_FEEDNAME",
      "This tag determines the name of the docset feed. A documentation feed provides "
      "a name under which multiple documentation sets from a single provider "
      "(such as a company or product suite) can be grouped. "
      "<br><br>"
      "The default value is: DoxyPress generated docs "
      "<br><br>"
      "Requires the tag 'GENERATE DOCSET' is set to YES");

   s_fullHelp.insert("DOCSET_BUNDLE_ID",
      "This tag specifies a string which should uniquely identify the documentation set bundle. "
      "The ID is a reversed domain name like com.mycompany.MyDocSet. DoxyPress will append .docset to the ID. "
      "<br><br>"
      "The default value is: org.doxypress.Project "
      "<br><br>"
      "Requires the tag 'GENERATE DOCSET' is set to YES");

   s_fullHelp.insert("DOCSET_PUBLISHER_ID",
      "This tag is used to specify a string which should uniquely identify the documentation publisher. "
      "The ID is a reversed domain name like com.mycompany.MyDocSet.documentation. "
      "<br><br>"
      "The default value is: org.doxypress.Publisher "
      "<br><br>"
      "Requires the tag 'GENERATE DOCSET' is set to YES");

   s_fullHelp.insert("DOCSET_PUBLISHER_NAME",
      "This tag identifies the documentation publisher. "
      "<br><br>"
      "The default value is: Publisher. "
      "<br><br>"
      "Requires the tag 'GENERATE DOCSET' is set to YES");

   // eclipse
   s_fullHelp.insert("GEN_ECLIPSE",
      "If this tag is set additional index files will be generated with the "
      "HTML files to create an Eclipse help plugin. "
      "To install the plugin the contents of the directory containing the HTML and XML files must "
      "be copied into the plugins directory of eclipse. The name of the directory "
      "within the plugins directory should be the same as the 'ECLIPSE DOC ID'. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("ECLIPSE_DOC_ID",
      "A unique identifier for the Eclipse help plugin. When installing the plugin "
      "the directory name containing the HTML and XML files should also have this "
      "name. Each documentation set should have its own identifier. "
      "<br><br>"
      "The default value is: org.doxypress.Project "
      "<br><br>"
      "Requires the tag 'GENERATE ECLIPSE HELP' is set to YES");

   s_fullHelp.insert("DISABLE_INDEX",
      "To obtain full control over the layout of generated HTML it can be "
      "necessary to disable the index and replace it with a custom index. "
      "This tag is used to turn enable (NO) or disable (YES) the index tabs at the top "
      "of each HTML page. Since the tabs in the index contain the same information as the navigation "
      "tree, you can set this option to YES if you also set 'GENERATE NAVIGATION TREEVIEW' to YES. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("GEN_TREEVIEW",
      "This tag is used to specify whether a tree index will be displayed in the HTML output. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("TREEVIEW_WIDTH",
      "If 'GENERATE NAVIGATION TREEVIEW' is enabled this tag can be used "
      "to set the initial width (in pixels) of the frame in which the tree is shown. "
      "<br><br>"
      "Minimum: 0, Maximum: 1500, Default: 250 "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("ENUM_VALUES_PER_LINE",
      "This tag is used to set the number of enum values  "
      "DoxyPress will group on one line in the generated HTML documentation. "
      "<br><br>"
      "A value of 0 will completely suppress the enum values from appearing "
      "in the overview section. "
      "<br><br>"
      "Minimum: 0, Maximum: 20, Default: 4 "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("EXTERNAL_LINKS_IN_WINDOW",
      "If this tag is set DoxyPress will open links to "
      "external symbols imported via tag files in a separate window. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_SEARCH",
      "When this tag is set DoxyPress will generate a search box for the HTML output. "
      "The search engine uses JavaScript and DHTML. "
      "If this search engine is too slow try using the 'SEARCH SERVER BASED'. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("SEARCH_SERVER_BASED",
      "When this tag is set the search engine will be implemented using a web server instead "
      "of a web client using JavaScript. There are two flavors of web server based searching "
      "depending on the 'EXTERNAL SEARCH' setting. When disabled, DoxyPress will generate a PHP "
      "script for searching and an index file used by the script. When 'EXTERNAL SEARCH' is enabled the indexing "
      "and searching needs to be provided by external tools. "
      "<br><br>"
      "Refer to the DoxyPress manual regarding 'External Searching'. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HTML SEARCH' is set to YES");

   s_fullHelp.insert("SEARCH_EXTERNAL",
      "When this tag is enabled DoxyPress will not generate the PHP script for searching. "
      "The search results will be written to an XML file which must be processed by an external "
      "indexing system. DoxyPress will call the external search engine referenced in the "
      "tag 'SEARCH ENGINE URL'. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'HTML SEARCH' is set to YES");

   s_fullHelp.insert("SEARCH_ENGINE_URL",
      "This tag references a search engine hosted by a web server "
      "which will return the search results when 'EXTERNAL SEARCH' is enabled. "
      "<br><br>"
      "Refer to the DoxyPress manual regarding 'External Searching'. "
      "<br><br>"
      "Requires the tag 'HTML SEARCH' is set to YES");

   s_fullHelp.insert("SEARCH_DATA_FILE",
      "When 'SERVER BASED SEARCH' and 'EXTERNAL SEARCH' are both enabled the unindexed "
      "search data is written to a file for indexing by an external program. This tag "
      "is used to specify the name of the file. "
      "<br><br>"
      "The default file is: searchdata.xml "
      "<br><br>"
      "Requires the tag 'HTML SEARCH' is set to YES");

   s_fullHelp.insert("SEARCH_EXTERNAL_ID",
      "When the tags 'SERVER BASED SEARCH' and 'EXTERNAL SEARCH' are both enabled, this tag is used as "
      "an identifier. This is useful in combination with 'SEARCH MAPPINGS' to search through "
      "multiple projects and redirect the results to the correct project. "
      "<br><br>"
      "Requires the tag 'SEARCH ENGINE is set to YES");

   s_fullHelp.insert("SEARCH_MAPPINGS",
      "This tag is used to enable searching in multiple DoxyPress projects. "
      "Each project needs to have a unique id set via the 'EXTERNAL SEARCH ID' tag. "
      "This tag is used to map the id to a relative location where the documentation can be found. "
      "The format is: tagname1=loc1 tagname2=loc2 ... "
      "<br><br>"
      "Requires the tag 'HTML SEARCH' is set to YES");

   s_fullHelp.insert("FORMULA_FORMAT",
      "When this tag is set to svg DoxyPress will use the pdf2svg program or inkscape to render formulas "
      "as svg images."
      "<br><br>"
      "Default: png"
      "<br><br>"
       "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("FORMULA_FONTSIZE",
      "Use this tag to change the font size of LaTeX formulas included as images in "
      "the HTML documentation. If the font size is changed, ensure there are no "
      "existing form_*.png images in the HTML output directory. This will force DoxyPress "
      "to regenerate the images the next time it is run."
      "<br><br>"
      "Minimum: 8, Maximum: 50, Default: 10 "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("FORMULA_TRANSPARENT",
      "If this tag is set transparent PNGs will be used in formulas. Transparent PNGs may not "
      "be supported properly in older browsers. "
      "<br><br>"
      "If this tag is modified make sure to delete all files named form_*.png in the HTML output directory. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("FORMULA_MACROFILE",
      "This tag specifies the name of a file which contains Latex commands used in formulas."
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("GHOSTSCRIPT",
      "This tag specifies the full path and file name for the GhostScript program. "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("USE_MATHJAX",
      "Set this tag to render LaTeX formulas using MathJax. Client side JavaScript will be used for rendering "
      "instead of pre-rendered bitmaps. This tag should be used if you do not have LaTeX "
      "installed or if you want formulas to look better in the HTML output. When "
      "enabled, you may also need to install MathJax and configure the path "
      "to it using the 'MATHJAX RELATIVE PATH' option. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("MATHJAX_FORMAT",
      "This tag is used to set the default output format to be used for the MathJax output. "
      "Possible values are: HTML-CSS (which is slower, but has the best "
      "compatibility), NativeMML (i.e. MathML) and SVG. "
      "<br><br>"
      "The default value is: HTML-CSS "
      "<br><br>"
      "Requires the tag 'USE MATHJAX' is set to YES");

   s_fullHelp.insert("MATHJAX_RELPATH",
      "When MathJax is enabled specify the location relative to the HTML "
      "output directory using the 'MATHJAX RELATIVE PATH' tag. The destination directory "
      "should contain the MathJax.js script. For example, if the mathjax directory "
      "is located at the same level as the HTML output directory, then this tag should be ../mathjax. "
      "The default value points to the MathJax Content Delivery Network so you can quickly see "
      "the result without installing MathJax. "
      "<br><br>"
      "The default value is: https://https://cdn.jsdelivr.net/npm/mathjax@2"
      "<br><br>"
      "Requires the tag 'USE MATHJAX' is set to YES");

   s_fullHelp.insert("MATHJAX_EXTENSIONS",
      "This tag is used to specify one or more MathJax extension names which will be enabled "
      "during MathJax rendering. "
      "<br><br>"
      "For example: TeX/AMSmath, TeX/AMSsymbols "
      "<br><br>"
      "Requires the tag 'USE MATHJAX' is set to YES");

   s_fullHelp.insert("MATHJAX_CODEFILE",
      "This tag is used to specify a file with JavaScript fragments which will be used during "
      "startup of the MathJax code. "
      "<br><br>"
      "Requires the tag 'USE MATHJAX' is set to YES");

   // latex
   s_fullHelp.insert("GEN_LATEX",
      "If this tag is set DoxyPress will generate LaTeX output. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("LATEX_OUTPUT",
      "This tag is used to specify the location for the LaTeX output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will be prepended. "
      "<br><br>"
      "The default directory is: latex "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_CMD_NAME",
      "This tag is used to specify the LaTeX command name to be invoked. "
      "<br><br>"
      "When 'USE PDF LATEX' is set, this option is only used for generating "
      "bitmaps for formulas in the HTML output. "
      "<br><br>"
      "The default file is: latex "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("MAKE_INDEX_CMD_NAME",
      "This tag is used to specify the command name to generate an index for LaTeX. "
      "<br><br>"
      "The default file is: makeindex "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_COMPACT",
      "If this tag is set DoxyPress generates compact LaTeX documents. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_PAPER_TYPE",
      "This tag is used to set the paper type used by the printer. "
      "Possible values are: a4 (210 x 297 mm), letter (8.5 x 11 inches), legal "
      "(8.5 x 14 inches) and executive (7.25 x 10.5 inches). "
      "<br><br>"
      "The default value is: a4 "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_EXTRA_PACKAGES",
      "This tag is used to specify one or more LaTeX packages to include in the LaTeX output. Each entry "
      "can be a package name or the literal argument passed to the \\usepackage command. "
      "<br><br>"
      "As an example, to use the times font in your LaTeX output specify either 'times' or '{times}' for this tag. "
      "<br><br>"
      "If this tag is blank no extra packages will be included. "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_HEADER",
      "This tag is used to specify a user-defined LaTeX header file for the generated LaTeX output. "
      "<br><br>"
      "The header should contain everything until the first chapter. "
      "The following commands have a special meaning: $title, $datetime, $date, $DoxyPressversion, $projectname, "
      "$projectnumber, $projectbrief, $projectlogo. DoxyPress will replace $title with the empty "
      "string for the replacement values of the other commands the user is referred to 'HTML HEADER FILE'. "
      "<br><br>"
      "If this tag is blank DoxyPress will generate a standard header. "
      "<br><br>"
      "To create a new header file run 'DoxyPress --w latex-head [header file name]' "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_FOOTER",
      "This tag is used to specify a user-defined LaTeX footer for the generated LaTeX document."
      "If left blank DoxyPress will generate a standard footer. The footer should contain everything after the last chapter. "
      "<br><br>"
      "To create a new footer file run 'DoxyPress --w latex-foot [footer file name]' "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_STYLESHEETS",
      "This tag is used to specify additional custom style sheets which will be included "
      "after the standard stylesheets. DoxyPress will automatically copy  "
      "any files listed in this tag to the 'LATEX OUTPUT DIRECTORY'. "
      "<br><br>"
      "The order of stylesheets is important. The last stylesheet will override the "
      "settings of any previous stylesheets. "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_EXTRA_FILES",
      "This tag is used to specify one or more extra images or "
      "other source files which should be copied to the 'LATEX OUTPUT DIRECTORY' "
      "directory. The files will be copied verbatim. "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_TIMESTAMP",
      "If this tag is set the footer page will contain the date and time when the page was generated. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_HYPER_PDF",
      "If this tag is set the generated LaTeX is "
      "prepared for conversion to PDF using ps2pdf or pdflatex. The PDF file will "
      "contain links instead of page references. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_PDF",
      "If this tag is set DoxyPress will use the pdflatex program to generate the PDF file "
      "directly from the LaTeX files. Set this option to obtain higher quality PDF documentation. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_BATCH_MODE",
      "If this tag is set DoxyPress will add the \\batchmode command to the generated LaTeX files. "
      "This will instruct LaTeX to keep running if errors occur. "
      "This option is also used when generating formulas in HTML. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_HIDE_INDICES",
      "If this tag is set DoxyPress will not include the "
      "index chapters such as File Index, Compound Index, etc. in the output. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_SOURCE_CODE",
      "If this tag is set DoxyPress will include source code with syntax highlighting in the LaTeX output. "
      "Which sources are shown depend on other settings such as 'SOURCE CODE'"
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");


   // latex bio
   s_fullHelp.insert("LATEX_BIB_STYLE",
      "This tag is used to specify the style to use for the bibliography, plainnat, or IEEEtranN, etc. "
      "Refer to the DoxyPress manual regarding the \\cite command."
      "<br><br>"
      "The default value is: plain "
      "<br><br>"
      "Requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("CITE_BIB_FILES",
      "This tag is used to specify one or more bib files containing the reference definitions. "
      "TO use this feature both perl and bibtex must be available in the search path. "
      "For LaTeX the style of the bibliography can be controlled using 'LATEX BIB STYLE'"
      "Refer to the DoxyPress manual regarding the \\cite command for additional information.");

   // man
   s_fullHelp.insert("GEN_MAN",
      "If this tag is set man pages for classes and files will be generated."
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("MAN_OUTPUT",
      "This tag is used to specify the location for the man page output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will  be prepended. "
      "A directory named man3 will be created inside the directory specified by this tag. "
      "<br><br>"
      "The default directory is: man "
      "<br><br>"
      "Requires the tag 'GENERATE MAN' is set to YES");

   s_fullHelp.insert("MAN_EXTENSION",
      "This tag determines the extension which is added to the generated man pages. "
      "In case the manual section does not start with a number, the number "
      "3 is prepended. The dot (.) at the beginning of this tag is optional. "
      "<br><br>"
      "The default value is: .3 "
      "<br><br>"
      "Requires the tag 'GENERATE MAN' is set to YES");

   s_fullHelp.insert("MAN_SUBDIR",
      "This tag determines the name of the directory created under the value for "
      "the 'MAN PAGE OUTPUT DIRECTORY' tag. The default directory will be man3. "
      "<br><br>"
      "Requires the tag 'GENERATE MAN' is set to YES");

   s_fullHelp.insert("MAN_LINKS",
      "If this tag is set then for each entity documented in the man pages, one additional file "
      "will be generated linking the name of the entity with the correct man page. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE MAN' is set to YES");

   // tab 3 perl module
   s_fullHelp.insert("GEN_PERL",
      "If this tag is set DoxyPress will generate a Perl module "
      "file which contains the structure of the code including all documentation. "
      "This feature is currently experimental. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("PERL_LATEX",
      "If this tag is set DoxyPress will generate the necessary "
      "Makefile rules, Perl scripts, and LaTeX code to be able to generate PDF and DVI "
      "output from the Perl module output. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE PERL MODULE' is set to YES");

   s_fullHelp.insert("PERL_PRETTY",
      "If this tag is set the Perl module output will be "
      "formatted so it is readable. If this tag is set to NO, the "
      "size of the Perl module output will be much smaller. Perl can parse either format. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'GENERATE PERL MODULE' is set to YES");

   s_fullHelp.insert("PERL_PREFIX",
      "The names of the make variables in the generated doxyrules.make file are "
      "prefixed with the string contained in 'PERL MODULE MAKEVAR PREFIX'. This is useful "
      "so different doxyrules.make files included by the same Makefile do not overwrite each other's variables. "
      "<br><br>"
      "Requires the tag 'GENERATE PERL MODULE' is set to YES");

   // QtHelp
   s_fullHelp.insert("GEN_QTHELP",
      "If this tag is set and both 'QHP NAMESPACE' and "
      "'QHP VIRTUAL FOLDER' are set, an additional index file will be generated. This file "
      "can be used as input for qhelpgenerator to create a Compressed Help file "
      "(.qch) from the generated HTML documentation. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("QCH_FILE",
      "This tag is used to specify the .qch output file name of the QtHelp Generator. "
      "<br><br>"
      "Requires the tag 'GENERATE QT HELP' is set to YES");

   s_fullHelp.insert("QHP_NAMESPACE",
      "This tag specifies the namespace to use when generating QtHelp output. "
      "<br><br>"
      "The default value is: org.doxypress.Project "
      "<br><br>"
      "Requires the tag 'GENERATE QT HELP' is set to YES");

   s_fullHelp.insert("QHP_VIRTUAL_FOLDER",
      "This tag specifies the virtual folder to use when generating QtHelp output. "
      "<br><br>"
      "The default value is: doc "
      "<br><br>"
      "Requires the tag 'GENERATE QT HELP' is set to YES");

   s_fullHelp.insert("QHP_CUST_FILTER_NAME",
      "This tag specifies the name of a custom filter for the QtHelp output. "
      "<br><br>"
      "Requires the tag 'GENERATE QT HELP' is set to YES");

   s_fullHelp.insert("QHP_CUST_ATTRIB",
      "This tag specifies the list of attributes for the custom filter used by QtHelp. "
      "<br><br>"
      "Requires the tag 'GENERATE QT HELP' is set to YES");

   s_fullHelp.insert("QHP_SECT_ATTRIB",
      "This tag specifies the list of filter section attributes used by QtHelp. "
      "<br><br>"
      "Requires the tag 'GENERATE QT HELP' is set to YES");

   s_fullHelp.insert("QTHELP_GEN_PATH",
      "This tag is used to specify the path for the QtHelp Generator. If this tag is empty "
      "DoxyPress will search for the qhelpgenerator program in your path. "
      "<br><br>"
      "Requires the tag 'GENERATE QT HELP' is set to YES");

   // rtf
   s_fullHelp.insert("GEN_RTF",
      "If this tag is set RTF output will be generated."
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("RTF_OUTPUT",
      "This tag is used to specify the location for the RTF output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will be prepended. "
      "<br><br>"
      "The default directory is: rtf "
      "<br><br>"
      "Requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_STYLESHEET",
      "Load stylesheet definitions from file. "
      "<br><br>"
      "Refer to the DoxyPress manual regarding \"DoxyPress Usage\" for information on how to generate "
      "the default stylesheet. "
      "<br><br>"
      "Requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_EXTENSION",
      "This tag is used to set the path to an RTF Extension file. This file contains optional variables used "
      "in the generation of an RTF document."
      "To create a sample extension file run 'DoxyPress --w rtf-ext [extensions file name]' "
      "<br><br>"
      "Requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_PAPER_TYPE",
      "This tag is used to set the paper type used by the printer. "
      "Possible values are: a4 (210 x 297 mm), letter (8.5 x 11 inches), legal "
      "(8.5 x 14 inches) and executive (7.25 x 10.5 inches). "
      "<br><br>"
      "The default value is: a4 "
      "<br><br>"
      "Requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_SOURCE_CODE",
      "If this tag is set DoxyPress will include source code with syntax highlighting in the RTF output. "
      "Which source files are shown depend on other settings such as the 'SOURCE CODE' tag."
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_COMPACT",
      "If this tag is set DoxyPress generates compact RTF documents. "
      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_HYPERLINKS",
      "If this tag is set the RTF generated will contain hyperlinks. "
      "The output may be more suitable for browsing rather than printing. "

      "<br><br>"
      "The default value is: NO "
      "<br><br>"
      "Requires the tag 'GENERATE RTF' is set to YES");

   // xml
   s_fullHelp.insert("GEN_XML",
      "If this tag is set XML documentation will be generated. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("XML_OUTPUT",
      "This tag is used to specify the location for the XML output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will  be prepended. "
      "<br><br>"
      "The default directory is: xml "
      "<br><br>"
      "Requires the tag 'GENERATE XML' is set to YES");

   s_fullHelp.insert("XML_PROGRAM_LISTING",
      "If this tag is set program source code, including syntax highlighting and cross-referencing, will be generated. "
      "<br><br>"
      "The default value is: YES "
      "<br><br>"
      "Requires the tag 'GENERATE XML' is set to YES");

   s_fullHelp.insert("XML_INCLUDE_NS_MEMBERS",
      "If this tag is set then the output will include namespace members in the file scope.");
}
