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

static void configFullHelp(const QString &m_filePatterns);
static QString find_HelpBody(QString name);

static QMap<QString, QString> s_fullHelp;

void MainWindow::createMap()
{
   // 1  load help data
   configFullHelp(m_filePatterns);

   // 2  load bigMap
   HelpData temp;

   QList<QCheckBox *> temp_CB = this->findChildren<QCheckBox *>();

   for (auto ptr : temp_CB)  {
      temp = HelpData{nullptr, "", "", find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QGroupBox *> temp_GB = this->findChildren<QGroupBox *>();

   for (auto ptr : temp_GB)  {
      temp = HelpData{nullptr, "", "", find_HelpBody(ptr->objectName()) };
      m_bigMap.insert(ptr, temp);
   }

   QList<QComboBox *> temp_CM = this->findChildren<QComboBox *>();

   for (auto ptr : temp_CM)  {
      temp = HelpData{nullptr, "", "", find_HelpBody(ptr->objectName()) };
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->installEventFilter(this);
   }

   QList<QLineEdit *> temp_LE = this->findChildren<QLineEdit *>();

   for (auto ptr : temp_LE)  {
      temp = HelpData{nullptr, "", "", find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QPlainTextEdit *> temp_PT = this->findChildren<QPlainTextEdit *>();

   for (auto ptr : temp_PT)  {
      temp = HelpData{nullptr, "", "", find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QRadioButton *> temp_RB = this->findChildren<QRadioButton *>();

   for (auto ptr : temp_RB)  {
      temp = HelpData{nullptr, "", "", find_HelpBody(ptr->objectName())};
      m_bigMap.insert(ptr, temp);

      // event setup
      ptr->setAttribute(Qt::WA_Hover);
      ptr->installEventFilter(this);
   }

   QList<QSpinBox *> temp_SB = this->findChildren<QSpinBox *>();

   for (auto ptr : temp_SB)  {
      temp = HelpData{nullptr, "", "", find_HelpBody(ptr->objectName())};
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
            index->title = ptr->text().left( ptr->text().length() - 1);
         }
      }
   }

   //
   setupLimits();
   clearAllFields();
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

QLabel *MainWindow::getLabel(QWidget *name)
{
   QLabel *retval = nullptr;

   auto index = m_bigMap.find(name);

   if (index != m_bigMap.end()) {
      retval = index->label;
   }

   return retval;
}

void MainWindow::setLabelColor(int option, QWidget *label)
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

void MainWindow::hoverChanged(QWidget *old_Widget, QWidget *new_Widget)
{
   // adjust help message for new_Widget
   setHelpText(new_Widget);


   // adjust color on old_Widget if updated
   QString defValue = getDefault(old_Widget);

   QCheckBox *temp_CB = dynamic_cast<QCheckBox *>(old_Widget);

   if (temp_CB) {
      QLabel *label = getLabel(temp_CB);

      if (label) {
         // broom - incorrect -->  need to add a way to test for the defValue

         if (! temp_CB->isChecked()) {
            setLabelColor(DEFAULT, label);

         } else {
            setLabelColor(MODIFIED, label);

         }
      }
   }

   // QGroupBox - do nothing


   // QCombo  - do nothing


   QLineEdit *temp_LE = dynamic_cast<QLineEdit *>(old_Widget);

   if (temp_LE) {
      QLabel *label = getLabel(temp_LE);

      if (label) {

         if (temp_LE->text().trimmed() == defValue ) {
            setLabelColor(DEFAULT, label);

         } else {
            setLabelColor(MODIFIED, label);

         }
      }
   }

   QPlainTextEdit *temp_PT = dynamic_cast<QPlainTextEdit *>(old_Widget);

   if (temp_PT) {
      QLabel *label = getLabel(temp_PT);

      if (label) {

         if (temp_PT->toPlainText().trimmed() == defValue) {
            setLabelColor(DEFAULT, label);

         } else {
            setLabelColor(MODIFIED, label);

         }
      }
   }

   // QRadioButton - do nothing


   QSpinBox *temp_SB = dynamic_cast<QSpinBox *>(old_Widget);

   if (temp_SB) {
      QLabel *label = getLabel(temp_SB);

      if (label) {
         QString defValue_SB = getDefault(old_Widget);

/*  BROOM

         if (temp_SB->value == defValue_SB) {
            setLabelColor(DEFAULT, label);

         } else {
            setLabelColor(MODIFIED, label);

         }
*/

      }
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
      retval = "missing help, how?";
   }

   return retval;
}

static QString find_HelpBody(QString name)
{
   QString retval;

   if (name.endsWith("_CB") || name.endsWith("_SB") || name.endsWith("_RB") || name.endsWith("_CM"))  {
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

static void configFullHelp(const QString &m_filePatterns)
{
   s_fullHelp.clear();

   s_fullHelp.insert("PROJECT_NAME",
      "This tag is a single word or a sequence of words surrounded by double quotes "
      "which identifies your project. This name is used in the title of varies generated pages.");

   s_fullHelp.insert("PROJECT_BRIEF",
      "This is an optional tag used as a single line description which will   "
      "appear  at the top of each generate documention page.");

   s_fullHelp.insert("PROJECT_NUMBER",
      "This tag can be used to enter a project number or revision number. This is of "
      "value when archiving generated documentation or if a version control system is used.");

   s_fullHelp.insert("PROJECT_ICON",
      "This tag is used to specify a logo or an icon which is included "
      "in the documentation. The maximum height of the logo should not exceed 55 "
      "pixels and the maximum width should not exceed 200 pixels. DoxyPress will copy "
      "the logo to the specified output directory.");

   s_fullHelp.insert("OUTPUT_DIR",
      "The 'OUTPUT DIRECTORY' tag is used to specify the (relative or absolute) path where the generated "
      "documentation will be written. If a relative path is entered it will be relative to the "
      "location where DoxyPress was started. If left blank the current directory will be used.");

   s_fullHelp.insert("OPTIMIZE_CPLUS",
      "Set this tag to optimzie for C++ "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("OPTIMIZE_JAVA",
      "Set this tag if your project consists of Java or Python sources only. "
      "DoxyPress will then generate output that is more tailored "
      "for that language. For instance, namespaces will be presented as packages, "
      "qualified scopes will look different, etc. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("OPTIMIZE_C",
      "Set this tag if your project consists of C sources "
      "only. DoxyPress will then generate output that is more tailored for C. For "
      "instance, some of the names that are used will be different. The list of all "
      "members will be omitted, etc. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("OPTIMIZE_FORTRAN",
      "Set this tag if your project consists of Fortran "
      "sources. DoxyPress will then generate output that is tailored for Fortran. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HTML_PLAIN",
      "This tag is set ON if neither 'GENERATE CHM' or 'HTML NAVIGATION' are set."
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("HTML_NAV",
      "This tag is set ON if 'HTML NAVIGATION' is set."
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HTML_CHM",
      "If this tag is set then DoxyPress generates three "
      "additional HTML index files: index.hhp, index.hhc, and index.hhk. The "
      "index.hhp is a project file that can be read by Microsoft's HTML Help Workshop "
      "on Windows. "
      "<br><br>"
      "The HTML Help Workshop contains a compiler which can convert HTML output "
      "into a single compiled HTML .chm file. "
      "<br><br>"
      "The default value is: NO"
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   //
   s_fullHelp.insert("DIAGRAM_NONE",
      "If this tag is set then no Diagrams will be generated. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("DIAGRAM_BUILT_IN",
      "If this tag is then then DoxyPress will us the built in class diagram genearator. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("DAIGRAM_DOT",
      ""
      "<br><br>"
      "The default value is: NO");

   //
   s_fullHelp.insert("CREATE_SUBDIRS",
      "If this tag is set then DoxyPress will create 4096 sub-directories (in 2 levels) "
      "under the output directory of each output format and will distribute the generated files "
      "over these directories. Enabling this option can be useful when a large number of source "
      "files are specified. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("ALLOW_UNICODE_NAMES",
      "If this tag is set then DoxyPress will allow non-ASCII characters to appear "
      "in the names of generated files. If set to NO, non-ASCII characters will be "
      "escaped. For example, _xE3_x81_x84 will be used for Unicode U+3044. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("OUTPUT_LANGUAGE",
      "This tag is used to specify the language for the generated documentation. "
      "<br><br>"
      "Possible values are: Afrikaans, Arabic, Armenian, Brazilian, Catalan, Chinese, "
      "Chinese-Traditional, Croatian, Czech, Danish, Dutch, English (United States), "
      "Esperanto, Farsi (Persian), Finnish, French, German, Greek, Hungarian, "
      "Indonesian, Italian, Japanese, Japanese-en (Japanese with English messages), "
      "Korean, Korean-en (Korean with English messages), Latvian, Lithuanian, Macedonian, "
      "Norwegian, Persian (Farsi), Polish, Portuguese, Romanian, Russian, Serbian, Serbian-Cyrillic, "
      "Slovak, Slovene, Spanish, Swedish, Turkish, Ukrainian and Vietnamese. "
      "<br><br>"
      "The default value is: English");

   s_fullHelp.insert("BRIEF_MEMBER_DESC",
      "If this tag is set then DoxyPress will include brief member descriptions after the "
      "members which are listed in the file and class documentation. This is similar to Javadocs "
      "Set this tag to NO to disable. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("REPEAT_BRIEF",
      "If this tag is set then DoxyPress will prepend the brief description of a member or "
      "function before the detailed description. "
      "<br><br>"
      "Note: If both 'HIDE UNDOC MEMBERS' and 'BRIEF MEMBER DESC' are set to NO, the "
      "brief descriptions will be completely suppressed. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("ABBREVIATE_BRIEF",
      "This tag implements a quasi-intelligent brief description abbreviator which is "
      "used to form the text in various listings. Each string in this list, if found "
      "as the leading text of the brief description, will be stripped from the text "
      "and the result, after processing the whole list, is used as the annotated "
      "text. Otherwise, the brief description is used as-is. "
      "<br><br>"
      "The default value ares: The $name class, The $name widget, The $name file, is, provides, "
      "specifies, contains, represents, a, an, the");

   s_fullHelp.insert("ALWAYS_DETAILED_SEC",
      "If this tag and 'REPEAT BRIEF' is set then DoxyPress will generate a "
      "detailed section even if there is only a brief description. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INLINE_INHERITED_MEMBER",
      "If this tag is set then DoxyPress will show all "
      "inherited members of a class in the documentation of that class as if those "
      "members were ordinary class members. Constructors, destructors and assignment "
      "operators of the base classes will not be shown. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("FULL_PATH_NAMES",
      "If this tag is set then DoxyPress will prepend the full path "
      "before files names in the file list and in the header files. If not set then the "
      "shortest path which makes the file name unique will be used. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("STRIP_FROM_PATH",
      "This tag is used to specify what portion of the path to remove from the source files. "
      "The path is only stripped if one of the specified strings matches the beginning  "
      "part of the path. The tag can be used to show relative paths in the file list. "
      "If left blank the directory from which DoxyPress is run is used as the path to strip. "
      "<br><br>"
      "This tag requires the tag 'FULL PATH NAMES' is set to YES");

   s_fullHelp.insert("STRIP_FROM_INC_PATH",
      "This tag is used specify what portion of the path to remove for include files. "
      "The include file shown in the documentation will indicate which header file to include, "
      "in order to use a particular class. If tag is left blank the full path will be removed. ");

   s_fullHelp.insert("SHORT_NAMES",
      "If this tag is set then DoxyPress will generate shorter but may be less readable file names. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("JAVADOC_AUTO_BRIEF",
      "If this tag is set then DoxyPress will interpret the first line "
      "(until the first dot) of a Javadoc-style comment (/**) as the brief "
      "description. If set to NO, an explicit @brief command is required. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("QT_AUTO_BRIEF",
      "If this tag is set then DoxyPress will interpret the first line "
      "(until the first dot) of a Qt-style comment (/*!) as the brief description. "
      "<br><br>"
      "If set to NO, an explicit @brief command is required. "
      "The default value is: NO");

   s_fullHelp.insert("MULTILINE_CPP_BRIEF",
      "If this tag is set then DoxyPress will treat a multi-line C++ special comment "
      "block (a block of //! or /// comments) as a brief description. "
      "The default is to treat a multi-line C++ special comment block as a detailed description. "
      "Setting this tag, means Rational Rose comments are not recognized. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INHERIT_DOCS",
      "If this tag is set then an undocumented member inherits the "
      "documentation from any documented member which it re-implements. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SEPARATE_MEMBER_PAGES",
      "If this tag is set then DoxyPress will produce a new "
      "page for each member. If not set the documentation of a member will be part "
      "of the file/class/namespace that contains it. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("TAB_SIZE",
      "This tag is used to set the number of spaces in a tab. DoxyPress "
      "uses this value to replace tabs with spaces in code fragments. "
      "<br><br>"
      "Minimum: 1, Maximum: 16, Default: 4");

   s_fullHelp.insert("ALIASES",
      "This tag is used to specify a number of aliases which act as commands in "
      "the documentation. An alias has the form: "
      "<br><br>"
      "name=value"
      "<br><br>"
      "Adding \"sideeffect=@par Side Effects:\\n\" "
      "defines the command \\sideeffect (or @sideeffect) "
      "The result is a user-defined paragraph with a heading of \"Side Effects:\" ");

   s_fullHelp.insert("TCL_SUBST",
      "This tag is used to specify a number of word-keyword mappings (TCL only). "
      "A mapping has the form \"name=value\". For example, adding \"class=itcl::class\" "
      "allows you to use the command class in the itcl::class meaning.");

   s_fullHelp.insert("PROJECT_ENCODING",
      "This tag specifies the encoding used for all characters in the config file "
      "that follow. The default is UTF-8 which is also the encoding used for all text "
      "before the first occurrence of this tag. DoxyPress uses libiconv (or the iconv "
      "built into libc) for the transcoding. See http://www.gnu.org/software/libiconv "
      "for the list of possible encodings. "
      "<br><br>"
      "The default value is: UTF-8");

   s_fullHelp.insert("EXTENSION_MAPPING",
      "DoxyPress selects the language parser depending on the extension of the files it parses. "
      "Use this tag to assign a specific parser to use for a given file extension. "
      "The format for this tag is ext=language, where ext is a file extension and language"
      "is one of the parsers supported by DoxyPress. "
      "<br><br>"
      "The supported parsers are: C#, C, C++, D, IDL, Java, Javascript, Objective-C, "
      "PHP, Python, Fortran, FortranFixed, FortranFree. "
      "<br><br>"
      "As and example, to treat .f files as C (default is Fortran), use: f=C "
      "<br><br>"
      "Note: For files with no extensions use no_extension as a placeholder. "
      "For custom extensions set 'FILE PATTERNS', otherwise the files are not read by DoxyPress.");

   s_fullHelp.insert("MARKDOWN",
      "If this tag is set then DoxyPress pre-processes all comments "
      "according to the Markdown format. "
      "You can mix Markdown, HTML, and XML commands. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("AUTO_LINK",
      "When this tag  is set then DoxyPress tries to link words which correspond to documented "
      "classes, or namespaces to their corresponding documentation. Such a link can "
      "be prevented in individual cases by putting a % sign in front of the word or "
      "globally by setting 'AUTOLINK SUPPORT' to NO. "
      "<br><br>"
      "The default value is: YES");

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
      "getter and setter methods for a property. Setting this option to YES will make "
      "DoxyPress to replace the get and set methods by a property in the documentation. "
      "This will only work if the methods are indeed getting or setting a simple "
      "type. If this is not the case, or you want to show the methods anyway, you "
      "should set this option to NO. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("DIST_GROUP_DOC",
      "If member grouping is used in the documentation and this tag "
      "is set, DoxyPress will reuse the documentation of the first "
      "member in the group (if any) for the other members of the group. By default "
      "all members of a group must be documented explicitly. "
     "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("ALLOW_SUB_GROUPING",
      "Set this tag to allow class member groups of the same type "
      "(for instance a group of public functions) to be put as a subgroup of that type. "
      "Set this tag to NO to prevent subgrouping. Alternatively, sub-grouping can be done per "
      "class using the \\nosubgrouping command. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("INLINE_GROUPED_CLASSES",
      "When this tag is set classes, structs and unions are shown inside the group "
      "in which they are included (using the \\ingroup command) "
      "instead of on a separate page (for HTML and Man pages) or section (for LaTeX and RTF). "
      "<br><br>"
      "Note: This feature does not work in combination with 'SEPARATE MEMBER PAGES' "
      "The default value is: NO");

   s_fullHelp.insert("INLINE_SIMPLE_STRUCT",
      "When this tag is set structs, classes, and unions with only public data fields or simple "
      "typedef fields, will be shown inline in the documentation of the scope in which they are "
      "defined. If this tag is set to NO then structs, classes, and unions are shown on a separate page "
      "(for HTML and Man pages) or section (for LaTeX and RTF). "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("TYPEDEF_HIDES_STRUCT",
      "When the tag is set a typedef of a struct, union, or enum is documented as struct, union, or "
      "enum with the name of the typedef. For example, Typedef struct TypeS {} TypeT, will appear "
      "as a struct with name TypeT. When disabled, the typedef will appear as a member of a file, "
      "namespace, or class and the struct will be named TypeS. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("LOOKUP_CACHE_SIZE",
      "The size of the symbol lookup cache can be set using this tag. If the cache is too small "
      "DoxyPress may run slower. At the end of a run DoxyPress will report the cache usage and "
      " suggest the optimal cache size. "
      "<br><br>"
      "Minimum: 0, Maximum: 9, Default: 0");

   s_fullHelp.insert("EXTRACT_ALL",
      "If this tag is set then DoxyPress will assume all entities are documented. "
      "Private class members and static file members will be hidden unless the "
      "'EXTRACT PRIVATE' or the 'EXTRACT STATIC' tags are set to YES. "
      "<br><br>"
      "Note: Setting this tag will also disable warnings about undocumented members. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_PRIVATE",
      "If this tag is set then all private members of a class will be included in the documentation. "
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
      "If this tag is set then classes (and structs) defined locally in source files will "
      "be included in the documentation. If set to NO, only classes defined in "
      "header files are included. Does not have any effect for Java sources. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("EXTRACT_LOCAL_METHODS",
      "This tag is only useful for Objective-C code. If set to YES then local methods, "
      "which are defined in the implementation section, but not in the interface, are "
      "included in the documentation. If set to NO, only methods in the interface are included. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTRACT_ANON_NAMESPACES",
      "If this tag is set then the members of anonymous namespaces will be extracted and appear "
      "as a namespace called 'anonymous_namespace{file}'. The value for 'file' will be "
      "replaced with the base name of the file which contains the anonymous namespace. "
      "By default anonymous namespace are hidden. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_UNDOC_MEMBERS",
      "If this tag is set then DoxyPress will hide all undocumented members  inside "
      "documented classes or files. If set to NO, these members will be included in the "
      "various overviews, but no documentation section is generated. "
      "This tag has no effect if 'EXTRACT ALL' is set to YES. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_UNDOC_CLASSES",
      "If this tag is set then DoxyPress will hide all undocumented classes which are "
      "normally visible in the class hierarchy. If set to NO, these clases will be "
      "included in the various overviews. This tag has no effect if 'EXTRACT ALL' is set to YES. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_FRIEND_COMPOUNDS",
      "If this tag is set then DoxyPress will hide all friend (class|struct|union) declarations. "
      "If set to NO, these declarations will be included in the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("HIDE_IN_BODY_DOCS",
      "If this tag is set then DoxyPress will hide any documentation blocks found inside the body "
      "of a function. If set to NO, these blocks will be appended to the function's detailed "
      "documentation block. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INTERNAL_DOCS",
      "This tag determines if documentation which is typed after an \\internal command "
      "is included. If the tag is set to NO then the documentation will be excluded. "
      "Set to YES to include the internal documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("CASE_SENSE_NAMES",
      "If this tag is set to NO then DoxyPress will generates all file names in lower case.  "
      "If set to YES, case is preserved "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("HIDE_SCOPE_NAMES",
      "If this tag is set to NO then DoxyPress will show members with their full class "
      "and namespace scopes in the documentation. If set to YES, the scope will be hidden. "
      "The default value is: NO");

   s_fullHelp.insert("HIDE_COMPOUND_REFERENCE",
      "If this tag is set to NO then DoxyPress will append additional text to a page's title, "
      "such as Class Reference. If set to YES the compound reference will be hidden. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SHOW_INCLUDE_FILES",
      "If this tag is set then DoxyPress will put a list of the files which are"
      "included by a file in the documentation of that file. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SHOW_GROUPED_MEMBERS_INC",
      "If this tag is set then DoxyPress will add for each grouped member, an include statement "
      "indicating which file to include in order to use the member. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("FORCE_LOCAL_INCLUDES",
      "If this tag is set then DoxyPress will list include files with double quotes "
      "rather than with less than / greater than symbols. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INLINE_INFO",
      "If this tag is set then '[inline]' is displayed for inline members. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SORT_MEMBER_DOCS",
      "If this tag is set then DoxyPress will sort the (detailed) documentation of "
      "file and class members alphabetically by member name. If set to NO, "
      "the members will appear in declaration order. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SORT_BRIEF_DOCS",
      "If this tag is set then DoxyPress will sort the brief descriptions of file, namespace, "
      "and class members alphabetically by member name. If set to NO, the members will appear "
      "appear in declaration order. This will also influence the order of the classes in the class list. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SORT_MEMBERS_CTORS_FIRST",
      "If this tag is set to then DoxyPress will sort the brief and detailed) documentation of "
      "class members so that constructors and destructors are listed first. If set to NO, the"
      "constructors will appear in the respective orders defined by 'SORT BRIEF DOCS' "
      "and 'SORT MEMBER DOCS'. "
      "<br><br>"
      "If 'SORT BRIEF DOCS' is set to NO, this tag is ignored for sorting brief member documentation. "
      "<br><br>"
      "If 'SORT MEMBER DOCS' is set to NO this tag is ignored for sorting detailed member documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SORT_GROUP_NAMES",
      "If this tag is set then DoxyPress will sort the hierarchy of group names into alphabetical "
      "If set to NO the group names will appear in their defined order. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("SORT_BY_SCOPE_NAME",
      "If this tag is set the class list will be sorted by fully-qualified names, "
      "including namespaces. If set to NO, the class list will be sorted only by "
      "class name, not including the namespace part. "
      "<br><br>"
      "Note: This option is not very useful if HIDE_SCOPE_NAMES is set to YES. "
      "Note: This option applies only to the class list, not to the alphabetical list. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("STRICT_PROTO_MATCHING",
      "If this tag is set then an exact match of the function signature is required to docueent "
      "a function. An exact match means the function name and parameteres must all match. "
      "If this tag is set to NO, DoxyPress will do an approximate match, doing the best it can. "
      "<br><br>"
      "The default value is: NO");

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

   s_fullHelp.insert("ENABLED_SECTIONS",
      "This tag is used to enable conditional documentation by marking a section with one of the following: "
      "<br><br>"
      "\\if <section_label> ... \\endif "
      "<br>"
      "\\cond <section_label> ... \\endcond");

   s_fullHelp.insert("MAX_INIT_LINES",
      "This tag specifies the maximum number of lines (in your source) for the initial value of a "
      "variable or the defintion of a macro. If the initializer consists of more lines than specified in "
      "this tag, the entire initializer will be hidden. "
      "This tag can be overridden on a per variable or macro basis by using the \\showinitializer or "
      "\\hideinitializer command. "
      "<br><br>"
      "A value of 0 will hide initializers completely. "
      "Minimum: 0, Maximum: 10000, Default: 30");

   s_fullHelp.insert("SHOW_USED_FILES",
      "Setting this tag to NO disables the list of files generated at the bottom of the "
      "documentation for classes and structs. If this tag is set to YES, the list will "
      "contain a list of the files used to generate the documentation. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SHOW_FILES",
      "Set this tag to NO to disable the generation of the Files page. "
      "This will remove the Files entry from the Quick Index and from the Folder Tree View if specified. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SHOW_NAMESPACES",
      "Set this tag to NO to disable the generation of the Namespaces page. "
      "This will remove the Namespaces entry from the Quick Index and from the Folder Tree "
      "View if specified. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("FILE_VERSION_FILTER",
      "This tag is used to specify a program DoxyPress should invoke to obtain the current version "
      "for each file. DoxyPress will invoke this program and pass a file name as the input parameter. "
      "The value written to standard output is used as the file version.");

   s_fullHelp.insert("LAYOUT_FILE",
      "This tag can be used to specify a layout file which will be parsed "
      "by DoxyPress. The layout file controls the global structure of the generated output files. "
      "To create a new layout file run 'DoxyPress --l [layout file name]' "
      "<br><br>"
      "Note: If run from a directory containing a file called DoxyPress_Layout.xml, "
      "DoxyPress will parse it automatically even if this tag is left empty.");

   s_fullHelp.insert("CITE_BIB_FILES",
      "This tag is used to specify one or more bib files containing the reference definitions. "
      "The .bib extension is automatically appended if omitted from the file name. "
      "For LaTeX the style of the bibliography can be controlled using 'LATEX BIB STYLE' tag. "
      "In order to use this tool, the bibtex tool and perl must be available in the "
      "search path. Refer to the DoxyPress manual regarding \\cite.");

   s_fullHelp.insert("QUIET",
      "This tag is used to turn on/off the messages which are generated to "
      "standard output by DoxyPress. If this tag is set messages are off. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("WARNINGS",
      "This tag can be used to turn on/off the warning messages which are sent to "
      "standard error (stderr) by DoxyPress. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("WARN_UNDOC",
      "If this tag is set then DoxyPress will generate warnings for undocumented members. "
      "If 'EXTRACT ALL' is set then this tag will automatically be disabled. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("WARN_DOC_ERROR",
      "If this tag is set then DoxyPress will generate warnings for potential errors "
      "such as not documenting some parameters in a documented function, documenting parameters "
      " which do not exist, or using markup commands incorectly. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("WARN_UNDOC_PARM",
      "This tag  is used to warnings for functions which are are documented, but have no "
      "documentation for their parameters or return value. "
      "If set to NO, DoxyPress will only warn about wrong or incomplete "
      "parameter documentation, but not about the absence of documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("WARN_FORMAT",
      "This tag determines the format of the warning messages DoxyPress will output. "
      "The string normally contains the $file, $line, and $text tags, which "
      "will be replaced by the file name, line number from which the warning originated, "
      "and the warning text. The format can contain $version, which will "
      "be replaced by the version of the file, if it can be obtained from 'FILE VERSION FILTER'. "
      "<br><br>"
      "The default value is: $file:$line: $text");

   s_fullHelp.insert("WARN_LOGFILE",
      "This tag is used to specify a file which warning and error "
      "messages should be written. If left blank the output is written to standard "
      "error (stderr).");

   s_fullHelp.insert("INPUT_SOURCE",
      "This tag is used to specify the files and/or directories which contain "
      "documented source files. File names like myfile.cpp or directories like /usr/src/myproject "
      "cen be entered. Separate the files or directories with a comma.  "
      "<br><br>"
      "Note: If this tag is empty only the current directory is searched.");

   s_fullHelp.insert("INPUT_ENCODING",
      "This tag is used to specify the character encoding of the source files DoxyPress will parse. "
      "<br><br>"
      "The default value is: UTF-8");

   s_fullHelp.insert("FILE_PATTERNS",
      "If the 'INPUT / SOURCE DIRECTORIES' tag contains directories, this tag will be used to "
      "specify wildcard patterns to filter out the source files in the directories. "
      "<br><br>"
      "The defualt values are: " + m_filePatterns);

   s_fullHelp.insert("SOURCE_RECURSIVE",
      "This tag can be used to specify whether or not subdirectories should "
      "be searched for input files. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXCLUDE_FILES",
      "This tag is used to specify files and/or directories which should be "
      "excluded from the INPUT source files. "
      "<br><br>"
      "Note: The relative paths are relative to the directory from which DoxyPress is run.");

   s_fullHelp.insert("EXCLUDE_SYMLINKS",
      "This tag is used to select if symbolic links are excluded from the input. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXCLUDE_PATTERNS",
      "If the 'INPUT / SOURCE DIRECTORIES' tag contains directories, this tag will be used to "
      "specify wildcard patterns to exclude certain files from those directories. "
      "<br><br>"
      "Note: Wildcards are matched against the file with absolute path. As an example, to "
      "exclude all test directories use the pattern */test/* ");

   s_fullHelp.insert("EXCLUDE_SYMBOLS",
      "This tag is used to specify one or more symbol names "
      "(namespaces, classes, functions, etc.) which should be excluded from the "
      "output. The symbol name can be a fully qualified name, a word, or the * wildcard. "
      "Examples: ANamespace, AClass, AClass::ANamespace, ANamespace::*Test "
      "<br><br>"
      "Note: Wildcards are matched against the file with absolute path, so to "
      "exclude all test directories use the pattern */test/*");

   s_fullHelp.insert("EXAMPLE_SOURCE",
      "Used  to specify one or more files or directories "
      "which contain example code fragments that are included (see the \\include command).");

   s_fullHelp.insert("EXAMPLE_PATTERNS",
      "If the value of the 'EXAMPLE PATH' tag contains directories, then this tag can be used "
      "to specify one or more wildcard patterns like *.cpp or *.h to filter out the "
      "source files in the example directories. "
      "<br><br>"
      "The default value is: *");

   s_fullHelp.insert("EXAMPLE_RECURSIVE",
      "If this tag is set then subdirectories will be searched for input files to be used "
      "with the \\include or \\dontinclude commands irrespective of the value of the RECURSIVE tag. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("IMAGE_PATH",
      "This tag can be used to specify one or more files or directories "
      "which contain images to be included in the documentation. Refer to the \\image command.");

   s_fullHelp.insert("INPUT_FILTER",
      "This tag can be used to specify a program DoxyPress should invoke to filter each "
      "input file. DoxyPress will invoke the filter program by executing the command: "
      "<br><br>"
      "<filter> <input-file> "
      "<br><br>"
      "where <filter> is the value of the INPUT_FILTER tag, and <input-file> is the "
      "name of an input file. DoxyPress will then use the output that the filter "
      "program writes to standard output. If FILTER_PATTERNS is specified, this tag "
      "will be ignored. "
      "<br><br>"
      "Note: The filter must not add or remove lines and is applied before the "
      "code is scanned, not when the output code is generated. If lines are added "
      "or removed, the anchors will not be placed correctly.");

   s_fullHelp.insert("FILTER_PATTERNS",
      "This tag is used to specify different filter programs on a per source file basis. "
      "DoxyPress will compare each source file name with the list of patterns and apply the "
      "filter program which matches. "
      "If this tag is empty or none of the patterns match, then the 'INPUT FILTER' "
      "program is used. The filter programs are listed in the format: "
      "<br><br>"
      "pattern=filter "
      "<br><br>"
      "Example: *.cpp=my_cpp_filter");

   s_fullHelp.insert("FILTER_SOURCE_FILES",
      "If this tag is set then the 'INPUT FILTER' will be used to filter the input "
      "source files when generating source documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("FILTER_SOURCE_PATTERNS",
      "This tag is used to specify different filter programs on a per source file basis. "
      "DoxyPress will compare each source file name with the list of patterns and apply the "
      "filter program which matches. This tag will override the setting for 'FILTER PATTERNS'"
      "<br><br>"
      "This tag requires the tag 'FILTER SOURCE FILES' is set to YES");

   s_fullHelp.insert("MDFILE_MAINPAGE",
      "This tag refers to the name of a markdown file which is part of the input. "
      "Its contents will be placed on the main index.html page.");

   s_fullHelp.insert("SOURCE_BROWSER",
      "If this tag is set then a list of source files will be generated. "
      "Documented entities will be cross-referenced with these sources. "
      "To remove all source code in the generated output ensure "
      "the 'VERBATIM HEADERS' is set to NO "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("INLINE_SOURCES",
      "Setting this tag will include the body of functions, classes and enums "
      "directly into the documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("STRIP_CODE_COMMENTS",
      "If this tag is set DoxyPress will hide any special comment blocks from generated "
      "source code fragments. Normal C, C++ and Fortran comments will always remain visible. "
      "<br><br>"
      "The default value is: YES");


// BROOM - start here

   s_fullHelp.insert("REF_BY_RELATION",
      "If the REFERENCED_BY_RELATION tag is set to YES then for each documented "
      "function all documented functions referencing it will be listed. "
      "The default value is: NO");

   s_fullHelp.insert("REF_RELATION",
      "If the REFERENCES_RELATION tag is set to YES then for each documented function "
      "all documented entities called/used by that function will be listed. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("REF_LINK_SOURCE",
      "If the REFERENCES_LINK_SOURCE tag is set to YES and SOURCE_BROWSER tag is set "
      "to YES then the hyperlinks from functions in REFERENCES_RELATION and "
      "REFERENCED_BY_RELATION lists will link to the source code. Otherwise they will "
      "link to the documentation. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("SOURCE_TOOLTIPS",
      "If SOURCE_TOOLTIPS is enabled (the default) then hovering a hyperlink in the "
      "source code will show a tooltip with additional information such as prototype, "
      "brief description and links to the definition and documentation. Since this "
      "will make the HTML file larger and loading of large files a bit slower, you "
      "can opt to disable this feature. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'SOURCE BROWSER' is set to YES");

   s_fullHelp.insert("USE_HTAGS",
      "If this tag is set then the references to source code will "
      "point to the HTML generated by the htags(1) tool instead of DoxyPress built-in "
      "source browser. The htags tool is part of GNU's global source tagging system "
      "Refer to http://www.gnu.org/software/global/global.html, version 4.8.6 or higher. "
      "<br><br>"
      "Once this is installed enable SOURCE_BROWSER and USE_HTAGS. Make sure the INPUT "
      "points to the root of the source tree "
      "<br><br>"
      "DoxyPress will invoke htags which will invoke gtags. These tools must be available "
      "from your search path. "
      "<br><br>"
      "The result is, instead of the source browser generated by DoxyPress, the links to "
      "source code will point to the output of htags. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'SOURCE BROWSER' is set to YES");

   s_fullHelp.insert("VERBATIM_HEADERS",
      "If this tag is set then DoxyPress will generate a "
      "verbatim copy of the header file for each class for which an include is "
      "specified. Set to NO to disable this feature. "
      "Refer to the DoxPress manual regarding \\class. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("CLANG_PARSING",
      "If the CLANG_ASSISTED_PARSING tag is set to YES then DoxyPress will use the "
      "clang parser (see: http://clang.llvm.org/) for more accurate parsing at the "
      "cost of reduced performance. This can be particularly helpful with template "
      "rich C++ code for which DoxyPress's built-in parser lacks the necessary type "
      "information. "
      "Note: The availability of this option depends on whether or not DoxyPress was "
      "compiled with the --with-libclang option. "
      "The default value is: NO");

   s_fullHelp.insert("CLANG_OPTIONS",
      "If clang assisted parsing is enabled you can provide the compiler with command "
      "line options that you would normally use when invoking the compiler. Note that "
      "the include paths will already be set by DoxyPress for the files and directories "
      "specified with INPUT and INCLUDE_PATH. "
      "This tag requires the tag CLANG_ASSISTED_PARSING is set to YES");

   s_fullHelp.insert("ALPHA_INDEX",
      "If the ALPHABETICAL_INDEX tag is set to YES, an alphabetical index of all "
      "compounds will be generated. Enable this if the project contains a lot of "
      "classes, structs, unions or interfaces. "
      "The default value is: YES");

   s_fullHelp.insert("COLS_IN_INDEX",
      "The COLS_IN_ALPHA_INDEX tag can be used to specify the number of columns in "
      "which the alphabetical index list will be split. "
      "<br><br>"
      "Minimum: 1, Maximum: 20, Default: 5 "
      "This tag requires the tag 'ALPHABETICAL INDEX' is set to YES");

   s_fullHelp.insert("IGNORE_PREFIX",
      "In case all classes in a project start with a common prefix, all classes will "
      "be put under the same header in the alphabetical index. The IGNORE_PREFIX tag "
      "can be used to specify a prefix (or a list of prefixes) that should be ignored "
      "while generating the index headers. "
      "<br><br>"
      "This tag requires the tag 'ALPHABETICAL INDEX' is set to YES");

   s_fullHelp.insert("GEN_AUTOGEN_DEF",
      "If the GENERATE_AUTOGEN_DEF tag is set to YES, DoxyPress will generate an "
      "AutoGen Definitions (see http://autogen.sf.net) file that captures the "
      "structure of the code including all documentation. Note that this feature is "
      "still experimental and incomplete at the moment. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("ENABLE_PREPROCESSING",
      "If this tag is set then DoxyPress will evaluate all "
      "C-preprocessor directives found in the sources and include files. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("MACRO_EXPANSION",
      "If the MACRO_EXPANSION tag is set to YES, DoxyPress will expand all macro names "
      "in the source code. If set to NO, only conditional compilation will be "
      "performed. Macro expansion can be done in a controlled way by setting "
      "EXPAND_ONLY_PREDEF to YES. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("EXPAND_ONLY_PREDEFINED",
      "If the EXPAND_ONLY_PREDEF and MACRO_EXPANSION tags are both set to YES then "
      "the macro expansion is limited to the macros specified with the PREDEFINED and "
      "EXPAND_AS_DEFINED tags. "
      "The default value is: NO "
      "This tag requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("SEARCH_INCLUDES",
      "If the SEARCH_INCLUDES tag is set to YES, the include files in the "
      "INCLUDE_PATH will be searched if a #include is found. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("INCLUDE_PATH",
      "The INCLUDE_PATH tag can be used to specify one or more directories that "
      "contain include files that are not input files but should be processed by the "
      "preprocessor. "
      "<br><br>"
      "This tag requires the tag 'SEARCH INCLUDES' is set to YES");

   s_fullHelp.insert("INCLUDE_FILE_PATTERNS",
      "You can use the INCLUDE_FILE_PATTERNS tag to specify one or more wildcard "
      "patterns (like *.h and *.hpp) to filter out the header-files in the "
      "directories. If left blank, the patterns specified with FILE_PATTERNS will be "
      "used. "
      "This tag requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("PREDEFINED_MACROS",
      "The PREDEFINED tag can be used to specify one or more macro names that are "
      "defined before the preprocessor is started (similar to the -D option of e.g. "
      "gcc). The argument of the tag is a list of macros of the form: name or "
      "name=definition (no spaces). If the definition and the \"=\" are omitted, \"=1\" "
      "is assumed. To prevent a macro definition from being undefined via #undef or "
      "recursively expanded use the := operator instead of the = operator. "
      "<br><br>"
      "This tag requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("EXPAND_AS_DEFINED",
      "If the MACRO_EXPANSION and EXPAND_ONLY_PREDEF tags are set to YES then this "
      "tag can be used to specify a list of macro names that should be expanded. The "
      "macro definition that is found in the sources will be used. Use the PREDEFINED "
      "tag if you want to use a different macro definition that overrules the "
      "definition found in the source code. "
      "<br><br>"
      "This tag requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("SKIP_FUNCTION_MACROS",
      "If the SKIP_FUNCTION_MACROS tag is set to YES then DoxyPress's preprocessor will "
      "remove all references to function-like macros that are alone on a line, have "
      "an all uppercase name, and do not end with a semicolon. Such function macros "
      "are typically used for boiler-plate code, and will confuse the parser if not "
      "removed. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'ENABLE PREPROCESSING' is set to YES");

   s_fullHelp.insert("TAG_FILES",
      "The TAGFILES tag can be used to specify one or more tag files. For each tag "
      "file the location of the external documentation should be added. The format of "
      "a tag file without this location is as follows: "
      "TAGFILES = file1 file2 ... "
      "Adding location for the tag files is done as follows: "
      "TAGFILES = file1=loc1 \"file2 = loc2\" ... "
      "where loc1 and loc2 can be relative or absolute paths or URLs. See the "
      "section \"Linking to external documentation\" for more information about the use "
      "of tag files. "
      "<br><br>"
      "Note: Each tag file must have a unique name (where the name does NOT include "
      "the path). If a tag file is not located in the directory in which DoxyPress is "
      "run you must also specify the path to the tagfile here.");

   s_fullHelp.insert("GEN_TAGFILE",
      "When a file name is specified after GENERATE_TAGFILE, DoxyPress will create a "
      "tag file that is based on the input files it reads. See section \"Linking to "
      "external documentation\" for more information about the usage of tag files.");

   s_fullHelp.insert("ALL_EXTERNALS",
      "If the ALLEXTERNALS tag is set to YES, all external class will be listed in "
      "the class index. If set to NO, only the inherited external classes will be "
      "listed. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("EXTERNAL_GROUPS",
      "If the EXTERNAL_GROUPS tag is set to YES, all external groups will be listed "
      "in the modules index. If set to NO, only the current project's groups will be listed. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("EXTERNAL_PAGES",
      "If the EXTERNAL_PAGES tag is set to YES, all external pages will be listed in "
      "the related pages index. If set to NO, only the current project's pages will be listed. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("PERL_PATH",
      "This tage should be the absolute path and name of the perl script interpreter. "
      "<br><br>"
      "The default file (with absolute path) is: /usr/bin/perl.");

   s_fullHelp.insert("CLASS_DIAGRAMS",
      "If this tag is set DoxyPress will generate a class diagram in HTML and LaTeX "
      "for classes with base or super classes. Setting the tag to NO turns the diagrams "
      "off. This option also works with 'HAVE DOT' disabled. "
      "It is recommended to install and use dot, since it yields more powerful graphs. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("MSCGEN_PATH",
      "You can define message sequence charts within DoxyPress comments using the \\msc "
      "command. DoxyPress will then run the mscgen tool to produce the chart and insert it in the "
      "documentation. The MSCGEN_PATH tag allows you to specify the directory where "
      "the mscgen tool resides. If left empty the tool is assumed to be found in the "
      "default search path. Refere to: http://www.mcternan.me.uk/mscgen/");

   s_fullHelp.insert("DIA_PATH",
      "You can include diagrams made with dia in DoxyPress documentation. DoxyPress will "
      "then run dia to produce the diagram and insert it in the documentation. The "
      "DIA_PATH tag allows you to specify the directory where the dia binary resides. "
      "If left empty dia is assumed to be found in the default search path.");

   s_fullHelp.insert("HIDE_UNDOC_RELATIONS",
      "If set to YES the inheritance and collaboration graphs will hide inheritance "
      "and usage relations if the target is undocumented or is not a class. "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("HAVE_DOT",
      "If this tag is set then DoxyPress will assume the dot tool is available from the path. "
      "This tool is part of Graphviz, a graph visualization toolkit from AT&T and Lucent "
      "Bell Labs. The other options in this section have no effect if this option is set to NO "
      "Refer to: http://www.graphviz.org/ "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("DOT_NUM_THREADS",
      "This tag specifies the number of dot invocations DoxyPress is allowed "
      "to run in parallel. When set to 0 DoxyPress will base this on the number of "
      "processors available in the system. You can set it explicitly to a value "
      "larger than 0 to get control over the balance between CPU load and processing speed. "
      "<br><br>"
      "Minimum: 0, Maximum: 32, Default: 0 "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FONT_NAME",
      "Specifies a different font for DOT. The path can be set using the 'DOT FONT PATH' tag. "
      "<br><br>"
      "The default value is: Helvetica "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FONT_SIZE",
      "This tag can be used to set the size (in points) of the font of dot graphs. "
      "<br><br>"
      "Minimum: 4, Maximum: 24, Default: 10 "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FONT_PATH",
      "By default DOT will use the default font as specified by 'DOT FONTNAME'. "
      "If you specify a different font, this tag is used to specify the path to the font file. "
      "<br><br>"
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CLASS_GRAPH",
      "If the CLASS_GRAPH tag is set to YES then DoxyPress will generate a graph for "
      "each documented class showing the direct and indirect inheritance relations. "
      "Setting this tag to YES will force the CLASS_DIAGRAMS tag to NO. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_COLLABORATION",
      "If the COLLABORATION_GRAPH tag is set to YES then DoxyPress will generate a "
      "graph for each documented class showing the direct and indirect implementation "
      "dependencies (inheritance, containment, and class references variables) of the "
      "class with other documented classes. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("GROUP_GRAPHS",
      "If the GROUP_GRAPHS tag is set to YES then DoxyPress will generate a graph for "
      "groups, showing the direct groups dependencies. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("UML_LOOK",
      "If this tag is set to YES DoxyPress will generate inheritance and "
      "collaboration diagrams in a style similar to the OMG's Unified Modeling Language. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("UML_LIMIT_NUM_FIELDS",
      "If the UML_LOOK tag is enabled, the fields and methods are shown inside the "
      "class node. If there are many fields or methods and many nodes the graph may "
      "become too big to be useful. The UML_LIMIT_NUM_FIELDS threshold limits the "
      "number of items for each type to make the size more manageable. Set this to 0 "
      "for no limit. Note that the threshold may be exceeded by 50% before the limit "
      "is enforced. So when you set the threshold to 10, up to 15 fields may appear, "
      "but if the number exceeds 15, the total amount of fields shown is limited to 10. "
      "<br><br>"
      "Minimum: 0, Maximum: 100, Default: 10 "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("TEMPLATE_RELATIONS",
      "If the TEMPLATE_RELATIONS tag is set to YES then the inheritance and "
      "collaboration graphs will show the relations between templates and their "
      "instances. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_INCLUDE",
      "If the INCLUDE_GRAPH, ENABLE_PREPROCESSING and SEARCH_INCLUDES tags are set to "
      "YES then DoxyPress will generate a graph for each documented file showing the "
      "direct and indirect include dependencies of the file with other documented "
      "files. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_INCLUDED_BY",
      "If the INCLUDED_BY_GRAPH, ENABLE_PREPROCESSING and SEARCH_INCLUDES tags are "
      "set to YES then DoxyPress will generate a graph for each documented file showing "
      "the direct and indirect include dependencies of the file with other documented files. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CALL",
      "If the CALL_GRAPH tag is set to YES then DoxyPress will generate a call "
      "dependency graph for every global function or class method. "
      "<br><br>"
      "Note that enabling this option will significantly increase the time of a run. "
      "So in most cases it will be better to enable call graphs for selected "
      "functions only using the \\callgraph command. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CALLED_BY",
      "If the CALLER_GRAPH tag is set to YES then DoxyPress will generate a caller "
      "dependency graph for every global function or class method. "
      "<br><br>"
      "Note that enabling this option will significantly increase the time of a run. "
      "So in most cases it will be better to enable caller graphs for selected "
      "functions only using the \\callergraph command. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_HIERARCHY",
      "If this tag is set then DoxyPress will graphical hierarchy of all "
      "classes instead of a textual one. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DIRECTORY_GRAPH",
      "If the DIRECTORY_GRAPH tag is set to YES then DoxyPress will show the "
      "dependencies a directory has on other directories in a graphical way. The "
      "dependency relations are determined by the #include relations between the "
      "files in the directories. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_IMAGE_FORMAT",
      "This tag is used to set the image format of the images generated by dot. "
      "If you select 'svg' also set 'HTML FILE EXTENSION' to 'xhtml' in order "
      "to ensure the SVG files are visible in IE 9+. "
      "<br><br>"
      "Possible values are: png, jpg, gif or svg "
      "The default value is: png "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("INTERACTIVE_SVG",
      "If DOT_IMAGE_FORMAT is set to svg, then this option can be set to YES to "
      "enable generation of interactive SVG images that allow zooming and panning. "
      " "
      "Note this requires a modern browser other than Internet Explorer. Tested "
      "and working are Firefox, Chrome, Safari, and Opera. "
      "Note: For IE 9+ you need to set HTML_FILE_EXTENSION to xhtml in order to make "
      "the SVG files visible. Older versions of IE do not have SVG support. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_PATH",
      "The tag can be used to specify the path where the dot tool will be  "
      "found. If left blank it is assumed the dot tool can be found in the users path. "
      "<br><br>"
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_FILE_DIRS",
      "The DOTFILE_DIRS tag can be used to specify one or more directories that "
      "contain dot files that are included in the documentation (see the \\dotfile "
      "command). "
      "<br><br>"
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("MSC_FILE_DIRS",
      "This tag is used to specify one or more directories which "
      "contain msc files, that are included in the documentation. "
      "Refer to the DoxyPress manual regarding \\mscfile ");

   s_fullHelp.insert("DIA_FILE_DIRS",
      "This tag is used to specify one or more directories which "
      "contain dia files that are included in the documentation. "
      "Refer to the DoxyPress manual regarding \\diafile ");

   s_fullHelp.insert("PLANTUML_JAR_PATH",
      "When using plantuml, this tag specifies the path where java can find the "
      "plantuml.jar file. If left blank, it is assumed "
      "PlantUML is not used or called during a preprocessing step. DoxyPress will "
      "generate a warning when it encounters a \\startuml command in this case and "
      "will not generate output for the diagram.");

   s_fullHelp.insert("PLANTUML_INCLUDE_PATH",
      "When using plantuml, the specified paths are searched for files specified by "
      "the !include statement in a plantuml block.");

   s_fullHelp.insert("DOT_GRAPH_MAX_NODES",
      "The DOT_GRAPH_MAX_NODES tag can be used to set the maximum number of nodes "
      "that will be shown in the graph. If the number of nodes in a graph becomes "
      "larger than this value, DoxyPress will truncate the graph, which is visualized "
      "by representing a node as a red box. Note that DoxyPress if the number of direct "
      "children of the root node in a graph is already larger than "
      "DOT_GRAPH_MAX_NODES then the graph will not be shown at all. Also note that "
      "the size of a graph can be further restricted by MAX_DOT_GRAPH_DEPTH. "
      "<br><br>"
      "Minimum: 0, Maximum: 10000, Default: 50 "
      "This tag requires the tag'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_GRAPH_MAX_DEPTH",
      "This tag can be used to set the maximum depth of the graphs "
      "generated by dot. A depth value of 3 means that only nodes reachable from the "
      "root by following a path via at most 3 edges will be shown. Nodes that lay "
      "further from the root node will be omitted. Note that setting this option to 1 "
      "or 2 may greatly reduce the computation time needed for large code bases. Also "
      "note that the size of a graph can be further restricted by "
      "DOT_GRAPH_MAX_NODES. Using a depth of 0 means no depth restriction. "
      "<br><br>"
      "Minimum: 0, Maximum: 1000, Default: 0 "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_TRANSPARENT",
      "Set this tag to YES to generate images with a transparent background. This tag is "
      "disabled by default since dot on Windows does not support this feature. "
      "Warning: Depending on your platform, enabling this option may lead to "
      "badly anti-aliased labels on the edges of a graph (i.e. they become hard to read). "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_MULTIPLE_TARGETS",
      "Set this tag to YES to allow dot to generate multiple output "
      "files in one run (i.e. multiple -o and -T options on the command line). Setting this tag to YEST "
      "makes dot run faster, however only newer versions of dot (>1.8.10) support this feature. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("GEN_LEGEND",
      "If this tag is set to YES DoxyPress will generate a legend page "
      "explaining the meaning of the various boxes and arrows in the dot generated graphs. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   s_fullHelp.insert("DOT_CLEANUP",
      "If this tag is set to YES DoxyPress will remove the intermediate dot "
      "files which are used to generate the various graphs. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'HAVE DOT' is set to YES");

   // tab 3
   s_fullHelp.insert("GEN_HTML",
      "If the 'GENERATE HTML' tag is set to YES, DoxyPress will generate HTML output "
      "<br><br>"
      "The default value is: YES");

   s_fullHelp.insert("HTML_OUTPUT",
      "This tag is used to specify the location for the HTML output. If a "
      "relative path is entered the value of 'OUTPUT DIRECTORY' will be prepended. "
      "<br><br>"
      "The default directory is: html "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_FILE_EXTENSION",
      "The HTML_FILE_EXTENSION tag can be used to specify the file extension for each "
      "generated HTML page (for example: .htm, .php, .asp). "
      "<br><br>"
      "The default value is: .html "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_HEADER",
      "Used to specify a user-defined HTML header file for each generated HTML page. "
      "If the tag is left blank DoxyPress will generate a standard header."
      "For valid HTML the header file, which includes any scripts and style sheets, "
      "is dependent on the configuration options used (for example, GENERATE_TREEVIEW). "
      "Start with a default header and modify the file as required. "
      "<br><br>"
      "To create a new header file run 'DoxyPress --w html-head [header file name]' "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_FOOTER",
      "The HTML_FOOTER tag can be used to specify a user-defined HTML footer for each "
      "generated HTML page. If the tag is left blank DoxyPress will generate a standard "
      "footer. See HTML_HEADER for more information on how to generate a default "
      "footer and what special commands can be used inside the footer. "
      "<br><br>"
      "To create a new footer file run 'DoxyPress --w html-foot [footer file name]' "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_STYLESHEETS",
      "This tag is used to specify additional custom css files which will be included "
      "after the standard 'doxy_style.css' file. DoxyPress will automatically copy  "
      "any css files listed in this tag to the 'HTML OUTPUT DIRECTORY'. "
      "<br><br>"
      "Note: The order of the css files is important. The last stylesheet will override the "
      "settings of any previous stylesheets. "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_EXTRA_FILES",
      "This tag is  used to specify one or more extra images or other source files which "
      "should be copied to the base 'HTML OUTPUT DIRECTORY'. Use the $relpath^ varialbe in your "
      "'HTML HEADER' or 'HTML FOOTER' files to load the extra files. "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_COLORSTYLE_HUE",
      "This tag controls the color of the HTML output. DoxyPress will adjust the colors in "
      "the stylesheet and background images according to this color. "
      "<br><br>"
      "Minimum: 0, Maximum: 359, Default: 220 "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_COLORSTYLE_SAT",
      "This tag controls the purity (or saturation) of the colors in the HTML output. "
      "<br><br>"
      "Minimum: 0, Maximum: 255, Default: 100 "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_COLORSTYLE_GAMMA",
      "This controls the gamma correction applied to the luminance component of the colors "
      "in the HTML output. "
      "<br><br>"
      "Minimum: 40, Maximum: 240, Default: 80 "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_TIMESTAMP",
      "If this tag is set to YES, the footer of each generated HTML "
      "page will contain the date and time when the page was generated. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_DYNAMIC_SECTIONS",
      "If the HTML_DYNAMIC_SECTIONS tag is set to YES then the generated HTML "
      "documentation will contain sections that can be hidden and shown after the "
      "page has loaded. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_INDEX_NUM_ENTRIES",
      "This tag defines the preferred number of entries shown in the various tree "
      "structured indices. DoxyPress will expand the tree until the specified "
      "number of entries are visible. Setting the number of entries to 1 will "
      "produce a full collapsed tree. A value of 0 will result in a fully expanded tree. "
      "<br><br>"
      "Minimum: 0, Maximum: 9999, Default: 100 "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   // chm
   s_fullHelp.insert("GEN_CHM",
      "If this tag is set then DoxyPress generates three "
      "additional HTML index files: index.hhp, index.hhc, and index.hhk. The "
      "index.hhp is a project file that can be read by Microsoft's HTML Help Workshop "
      "on Windows. "
      "<br><br>"
      "The HTML Help Workshop contains a compiler which can convert HTML output "
      "into a single compiled HTML .chm file. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("CHM_FILE",
      "The CHM_FILE tag can be used to specify the file name of the resulting .chm "
      "file. You can add a path in front of the file if the result should not be "
      "written to the html output directory. "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML'HELP is set to YES");

   s_fullHelp.insert("HHC_LOCATION",
      "The HHC_LOCATION tag can be used to specify the location (absolute path "
      "including file name) of the HTML help compiler (hhc.exe). If non-empty, "
      "DoxyPress will try to run the HTML help compiler on the generated index.hhp. "
      "The file has to be specified with full path. "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML'HELP is set to YES");

   s_fullHelp.insert("GEN_CHI",
      "The GENERATE_CHI flag controls if a separate .chi index file is generated "
      "(YES) or that it should be included in the master .chm file (NO). "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML'HELP is set to YES");

   s_fullHelp.insert("CHM_INDEX_ENCODING",
      "The CHM_INDEX_ENCODING is used to encode HtmlHelp index (hhk), content (hhc) "
      "and project file content. "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML'HELP is set to YES");

   s_fullHelp.insert("BINARY_TOC",
      "The BINARY_TOC flag controls whether a binary table of contents is generated "
      "(YES) or a normal table of contents (NO) in the .chm file. Furthermore it "
      "enables the Previous and Next buttons. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML'HELP is set to YES");

   s_fullHelp.insert("TOC_EXPANDED",
      "If this tag is set to YES, then group members will be added to  "
      "the table of contents of the HTML help documentation and to the tree view. "
      "The default value is: NO "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML HELP' is set to YES");

   // docbook
   s_fullHelp.insert("GEN_DOCBOOK",
      "If this tag is set then DoxyPress will generate Docbook files which can be used "
      "to generate a PDF. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("DOCBOOK_OUTPUT",
      "This tag is used to specify the location for the DocBook output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will be prepended. "
      "<br><br>"
      "The default directory is: docbook "
      "This tag requires the tag 'GENERATE DOCBOOK' is set to YES");

   s_fullHelp.insert("DOCBOOK_PROGRAM_LISTING",
      "If this tage is set then DoxyPress will include the "
      "program listings, including syntax highlighting and cross-referencing "
      "information, to the Docbook output. "
      "<br><br>"
      "Note: Enabling this will significantly increase the size of the  output. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE DOCBOOK' is set to YES");

   // docset
   s_fullHelp.insert("GEN_DOCSET",
      "If this tag is set additional index files will be "
      "generated which can be used as input for Apple's Xcode IDE. DoxyPress will generate a "
      "Makefile in the HTML output directory. Running make will produce the docset "
      "and running make install will install the docset. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("DOCSET_FEEDNAME",
      "This tag determines the name of the docset feed. A documentation feed provides "
      "an umbrella under which multiple documentation sets from a single provider "
      "(such as a company or product suite) can be grouped. "
      "<br><br>"
      "The default value is: DoxyPress generated docs "
      "This tag requires the tag GENERATE_DOCSET is set to YES");

   s_fullHelp.insert("DOCSET_BUNDLE_ID",
      "This tag specifies a string that should uniquely identify the documentation "
      "set bundle. This should be a reverse domain-name style string, e.g. "
      "com.mycompany.MyDocSet. DoxyPress will append .docset to the name. "
      "<br><br>"
      "The default value is: org.doxypress.Project "
      "This tag requires the tag GENERATE_DOCSET is set to YES");

   s_fullHelp.insert("DOCSET_PUBLISHER_ID",
      "The DOCSET_PUBLISHER_ID tag specifies a string that should uniquely identify "
      "the documentation publisher. This should be a reverse domain-name style "
      "string, e.g. com.mycompany.MyDocSet.documentation. "
      "<br><br>"
      "The default value is: org.doxypress.Publisher "
      "This tag requires the tag GENERATE_DOCSET is set to YES");

   s_fullHelp.insert("DOCSET_PUBLISHER_NAME",
      "The DOCSET_PUBLISHER_NAME tag identifies the documentation publisher. "
      "<br><br>"
      "The default value is: Publisher. "
      "This tag requires the tag GENERATE_DOCSET is set to YES");

   // tab 3 perl module
   s_fullHelp.insert("GEN_PERL",
      "If this tag is set then DoxyPress will generate a Perl module "
      "file which captures the structure of the code including all documentation. "
      "Note that this feature is still experimental and incomplete at the moment. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("PERL_LATEX",
      "If this tag is set then DoxyPress will generate the necessary "
      "Makefile rules, Perl scripts, and LaTeX code to be able to generate PDF and DVI "
      "output from the Perl module output. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE PERLMOD' is set to YES");

   s_fullHelp.insert("PERL_PRETTY",
      "If the PERLMOD_PRETTY tag is set to YES, the Perl module output will be nicely "
      "formatted so it can be parsed by a human reader. This is useful if you want to "
      "understand what is going on. On the other hand, if this tag is set to NO, the "
      "size of the Perl module output will be much smaller and Perl will parse it "
      "just the same. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'GENERATE PERLMOD' is set to YES");

   s_fullHelp.insert("PERL_PREFIX",
      "The names of the make variables in the generated doxyrules.make file are "
      "prefixed with the string contained in PERLMOD_MAKEVAR_PREFIX. This is useful "
      "so different doxyrules.make files included by the same Makefile don't "
      "overwrite each other's variables. "
      "<br><br>"
      "This tag requires the tag 'GENERATE PERLMOD' is set to YES");

   // Qt Help
   s_fullHelp.insert("GEN_QTHELP",
      "If the GENERATE_QHP tag is set to YES and both QHP_NAMESPACE and "
      "QHP_VIRTUAL_FOLDER are set, an additional index file will be generated that "
      "can be used as input for Qt's qhelpgenerator to generate a Qt Compressed Help "
      "(.qch) of the generated HTML documentation. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("QCH_FILE",
      "This tag is used to specify the .qch output file name of the Qt Help Generator. "
      "<br><br>"
      "This tag requires the tag 'GENERATE Qt HELP' is set to YES");

   s_fullHelp.insert("QHP_NAMESPACE",
      "This tag specifies the namespace to use when generating Qt Help output. "
      "<br><br>"
      "The default value is: org.doxypress.Project "
      "This tag requires the tag 'GENERATE Qt HELP' is set to YES");

   s_fullHelp.insert("QHP_VIRTUAL_FOLDER",
      "This tag specifies the virtual folder to use when generating Qt Help output. "
      "<br><br>"
      "The default value is: doc "
      "This tag requires the tag 'GENERATE Qt HELP' is set to YES");

   s_fullHelp.insert("QHP_CUST_FILTER_NAME",
      "This tag specifies the name of a custom filter for the Qt Help output. "
      "<br><br>"
      "This tag requires the tag 'GENERATE Qt HELP' is set to YES");

   s_fullHelp.insert("QHP_CUST_ATTRIB",
      "This tag specifies the list of attributes for the custom filter used by Qt Help. "
      "<br><br>"
      "This tag requires the tag 'GENERATE Qt HELP' is set to YES");

   s_fullHelp.insert("QHP_SECT_ATTRIB",
      "This tag specifies the list of filter section attributes used by Qt Help "
      "<br><br>"
      "This tag requires the tag 'GENERATE Qt HELP' is set to YES");

   s_fullHelp.insert("QTHELP_GEN_PATH",
      "This tag is used to specify the path for the Qt Help Generator. If this tag is empty "
      "DoxyPress will search for 'qhelpgenerator' in your path. "
      "<br><br>"
      "This tag requires the tag 'GENERATE Qt HELP' is set to YES");

   s_fullHelp.insert("GEN_ECLIPSE",
      "If the GENERATE_ECLIPSEHELP tag is set to YES, additional index files will be "
      "generated, together with the HTML files, they form an Eclipse help plugin. To "
      "install this plugin and make it available under the help contents menu in "
      "Eclipse, the contents of the directory containing the HTML and XML files needs "
      "to be copied into the plugins directory of eclipse. The name of the directory "
      "within the plugins directory should be the same as the ECLIPSE_DOC_ID value. "
      "After copying Eclipse needs to be restarted before the help appears. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("ECLIPSE_DOC_ID",
      "A unique identifier for the Eclipse help plugin. When installing the plugin "
      "the directory name containing the HTML and XML files should also have this "
      "name. Each documentation set should have its own identifier. "
      "<br><br>"
      "The default value is: org.doxypress.Project "
      "This tag requires the tag 'GENERATE ECLIPSEHELP' is set to YES");

   s_fullHelp.insert("DISABLE_INDEX",
      "If you want full control over the layout of the generated HTML pages it might "
      "be necessary to disable the index and replace it with your own. The "
      "DISABLE_INDEX tag can be used to turn on/off the condensed index (tabs) at top "
      "of each HTML page. A value of NO enables the index and the value YES disables "
      "it. Since the tabs in the index contain the same information as the navigation "
      "tree, you can set this option to YES if you also set GENERATE_TREEVIEW to YES. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("GEN_TREEVIEW",
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
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("TREEVIEW_WIDTH",
      "If the treeview is enabled (see GENERATE_TREEVIEW) then this tag can be used "
      "to set the initial width (in pixels) of the frame in which the tree is shown. "
      "<br><br>"
      "Minimum: 0, Maximum: 1500, Default: 250 "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("ENUM_VALUES_PER_LINE",
      "This tag is used to set the number of enum values  "
      "DoxyPress will group on one line in the generated HTML documentation. "
      "<br><br>"
      "Note: A value of 0 will completely suppress the enum values from appearing "
      "in the overview section. "
      "<br><br>"
      "Minimum: 0, Maximum: 20, Default: 4 "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("EXTERNAL_LINKS_IN_WINDOW",
      "If the EXT_LINKS_IN_WINDOW option is set to YES, DoxyPress will open links to "
      "external symbols imported via tag files in a separate window. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("HTML_SEARCH",
      "When this tag is set DoxyPress will generate a search box for the HTML output. "
      "The underlying search engine uses javascript and DHTML. HTML help, "
      "Qt Help, and docsets have there own search function. "
      "If this this search engine is too slow, try using the 'SERVER BASED SEARCH'. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("SERVER_BASED_SEARCH",
      "When the is set the search engine will be implemented using a web server instead "
      "of a web client using Javascript. There "
      "are two flavors of web server based searching depending on the EXTERNAL_SEARCH "
      "setting. When disabled, DoxyPress will generate a PHP script for searching and "
      "an index file used by the script. When EXTERNAL_SEARCH is enabled the indexing "
      "and searching needs to be provided by external tools. See the section "
      "\"External Indexing and Searching\" for details. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'SEARCH ENGINE' is set to YES");

   s_fullHelp.insert("EXTERNAL_SEARCH",
      "When this tag is enabled DoxyPress will not generate the PHP script for searching. "
      "The search results will be written to an XML file which must be processed by an external "
      "indexing system. DoxyPress will call the external search engine referenced in the "
      "tag 'SEARCH ENGINE URL' "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'SEARCH ENGINE' is set to YES");

   s_fullHelp.insert("SEARCH_ENGINE_URL",
      "This tag references a search engine hosted by a web server "
      "which will return the search results when 'EXTERNAL SEARCH' is enabled. "
      "DoxyPress ships with an example indexer (doxyindexer) and search engine "
      "(doxysearch.cgi) which are based on the open source search engine library "
      "Xapian (see: http://xapian.org/). "
      "<br><br>"
      "Refer to the section 'External Indexing and Searching' for details. "
      "This tag requires the tag 'SEARCH ENGINE is set to YES");

   s_fullHelp.insert("SEARCH_DATA_FILE",
      "When SERVER_BASED_SEARCH and EXTERNAL_SEARCH are both enabled the unindexed "
      "search data is written to a file for indexing by an external tool. With the "
      "SEARCHDATA_FILE tag the name of this file can be specified. "
      "<br><br>"
      "The default file is: searchdata.xml "
      "This tag requires the tag 'SEARCH ENGINE is set to YES");

   s_fullHelp.insert("EXTERNAL_SEARCH_ID",
      "When SERVER_BASED_SEARCH and EXTERNAL_SEARCH are both enabled the "
      "EXTERNAL_SEARCH_ID tag can be used as an identifier for the project. This is "
      "useful in combination with EXTRA_SEARCH_MAPPINGS to search through multiple "
      "projects and redirect the results back to the right project. "
      "<br><br>"
      "This tag requires the tag 'SEARCH ENGINE is set to YES");

   s_fullHelp.insert("EXTRA_SEARCH_MAPPINGS",
      "The EXTRA_SEARCH_MAPPINGS tag can be used to enable searching through DoxyPress "
      "projects other than the one defined by this configuration file, but that are "
      "all added to the same external search index. Each project needs to have a "
      "unique id set via EXTERNAL_SEARCH_ID. The search mapping then maps the id of "
      "to a relative location where the documentation can be found. The format is: "
      "EXTRA_SEARCH_MAPPINGS = tagname1=loc1 tagname2=loc2 ... "
      "<br><br>"
      "This tag requires the tag 'SEARCH ENGINE is set to YES");

   s_fullHelp.insert("FORMULA_FONTSIZE",
      "Use this tag to change the font size of LaTeX formulas included as images in "
      "the HTML documentation. When you change the font size after a successful "
      "DoxyPress run you need to manually remove any form_*.png images from the HTML "
      "output directory to force them to be regenerated. "
      "<br><br>"
      "Minimum: 8, Maximum: 50, Default: 10 "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("FORMULA_TRANSPARENT",
      "Use the FORMULA_TRANPARENT tag to determine whether or not the images "
      "generated for formulas are transparent PNGs. Transparent PNGs are not "
      "supported properly for IE 6.0, but are supported on all modern browsers. "
      "<br><br>"
      "Note: When changing this tag delete any files names form_*.png in "
      "the HTML output directory for this change to be effective. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("GHOSTSCRIPT",
      "This tag specifies the full path name for the GhostScript program. "
      "<br><br>"
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("USE_MATHJAX",
      "Enable the USE_MATHJAX option to render LaTeX formulas using MathJax (see "
      "http://www.mathjax.org) which uses client side Javascript for the rendering "
      "instead of using pre-rendered bitmaps. Use this if you do not have LaTeX "
      "installed or if you want to formulas look prettier in the HTML output. When "
      "enabled you may also need to install MathJax separately and configure the path "
      "to it using the MATHJAX_RELPATH option. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE HTML' is set to YES");

   s_fullHelp.insert("MATHJAX_FORMAT",
      "When MathJax is enabled you can set the default output format to be used for "
      "the MathJax output. See the MathJax site (see: "
      "http://docs.mathjax.org/en/latest/output.html) for more details. "
      "Possible values are: HTML-CSS (which is slower, but has the best "
      "compatibility), NativeMML (i.e. MathML) and SVG. "
      "<br><br>"
      "The default value is: HTML-CSS "
      "This tag requires the tag USE_MATHJAX is set to YES");

   s_fullHelp.insert("MATHJAX_RELPATH",
      "When MathJax is enabled you need to specify the location relative to the HTML "
      "output directory using the MATHJAX_RELPATH option. The destination directory "
      "should contain the MathJax.js script. For instance, if the mathjax directory "
      "is located at the same level as the HTML output directory, then "
      "MATHJAX_RELPATH should be ../mathjax. The default value points to the MathJax "
      "Content Delivery Network so you can quickly see the result without installing "
      "MathJax. However, it is strongly recommended to install a local copy of "
      "MathJax from http://www.mathjax.org before deployment. "
      "<br><br>"
      "The default value is: http://cdn.mathjax.org/mathjax/latest "                     
      "This tag requires the tag USE_MATHJAX is set to YES");

   s_fullHelp.insert("MATHJAX_EXTENSIONS",
      "The MATHJAX_EXTENSIONS tag can be used to specify one or more MathJax "
      "extension names that should be enabled during MathJax rendering. For example "
      "MATHJAX_EXTENSIONS = TeX/AMSmath TeX/AMSsymbols "
      "<br><br>"
      "This tag requires the tag USE_MATHJAX is set to YES");

   s_fullHelp.insert("MATHJAX_CODEFILE",
      "The MATHJAX_CODEFILE tag can be used to specify a file with javascript pieces "
      "of code that will be used on startup of the MathJax code. See the MathJax site "
      "(see: http://docs.mathjax.org/en/latest/output.html) for more details. For an "
      "example see the documentation. "
      "<br><br>"
      "This tag requires the tag USE_MATHJAX is set to YES");

   // latex
   s_fullHelp.insert("GEN_LATEX",
      "If the GENERATE_LATEX tag is set to YES, DoxyPress will generate LaTeX output. "
      "The default value is: YES");

   s_fullHelp.insert("LATEX_OUTPUT",                     
      "This tag is used to specify the location for the LaTeX output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will be prepended. "
      "<br><br>"
      "The default directory is: latex "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_CMD_NAME",
      "This tag is used to specify the LaTeX command name to be invoked. "
      "<br><br>"
      "Note: When enabling 'LATEX PDF' this option is only used for generating "
      "bitmaps for formulas in the HTML output, not in the Makefile which is "
      "written to the output directory. "
      "<br><br>"
      "The default file is: latex "
      "This tag requires the tag GENERATE LATEX' is set to YES");

   s_fullHelp.insert("MAKE_INDEX_CMD_NAME",
      "The MAKEINDEX_CMD_NAME tag can be used to specify the command name to generate "
      "index for LaTeX. "
      "<br><br>"
      "The default file is: makeindex "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_COMPACT",
      "If the COMPACT_LATEX tag is set to YES, DoxyPress generates more compact LaTeX "
      "documents. This may be useful for small projects and may help to save some "
      "trees in general. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_PAPERTYPE",
      "The PAPER_TYPE tag can be used to set the paper type that is used by the "
      "printer. Possible values are: a4 (210 x 297 mm), letter (8.5 x 11 inches), legal ("
      "8.5 x 14 inches) and executive (7.25 x 10.5 inches). "
      "<br><br>"
      "The default value is: a4 "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_EXTRA_PACKAGES",
      "This tag is used to specify one or more LaTeX package names which should be included in "
      "the LaTeX output. To get the times font for instance you can specify EXTRA_PACKAGES=times "
      "<br><br>"
      "If left blank no extra packages will be included. "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_HEADER",
      "The LATEX_HEADER tag can be used to specify a personal LaTeX header for the "
      "generated LaTeX document. The header should contain everything until the first "
      "chapter. If it is left blank DoxyPress will generate a standard header. "      
      "To create a new header file run 'DoxyPress --w latex-head [header file name]' "
      "<br><br>"
      "Note: Changing this header should be done with care. The "
      "following commands have a special meanings: $title, "
      "$datetime, $date, $DoxyPressversion, $projectname, $projectnumber, "
      "$projectbrief, $projectlogo. DoxyPress will replace $title with the empty "
      "string for the replacement values of the other commands the user is referred "
      "to HTML_HEADER. "
      "<br><br>"
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_FOOTER",
      "The LATEX_FOOTER tag can be used to specify a personal LaTeX footer for the "
      "generated LaTeX document. The footer should contain everything after the last "
      "chapter. If it is left blank DoxyPress will generate a standard footer. See "
      "LATEX_HEADER for more information on how to generate a default footer and what "
      "special commands can be used inside the footer. "
      "<br><br>"
      "Note: Changing this header should be done with care. "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_EXTRA_FILES",
      "The LATEX_EXTRA_FILES tag can be used to specify one or more extra images or "
      "other source files which should be copied to the LATEX_OUTPUT output "
      "directory. Note that the files will be copied as-is; there are no commands or "
      "markers available. "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_HYPER_PDF",
      "If the PDF_HYPERLINKS tag is set to YES, the LaTeX that is generated is "
      "prepared for conversion to PDF (using ps2pdf or pdflatex). The PDF file will "
      "contain links (just like the HTML output) instead of page references. This "
      "makes the output suitable for online browsing using a PDF viewer. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_PDF",
      "If this tag is set then DoxyPress will use 'pdf latex' to generate "
      "the PDF file directly from the LaTeX files. Set this option to YES, to get a "
      "higher quality PDF documentation. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_PS",
      ""
      "<br><br>"
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_BATCH_MODE",
      "If the LATEX_BATCHMODE tag is set to YES, DoxyPress will add the \\batchmode "
      "command to the generated LaTeX files. This will instruct LaTeX to keep running "
      "if errors occur, instead of asking the user for help. This option is also used "
      "when generating formulas in HTML. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_HIDE_INDICES",
      "If this tag is set then DoxyPress will not include the "
      "index chapters such as File Index, Compound Index, etc. in the output. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_SOURCE_CODE",
      "If this tag is set then DoxyPress will include source "
      "code with syntax highlighting in the LaTeX output. "
      "Note: Which sources are shown also depends on other settings such as 'SOURCE BROWSER'"
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("LATEX_BIB_STYLE",
      "This tag is used to specify the style to use for the "
      "bibliography, plainnat, or IEEEtranN, etc. "
      "Refer to the DoxyPress manual regarding \\cite "
      "<br><br>"
      "The default value is: plain "
      "This tag requires the tag 'GENERATE LATEX' is set to YES");

   s_fullHelp.insert("GEN_RTF",
      "If this tag is set then DoxyPress will generate RTF output. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("RTF_OUTPUT",                     
      "This tag is used to specify the location for the RTF output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will  be prepended. "
      "<br><br>"
      "The default directory is: rtf "
      "This tag requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_COMPACT",
      "If the COMPACT_RTF tag is set to YES, DoxyPress generates more compact RTF "
      "documents. This may be useful for small projects and may help to save some "
      "trees in general. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_HYPERLINKS",
      "If the RTF_HYPERLINKS tag is set to YES, the RTF that is generated will "
      "contain hyperlink fields. The RTF file will contain links (just like the HTML "
      "output) instead of page references. This makes the output suitable for online "
      "browsing using Word or some other Word compatible readers that support those "
      "fields. "
      "Note: WordPad (write) and others do not support links. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_STYLESHEET",
      "Load stylesheet definitions from file. Syntax is similar to DoxyPress's config "
      "file, i.e. a series of assignments. You only have to provide replacements, "
      "missing definitions are set to their default value. "
      "<br><br>"
      "See also section \"DoxyPress usage\" for information on how to generate the "
      "default style sheet that DoxyPress normally uses. "
      "<br><br>"
      "This tag requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("RTF_EXTENSION",
      "This tag is used to set optional variables used in the generation of an RTF document. "
      "A template extensions file can be generated by running: 'DoxyPress --w rtf-ext [extensions file name]' "
      "<br><br>"
      "This tag requires the tag 'GENERATE RTF' is set to YES");

   s_fullHelp.insert("GEN_MAN",
      "If this tag is set then DoxyPress will generate man pages for classes and files. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("MAN_OUTPUT",
      "This tag is used to specify the location for the man page output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will  be prepended. "
      "A directory man3 will be created inside the directory specified by this tag. "
      "<br><br>"
      "The default directory is: man "
      "This tag requires the tag 'GENERATE MAN' is set to YES");

   s_fullHelp.insert("MAN_EXTENSION",
      "The MAN_EXTENSION tag determines the extension that is added to the generated "
      "man pages. In case the manual section does not start with a number, the number "
      "3 is prepended. The dot (.) at the beginning of the MAN_EXTENSION tag is optional. "
      "<br><br>"
      "The default value is: .3 "
      "This tag requires the tag 'GENERATE MAN' is set to YES");

   s_fullHelp.insert("MAN_SUBDIR",
      "This tag determines the name of the directory created under the value for "
      "the 'MAN PAGE OUTPUT DIRECTORY' tag. "
      "The default directoy will be man/3  "
      "<br><br>"
      "This tag requires the tag 'GENERATE MAN' is set to YES");

   s_fullHelp.insert("MAN_LINKS",
      "If this tage is set then DoxyPress will generate one additional man file for"
      "each entity documented in the real man page(s). "
      "These additional files only source the real man page, but without "
      "them the man command would be unable to find the correct page. "
      "<br><br>"
      "The default value is: NO "
      "This tag requires the tag 'GENERATE MAN' is set to YES");

   s_fullHelp.insert("GEN_XML",
      "If this tag is set then DoxyPress will generate an XML file which "
      "captures the structure of the code including all documentation. "
      "<br><br>"
      "The default value is: NO");

   s_fullHelp.insert("XML_OUTPUT",
      "This tag is used to specify the location for the XML output. If a relative "
      "path is entered the value of 'OUTPUT DIRECTORY' will  be prepended. "
      "<br><br>"
      "The default directory is: xml "
      "This tag requires the tag 'GENERATE XML' is set to YES");

   s_fullHelp.insert("XML_PROGRAM_LISTING",
      "If this tage is set then DoxyPress will save the program "
      "listings (including syntax highlighting and cross-referencing information) to "
      "the XML output. Note: Enabling this tag will significantly increase the size "
      "of the XML output. "
      "<br><br>"
      "The default value is: YES "
      "This tag requires the tag 'GENERATE XML' is set to YES");  
}
