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

void MainWindow::convertDoxy(QByteArray data)
{
   QString tempStr;
   QString tempText;
   bool tempBool;
   int tempInt;

   // tab 1
   tempText = convert_PlainText(data,"PROJECT_NUMBER");
   m_ui->project_number->setText(tempText);

   tempText = convert_PlainText(data,"PROJECT_BRIEF");
   m_ui->project_brief->setText(tempText);

   tempStr = convert_Str(data,"PROJECT_NUMBER");
   m_ui->project_number->setText(tempStr);

   m_project_iconFN = convert_Str(data,"PROJECT_LOGO");

   tempStr = convert_Str(data,"OUTPUT_DIRECTORY");
   m_ui->output_dir->setText(tempStr);


/*
   tempBool = convert_Bool(data,"optimize-cplus");
   m_ui->optimize_cplus_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"optimize-cli");
   m_ui->optimize_cli_CB->setChecked(tempBool);
*/


   tempBool = convert_Bool(data,"OPTIMIZE_OUTPUT_JAVA");
   m_ui->optimize_java_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"OPTIMIZE_OUTPUT_FOR_C");
   m_ui->optimize_c_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"OPTIMIZE_FOR_FORTRAN");
   m_ui->optimize_fortran_CB->setChecked(tempBool);

   //
   tempBool = convert_Bool(data,"GENERATE_HTML");
   m_ui->gen_html_CB1->setChecked(tempBool);


/*
   tempBool = convert_Bool(data,"html-plain");
   m_ui->html_plain_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"html-nav");
   m_ui->html_nav_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"html-chm");
   m_ui->html_chm_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"search-enabled");
   m_ui->searchEnabled_CB->setChecked(tempBool);
*/


   //
   tempBool = convert_Bool(data,"GENERATE_LATEX");
   m_ui->gen_latex_CB1->setChecked(tempBool);


/*
   tempBool = convert_Bool(data,"latex-hyper-pdf");
   m_ui->latex_hyper_pdf_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"latex-pdf");
   m_ui->latex_pdf_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"latex-ps");
   m_ui->latex_ps_RB->setChecked(tempBool);
*/

   //
   tempBool = convert_Bool(data,"GENERATE_RTF");;
   m_ui->gen_rtf_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_MAN");
   m_ui->gen_man_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_XML");
   m_ui->gen_xml_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_DOCBOOK");
   m_ui->gen_docbook_CB1->setChecked(tempBool);



// match the following wizard fields with expert->dot, use "2"

   tempBool = convert_Bool(data,"diagram-none");
   m_ui->diagram_none_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"diagram-built-in");
   m_ui->diagram_built_in_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"diagram-dot");
   m_ui->diagram_dot_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"CLASS_DIAGRAMS");
   m_ui->dot_class_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"COLLABORATION_GRAPH");
   m_ui->dot_collaboration_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"dot-overall");
   m_ui->dot_overall_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"INCLUDE_GRAPH");
   m_ui->dot_include_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"INCLUDED_BY_GRAPH");
   m_ui->dot_included_by_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"CALL_GRAPH");
   m_ui->dot_call_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"CALLER_GRAPH");
   m_ui->dot_called_by_CB->setChecked(tempBool);


   //  ***
   // tab 2 - project
   tempStr = convert_Str(data,"PROJECT_ENCODING");
   m_ui->project_encoding->setText(tempStr);


// --> missing lines


   // tab 2 - build


// --> missing lines


   // tab 2 - messages
   tempBool = convert_Bool(data, "QUIET");
   m_ui->quiet_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARNINGS");
   m_ui->warnings_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARN_IF_UNDOCUMENTED");
   m_ui->warn_undoc_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARN_IF_DOC_ERROR");
   m_ui->warn_doc_error_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARN_NO_PARAMDOC");
   m_ui->warn_undoc_parm_CB->setChecked(tempBool);

   tempStr = convert_Str(data,"WARN_FORMAT");
   m_ui->warn_forrmat->setText(tempStr);

   tempStr = convert_Str(data,"WARN_LOGFILE");
   m_ui->warn_logfile->setText(tempStr);

   // tab 2 -input

// --> missing lines

   // tab 2 -browser

