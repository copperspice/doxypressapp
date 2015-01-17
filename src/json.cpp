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

#include "dialog_selectcfg.h"
#include "mainwindow.h"

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QFlags>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QKeySequence>
#include <QPoint>
#include <QSettings>
#include <QStandardPaths>

#include <qglobal.h>

bool MainWindow::json_Read(Config trail)
{
   bool ok = true;

   m_appPath = QCoreApplication::applicationDirPath();

   QSettings settings("CS Doxygen", "Settings");
   m_jsonFname = settings.value("configName").toString();

   if (m_jsonFname.isEmpty() || ! QFile::exists(m_jsonFname)) {
      // get a new file name or location
      json_getFileName();

      if (m_jsonFname.isEmpty()) {
         return false;
      }
      settings.setValue("configName", m_jsonFname);

      if (! QFile::exists(m_jsonFname))  {
         json_CreateNew();
      }
   }

   if (ok) {

      // get existing json data
      QByteArray data = json_ReadFile();

      QJsonDocument doc = QJsonDocument::fromJson(data);

      QJsonObject object = doc.object();
      QJsonValue value;            
      QJsonArray list;

      int cnt;

      //
      value = object.value("pos-x");
      int x = value.toDouble();

      value = object.value("pos-y");
      int y = value.toDouble();

      QPoint pos = QPoint(x, y);
      move(pos);      

      //
      value = object.value("size-width");
      int width = value.toDouble();

      value = object.value("size-height");
      int height = value.toDouble();

      QSize size = QSize(width, height);
      resize(size);

      m_struct.pathPrior = object.value("pathPrior").toString();

      // recent files
      list = object.value("recent-files").toArray();
      cnt = list.count();

      for (int k = 0; k < cnt; k++)  {
//BROOM     m_rf_List.append(list.at(k).toString());
      }

      // opened files
      list = object.value("opened-files").toArray();
      cnt  = list.count();

      for (int k = 0; k < cnt; k++)  {
         QString fname = list.at(k).toString();

         if (! fname.isEmpty()) {
//BROOM     m_openedFiles.append(fname);
//BROOM     m_openedModified.append(false);
         }
      }
   }

   return ok;
}

bool MainWindow::json_Write(Option route, Config trail)
{     
   QSettings settings("CS Doxygen", "Settings");
   m_jsonFname = settings.value("configName").toString();

   if (m_jsonFname.isEmpty()) {
      // get a new file name
      json_getFileName();

      if (m_jsonFname.isEmpty()) {
         return false;
      }
      settings.setValue("configName", m_jsonFname);
   }

   if (true) {
      // get existing json data
      QByteArray data = json_ReadFile();

      if (data.isEmpty()) {
         csError("Save Configuration", "Configuration data is empty, aborting update...");
         return false;
      }

      QJsonDocument doc  = QJsonDocument::fromJson(data);
      QJsonObject object = doc.object();

      switch (route)  {

         case CLOSE:            
            object.insert("pos-x",       pos().x()  );
            object.insert("pos-y",       pos().y()  );
            object.insert("size-width",  size().width()  );
            object.insert("size-height", size().height() );

            {
              // opened files
              QJsonArray temp = QJsonArray::fromStringList(m_openedFiles);
              object.insert("opened-files", temp);
            }

            break;

          case PATH_PRIOR:
            object.insert("pathPrior", m_struct.pathPrior);
            break;

         case RECENTFILE:
            {
//BROOM        QJsonArray temp = QJsonArray::fromStringList(m_rf_List);
//BROOM        object.insert("recent-files", temp);
               break;
            }
      }

      // save the new data
      doc.setObject(object);
      data = doc.toJson();

      json_SaveFile(data);
   }

   return true;
}

void MainWindow::json_getFileName()
{   

#if defined(Q_OS_UNIX) && ! defined(Q_OS_MAC)

   QString homePath = QDir::homePath();
   m_jsonFname = homePath + "/.config/CS_Doxygen/wizard.json";
   
   return;

#elif defined(Q_OS_MAC)
   if (m_appPath.contains(".app/Contents/MacOS")) {
      QString homePath = QDir::homePath();      
      m_jsonFname = homePath + "/Library/CS_Doxygen/wizard.json";
     
      return;
   }
#endif         

   QString selectedFilter;       
   QFileDialog::Options options;

   Dialog_SelectCfg *dw = new Dialog_SelectCfg(this);
   int result = dw->exec();

   if (result == Dialog_SelectCfg::Result::SysDefault) {
      m_jsonFname = m_appPath + "/wizard.json";

#ifdef Q_OS_WIN
      QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
      m_jsonFname  = path + "/wizard.json";
#endif

   } else if (result == Dialog_SelectCfg::Result::Pick)  {
      QString fname = m_appPath + "/wizard.json";

      // force windows 7 and 8 to honor initial path
      options = QFileDialog::ForceInitialDir_Win7;

      m_jsonFname = QFileDialog::getSaveFileName(this, tr("Create new Configuration File"),
            fname, tr("Json Files (*.json)"), &selectedFilter, options);                  

   } else if (result == Dialog_SelectCfg::Result::Existing) {

      m_jsonFname = QFileDialog::getOpenFileName(this, tr("Select Existing CS Doxygen Configuration File"),
            "", tr("Json Files (*.json)"), &selectedFilter, options);

   } else {
      // user aborted
      m_jsonFname = "";

   }
}

