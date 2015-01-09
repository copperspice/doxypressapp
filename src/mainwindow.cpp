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

#include "dox_build_info.h"
#include "mainwindow.h"

#include <QKeySequence>

MainWindow::MainWindow()
   : m_ui(new Ui::MainWindow)
{
   m_ui->setupUi(this);   

   m_ui->wizard_Splitter->setStretchFactor(0, 1);
   m_ui->wizard_Splitter->setStretchFactor(1, 3);

   m_ui->build_Splitter->setStretchFactor(0, 1);
   m_ui->build_Splitter->setStretchFactor(1, 3);

   m_ui->build_leftSplitter->setStretchFactor(0, 3);
   m_ui->build_leftSplitter->setStretchFactor(1, 2);

   m_ui->output_Splitter->setStretchFactor(0, 1);
   m_ui->output_Splitter->setStretchFactor(1, 3);

   m_ui->output_leftSplitter->setStretchFactor(0, 5);
   m_ui->output_leftSplitter->setStretchFactor(1, 4);

   //
   for (int k = 0; k < m_ui->build_TreeWidget->topLevelItemCount(); ++k) {

      QTreeWidgetItem *item = m_ui->build_TreeWidget->topLevelItem(k);

      if (item->text(0) == "General") {
         item->setFlags(Qt::ItemIsEnabled);

         m_ui->build_TreeWidget->expandItem(item);

      } else if (item->text(0) == "Build") {
         item->setFlags(Qt::ItemIsEnabled);

         m_ui->build_TreeWidget->expandItem(item);

      } if (item->text(0) == "Input") {
         item->setFlags(Qt::ItemIsEnabled);

         m_ui->build_TreeWidget->expandItem(item);

      } if (item->text(0) == "Dot") {
         item->setFlags(Qt::ItemIsEnabled);

         m_ui->build_TreeWidget->expandItem(item);
      }
   }


   for (int k = 0; k < m_ui->output_TreeWidget->topLevelItemCount(); ++k) {

      QTreeWidgetItem *item = m_ui->output_TreeWidget->topLevelItem(k);

      if (item->text(0) == "HTML") {
         item->setFlags(Qt::ItemIsEnabled);

         m_ui->output_TreeWidget->expandItem(item);
      }
   }

   m_ConfigFile = "";
   setDoxygenTitle(false);

   setIconSize(QSize(32,32));
   setWindowIcon(QIcon(":/resources/cs_doxygen.png"));

   if (! json_Read(CFG_STARTUP) ) {
      // do not start program
      csError(tr("Configuration File Missing"), tr("Unable to locate the CS Doxygen Configuration file."));
      throw std::runtime_error("abort_no_message");
   }

   // screen setup
   createConnections();
   createShortCuts();

   // next 3, watch order
   createMap();
   setupLimits();
   clearAllFields();

/*
   // recent files
   rf_CreateMenus();

   // recent files, context menu
   m_ui->menuFile->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(m_ui->menuFile, SIGNAL(customContextMenuRequested(const QPoint &)),   this, SLOT(showContext_Files(const QPoint &)));

   m_wizard->refresh();
   updateLaunchButtonState();
*/

   m_ui->setup_StackedWidget->setCurrentWidget(m_ui->page_Project);
   m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_General_A);
   m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_A);

   m_ui->tabWidget->setCurrentIndex(0);

   setUnifiedTitleAndToolBarOnMac(true);
   setStatusBar(tr("Ready"));
}

void MainWindow::about()
{
   QString textBody = "<font color='#000080'><table style=margin-right:25>"
                      "<tr><td><nobr>Developed by Barbara Geller</nobr></td><td>barbara@copperspice.com</td></tr>"
                      "<tr><td style=padding-right:25><nobr>Developed by Ansel Sermersheim</nobr></td><td>ansel@copperspice.com</td></tr>"
                      "</table></font>"
                      "<p><nobr><small>Original development by Dimitri van Heesch</small></nobr></p>"
                      "<p><small>Copyright 2014-2015 BG Consulting, All rights reserved.<br>"
                      "This program is provided AS IS with no warranty of any kind.<br></small></p>";

   //
   QMessageBox msgB;
   msgB.setIcon(QMessageBox::NoIcon);
   msgB.setWindowIcon(QIcon(":/resources/cs_doxygen.png"));

   msgB.setWindowTitle(tr("About CS Doxygen"));
   msgB.setText(tr("<p style=margin-right:25><center><h5>Version: %1<br>Build # 2.01.2015</h5></center></p>").arg(versionString));
   msgB.setInformativeText(textBody);

   msgB.setStandardButtons(QMessageBox::Ok);
   msgB.setDefaultButton(QMessageBox::Ok);

   msgB.exec();
}