// --> missing lines


   /*
      tempBool = convert_Bool(data, "");
      tempBool);

      tempInt = convert_Int(data, "");
      tempInt);

      tempStr = convert_Str(data, "");
      tempStr);

      tempText = convert_PlainText(data,"");
      tempText);
   */


   // tab 2 -index
   tempBool = convert_Bool(data, "ALPHABETICAL_INDEX");
   m_ui->alpha_index_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "COLS_IN_ALPHA_INDEX");
   m_ui->cols_in_index_SB->setValue(tempInt);

   tempText = convert_PlainText(data,"IGNORE_PREFIX");
   m_ui->ignore_prefix->setPlainText(tempText);

   // tab 2 - autogen
   tempBool = convert_Bool(data, "GENERATE_AUTOGEN_DEF");
   m_ui->gen_autogen_def_CB->setChecked(tempBool);

   // tab 2 - perlmod
   tempBool = convert_Bool(data, "GENERATE_PERLMOD");
   m_ui->gen_perl_CB->setChecked(tempBool);;

   tempBool = convert_Bool(data, "PERLMOD_LATEX");
   m_ui->perl_latex_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "PERLMOD_PRETTY");
   m_ui->perl_pretty_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "PERLMOD_MAKEVAR_PREFIX");
   m_ui->perlmod_prefix->setText(tempStr);

   // tab 2 - preprocess
   tempBool = convert_Bool(data, "ENABLE_PREPROCESSING");
   m_ui->enable_preprocessing_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "MACRO_EXPANSION");
   m_ui->macro_expansion_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXPAND_ONLY_PREDEF");
   m_ui->expand_only_predefined_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SEARCH_INCLUDES");
   m_ui->search_includes_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"INCLUDE_PATH");
   m_ui->include_path->setPlainText(tempText);

   tempText = convert_PlainText(data,"INCLUDE_FILE_PATTERNS");
   m_ui->include_file_patterns->setPlainText(tempText);

   tempText = convert_PlainText(data,"PREDEFINED");
   m_ui->predefined_macros->setPlainText(tempText);

   tempText = convert_PlainText(data,"EXPAND_AS_DEFINED");
   m_ui->expand_as_defined->setPlainText(tempText);

   tempBool = convert_Bool(data, "SKIP_FUNCTION_MACROS");
   m_ui->skip_function_macros_CB->setChecked(tempBool);

   // tab 2 - external

// --> missing lines


   // tab 2 - dot

// --> missing lines


   //  ***
   // tab 3 - html

// --> missing lines


   // tab 3 - latex

// --> missing lines


   // tab 3 - rtf
// --> missing lines



   // tab 3 - man

// --> missing lines


   // tab 3 - xml

// --> missing lines


   // tab 3 - docbook
// --> missing lines




   tempBool = convert_Bool(data,"GENERATE_DOCSET");
   m_ui->gen_docbook_CB1->setChecked(tempBool);

   tempStr = convert_Str(data,"INPUT");
   m_ui->input_source->setPlainText(tempStr);

   tempBool = convert_Bool(data,"RECURSIVE");
   m_ui->source_recursive_CB->setChecked(tempBool);
}


