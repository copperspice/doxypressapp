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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "util.h"

#include <QAction>
#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QColor>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTextEdit>
#include <QTextStream>
#include <QWidget>

static const int RECENT_FILES_MAX = 10;

struct Settings {
   QFont   fontNormal;
   QColor  colorText;
   QString pathPrior;
};

struct LookUpInfo {
   QString title;   
   QStringList dataList;
   QString path;
   QString relPath;
   bool isFolderPB;
   bool isFilePB;
};

struct HelpData {
   QLabel *label;
   QString defValue;
   QString title;
   QString body;
};

enum Validate{
   MODIFIED,
   DEFAULT
};

class MainWindow : public QMainWindow
{
   CS_OBJECT(MainWindow)

   public:
      MainWindow();

      QString m_appPath;
      QString m_jsonFname;

      QString m_project_iconFN;

      struct Settings m_struct;

      QString pathName(QString fileName) const;

      bool openDoxy_Internal(const QString fname);

      void setStatusBar(QString msg);
      void setStatusBar(QString msg, int timeOut);

      void saveSettings();
      struct Settings get_StructData();
      void setDoxyTitle(bool isModified);

      QSize sizeHint() const;

      QString get_DirPath(QString message, QString path, QString relativePath = "", bool isOutputDir = false);
      QString getSingleFile(QString title, QString fname, QString filter = "");

   protected:
      void closeEvent(QCloseEvent *event);

   private:
      Ui::MainWindow *m_ui;

      QString m_curFile;
      QStringList m_openedFiles;

      enum Config { CFG_STARTUP, CFG_DEFAULT };
      enum Option { ABOUTURL, CLOSE, PATH_PRIOR, RECENTFILE };

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
      void saveDoxy_Internal();

      // json
      bool json_Read(Config trail = CFG_DEFAULT);
      bool json_Write(Option route, Config trail = CFG_DEFAULT);
      void json_getFileName();
      bool json_CreateNew();
      bool json_SaveFile(QByteArray route);

      QByteArray json_ReadFile();
      void save_ConfigFile();

      void json_OpenDoxy(QByteArray data);
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

      QString getHtmlOutputIndex() const;
      bool htmlOutputPresent() const;
      void updateRunButtons();

      // help and label text color
      static QMap<QWidget *, HelpData> m_bigMap;
      static const QString m_filePatterns;

      QString getDefault(QWidget *name);

      QLabel *getLabel(QWidget *name);
      void setLabelColor(int option, QWidget *label);

      void setHelpText(QWidget *name);
      QString getHelpBody(QWidget *name);
      QString getHelpTitle(QWidget *name);
      void createMap();

      bool eventFilter(QObject *object, QEvent *event);
      void hoverChanged(QWidget *old_Widget, QWidget *new_Widget);

      // menu options
      CS_SLOT_1(Private, void newDoxy())
      CS_SLOT_2(newDoxy)

      CS_SLOT_1(Private, void openDoxy())
      CS_SLOT_2(openDoxy)

      CS_SLOT_1(Private, void reloadDoxy())
      CS_SLOT_2(reloadDoxy)

      CS_SLOT_1(Private, void saveDoxy())
      CS_SLOT_2(saveDoxy)

      CS_SLOT_1(Private, bool saveDoxyAs())
      CS_SLOT_2(saveDoxyAs)

      CS_SLOT_1(Private, void importDoxy())
      CS_SLOT_2(importDoxy)

      CS_SLOT_1(Private, void move_Settings())
      CS_SLOT_2(move_Settings)

      CS_SLOT_1(Private, void save_Settings())
      CS_SLOT_2(save_Settings)

      // recent files
      CS_SLOT_1(Private, void showContext_Files(const QPoint &pt))
      CS_SLOT_2(showContext_Files)

      CS_SLOT_1(Private, void rf_Open())
      CS_SLOT_2(rf_Open)

      CS_SLOT_1(Private, void rf_ClearList())
      CS_SLOT_2(rf_ClearList)

      CS_SLOT_1(Private, void rf_DeleteName())
      CS_SLOT_2(rf_DeleteName)

      CS_SLOT_1(Private, void rf_RemoveFName())
      CS_SLOT_2(rf_RemoveFName)

