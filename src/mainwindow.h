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

// status bar message timeout in millisec
static const int messageTimeout = 5000;

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

      void loadDoxCfg(const QString fname);

      QString pathName(QString fileName) const;

      void saveSettings();
      struct Settings get_StructData();
      void setDoxygenTitle(bool isModified);

      CS_SLOT_1(Public, void manual())
      CS_SLOT_2(manual)

      CS_SLOT_1(Public, void about())
      CS_SLOT_2(about)

      CS_SLOT_1(Public, void openConfig())
      CS_SLOT_2(openConfig)

      CS_SLOT_1(Public, void saveDoxFile())
      CS_SLOT_2(saveDoxFile)

      CS_SLOT_1(Public, bool saveDoxCfgAs())
      CS_SLOT_2(saveDoxCfgAs)

      CS_SLOT_1(Public, void makeDefaults())
      CS_SLOT_2(makeDefaults)

      CS_SLOT_1(Public, void resetToDefaults())
      CS_SLOT_2(resetToDefaults)

      CS_SLOT_1(Public, void selectTab(int un_named_arg1))
      CS_SLOT_2(selectTab)     

   protected:
      void closeEvent(QCloseEvent *event);

   private :
      Ui::MainWindow *m_ui;

      QStringList m_openedFiles;

      enum Config { CFG_STARTUP, CFG_DEFAULT };
      enum Option { ABOUTURL, AUTOLOAD, CLOSE, COLORS, FONT, PATH_PRIOR, RECENTFILE };

      void createShortCuts();
      void createConnections();

      //
      void addRecentFile(const QString &fileName);

      bool querySave();
      void saveDoxCfg();

      void updateLaunchButtonState();
      void updateConfigFileName(const QString &fileName);

      // json
      bool json_Read(Config trail = CFG_DEFAULT);
      bool json_Write(Option route, Config trail = CFG_DEFAULT);
      void json_getFileName();
      bool json_CreateNew();
      bool json_SaveFile(QByteArray route);
      QByteArray json_ReadFile();

      void save_ConfigFile();
      QString get_xxFile(QString title, QString fname, QString filter);

      QFont json_SetFont(QString value);
      QColor json_SetColor(QString values);
      QString json_GetRGB(QColor color);


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

      CS_SLOT_1(Private, void openRecent(QAction *action))
      CS_SLOT_2(openRecent)

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

      CS_SLOT_1(Private, void configChanged())
      CS_SLOT_2(configChanged)

      CS_SLOT_1(Private, void clearRecent())
      CS_SLOT_2(clearRecent)

      CS_SLOT_1(Private, void selectRunTab())
      CS_SLOT_2(selectRunTab)

      CS_SLOT_1(Private, void move_ConfigFile())
      CS_SLOT_2(move_ConfigFile)

};

#endif
