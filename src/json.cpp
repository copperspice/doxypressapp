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

   int index;

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
   m_ui->project_encoding->setText(                object.value("project-encoding").toString());
   m_ui->create_subdirs_CB->setChecked(            object.value("create-subdirs").toBool());
   m_ui->allow_unicode_names_CB->setChecked(       object.value("allow-unicode-names").toBool());

   index = m_ui->output_langugae_CM->findText(object.value("output-langugae").toString());
   m_ui->output_langugae_CM->setCurrentIndex(index);

   m_ui->brief_member_desc_CB->setChecked(         object.value("brief-member-desc").toBool());
   m_ui->repeat_brief_CB->setChecked(              object.value("repeat-brief").toBool());
   m_ui->abbreviate_brief->setPlainText(           getDataList(object, "abbreviate-brief"));
   m_ui->always_detailed_sec_CB->setChecked(       object.value("always-detailed_sec").toBool());
   m_ui->inline_inherited_member_CB->setChecked(   object.value("inline-inherited_member").toBool());
   m_ui->full_path_names_CB->setChecked(           object.value("full-path_names").toBool());
   m_ui->strip_from_path->setPlainText(            getDataList(object, "strip-from-path"));
   m_ui->strip_from_inc_path->setPlainText(        getDataList(object, "strip-from-inc-path"));

   m_ui->short_names_CB->setChecked(               object.value("short_names").toBool());
   m_ui->javadoc_auto_brief_CB->setChecked(        object.value("javadoc_auto_brief").toBool());
   m_ui->qt_auto_brief_CB->setChecked(             object.value("qt-auto-brief").toBool());
   m_ui->multiline_cpp_brief_CB->setChecked(       object.value("multiline-cpp-brief").toBool());
   m_ui->inherit_docs_CB->setChecked(              object.value("inherit-docs").toBool());
   m_ui->separate_member_pages_CB->setChecked(     object.value("separate-member-pages").toBool());

   m_ui->tab_size_SB->setValue(                    object.value("tab-size").toInt());
   m_ui->aliases->setPlainText(                    getDataList(object, "aliases"));
   m_ui->tcl_subst->setPlainText(                  getDataList(object, "tcl_subst"));
   m_ui->extension_mapping->setPlainText(          getDataList(object, "extension_mapping"));

   m_ui->markdown_CB->setChecked(                  object.value("markdown").toBool());
   m_ui->auto_link_CB->setChecked(                 object.value("auto_link").toBool());
   m_ui->built_in_stl_support_CB->setChecked(      object.value("built_in_stl_support").toBool());
   m_ui->cpp_cli_support_CB->setChecked(           object.value("cpp-cli_support").toBool());
   m_ui->sip_support_CB->setChecked(               object.value("sip-support").toBool());
   m_ui->idl_support_CB->setChecked(               object.value("idl-support").toBool());
   m_ui->dist_group_doc_CB->setChecked(            object.value("dist-group-doc").toBool());
   m_ui->allow_sub_grouping_CB->setChecked(        object.value("allow-sub-grouping").toBool());
   m_ui->inline_grouped_classes_CB->setChecked(    object.value("inline-grouped-classes").toBool());
   m_ui->inline_simple_struct_CB->setChecked(      object.value("inline-simple-struct").toBool());
   m_ui->typedef_hids_struct_CB->setChecked(       object.value("typedef-hids-struct").toBool());
   m_ui->lookup_cache_size_SB->setValue(           object.value("lookup-cache-size").toInt());

   // tab 2 - build
   m_ui->extract_all_CB->setChecked(               object.value("extract-all").toBool());
   m_ui->extract_private_CB->setChecked(           object.value("extract-private").toBool());
   m_ui->extract_package_CB->setChecked(           object.value("extract-package").toBool());
   m_ui->extract_static_CB->setChecked(            object.value("extract-static").toBool());
   m_ui->extract_local_classes_CB->setChecked(     object.value("extract-local-classes").toBool());
   m_ui->extract_local_methods_CB->setChecked(     object.value("extract-local-methods").toBool());
   m_ui->extract_anon_namespaces_CB->setChecked(   object.value("extract-anon-namespaces").toBool());
   m_ui->hide_undoc_members_CB->setChecked(        object.value("hide-undoc-members").toBool());
   m_ui->hide_undoc_classes_CB->setChecked(        object.value("hide-undoc-classes").toBool());
   m_ui->hide_friend_compounds_CB->setChecked(     object.value("hide-friend-compounds").toBool());
   m_ui->hide_in_body_docs_CB->setChecked(         object.value("hide-in_body-docs").toBool());
   m_ui->hide_scope_names_CB->setChecked(          object.value("hide-scope-names").toBool());

   m_ui->internal_docs_CB->setChecked(             object.value("internal-docs").toBool());
   m_ui->case_sense_names_CB->setChecked(          object.value("case-sense-names").toBool());
   m_ui->show_include_files_CB->setChecked(        object.value("show-include-files").toBool());
   m_ui->show_grouped_members_incl_CB->setChecked( object.value("show-grouped-members_incl").toBool());
   m_ui->force_local_includes_CB->setChecked(      object.value("force-local-includes").toBool());
   m_ui->inline_info_CB->setChecked(               object.value("inline-info").toBool());

   m_ui->sort_member_docs_CB->setChecked(          object.value("sort-member-docs").toBool());
   m_ui->sort_brief_docs_CB->setChecked(           object.value("sort-brief-docs").toBool());
   m_ui->sort_members_ctors_first_CB->setChecked(  object.value("sort-members-ctors-first").toBool());
   m_ui->sort_group_names_CB->setChecked(          object.value("sort-group-names").toBool());
   m_ui->sort_by_scope_name_CB->setChecked(        object.value("sort-by_scope-name").toBool());

   m_ui->strict_proto_matching_CB->setChecked(    object.value("strict_proto_matching").toBool());
   m_ui->gen_todo_list_CB->setChecked(            object.value("gen-todo-list").toBool());
   m_ui->gen_test_list_CB->setChecked(            object.value("gen-test-list").toBool());
   m_ui->gen_bug_list_CB->setChecked(             object.value("gen-bug-list").toBool());
   m_ui->gen_deprecatcated_list_CB->setChecked(   object.value("gen-deprecatcated-list").toBool());

   m_ui->enabled_sections->setPlainText(          getDataList(object, "enabled-sections"));
   m_ui->max_init_lines_SB->setValue(             object.value("max-init-lines").toInt());
   m_ui->show_used_files_CB->setChecked(          object.value("show-used-files").toBool());
   m_ui->show_files_CB->setChecked(               object.value("show-files").toBool());
   m_ui->show_namespaces_CB->setChecked(          object.value("show-namespaces").toBool());
   m_ui->file_version_filter->setText(            object.value("file-version-filter").toString());
   m_ui->layout_file->setText(                    object.value("layout-file").toString());
   m_ui->cite_bib_files->setPlainText(            getDataList(object, "cite-bib-files"));

   // tab 2 - messages
   m_ui->quiet_CB->setChecked(                 object.value("quiet").toBool());
   m_ui->warnings_CB->setChecked(              object.value("warnings").toBool());
   m_ui->warn_undoc_CB->setChecked(            object.value("warn-undoc").toBool());
   m_ui->warn_undoc_parm_CB->setChecked(       object.value("warn-undoc-parm").toBool());
   m_ui->warn_forrmat->setText(                object.value("warn-forrmat").toString());
   m_ui->warn_logfile->setText(                object.value("warn-logfile").toString());

   // tab 2 -input
   m_ui->input_source->setPlainText(           getDataList(object, "input-source"));
   m_ui->input_encoding->setText(              object.value("input-encoding").toString());
   m_ui->file_patterns->setPlainText(          getDataList(object, "file-patterns"));
   m_ui->source_recursive_CB->setChecked(      object.value("source-recursive").toBool());

   m_ui->exclude_files->setPlainText(          getDataList(object, "exclude-files"));
   m_ui->exclude_symlinks_CB->setChecked(      object.value("exclude-symlinks").toBool());
   m_ui->exclude_patterns->setPlainText(       getDataList(object, "exclude-patterns"));
   m_ui->exclude_symbols->setPlainText(        getDataList(object, "exclude-symbols"));

   m_ui->example_source->setPlainText(         getDataList(object, "example-source"));
   m_ui->example_patterns->setPlainText(       getDataList(object, "example-pattens"));
   m_ui->example_recursive_CB->setChecked(     object.value("example-recursive").toBool());

   m_ui->image_path->setPlainText(             getDataList(object, "image-path"));
   m_ui->input_filter->setText(                object.value("input-filter").toString());
   m_ui->filter_patterns->setPlainText(        getDataList(object, "filter-patterns"));
   m_ui->filter_source_files_CB->setChecked(   object.value("filter-source-files").toBool());
   m_ui->filter_source_patterns->setPlainText( getDataList(object, "filter-source-patterns"));

   m_ui->mdfile_mainpage->setText(             object.value("mdfile-mainpage").toString());

   // tab 2 -browser
   m_ui->source_browser_CB->setChecked(        object.value("source-browser").toBool());
   m_ui->inline_sources_CB->setChecked(        object.value("inline-sources").toBool());
   m_ui->strip_code_comments_CB->setChecked(   object.value("strip-code-comments").toBool());
   m_ui->ref_by_relation_CB->setChecked(       object.value("ref-by-relation").toBool());
   m_ui->ref_relation_CB->setChecked(          object.value("ref-relation").toBool());
   m_ui->ref_link_source_CB->setChecked(       object.value("ref-link-source").toBool());
   m_ui->source_tooltips_CB->setChecked(       object.value("source-tooltips").toBool());
   m_ui->use_htags_CB->setChecked(             object.value("use-htags").toBool());
   m_ui->verbatim_headers_CB->setChecked(      object.value("verbatim-headers").toBool());
   m_ui->clang_parsing_CB->setChecked(         object.value("clang-parsing").toBool());
   m_ui->clang_options->setPlainText(          getDataList(object, "clang-options"));

   // tab 2 -index
   m_ui->alpha_index_CB->setChecked(           object.value("alpha-index").toBool());
   m_ui->cols_in_index_SB->setValue(           object.value("cols-in-index").toInt());
   m_ui->ignore_prefix->setPlainText(          getDataList(object, "ignore-prefix"));

   // tab 2 - autogen
   m_ui->gen_autogen_def_CB->setChecked(       object.value("gen-autogen-def").toBool());

   // tab 2 - perlmod         
   m_ui->gen_perl_CB->setChecked(              object.value("gen-perl").toBool());
   m_ui->perl_latex_CB->setChecked(            object.value("perl-latex").toBool());
   m_ui->perl_pretty_CB->setChecked(           object.value("perl-pretty").toBool());
   m_ui->perlmod_prefix->setText(              object.value("perlmod-prefix").toString());

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
   m_ui->tag_files->setPlainText(              getDataList(object,"tag-files"));
   m_ui->generate_tagfile->setText(            object.value("generate-tagfile").toString());
   m_ui->all_externals_CB->setChecked(         object.value("all-externals").toBool());
   m_ui->external_groups_CB->setChecked(       object.value("external-groups").toBool());
   m_ui->external_pages_CB->setChecked(        object.value("external-pages").toBool());
   m_ui->perl_path->setText(                   object.value("perl-path").toString());

   // tab 2 - dot
   m_ui->class_diagrams_CB->setChecked(              object.value("class-diagrams").toBool());
   m_ui->mscgen_path->setText(                       object.value("mscgen-path").toString());
   m_ui->dia_path->setText(                          object.value("dia-path").toString());
   m_ui->hide_undocumented_relations_CB->setChecked( object.value("hide-undocumented-relations").toBool());
   m_ui->have_dot_CB->setChecked(                    object.value("have-dot").toBool());
   m_ui->dot_num_threads_SB->setValue(               object.value("dot-num-threads").toInt());
   m_ui->dot_font_name->setText(                     object.value("dot-font-name").toString());
   m_ui->dot_font_size_SB->setValue(                 object.value("dot-font-size").toInt());
   m_ui->dot_font_path->setText(                     object.value("dot_font_path").toString());
   m_ui->class_graph_CB->setChecked(                 object.value("class_graph").toBool());
   m_ui->collaboration_graph_CB->setChecked(         object.value("collaboration_graph").toBool());
   m_ui->group_graphs_CB->setChecked(                object.value("group_graphs").toBool());
   m_ui->uml_look_CB->setChecked(                    object.value("uml-look").toBool());
   m_ui->uml_limit_num_fields_SB->setValue(          object.value("uml-limit-num-fields").toInt());
   m_ui->template_relations_CB->setChecked(          object.value("template_relations").toBool());
   m_ui->include_graph_CB->setChecked(               object.value("include-graph").toBool());
   m_ui->included_by_graph_CB->setChecked(           object.value("included-by-graph").toBool());
   m_ui->call_graph_CB->setChecked(                  object.value("call-graph").toBool());
   m_ui->caller_graph_CB->setChecked(                object.value("caller-graph").toBool());
   m_ui->graphical_hierarchy_CB->setChecked(         object.value("graphical-hierarchy").toBool());
   m_ui->directory_graph_CB->setChecked(             object.value("directory-graph").toBool());

   index = m_ui->dot_image_format_CM->findText(      object.value("dot-image-format").toString());
   m_ui->dot_image_format_CM->setCurrentIndex(index);

   m_ui->interactive_svg_CB->setChecked(       object.value("interactive_svg").toBool());

   m_ui->dot_path->setText(                    object.value("dot-path").toString());
   m_ui->dot_file_dirs->setPlainText(          getDataList(object, "dot-file_dirs"));
   m_ui->msc_file_dirs->setPlainText(          getDataList(object, "msc-file_dirs"));
   m_ui->dia_file_dirs->setPlainText(          getDataList(object, "dia-file_dirs"));
   m_ui->platuml_jar_path->setText(            object.value("platuml-jar-path").toString());

   m_ui->dot_graph_max_nodes_SB->setValue(     object.value("dot_graph_max_nodes").toInt());
   m_ui->dot_graph_max_depth_SB->setValue(     object.value("dot_graph_max_depth").toInt());
   m_ui->dot_transparent_CB->setChecked(       object.value("dot_transparent").toBool());
   m_ui->dot_multple_targets_CB->setChecked(   object.value("dot_multple_targets").toBool());
   m_ui->gen_legend_CB->setChecked(            object.value("gen_legend").toBool());
   m_ui->dot_cleanup_CB->setChecked(           object.value("dot_cleanup").toBool());