      CS_SLOT_1(Private, void manual())
      CS_SLOT_2(manual)

      CS_SLOT_1(Private, void about())
      CS_SLOT_2(about)

      // tabs
      CS_SLOT_1(Private, void setupPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(setupPage)

      CS_SLOT_1(Private, void buildPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(buildPage)

      CS_SLOT_1(Private, void outputPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(outputPage)

      // application
      CS_SLOT_1(Private, void configChanged())
      CS_SLOT_2(configChanged)

      // tab 1 - valid
      CS_SLOT_1(Private, void valid_output_dir())
      CS_SLOT_2(valid_output_dir)

      void finalLoad();
      void validGet_html();
      void validGet_latex();
      void validGet_dot();          

      CS_SLOT_1(Private, void validSet_html(QAbstractButton *))
      CS_SLOT_2(validSet_html)

      CS_SLOT_1(Private, void validSet_latex(QAbstractButton *))
      CS_SLOT_2(validSet_latex)

      CS_SLOT_1(Private, void validSet_dot(QAbstractButton *))
      CS_SLOT_2(validSet_dot)

      CS_SLOT_1(Private, void valid_gen_html_1(bool checked))
      CS_SLOT_2(valid_gen_html_1)

      CS_SLOT_1(Private, void valid_gen_docbook_1(bool checked))
      CS_SLOT_2(valid_gen_docbook_1)

      CS_SLOT_1(Private, void valid_gen_latex_1(bool checked))
      CS_SLOT_2(valid_gen_latex_1)

      CS_SLOT_1(Private, void valid_gen_man_1(bool checked))
      CS_SLOT_2(valid_gen_man_1)

      CS_SLOT_1(Private, void valid_gen_rtf_1(bool checked))
      CS_SLOT_2(valid_gen_rtf_1)

      CS_SLOT_1(Private, void valid_gen_xml_1(bool checked))
      CS_SLOT_2(valid_gen_xml_1)

      // tab 3 valids
      CS_SLOT_1(Private, void valid_gen_html())
      CS_SLOT_2(valid_gen_html)

      CS_SLOT_1(Private, void valid_gen_chm())
      CS_SLOT_2(valid_gen_chm)

      CS_SLOT_1(Private, void valid_gen_docbook())
      CS_SLOT_2(valid_gen_docbook)

      CS_SLOT_1(Private, void valid_gen_docset())
      CS_SLOT_2(valid_gen_docset)

      CS_SLOT_1(Private, void valid_gen_eclipse())
      CS_SLOT_2(valid_gen_eclipse)

      CS_SLOT_1(Private, void valid_gen_latex())
      CS_SLOT_2(valid_gen_latex)

      CS_SLOT_1(Private, void valid_gen_man())
      CS_SLOT_2(valid_gen_man)

      CS_SLOT_1(Private, void valid_gen_qthelp())
      CS_SLOT_2(valid_gen_qthelp)

      CS_SLOT_1(Private, void valid_gen_rtf())
      CS_SLOT_2(valid_gen_rtf)

      CS_SLOT_1(Private, void valid_gen_xml())
      CS_SLOT_2(valid_gen_xml)

      CS_SLOT_1(Private, void valid_search_engine())
      CS_SLOT_2(valid_search_engine)

      // tab 1 - look up
      CS_SLOT_1(Private, void getIcon(const QString route = ""))
      CS_SLOT_2(getIcon)

      CS_SLOT_1(Private, void output_dir_PB())
      CS_SLOT_2(output_dir_PB)

      CS_SLOT_1(Private, void tune_colors_PB())
      CS_SLOT_2(tune_colors_PB)

      // tab 2- look up (general)
      CS_SLOT_1(Private, void abbreviate_brief_PB())
      CS_SLOT_2(abbreviate_brief_PB)

      CS_SLOT_1(Private, void strip_from_path_PB())
      CS_SLOT_2(strip_from_path_PB)

      CS_SLOT_1(Private, void strip_from_inc_path_PB())
      CS_SLOT_2(strip_from_inc_path_PB)

      CS_SLOT_1(Private, void aliases_PB())
      CS_SLOT_2(aliases_PB)

      CS_SLOT_1(Private, void tcl_subst_PB())
      CS_SLOT_2(tcl_subst_PB)

      CS_SLOT_1(Private, void extension_mapping_PB())
      CS_SLOT_2(extension_mapping_PB)

      // tab 2- look up (build)
      CS_SLOT_1(Private, void enabled_sections_PB())
      CS_SLOT_2(enabled_sections_PB)

      CS_SLOT_1(Private, void file_version_filter_PB())
      CS_SLOT_2(file_version_filter_PB)

      CS_SLOT_1(Private, void layout_file_PB())
      CS_SLOT_2(layout_file_PB)

      CS_SLOT_1(Private, void cite_bib_files_PB())
      CS_SLOT_2(cite_bib_files_PB)

      // tab 2- look up (messages)
      CS_SLOT_1(Private, void warn_logfile_PB())
      CS_SLOT_2(warn_logfile_PB)

      // tab 2- look up (input)
      CS_SLOT_1(Private, void input_source_PB())
      CS_SLOT_2(input_source_PB)

      CS_SLOT_1(Private, void file_patterns_PB())
      CS_SLOT_2(file_patterns_PB)

      CS_SLOT_1(Private, void exclude_files_PB())
      CS_SLOT_2(exclude_files_PB)

      CS_SLOT_1(Private, void exclude_patterns_PB())
      CS_SLOT_2(exclude_patterns_PB)

      CS_SLOT_1(Private, void exclude_symbols_PB())
      CS_SLOT_2(exclude_symbols_PB)

      CS_SLOT_1(Private, void example_source_PB())
      CS_SLOT_2(example_source_PB)

      CS_SLOT_1(Private, void example_patterns_PB())
      CS_SLOT_2(example_patterns_PB)

      CS_SLOT_1(Private, void image_path_PB())
      CS_SLOT_2(image_path_PB)

      CS_SLOT_1(Private, void input_filter_PB())
      CS_SLOT_2(input_filter_PB)

      CS_SLOT_1(Private, void filter_patterns_PB())
      CS_SLOT_2(filter_patterns_PB)

      CS_SLOT_1(Private, void filter_source_patterns_PB())
      CS_SLOT_2(filter_source_patterns_PB)

      // tab 2- look up (browser)
      CS_SLOT_1(Private, void clang_options_PB())
      CS_SLOT_2(clang_options_PB)

      // tab 2- look up (index)
      CS_SLOT_1(Private, void ignore_prefix_PB())
      CS_SLOT_2(ignore_prefix_PB)

      // tab 2- look up (preprocess)
      CS_SLOT_1(Private, void include_path_PB())
      CS_SLOT_2(include_path_PB)

      CS_SLOT_1(Private, void include_file_patterns_PB())
      CS_SLOT_2(include_file_patterns_PB)

      CS_SLOT_1(Private, void predefined_macros_PB())
      CS_SLOT_2(predefined_macros_PB)

      CS_SLOT_1(Private, void expand_as_defined_PB())
      CS_SLOT_2(expand_as_defined_PB)

      // tab 2- look up (external)
      CS_SLOT_1(Private, void tag_files_PB())
      CS_SLOT_2(tag_files_PB)

      CS_SLOT_1(Private, void gen_tagfile_PB())
      CS_SLOT_2(gen_tagfile_PB)

      CS_SLOT_1(Private, void perl_path_PB())
      CS_SLOT_2(perl_path_PB)

      // tab 2- look up (dot)
      CS_SLOT_1(Private, void mscgen_path_PB())
      CS_SLOT_2(mscgen_path_PB)

      CS_SLOT_1(Private, void dia_path_PB())
      CS_SLOT_2(dia_path_PB)

      CS_SLOT_1(Private, void dot_font_name_PB())
      CS_SLOT_2(dot_font_name_PB)

      CS_SLOT_1(Private, void dot_font_path_PB())
      CS_SLOT_2(dot_font_path_PB)

      CS_SLOT_1(Private, void dot_path_PB())
      CS_SLOT_2(dot_path_PB)

      CS_SLOT_1(Private, void dot_file_dirs_PB())
      CS_SLOT_2(dot_file_dirs_PB)

      CS_SLOT_1(Private, void msc_file_dirs_PB())
      CS_SLOT_2(msc_file_dirs_PB)

      CS_SLOT_1(Private, void dia_file_dirs_PB())
      CS_SLOT_2(dia_file_dirs_PB)

      CS_SLOT_1(Private, void plantuml_jar_path_PB())
      CS_SLOT_2(plantuml_jar_path_PB)

      // tab 3 look up (html)
      CS_SLOT_1(Private, void html_output_PB())
      CS_SLOT_2(html_output_PB)

      CS_SLOT_1(Private, void html_header_PB())
      CS_SLOT_2(html_header_PB)

      CS_SLOT_1(Private, void html_footer_PB())
      CS_SLOT_2(html_footer_PB)

      CS_SLOT_1(Private, void html_extra_stylesheets_PB())
      CS_SLOT_2(html_extra_stylesheets_PB)

      CS_SLOT_1(Private, void html_extra_files_PB())
      CS_SLOT_2(html_extra_files_PB)

      CS_SLOT_1(Private, void chm_file_PB())
      CS_SLOT_2(chm_file_PB)

      CS_SLOT_1(Private, void hhc_location_PB())
      CS_SLOT_2(hhc_location_PB)

      CS_SLOT_1(Private, void qch_file_PB())
      CS_SLOT_2(qch_file_PB)

      CS_SLOT_1(Private, void qt_help_gen_path_PB())
      CS_SLOT_2(qt_help_gen_path_PB)

      CS_SLOT_1(Private, void mathjax_extensions_PB())
      CS_SLOT_2(mathjax_extensions_PB)

      CS_SLOT_1(Private, void search_data_file_PB())
      CS_SLOT_2(search_data_file_PB)

      CS_SLOT_1(Private, void search_mappings_PB())
      CS_SLOT_2(search_mappings_PB)

      // tab 3 look up (latex)
      CS_SLOT_1(Private, void latex_output_PB())
      CS_SLOT_2(latex_output_PB)

      CS_SLOT_1(Private, void latex_cmd_name_PB())
      CS_SLOT_2(latex_cmd_name_PB)

      CS_SLOT_1(Private, void make_index_cmd_name_PB())
      CS_SLOT_2(make_index_cmd_name_PB)

      CS_SLOT_1(Private, void latex_extra_packages_PB())
      CS_SLOT_2(latex_extra_packages_PB)

      CS_SLOT_1(Private, void latex_header_PB())
      CS_SLOT_2(latex_header_PB)

      CS_SLOT_1(Private, void latex_footer_PB())
      CS_SLOT_2(latex_footer_PB)

      CS_SLOT_1(Private, void latex_extra_files_PB())
      CS_SLOT_2(latex_extra_files_PB)

      // tab 3 look up (rtf)
      CS_SLOT_1(Private, void rtf_output_PB())
      CS_SLOT_2(rtf_output_PB)

      CS_SLOT_1(Private, void rtf_stylesheet_PB())
      CS_SLOT_2(rtf_stylesheet_PB)

      CS_SLOT_1(Private, void rtf_extension_PB())
      CS_SLOT_2(rtf_extension_PB)

      // tab 3 look up (man)
      CS_SLOT_1(Private, void man_output_PB())
      CS_SLOT_2(man_output_PB)

      // tab 3 look up (xml)
      CS_SLOT_1(Private, void xml_output_PB())
      CS_SLOT_2(xml_output_PB)

      // tab 3 look up (docbook)
      CS_SLOT_1(Private, void docbook_output_PB())
      CS_SLOT_2(docbook_output_PB)

      // tab 4
      CS_SLOT_1(Private, void runDoxyPress())
      CS_SLOT_2(runDoxyPress)

      CS_SLOT_1(Private, void readStdout())
      CS_SLOT_2(readStdout)

      CS_SLOT_1(Private, void runComplete())
      CS_SLOT_2(runComplete)

      CS_SLOT_1(Private, void showHtmlOutput())
      CS_SLOT_2(showHtmlOutput)

      CS_SLOT_1(Private, void saveLog())
      CS_SLOT_2(saveLog)
};

#endif
