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

#include "doxy_build_info.h"
#include "mainwindow.h"

#include <QKeySequence>

MainWindow::MainWindow()
   : m_ui(new Ui::MainWindow)
{
   m_ui->setupUi(this);   

   QList<int> temp;
   temp.append(250);
   temp.append(500);

   m_ui->setup_Splitter->setSizes(temp);
   m_ui->build_Splitter->setSizes(temp);
   m_ui->output_Splitter->setSizes(temp);

   m_ui->setup_leftSplitter->setStretchFactor(0, 3);
   m_ui->setup_leftSplitter->setStretchFactor(1, 2);

   m_ui->build_leftSplitter->setStretchFactor(0, 3);
   m_ui->build_leftSplitter->setStretchFactor(1, 2);

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

   m_curFile = "";
   setDoxyTitle(false);

   setIconSize(QSize(32,32));
   setWindowIcon(QIcon(":/resources/doxypress.png"));

   if (! json_Read(CFG_STARTUP) ) {
      // do not start program
      csError(tr("DoxyPressApp Startup"), tr("Unable to locate the DoxyPressApp settings file"));
      throw std::runtime_error("abort_no_message");
   }

   // set up help & validation
   createMap();

   // recent files
   rf_CreateMenus();

   // recent files, context menu
   m_ui->menuFile->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(m_ui->menuFile, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContext_Files(const QPoint &)));

// m_timer = new QTimer;
   m_runProcess = new QProcess;

   m_running = false;
   updateRunButtons();

   m_ui->setup_StackedWidget->setCurrentWidget(m_ui->page_Project);
   m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_General_A);
   m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_A);

   m_ui->tabWidget->setCurrentIndex(0);

   // screen setup
   createConnections();
   createShortCuts();

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
   msgB.setWindowIcon(QIcon(":/resources/doxypress.png"));

   msgB.setWindowTitle(tr("About DoxyPressApp"));
   msgB.setText(tr("<p style=margin-right:25><center><h5>Version: %1<br>Build # 3.01.2015</h5></center></p>").arg(versionString));
   msgB.setInformativeText(textBody);

   msgB.setStandardButtons(QMessageBox::Ok);
   msgB.setDefaultButton(QMessageBox::Ok);

   msgB.exec();
}