void MainWindow::createConnections()
{
   // file
   connect(m_ui->actionNew,        SIGNAL(triggered()), this, SLOT(newDoxy()));
   connect(m_ui->actionOpen,       SIGNAL(triggered()), this, SLOT(openDoxy()));
   connect(m_ui->actionReload,     SIGNAL(triggered()), this, SLOT(reloadDoxy()));

   connect(m_ui->actionSave,       SIGNAL(triggered()), this, SLOT(saveDoxy()));
   connect(m_ui->actionSave_As,    SIGNAL(triggered()), this, SLOT(saveDoxyAs()));

   connect(m_ui->actionExit,       SIGNAL(triggered()), this, SLOT(close()));

   // settings
   connect(m_ui->actionImport,          &QAction::triggered, this, [this](bool){ importDoxy(); } );
   connect(m_ui->actionMove_WizardCfg,  &QAction::triggered, this, [this](bool){ move_WizardCfg(); } );
   connect(m_ui->actionSave_WizardCfg,  &QAction::triggered, this, [this](bool){ save_WizardCfg(); } );

   // help
   connect(m_ui->actionDoxyHelp,    SIGNAL(triggered()), this, SLOT(manual()));
   connect(m_ui->actionAbout,       SIGNAL(triggered()), this, SLOT(about()));

   //
   connect(qApp,  SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(focusChanged(QWidget *, QWidget *)));


   // connections for tabs
   connect(m_ui->setup_TreeWidget,  SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                                    SLOT(setupPage(QTreeWidgetItem *, QTreeWidgetItem *)));

   connect(m_ui->build_TreeWidget,  SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                                    SLOT(buildPage(QTreeWidgetItem *, QTreeWidgetItem *)));

   connect(m_ui->output_TreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                                    SLOT(outputPage(QTreeWidgetItem *, QTreeWidgetItem *)));

   // tab 1
   connect(m_ui->icon_PB,             &QPushButton::clicked, this, [this](bool){ icon_PB(""); } );
   connect(m_ui->output_dir_PB,       SIGNAL(clicked()),     this, SLOT(output_dir_PB()));
   connect(m_ui->html_colors_PB,      SIGNAL(clicked()),     this, SLOT(tune_colors_PB()));

   // tab 2
   connect(m_ui->input_source_PB,     SIGNAL(clicked()),   this, SLOT(input_source_PB()));
   connect(m_ui->file_patterns_PB,    SIGNAL(clicked()),   this, SLOT(file_patterns_PB()));
   connect(m_ui->exclude_files_PB,    SIGNAL(clicked()),   this, SLOT(exclude_files_PB()));
   connect(m_ui->exclude_patterns_PB, SIGNAL(clicked()),   this, SLOT(exclude_patterns_PB()));
   connect(m_ui->exclude_symbols_PB,  SIGNAL(clicked()),   this, SLOT(exclude_symbols_PB()));


   // tab 3


   // tab 4


}