QByteArray MainWindow::json_ReadFile()
{        
   QByteArray data;

   QFile file(m_jsonFname);
   if (! file.open(QFile::ReadWrite | QFile::Text)) {
      const QString msg = tr("Unable to open Configuration File: ") +  m_jsonFname + " : " + file.errorString();
      csError(tr("Read Json"), msg);
      return data;
   }

   file.seek(0);
   data = file.readAll();
   file.close();

   return data;
}

bool MainWindow::json_SaveFile(QByteArray data)
{
   QString path = pathName(m_jsonFname);
   QDir directory(path);

   if (! directory.exists()) {
      directory.mkpath(path);
   }

   QFile file(m_jsonFname);

   if (! file.open(QFile::ReadWrite | QFile::Truncate | QFile::Text)) {
      const QString msg = tr("Unable to save Configuration File: ") +  m_jsonFname + " : " + file.errorString();
      csError(tr("Save Json"), msg);
      return false;
   }

   file.seek(0);
   file.write(data);
   file.close();

   return true;
}

bool MainWindow::json_CreateNew()
{
   QJsonObject object;
   QJsonValue value;

   object.insert("pos-x",       400);
   object.insert("pos-y",       200);
   object.insert("size-width",  800);
   object.insert("size-height", 600);

   value = QJsonValue(m_appPath);
   object.insert("pathPrior", value);

   bool isAutoDetect = false;

#if defined(Q_OS_UNIX) && ! defined(Q_OS_MAC)

   isAutoDetect = true;

   QString resourcePath = m_appPath;
   QString libraryPath  = QDir::homePath() + "/.config/CS_Doxygen/";   

#elif defined(Q_OS_MAC)
   if (m_appPath.contains(".app/Contents/MacOS")) {
      isAutoDetect = true;

      QString resourcePath = this->pathName(m_appPath) + "/../Contents/Resources";
      QString libraryPath  = QDir::homePath() + "/Library/CS_Doxygen/";
   }
#endif

   if (! isAutoDetect) {
   }
     
   // save the data
   QJsonDocument doc(object);
   QByteArray data = doc.toJson();

   bool ok = json_SaveFile(data);

   return ok;
}

// **
void MainWindow::move_WizardCfg()
{
   QSettings settings("CS Doxygen", "Settings");
   m_jsonFname = settings.value("configName").toString();

   //
// BROOM   Dialog_Config *dw = new Dialog_Config(m_jsonFname);
// BROOM   int result = dw->exec();

   int result = 0;

   switch (result) {
      case QDialog::Rejected:
         break;

      case 1:
         // create
         {
            QString selectedFilter;
            QFileDialog::Options options;

            // force windows 7 and 8 to honor initial path
            options = QFileDialog::ForceInitialDir_Win7;

            QString newName = QFileDialog::getSaveFileName(this, tr("Create New Configuration File"),
                  m_appPath + "/wizard.json", tr("Json Files (*.json)"), &selectedFilter, options);

            if (newName.isEmpty()) {
               // do nothing

            } else if (QFile::exists(newName) ) {
               // can this happen?
               csError("CS Doxygen  Configuration", "Configuration file already exists, unable to create new file.");

            } else {
               m_jsonFname = newName;
               settings.setValue("configName", m_jsonFname);

               json_CreateNew();
               json_Read();

               // maybe add reset later
               csError("CS Doxygen  Configuration", "New configuration file selected."
                        " Restart CS Doxygen  to utilize the new configuration file settings.");
            }

            break;
         }

      case 2:
         // select
         {
            QString selectedFilter;
            QFileDialog::Options options;

            QString newName = QFileDialog::getOpenFileName(this, tr("Select CS Doxygen Configuration File"),
                  "*.json", tr("Json Files (*.json)"), &selectedFilter, options);

            if (newName.isEmpty()) {
               // do nothing

            } else if (QFile::exists(newName) ) {
               m_jsonFname = newName;
               settings.setValue("configName", m_jsonFname);

               json_Read();

               // maybe add reset later
               csError("CS Doxygen  Configuration", "New configuration file selected."
                        " Restart Diamond to utilize the new configuration file settings.");
            }

            break;
         }

      case 3:
         // rename
// BROOM         QString newName = dw->get_newName();

         QString newName = "";

         if (newName.isEmpty()) {
            csError("CS Doxygen Configuration", "No configuration file name specified, unable to rename.");

         } if (QFile::exists(newName) ) {
            csError("CS Doxygen Configuration", "New configuration file already exists, unable to rename.");

         } else  {

            QString path = pathName(newName);
            QDir directory(path);

            if (! directory.exists()) {
               directory.mkpath(path);
            }

            if (QFile::rename(m_jsonFname, newName)) {
               m_jsonFname = newName;
               settings.setValue("configName", m_jsonFname);

            } else {
               csError("CS Doxygen Configuration", "Configuration file rename failed.");

            }
         }

         break;
   }
}