void MainWindow::createConnections()
{
   // menu options
   connect(m_ui->actionNew,           SIGNAL(triggered()), this, SLOT(newDoxy()));
   connect(m_ui->actionOpen,          SIGNAL(triggered()), this, SLOT(openDoxy()));
   connect(m_ui->actionReload,        SIGNAL(triggered()), this, SLOT(reloadDoxy()));

   connect(m_ui->actionSave,          SIGNAL(triggered()), this, SLOT(saveDoxy()));
   connect(m_ui->actionSave_As,       SIGNAL(triggered()), this, SLOT(saveDoxyAs()));
   connect(m_ui->actionExit,          SIGNAL(triggered()), this, SLOT(close()));

   connect(m_ui->actionImport,        &QAction::triggered, this, [this](bool){ importDoxy(); } );
   connect(m_ui->actionMove_Settings, &QAction::triggered, this, [this](bool){ move_Settings(); } );
   connect(m_ui->actionSave_Settings, &QAction::triggered, this, [this](bool){ save_Settings(); } );

   connect(m_ui->actionDoxyHelp,      SIGNAL(triggered()), this, SLOT(manual()));
   connect(m_ui->actionAbout,         SIGNAL(triggered()), this, SLOT(about()));

   // tabs
   connect(m_ui->setup_TreeWidget,    SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                                      SLOT(setupPage(QTreeWidgetItem *, QTreeWidgetItem *)));

   connect(m_ui->build_TreeWidget,    SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                                      SLOT(buildPage(QTreeWidgetItem *, QTreeWidgetItem *)));

   connect(m_ui->output_TreeWidget,   SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
                                      SLOT(outputPage(QTreeWidgetItem *, QTreeWidgetItem *)));


   // tab 1 - valid
   connect(m_ui->output_dir,          &QLineEdit::textChanged, this, [this](){ valid_output_dir(); } );

   connect(m_ui->buttonGroup_html,    SIGNAL(buttonClicked(QAbstractButton *)), this,
                                      SLOT(validSet_html(QAbstractButton *)));

   connect(m_ui->buttonGroup_latex,   SIGNAL(buttonClicked(QAbstractButton *)), this,
                                      SLOT(validSet_latex(QAbstractButton *)));

   connect(m_ui->buttonGroup_diagram, SIGNAL(buttonClicked(QAbstractButton *)), this,
                                      SLOT(validSet_dot(QAbstractButton *)));


   // tab 1 - lookups
   connect(m_ui->icon_PB,                   &QPushButton::clicked, this, [this](){ getIcon(); } );
   connect(m_ui->output_dir_PB,             SIGNAL(clicked()),     this, SLOT(output_dir_PB()));
   connect(m_ui->html_colors_PB,            SIGNAL(clicked()),     this, SLOT(tune_colors_PB()));

   // tab 2- look up (general)
   connect(m_ui->abbreviate_brief_PB,       SIGNAL(clicked()), this, SLOT(abbreviate_brief_PB()));
   connect(m_ui->strip_from_path_PB,        SIGNAL(clicked()), this, SLOT(strip_from_path_PB()));
   connect(m_ui->strip_from_inc_path_PB,    SIGNAL(clicked()), this, SLOT(strip_from_inc_path_PB()));
   connect(m_ui->aliases_PB,                SIGNAL(clicked()), this, SLOT(aliases_PB()));
   connect(m_ui->tcl_subst_PB,              SIGNAL(clicked()), this, SLOT(tcl_subst_PB()));
   connect(m_ui->extension_mapping_PB,      SIGNAL(clicked()), this, SLOT(extension_mapping_PB()));

   // tab 2- look up (build)
   connect(m_ui->enabled_sections_PB,       SIGNAL(clicked()), this, SLOT(enabled_sections_PB()));
   connect(m_ui->file_version_filter_PB,    SIGNAL(clicked()), this, SLOT(file_version_filter_PB()));
   connect(m_ui->layout_file_PB,            SIGNAL(clicked()), this, SLOT(layout_file_PB()));
   connect(m_ui->cite_bib_files_PB,         SIGNAL(clicked()), this, SLOT(cite_bib_files_PB()));

   // tab 2- look up (messages)
   connect(m_ui->warn_logfile_PB,           SIGNAL(clicked()), this, SLOT(warn_logfile_PB()));

   // tab 2- look up (input)
   connect(m_ui->input_source_PB,           SIGNAL(clicked()),   this, SLOT(input_source_PB()));
   connect(m_ui->file_patterns_PB,          SIGNAL(clicked()),   this, SLOT(file_patterns_PB()));
   connect(m_ui->exclude_files_PB,          SIGNAL(clicked()),   this, SLOT(exclude_files_PB()));
   connect(m_ui->exclude_patterns_PB,       SIGNAL(clicked()),   this, SLOT(exclude_patterns_PB()));
   connect(m_ui->exclude_symbols_PB,        SIGNAL(clicked()),   this, SLOT(exclude_symbols_PB()));

   connect(m_ui->example_source_PB,          SIGNAL(clicked()), this, SLOT(example_source_PB()));
   connect(m_ui->example_patterns_PB,        SIGNAL(clicked()), this, SLOT(example_patterns_PB()));
   connect(m_ui->image_path_PB,              SIGNAL(clicked()), this, SLOT(image_path_PB()));
   connect(m_ui->input_filter_PB,            SIGNAL(clicked()), this, SLOT(input_filter_PB()));
   connect(m_ui->filter_patterns_PB,         SIGNAL(clicked()), this, SLOT(filter_patterns_PB()));
   connect(m_ui->filter_source_patterns_PB,  SIGNAL(clicked()), this, SLOT(filter_source_patterns_PB()));

   // tab 2- look up (browser)
   connect(m_ui->clang_options_PB,          SIGNAL(clicked()), this, SLOT(clang_options_PB()));

   // tab 2- look up (index)
   connect(m_ui->ignore_prefix_PB,          SIGNAL(clicked()), this, SLOT(ignore_prefix_PB()));

   // tab 2- look up (preprocessor)
   connect(m_ui->include_path_PB,           SIGNAL(clicked()), this, SLOT(include_path_PB()));
   connect(m_ui->include_file_patterns_PB,  SIGNAL(clicked()), this, SLOT(include_file_patterns_PB()));
   connect(m_ui->predefined_macros_PB,      SIGNAL(clicked()), this, SLOT(predefined_macros_PB()));
   connect(m_ui->expand_as_defined_PB,      SIGNAL(clicked()), this, SLOT(expand_as_defined_PB()));

   // tab 2- look up (external)
   connect(m_ui->tag_files_PB,              SIGNAL(clicked()), this, SLOT(tag_files_PB()));
   connect(m_ui->gen_tagfile_PB,            SIGNAL(clicked()), this, SLOT(gen_tagfile_PB()));
   connect(m_ui->perl_path_PB,              SIGNAL(clicked()), this, SLOT(perl_path_PB()));

   // tab 2- look up (dot)
   connect(m_ui->mscgen_path_PB,            SIGNAL(clicked()), this, SLOT(mscgen_path_PB()));
   connect(m_ui->dia_path_PB,               SIGNAL(clicked()), this, SLOT(dia_path_PB()));
   connect(m_ui->dot_font_name_PB,          SIGNAL(clicked()), this, SLOT(dot_font_name_PB()));
   connect(m_ui->dot_font_path_PB,          SIGNAL(clicked()), this, SLOT(dot_font_path_PB()));

   connect(m_ui->dot_path_PB,               SIGNAL(clicked()), this, SLOT(dot_path_PB()));
   connect(m_ui->dot_file_dirs_PB,          SIGNAL(clicked()), this, SLOT(dot_file_dirs_PB()));
   connect(m_ui->msc_file_dirs_PB,          SIGNAL(clicked()), this, SLOT(msc_file_dirs_PB()));
   connect(m_ui->dia_file_dirs_PB,          SIGNAL(clicked()), this, SLOT(dia_file_dirs_PB()));
   connect(m_ui->plantuml_jar_path_PB,      SIGNAL(clicked()), this, SLOT(plantuml_jar_path_PB()));

   // tab 3 look up (html)
   connect(m_ui->html_output_PB,            SIGNAL(clicked()), this, SLOT(html_output_PB()));
   connect(m_ui->html_header_PB,            SIGNAL(clicked()), this, SLOT(html_header_PB()));
   connect(m_ui->html_footer_PB,            SIGNAL(clicked()), this, SLOT(html_footer_PB()));
   connect(m_ui->html_stylesheet_PB,        SIGNAL(clicked()), this, SLOT(html_stylesheet_PB()));
   connect(m_ui->html_extra_stylesheets_PB, SIGNAL(clicked()), this, SLOT(html_extra_stylesheets_PB()));
   connect(m_ui->html_extra_files_PB,       SIGNAL(clicked()), this, SLOT(html_extra_files_PB()));

   connect(m_ui->chm_file_PB,               SIGNAL(clicked()), this, SLOT(chm_file_PB()));
   connect(m_ui->hhc_location_PB,           SIGNAL(clicked()), this, SLOT(hhc_location_PB()));
   connect(m_ui->qch_file_PB,               SIGNAL(clicked()), this, SLOT(qch_file_PB()));
   connect(m_ui->qhg_location_PB,           SIGNAL(clicked()), this, SLOT(qhg_location_PB()));

   connect(m_ui->mathjax_extensions_PB,     SIGNAL(clicked()), this, SLOT(mathjax_extensions_PB()));
   connect(m_ui->search_data_file_PB,       SIGNAL(clicked()), this, SLOT(search_data_file_PB()));
   connect(m_ui->extra_search_mappings_PB,  SIGNAL(clicked()), this, SLOT(extra_search_mappings_PB()));

   // tab 3 look up (latex)
   connect(m_ui->latex_output_PB,           SIGNAL(clicked()), this, SLOT(latex_output_PB()));
   connect(m_ui->latex_cmd_name_PB,         SIGNAL(clicked()), this, SLOT(latex_cmd_name_PB()));
   connect(m_ui->make_index_cmd_name_PB,    SIGNAL(clicked()), this, SLOT(make_index_cmd_name_PB()));

   connect(m_ui->latex_extra_packages_PB,   SIGNAL(clicked()), this, SLOT(latex_extra_packages_PB()));
   connect(m_ui->latex_header_PB,           SIGNAL(clicked()), this, SLOT(latex_header_PB()));
   connect(m_ui->latex_footer_PB,           SIGNAL(clicked()), this, SLOT(latex_footer_PB()));
   connect(m_ui->latex_extra_files_PB,      SIGNAL(clicked()), this, SLOT(latex_extra_files_PB()));

   // tab 3 look up (rtf)
   connect(m_ui->rtf_output_PB,       SIGNAL(clicked()),   this, SLOT(rtf_output_PB()));
   connect(m_ui->rtf_stylesheet_PB,   SIGNAL(clicked()),   this, SLOT(rtf_stylesheet_PB()));
   connect(m_ui->rtf_extension_PB,    SIGNAL(clicked()),   this, SLOT(rtf_extension_PB()));

   // tab 3 look up (man)
   connect(m_ui->man_output_PB,       SIGNAL(clicked()),   this, SLOT(man_output_PB()));

   // tab 3 look up (xml)
   connect(m_ui->xml_output_PB,       SIGNAL(clicked()),   this, SLOT(xml_output_PB()));

   // tab 3 look up (docbook)
   connect(m_ui->docbook_output_PB,   SIGNAL(clicked()),   this, SLOT(docbook_output_PB()));

   // tab 4
   connect(m_runProcess,              SIGNAL(readyReadStandardOutput()),           this, SLOT(readStdout()));
   connect(m_runProcess,              SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(runComplete()));

   connect(m_ui->run_PB,              SIGNAL(clicked()), this,  SLOT(runDoxyPress()));
   connect(m_ui->display_PB,          SIGNAL(clicked()), this,  SLOT(showHtmlOutput()));
   connect(m_ui->save_log_PB,         SIGNAL(clicked()), this,  SLOT(saveLog()));

// connect(m_timer,         SIGNAL(timeout()), this,  SLOT(readStdout()));

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
   QDesktopServices::openUrl(QUrl("http://www.copperspice.org/documentation-doxypress.html"));
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

QSize MainWindow::sizeHint() const
{
   return QSize(1000, 600);
}