/*

// enum  changes to a string

CREATE_SUBDIRS         = NO
ALLOW_UNICODE_NAMES    = NO
OUTPUT_LANGUAGE        = English
BRIEF_MEMBER_DESC      = YES
REPEAT_BRIEF           = YES
ABBREVIATE_BRIEF       = "The $name class" \
ALWAYS_DETAILED_SEC    = NO
INLINE_INHERITED_MEMB  = NO
FULL_PATH_NAMES        = YES
STRIP_FROM_PATH        = 
STRIP_FROM_INC_PATH    = 
SHORT_NAMES            = NO
JAVADOC_AUTOBRIEF      = NO
QT_AUTOBRIEF           = NO
MULTILINE_CPP_IS_BRIEF = NO
INHERIT_DOCS           = YESEeeee
SEPARATE_MEMBER_PAGES  = NO
TAB_SIZE               = 4
ALIASES                = 
TCL_SUBST              = 
EXTENSION_MAPPING      = 
MARKDOWN_SUPPORT       = YES
AUTOLINK_SUPPORT       = YES
BUILTIN_STL_SUPPORT    = NO
CPP_CLI_SUPPORT        = NO
SIP_SUPPORT            = NO
IDL_PROPERTY_SUPPORT   = YES
DISTRIBUTE_GROUP_DOC   = NO
SUBGROUPING            = YES
INLINE_GROUPED_CLASSES = NO
INLINE_SIMPLE_STRUCTS  = NO
TYPEDEF_HIDES_STRUCT   = NO
LOOKUP_CACHE_SIZE      = 0
EXTRACT_ALL            = NO
EXTRACT_PRIVATE        = NO
EXTRACT_PACKAGE        = NO
EXTRACT_STATIC         = NO
EXTRACT_LOCAL_CLASSES  = YES
EXTRACT_LOCAL_METHODS  = NO
EXTRACT_ANON_NSPACES   = NO
HIDE_UNDOC_MEMBERS     = NO
HIDE_UNDOC_CLASSES     = NO
HIDE_FRIEND_COMPOUNDS  = NO
HIDE_IN_BODY_DOCS      = NO
INTERNAL_DOCS          = NO
CASE_SENSE_NAMES       = NO
HIDE_SCOPE_NAMES       = NO
SHOW_INCLUDE_FILES     = YES
SHOW_GROUPED_MEMB_INC  = NO
FORCE_LOCAL_INCLUDES   = NO
INLINE_INFO            = YES
SORT_MEMBER_DOCS       = YES
SORT_BRIEF_DOCS        = NO
SORT_MEMBERS_CTORS_1ST = NO
SORT_GROUP_NAMES       = NO
SORT_BY_SCOPE_NAME     = NO
STRICT_PROTO_MATCHING  = NO
GENERATE_TODOLIST      = YES
GENERATE_TESTLIST      = YES
GENERATE_BUGLIST       = YES
GENERATE_DEPRECATEDLIST= YES
ENABLED_SECTIONS       = 
MAX_INITIALIZER_LINES  = 30
SHOW_USED_FILES        = YES
SHOW_FILES             = YES
SHOW_NAMESPACES        = YES
FILE_VERSION_FILTER    = 
LAYOUT_FILE            = 
CITE_BIB_FILES         = 
INPUT                  = Input
INPUT_ENCODING         = UTF-8
FILE_PATTERNS          = *.c 
RECURSIVE              = NO
EXCLUDE                = 
EXCLUDE_SYMLINKS       = NO
EXCLUDE_PATTERNS       = 
EXCLUDE_SYMBOLS        = 
EXAMPLE_PATH           = 
EXAMPLE_PATTERNS       = *
EXAMPLE_RECURSIVE      = NO
IMAGE_PATH             = 
INPUT_FILTER           = 
FILTER_PATTERNS        = 
FILTER_SOURCE_FILES    = NO
FILTER_SOURCE_PATTERNS = 
USE_MDFILE_AS_MAINPAGE = 
SOURCE_BROWSER         = NO
INLINE_SOURCES         = NO
STRIP_CODE_COMMENTS    = YES
REFERENCED_BY_RELATION = NO
REFERENCES_RELATION    = NO
REFERENCES_LINK_SOURCE = YES
SOURCE_TOOLTIPS        = YES
USE_HTAGS              = NO
VERBATIM_HEADERS       = YES
CLANG_ASSISTED_PARSING = NO
CLANG_OPTIONS          =

HTML_OUTPUT            = html
HTML_FILE_EXTENSION    = .html
HTML_HEADER            = 
HTML_FOOTER            = 
HTML_STYLESHEET        = 
HTML_EXTRA_STYLESHEET  = 
HTML_EXTRA_FILES       = 
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100
HTML_COLORSTYLE_GAMMA  = 80
HTML_TIMESTAMP         = YES
HTML_DYNAMIC_SECTIONS  = NO
HTML_INDEX_NUM_ENTRIES = 100
GENERATE_DOCSET        = NO
DOCSET_FEEDNAME        = "Doxygen generated docs"
DOCSET_BUNDLE_ID       = org.doxygen.Project
DOCSET_PUBLISHER_ID    = org.doxygen.Publisher
DOCSET_PUBLISHER_NAME  = Publisher
GENERATE_HTMLHELP      = NO
CHM_FILE               = 
HHC_LOCATION           = 
GENERATE_CHI           = NO
CHM_INDEX_ENCODING     = 
BINARY_TOC             = NO
TOC_EXPAND             = NO
GENERATE_QHP           = NO
QCH_FILE               = 
QHP_NAMESPACE          = org.doxygen.Project
QHP_VIRTUAL_FOLDER     = doc
QHP_CUST_FILTER_NAME   = 
QHP_CUST_FILTER_ATTRS  = 
QHP_SECT_FILTER_ATTRS  = 
QHG_LOCATION           = 
GENERATE_ECLIPSEHELP   = NO
ECLIPSE_DOC_ID         = org.doxygen.Project
DISABLE_INDEX          = NO
GENERATE_TREEVIEW      = NO
ENUM_VALUES_PER_LINE   = 4
TREEVIEW_WIDTH         = 250
EXT_LINKS_IN_WINDOW    = NO
FORMULA_FONTSIZE       = 10
FORMULA_TRANSPARENT    = YES
USE_MATHJAX            = NO
MATHJAX_FORMAT         = HTML-CSS
MATHJAX_RELPATH        = http://cdn.mathjax.org/mathjax/latest
MATHJAX_EXTENSIONS     = 
MATHJAX_CODEFILE       = 
SEARCHENGINE           = YES
SERVER_BASED_SEARCH    = NO
EXTERNAL_SEARCH        = NO
SEARCHENGINE_URL       = 
SEARCHDATA_FILE        = searchdata.xml
EXTERNAL_SEARCH_ID     = 
EXTRA_SEARCH_MAPPINGS  = 
LATEX_OUTPUT           = latex
LATEX_CMD_NAME         = latex
MAKEINDEX_CMD_NAME     = makeindex
COMPACT_LATEX          = NO
PAPER_TYPE             = a4
EXTRA_PACKAGES         = 
LATEX_HEADER           = 
LATEX_FOOTER           = 
LATEX_EXTRA_FILES      = 
PDF_HYPERLINKS         = YES
USE_PDFLATEX           = YES
LATEX_BATCHMODE        = NO
LATEX_HIDE_INDICES     = NO
LATEX_SOURCE_CODE      = NO
LATEX_BIB_STYLE        = plain   
RTF_OUTPUT             = rtf
COMPACT_RTF            = NO

RTF_HYPERLINKS         = NO
RTF_STYLESHEET_FILE    = 
RTF_EXTENSIONS_FILE    =    

MAN_OUTPUT             = man
MAN_EXTENSION          = .3
MAN_SUBDIR             = 
MAN_LINKS              = NO
XML_OUTPUT             = xml
XML_PROGRAMLISTING     = YES
DOCBOOK_OUTPUT         = docbook
DOCBOOK_PROGRAMLISTING = NO

TAGFILES               = 
GENERATE_TAGFILE       = 
ALLEXTERNALS           = NO
EXTERNAL_GROUPS        = YES
EXTERNAL_PAGES         = YES
PERL_PATH              = /usr/bin/perl

MSCGEN_PATH            = 
DIA_PATH               = 
HIDE_UNDOC_RELATIONS   = YES
HAVE_DOT               = NO
DOT_NUM_THREADS        = 0
DOT_FONTNAME           = Helvetica
DOT_FONTSIZE           = 10
DOT_FONTPATH           = 
CLASS_GRAPH            = YES

GROUP_GRAPHS           = YES
UML_LOOK               = NO
UML_LIMIT_NUM_FIELDS   = 10
TEMPLATE_RELATIONS     = NO

GRAPHICAL_HIERARCHY    = YES
DIRECTORY_GRAPH        = YES
DOT_IMAGE_FORMAT       = png
INTERACTIVE_SVG        = NO
DOT_PATH               = 
DOTFILE_DIRS           = 
MSCFILE_DIRS           = 
DIAFILE_DIRS           = 
PLANTUML_JAR_PATH      = 
DOT_GRAPH_MAX_NODES    = 50
MAX_DOT_GRAPH_DEPTH    = 0
DOT_TRANSPARENT        = NO
DOT_MULTI_TARGETS      = NO
GENERATE_LEGEND        = YES
DOT_CLEANUP            = YES

*/


bool MainWindow::convert_Bool(QByteArray data, QString key)
{
   bool retval = false;
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();

      if (tempStr == "YES") {
         retval = true;
      }
   }

   return retval;
}

int MainWindow::convert_Int(QByteArray data, QString key)
{
   int retval;
   QString temp;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      temp = data.mid(posBeg, posEnd - posBeg);

      retval = temp.toInt();
   }

   return retval;
}

QString MainWindow::convert_Str(QByteArray data, QString key)
{
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();
   }

   return tempStr;
}

QString MainWindow::convert_PlainText(QByteArray data, QString key)
{
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1);

      tempStr.replace("\"","");
      tempStr = tempStr.trimmed();
   }

   return tempStr;
}