/*

   m_ui->X_CB->setChecked(   object.value("").toBool());
   m_ui->X->setText(         object.value("").toString());
   m_ui->X_SB->setValue(     object.value("").toInt());
   m_ui->X->setPlainText(    getDataList(object, ""));

*/



   //  ***
   // tab 3 - html


/*
   <tabstop>gen_html_CB2</tabstop>
   <tabstop>gen_html_CB2</tabstop>
   <tabstop>html_ouput</tabstop>

   <tabstop>htnl_file_extension</tabstop>
   <tabstop>html_header</tabstop>

   <tabstop>html_footer</tabstop>

   <tabstop>html_stylesheet</tabstop>

   <tabstop>html_extra_stylesheet</tabstop>

   <tabstop>html_extra_file</tabstop>

   <tabstop>html_colorstyle_hue</tabstop>
   <tabstop>html_colorstyle_sat</tabstop>
   <tabstop>html_colorstyle_gamma</tabstop>
   <tabstop>html_timestamp_CB</tabstop>
   <tabstop>htl_synamic_sections_CB</tabstop>
   <tabstop>html_index_num_entries_SB</tabstop>
   <tabstop>gen_docset_CB</tabstop>
   <tabstop>docset_feedname</tabstop>
   <tabstop>docset_bundle_id</tabstop>
   <tabstop>docset_publisher_id</tabstop>
   <tabstop>docset_publisher_name</tabstop>
   <tabstop>gen_html_help</tabstop>
   <tabstop>chm_file</tabstop>

   <tabstop>hhc_location</tabstop>

   <tabstop>gen_chi_CB</tabstop>
   <tabstop>chm_index_encoding</tabstop>
   <tabstop>binary_toc_CB</tabstop>
   <tabstop>toc_expanded_CB</tabstop>
   <tabstop>gen_qhp_CB</tabstop>
   <tabstop>qch_file</tabstop>

   <tabstop>qhp_namespace</tabstop>
   <tabstop>qhp_virtual_folder</tabstop>
   <tabstop>qhp_cust_fiilter_name</tabstop>
   <tabstop>qhp_cust_filter_attrib</tabstop>
   <tabstop>qhp_section_filter_name</tabstop>
   <tabstop>qhg_location</tabstop>

   <tabstop>gen_eclipse_help_CB</tabstop>
   <tabstop>eclipse_doc_id</tabstop>
   <tabstop>disable_index_CB</tabstop>
   <tabstop>gen_treeview_CB</tabstop>
   <tabstop>enum_values_per_line_SB</tabstop>
   <tabstop>treeview_width_SB</tabstop>
   <tabstop>external_links_in_window_CB</tabstop>
   <tabstop>formula_fontsize_SB</tabstop>
   <tabstop>formula_transparent_CB</tabstop>
   <tabstop>use_mathjax_CB</tabstop>
   <tabstop>mathjax_format_CM</tabstop>
   <tabstop>mathjax_repath</tabstop>
   <tabstop>mathjax_extensions</tabstop>

   <tabstop>mathjax_codefile</tabstop>
   <tabstop>search_engine_CB</tabstop>
   <tabstop>server_based_search_CB</tabstop>
   <tabstop>external_search_CB</tabstop>
   <tabstop>search_engine_url</tabstop>
   <tabstop>search_data_file</tabstop>

   <tabstop>external_search_id</tabstop>
   <tabstop>extra_search_mappings</tabstop>
*/



   // tab 3 - latex