void MainWindow::createShortCuts()
{
   struct {
      QKeySequence key_open;
      QKeySequence key_close;
      QKeySequence key_save;
      QKeySequence key_saveAs;
      QKeySequence key_exit;
      QKeySequence key_DoxyHelp;
   }  struct_key;

   struct_key.key_open      = QKeySequence(QKeySequence::Open);
   struct_key.key_save      = QKeySequence(QKeySequence::Save);
   struct_key.key_saveAs    = QKeySequence(QKeySequence::SaveAs);
   struct_key.key_exit      = QKeySequence(QKeySequence::Quit);
   struct_key.key_DoxyHelp  = QKeySequence(QKeySequence::HelpContents);

#ifdef Q_OS_WIN
   struct_key.key_saveAs    = QKeySequence("Ctrl+Shift+S");
   struct_key.key_exit      = QKeySequence("Ctrl+Q");
#endif

#ifdef Q_OS_MAC
#endif

   m_ui->actionOpen->setShortcut(QKeySequence(struct_key.key_open));
   m_ui->actionSave->setShortcut(QKeySequence(struct_key.key_save));
   m_ui->actionSave_As->setShortcut(QKeySequence(struct_key.key_saveAs));
   m_ui->actionExit->setShortcut(QKeySequence(struct_key.key_exit));

   m_ui->actionDoxyHelp->setShortcut(QKeySequence(struct_key.key_DoxyHelp));
}

void MainWindow::manual()
{
   // broom - move when ready
   QDesktopServices::openUrl(QUrl("http://www.copperspice.org/docs/cs_api/index.html"));
}

void MainWindow::setupPage(QTreeWidgetItem *item, QTreeWidgetItem *)
{
   if (item) {
      QString label = item->text(0);

      if (label == tr("Project")) {
         m_ui->setup_StackedWidget->setCurrentWidget(m_ui->page_Project);

      } else if (label == tr("Output")) {
         m_ui->setup_StackedWidget->setCurrentWidget(m_ui->page_Output);

      } else if (label == tr("Diagrams")) {
         m_ui->setup_StackedWidget->setCurrentWidget(m_ui->page_Diagrams);

      }
   }
}

void MainWindow::buildPage(QTreeWidgetItem *item, QTreeWidgetItem *)
{
   if (item) {
      QString label = item->text(0);

      if (label == tr("General A")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_General_A);

      } else  if (label == tr("General B")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_General_B);

      } else if (label == tr("Build A")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Build_A);

      } else if (label == tr("Build B")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Build_B);

      } else if (label == tr("Messages")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Messages);

      } else if (label == tr("Input A")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Input_A);

      } else if (label == tr("Input B")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Input_B);

      } else if (label == tr("Source Browser")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Browser);

      } else if (label == tr("Index Page")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Index);

      } else if (label == tr("AutoGen")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_AutoGen);

      } else if (label == tr("PerlMod")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_PerlMod);

      } else if (label == tr("Preprocessor")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Process);

      } else if (label == tr("External")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_External);

      } else if (label == tr("Dot A")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Dot_A);

      } else if (label == tr("Dot B")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Dot_B);

      }
   }
}

void MainWindow::outputPage(QTreeWidgetItem *item, QTreeWidgetItem *)
{
   if (item) {
      QString label = item->text(0);

      if (label == tr("HTML A")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_A);

      } else if (label == tr("HTML B")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_B);

      } else if (label == tr("HTML C")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_C);

      } else if (label == tr("LaTeX")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Latex);

      } else if (label == tr("RTF")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Rtf);

      } else if (label == tr("Man")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Man);

      } else if (label == tr("XML")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Xml);

      } else if (label == tr("Docbook")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Docbook);

      }
   }
}



/*

   m_runProcess = new QProcess;
   m_running    = false;
   m_timer      = new QTimer;

   connect(m_runProcess,    SIGNAL(readyReadStandardOutput()), SLOT(readStdout()));
   connect(m_runProcess,    SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(runComplete()));
   connect(m_timer,         SIGNAL(timeout()), SLOT(readStdout()));
   connect(m_run,           SIGNAL(clicked()), SLOT(runDoxygen()));
   connect(m_launchHtml,    SIGNAL(clicked()), SLOT(showHtmlOutput()));
   connect(m_saveLog,       SIGNAL(clicked()), SLOT(saveLog()));
   connect(showSettings,    SIGNAL(clicked()), SLOT(showSettings()));

   connect(m_expert,        SIGNAL(changed()), SLOT(configChanged()));

*/


/*
   missing icon on all windows
   recent files
   test move cfg file

   add all widgets
   ensure field names are correct

   import old dox file

   enhande load/save CFG to json format

   add RELOAD   
   put in correct url for mannual() when ready

   need to update m_modifed when doxy file changes
*/


