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
   bool isFolderPB;
   bool isFilePB;
};

class MainWindow : public QMainWindow
{
   CS_OBJECT(MainWindow)

   public:
      MainWindow();

      QString m_appPath;
      QString m_jsonFname;
      QString m_ConfigFile;

      QString m_project_iconFN;

      struct Settings m_struct;

      QString pathName(QString fileName) const;

      void openDoxy_Internal(const QString fname);

      void setStatusBar(QString msg);
      void setStatusBar(QString msg, int timeOut);

      void saveSettings();
      struct Settings get_StructData();
      void setDoxyTitle(bool isModified);

       QSize sizeHint() const;

   protected:
      void closeEvent(QCloseEvent *event);

   private :
      Ui::MainWindow *m_ui;

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

      QString getFile_CS(QString title, QString fname, QString filter);

      bool htmlOutputPresent(const QString &workingDir) const;

      void initTabs();

      bool querySave();
      void saveDoxy_Internal();

      void updateLaunchButtonState();

      void setHelpColor(int option, QWidget *label);
      void setHelpText(QWidget *name);
      void createMap();

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

      QPushButton *m_run;
      QPushButton *m_saveLog;
      QPushButton *m_launchHtml;
      QPushButton *m_launchPdf;
      QTextEdit *m_outputLog;
      QLabel *m_runStatus;

      QSettings m_settings;

      QMenu *m_recentMenu;
      QStringList m_recentFiles;

      QProcess *m_runProcess;
      QTimer *m_timer;

      bool m_running;
      bool m_modified;

      // validation
      void validGet_html();
      void validGet_latex();
      void validGet_dot();

      CS_SLOT_1(Private, void validSet_html(QAbstractButton *))
      CS_SLOT_2(validSet_html)

      CS_SLOT_1(Private, void validSet_latex(QAbstractButton *))
      CS_SLOT_2(validSet_latex)

      CS_SLOT_1(Private, void validSet_dot(QAbstractButton *))
      CS_SLOT_2(validSet_dot)

      //
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

      //
      CS_SLOT_1(Private, void importDoxy())
      CS_SLOT_2(importDoxy)

      CS_SLOT_1(Private, void move_WizardCfg())
      CS_SLOT_2(move_WizardCfg)

      CS_SLOT_1(Private, void save_WizardCfg())
      CS_SLOT_2(save_WizardCfg)

      // app level
      CS_SLOT_1(Private, void focusChanged(QWidget *, QWidget *))
      CS_SLOT_2(focusChanged)

      //
      CS_SLOT_1(Private, void setupPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(setupPage)

      CS_SLOT_1(Private, void buildPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(buildPage)

      CS_SLOT_1(Private, void outputPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(outputPage)

      CS_SLOT_1(Private, void manual())
      CS_SLOT_2(manual)

      CS_SLOT_1(Private, void about())
      CS_SLOT_2(about)

      // tab 1
      CS_SLOT_1(Private, void icon_PB(const QString route))
      CS_SLOT_2(icon_PB)

      CS_SLOT_1(Private, void output_dir_PB())
      CS_SLOT_2(output_dir_PB)

      CS_SLOT_1(Private, void tune_colors_PB())
      CS_SLOT_2(tune_colors_PB)

      // tab 2
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

      // tab 3


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
