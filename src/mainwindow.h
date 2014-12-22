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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "expert.h"
#include "ui_mainwindow.h"
#include "util.h"
#include "wizard.h"

#include <QAction>
#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QColor>
#include <QDesktopServices>
#include <QDir>
#include <QFont>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTextEdit>
#include <QTextStream>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

static const int RECENT_FILES_MAX = 10;

struct Settings {
   QFont   fontNormal;
   QColor  colorText;
   QString pathPrior;
};

class MainWindow : public QMainWindow
{
   CS_OBJECT(MainWindow)

   public:
      MainWindow();

      QString m_appPath;
      QString m_jsonFname;
      QString m_ConfigFile;

      struct Settings m_struct;

      QString pathName(QString fileName) const;

      void openDoxy_Internal(const QString fname);

      void setStatusBar(QString msg);
      void setStatusBar(QString msg, int timeOut);

      void saveSettings();
      struct Settings get_StructData();
      void setDoxygenTitle(bool isModified);

      CS_SLOT_1(Public, void newDoxy())
      CS_SLOT_2(newDoxy)

      CS_SLOT_1(Public, void openDoxy())
      CS_SLOT_2(openDoxy)

      CS_SLOT_1(Public, void saveDoxy())
      CS_SLOT_2(saveDoxy)

      CS_SLOT_1(Public, bool saveDoxyAs())
      CS_SLOT_2(saveDoxyAs)

      //
      CS_SLOT_1(Public, void importDoxy())
      CS_SLOT_2(importDoxy)

      CS_SLOT_1(Public, void move_WizardCfg())
      CS_SLOT_2(move_WizardCfg)

      CS_SLOT_1(Public, void save_WizardCfg())
      CS_SLOT_2(save_WizardCfg)

      //
      CS_SLOT_1(Public, void manual())
      CS_SLOT_2(manual)

      CS_SLOT_1(Public, void about())
      CS_SLOT_2(about)

   protected:
      void closeEvent(QCloseEvent *event);

   private :
      Ui::MainWindow *m_ui;

      QStringList m_openedFiles;

      enum Config { CFG_STARTUP, CFG_DEFAULT };
      enum Option { ABOUTURL, CLOSE, PATH_PRIOR, RECENTFILE };

      void createShortCuts();
      void createConnections();

      bool querySave();
      void saveDox_Internal();

      void updateLaunchButtonState();

      // json
      bool json_Read(Config trail = CFG_DEFAULT);
      bool json_Write(Option route, Config trail = CFG_DEFAULT);
      void json_getFileName();
      bool json_CreateNew();
      bool json_SaveFile(QByteArray route);

      QByteArray json_ReadFile();
      void save_ConfigFile();
      QString get_xxFile(QString title, QString fname, QString filter);

      void json_OpenDoxy(QByteArray data);
      QByteArray json_SaveDoxy();

      QPushButton *m_run;
      QPushButton *m_saveLog;
      QPushButton *m_launchHtml;
      QPushButton *m_launchPdf;
      QTextEdit *m_outputLog;
      QLabel *m_runStatus;
      Expert *m_expert;
      Wizard *m_wizard;
      QSettings m_settings;
      QMenu *m_recentMenu;
      QStringList m_recentFiles;
      QProcess *m_runProcess;
      QTimer *m_timer;
      QTabWidget *m_tabs;

      bool m_running;
      bool m_modified;

      //
      CS_SLOT_1(Private, void setupPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(setupPage)

      CS_SLOT_1(Private, void buildPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(buildPage)

      CS_SLOT_1(Private, void outputPage(QTreeWidgetItem *, QTreeWidgetItem *))
      CS_SLOT_2(outputPage)

      //
      CS_SLOT_1(Private, void runDoxygen())
      CS_SLOT_2(runDoxygen)

      CS_SLOT_1(Private, void readStdout())
      CS_SLOT_2(readStdout)

      CS_SLOT_1(Private, void runComplete())
      CS_SLOT_2(runComplete)

      CS_SLOT_1(Private, void showHtmlOutput())
      CS_SLOT_2(showHtmlOutput)

      CS_SLOT_1(Private, void saveLog())
      CS_SLOT_2(saveLog)

      CS_SLOT_1(Private, void showSettings())
      CS_SLOT_2(showSettings)     
};

#endif
