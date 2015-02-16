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

#include "dialog_config.h"
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

   QSettings settings("DoxyPressApp", "Settings");
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
         m_rf_List.append(list.at(k).toString());
      }

      // opened files
      list = object.value("opened-files").toArray();
      cnt  = list.count();

      for (int k = 0; k < cnt; k++)  {
         QString fname = list.at(k).toString();

         if (! fname.isEmpty()) {
            m_openedFiles.append(fname);           
         }
      }
   }

   return ok;
}

bool MainWindow::json_Write(Option route, Config trail)
{     
   QSettings settings("DoxyPressApp", "Settings");
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
               QJsonArray temp = QJsonArray::fromStringList(m_rf_List);
               object.insert("recent-files", temp);
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
   m_jsonFname = homePath + "/.config/DoxyPressApp/DoxyPressApp.json";
   
   return;

#elif defined(Q_OS_MAC)
   if (m_appPath.contains(".app/Contents/MacOS")) {
      QString homePath = QDir::homePath();      
      m_jsonFname = homePath + "/Library/DoxyPressApp/DoxyPressApp.json";
     
      return;
   }
#endif         


   QString selectedFilter;       
   QFileDialog::Options options;

   Dialog_SelectCfg *dw = new Dialog_SelectCfg(this);
   int result = dw->exec();

   if (result == Dialog_SelectCfg::Result::SysDefault) {
      m_jsonFname = m_appPath + "/DoxyPressApp.json";

#ifdef Q_OS_WIN
      QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
      m_jsonFname  = path + "/DoxyPressApp.json";
#endif

   } else if (result == Dialog_SelectCfg::Result::Pick)  {
      QString fname = m_appPath + "/DoxyPressApp.json";

      // force windows 7 and 8 to honor initial path
      options = QFileDialog::ForceInitialDir_Win7;

      m_jsonFname = QFileDialog::getSaveFileName(this, tr("Create new DoxyPressApp Settings File"),
            fname, tr("Json Files (*.json)"), &selectedFilter, options);                  

   } else if (result == Dialog_SelectCfg::Result::Existing) {

      m_jsonFname = QFileDialog::getOpenFileName(this, tr("Select Existing DoxyPressApp Settings File"),
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
      const QString msg = tr("Unable to open Settings File: ") +  m_jsonFname + " : " + file.errorString();
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
      const QString msg = tr("Unable to save Settings File: ") +  m_jsonFname + " : " + file.errorString();
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
   QString libraryPath  = QDir::homePath() + "/.config/DoxyPressApp/";

#elif defined(Q_OS_MAC)
   if (m_appPath.contains(".app/Contents/MacOS")) {
      isAutoDetect = true;

      QString resourcePath = this->pathName(m_appPath) + "/../Contents/Resources";
      QString libraryPath  = QDir::homePath() + "/Library/DoxyPressApp/";
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
void MainWindow::move_Settings()
{
   QSettings settings("DoxyPressApp", "Settings");
   m_jsonFname = settings.value("configName").toString();

   //
   Dialog_Config *dw = new Dialog_Config(m_jsonFname);
   int result = dw->exec();

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

            QString newName = QFileDialog::getSaveFileName(this, tr("Create new DoxyPressApp Settings File"),
                  m_appPath + "/wizard.json", tr("Json Files (*.json)"), &selectedFilter, options);

            if (newName.isEmpty()) {
               // do nothing

            } else if (QFile::exists(newName) ) {
               // can this happen?
               csError("DoxyPressApp Settings", "File already exists, unable to create new file");

            } else {
               m_jsonFname = newName;
               settings.setValue("configName", m_jsonFname);

               json_CreateNew();
               json_Read();

               // maybe add reset later
               csError("DoxyPressApp Settings", "File selected, restart DoxyPressApp to utilize the new settings");
            }

            break;
         }

      case 2:
         // select
         {
            QString selectedFilter;
            QFileDialog::Options options;

            QString newName = QFileDialog::getOpenFileName(this, tr("Select DoxyPressApp Settings File"),
                  "*.json", tr("Json Files (*.json)"), &selectedFilter, options);

            if (newName.isEmpty()) {
               // do nothing

            } else if (QFile::exists(newName) ) {
               m_jsonFname = newName;
               settings.setValue("configName", m_jsonFname);

               json_Read();

               // maybe add reset later
               csError("DoxyPressApp Settings", "File selected, restart DoxyPressApp to utilize the new settings");
            }

            break;
         }

      case 3:
         // rename
         QString newName = dw->get_newName();

         if (newName.isEmpty()) {
            csError("DoxyPressApp Settings", "No settings file name specified, unable to rename");

         } if (QFile::exists(newName) ) {
            csError("DoxyPressApp Settings", "New settings file already exists, unable to rename");

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
               csError("DoxyPressApp Settings", "File rename failed");

            }
         }

         break;
   }
}

void MainWindow::save_Settings()
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
         csError("DoxyPressApp Settings File", "Unable to save backup settings");
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
   m_ui->project_name->setText(              object.value("project-name").toString());
   m_ui->project_brief->setText(             object.value("project-brief").toString());
   m_ui->project_number->setText(            object.value("project-number").toString());

   m_project_iconFN =                        object.value("project-icon").toString();
   m_ui->output_dir->setText(                object.value("output-dir").toString());

   m_ui->optimize_cplus_CB->setChecked(      object.value("optimize-cplus").toBool());  
   m_ui->optimize_java_CB->setChecked(       object.value("optimize-java").toBool());
   m_ui->optimize_c_CB->setChecked(          object.value("optimize-c").toBool());
   m_ui->optimize_fortran_CB->setChecked(    object.value("optimize-fortran").toBool());

   m_ui->gen_html_CB1->setChecked(           object.value("generate-html").toBool());
   m_ui->html_plain_RB->setChecked(          object.value("html-plain").toBool());
   m_ui->html_nav_RB->setChecked(            object.value("html-nav").toBool());
   m_ui->html_chm_RB->setChecked(            object.value("html-chm").toBool());

   // retrieve value from m_ui->search_engine_CB
   // m_ui->html_search_CB->setChecked(         object.value("html-search").toBool());

   m_ui->gen_latex_CB1->setChecked(          object.value("generate-latex").toBool());
   m_ui->latex_hyper_pdf_RB->setChecked(     object.value("latex-hyper-pdf").toBool());
   m_ui->latex_pdf_RB->setChecked(           object.value("latex-pdf").toBool());
   m_ui->latex_ps_RB->setChecked(            object.value("latex-ps").toBool());

   m_ui->gen_rtf_CB1->setChecked(            object.value("generate-rtf").toBool());
   m_ui->gen_man_CB1->setChecked(            object.value("generate-man").toBool());
   m_ui->gen_xml_CB1->setChecked(            object.value("generate-xml").toBool());
   m_ui->gen_docbook_CB1->setChecked(        object.value("generate-docbook").toBool());

   //
   m_ui->diagram_none_RB->setChecked(        object.value("diagram-none").toBool());
   m_ui->diagram_built_in_RB->setChecked(    object.value("diagram-built-in").toBool());
   m_ui->diagram_dot_RB->setChecked(         object.value("diagram-dot").toBool());

   m_ui->dot_class_graph_CB1->setChecked(    object.value("dot-class-graph").toBool());
   m_ui->dot_collaboration_CB1->setChecked(  object.value("dot-collaboration").toBool());
   m_ui->dot_hierarchy_CB1->setChecked(      object.value("dot-hierarchy").toBool());
   m_ui->dot_include_CB1->setChecked(        object.value("dot-include").toBool());
   m_ui->dot_included_by_CB1->setChecked(    object.value("dot-included-by").toBool());
   m_ui->dot_call_CB1->setChecked(           object.value("dot-call").toBool());
   m_ui->dot_called_by_CB1->setChecked(      object.value("dot-called-by").toBool());


   //  ***
   // tab 2 - project
   m_ui->project_encoding->setText(                object.value("project-encoding").toString());
   m_ui->create_subdirs_CB->setChecked(            object.value("create-subdirs").toBool());
   m_ui->allow_unicode_names_CB->setChecked(       object.value("allow-unicode-names").toBool());

   index = m_ui->output_language_CM->findText(     object.value("output-language").toString());
   m_ui->output_language_CM->setCurrentIndex(index);

   m_ui->brief_member_desc_CB->setChecked(         object.value("brief-member-desc").toBool());
   m_ui->repeat_brief_CB->setChecked(              object.value("repeat-brief").toBool());
   m_ui->abbreviate_brief->setPlainText(           getDataList(object, "abbreviate-brief"));
   m_ui->always_detailed_sec_CB->setChecked(       object.value("always-detailed-sec").toBool());
   m_ui->inline_inherited_member_CB->setChecked(   object.value("inline-inherited-member").toBool());
   m_ui->full_path_names_CB->setChecked(           object.value("full-path-names").toBool());
   m_ui->strip_from_path->setPlainText(            getDataList(object, "strip-from-path"));
   m_ui->strip_from_inc_path->setPlainText(        getDataList(object, "strip-from-inc-path"));

   m_ui->short_names_CB->setChecked(               object.value("short-names").toBool());
   m_ui->javadoc_auto_brief_CB->setChecked(        object.value("javadoc-auto-brief").toBool());
   m_ui->qt_auto_brief_CB->setChecked(             object.value("qt-auto-brief").toBool());
   m_ui->multiline_cpp_brief_CB->setChecked(       object.value("multiline-cpp-brief").toBool());
   m_ui->inherit_docs_CB->setChecked(              object.value("inherit-docs").toBool());
   m_ui->separate_member_pages_CB->setChecked(     object.value("separate-member-pages").toBool());

   m_ui->tab_size_SB->setValue(                    object.value("tab-size").toInt());
   m_ui->aliases->setPlainText(                    getDataList(object, "aliases"));
   m_ui->tcl_subst->setPlainText(                  getDataList(object, "tcl-subst"));
   m_ui->extension_mapping->setPlainText(          getDataList(object, "extension-mapping"));

   m_ui->markdown_CB->setChecked(                  object.value("markdown").toBool());
   m_ui->auto_link_CB->setChecked(                 object.value("auto-link").toBool());
   m_ui->built_in_stl_support_CB->setChecked(      object.value("built-in-stl-support").toBool());
   m_ui->cpp_cli_support_CB->setChecked(           object.value("cpp-cli-support").toBool());
   m_ui->sip_support_CB->setChecked(               object.value("sip-support").toBool());
   m_ui->idl_support_CB->setChecked(               object.value("idl-support").toBool());
   m_ui->dist_group_doc_CB->setChecked(            object.value("dist-group-doc").toBool());
   m_ui->allow_sub_grouping_CB->setChecked(        object.value("allow-sub-grouping").toBool());
   m_ui->inline_grouped_classes_CB->setChecked(    object.value("inline-grouped-classes").toBool());
   m_ui->inline_simple_struct_CB->setChecked(      object.value("inline-simple-struct").toBool());
   m_ui->typedef_hides_struct_CB->setChecked(      object.value("typedef-hides-struct").toBool());
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
   m_ui->hide_in_body_docs_CB->setChecked(         object.value("hide-in-body-docs").toBool());
   m_ui->hide_scope_names_CB->setChecked(          object.value("hide-scope-names").toBool());

   m_ui->internal_docs_CB->setChecked(             object.value("internal-docs").toBool());
   m_ui->case_sense_names_CB->setChecked(          object.value("case-sense-names").toBool());
   m_ui->show_include_files_CB->setChecked(        object.value("show-include-files").toBool());
   m_ui->show_grouped_members_inc_CB->setChecked(  object.value("show-grouped-members-inc").toBool());
   m_ui->force_local_includes_CB->setChecked(      object.value("force-local-includes").toBool());
   m_ui->inline_info_CB->setChecked(               object.value("inline-info").toBool());

   m_ui->sort_member_docs_CB->setChecked(          object.value("sort-member-docs").toBool());
   m_ui->sort_brief_docs_CB->setChecked(           object.value("sort-brief-docs").toBool());
   m_ui->sort_members_ctors_first_CB->setChecked(  object.value("sort-members-ctors-first").toBool());
   m_ui->sort_group_names_CB->setChecked(          object.value("sort-group-names").toBool());
   m_ui->sort_by_scope_name_CB->setChecked(        object.value("sort-by-scope-name").toBool());

   m_ui->strict_proto_matching_CB->setChecked(     object.value("strict-proto-matching").toBool());
   m_ui->gen_todo_list_CB->setChecked(             object.value("gen-todo-list").toBool());
   m_ui->gen_test_list_CB->setChecked(             object.value("gen-test-list").toBool());
   m_ui->gen_bug_list_CB->setChecked(              object.value("gen-bug-list").toBool());
   m_ui->gen_deprecate_list_CB->setChecked(        object.value("gen-deprecate-list").toBool());

   m_ui->enabled_sections->setPlainText(           getDataList(object, "enabled-sections"));
   m_ui->max_init_lines_SB->setValue(              object.value("max-init-lines").toInt());
   m_ui->show_used_files_CB->setChecked(           object.value("show-used-files").toBool());
   m_ui->show_files_CB->setChecked(                object.value("show-files").toBool());
   m_ui->show_namespaces_CB->setChecked(           object.value("show-namespaces").toBool());
   m_ui->file_version_filter->setText(             object.value("file-version-filter").toString());
   m_ui->layout_file->setText(                     object.value("layout-file").toString());
   m_ui->cite_bib_files->setPlainText(             getDataList(object, "cite-bib-files"));

   // tab 2 - messages
   m_ui->quiet_CB->setChecked(                     object.value("quiet").toBool());
   m_ui->warnings_CB->setChecked(                  object.value("warnings").toBool());
   m_ui->warn_undoc_CB->setChecked(                object.value("warn-undoc").toBool());
   m_ui->warn_doc_error_CB->setChecked(            object.value("warn-doc-error").toBool());
   m_ui->warn_undoc_parm_CB->setChecked(           object.value("warn-undoc-parm").toBool());
   m_ui->warn_format->setText(                     object.value("warn-format").toString());
   m_ui->warn_logfile->setText(                    object.value("warn-logfile").toString());

   // tab 2 -input
   m_ui->input_source->setPlainText(               getDataList(object, "input-source"));
   m_ui->input_encoding->setText(                  object.value("input-encoding").toString());
   m_ui->file_patterns->setPlainText(              getDataList(object, "file-patterns"));
   m_ui->source_recursive_CB->setChecked(          object.value("source-recursive").toBool());

   m_ui->exclude_files->setPlainText(              getDataList(object, "exclude-files"));
   m_ui->exclude_symlinks_CB->setChecked(          object.value("exclude-symlinks").toBool());
   m_ui->exclude_patterns->setPlainText(           getDataList(object, "exclude-patterns"));
   m_ui->exclude_symbols->setPlainText(            getDataList(object, "exclude-symbols"));
   m_ui->example_source->setPlainText(             getDataList(object, "example-source"));
   m_ui->example_patterns->setPlainText(           getDataList(object, "example-pattens"));
   m_ui->example_recursive_CB->setChecked(         object.value("example-recursive").toBool());

   m_ui->image_path->setPlainText(                 getDataList(object, "image-path"));
   m_ui->input_filter->setText(                    object.value("input-filter").toString());
   m_ui->filter_patterns->setPlainText(            getDataList(object, "filter-patterns"));
   m_ui->filter_source_files_CB->setChecked(       object.value("filter-source-files").toBool());
   m_ui->filter_source_patterns->setPlainText(     getDataList(object, "filter-source-patterns"));
   m_ui->mdfile_mainpage->setText(                 object.value("mdfile-mainpage").toString());

   // tab 2 -browser
   m_ui->source_browser_CB->setChecked(            object.value("source-browser").toBool());
   m_ui->inline_sources_CB->setChecked(            object.value("inline-sources").toBool());
   m_ui->strip_code_comments_CB->setChecked(       object.value("strip-code-comments").toBool());
   m_ui->ref_by_relation_CB->setChecked(           object.value("ref-by-relation").toBool());
   m_ui->ref_relation_CB->setChecked(              object.value("ref-relation").toBool());
   m_ui->ref_link_source_CB->setChecked(           object.value("ref-link-source").toBool());
   m_ui->source_tooltips_CB->setChecked(           object.value("source-tooltips").toBool());
   m_ui->use_htags_CB->setChecked(                 object.value("use-htags").toBool());
   m_ui->verbatim_headers_CB->setChecked(          object.value("verbatim-headers").toBool());
   m_ui->clang_parsing_CB->setChecked(             object.value("clang-parsing").toBool());
   m_ui->clang_options->setPlainText(              getDataList(object, "clang-options"));

   // tab 2 -index
   m_ui->alpha_index_CB->setChecked(               object.value("alpha-index").toBool());
   m_ui->cols_in_index_SB->setValue(               object.value("cols-in-index").toInt());
   m_ui->ignore_prefix->setPlainText(              getDataList(object, "ignore-prefix"));

   // tab 2 - autogen
   m_ui->gen_autogen_def_CB->setChecked(           object.value("gen-autogen-def").toBool());

   // tab 2 - perlmod         
   m_ui->gen_perl_CB->setChecked(                  object.value("gen-perl").toBool());
   m_ui->perl_latex_CB->setChecked(                object.value("perl-latex").toBool());
   m_ui->perl_pretty_CB->setChecked(               object.value("perl-pretty").toBool());
   m_ui->perlmod_prefix->setText(                  object.value("perlmod-prefix").toString());

   // tab 2 - preprocess
   m_ui->enable_preprocessing_CB->setChecked(      object.value("enable-preprocessing").toBool());
   m_ui->macro_expansion_CB->setChecked(           object.value("macro-expansion").toBool());
   m_ui->expand_only_predefined_CB->setChecked(    object.value("expand-only-predefined").toBool());
   m_ui->search_includes_CB->setChecked(           object.value("search-includes").toBool());

   m_ui->include_path->setPlainText(               getDataList(object,"include-path"));
   m_ui->include_file_patterns->setPlainText(      getDataList(object,"include-file-patterns"));
   m_ui->predefined_macros->setPlainText(          getDataList(object,"predefined-macros"));
   m_ui->expand_as_defined->setPlainText(          getDataList(object,"expand-as-defined"));
   m_ui->skip_function_macros_CB->setChecked(      object.value("skip-function-macros").toBool());

   // tab 2 - external
   m_ui->tag_files->setPlainText(                  getDataList(object,"tag-files"));
   m_ui->gen_tagfile->setText(                     object.value("generate-tagfile").toString());
   m_ui->all_externals_CB->setChecked(             object.value("all-externals").toBool());
   m_ui->external_groups_CB->setChecked(           object.value("external-groups").toBool());
   m_ui->external_pages_CB->setChecked(            object.value("external-pages").toBool());
   m_ui->perl_path->setText(                       object.value("perl-path").toString());

   // tab 2 - dot
   m_ui->class_diagrams_CB->setChecked(            object.value("class-diagrams").toBool());
   m_ui->mscgen_path->setText(                     object.value("mscgen-path").toString());
   m_ui->dia_path->setText(                        object.value("dia-path").toString());
   m_ui->hide_undoc_relations_CB->setChecked(      object.value("hide-undoc-relations").toBool());
   m_ui->have_dot_CB->setChecked(                  object.value("have-dot").toBool());
   m_ui->dot_num_threads_SB->setValue(             object.value("dot-num-threads").toInt());
   m_ui->dot_font_name->setText(                   object.value("dot-font-name").toString());
   m_ui->dot_font_size_SB->setValue(               object.value("dot-font-size").toInt());
   m_ui->dot_font_path->setText(                   object.value("dot-font-path").toString());

   m_ui->group_graphs_CB->setChecked(              object.value("group-graphs").toBool());
   m_ui->uml_look_CB->setChecked(                  object.value("uml-look").toBool());
   m_ui->uml_limit_num_fields_SB->setValue(        object.value("uml-limit-num-fields").toInt());      
   m_ui->template_relations_CB->setChecked(        object.value("template-relations").toBool());
   m_ui->directory_graph_CB->setChecked(           object.value("directory-graph").toBool());

   index = m_ui->dot_image_format_CM->findText(    object.value("dot-image-format").toString());
   m_ui->dot_image_format_CM->setCurrentIndex(index);

   m_ui->interactive_svg_CB->setChecked(           object.value("interactive_svg").toBool());
   m_ui->dot_path->setText(                        object.value("dot-path").toString());
   m_ui->dot_file_dirs->setPlainText(              getDataList(object, "dot-file_dirs"));
   m_ui->msc_file_dirs->setPlainText(              getDataList(object, "msc-file_dirs"));
   m_ui->dia_file_dirs->setPlainText(              getDataList(object, "dia-file_dirs"));
   m_ui->plantuml_jar_path->setText(               object.value("plantuml-jar-path").toString());

   m_ui->dot_graph_max_nodes_SB->setValue(         object.value("dot-graph_max-nodes").toInt());
   m_ui->dot_graph_max_depth_SB->setValue(         object.value("dot-graph-max-depth").toInt());
   m_ui->dot_transparent_CB->setChecked(           object.value("dot-transparent").toBool());
   m_ui->dot_multple_targets_CB->setChecked(       object.value("dot-multple-targets").toBool());
   m_ui->gen_legend_CB->setChecked(                object.value("gen-legend").toBool());
   m_ui->dot_cleanup_CB->setChecked(               object.value("dot-cleanup").toBool());

   //  ***
   // tab 3 - html
   m_ui->html_output->setText(                     object.value("html-output").toString());
   m_ui->html_file_extension->setText(             object.value("html-file-extension").toString());
   m_ui->html_header->setText(                     object.value("html-header").toString());
   m_ui->html_footer->setText(                     object.value("html-footer").toString());
   m_ui->html_stylesheet->setText(                 object.value("html-stylesheet").toString());
   m_ui->html_extra_stylesheets->setPlainText(      getDataList(object, "html-extra-stylesheets"));
   m_ui->html_extra_files->setPlainText(           getDataList(object, "html-extra-files"));

   m_ui->html_colorstyle_hue->setValue(            object.value("html-colorstyle-hue").toInt());
   m_ui->html_colorstyle_sat->setValue(            object.value("html-colorstyle-sat").toInt());
   m_ui->html_colorstyle_gamma->setValue(          object.value("html-colorstyle-gamma").toInt());

   m_ui->html_timestamp_CB->setChecked(            object.value("html-timestamp").toBool());
   m_ui->html_synamic_sections_CB->setChecked(     object.value("htl-synamic-sections").toBool());
   m_ui->html_index_num_entries_SB->setValue(      object.value("html-index-num-entries").toInt());
   m_ui->gen_docset_CB->setChecked(                object.value("gen-docset").toBool());
   m_ui->docset_feedname->setText(                 object.value("docset-feedname").toString());
   m_ui->docset_bundle_id->setText(                object.value("docset-bundle-id").toString());
   m_ui->docset_publisher_id->setText(             object.value("docset-publisher-id").toString());
   m_ui->docset_publisher_name->setText(           object.value("docset-publisher-name").toString());
   m_ui->gen_html_help_CB->setChecked(             object.value("gen-html-help").toBool());
   m_ui->chm_file->setText(                        object.value("chm-file").toString());

   m_ui->hhc_location->setText(                    object.value("hhc-location").toString());
   m_ui->gen_chi_CB->setChecked(                   object.value("gen-chi").toBool());
   m_ui->chm_index_encoding->setText(              object.value("chm-index-encoding").toString());
   m_ui->binary_toc_CB->setChecked(                object.value("binary-toc").toBool());
   m_ui->toc_expanded_CB->setChecked(              object.value("toc-expanded").toBool());
   m_ui->gen_qhp_CB->setChecked(                   object.value("gen-qhp").toBool());

   m_ui->qch_file->setText(                        object.value("qch-file").toString());
   m_ui->qhp_namespace->setText(                   object.value("qhp-namespace").toString());
   m_ui->qhp_virtual_folder->setText(              object.value("qhp-virtual-folder").toString());
   m_ui->qhp_cust_filter_name->setText(            object.value("qhp-cust-filter-name").toString());
   m_ui->qhp_cust_filter_attrib->setText(          object.value("qhp-cust-filter-attrib").toString());
   m_ui->qhp_section_filter_name->setText(         object.value("qhp-section-filter-name").toString());
   m_ui->qhg_location->setText(                    object.value("qhg-location").toString());

   m_ui->gen_eclipse_help_CB->setChecked(          object.value("gen-eclipse-help").toBool());
   m_ui->eclipse_doc_id->setText(                  object.value("eclipse-doc-id").toString());
   m_ui->disable_index_CB->setChecked(             object.value("disable-index").toBool());
   m_ui->gen_treeview_CB->setChecked(              object.value("gen-treeview").toBool());
   m_ui->enum_values_per_line_SB->setValue(        object.value("enum-values-per-line").toInt());
   m_ui->treeview_width_SB->setValue(              object.value("treeview-width").toInt());
   m_ui->external_links_in_window_CB->setChecked(  object.value("external-links-in-window").toBool());
   m_ui->formula_fontsize_SB->setValue(            object.value("formula-fontsize").toInt());
   m_ui->formula_transparent_CB->setChecked(       object.value("formula-transparent").toBool());

   m_ui->use_mathjax_CB->setChecked(               object.value("use_mathjax").toBool());

   index = m_ui->mathjax_format_CM->findText(      object.value("mathjax-format").toString());
   m_ui->mathjax_format_CM->setCurrentIndex(index);

   m_ui->mathjax_repath->setText(                  object.value("mathjax-repath").toString());
   m_ui->mathjax_extensions->setPlainText(         getDataList(object, "mathjax-extensions"));
   m_ui->mathjax_codefile->setText(                object.value("mathjax-codefile").toString());

   m_ui->search_engine_CB->setChecked(             object.value("search-engine").toBool());
   m_ui->server_based_search_CB->setChecked(       object.value("server-based-search").toBool());
   m_ui->external_search_CB->setChecked(           object.value("external-search").toBool());
   m_ui->search_engine_url->setText(               object.value("search-engine-url").toString());
   m_ui->search_data_file->setText(                object.value("search-data-file").toString());
   m_ui->external_search_id->setText(              object.value("external-search-id").toString());
   m_ui->extra_search_mappings->setPlainText(      getDataList(object, "extra-search-mappings"));

   // tab 3 - latex
   m_ui->latex_output->setText(                    object.value("latex-output").toString());
   m_ui->latex_cmd_name->setText(                  object.value("latex-cmd-name").toString());
   m_ui->make_index_cmd_name->setText(             object.value("make-index-cmd-name").toString());
   m_ui->compact_latex_CB->setChecked(             object.value("compact-latex").toBool());

   index = m_ui->paper_type_CM->findText(          object.value("paper-type").toString());
   m_ui->paper_type_CM->setCurrentIndex(index);

   m_ui->latex_extra_packages->setPlainText(       getDataList(object, "latex-extra-packages"));
   m_ui->latex_header->setText(                    object.value("latex-header").toString());
   m_ui->latex_footer->setText(                    object.value("latex-footer").toString());
   m_ui->latex_extra_files->setPlainText(          getDataList(object, "latex-extra-files"));

   m_ui->pdf_hyperlinks_CB->setChecked(            object.value("pdf-hyperlinks").toBool());
   m_ui->pdf_latex_CB->setChecked(                 object.value("pdf-latex").toBool());
   m_ui->latex_batch_mode_CB->setChecked(         object.value("latex-batch-mode").toBool());
   m_ui->latex_hide_indices_CB->setChecked(       object.value("latex-hide-indices").toBool());
   m_ui->latex_source_code_CB->setChecked(        object.value("latex-source-code").toBool());
   m_ui->latex_bib_style->setText(                object.value("latex-bib-style").toString());

   // tab 3 - rtf
   m_ui->rtf_output->setText(                      object.value("rtf-output").toString());
   m_ui->rtf_compact_CB->setChecked(               object.value("rtf-compact").toBool());
   m_ui->rtf_hyperlinks_CB->setChecked(            object.value("rtf-hyperlinks").toBool());
   m_ui->rtf_stylesheet->setText(                  object.value("rtf-stylesheet").toString());
   m_ui->rtf_extension->setText(                   object.value("rtf-extension").toString());

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
   getIcon("load");

   setDuplicates();
   validGet_html();
   validGet_latex();
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
   object.insert("optimize-java",         m_ui->optimize_java_CB->isChecked());
   object.insert("optimize-c",            m_ui->optimize_c_CB->isChecked());
   object.insert("optimize-fortran",      m_ui->optimize_fortran_CB->isChecked());

   object.insert("generate-html",         m_ui->gen_html_CB1->isChecked());
   object.insert("html-plain",            m_ui->html_plain_RB->isChecked());
   object.insert("html-nav",              m_ui->html_nav_RB->isChecked());
   object.insert("html-chm",              m_ui->html_chm_RB->isChecked());

   // retrieve value from m_ui->search_engine_CB
   // object.insert("html-search",           m_ui->html_search_CB->isChecked());

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

   object.insert("dot-class-graph",       m_ui->dot_class_graph_CB1->isChecked());
   object.insert("dot-collaboration",     m_ui->dot_collaboration_CB1->isChecked());
   object.insert("dot-overall",           m_ui->dot_hierarchy_CB1->isChecked());
   object.insert("dot-include",           m_ui->dot_include_CB1->isChecked());
   object.insert("dot-included-by",       m_ui->dot_included_by_CB1->isChecked());
   object.insert("dot-call",              m_ui->dot_call_CB1->isChecked());
   object.insert("dot-called-by",         m_ui->dot_called_by_CB1->isChecked());

   //  ***
   // tab 2 - project
   object.insert("project-encoding",         m_ui->project_encoding->text());
   object.insert("create-subdirs",           m_ui->create_subdirs_CB->isChecked());
   object.insert("allow-unicode-names",      m_ui->allow_unicode_names_CB->isChecked());
   object.insert("output-language",          m_ui->output_language_CM->currentText());
   object.insert("brief-member-desc",        m_ui->brief_member_desc_CB->isChecked());
   object.insert("repeat-brief",             m_ui->repeat_brief_CB->isChecked());

   object.insert("abbreviate-brief",         putDataList(m_ui->abbreviate_brief->toPlainText()));
   object.insert("always-detailed-sec",      m_ui->always_detailed_sec_CB->isChecked());
   object.insert("inline-inherited-member",  m_ui->inline_inherited_member_CB->isChecked());
   object.insert("full-path-names",          m_ui->full_path_names_CB->isChecked());
   object.insert("strip-from-path",          putDataList(m_ui->strip_from_path->toPlainText()));
   object.insert("strip-from-inc-path",      putDataList(m_ui->strip_from_inc_path->toPlainText()));

   object.insert("short-names",              m_ui->short_names_CB->isChecked());
   object.insert("javadoc-auto-brief",       m_ui->javadoc_auto_brief_CB->isChecked());
   object.insert("qt-auto-brief",            m_ui->qt_auto_brief_CB->isChecked());
   object.insert("multiline-cpp-brief",      m_ui->multiline_cpp_brief_CB->isChecked());
   object.insert("inherit-docs",             m_ui->inherit_docs_CB->isChecked());
   object.insert("separate-member-pages",    m_ui->separate_member_pages_CB->isChecked());
   object.insert("tab-size",                 m_ui->tab_size_SB->value());
   object.insert("aliases",                  putDataList(m_ui->aliases->toPlainText()));
   object.insert("tcl-subst",                putDataList(m_ui->tcl_subst->toPlainText()));
   object.insert("extension-mapping",        putDataList(m_ui->extension_mapping->toPlainText()));

   object.insert("markdown",                 m_ui->markdown_CB->isChecked());
   object.insert("auto-link",                m_ui->auto_link_CB->isChecked());
   object.insert("built-in-stl-support",     m_ui->built_in_stl_support_CB->isChecked());
   object.insert("cpp-cli-support",          m_ui->cpp_cli_support_CB->isChecked());
   object.insert("sip-support",              m_ui->sip_support_CB->isChecked());
   object.insert("idl-support",              m_ui->idl_support_CB->isChecked());
   object.insert("dist-group-doc",           m_ui->dist_group_doc_CB->isChecked());
   object.insert("allow-sub-grouping",       m_ui->allow_sub_grouping_CB->isChecked());
   object.insert("inline-grouped-classes",   m_ui->inline_grouped_classes_CB->isChecked());
   object.insert("inline-simple-struct",     m_ui->inline_simple_struct_CB->isChecked());
   object.insert("typedef-hides-struct",     m_ui->typedef_hides_struct_CB->isChecked());
   object.insert("lookup-cache-size",        m_ui->lookup_cache_size_SB->value());

   // tab 2 - build
   object.insert("extract-all",              m_ui->extract_all_CB->isChecked());
   object.insert("extract-private",          m_ui->extract_private_CB->isChecked());
   object.insert("extract-package",          m_ui->extract_package_CB->isChecked());
   object.insert("extract-static",           m_ui->extract_static_CB->isChecked());
   object.insert("extract-local-classes",    m_ui->extract_local_classes_CB->isChecked());
   object.insert("extract-local-methods",    m_ui->extract_local_methods_CB->isChecked());
   object.insert("extract-anon-namespaces",  m_ui->extract_anon_namespaces_CB->isChecked());
   object.insert("hide-undoc-members",       m_ui->hide_undoc_members_CB->isChecked());
   object.insert("hide-undoc-classes",       m_ui->hide_undoc_classes_CB->isChecked());
   object.insert("hide-friend-compounds",    m_ui->hide_friend_compounds_CB->isChecked());
   object.insert("hide-in-body-docs",        m_ui->hide_in_body_docs_CB->isChecked());
   object.insert("hide-scope-names",         m_ui->hide_scope_names_CB->isChecked());

   object.insert("internal-docs",            m_ui->internal_docs_CB->isChecked());
   object.insert("case-sense-names",         m_ui->case_sense_names_CB->isChecked());
   object.insert("show-include-files",       m_ui->show_include_files_CB->isChecked());
   object.insert("show-grouped-members-inc", m_ui->show_grouped_members_inc_CB->isChecked());
   object.insert("force-local-includes",     m_ui->force_local_includes_CB->isChecked());
   object.insert("inline-info",              m_ui->inline_info_CB->isChecked());

   object.insert("sort-member-docs",         m_ui->sort_member_docs_CB->isChecked());
   object.insert("sort-brief-docs",          m_ui->sort_brief_docs_CB->isChecked());
   object.insert("sort-members-ctors-first", m_ui->sort_members_ctors_first_CB->isChecked());
   object.insert("sort-group-names",         m_ui->sort_group_names_CB->isChecked());
   object.insert("sort-by-scope-name",       m_ui->sort_by_scope_name_CB->isChecked());

   object.insert("strict-proto-matching",    m_ui->strict_proto_matching_CB->isChecked());
   object.insert("generate-todo-list",       m_ui->gen_todo_list_CB->isChecked());
   object.insert("generate-test-list",       m_ui->gen_test_list_CB->isChecked());
   object.insert("generate-bug-list",        m_ui->gen_bug_list_CB->isChecked());
   object.insert("generate-deprecate-list",  m_ui->gen_deprecate_list_CB->isChecked());

   object.insert("enabled-sections",         putDataList(m_ui->enabled_sections->toPlainText()));
   object.insert("max-init-lines",           m_ui->max_init_lines_SB->value());
   object.insert("show-used-files",          m_ui->show_used_files_CB->isChecked());
   object.insert("show-files",               m_ui->show_files_CB->isChecked());
   object.insert("show-namespaces",          m_ui->show_namespaces_CB->isChecked());
   object.insert("file-version-filter",      m_ui->file_version_filter->text());
   object.insert("layout-file",              m_ui->layout_file->text());
   object.insert("cite-bib-files",           putDataList(m_ui->cite_bib_files->toPlainText()));

   // tab 2 - messages
   object.insert("quiet",                    m_ui->quiet_CB->isChecked());
   object.insert("warnings",                 m_ui->warnings_CB->isChecked());
   object.insert("warn-undoc",               m_ui->warn_undoc_CB->isChecked());
   object.insert("warn-doc-error",           m_ui->warn_doc_error_CB->isChecked());
   object.insert("warn-undoc-parm",          m_ui->warn_undoc_parm_CB->isChecked());
   object.insert("warn-format",              m_ui->warn_format->text());
   object.insert("warn-logfile",             m_ui->warn_logfile->text());

   // tab 2 - input
   object.insert("input-source",             putDataList(m_ui->input_source->toPlainText()));
   object.insert("input-encoding",           m_ui->input_encoding->text());
   object.insert("file-patterns",            putDataList(m_ui->file_patterns->toPlainText()));
   object.insert("source-recursive",         m_ui->source_recursive_CB->isChecked());

   object.insert("exclude-files",            putDataList(m_ui->exclude_files->toPlainText()));
   object.insert("exclude-symlinks",         m_ui->exclude_symlinks_CB->isChecked());
   object.insert("exclude-patterns",         putDataList(m_ui->exclude_patterns->toPlainText()));
   object.insert("exclude-symbols",          putDataList(m_ui->exclude_symbols->toPlainText()));
   object.insert("example-source",           putDataList(m_ui->example_source->toPlainText()));
   object.insert("example-pattens",          putDataList(m_ui->example_patterns->toPlainText()));
   object.insert("example-recursive",        m_ui->example_recursive_CB->isChecked());

   object.insert("image-path",               putDataList(m_ui->image_path->toPlainText()));
   object.insert("input-filter",             m_ui->input_filter->text());
   object.insert("filter-patterns",          putDataList(m_ui->filter_patterns->toPlainText()));
   object.insert("filter-source-files",      m_ui->filter_source_files_CB->isChecked());
   object.insert("filter-source-patterns",   putDataList(m_ui->filter_source_patterns->toPlainText()));
   object.insert("mdfile-mainpage",          m_ui->mdfile_mainpage->text());

   // tab 2 -browser
   object.insert("source-browser",           m_ui->source_browser_CB->isChecked());
   object.insert("inline-sources",           m_ui->inline_sources_CB->isChecked());
   object.insert("strip-code-comments",      m_ui->strip_code_comments_CB->isChecked());
   object.insert("ref-by-relation",          m_ui->ref_by_relation_CB->isChecked());
   object.insert("ref-relation",             m_ui->ref_relation_CB->isChecked());
   object.insert("ref-link-source",          m_ui->ref_link_source_CB->isChecked());
   object.insert("source-tooltips",          m_ui->source_tooltips_CB->isChecked());
   object.insert("use-htags",                m_ui->use_htags_CB->isChecked());
   object.insert("verbatim-headers",         m_ui->verbatim_headers_CB->isChecked());
   object.insert("clang-parsing",            m_ui->clang_parsing_CB->isChecked());
   object.insert("clang-options",            putDataList(m_ui->clang_options->toPlainText()));

   // tab 2 -index
   object.insert("alpha-index",              m_ui->alpha_index_CB->isChecked());
   object.insert("cols-in-index",            m_ui->cols_in_index_SB->value());
   object.insert("ignore-prefix",            putDataList(m_ui->ignore_prefix->toPlainText()));

   // tab 2 - autogen
   object.insert("generate-autogen-def",    m_ui->gen_autogen_def_CB->isChecked());

   // tab 2 - perlmod
   object.insert("generate-perl",            m_ui->gen_perl_CB->isChecked());
   object.insert("perl-latex",               m_ui->perl_latex_CB->isChecked());
   object.insert("perl-pretty",              m_ui->perl_pretty_CB->isChecked());
   object.insert("perlmod-prefix",           m_ui->perlmod_prefix->text());

   // tab 2 - preprocess
   object.insert("enable-preprocessing",     m_ui->enable_preprocessing_CB->isChecked());
   object.insert("macro-expansion",          m_ui->macro_expansion_CB->isChecked());
   object.insert("expand-only-predefined",   m_ui->expand_only_predefined_CB->isChecked());
   object.insert("search-includes",          m_ui->search_includes_CB->isChecked());

   object.insert("include-path",             putDataList(m_ui->include_path->toPlainText()));
   object.insert("include-file-patterns",    putDataList(m_ui->include_file_patterns->toPlainText()));
   object.insert("predefined-macros",        putDataList(m_ui->predefined_macros->toPlainText()));
   object.insert("expand-as-defined",        putDataList(m_ui->expand_as_defined->toPlainText()));
   object.insert("skip-function-macros",     m_ui->skip_function_macros_CB->isChecked());

   // tab 2 - external
   object.insert("tag-files",                putDataList(m_ui->tag_files->toPlainText()));
   object.insert("generate-tagfile",         m_ui->gen_tagfile->text());
   object.insert("all-externals",            m_ui->all_externals_CB->isChecked());
   object.insert("external-groups",          m_ui->external_groups_CB->isChecked());
   object.insert("external-pages",           m_ui->external_pages_CB->isChecked());
   object.insert("perl-path",                m_ui->perl_path->text());

   // tab 2 - dot
   object.insert("class-diagrams",           m_ui->class_diagrams_CB->isChecked());
   object.insert("mscgen-path",              m_ui->mscgen_path->text());
   object.insert("dia-path",                 m_ui->dia_path->text());
   object.insert("hide-undoc-relations",     m_ui->hide_undoc_relations_CB->isChecked());
   object.insert("have-dot",                 m_ui->have_dot_CB->isChecked());
   object.insert("dot-num-threads",          m_ui->dot_num_threads_SB->value());
   object.insert("dot-font-name",            m_ui->dot_font_name->text());
   object.insert("dot-font-size",            m_ui->dot_font_size_SB->value());
   object.insert("dot-font-path",            m_ui->dot_font_path->text());

   object.insert("group-graphs",             m_ui->group_graphs_CB->isChecked());
   object.insert("uml-look",                 m_ui->uml_look_CB->isChecked());
   object.insert("uml-limit-num-fields",     m_ui->uml_limit_num_fields_SB->value());
   object.insert("template-relations",       m_ui->template_relations_CB->isChecked());      
   object.insert("directory-graph",          m_ui->directory_graph_CB->isChecked());
   object.insert("dot-image-format",         m_ui->dot_image_format_CM->currentText());

   object.insert("interactive-svg",          m_ui->interactive_svg_CB->isChecked());
   object.insert("dot-path",                 m_ui->dot_path->text());
   object.insert("dot-file-dirs",            putDataList(m_ui->dot_file_dirs->toPlainText()));
   object.insert("msc-file-dirs",            putDataList(m_ui->msc_file_dirs->toPlainText()));
   object.insert("dia-file-dirs",            putDataList(m_ui->dia_file_dirs->toPlainText()));
   object.insert("plantuml-jar-path",        m_ui->plantuml_jar_path->text());

   object.insert("dot-graph-max-nodes",      m_ui->dot_graph_max_nodes_SB->value());
   object.insert("dot-graph-max-depth",      m_ui->dot_graph_max_depth_SB->value());
   object.insert("dot-transparent",          m_ui->dot_transparent_CB->isChecked());
   object.insert("dot-multple-targets",      m_ui->dot_multple_targets_CB->isChecked());
   object.insert("generate-legend",          m_ui->gen_legend_CB->isChecked());
   object.insert("dot-cleanup",              m_ui->dot_cleanup_CB->isChecked());

   //  ***
   // tab 3 - html
   object.insert("html-output",              m_ui->html_output->text());
   object.insert("html-file-extension",      m_ui->html_file_extension->text());
   object.insert("html-header",              m_ui->html_header->text());
   object.insert("html-footer",              m_ui->html_footer->text());
   object.insert("html-stylesheet",          m_ui->html_stylesheet->text());
   object.insert("html-extra-stylesheets",   putDataList(m_ui->html_extra_stylesheets->toPlainText()));
   object.insert("html-extra-files",         putDataList(m_ui->html_extra_files->toPlainText()));

   object.insert("html-colorstyle-hue",      m_ui->html_colorstyle_hue->value());
   object.insert("html-colorstyle-sat",      m_ui->html_colorstyle_sat->value());
   object.insert("html-colorstyle-gamma",    m_ui->html_colorstyle_gamma->value());

   object.insert("html-timestamp",           m_ui->html_timestamp_CB->isChecked());
   object.insert("html-synamic-sections",    m_ui->html_synamic_sections_CB->isChecked());;
   object.insert("html-index-num-entries",   m_ui->html_index_num_entries_SB->value());
   object.insert("generate-docset",          m_ui->gen_docset_CB->isChecked());
   object.insert("docset-feedname",          m_ui->docset_feedname->text());
   object.insert("docset-bundle-id",         m_ui->docset_bundle_id->text());
   object.insert("docset-publisher-id",      m_ui->docset_publisher_id->text());
   object.insert("docset-publisher-name",    m_ui->docset_publisher_name->text());
   object.insert("generate-html-help",       m_ui->gen_html_help_CB->isChecked());
   object.insert("chm-file",                 m_ui->chm_file->text());

   object.insert("hhc-location",             m_ui->hhc_location->text());
   object.insert("generate-chi",             m_ui->gen_chi_CB->isChecked());
   object.insert("chm-index-encoding",       m_ui->chm_index_encoding->text());
   object.insert("binary-toc",               m_ui->binary_toc_CB->isChecked());
   object.insert("toc-expanded",             m_ui->toc_expanded_CB->isChecked());

   object.insert("generate-qhp",             m_ui->gen_qhp_CB->isChecked());
   object.insert("qch-file",                 m_ui->qch_file->text());
   object.insert("qhp-namespace",            m_ui->qhp_namespace->text());
   object.insert("qhp-virtual-folder",       m_ui->qhp_virtual_folder->text());
   object.insert("qhp-cust-filter-name",     m_ui->qhp_cust_filter_name->text());
   object.insert("qhp-cust-filter-attrib",   m_ui->qhp_cust_filter_attrib->text());
   object.insert("qhp-section-filter-name",  m_ui->qhp_section_filter_name->text());
   object.insert("qhg-location",             m_ui->qhg_location->text());

   object.insert("generate-eclipse-help",    m_ui->gen_eclipse_help_CB->isChecked());
   object.insert("eclipse-doc-id",           m_ui->eclipse_doc_id->text());
   object.insert("disable-index",            m_ui->disable_index_CB->isChecked());
   object.insert("generate-treeview",        m_ui->gen_treeview_CB->isChecked());
   object.insert("enum-values-per-line",     m_ui->enum_values_per_line_SB->value());
   object.insert("treeview-width",           m_ui->treeview_width_SB->value());

   object.insert("external-links-in-window", m_ui->external_links_in_window_CB->isChecked());
   object.insert("formula-fontsize",         m_ui->formula_fontsize_SB->value());
   object.insert("formula-transparent",      m_ui->formula_transparent_CB->isChecked());

   object.insert("use-mathjax",              m_ui->use_mathjax_CB->isChecked());
   object.insert("mathjax-format",           m_ui->mathjax_format_CM->currentText());
   object.insert("mathjax-repath",           m_ui->mathjax_repath->text());
   object.insert("mathjax-extensionss",      putDataList(m_ui->mathjax_extensions->toPlainText()));

   object.insert("mathjax-codefile",         m_ui->mathjax_codefile->text());

   object.insert("search-engine",            m_ui->search_engine_CB->isChecked());
   object.insert("server-based-search",      m_ui->server_based_search_CB->isChecked());
   object.insert("external-search",          m_ui->external_search_CB->isChecked());
   object.insert("search-engine-url",        m_ui->search_engine_url->text());
   object.insert("search-data-file",         m_ui->search_data_file->text());
   object.insert("external-search-id",       m_ui->external_search_id->text());
   object.insert("extra-search-mappings",    putDataList(m_ui->extra_search_mappings->toPlainText()));

   // tab 3 - latex
   object.insert("latex-output",             m_ui->latex_output->text());
   object.insert("latex-cmd-name",           m_ui->latex_cmd_name->text());
   object.insert("make-index-cmd-name",      m_ui->make_index_cmd_name->text());
   object.insert("compact-latex",            m_ui->compact_latex_CB->isChecked());
   object.insert("paper-type",               m_ui->paper_type_CM->currentText());

   object.insert("latex-extra-packages",     putDataList(m_ui->latex_extra_packages->toPlainText()));
   object.insert("latex-header",             m_ui->latex_header->text());
   object.insert("latex-footer",             m_ui->latex_footer->text());
   object.insert("latex-extra-files",        putDataList(m_ui->latex_extra_files->toPlainText()));

   object.insert("pdf-hyperlinks",           m_ui->pdf_hyperlinks_CB->isChecked());
   object.insert("pdf-latex",                m_ui->pdf_latex_CB->isChecked());
   object.insert("latex-batch-mode",         m_ui->latex_batch_mode_CB->isChecked());
   object.insert("latex-hide-indices",       m_ui->latex_hide_indices_CB->isChecked());
   object.insert("latex-source-code",        m_ui->latex_source_code_CB->isChecked());
   object.insert("latex-bib-style",          m_ui->latex_bib_style->text());

   // tab 3 - rtf
   object.insert("rtf-output",               m_ui->rtf_output->text());
   object.insert("rtf-compact",              m_ui->rtf_compact_CB->isChecked());
   object.insert("rtf-hyperlinks",           m_ui->rtf_hyperlinks_CB->isChecked());
   object.insert("rtf-stylesheet",           m_ui->rtf_stylesheet->text());
   object.insert("rtf-extension",            m_ui->rtf_extension->text());

   // tab 3 - man
   object.insert("man-output",               m_ui->man_output->text());
   object.insert("man-extension",            m_ui->man_extension->text());
   object.insert("man-subdir",               m_ui->man_subdir->text());
   object.insert("man-links",                m_ui->man_links_CB->isChecked());

   // tab 3 - xml
   object.insert("xml-output",               m_ui->xml_output->text());
   object.insert("xml-program-listing",      m_ui->xml_program_listing_CB->isChecked());

   // tab 3 - docbook
   object.insert("docbook-output",           m_ui->docbook_output->text());
   object.insert("docbook-program-listing",  m_ui->docbook_program_listing_CB->isChecked());

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