/*

  <tabstop>latex_output</tabstop>

  <tabstop>latex_cmd_name</tabstop>

  <tabstop>make_index_cmd_name</tabstop>

  <tabstop>compact_latex_CB</tabstop>
  <tabstop>paper_type_CM</tabstop>
  <tabstop>latex_extra_packages</tabstop>

  <tabstop>latex_header</tabstop>

  <tabstop>latex_footer</tabstop>

  <tabstop>latex_extra_files</tabstop>
  <tabstop>latex_extra_files_PB</tabstop>
  <tabstop>pdf_hyperlinks_CB</tabstop>
  <tabstop>use_pdf_latex_CB</tabstop>
  <tabstop>laxtex_batch_mode_CB</tabstop>
  <tabstop>laxtex_hide_indices_CB</tabstop>
  <tabstop>laxtex_source_code_CB</tabstop>
  <tabstop>laxtex_bib_style</tabstop>

*/


   // tab 3 - rtf
   m_ui->rtf_output->setText(                      object.value("rtf_output").toString());
   m_ui->rtf_compact_CB->setChecked(               object.value("rtf_compact").toBool());
   m_ui->rtf_hyperlinks_CB->setChecked(            object.value("rtf_hyperlinks").toBool());
   m_ui->rtf_stylesheet->setText(                  object.value("rtf_stylesheet").toString());
   m_ui->rtf_extension->setText(                   object.value("rtf_extension").toString());

   // tab 3 - man
   m_ui->man_output->setText(                      object.value("man-output").toString());
   m_ui->man_extension->setText(                   object.value("man-extension").toString());
   m_ui->man_subdir->setText(                      object.value("man-subdir").toString());
   m_ui->man_links_CB->setChecked(                 object.value("man-links").toBool());

   // tab 3 - xml
   m_ui->xml_output->setText(                      object.value("xml-output").toString());
   m_ui->xml_program_listing_CB->setChecked(       object.value("xml-program-listing").toBool());

   // tab 3 - docbook     
   m_ui->docbook_output->setText(                  object.value("docbook-output").toString());
   m_ui->docbook_program_listing_CB->setChecked(   object.value("docbook-program-listing").toBool());

   // set duplicate values
   setDuplicates();
}

