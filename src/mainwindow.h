/*************************************************************************
 *
 * Copyright (C) 2014-2016 Barbara Geller & Ansel Sermersheim
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <syntax.h>
#include "ui_mainwindow.h"
#include "util.h"

#include <QAction>
#include <QApplication>
#include <QByteArray>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QColor>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QGroupBox>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProcess>
#include <QRadioButton>
#include <QSettings>
#include <QSpinBox>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTextEdit>
#include <QTextStream>
#include <QWidget>

static const int DOXYPRESS_FORMAT = 1;
static const int RECENT_FILES_MAX = 10;

enum Validate {
   MODIFIED,
   DEFAULT
};

enum RelativeX {
   PROJECT_DIR,
   OUTPUT_DIR,
   ABSOLUTE_ONLY
};

struct Args {
   bool blank_layout;  
   bool setDateTime;

   QString layout; 
   QString dateTime;

   QString style_html_header;
   QString style_html_footer;
   QString style_html_css;
};

struct LookUpInfo {
   QString title;   
   QStringList dataList;
   QString path;
   QString relPath;
   bool isFolderPB;
   bool isFilePB;
   enum RelativeX relativeTo;
};

struct HelpData {     

   enum DefType {
      BOOL,
      INT,
      STRING
   };

   HelpData(DefType x_type, QLabel *x_label, QString x_body)
      : type(x_type), label(x_label), body(x_body)
   {
      defBool = false;
      defInt  = 0;
   }

   DefType type;
   QString defValue;
   bool defBool;
   int defInt;

   QLabel *label;
   QString title;
   QString body;
};

struct Settings {
   QFont   fontNormal;
   QColor  colorText;
   QString pathPrior;
   QString doxyPressExe;
};

class MainWindow : public QMainWindow
{
   CS_OBJECT(MainWindow)

   public:
      MainWindow();

      QString m_appPath;
      QString m_jsonFname;
      int m_doxypressFormat;

      struct Settings m_settings;
      struct Args m_args;

      QString pathName(QString fileName) const;
      bool openDoxy_Internal(const QString fname);

      void setStatusBar(QString msg);
      void saveSettings();
      struct Settings get_StructData();
      void setDoxyTitle(bool isModified);

      QSize sizeHint() const;

      QString get_DirPath(QString message, QString path, enum RelativeX relativeTo);
      QString getSingleFile(QString title, QString fname, QString filter = "");

      void autoConvert(QString fromFile, QString toFile);

   protected:
      void closeEvent(QCloseEvent *event);

   private:
      Ui::MainWindow *m_ui;

      Syntax *m_syntaxParser;

      QString m_curFile;
      QStringList m_openedFiles;

      enum Config { CFG_STARTUP, CFG_DEFAULT };
      enum Option { ABOUTURL, CLOSE, DOXYPRESS_EXE, PATH_PRIOR, RECENTFILE };

      void clearAllFields();
      void convertDoxy(QByteArray data);

      bool convert_Bool(QByteArray data, QString key);
      int convert_Int(QByteArray data, QString key);
      QString convert_Str(QByteArray data, QString key);
      QString convert_PlainText(QByteArray data, QString key);

      void createShortCuts();
      void createConnections();
      void setDuplicates();
      void setupLimits();

      bool querySave();
      bool saveDoxy_Internal();

      // json
      bool json_Read(Config trail = CFG_DEFAULT);
      bool json_Write(Option route, Config trail = CFG_DEFAULT);
      void json_getFileName();
      bool json_CreateNew();
      bool json_SaveFile(QByteArray route);

      QByteArray json_ReadFile();
      void save_ConfigFile();

      bool json_OpenDoxy(QByteArray data);
      QByteArray json_SaveDoxy();
      QString getDataList(QJsonObject &object, QString fieldData);
      QJsonArray putDataList(QString fieldData);

      // recent files
      QAction *rf_Actions[RECENT_FILES_MAX];
      QStringList m_rf_List;

      void rf_CreateMenus();
      void rf_Update();
      void rf_UpdateActions();   

      QProcess *m_runProcess;
      QTimer *m_timer;
      bool m_running;
      bool m_modified;

      void runText_Append(const QString &text);
      QString getHtmlOutputIndex() const;
      bool htmlOutputPresent() const;
      void updateRunButtons();

      // help and label text color
      static QMap<QWidget *, HelpData> m_bigMap;

      static const QString m_filePatterns;
      static const QString m_suffixSource;
      static const QString m_suffixHeader;
      static const QString m_suffixExclude;

      bool getDefault(QCheckBox *name);
      QString getDefault(QWidget *name);
      int getDefault(QSpinBox *name);

      QLabel *getLabel(QWidget *name);
      void setLabelColor(int option, QWidget *label);

      void setDefault(QCheckBox *name);
      void setDefault(QComboBox *name);
      void setDefault(QGroupBox *name);
      void setDefault(QLineEdit *name);
      void setDefault(QPlainTextEdit *name);
      void setDefault(QSpinBox *name);

      void setHelpText(QWidget *name);
      QString getHelpBody(QWidget *name);
      QString getHelpTitle(QWidget *name);
      void createMap();
      void configFullHelp();

      bool eventFilter(QObject *object, QEvent *event);
      void hoverChanged(QWidget *old_Widget, QWidget *new_Widget);

      // menu options
      void newDoxy();
      void openDoxy();
      void reloadDoxy();

      bool saveDoxy();
      bool saveDoxyAs();

      void importDoxy();
      void edit_Cfg();
      void move_Cfg();
      void save_Cfg();

      void manual();
      void about();

      // tabs
      void setupPage(QTreeWidgetItem *item, QTreeWidgetItem *);
      void buildPage(QTreeWidgetItem *item, QTreeWidgetItem *);
      void outputPage(QTreeWidgetItem *item, QTreeWidgetItem *);

      // menu recent files
      void showContext_Files(const QPoint &pt);

      void rf_Open();    
      void rf_DeleteName();

      CS_SLOT_1(Private, void rf_ClearList())
      CS_SLOT_2(rf_ClearList)

      CS_SLOT_1(Private, void rf_RemoveFName())
      CS_SLOT_2(rf_RemoveFName)

      void adjustDefaults();
      void finalLoad();
      void validGet_html();
      void validGet_latex();
      void validGet_dot();

      // tab 1
      void valid_output_dir();
      void getLogo(const QString route);

      void valid_gen_html_1(bool checked);
      void valid_gen_docbook_1(bool checked);
      void valid_gen_latex_1(bool checked);
      void valid_gen_man_1(bool checked);
      void valid_gen_rtf_1(bool checked);
      void valid_gen_xml_1(bool checked);

      void validSet_html(QAbstractButton *);
      void validSet_latex(QAbstractButton *);
      void validSet_dot(QAbstractButton *);

      // tab 2
      void valid_full_path_names();
      void valid_filter_source_files();
      void valid_alpha_index();
      void valid_enable_preprocessing();
      void valid_have_dot();

      // tab 3
      void valid_gen_html();
      void valid_gen_chm();
      void valid_gen_docbook();
      void valid_gen_docset();
      void valid_gen_eclipse();
      void valid_gen_latex();
      void valid_gen_man();
      void valid_gen_qthelp();
      void valid_gen_rtf();
      void valid_gen_xml();
      void valid_html_search();

      // ** lookups
      // tab 1
      void output_dir_PB();
      void tune_colors_PB();

      // tab 2 - project config
      void abbreviate_brief_PB();
      void strip_from_path_PB();
      void strip_from_inc_path_PB();
      void aliases_PB();
      void tcl_subst_PB();
      void language_mapping_PB();

      // tab 2- build
      void enabled_sections_PB();
      void file_version_filter_PB();
      void main_page_name_PB();
      void layout_file_PB();
      void ns_alias_PB();

      // tab 2- input
      void input_source_PB();
      void input_patterns_PB();
      void exclude_files_PB();
      void exclude_patterns_PB();
      void exclude_symbols_PB();
      void example_source_PB();
      void example_patterns_PB();
      void image_path_PB();
      void filter_program_PB();
      void filter_patterns_PB();
      void filter_source_patterns_PB();

      // tab 2- index
      void ignore_prefix_PB();

      // tab 2- messages
      void warn_logfile_PB();

      // tab 2- source code
      void valid_source_code();
      void suffix_source_navtree_PB();
      void suffix_header_navtree_PB();
      void suffix_exclude_navtree_PB();
      void clang_options_PB();

      // tab 2- preprocess
      void include_path_PB();
      void include_file_patterns_PB();
      void predefined_macros_PB();
      void expand_as_defined_PB();

      // tab 2- external
      void tag_files_PB();
      void gen_tagfile_PB();
      void perl_path_PB();

      // tab 2- dot
      void mscgen_path_PB();
      void dia_path_PB();
      void dot_font_name_PB();
      void dot_font_path_PB();
      void dot_path_PB();
      void dot_file_dirs_PB();
      void msc_file_dirs_PB();
      void dia_file_dirs_PB();
      void plantuml_jar_path_PB();
      void plantuml_inc_path_PB();

      // tab 3 html
      void html_output_PB();
      void html_header_PB();
      void html_footer_PB();
      void html_stylesheets_PB();
      void html_extra_files_PB();

      void ghostscript_PB();
      void mathjax_extensions_PB();
      void search_data_file_PB();
      void search_mappings_PB();

      // tab 3 chm
      void chm_file_PB();
      void hhc_location_PB();

      // tab 3 docbook
      void docbook_output_PB();

      // tab 3 latex
      void latex_output_PB();
      void latex_cmd_name_PB();
      void make_index_cmd_name_PB();

      void latex_extra_packages_PB();
      void latex_header_PB();
      void latex_footer_PB();
      void latex_stylesheets_PB();
      void latex_extra_files_PB();
      void cite_bib_files_PB();

      // tab 3 man
      void man_output_PB();

      // tab 3 qthelp
      void qch_file_PB();
      void qhp_cust_attrib_PB();
      void qhp_sect_attrib_PB();
      void qthelp_gen_path_PB();

      // tab 3 rtf
      void rtf_output_PB();
      void rtf_stylesheet_PB();
      void rtf_extension_PB();

      // tab 3 xml
      void xml_output_PB();

      // tab 4
      void runDoxyPress();
      void setArgs();
      void showHtmlOutput();
      void clearOutput();
      void saveLog();

      void readStdout();
      void runComplete();
};

#endif
