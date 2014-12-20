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

#include "doxywizard.h"
#include "dox_build_info.h"
#include "expert.h"
#include "mainwindow.h"
#include "wizard.h"

#ifdef WIN32
#include <windows.h>
#endif

MainWindow::MainWindow()
   : m_ui(new Ui::MainWindow)
{
   m_ui->setupUi(this);   

   m_ui->wizardSplitter->setStretchFactor(0, 1);
   m_ui->wizardSplitter->setStretchFactor(1, 4);

   m_ConfigFile = "";
   setDoxygenTitle(false);

   setIconSize(QSize(32,32));
   setWindowIcon(QIcon("://resources/doxygen.ico"));

   if (! json_Read(CFG_STARTUP) ) {
      // do not start program
      csError(tr("Configuration File Missing"), tr("Unable to locate or open the CS Doxygen Configuration file."));
      throw std::runtime_error("abort_no_message");
   }

   // screen setup
// createStatusBar();         BROOM

   createConnections();
   createShortCuts();

/*  BROOM

   // recent files
   rf_CreateMenus();

   // recent files, context menu
   m_ui->menuFile->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(m_ui->menuFile, SIGNAL(customContextMenuRequested(const QPoint &)),   this, SLOT(showContext_Files(const QPoint &)));

   // user requested files on the command line
   if (fileList.count() > 1 ) {
      argLoad(fileList);
   }

   updateLaunchButtonState();
   m_wizard->refresh();


   setStatusBar(tr("Ready"), 0);
*/


   setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::about()
{
   // BROOM
   // QString::fromAscii("<qt><center>A tool to configure and run doxygen version ") +
   // QString::fromAscii(" on your source files.</center><p><br>"
   //                     "<center>Written by<br> Dimitri van Heesch<br>&copy; 2000-2014</center><p>""</qt>");
   // QString::fromAscii(versionString)

   QString textBody = "<font color='#000080'><table style=margin-right:25>"
                      "<tr><td><nobr>Developed by Barbara Geller</nobr></td><td>barbara@copperspice.com</td></tr>"
                      "<tr><td style=padding-right:25><nobr>Developed by Ansel Sermersheim</nobr></td><td>ansel@copperspice.com</td></tr>"
                      "</table></font>"
                      "<br>"
                      "<p><small>Copyright 2012-2015 BG Consulting, All rights reserved.<br>"
                      "This program is provided AS IS with no warranty of any kind.<br></small></p>";

   //
   QMessageBox msgB;
   msgB.setIcon(QMessageBox::NoIcon);
   msgB.setWindowIcon(QIcon("://resources/diamond.png"));

   msgB.setWindowTitle(tr("About CS Doxygen"));
   msgB.setText(tr("<p style=margin-right:25><center><h5>Version: 1.1.1<br>Build # 7.24.2014</h5></center></p>"));
   msgB.setInformativeText(textBody);

   msgB.setStandardButtons(QMessageBox::Ok);
   msgB.setDefaultButton(QMessageBox::Ok);

   msgB.exec();
}

void MainWindow::createConnections()
{
   // file
// connect(m_ui->actionNew,               SIGNAL(triggered()), this, SLOT(newFile()));

   connect(m_ui->actionOpen,              SIGNAL(triggered()), this, SLOT(openConfig()));

// connect(m_ui->actionClose,             SIGNAL(triggered()), this, SLOT(close_Doc()));
// connect(m_ui->actionReload,            SIGNAL(triggered()), this, SLOT(reload()));

   connect(m_ui->actionSave,              SIGNAL(triggered()), this, SLOT(saveDoxFile()));
   connect(m_ui->actionSave_As,           SIGNAL(triggered()), this, SLOT(saveDoxCfgAs()));

   connect(m_ui->actionExit,              SIGNAL(triggered()), this, SLOT(close()));

   // tools


   // settings
//   connect(m_ui->actionColors,            SIGNAL(triggered()), this, SLOT(setColors()));
//   connect(m_ui->actionFonts,             SIGNAL(triggered()), this, SLOT(setFont()));
//   connect(m_ui->actionMove_ConfigFile,   SIGNAL(triggered()), this, SLOT(move_ConfigFile()));
//   connect(m_ui->actionSave_ConfigFile,   &QAction::triggered, this, [this](bool){ save_ConfigFile(); } );


   // help
   connect(m_ui->actionDoxygen_Help,      SIGNAL(triggered()), this, SLOT(manual()));
   connect(m_ui->actionAbout,             SIGNAL(triggered()), this, SLOT(about()));
}


void MainWindow::createShortCuts()
{

/*
   m_ui->actionOpen->setShortcut(QKeySequence(struct_temp.key_open));
   m_ui->actionClose->setShortcut(QKeySequence(struct_temp.key_close));
   m_ui->actionSave->setShortcut(QKeySequence(struct_temp.key_save));
   m_ui->actionSave_As->setShortcut(QKeySequence(struct_temp.key_saveAs));
   m_ui->actionExit->setShortcuts(Qt::CTRL + Qt::Key_Q);

   connect(m_ui->actionDoxygen_Help,  --> add Qt::Key_F1

*/

}

void MainWindow::manual()
{
   QDesktopServices::openUrl(QUrl(QString::fromAscii("http://www.doxygen.org/manual.html")));
}



/*

   file->addAction(tr("Open..."), this, SLOT(openConfig()), Qt::CTRL + Qt::Key_O);
   file->addAction(tr("Save"), this, SLOT(saveConfig()), Qt::CTRL + Qt::Key_S);
   file->addAction(tr("Save as..."), this, SLOT(saveConfigAs()), Qt::SHIFT + Qt::CTRL + Qt::Key_S);
   file->addAction(tr("Quit"), this, SLOT(quit()), Qt::CTRL + Qt::Key_Q);

   settings->addAction(tr("Reset to factory defaults"), this, SLOT(resetToDefaults()));
   settings->addAction(tr("Use current settings at startup"), this, SLOT(makeDefaults()));
   settings->addAction(tr("Clear recent list"),this, SLOT(clearRecent()));

   launchLayout->addStretch(1);
   grid->addLayout(launchLayout, 1, 0);
   runTabLayout->addLayout(grid);

   m_expert = new Expert;
   m_wizard = new Wizard(m_expert->modelData());

   m_tabs = new QTabWidget;
   m_tabs->addTab(m_wizard, tr("Wizard"));
   m_tabs->addTab(m_expert, tr("Expert"));
   m_tabs->addTab(runTab,   tr("Run"));

   rowLayout->addWidget(new QLabel(tr("Step 1: Specify the working directory from which doxygen will run")));
   rowLayout->addLayout(dirLayout);

   rowLayout->addWidget(new QLabel(tr("Step 2: Configure doxygen using the Wizard and/or Expert tab, then "
                                      "switch to the Run tab to generate the documentation")));

   rowLayout->addWidget(m_tabs);

   setCentralWidget(topPart);
   statusBar()->showMessage(tr("Welcome to CS Doxygen"), messageTimeout);


   m_runProcess = new QProcess;
   m_running    = false;

   m_timer      = new QTimer;

   // connect signals and slots
   connect(m_tabs,          SIGNAL(currentChanged(int)),       SLOT(selectTab(int)));

   connect(m_selWorkingDir, SIGNAL(clicked()),                 SLOT(selectWorkingDir()));
   connect(m_recentMenu,    SIGNAL(triggered(QAction *)),      SLOT(openRecent(QAction *)));
   connect(m_workingDir,    SIGNAL(returnPressed()),           SLOT(updateWorkingDir()));
   connect(m_runProcess,    SIGNAL(readyReadStandardOutput()), SLOT(readStdout()));
   connect(m_runProcess,    SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(runComplete()));
   connect(m_timer,         SIGNAL(timeout()), SLOT(readStdout()));
   connect(m_run,           SIGNAL(clicked()), SLOT(runDoxygen()));
   connect(m_launchHtml,    SIGNAL(clicked()), SLOT(showHtmlOutput()));
   connect(m_saveLog,       SIGNAL(clicked()), SLOT(saveLog()));
   connect(showSettings,    SIGNAL(clicked()), SLOT(showSettings()));
   connect(m_expert,        SIGNAL(changed()), SLOT(configChanged()));

   connect(m_wizard, SIGNAL(done()), SLOT(selectRunTab()));
   connect(m_expert, SIGNAL(done()), SLOT(selectRunTab()));

*/