QByteArray MainWindow::json_SaveDoxy()
{
   QJsonObject object;

   // tab 1
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
/*
   m_ui->project_encoding->setText(                object.value("project-encoding").toString());
   m_ui->create_subdirs_CB->setChecked(            object.value("create-subdirs").toBool());
   m_ui->allow_unicode_names_CB->setChecked(       object.value("allow-unicode-names").toBool());

   index = m_ui->output_langugae_CM->findText(object.value("output-langugae").toString());
   m_ui->output_langugae_CM->setCurrentIndex(index);

   m_ui->brief_member_desc_CB->setChecked(         object.value("brief-member-desc").toBool());
   m_ui->repeat_brief_CB->setChecked(              object.value("repeat-brief").toBool());
   m_ui->abbreviate_brief->setPlainText(           getDataList(object, "abbreviate-brief"));
   m_ui->always_detailed_sec_CB->setChecked(       object.value("always-detailed_sec").toBool());
   m_ui->inline_inherited_member_CB->setChecked(   object.value("inline-inherited_member").toBool());
   m_ui->full_path_names_CB->setChecked(           object.value("full-path_names").toBool());
   m_ui->strip_from_path->setPlainText(            getDataList(object, "strip-from-path"));
   m_ui->strip_from_inc_path->setPlainText(        getDataList(object, "strip-from-inc-path"));

   m_ui->short_names_CB->setChecked(               object.value("short_names").toBool());
   m_ui->javadoc_auto_brief_CB->setChecked(        object.value("javadoc_auto_brief").toBool());
   m_ui->qt_auto_brief_CB->setChecked(             object.value("qt-auto-brief").toBool());
   m_ui->multiline_cpp_brief_CB->setChecked(       object.value("multiline-cpp-brief").toBool());
   m_ui->inherit_docs_CB->setChecked(              object.value("inherit-docs").toBool());
   m_ui->separate_member_pages_CB->setChecked(     object.value("separate-member-pages").toBool());

   m_ui->tab_size_SB->setValue(                    object.value("tab-size").toInt());
   m_ui->aliases->setPlainText(                    getDataList(object, "aliases"));
   m_ui->tcl_subst->setPlainText(                  getDataList(object, "tcl_subst"));
   m_ui->extension_mapping->setPlainText(          getDataList(object, "extension_mapping"));

   m_ui->markdown_CB->setChecked(                  object.value("markdown").toBool());
   m_ui->auto_link_CB->setChecked(                 object.value("auto_link").toBool());
   m_ui->built_in_stl_support_CB->setChecked(      object.value("built_in_stl_support").toBool());
   m_ui->cpp_cli_support_CB->setChecked(           object.value("cpp-cli_support").toBool());
   m_ui->sip_support_CB->setChecked(               object.value("sip-support").toBool());
   m_ui->idl_support_CB->setChecked(               object.value("idl-support").toBool());
   m_ui->dist_group_doc_CB->setChecked(            object.value("dist-group-doc").toBool());
   m_ui->allow_sub_grouping_CB->setChecked(        object.value("allow-sub-grouping").toBool());
   m_ui->inline_grouped_classes_CB->setChecked(    object.value("inline-grouped-classes").toBool());
   m_ui->inline_simple_struct_CB->setChecked(      object.value("inline-simple-struct").toBool());
   m_ui->typedef_hids_struct_CB->setChecked(       object.value("typedef-hids-struct").toBool());
   m_ui->lookup_cache_size_SB->setValue(           object.value("lookup-cache-size").toInt());
*/

   // tab 2 - build
/*
   m_ui->extract_all_CB->setChecked(               object.value("extract-all").toBool());
   m_ui->extract_private_CB->setChecked(           object.value("extract-private").toBool());
   m_ui->extract_package_CB->setChecked(           object.value("extract-package").toBool());
   m_ui->extract_static_CB->setChecked(            object.value("extract-static").toBool());
   m_ui->extract_local_classe_CB->setChecked(      object.value("extract-local-classe").toBool());
   m_ui->extract_local_methods_CB->setChecked(     object.value("extract-local-methods").toBool());
   m_ui->extract_anon_namespaces_CB->setChecked(   object.value("extract-anon-namespaces").toBool());
   m_ui->hide_undoc_members_CB->setChecked(        object.value("hide-undoc-members").toBool());
   m_ui->hide_undoc_classes_CB->setChecked(        object.value("hide-undoc-classes").toBool());
   m_ui->hide_friend_compounds_CB->setChecked(     object.value("hide-friend-compounds").toBool());
   m_ui->hide_in_body_docs_CB->setChecked(         object.value("hide-in_body-docs").toBool());
   m_ui->hide_scope_names_CB->setChecked(          object.value("hide-scope-names").toBool());

   m_ui->internal_docs_CB->setChecked(             object.value("internal-docs").toBool());
   m_ui->case_sense_names_CB->setChecked(          object.value("case-sense-names").toBool());
   m_ui->show_include_files_CB->setChecked(        object.value("show-include-files").toBool());
   m_ui->show_grouped_members_incl_CB->setChecked( object.value("show-grouped-members_incl").toBool());
   m_ui->force_local_includes_CB->setChecked(      object.value("force-local-includes").toBool());
   m_ui->inline_info_CB->setChecked(               object.value("inline-info").toBool());

   m_ui->sort_member_docs_CB->setChecked(          object.value("sort-member-docs").toBool());
   m_ui->sort_brief_docs_CB->setChecked(           object.value("sort-brief-docs").toBool());
   m_ui->sort_members_ctors_first_CB->setChecked(  object.value("sort-members-ctors-first").toBool());
   m_ui->sort_group_names_CB->setChecked(          object.value("sort-group-names").toBool());
   m_ui->sort_by_scope_name_CB->setChecked(        object.value("sort-by_scope-name").toBool());

   m_ui->strict_proto_matching_CB->setChecked(    object.value("strict_proto_matching").toBool());
   m_ui->gen_todo_list_CB->setChecked(            object.value("gen-todo-list").toBool());
   m_ui->gen_test_list_CB->setChecked(            object.value("gen-test-list").toBool());
   m_ui->gen_bug_list_CB->setChecked(             object.value("gen-bug-list").toBool());
   m_ui->gen_deprecatcated_list_CB->setChecked(   object.value("gen-deprecatcated-list").toBool());

   m_ui->enabled_sections->setPlainText(          getDataList(object, "enabled-sections"));
   m_ui->max_init_lines_SB->setValue(             object.value("max-init-lines").toInt());
   m_ui->show_used_files_CB->setChecked(          object.value("show-used-files").toBool());
   m_ui->show_files_CB->setChecked(               object.value("show-files").toBool());
   m_ui->show_namespaces_CB->setChecked(          object.value("show-namespaces").toBool());
   m_ui->file_version_filter->setText(            object.value("file-version-filter").toString());
   m_ui->layout_file->setText(                    object.value("layout-file").toString());
   m_ui->cite_bib_files->setPlainText(            getDataList(object, "cite-bib-files"));
*/

   // tab 2 - messages
   object.insert("quiet",                      m_ui->quiet_CB->isChecked());
   object.insert("warnings",                   m_ui->warnings_CB->isChecked());
   object.insert("warn-undoc",                 m_ui->warn_undoc_CB->isChecked());
   object.insert("warn-undoc-parm",            m_ui->warn_undoc_parm_CB->isChecked());
   object.insert("warn-forrmat",               m_ui->warn_forrmat->text());
   object.insert("warn-logfile",               m_ui->warn_logfile->text());

   // tab 2 - input
   object.insert("input-source",               putDataList(m_ui->input_source->toPlainText()));
   object.insert("input-encoding",             m_ui->input_encoding->text());
   object.insert("file-patterns",              putDataList(m_ui->file_patterns->toPlainText()));
   object.insert("source-recursive",           m_ui->source_recursive_CB->isChecked());

   object.insert("exclude-files",              putDataList(m_ui->exclude_files->toPlainText()));
/* m_ui->exclude_symlinks_CB->setChecked(         object.value("exclude-symlinks").toBool());
   m_ui->exclude_patterns->setPlainText(          getDataList(object, "exclude-patterns"));
   m_ui->exclude_symbols->setPlainText(           getDataList(object, "exclude-symbols"));

   m_ui->example_source->setPlainText(            getDataList(object, "example-source"));
   m_ui->example_patterns->setPlainText(          getDataList(object, "example-pattens"));
   m_ui->example_recursive_CB->setChecked(        object.value("example-recursive").toBool());

   m_ui->image_path->setPlainText(                getDataList(object, "image-path"));
   m_ui->input_filter->text());                  object.value("input-filter").toString());
   m_ui->filter_patterns->setPlainText(           getDataList(object, "filter-patterns"));
   m_ui->filter_source_files_CB->setChecked(      object.value("filter-source-files").toBool());
   m_ui->filter_source_patterns->setPlainText(    getDataList(object, "filter-source-patterns"));

   m_ui->mdfile_mainpage->text());               object.value("mdfile-mainpage").toString());

*/

   // tab 2 -browser

/*
   m_ui->source_browse_CB->isChecked());       object.value("source-browse").toBool());
   m_ui->inline_sources_CB->isChecked());       object.value("inline-sources").toBool());
   m_ui->strip_code_comments_CB->isChecked());  object.value("strip-code-comments").toBool());
   m_ui->ref_by_relation_CB->isChecked());      object.value("ref-by-relation").toBool());
   m_ui->ref_relation_CB->isChecked());        object.value("ref-relation").toBool());
   m_ui->ref_link_source_CB->isChecked());     object.value("ref-link-source").toBool());
   m_ui->source_tooltips_CB->isChecked());      object.value("source-tooltips").toBool());
   m_ui->use_htags_CB->isChecked());            object.value("use-htags").toBool());
   m_ui->verbatim_headers_CB->isChecked());     object.value("verbatim-headers").toBool());
   m_ui->clang_parsing_CB->isChecked());       object.value("clang-parsing").toBool());
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
/*
   m_ui->class_diagrams_CB->isChecked());             object.value("class-diagrams").toBool());
   m_ui->mscgen_path->text());                      object.value("mscgen-path").toString());
   m_ui->dia_path->text());                          object.value("dia-path").toString());
   m_ui->hide_undocumented_relations_CB->setChecked( object.value("hide-undocumented-relations").toBool());
   m_ui->have_dot_CB->isChecked());                    object.value("have-dot").toBool());
   m_ui->dot_num_threads_SB->setValue(               object.value("dot-num-threads").toInt());
   m_ui->dot_font_name->text());                     object.value("dot-font-name").toString());
   m_ui->dot_font_size_SB->setValue(                 object.value("dot-font-size").toInt());
   m_ui->dot_font_path->text());                    object.value("dot_font_path").toString());
   m_ui->class_graph_CB->isChecked());               object.value("class_graph").toBool());
   m_ui->collaboration_graph_CB->isChecked());         object.value("collaboration_graph").toBool());
   m_ui->group_graphs_CB->isChecked());                object.value("group_graphs").toBool());
   m_ui->uml_look_CB->isChecked());                    object.value("uml-look").toBool());
   m_ui->uml_limit_num_fields_SB->setValue(          object.value("uml-limit-num-fields").toInt());
   m_ui->template_relations_CB->isChecked());          object.value("template_relations").toBool());
   m_ui->include_graph_CB->isChecked());               object.value("include-graph").toBool());
   m_ui->included_by_graph_CB->isChecked());           object.value("included-by-graph").toBool());
   m_ui->call_graph_CB->isChecked());                  object.value("call-graph").toBool());
   m_ui->caller_graph_CB->isChecked());               object.value("caller-graph").toBool());
   m_ui->graphical_hierarchy_CB->isChecked());         object.value("graphical-hierarchy").toBool());
   m_ui->directory_graph_CB->isChecked());             object.value("directory-graph").toBool());
*/


//   int index = m_ui->dot_image_format_CM->findText(  object.value("dot-image-format").toString());
//   m_ui->dot_image_format_CM->setCurrentIndex(index);

   object.insert("interactive-svg",            m_ui->interactive_svg_CB->isChecked());
   object.insert("dot-path",                   m_ui->dot_path->text());
   object.insert("dot-file-dirs",              putDataList(m_ui->dot_file_dirs->toPlainText()));
   object.insert("msc-file-dirs",              putDataList(m_ui->msc_file_dirs->toPlainText()));
   object.insert("dia-file-dirs",              putDataList(m_ui->dia_file_dirs->toPlainText()));
   object.insert("platuml-jar-path",           m_ui->platuml_jar_path->text());

   object.insert("dot-graph-max-nodes",        m_ui->dot_graph_max_nodes_SB->value());
   object.insert("dot-graph-max-depth",        m_ui->dot_graph_max_depth_SB->value());
   object.insert("dot-transparent",            m_ui->dot_transparent_CB->isChecked());
   object.insert("dot-multple-targets",        m_ui->dot_multple_targets_CB->isChecked());
   object.insert("gen-legend",                 m_ui->gen_legend_CB->isChecked());
   object.insert("dot-cleanup",                m_ui->dot_cleanup_CB->isChecked());

   //  ***
   // tab 3 - html



   // tab 3 - latex



   // tab 3 - rtf
   object.insert("rtf-output",                 m_ui->rtf_output->text());
   object.insert("rtf-compact",                m_ui->rtf_compact_CB->isChecked());
   object.insert("rtf-hyperlinks",             m_ui->rtf_hyperlinks_CB->isChecked());
   object.insert("rtf-stylesheet",             m_ui->rtf_stylesheet->text());
   object.insert("rtf-extension",              m_ui->rtf_extension->text());

   // tab 3 - man
   object.insert("man-output",                 m_ui->man_output->text());
   object.insert("man-extension",              m_ui->man_extension->text());
   object.insert("man-subdir",                 m_ui->man_subdir->text());
   object.insert("man-links",                  m_ui->man_links_CB->isChecked());

   // tab 3 - xml
   object.insert("xml-output",                 m_ui->xml_output->text());
   object.insert("xml-program-listing",        m_ui->xml_program_listing_CB->isChecked());

   // tab 3 - docbook
   object.insert("docbook-output",             m_ui->docbook_output->text());
   object.insert("docbook-program-listing",    m_ui->docbook_program_listing_CB->isChecked());

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