void MainWindow::save_WizardCfg()
{
   json_Write(CLOSE);

   // make a back up
   bool isOk = true;

   QString backName = QFileInfo(m_jsonFname).filePath() + ".bak";
   QString tempName = QFileInfo(m_jsonFname).filePath() + ".xxx";

   if (QFile::exists(backName)) {

      QFile::remove(tempName);

      if (! QFile::rename(backName, tempName)) {
         isOk = false;
         csError("Configuration File", "Unble to save backup configuration");
      }
   }

   if (isOk) {
      QFile::copy(m_jsonFname, backName);
      QFile::remove(tempName);
   }
}


// **
void MainWindow::json_OpenDoxy(QByteArray data)
{
   QJsonDocument doc  = QJsonDocument::fromJson(data);
   QJsonObject object = doc.object();   

   // tab 1
   m_ui->project_name->setText( object.value("project-name").toString());
   m_ui->project_brief->setText( object.value("project-brief").toString());
   m_ui->project_number->setText( object.value("project-number").toString());

   m_project_iconFN = object.value("project-icon").toString();
   m_ui->output_dir->setText( object.value("output-dir").toString());

   m_ui->optimize_cplus_CB->setChecked( object.value("optimize-cplus").toBool());
   m_ui->optimize_cli_CB->setChecked( object.value("optimize-cli").toBool());
   m_ui->optimize_java_CB->setChecked( object.value("optimize-java").toBool());
   m_ui->optimize_c_CB->setChecked( object.value("optimize-c").toBool());
   m_ui->optimize_fortran_CB->setChecked( object.value("optimize-fortran").toBool());

   m_ui->gen_html_CB1->setChecked( object.value("generate-html").toBool());
   m_ui->html_plain_RB->setChecked( object.value("latex-plain").toBool());
   m_ui->html_nav_RB->setChecked( object.value("latex-nav").toBool());
   m_ui->html_chm_RB->setChecked( object.value("latex-chm").toBool());

   m_ui->searchEnabled_CB->setChecked( object.value("search-enabled").toBool());
   m_ui->html_colorstyle_hue->setValue( object.value("html-colorstyle-hue").toInt());
   m_ui->html_colorstyle_sat->setValue( object.value("html-colorstyle-sat").toInt());
   m_ui->html_colorstyle_gamma->setValue( object.value("html-colorstyle-gamma").toInt());

   m_ui->gen_latex_CB1->setChecked( object.value("generate-latex").toBool());
   m_ui->latex_hyper_pdf_RB->setChecked( object.value("latex-hyper_pdf").toBool());
   m_ui->latex_pdf_RB->setChecked( object.value("latex-pdf").toBool());
   m_ui->latex_ps_RB->setChecked( object.value("latex-ps").toBool());

   m_ui->gen_rtf_CB1->setChecked( object.value("generate-rtf").toBool());
   m_ui->gen_man_CB1->setChecked( object.value("generate-man").toBool());
   m_ui->gen_xml_CB1->setChecked( object.value("generate-xml").toBool());
   m_ui->gen_docbook_CB1->setChecked( object.value("generate-docbook").toBool());

   m_ui->diagram_none_RB->setChecked( object.value("diagram-none").toBool());
   m_ui->diagram_built_in_RB->setChecked( object.value("diagram-built-in").toBool());
   m_ui->diagram_dot_RB->setChecked( object.value("diagram-dot").toBool());

   m_ui->dot_class_CB->setChecked( object.value("dot-class").toBool());
   m_ui->dot_collaboration_CB->setChecked( object.value("dot-collaboration").toBool());
   m_ui->dot_overall_CB->setChecked( object.value("dot-overall").toBool());
   m_ui->dot_include_CB->setChecked( object.value("dot-include").toBool());
   m_ui->dot_included_by_CB->setChecked( object.value("dot-included-by").toBool());
   m_ui->dot_call_CB->setChecked( object.value("dot-call").toBool());
   m_ui->dot_called_by_CB->setChecked( object.value("dot-called-by").toBool());


   //  ***
   // tab 2 - project
   m_ui->project_encoding->setText(          object.value("project-encoding").toString());
   m_ui->create_subdirs_CB->setChecked(      object.value("create-subdirs").toBool());
   m_ui->allow_unicode_names_CB->setChecked( object.value("allow-unicode-names").toBool());



   // tab 2 - build


// done already
   m_ui->show_used_files_CB->setChecked( object.value("show-used-files").toBool());;
   m_ui->layout_file->setText(           object.value("layout-file").toString());
   m_ui->file_version_filter->setText(   object.value("file-version-filter").toString());




   // tab 2 - messages
   m_ui->quiet_CB->setChecked(           object.value("quiet").toBool());
   m_ui->warnings_CB->setChecked(        object.value("warnings").toBool());
   m_ui->warn_undoc_CB->setChecked(      object.value("warn-undoc").toBool());
   m_ui->warn_undoc_parm_CB->setChecked( object.value("warn-undoc-parm").toBool());
   m_ui->warn_forrmat->setText(          object.value("warn-forrmat").toString());
   m_ui->warn_logfile->setText(          object.value("warn-logfile").toString());

   // tab 2 -input
   m_ui->input_source->setPlainText(getDataList(  object, "input-source"));
   m_ui->input_encoding->setText(                 object.value("input-encoding").toString());
   m_ui->file_patterns->setPlainText(getDataList( object, "file-patterns"));
   m_ui->source_recursive_CB->setChecked(         object.value("source-recursive").toBool());

   m_ui->exclude_files->setPlainText(getDataList( object, "exclude-files"));
   m_ui->exclude_symlinks_CB->setChecked(         object.value("exclude-symlinks").toBool());
   m_ui->exclude_patterns->setPlainText(          getDataList(object, "exclude-patterns"));
   m_ui->exclude_symbols->setPlainText(           getDataList(object, "exclude-symbols"));

   m_ui->example_source->setPlainText(            getDataList(object, "example-source"));
   m_ui->example_patterns->setPlainText(          getDataList(object, "example-pattens"));
   m_ui->example_recursive_CB->setChecked(        object.value("example-recursive").toBool());

   m_ui->image_path->setPlainText(                getDataList(object, "image-path"));
   m_ui->input_filter->setText(                   object.value("input-filter").toString());
   m_ui->filter_patterns->setPlainText(           getDataList(object, "filter-patterns"));
   m_ui->filter_source_files_CB->setChecked(      object.value("filter-source-files").toBool());
   m_ui->filter_source_patterns->setPlainText(    getDataList(object, "filter-source-patterns"));

   m_ui->mdfile_mainpage->setText(                object.value("mdfile-mainpage").toString());

   // tab 2 -browser
   m_ui->source_browser_CB->setChecked(      object.value("source-browser").toBool());
   m_ui->inline_sources_CB->setChecked(      object.value("inline-sources").toBool());
   m_ui->strip_code_comments_CB->setChecked( object.value("strip-code-comments").toBool());
   m_ui->ref_by_relation_CB->setChecked(     object.value("ref-by-relation").toBool());
   m_ui->ref_relation_CB->setChecked(        object.value("ref-relation").toBool());
   m_ui->ref_link_source_CB->setChecked(     object.value("ref-link-source").toBool());
   m_ui->source_tooltips_CB->setChecked(     object.value("source-tooltips").toBool());
   m_ui->use_htags_CB->setChecked(           object.value("use-htags").toBool());
   m_ui->verbatim_headers_CB->setChecked(    object.value("verbatim-headers").toBool());
   m_ui->clang_parsing_CB->setChecked(       object.value("clang-parsing").toBool());
   m_ui->clang_options->setPlainText(        getDataList(object, "clang-options"));

   // tab 2 -index
   m_ui->alpha_index_CB->setChecked(     object.value("alpha-index").toBool());
   m_ui->cols_in_index_SB->setValue(     object.value("cols-in-index").toInt());
   m_ui->ignore_prefix->setPlainText(    getDataList(object, "ignore-prefix"));

   // tab 2 - autogen
   m_ui->gen_autogen_def_CB->setChecked( object.value("gen-autogen-def").toBool());

   // tab 2 - perlmod         
   m_ui->gen_perl_CB->setChecked(    object.value("gen-perl").toBool());
   m_ui->perl_latex_CB->setChecked(  object.value("perl-latex").toBool());
   m_ui->perl_pretty_CB->setChecked( object.value("perl-pretty").toBool());
   m_ui->perlmod_prefix->setText(    object.value("perlmod-prefix").toString());

   // tab 2 - preprocess
   m_ui->enable_preprocessing_CB->setChecked(  object.value("enable-preprocessing").toBool());
   m_ui->macro_expansion_CB->setChecked(       object.value("macro-expansion").toBool());
   m_ui->expand_only_redefined_CB->setChecked( object.value("expand-only-redefined").toBool());
   m_ui->search_includes_CB->setChecked(       object.value("search-includes").toBool());

   m_ui->include_path->setPlainText(           getDataList(object,"include-path"));
   m_ui->include_file_patterns->setPlainText(  getDataList(object,"include-file-patterns"));
   m_ui->predefined_macros->setPlainText(      getDataList(object,"predefined-macros"));
   m_ui->expand_as_defined->setPlainText(      getDataList(object,"expand-as-definedx"));

   m_ui->skip_function_macros_CB->setChecked(  object.value("skip-function-macros").toBool());

   // tab 2 - external
   m_ui->tag_files->setPlainText(         getDataList(object,"tag-files"));
   m_ui->generate_tagfile->setText(       object.value("generate-tagfile").toString());
   m_ui->all_externals_CB->setChecked(    object.value("all-externals").toBool());
   m_ui->external_groups_CB->setChecked(  object.value("external-groups").toBool());
   m_ui->external_pages_CB->setChecked(   object.value("external-pages").toBool());
   m_ui->perl_path->setText(              object.value("perl-path").toString());

   // tab 2 - dot




// START HERE

/*
   <tabstop>class_diagrams</tabstop>
   <tabstop>mscgen_path</tabstop>
   <tabstop>dia_path</tabstop>
   <tabstop>hide_undocumented_relations_CB</tabstop>
   <tabstop>have_dot_CB</tabstop>
   <tabstop>dot_num_threads_SP</tabstop>
   <tabstop>dot_font_name</tabstop>
   <tabstop>dot_font_size_SB</tabstop>
   <tabstop>dot_font_path</tabstop>
   <tabstop>class_graph_CB</tabstop>
   <tabstop>collaboration_graph_CB</tabstop>
   <tabstop>group_graphs_CB</tabstop>
   <tabstop>uml_look_CB</tabstop>
   <tabstop>uml_limit_num_fileds_SB</tabstop>
   <tabstop>template_relations_CB</tabstop>
   <tabstop>include_graph_CB</tabstop>
   <tabstop>included_by_graph_CB</tabstop>
   <tabstop>call_graph_CB</tabstop>
   <tabstop>ccaller_graph_CB</tabstop>
   <tabstop>graphical_hierarchy_CB</tabstop>
   <tabstop>directory_graph_CB</tabstop>
   <tabstop>dot_image_format_CM</tabstop>
   <tabstop>interactive_svg_CB</tabstop>

   <tabstop>destDir_121</tabstop>
   <tabstop>plainTextEdit_127</tabstop>
   <tabstop>plainTextEdit_128</tabstop>
   <tabstop>plainTextEdit_129</tabstop>
   <tabstop>destDir_180</tabstop>
   <tabstop>spinBox_14</tabstop>
   <tabstop>spinBox_15</tabstop>
   <tabstop>checkBox_690</tabstop>
   <tabstop>checkBox_688</tabstop>
   <tabstop>checkBox_689</tabstop>
   <tabstop>checkBox_687</tabstop>

*/







   //  ***
   // tab 3 - html
   m_ui->gen_html_CB2->setChecked(m_ui->gen_html_CB1->isChecked());


   // tab 3 - latex
   m_ui->gen_latex_CB2->setChecked(m_ui->gen_latex_CB1->isChecked());


   // tab 3 - rtf
   m_ui->gen_rtf_CB2->setChecked(m_ui->gen_rtf_CB1->isChecked());


   // tab 3 - man
   m_ui->gen_man_CB2->setChecked(m_ui->gen_man_CB1->isChecked());


   // tab 3 - xml
   m_ui->gen_xml_CB2->setChecked(m_ui->gen_xml_CB1->isChecked());


   // tab 3 - docbook
   m_ui->gen_docbook_CB2->setChecked(m_ui->gen_docbook_CB1->isChecked());

}

