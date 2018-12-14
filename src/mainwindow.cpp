/*************************************************************************
 *
 * Copyright (C) 2014-2018 Barbara Geller & Ansel Sermersheim
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

#include "doxy_build_info.h"
#include "mainwindow.h"

#include <QButtonGroup>
#include <QKeySequence>
#include <QPushButton>
#include <QShortcut>
#include <QTreeWidget>

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

      if (item->text(0) == "Dot") {
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
   connect(m_ui->menuFile, &QMenu::customContextMenuRequested, this, &MainWindow::showContext_Files);

   // syntax highlighting
   m_syntaxParser = new Syntax(m_ui->runText->document());

   m_runProcess = new QProcess;
   m_running = false;
   updateRunButtons();

   m_args.blank_layout = false;
   m_args.setDateTime  = false;
   m_args.deleteHtml   = false;

   m_ui->setup_StackedWidget->setCurrentWidget(m_ui->page_Project);
   m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Project_Config);
   m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_A);

   QTreeWidgetItem *setupItem = m_ui->setup_TreeWidget->topLevelItem(0);
   setupItem->setSelected(true);

   QTreeWidgetItem *buildItem = m_ui->build_TreeWidget->topLevelItem(0);
   buildItem->setSelected(true);

   QList<QTreeWidgetItem *> list = m_ui->output_TreeWidget->findItems("HTML", Qt::MatchStartsWith | Qt::MatchRecursive);

/*
   // should be four matches, use element 1
   QTreeWidgetItem *outputItem = list.at(1);
   outputItem->setSelected(true);
*/

   m_ui->tabWidget->setCurrentIndex(0);

   // shows the cursor and adjusts colors in the process window for searching
   m_ui->runText->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

   QPalette colors = m_ui->runText->palette();
   colors.setColor( QPalette::Highlight, QColor(248,233,96, 255));
   colors.setColor( QPalette::HighlightedText, Qt::black);
   m_ui->runText->setPalette(colors);

   //search
   QShortcut *s;
   s = new QShortcut(QKeySequence(QKeySequence::Find), this);
   connect(s, &QShortcut::activated, this, &MainWindow::find);

   s = new QShortcut(QKeySequence(QKeySequence::FindNext), this);
   connect(s, &QShortcut::activated, this, &MainWindow::findNext);

   // screen setup
   createConnections();
   createShortCuts();

   this->setMinimumHeight(500);

   setUnifiedTitleAndToolBarOnMac(true);
   setStatusBar(tr("Ready"));
}

void MainWindow::about()
{
   QString icon = ":/resources/doxypress.png";

   QString textBody =
      "<font color='#000080'><table style='margin-right:25'>"

      "<tr><td><img width='96' height='96'src='" + icon + "'></td>"
      "    <td>"
      "      <table style='margin-left:25; margin-top:15px' >"
      "        <tr><td><nobr>Developed by Barbara Geller</nobr></td><td>barbara@copperspice.com</td></tr>"
      "        <tr><td style=padding-right:25><nobr>Developed by Ansel Sermersheim</nobr></td><td>ansel@copperspice.com</td></tr>"
      "       </table>"
      "    </td>"
      "</tr></table></font>"
      "<p><nobr><small>Original development by Dimitri van Heesch</small></nobr></p>"
      "<p><small>Copyright 2014-2018 BG Consulting, released under the terms of the GNU GPL version 2<br>"
      "This program is provided AS IS with no warranty of any kind.<br></small></p>";

   QMessageBox msgB;
   msgB.setIcon(QMessageBox::NoIcon);
   msgB.setWindowIcon(QIcon(icon));

   msgB.setWindowTitle(tr("About DoxyPressApp"));
   msgB.setText(tr("<p style=margin-right:25><center><h5>Version: %1<br>Build # %2</h5></center></p>")
                  .formatArgs(QString::fromLatin1(versionString), QString::fromLatin1(buildDate)));

   msgB.setInformativeText(textBody);

   msgB.setStandardButtons(QMessageBox::Ok);
   msgB.setDefaultButton(QMessageBox::Ok);

   msgB.exec();
}

