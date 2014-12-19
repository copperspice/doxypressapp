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

#ifndef DOXYWIZARD_H
#define DOXYWIZARD_H

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
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

class Expert;
class Wizard;

class MainWindow : public QMainWindow
{
   CS_OBJECT(MainWindow)

 public:
   MainWindow();

   void loadConfigFromFile(const QString &fileName);
   void loadSettings();
   void saveSettings();
   void closeEvent(QCloseEvent *event);

   QString configFileName() const {
      return m_fileName;
   }
   void updateTitle();

   CS_SLOT_1(Public, void manual())
   CS_SLOT_2(manual)

   CS_SLOT_1(Public, void about())
   CS_SLOT_2(about)

   CS_SLOT_1(Public, void openConfig())
   CS_SLOT_2(openConfig)

   CS_SLOT_1(Public, void saveConfig())
   CS_SLOT_OVERLOAD(saveConfig, ())

   CS_SLOT_1(Public, bool saveConfigAs())
   CS_SLOT_2(saveConfigAs)

   CS_SLOT_1(Public, void makeDefaults())
   CS_SLOT_2(makeDefaults)

   CS_SLOT_1(Public, void resetToDefaults())
   CS_SLOT_2(resetToDefaults)

   CS_SLOT_1(Public, void selectTab(int un_named_arg1))
   CS_SLOT_2(selectTab)

   CS_SLOT_1(Public, void quit())
   CS_SLOT_2(quit)

 private :
   CS_SLOT_1(Private, void openRecent(QAction *action))
   CS_SLOT_2(openRecent)

   CS_SLOT_1(Private, void selectWorkingDir())   
   CS_SLOT_2(selectWorkingDir)

   CS_SLOT_1(Private, void updateWorkingDir())
   CS_SLOT_2(updateWorkingDir)

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

 private:

   void saveConfig(const QString &fileName);
   void addRecentFile(const QString &fileName);
   void updateConfigFileName(const QString &fileName);
   void setWorkingDir(const QString &dirName);
   void updateLaunchButtonState();
   bool discardUnsavedChanges(bool saveOption = true);

   QLineEdit *m_workingDir;
   QPushButton *m_selWorkingDir;
   QPushButton *m_run;
   QPushButton *m_saveLog;
   QPushButton *m_launchHtml;
   QPushButton *m_launchPdf;
   QTextEdit *m_outputLog;
   QLabel *m_runStatus;

   Expert *m_expert;
   Wizard *m_wizard;

   QString m_fileName;
   QSettings m_settings;
   QMenu *m_recentMenu;
   QStringList m_recentFiles;
   QProcess *m_runProcess;
   QTimer *m_timer;
   QTabWidget *m_tabs;
   bool m_running;
   bool m_modified;
};

#endif