QByteArray MainWindow::json_SaveDoxy()
{
   QJsonObject object;

   object.insert("project-name",          m_ui->project_name->text());
   object.insert("project-brief",         m_ui->project_brief->text());
   object.insert("project-number",        m_ui->project_number->text());

   object.insert("project-icon",          m_project_iconFN);
   object.insert("output-dir",            m_ui->output_dir->text());

   object.insert("optimize-cplus",        m_ui->optimize_cplus_CB->isChecked());
   object.insert("optimize-cli",          m_ui->optimize_cli_CB->isChecked());
   object.insert("optimize-java",         m_ui->optimize_java_CB->isChecked());
   object.insert("optimize-c",            m_ui->optimize_c_CB->isChecked());
   object.insert("optimize-fortran",      m_ui->optimize_fortran_CB->isChecked());

   object.insert("generate-html",         m_ui->gen_html_CB1->isChecked());
   object.insert("html-plain",            m_ui->html_plain_RB->isChecked());
   object.insert("html-nav",              m_ui->html_nav_RB->isChecked());
   object.insert("html-chm",              m_ui->html_chm_RB->isChecked());

   object.insert("search-enabled",        m_ui->searchEnabled_CB->isChecked());
   object.insert("html-colorstyle-hue",   m_ui->html_colorstyle_hue->value());
   object.insert("html-colorstyle-sat",   m_ui->html_colorstyle_sat->value());
   object.insert("html-colorstyle-gamma", m_ui->html_colorstyle_gamma->value());

   object.insert("generate-latex",        m_ui->gen_latex_CB1->isChecked());
   object.insert("html-hyper-pdf",        m_ui->latex_hyper_pdf_RB->isChecked());
   object.insert("laxex-pdf",             m_ui->latex_pdf_RB->isChecked());
   object.insert("latex-ps",              m_ui->latex_ps_RB->isChecked());

   object.insert("generate-rtf",          m_ui->gen_rtf_CB1->isChecked());
   object.insert("generate-man",          m_ui->gen_man_CB1->isChecked());
   object.insert("generate-xml",          m_ui->gen_xml_CB1->isChecked());
   object.insert("generate-docbook",      m_ui->gen_docbook_CB1->isChecked());

   object.insert("diagram-none",          m_ui->diagram_none_RB->isChecked());
   object.insert("diagram-built-in",      m_ui->diagram_built_in_RB->isChecked());
   object.insert("diagram-dot",           m_ui->diagram_dot_RB->isChecked());


   object.insert("dot-class",             m_ui->dot_class_CB->isChecked());
   object.insert("dot-collaboratio",      m_ui->dot_collaboration_CB->isChecked());
   object.insert("dot-overall",           m_ui->dot_overall_CB->isChecked());
   object.insert("dot-include",           m_ui->dot_include_CB->isChecked());
   object.insert("dot-include-by",        m_ui->dot_included_by_CB->isChecked());
   object.insert("dot-call",              m_ui->dot_call_CB->isChecked());
   object.insert("dot-called-by",         m_ui->dot_called_by_CB->isChecked());


   //  ***
   // tab 2 - project



   // tab 2 - build




   // tab 2 - messages
   object.insert("quiet",                  m_ui->quiet_CB->isChecked());
   object.insert("warnings",               m_ui->warnings_CB->isChecked());
   object.insert("warn-undoc",             m_ui->warn_undoc_CB->isChecked());
   object.insert("warn-undoc-parm",        m_ui->warn_undoc_parm_CB->isChecked());
   object.insert("warn-forrmat",           m_ui->warn_forrmat->text());
   object.insert("warn-logfile",           m_ui->warn_logfile->text());

   // tab 2 - input

   object.insert("input-source",           putDataList(m_ui->input_source->toPlainText()));
   object.insert("input-encoding",         m_ui->input_encoding->text());
   object.insert("file-patterns",          putDataList(m_ui->file_patterns->toPlainText()));
   object.insert("source-recursive",       m_ui->source_recursive_CB->isChecked());

   object.insert("exclude-files",          putDataList(m_ui->exclude_files->toPlainText()));

/*
   m_ui->exclude_symlinks_CB->setChecked(         object.value("exclude-symlinks").toBool());
   m_ui->exclude_patterns->setPlainText(          getDataList(object, "exclude-patterns"));
   m_ui->exclude_symbols->setPlainText(           getDataList(object, "exclude-symbols"));

   m_ui->example_source->setPlainText(            getDataList(object, "example-source"));
   m_ui->example_patterns->setPlainText(          getDataList(object, "example-pattens"));
   m_ui->example_recursive_CB->setChecked(        object.value("example-recursive").toBool());

   m_ui->image_path->setPlainText(                getDataList(object, "image-path"));
   m_ui->input_filter->setText(                   object.value("input-filter").toString());
   m_ui->filter_patterns->setPlainText(           getDataList(object, "filter-patterns"));
   m_ui->filter_source_files_CB->setChecked(      object.value("filter-source-files").toBool());
   m_ui->filter_source_patterns->setPlainText(    getDataList(object, "filter-source-patterns"));

   m_ui->mdfile_mainpage->setText(                object.value("mdfile-mainpage").toString());

*/

   // tab 2 -browser

/*
   m_ui->source_browse_CB->setChecked(       object.value("source-browse").toBool());
   m_ui->inline_sources_CB->setChecked(      object.value("inline-sources").toBool());
   m_ui->strip_code_comments_CB->setChecked( object.value("strip-code-comments").toBool());
   m_ui->ref_by_relation_CB->setChecked(     object.value("ref-by-relation").toBool());
   m_ui->ref_relation_CB->setChecked(        object.value("ref-relation").toBool());
   m_ui->ref_link_source_CB->setChecked(     object.value("ref-link-source").toBool());
   m_ui->source_tooltips_CB->setChecked(     object.value("source-tooltips").toBool());
   m_ui->use_htags_CB->setChecked(           object.value("use-htags").toBool());
   m_ui->verbatim_headers_CB->setChecked(    object.value("verbatim-headers").toBool());
   m_ui->clang_parsing_CB->setChecked(       object.value("clang-parsing").toBool());
   m_ui->clang_options->setPlainText(        getDataList(object, "clang-options"));

*/


   // tab 2 -index
   object.insert("alpha-index",           m_ui->alpha_index_CB->isChecked());
   object.insert("cols-in-index",         m_ui->cols_in_index_SB->value());
   object.insert("ignore-prefix",         putDataList(m_ui->ignore_prefix->toPlainText()));

   // tab 2 - autogen
   object.insert("gen-autogen-def",       m_ui->gen_autogen_def_CB->isChecked());

   // tab 2 - perlmod
   object.insert("gen-perl",              m_ui->gen_perl_CB->isChecked());
   object.insert("perl-latex",            m_ui->perl_latex_CB->isChecked());
   object.insert("perl-pretty",           m_ui->perl_pretty_CB->isChecked());
   object.insert("perlmod-prefix",        m_ui->perlmod_prefix->text());

   // tab 2 - preprocess
   object.insert("enable-preprocessing",  m_ui->enable_preprocessing_CB->isChecked());
   object.insert("macro-expansion",       m_ui->macro_expansion_CB->isChecked());
   object.insert("expand-only-redefined", m_ui->expand_only_redefined_CB->isChecked());
   object.insert("search-includes",       m_ui->search_includes_CB->isChecked());

   object.insert("include-path",          putDataList(m_ui->include_path->toPlainText()));
   object.insert("include-file-patterns", putDataList(m_ui->include_file_patterns->toPlainText()));
   object.insert("predefined-macros",     putDataList(m_ui->predefined_macros->toPlainText()));
   object.insert("expand-as-definedx",    putDataList(m_ui->expand_as_defined->toPlainText()));

   object.insert("skip-function-macros",  m_ui->skip_function_macros_CB->isChecked());

   // tab 2 - external
   object.insert("tag-files",             putDataList(m_ui->tag_files->toPlainText()));
   object.insert("generate-tagfile",      m_ui->generate_tagfile->text());
   object.insert("all-externals",         m_ui->all_externals_CB->isChecked());
   object.insert("external-groups",       m_ui->external_groups_CB->isChecked());
   object.insert("external-pages",        m_ui->external_pages_CB->isChecked());
   object.insert("perl-path",             m_ui->perl_path->text());


   // tab 2 - dot




   //  ***
   // tab 3 - html


   // tab 3 - latex


   // tab 3 - rtf


   // tab 3 - man


   // tab 3 - xml


   // tab 3 - docbook




   // save the data
   QJsonDocument doc(object);
   QByteArray data = doc.toJson();

   return data;
}