void MainWindow::createConnections()
{
   // menu options
   connect(m_ui->actionNew,           &QAction::triggered, this, &MainWindow::newDoxy);
   connect(m_ui->actionOpen,          &QAction::triggered, this, &MainWindow::openDoxy);
   connect(m_ui->actionReload,        &QAction::triggered, this, &MainWindow::reloadDoxy);

   connect(m_ui->actionSave,          &QAction::triggered, this, [this](){ saveDoxy();   });
   connect(m_ui->actionSave_As,       &QAction::triggered, this, [this](){ saveDoxyAs(); });
   connect(m_ui->actionExit,          &QAction::triggered, this, [this](){ close();      });

   connect(m_ui->actionImport,        &QAction::triggered, this, &MainWindow::importDoxy);
   connect(m_ui->actionEdit_Cfg,      &QAction::triggered, this, &MainWindow::edit_Cfg);
   connect(m_ui->actionMove_Cfg,      &QAction::triggered, this, &MainWindow::move_Cfg);
   connect(m_ui->actionSave_Cfg,      &QAction::triggered, this, &MainWindow::save_Cfg);

   connect(m_ui->actionDoxyHelp,      &QAction::triggered, this, &MainWindow::manual);
   connect(m_ui->actionAbout,         &QAction::triggered, this, &MainWindow::about);

   // tabs
   connect(m_ui->setup_TreeWidget,    &QTreeWidget::currentItemChanged, this, &MainWindow::setupPage);
   connect(m_ui->build_TreeWidget,    &QTreeWidget::currentItemChanged, this, &MainWindow::buildPage);
   connect(m_ui->output_TreeWidget,   &QTreeWidget::currentItemChanged, this, &MainWindow::outputPage);


   // tab 1
   connect(m_ui->output_dir,          &QLineEdit::textChanged, this, &MainWindow::valid_output_dir);
   connect(m_ui->project_logo,        &QLineEdit::textChanged, this, [this](){ getLogo("load"); } );

   connect(m_ui->gen_html_CB1,        &QGroupBox::toggled,   this, &MainWindow::valid_gen_html_1);
   connect(m_ui->gen_docbook_CB1,     &QCheckBox::toggled,   this, &MainWindow::valid_gen_docbook_1);
   connect(m_ui->gen_latex_CB1,       &QGroupBox::toggled,   this, &MainWindow::valid_gen_latex_1);
   connect(m_ui->gen_man_CB1,         &QCheckBox::toggled,   this, &MainWindow::valid_gen_man_1);
   connect(m_ui->gen_rtf_CB1,         &QCheckBox::toggled,   this, &MainWindow::valid_gen_rtf_1);
   connect(m_ui->gen_xml_CB1,         &QCheckBox::toggled,   this, &MainWindow::valid_gen_xml_1);

   connect(m_ui->buttonGroup_html,          static_cast<void (QButtonGroup:: *)(QAbstractButton *)> (&QButtonGroup::buttonClicked),
           this, &MainWindow::validSet_html);
   connect(m_ui->buttonGroup_latex,         static_cast<void (QButtonGroup:: *)(QAbstractButton *)> (&QButtonGroup::buttonClicked),
           this, &MainWindow::validSet_latex);
   connect(m_ui->buttonGroup_diagram,       static_cast<void (QButtonGroup:: *)(QAbstractButton *)> (&QButtonGroup::buttonClicked),
           this, &MainWindow::validSet_dot);

   // tab 2
   connect(m_ui->full_path_names_CB,        &QPushButton::toggled, this, &MainWindow::valid_full_path_names);
   connect(m_ui->markdown_CB,               &QPushButton::toggled, this, &MainWindow::valid_markdown);
   connect(m_ui->filter_source_files_CB,    &QPushButton::toggled, this, &MainWindow::valid_filter_source_files);
   connect(m_ui->alpha_index_CB,            &QPushButton::toggled, this, &MainWindow::valid_alpha_index);
   connect(m_ui->enable_preprocessing_CB,   &QPushButton::toggled, this, &MainWindow::valid_enable_preprocessing);
   connect(m_ui->clang_parsing_CB,          &QPushButton::toggled, this, &MainWindow::valid_clang);
   connect(m_ui->source_code_CB,            &QPushButton::toggled, this, &MainWindow::valid_source_listing);
   connect(m_ui->have_dot_CB,               &QPushButton::toggled, this, &MainWindow::valid_have_dot);

   // tab 3
   connect(m_ui->gen_html_CB2,              &QPushButton::toggled, this, &MainWindow::valid_gen_html);
   connect(m_ui->gen_chm_CB,                &QPushButton::toggled, this, &MainWindow::valid_gen_chm);
   connect(m_ui->gen_docbook_CB2,           &QPushButton::toggled, this, &MainWindow::valid_gen_docbook);
   connect(m_ui->gen_docset_CB,             &QPushButton::toggled, this, &MainWindow::valid_gen_docset);
   connect(m_ui->gen_eclipse_CB,            &QPushButton::toggled, this, &MainWindow::valid_gen_eclipse);
   connect(m_ui->gen_latex_CB2,             &QPushButton::toggled, this, &MainWindow::valid_gen_latex);
   connect(m_ui->gen_man_CB2,               &QPushButton::toggled, this, &MainWindow::valid_gen_man);
   connect(m_ui->gen_qthelp_CB,             &QPushButton::toggled, this, &MainWindow::valid_gen_qthelp);
   connect(m_ui->gen_rtf_CB2,               &QPushButton::toggled, this, &MainWindow::valid_gen_rtf);
   connect(m_ui->gen_xml_CB2,               &QPushButton::toggled, this, &MainWindow::valid_gen_xml);

   connect(m_ui->html_search_CB2,           &QPushButton::toggled, this, &MainWindow::valid_html_search);


   // ** lookups
   // tab 1
   connect(m_ui->project_logo_PB,           &QPushButton::clicked, this, [this](){ getLogo("select"); } );
   connect(m_ui->output_dir_PB,             &QPushButton::clicked, this, &MainWindow::output_dir_PB);
   connect(m_ui->html_colors_PB,            &QPushButton::clicked, this, &MainWindow::tune_colors_PB);

   // tab 2- general
   connect(m_ui->abbreviate_brief_PB,       &QPushButton::clicked, this, &MainWindow::abbreviate_brief_PB);
   connect(m_ui->strip_from_path_PB,        &QPushButton::clicked, this, &MainWindow::strip_from_path_PB);
   connect(m_ui->strip_from_inc_path_PB,    &QPushButton::clicked, this, &MainWindow::strip_from_inc_path_PB);
   connect(m_ui->aliases_PB,                &QPushButton::clicked, this, &MainWindow::aliases_PB);
   connect(m_ui->tcl_subst_PB,              &QPushButton::clicked, this, &MainWindow::tcl_subst_PB);
   connect(m_ui->language_mapping_PB,       &QPushButton::clicked, this, &MainWindow::language_mapping_PB);

   // tab 2- build
   connect(m_ui->enabled_sections_PB,       &QPushButton::clicked, this, &MainWindow::enabled_sections_PB);
   connect(m_ui->file_version_filter_PB,    &QPushButton::clicked, this, &MainWindow::file_version_filter_PB);
   connect(m_ui->main_page_name_PB,         &QPushButton::clicked, this, &MainWindow::main_page_name_PB);
   connect(m_ui->layout_file_PB,            &QPushButton::clicked, this, &MainWindow::layout_file_PB);
   connect(m_ui->ns_alias_PB,               &QPushButton::clicked, this, &MainWindow::ns_alias_PB);

   // tab 2- input
   connect(m_ui->input_source_PB,           &QPushButton::clicked, this, &MainWindow::input_source_PB);
   connect(m_ui->input_patterns_PB,         &QPushButton::clicked, this, &MainWindow::input_patterns_PB);
   connect(m_ui->exclude_files_PB,          &QPushButton::clicked, this, &MainWindow::exclude_files_PB);
   connect(m_ui->exclude_patterns_PB,       &QPushButton::clicked, this, &MainWindow::exclude_patterns_PB);
   connect(m_ui->exclude_symbols_PB,        &QPushButton::clicked, this, &MainWindow::exclude_symbols_PB);

   connect(m_ui->example_source_PB,         &QPushButton::clicked, this, &MainWindow::example_source_PB);
   connect(m_ui->example_patterns_PB,       &QPushButton::clicked, this, &MainWindow::example_patterns_PB);
   connect(m_ui->image_path_PB,             &QPushButton::clicked, this, &MainWindow::image_path_PB);
   connect(m_ui->mdfile_mainpage_PB,        &QPushButton::clicked, this, &MainWindow::mdfile_mainpage_PB);

   connect(m_ui->filter_program_PB,         &QPushButton::clicked, this, &MainWindow::filter_program_PB);
   connect(m_ui->filter_patterns_PB,        &QPushButton::clicked, this, &MainWindow::filter_patterns_PB);
   connect(m_ui->filter_source_patterns_PB, &QPushButton::clicked, this, &MainWindow::filter_source_patterns_PB);

   // tab 2- index
   connect(m_ui->ignore_prefix_PB,          &QPushButton::clicked, this, &MainWindow::ignore_prefix_PB);

   // tab 2- messages
   connect(m_ui->warn_logfile_PB,           &QPushButton::clicked, this, &MainWindow::warn_logfile_PB);

   // tab 2- preprocessor
   connect(m_ui->include_path_PB,           &QPushButton::clicked, this, &MainWindow::include_path_PB);
   connect(m_ui->include_patterns_PB,       &QPushButton::clicked, this, &MainWindow::include_file_patterns_PB);
   connect(m_ui->predefined_macros_PB,      &QPushButton::clicked, this, &MainWindow::predefined_macros_PB);
   connect(m_ui->expand_as_defined_PB,      &QPushButton::clicked, this, &MainWindow::expand_as_defined_PB);

   // tab 2 - clang
   connect(m_ui->clang_compilation_path,    &QLineEdit::textChanged, this, &MainWindow::valid_clang);
   connect(m_ui->clang_compilation_path_PB, &QPushButton::clicked,   this, &MainWindow::clang_compilation_path_PB);
   connect(m_ui->clang_flags_PB,            &QPushButton::clicked,   this, &MainWindow::clang_options_PB);

   // tab 2- source listings
   connect(m_ui->source_code_CB,            &QPushButton::toggled, this, &MainWindow::valid_source_listing);
   connect(m_ui->suffix_source_navtree_PB,  &QPushButton::clicked, this, &MainWindow::suffix_source_navtree_PB);
   connect(m_ui->suffix_header_navtree_PB,  &QPushButton::clicked, this, &MainWindow::suffix_header_navtree_PB);
   connect(m_ui->suffix_exclude_navtree_PB, &QPushButton::clicked, this, &MainWindow::suffix_exclude_navtree_PB);

   // tab 2- external
   connect(m_ui->tag_files_PB,              &QPushButton::clicked, this, &MainWindow::tag_files_PB);
   connect(m_ui->gen_tagfile_PB,            &QPushButton::clicked, this, &MainWindow::gen_tagfile_PB);
   connect(m_ui->perl_path_PB,              &QPushButton::clicked, this, &MainWindow::perl_path_PB);

   // tab 2- dot
   connect(m_ui->mscgen_path_PB,            &QPushButton::clicked, this, &MainWindow::mscgen_path_PB);
   connect(m_ui->dia_path_PB,               &QPushButton::clicked, this, &MainWindow::dia_path_PB);
   connect(m_ui->dot_font_name_PB,          &QPushButton::clicked, this, &MainWindow::dot_font_name_PB);
   connect(m_ui->dot_font_path_PB,          &QPushButton::clicked, this, &MainWindow::dot_font_path_PB);

   connect(m_ui->dot_path_PB,               &QPushButton::clicked, this, &MainWindow::dot_path_PB);
   connect(m_ui->dot_file_dirs_PB,          &QPushButton::clicked, this, &MainWindow::dot_file_dirs_PB);
   connect(m_ui->msc_file_dirs_PB,          &QPushButton::clicked, this, &MainWindow::msc_file_dirs_PB);
   connect(m_ui->dia_file_dirs_PB,          &QPushButton::clicked, this, &MainWindow::dia_file_dirs_PB);
   connect(m_ui->plantuml_jar_path_PB,      &QPushButton::clicked, this, &MainWindow::plantuml_jar_path_PB);
   connect(m_ui->plantuml_cfg_file_PB,      &QPushButton::clicked, this, &MainWindow::plantuml_cfg_file_PB);
   connect(m_ui->plantuml_inc_path_PB,      &QPushButton::clicked, this, &MainWindow::plantuml_inc_path_PB);

   // tab 3 html
   connect(m_ui->html_output_PB,            &QPushButton::clicked, this, &MainWindow::html_output_PB);
   connect(m_ui->html_header_PB,            &QPushButton::clicked, this, &MainWindow::html_header_PB);
   connect(m_ui->html_footer_PB,            &QPushButton::clicked, this, &MainWindow::html_footer_PB);
   connect(m_ui->html_stylesheets_PB,       &QPushButton::clicked, this, &MainWindow::html_stylesheets_PB);
   connect(m_ui->html_extra_files_PB,       &QPushButton::clicked, this, &MainWindow::html_extra_files_PB);

   connect(m_ui->ghostscript_PB,            &QPushButton::clicked, this, &MainWindow::ghostscript_PB);
   connect(m_ui->mathjax_extensions_PB,     &QPushButton::clicked, this, &MainWindow::mathjax_extensions_PB);
   connect(m_ui->search_data_file_PB,       &QPushButton::clicked, this, &MainWindow::search_data_file_PB);
   connect(m_ui->search_mappings_PB,        &QPushButton::clicked, this, &MainWindow::search_mappings_PB);

   // tab 3 chm
   connect(m_ui->chm_file_PB,               &QPushButton::clicked, this, &MainWindow::chm_file_PB);
   connect(m_ui->hhc_location_PB,           &QPushButton::clicked, this, &MainWindow::hhc_location_PB);

   // tab 3 docbook
   connect(m_ui->docbook_output_PB,         &QPushButton::clicked, this, &MainWindow::docbook_output_PB);

   // tab 3 latex
   connect(m_ui->latex_output_PB,           &QPushButton::clicked, this, &MainWindow::latex_output_PB);
   connect(m_ui->latex_cmd_name_PB,         &QPushButton::clicked, this, &MainWindow::latex_cmd_name_PB);
   connect(m_ui->make_index_cmd_name_PB,    &QPushButton::clicked, this, &MainWindow::make_index_cmd_name_PB);

   connect(m_ui->latex_extra_packages_PB,   &QPushButton::clicked, this, &MainWindow::latex_extra_packages_PB);
   connect(m_ui->latex_header_PB,           &QPushButton::clicked, this, &MainWindow::latex_header_PB);
   connect(m_ui->latex_footer_PB,           &QPushButton::clicked, this, &MainWindow::latex_footer_PB);
   connect(m_ui->latex_stylesheets_PB,      &QPushButton::clicked, this, &MainWindow::latex_stylesheets_PB);
   connect(m_ui->latex_extra_files_PB,      &QPushButton::clicked, this, &MainWindow::latex_extra_files_PB);
   connect(m_ui->cite_bib_files_PB,         &QPushButton::clicked, this, &MainWindow::cite_bib_files_PB);

   // tab 3 man
   connect(m_ui->man_output_PB,             &QPushButton::clicked, this, &MainWindow::man_output_PB);

   // tab 3 qthelp
   connect(m_ui->qch_file_PB,               &QPushButton::clicked, this, &MainWindow::qch_file_PB);
   connect(m_ui->qhp_cust_attrib_PB,        &QPushButton::clicked, this, &MainWindow::qhp_cust_attrib_PB);
   connect(m_ui->qhp_sect_attrib_PB,        &QPushButton::clicked, this, &MainWindow::qhp_sect_attrib_PB);
   connect(m_ui->qthelp_gen_path_PB,        &QPushButton::clicked, this, &MainWindow::qthelp_gen_path_PB);

   // tab 3 rtf
   connect(m_ui->rtf_output_PB,             &QPushButton::clicked, this, &MainWindow::rtf_output_PB);
   connect(m_ui->rtf_stylesheet_PB,         &QPushButton::clicked, this, &MainWindow::rtf_stylesheet_PB);
   connect(m_ui->rtf_extension_PB,          &QPushButton::clicked, this, &MainWindow::rtf_extension_PB);

   // tab 3 xml
   connect(m_ui->xml_output_PB,             &QPushButton::clicked, this, &MainWindow::xml_output_PB);

   // tab 4
   connect(m_runProcess,               &QProcess::readyReadStandardOutput, this, &MainWindow::readStdout);
   connect(m_runProcess,               static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                                       this, &MainWindow::runComplete);

   connect(m_ui->run_PB,               &QPushButton::clicked, this, &MainWindow::runDoxyPress);
   connect(m_ui->parameters_PB,        &QPushButton::clicked, this, &MainWindow::setArgs);
   connect(m_ui->display_PB,           &QPushButton::clicked, this, &MainWindow::showHtmlOutput);
   connect(m_ui->clear_PB,             &QPushButton::clicked, this, &MainWindow::clearOutput);
   connect(m_ui->find_PB,              &QPushButton::clicked, this, &MainWindow::find);
   connect(m_ui->save_log_PB,          &QPushButton::clicked, this, &MainWindow::saveLog);
}

void MainWindow::createShortCuts()
{
   struct {
      QKeySequence key_new;
      QKeySequence key_open;
      QKeySequence key_reload;
      QKeySequence key_save;
      QKeySequence key_saveAs;
      QKeySequence key_exit;
      QKeySequence key_DoxyHelp;
   }  struct_key;


   struct_key.key_new       = QKeySequence(QKeySequence::New);
   struct_key.key_open      = QKeySequence(QKeySequence::Open);
   struct_key.key_reload    = QKeySequence(QKeySequence::Refresh);
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

   m_ui->actionNew->setShortcut(QKeySequence(struct_key.key_new));
   m_ui->actionOpen->setShortcut(QKeySequence(struct_key.key_open));
   m_ui->actionReload->setShortcut(QKeySequence(struct_key.key_reload));
   m_ui->actionSave->setShortcut(QKeySequence(struct_key.key_save));
   m_ui->actionSave_As->setShortcut(QKeySequence(struct_key.key_saveAs));
   m_ui->actionExit->setShortcut(QKeySequence(struct_key.key_exit));
   m_ui->actionDoxyHelp->setShortcut(QKeySequence(struct_key.key_DoxyHelp));
}

void MainWindow::manual()
{
   QDesktopServices::openUrl(QUrl("http://www.copperspice.com/documentation-doxypress.html"));
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

      if (label == tr("Project Configuration")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Project_Config);

      } else  if (label == tr("Build Configuration")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Build_Config);

      } else if (label == tr("Build Options")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Build_Output);

      } else if (label == tr("Programming Languages")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Language);

      } else if (label == tr("Messages")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Messages);

      } else if (label == tr("Input Source Files")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Input_Source);

      } else if (label == tr("Input Other Files")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Input_Other);

      } else if (label == tr("Input Filters")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Input_Filters);

      } else if (label == tr("Index Page")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Index);

      } else if (label == tr("Preprocessor")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Process);

      } else if (label == tr("Clang Parsing")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Clang);

      } else if (label == tr("Source Listings")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Source);

      } else if (label == tr("External Programs")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_External);

      } else if (label == tr("Dot") || label == tr("Dot (Part 1)")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Dot_A);

      } else if (label == tr("Dot (Part 2)")) {
         m_ui->build_StackedWidget->setCurrentWidget(m_ui->page_Dot_B);

      }
   }
}

void MainWindow::outputPage(QTreeWidgetItem *item, QTreeWidgetItem *)
{
   if (item) {
      QString label = item->text(0);

      if (label == tr("HTML") || label == tr("HTML (Part 1)")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_A);

      } else if (label == tr("HTML (Part 2)")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_B);

      } else if (label == tr("HTML (Part 3)")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Html_C);

      } else if (label == tr("CHM")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Chm);

      } else if (label == tr("Docset")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Docset);

      } else if (label == tr("Eclipse")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Eclipse);

      } else if (label == tr("LaTeX")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Latex);

      } else if (label == tr("LaTeX Bibliography")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Latex_Bio);

      } else if (label == tr("Man")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Man);

      } else if (label == tr("Perl Module")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Perl);

      } else if (label == tr("QtHelp")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_QtHelp);

      } else if (label == tr("RTF")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Rtf);

      } else if (label == tr("XML")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Xml);

      } else if (label == tr("Docbook")) {
         m_ui->output_StackedWidget->setCurrentWidget(m_ui->page_Docbook);

      }
   }
}

QSize MainWindow::sizeHint() const
{
   // sets the recommended size
   return QSize(1000, 600);
}