QString MainWindow::getDataList(QJsonObject &object, QString fieldData)
{
   QString retval;
   QStringList dataList;

   QJsonArray list = object.value(fieldData).toArray();
   int cnt = list.count();

   for (int k = 0; k < cnt; k++)  {
      dataList.append(list.at(k).toString());
   }

   retval = dataList.join(", ");

   return retval;
}

QJsonArray MainWindow::putDataList(QString fieldData)
{
   QJsonArray list = QJsonArray();

   QStringList temp = fieldData.split(", ");

   for (auto s : temp) {
      list.append(s);
   }

   return list;
}





/*

  // tab 2 - project
  <tabstop>output_langugae_CM</tabstop>
  <tabstop>brief_member_desc_CB</tabstop>
  <tabstop>repeat_brief_CB</tabstop>
  <tabstop>abbreviate_brief</tabstop>
  <tabstop>always_detailed_sec_CB</tabstop>
  <tabstop>inline_inherited_member_CB</tabstop>
  <tabstop>full_path_names_CB</tabstop>
  <tabstop>strip_from_path</tabstop>
  <tabstop>strip_from_inc_path</tabstop>
  <tabstop>short_names_CB</tabstop>
  <tabstop>javadoc_auto_brief_CB</tabstop>
  <tabstop>qt_auto_brief_CB</tabstop>
  <tabstop>multiline_cpp_brief_CB</tabstop>
  <tabstop>inherit_docs_CB</tabstop>
  <tabstop>separate_member_pages_CB</tabstop>
  <tabstop>tab_size</tabstop>
  <tabstop>aliases</tabstop>
  <tabstop>tcl_subst</tabstop>
  <tabstop>extension_mapping</tabstop>
  <tabstop>markdown_CB</tabstop>
  <tabstop>auto_link_CB</tabstop>
  <tabstop>built_in_stl_support_CB</tabstop>
  <tabstop>cpp_cli_support_CB</tabstop>
  <tabstop>sip_support_CB</tabstop>
  <tabstop>idl_support_CB</tabstop>
  <tabstop>dist_group_doc_CB</tabstop>
  <tabstop>allow_sub_grouping_CB</tabstop>
  <tabstop>inline_grouped_classes_CB</tabstop>
  <tabstop>inline_simple_struct_CB</tabstop>
  <tabstop>typedef_hids_struct_CB</tabstop>
  <tabstop>lookup_cache_size</tabstop>

  // build
  <tabstop>extract_all</tabstop>
  <tabstop>extract_private</tabstop>
  <tabstop>extract_package</tabstop>
  <tabstop>extract_static</tabstop>
  <tabstop>extract_local_classes</tabstop>
  <tabstop>extract_local_methods</tabstop>
  <tabstop>extract_anon_namespaces</tabstop>
  <tabstop>hide_undoc_members</tabstop>
  <tabstop>hide_undoc_classes</tabstop>
  <tabstop>hide_friend_compounds</tabstop>
  <tabstop>hide_in_body_docs</tabstop>
  <tabstop>hide_scope_names</tabstop>
  <tabstop>internal_docs</tabstop>
  <tabstop>case_sense_names</tabstop>
  <tabstop>show_include_files</tabstop>
  <tabstop>show_grouped_members_incl</tabstop>
  <tabstop>force_local_includes</tabstop>
  <tabstop>inline_info</tabstop>
  <tabstop>sort_member_docs</tabstop>
  <tabstop>sort_brief_docs</tabstop>
  <tabstop>sort_members_ctors_first</tabstop>
  <tabstop>sort_group_names</tabstop>
  <tabstop>sort_by_scope_name</tabstop>
  <tabstop>strict_proto_matching</tabstop>
  <tabstop>gen_todo_list</tabstop>
  <tabstop>gen_test_list</tabstop>
  <tabstop>gen_bug_list</tabstop>
  <tabstop>gen_deprecatcated_list</tabstop>

  <tabstop>enabled_sections</tabstop>
  <tabstop>spinBox</tabstop>
  <tabstop>show_used_files_CB</tabstop>
  <tabstop>show_files_CB</tabstop>
  <tabstop>show_namespaces_CB</tabstop>
  <tabstop>file_version_filter</tabstop>
  <tabstop>layout_file</tabstop>
  <tabstop>cite_bib_files</tabstop>



  // ** TAB 3

  // tab 3 - html
  <tabstop>gen_html_CB2</tabstop>
  <tabstop>destDir_32</tabstop>  
  <tabstop>destDir_35</tabstop>
  <tabstop>destDir_39</tabstop>
  <tabstop>destDir_40</tabstop>
  <tabstop>destDir_52</tabstop>
  <tabstop>plainTextEdit_50</tabstop>
  <tabstop>plainTextEdit_51</tabstop>
  <tabstop>html_colorstyle_hue</tabstop>
  <tabstop>html_colorstyle_sat</tabstop>
  <tabstop>html_colorstyle_gamma</tabstop>
  <tabstop>checkBox_622</tabstop>
  <tabstop>checkBox_623</tabstop>
  <tabstop>spinBox_6</tabstop>
  <tabstop>gen_docset_CB</tabstop>
  <tabstop>destDir_56</tabstop>
  <tabstop>destDir_57</tabstop>
  <tabstop>destDir_58</tabstop>
  <tabstop>destDir_59</tabstop>
  <tabstop>checkBox_624</tabstop>
  <tabstop>destDir_55</tabstop>
  <tabstop>destDir_60</tabstop>
  <tabstop>checkBox_625</tabstop>
  <tabstop>destDir_61</tabstop>
  <tabstop>checkBox_626</tabstop>
  <tabstop>checkBox_627</tabstop>
  <tabstop>checkBox_628</tabstop>
  <tabstop>destDir_62</tabstop> 
  <tabstop>destDir_63</tabstop>
  <tabstop>destDir_64</tabstop>
  <tabstop>destDir_65</tabstop>
  <tabstop>destDir_66</tabstop>
  <tabstop>destDir_67</tabstop>
  <tabstop>destDir_71</tabstop>
  <tabstop>checkBox_629</tabstop>
  <tabstop>destDir_68</tabstop>
  <tabstop>checkBox_632</tabstop>
  <tabstop>checkBox_633</tabstop>
  <tabstop>spinBox_7</tabstop>
  <tabstop>spinBox_8</tabstop>
  <tabstop>checkBox_634</tabstop>
  <tabstop>spinBox_9</tabstop>
  <tabstop>checkBox_630</tabstop>
  <tabstop>checkBox_631</tabstop>
  <tabstop>comboBox_6</tabstop>
  <tabstop>destDir_69</tabstop>
  <tabstop>plainTextEdit_30</tabstop>
   <tabstop>destDir_70</tabstop>
  <tabstop>checkBox_635</tabstop>
  <tabstop>checkBox_636</tabstop>
  <tabstop>checkBox_637</tabstop>
  <tabstop>destDir_76</tabstop>
  <tabstop>lineEdit</tabstop>
  <tabstop>destDir_77</tabstop>
  <tabstop>plainTextEdit_32</tabstop>

  // latex
  <tabstop>gen_latex_CB2</tabstop>
  <tabstop>destDir_33</tabstop>
  <tabstop>destDir_78</tabstop>
  <tabstop>destDir_79</tabstop>
  <tabstop>checkBox_638</tabstop>
  <tabstop>comboBox_2</tabstop>
  <tabstop>plainTextEdit_33</tabstop>
  <tabstop>destDir_80</tabstop>
  <tabstop>destDir_81</tabstop>
  <tabstop>plainTextEdit_34</tabstop>
  <tabstop>pushButton_33</tabstop>
  <tabstop>checkBox_639</tabstop>
  <tabstop>checkBox_640</tabstop>
  <tabstop>checkBox_641</tabstop>
  <tabstop>checkBox_642</tabstop>
  <tabstop>checkBox_643</tabstop>
  <tabstop>destDir_83</tabstop>

  // rtf
  <tabstop>gen_rtf_CB2</tabstop>
  <tabstop>rtf_output</tabstop> 
  <tabstop>compact_rtf_CB</tabstop>
  <tabstop>rtf_hyperlinks_CB</tabstop>
  <tabstop>rtf_stylesheet</tabstop>
  <tabstop>rtf_extension</tabstop>

  // man
  <tabstop>gen_man_CB2</tabstop>
  <tabstop>man_output</tabstop> 
  <tabstop>man_extension</tabstop>
  <tabstop>man_subdir</tabstop>
  <tabstop>man_links_CB</tabstop>
  <tabstop>gen_xml_CB2</tabstop>

  // xml
  <tabstop>xml_output</tabstop>
  <tabstop>xml_program_llisting</tabstop>

  // docbook
  <tabstop>gen_docbook_CB2</tabstop>
  <tabstop>docbook_output</tabstop> 
  <tabstop>docbook_program_listing_CB

*/
