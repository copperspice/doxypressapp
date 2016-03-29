/*************************************************************************
 *
 * Copyright (C) 2014-2016 Barbara Geller & Ansel Sermersheim
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

#include "dialog_config.h"
#include "dialog_editcfg.h"
#include "dialog_selectcfg.h"
#include "util.h"
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

      m_settings.doxyPressExe = object.value("doxyPress-location").toString();
      m_settings.pathPrior = object.value("pathPrior").toString();

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
         csError("Save Settings", "Configuration data is empty, aborting update...");
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

            object.insert("doxyPress-location", m_settings.doxyPressExe);

            {
              // opened files
              QJsonArray temp = QJsonArray::fromStringList(m_openedFiles);
              object.insert("opened-files", temp);
            }

            break;

         // missing option for the user to set this by hand
         case DOXYPRESS_EXE:
            object.insert("doxyPress-location", m_settings.doxyPressExe);
            break;

         case PATH_PRIOR:
            object.insert("pathPrior", m_settings.pathPrior);
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
      csError(tr("Read Settings"), msg);
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
      csError(tr("Save Settings"), msg);
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

   value = QJsonValue(QString(""));
   object.insert("doxyPress-location", value);

   // save the data
   QJsonDocument doc(object);
   QByteArray data = doc.toJson();

   bool ok = json_SaveFile(data);

   return ok;
}

// **
void MainWindow::edit_Cfg()
{
   Dialog_EditCfg *dw = new Dialog_EditCfg(this, m_settings.doxyPressExe);
   int result = dw->exec();

   switch (result) {

      case QDialog::Rejected:
         break;

      case QDialog::Accepted:
         m_settings.doxyPressExe = dw->get_doxyPressFn();
         json_Write(DOXYPRESS_EXE);

         break;
   }
}

void MainWindow::move_Cfg()
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

void MainWindow::save_Cfg()
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
   QJsonDocument doc = QJsonDocument::fromJson(data);

   if (! doc.isObject()) {

      if (data.contains("PROJECT_NUMBER"))  {
         csError(tr("Open Project file"), tr("The selected file appears to be a Doxygen project file. To convert to "
                    "a DoxyPress project file select Tools, then 'Convert to DoxyPress format'"));

      } else {
         csError(tr("Open Project file"), tr("The selected file is not a valid JSON file format, open "
                    "project file aborted"));
      }

      return;
   }

   QJsonObject object = doc.object();   
   int index;

   // test json format
   QJsonValue temp = object.value("doxypress-format");

   if (temp.isNull())  {
      m_doxypressFormat = 0;
   } else {
      m_doxypressFormat = temp.toInt();
   }

   if (m_doxypressFormat == 0)  {

      // tab 1
      m_ui->project_name->setText(              object.value("project-name").toString());
      m_ui->project_brief->setText(             object.value("project-brief").toString());
      m_ui->project_version->setText(           object.value("project-version").toString());
      m_ui->project_logo->setText(              object.value("project-logo").toString());

      m_ui->output_dir->setText(                object.value("output-dir").toString());

      m_ui->optimize_cplus_RB->setChecked(      object.value("optimize-cplus").toBool());
      m_ui->optimize_java_RB->setChecked(       object.value("optimize-java").toBool());
      m_ui->optimize_c_RB->setChecked(          object.value("optimize-c").toBool());
      m_ui->optimize_fortran_RB->setChecked(    object.value("optimize-fortran").toBool());

      m_ui->gen_html_CB1->setChecked(           object.value("generate-html").toBool());

      m_ui->gen_latex_CB1->setChecked(          object.value("generate-latex").toBool());
      m_ui->latex_hyper_pdf_RB->setChecked(     object.value("latex-hyper-pdf").toBool());
      m_ui->latex_pdf_RB->setChecked(           object.value("latex-pdf").toBool());
      m_ui->latex_ps_RB->setChecked(            object.value("latex-ps").toBool());

      m_ui->gen_rtf_CB1->setChecked(            object.value("generate-rtf").toBool());
      m_ui->gen_man_CB1->setChecked(            object.value("generate-man").toBool());
      m_ui->gen_xml_CB1->setChecked(            object.value("generate-xml").toBool());
      m_ui->gen_docbook_CB1->setChecked(        object.value("generate-docbook").toBool());

      m_ui->dot_class_graph_CB1->setChecked(    object.value("dot-class-graph").toBool());
      m_ui->dot_collaboration_CB1->setChecked(  object.value("dot-collaboration").toBool());
      m_ui->dot_hierarchy_CB1->setChecked(      object.value("dot-hierarchy").toBool());
      m_ui->dot_include_CB1->setChecked(        object.value("dot-include").toBool());
      m_ui->dot_included_by_CB1->setChecked(    object.value("dot-included-by").toBool());
      m_ui->dot_call_CB1->setChecked(           object.value("dot-call").toBool());
      m_ui->dot_called_by_CB1->setChecked(      object.value("dot-called-by").toBool());

      //  ***
      // tab 2 - general
      index = m_ui->output_language_CM->findText(     object.value("output-language").toString());
      m_ui->output_language_CM->setCurrentIndex(index);

      m_ui->aliases->setPlainText(                    getDataList(object, "aliases"));
      m_ui->abbreviate_brief->setPlainText(           getDataList(object, "abbreviate-brief"));
      m_ui->strip_from_path->setPlainText(            getDataList(object, "strip-from-path"));
      m_ui->strip_from_inc_path->setPlainText(        getDataList(object, "strip-from-inc-path"));

      m_ui->full_path_names_CB->setChecked(           object.value("full-path-names").toBool());
      m_ui->brief_member_desc_CB->setChecked(         object.value("brief-member-desc").toBool());
      m_ui->repeat_brief_CB->setChecked(              object.value("repeat-brief").toBool());
      m_ui->always_detailed_sec_CB->setChecked(       object.value("always-detailed-sec").toBool());
      m_ui->javadoc_auto_brief_CB->setChecked(        object.value("javadoc-auto-brief").toBool());
      m_ui->qt_auto_brief_CB->setChecked(             object.value("qt-auto-brief").toBool());
      m_ui->multiline_cpp_brief_CB->setChecked(       object.value("multiline-cpp-brief").toBool());

      m_ui->tab_size_SB->setValue(                    object.value("tab-size").toInt());
      m_ui->lookup_cache_size_SB->setValue(           object.value("lookup-cache-size").toInt());

      // tab 2 - build confg
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
      m_ui->hide_compound_ref_CB->setChecked(         object.value("hide-compound-ref").toBool());
      m_ui->hide_navtree_members_CB->setChecked(      false);

      m_ui->show_include_files_CB->setChecked(        object.value("show-include-files").toBool());
      m_ui->show_grouped_members_inc_CB->setChecked(  object.value("show-grouped-members-inc").toBool());
      m_ui->show_used_files_CB->setChecked(           object.value("show-used-files").toBool());
      m_ui->show_file_page_CB->setChecked(            object.value("show-file-page").toBool());
      m_ui->show_namespace_page_CB->setChecked(       object.value("show-namespace-page").toBool());

      m_ui->inline_inherited_member_CB->setChecked(   object.value("inline-inherited-member").toBool());
      m_ui->inline_info_CB->setChecked(               object.value("inline-info").toBool());
      m_ui->inline_grouped_classes_CB->setChecked(    object.value("inline-grouped-classes").toBool());
      m_ui->inline_simple_struct_CB->setChecked(      object.value("inline-simple-struct").toBool());
      m_ui->use_typedef_name_CB->setChecked(          object.value("use-typedef-name").toBool());

      m_ui->sort_member_docs_CB->setChecked(          object.value("sort-member-docs").toBool());
      m_ui->sort_brief_docs_CB->setChecked(           object.value("sort-brief-docs").toBool());
      m_ui->sort_constructors_first_CB->setChecked(   object.value("sort-constructors-first").toBool());
      m_ui->sort_group_names_CB->setChecked(          object.value("sort-group-names").toBool());
      m_ui->sort_by_scope_name_CB->setChecked(        object.value("sort-by-scope-name").toBool());
      m_ui->sort_class_case_sensitive_CB->setChecked( object.value("sort-class-case-sensitive").toBool());

      m_ui->gen_todo_list_CB->setChecked(             object.value("generate-todo-list").toBool());
      m_ui->gen_test_list_CB->setChecked(             object.value("generate-test-list").toBool());
      m_ui->gen_bug_list_CB->setChecked(              object.value("generate-bug-list").toBool());
      m_ui->gen_deprecate_list_CB->setChecked(        object.value("generate-deprecate-list").toBool());

      m_ui->short_names_CB->setChecked(               object.value("short-names").toBool());
      m_ui->allow_unicode_names_CB->setChecked(       object.value("allow-unicode-names").toBool());
      m_ui->create_subdirs_CB->setChecked(            object.value("create-subdirs").toBool());
      m_ui->case_sensitive_fname_CB->setChecked(      object.value("case-sensitive-fname").toBool());

      m_ui->markdown_CB->setChecked(                  object.value("markdown").toBool());
      m_ui->auto_link_CB->setChecked(                 object.value("auto-link").toBool());
      m_ui->strict_sig_matching_CB->setChecked(       object.value("strict-sig-matching").toBool());

      m_ui->internal_docs_CB->setChecked(             object.value("internal-docs").toBool());
      m_ui->force_local_includes_CB->setChecked(      object.value("force-local-includes").toBool());
      m_ui->inherit_docs_CB->setChecked(              object.value("inherit-docs").toBool());

      m_ui->separate_member_pages_CB->setChecked(     object.value("separate-member-pages").toBool());
      m_ui->allow_sub_grouping_CB->setChecked(        object.value("allow-sub-grouping").toBool());
      m_ui->duplicate_docs_CB->setChecked(            object.value("duplicate-docs").toBool());
      m_ui->group_nested_compounds_CB->setChecked(    object.value("group-nested-compounds").toBool());

      // tab 2 -output
      m_ui->max_init_lines_SB->setValue(              object.value("max-init-lines").toInt());
      m_ui->enabled_sections->setPlainText(           getDataList(object, "enabled-sections"));     
      m_ui->file_version_filter->setText(             object.value("file-version-filter").toString());      
      m_ui->main_page_name->setText(                  "");
      m_ui->main_page_omit->setChecked(               false);
      m_ui->layout_file->setText(                     object.value("layout-file").toString());             
      m_ui->ns_alias->setPlainText(                   "");
      m_ui->bb_style_CB->setChecked(                  false);

      // tab 2 -programming
      m_ui->tcl_subst->setPlainText(                  getDataList(object, "tcl-subst"));
      m_ui->language_mapping->setPlainText(           getDataList(object, "language-mapping"));
      m_ui->built_in_stl_support_CB->setChecked(      object.value("built-in-stl-support").toBool());
      m_ui->cpp_cli_support_CB->setChecked(           object.value("cpp-cli-support").toBool());
      m_ui->sip_support_CB->setChecked(               object.value("sip-support").toBool());
      m_ui->idl_support_CB->setChecked(               object.value("idl-support").toBool());

      // tab 2 - messages
      m_ui->quiet_CB->setChecked(                     object.value("quiet").toBool());
      m_ui->warnings_CB->setChecked(                  object.value("warnings").toBool());
      m_ui->warn_undoc_CB->setChecked(                object.value("warn-undoc").toBool());
      m_ui->warn_doc_error_CB->setChecked(            object.value("warn-doc-error").toBool());
      m_ui->warn_undoc_param_CB->setChecked(          object.value("warn-undoc-param").toBool());
      m_ui->warn_format->setText(                     object.value("warn-format").toString());
      m_ui->warn_logfile->setText(                    object.value("warn-logfile").toString());

      // tab 2 -input src
      m_ui->input_source->setPlainText(               getDataList(object, "input-source"));
      m_ui->input_patterns->setPlainText(             getDataList(object, "input-patterns"));
      m_ui->input_encoding->setText(                  object.value("input-encoding").toString());
      m_ui->input_recursive_CB->setChecked(           object.value("input-recursive").toBool());

      m_ui->exclude_files->setPlainText(              getDataList(object, "exclude-files"));
      m_ui->exclude_symlinks_CB->setChecked(          object.value("exclude-symlinks").toBool());
      m_ui->exclude_patterns->setPlainText(           getDataList(object, "exclude-patterns"));
      m_ui->exclude_symbols->setPlainText(            getDataList(object, "exclude-symbols"));

      // tab 2 - input other
      m_ui->example_source->setPlainText(             getDataList(object, "example-source"));
      m_ui->example_patterns->setPlainText(           getDataList(object, "example-patterns"));
      m_ui->example_recursive_CB->setChecked(         object.value("example-recursive").toBool());

      m_ui->image_path->setPlainText(                 getDataList(object, "image-path"));
      m_ui->mdfile_mainpage->setText(                 object.value("mdfile-mainpage").toString());

      // tab 2 -index filters
      m_ui->filter_program->setText(                  object.value("filter-program").toString());
      m_ui->filter_patterns->setPlainText(            getDataList(object, "filter-patterns"));
      m_ui->filter_source_files_CB->setChecked(       object.value("filter-source-files").toBool());
      m_ui->filter_source_patterns->setPlainText(     getDataList(object, "filter-source-patterns"));

      // tab 2 - index page
      m_ui->alpha_index_CB->setChecked(               object.value("alpha-index").toBool());
      m_ui->cols_in_index_SB->setValue(               object.value("cols-in-index").toInt());
      m_ui->ignore_prefix->setPlainText(              getDataList(object, "ignore-prefix"));  

      // tab 2 -source code
      m_ui->source_code_CB->setChecked(               object.value("source-code").toBool());
      m_ui->inline_source_CB->setChecked(             object.value("inline-source").toBool());
      m_ui->strip_code_comments_CB->setChecked(       object.value("strip-code-comments").toBool());
      m_ui->verbatim_headers_CB->setChecked(          object.value("verbatim-headers").toBool());
      m_ui->ref_by_relation_CB->setChecked(           object.value("ref-by-relation").toBool());
      m_ui->ref_relation_CB->setChecked(              object.value("ref-relation").toBool());
      m_ui->ref_link_source_CB->setChecked(           object.value("ref-link-source").toBool());
      m_ui->source_tooltips_CB->setChecked(           object.value("source-tooltips").toBool());
      m_ui->use_htags_CB->setChecked(                 object.value("use-htags").toBool());
      m_ui->suffix_source_navtree->setPlainText(      getDataList(object, "suffix-source-navtree"));
      m_ui->suffix_header_navtree->setPlainText(      getDataList(object, "suffix-header-navtree"));
      m_ui->suffix_exclude_navtree->setPlainText(     getDataList(object, "suffix-exclude-navtree"));
      m_ui->clang_parsing_CB->setChecked(             object.value("clang-parsing").toBool());
      m_ui->clang_flags->setPlainText(                getDataList(object, "clang-flags"));

      // tab 2 - preprocessor
      m_ui->enable_preprocessing_CB->setChecked(      object.value("enable-preprocessing").toBool());
      m_ui->search_includes_CB->setChecked(           object.value("search-includes").toBool());

      m_ui->include_path->setPlainText(               getDataList(object,"include-path"));
      m_ui->include_patterns->setPlainText(           getDataList(object,"include-patterns"));

      m_ui->macro_expansion_CB->setChecked(           object.value("macro-expansion").toBool());
      m_ui->expand_only_predefined_CB->setChecked(    object.value("expand-only-predefined").toBool());
      m_ui->skip_function_macros_CB->setChecked(      object.value("skip-function-macros").toBool());

      m_ui->predefined_macros->setPlainText(          getDataList(object,"predefined-macros"));
      m_ui->expand_as_defined->setPlainText(          getDataList(object,"expand-as-defined"));     

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

      m_ui->interactive_svg_CB->setChecked(           object.value("interactive-svg").toBool());
      m_ui->dot_path->setText(                        object.value("dot-path").toString());
      m_ui->dot_file_dirs->setPlainText(              getDataList(object, "dot-file-dirs"));
      m_ui->msc_file_dirs->setPlainText(              getDataList(object, "msc-file-dirs"));
      m_ui->dia_file_dirs->setPlainText(              getDataList(object, "dia-file-dirs"));
      m_ui->plantuml_jar_path->setText(               object.value("plantuml-jar-path").toString());
      m_ui->plantuml_inc_path->setPlainText(          getDataList(object, "plantuml-inc-path"));

      m_ui->dot_graph_max_nodes_SB->setValue(         object.value("dot-graph-max-nodes").toInt());
      m_ui->dot_graph_max_depth_SB->setValue(         object.value("dot-graph-max-depth").toInt());
      m_ui->dot_transparent_CB->setChecked(           object.value("dot-transparent").toBool());
      m_ui->dot_multiple_targets_CB->setChecked(      object.value("dot-multiple-targets").toBool());
      m_ui->gen_legend_CB->setChecked(                object.value("generate-legend").toBool());
      m_ui->dot_cleanup_CB->setChecked(               object.value("dot-cleanup").toBool());

      //  ***
      // tab 3 - html
      m_ui->html_output->setText(                     object.value("html-output").toString());
      m_ui->html_file_extension->setText(             object.value("html-file-extension").toString());
      m_ui->html_header->setText(                     object.value("html-header").toString());
      m_ui->html_footer->setText(                     object.value("html-footer").toString());
      m_ui->html_stylesheets->setPlainText(           getDataList(object, "html-stylesheets"));
      m_ui->html_extra_files->setPlainText(           getDataList(object, "html-extra-files"));

      m_ui->html_colorstyle_hue->setValue(            object.value("html-colorstyle-hue").toInt());
      m_ui->html_colorstyle_sat->setValue(            object.value("html-colorstyle-sat").toInt());
      m_ui->html_colorstyle_gamma->setValue(          object.value("html-colorstyle-gamma").toInt());

      m_ui->html_timestamp_CB->setChecked(            object.value("html-timestamp").toBool());
      m_ui->html_dynamic_sections_CB->setChecked(     object.value("html-dynamic-sections").toBool());
      m_ui->html_index_num_entries_SB->setValue(      object.value("html-index-num-entries").toInt());

      m_ui->disable_index_CB->setChecked(             object.value("disable-index").toBool());
      m_ui->gen_treeview_CB->setChecked(              object.value("generate-treeview").toBool());
      m_ui->enum_values_per_line_SB->setValue(        object.value("enum-values-per-line").toInt());
      m_ui->treeview_width_SB->setValue(              object.value("treeview-width").toInt());
      m_ui->external_links_in_window_CB->setChecked(  object.value("external-links-in-window").toBool());

      m_ui->html_search_CB2->setChecked(              object.value("html-search").toBool());
      m_ui->search_server_based_CB->setChecked(       object.value("search-server-based").toBool());
      m_ui->search_external_CB->setChecked(           object.value("search-external").toBool());
      m_ui->search_external_url->setText(             object.value("search-external-url").toString());
      m_ui->search_data_file->setText(                object.value("search-data-file").toString());
      m_ui->search_external_id->setText(              object.value("search-external-id").toString());
      m_ui->search_mappings->setPlainText(            getDataList(object, "search-mappings"));

      m_ui->formula_fontsize_SB->setValue(            object.value("formula-fontsize").toInt());
      m_ui->formula_transparent_CB->setChecked(       object.value("formula-transparent").toBool());
      m_ui->ghostscript->setText(                     object.value("ghostscript").toString());
      m_ui->use_mathjax_CB->setChecked(               object.value("use-mathjax").toBool());

      index = m_ui->mathjax_format_CM->findText(      object.value("mathjax-format").toString());
      m_ui->mathjax_format_CM->setCurrentIndex(index);

      m_ui->mathjax_relpath->setText(                 object.value("mathjax-relpath").toString());
      m_ui->mathjax_extensions->setPlainText(         getDataList(object, "mathjax-extensions"));
      m_ui->mathjax_codefile->setText(                object.value("mathjax-codefile").toString());

      // tab 3 - chm
      m_ui->gen_chm_CB->setChecked(                   object.value("generate-chm").toBool());
      m_ui->chm_file->setText(                        object.value("chm-file").toString());
      m_ui->hhc_location->setText(                    object.value("hhc-location").toString());
      m_ui->gen_chi_CB->setChecked(                   object.value("generate-chi").toBool());
      m_ui->chm_index_encoding->setText(              object.value("chm-index-encoding").toString());
      m_ui->binary_toc_CB->setChecked(                object.value("binary-toc").toBool());
      m_ui->toc_expanded_CB->setChecked(              object.value("toc-expanded").toBool());

      // tab 3 - docset
      m_ui->gen_docset_CB->setChecked(                object.value("generate-docset").toBool());
      m_ui->docset_feedname->setText(                 object.value("docset-feedname").toString());
      m_ui->docset_bundle_id->setText(                object.value("docset-bundle-id").toString());
      m_ui->docset_publisher_id->setText(             object.value("docset-publisher-id").toString());
      m_ui->docset_publisher_name->setText(           object.value("docset-publisher-name").toString());

      // tab 3 - eclipse
      m_ui->gen_eclipse_CB->setChecked(               object.value("generate-eclipse").toBool());
      m_ui->eclipse_doc_id->setText(                  object.value("eclipse-doc-id").toString());

      // tab 3 - latex
      m_ui->latex_output->setText(                    object.value("latex-output").toString());
      m_ui->latex_cmd_name->setText(                  object.value("latex-cmd-name").toString());
      m_ui->make_index_cmd_name->setText(             object.value("make-index-cmd-name").toString());
      m_ui->latex_compact_CB->setChecked(             object.value("latex-compact").toBool());

      index = m_ui->latex_paper_type_CM->findText(    object.value("latex-paper-type").toString());
      m_ui->latex_paper_type_CM->setCurrentIndex(index);

      m_ui->latex_extra_packages->setPlainText(       getDataList(object, "latex-extra-packages"));
      m_ui->latex_header->setText(                    object.value("latex-header").toString());
      m_ui->latex_footer->setText(                    object.value("latex-footer").toString());
      m_ui->latex_stylesheets->setPlainText(          getDataList(object, "latex-stylesheets"));
      m_ui->latex_extra_files->setPlainText(          getDataList(object, "latex-extra-files"));

      m_ui->latex_timestamp_CB->setChecked(           object.value("latex-timestamp").toBool());
      m_ui->latex_hyper_pdf_CB->setChecked(           object.value("latex-hyper-pdf").toBool());
      m_ui->latex_pdf_CB->setChecked(                 object.value("latex-pdf").toBool());
      m_ui->latex_batch_mode_CB->setChecked(          object.value("latex-batch-mode").toBool());
      m_ui->latex_hide_indices_CB->setChecked(        object.value("latex-hide-indices").toBool());
      m_ui->latex_source_code_CB->setChecked(         object.value("latex-source-code").toBool());

      // tab 3 - latex bib
      m_ui->latex_bib_style->setText(                 object.value("latex-bib-style").toString());
      m_ui->cite_bib_files->setPlainText(             getDataList(object, "cite-bib-files"));

      // tab 3 - man
      m_ui->man_output->setText(                      object.value("man-output").toString());
      m_ui->man_extension->setText(                   object.value("man-extension").toString());
      m_ui->man_subdir->setText(                      object.value("man-subdir").toString());
      m_ui->man_links_CB->setChecked(                 object.value("man-links").toBool());

      // tab 3 - perl modoule
      m_ui->gen_perl_CB->setChecked(                  object.value("generate-perl").toBool());
      m_ui->perl_latex_CB->setChecked(                object.value("perl-latex").toBool());
      m_ui->perl_pretty_CB->setChecked(               object.value("perl-pretty").toBool());
      m_ui->perl_prefix->setText(                     object.value("perl-prefix").toString());

      // tab 3 - qt help
      m_ui->gen_qthelp_CB->setChecked(                object.value("generate-qthelp").toBool());
      m_ui->qch_file->setText(                        object.value("qch-file").toString());
      m_ui->qhp_namespace->setText(                   object.value("qhp-namespace").toString());
      m_ui->qhp_virtual_folder->setText(              object.value("qhp-virtual-folder").toString());
      m_ui->qhp_cust_filter_name->setText(            object.value("qhp-cust-filter-name").toString());
      m_ui->qhp_cust_attrib->setPlainText(            getDataList(object, "qhp-cust-attrib"));
      m_ui->qhp_sect_attrib->setPlainText(            getDataList(object, "qhp-sect-attrib"));
      m_ui->qthelp_gen_path->setText(                 object.value("qthelp-gen-path").toString());

      // tab 3 - rtf
      m_ui->rtf_output->setText(                      object.value("rtf-output").toString());
      m_ui->rtf_compact_CB->setChecked(               object.value("rtf-compact").toBool());
      m_ui->rtf_hyperlinks_CB->setChecked(            object.value("rtf-hyperlinks").toBool());
      m_ui->rtf_stylesheet->setText(                  object.value("rtf-stylesheet").toString());
      m_ui->rtf_extension->setText(                   object.value("rtf-extension").toString());
      m_ui->rtf_source_code_CB->setChecked(           object.value("rtf-source-code").toBool());

      // tab 3 - xml
      m_ui->xml_output->setText(                      object.value("xml-output").toString());
      m_ui->xml_program_listing_CB->setChecked(       object.value("xml-program-listing").toBool());

      // tab 3 - docbook
      m_ui->docbook_output->setText(                  object.value("docbook-output").toString());
      m_ui->docbook_program_listing_CB->setChecked(   object.value("docbook-program-listing").toBool());

   } else {
      // m_doxypressFormat == DOXYPRESS_FORMAT  (current value is one)

      // read objects
      QJsonObject projectObj     = object.value("project").toObject();
      QJsonObject genObj         = object.value("general").toObject();
      QJsonObject configObj      = object.value("configuration").toObject();
      QJsonObject msgObj         = object.value("messages").toObject();
      QJsonObject inputObj       = object.value("input").toObject();
      QJsonObject indexObj       = object.value("index").toObject();
      QJsonObject sourceObj      = object.value("source").toObject();
      QJsonObject ppObj          = object.value("preprocessor").toObject();
      QJsonObject extObj         = object.value("external").toObject();
      QJsonObject dotObj         = object.value("dot").toObject();
      QJsonObject bbObj          = object.value("bb").toObject();

      QJsonObject htmlObj        = object.value("output-html").toObject();
      QJsonObject chmObj         = object.value("output-chm").toObject();
      QJsonObject docbookObj     = object.value("output-docbook").toObject();
      QJsonObject docsetObj      = object.value("output-docset").toObject();
      QJsonObject eclipseObj     = object.value("output-eclipse").toObject();
      QJsonObject latexObj       = object.value("output-latex").toObject();
      QJsonObject manObj         = object.value("output-man").toObject();
      QJsonObject perlObj        = object.value("output-perl").toObject();
      QJsonObject qhelpObj       = object.value("output-qhelp").toObject();
      QJsonObject rtfObj         = object.value("output-rtf").toObject();
      QJsonObject xmlObj         = object.value("output-xml").toObject();

      // tab 1
      m_ui->project_name->setText(              projectObj.value("project-name").toString());
      m_ui->project_brief->setText(             projectObj.value("project-brief").toString());
      m_ui->project_version->setText(           projectObj.value("project-version").toString());
      m_ui->project_logo->setText(              projectObj.value("project-logo").toString());

      m_ui->output_dir->setText(                genObj.value("output-dir").toString());

      m_ui->optimize_cplus_RB->setChecked(      genObj.value("optimize-cplus").toBool());
      m_ui->optimize_java_RB->setChecked(       genObj.value("optimize-java").toBool());
      m_ui->optimize_c_RB->setChecked(          genObj.value("optimize-c").toBool());
      m_ui->optimize_fortran_RB->setChecked(    genObj.value("optimize-fortran").toBool());

      m_ui->gen_html_CB1->setChecked(           htmlObj.value("generate-html").toBool());

      m_ui->gen_latex_CB1->setChecked(          latexObj.value("generate-latex").toBool());
      m_ui->latex_hyper_pdf_RB->setChecked(     latexObj.value("latex-hyper-pdf").toBool());
      m_ui->latex_pdf_RB->setChecked(           latexObj.value("latex-pdf").toBool());
      m_ui->latex_ps_RB->setChecked(            latexObj.value("latex-ps").toBool());

      m_ui->gen_rtf_CB1->setChecked(            rtfObj.value("generate-rtf").toBool());
      m_ui->gen_man_CB1->setChecked(            manObj.value("generate-man").toBool());
      m_ui->gen_xml_CB1->setChecked(            xmlObj.value("generate-xml").toBool());
      m_ui->gen_docbook_CB1->setChecked(        docbookObj.value("generate-docbook").toBool());

      m_ui->dot_class_graph_CB1->setChecked(    dotObj.value("dot-class-graph").toBool());
      m_ui->dot_collaboration_CB1->setChecked(  dotObj.value("dot-collaboration").toBool());
      m_ui->dot_hierarchy_CB1->setChecked(      dotObj.value("dot-hierarchy").toBool());
      m_ui->dot_include_CB1->setChecked(        dotObj.value("dot-include").toBool());
      m_ui->dot_included_by_CB1->setChecked(    dotObj.value("dot-included-by").toBool());
      m_ui->dot_call_CB1->setChecked(           dotObj.value("dot-call").toBool());
      m_ui->dot_called_by_CB1->setChecked(      dotObj.value("dot-called-by").toBool());

      //  ***
      // tab 2 - general
      index = m_ui->output_language_CM->findText(     genObj.value("output-language").toString());
      m_ui->output_language_CM->setCurrentIndex(index);

      m_ui->aliases->setPlainText(                    getDataList(genObj, "aliases"));
      m_ui->abbreviate_brief->setPlainText(           getDataList(genObj, "abbreviate-brief"));
      m_ui->strip_from_path->setPlainText(            getDataList(genObj, "strip-from-path"));
      m_ui->strip_from_inc_path->setPlainText(        getDataList(genObj, "strip-from-inc-path"));

      m_ui->full_path_names_CB->setChecked(           configObj.value("full-path-names").toBool());
      m_ui->brief_member_desc_CB->setChecked(         configObj.value("brief-member-desc").toBool());
      m_ui->repeat_brief_CB->setChecked(              configObj.value("repeat-brief").toBool());
      m_ui->always_detailed_sec_CB->setChecked(       configObj.value("always-detailed-sec").toBool());
      m_ui->javadoc_auto_brief_CB->setChecked(        configObj.value("javadoc-auto-brief").toBool());
      m_ui->qt_auto_brief_CB->setChecked(             configObj.value("qt-auto-brief").toBool());
      m_ui->multiline_cpp_brief_CB->setChecked(       configObj.value("multiline-cpp-brief").toBool());

      m_ui->tab_size_SB->setValue(                    genObj.value("tab-size").toInt());
      m_ui->lookup_cache_size_SB->setValue(           genObj.value("lookup-cache-size").toInt());

      // tab 2 - build confg
      m_ui->extract_all_CB->setChecked(               configObj.value("extract-all").toBool());
      m_ui->extract_private_CB->setChecked(           configObj.value("extract-private").toBool());
      m_ui->extract_package_CB->setChecked(           configObj.value("extract-package").toBool());
      m_ui->extract_static_CB->setChecked(            configObj.value("extract-static").toBool());
      m_ui->extract_local_classes_CB->setChecked(     configObj.value("extract-local-classes").toBool());
      m_ui->extract_local_methods_CB->setChecked(     configObj.value("extract-local-methods").toBool());
      m_ui->extract_anon_namespaces_CB->setChecked(   configObj.value("extract-anon-namespaces").toBool());
      m_ui->hide_undoc_members_CB->setChecked(        configObj.value("hide-undoc-members").toBool());
      m_ui->hide_undoc_classes_CB->setChecked(        configObj.value("hide-undoc-classes").toBool());
      m_ui->hide_friend_compounds_CB->setChecked(     configObj.value("hide-friend-compounds").toBool());
      m_ui->hide_in_body_docs_CB->setChecked(         configObj.value("hide-in-body-docs").toBool());
      m_ui->hide_scope_names_CB->setChecked(          configObj.value("hide-scope-names").toBool());
      m_ui->hide_compound_ref_CB->setChecked(         configObj.value("hide-compound-ref").toBool());
      m_ui->hide_navtree_members_CB->setChecked(      configObj.value("hide-navtree-members").toBool());

      m_ui->show_include_files_CB->setChecked(        configObj.value("show-include-files").toBool());
      m_ui->show_grouped_members_inc_CB->setChecked(  configObj.value("show-grouped-members-inc").toBool());
      m_ui->show_used_files_CB->setChecked(           configObj.value("show-used-files").toBool());
      m_ui->show_file_page_CB->setChecked(            configObj.value("show-file-page").toBool());
      m_ui->show_namespace_page_CB->setChecked(       configObj.value("show-namespace-page").toBool());

      m_ui->inline_inherited_member_CB->setChecked(   configObj.value("inline-inherited-member").toBool());
      m_ui->inline_info_CB->setChecked(               configObj.value("inline-info").toBool());
      m_ui->inline_grouped_classes_CB->setChecked(    configObj.value("inline-grouped-classes").toBool());
      m_ui->inline_simple_struct_CB->setChecked(      configObj.value("inline-simple-struct").toBool());
      m_ui->use_typedef_name_CB->setChecked(          configObj.value("use-typedef-name").toBool());

      m_ui->sort_member_docs_CB->setChecked(          configObj.value("sort-member-docs").toBool());
      m_ui->sort_brief_docs_CB->setChecked(           configObj.value("sort-brief-docs").toBool());
      m_ui->sort_constructors_first_CB->setChecked(   configObj.value("sort-constructors-first").toBool());
      m_ui->sort_group_names_CB->setChecked(          configObj.value("sort-group-names").toBool());
      m_ui->sort_by_scope_name_CB->setChecked(        configObj.value("sort-by-scope-name").toBool());
      m_ui->sort_class_case_sensitive_CB->setChecked( configObj.value("sort-class-case-sensitive").toBool());

      m_ui->gen_todo_list_CB->setChecked(             configObj.value("generate-todo-list").toBool());
      m_ui->gen_test_list_CB->setChecked(             configObj.value("generate-test-list").toBool());
      m_ui->gen_bug_list_CB->setChecked(              configObj.value("generate-bug-list").toBool());
      m_ui->gen_deprecate_list_CB->setChecked(        configObj.value("generate-deprecate-list").toBool());

      m_ui->short_names_CB->setChecked(               configObj.value("short-names").toBool());
      m_ui->allow_unicode_names_CB->setChecked(       configObj.value("allow-unicode-names").toBool());
      m_ui->create_subdirs_CB->setChecked(            configObj.value("create-subdirs").toBool());
      m_ui->case_sensitive_fname_CB->setChecked(      configObj.value("case-sensitive-fname").toBool());

      m_ui->markdown_CB->setChecked(                  configObj.value("markdown").toBool());
      m_ui->auto_link_CB->setChecked(                 configObj.value("auto-link").toBool());
      m_ui->strict_sig_matching_CB->setChecked(       configObj.value("strict-sig-matching").toBool());

      m_ui->internal_docs_CB->setChecked(             configObj.value("internal-docs").toBool());    
      m_ui->force_local_includes_CB->setChecked(      configObj.value("force-local-includes").toBool());
      m_ui->inherit_docs_CB->setChecked(              configObj.value("inherit-docs").toBool());

      m_ui->separate_member_pages_CB->setChecked(     configObj.value("separate-member-pages").toBool());
      m_ui->allow_sub_grouping_CB->setChecked(        configObj.value("allow-sub-grouping").toBool());
      m_ui->duplicate_docs_CB->setChecked(            configObj.value("duplicate-docs").toBool());
      m_ui->group_nested_compounds_CB->setChecked(    configObj.value("group-nested-compounds").toBool());

      // tab 2 -build options
      m_ui->max_init_lines_SB->setValue(              configObj.value("max-init-lines").toInt());
      m_ui->enabled_sections->setPlainText(           getDataList(configObj, "enabled-sections"));
      m_ui->file_version_filter->setText(             configObj.value("file-version-filter").toString());
      m_ui->main_page_name->setText(                  configObj.value("main-page-name").toString());
      m_ui->main_page_omit->setChecked(               configObj.value("main-page-omit").toBool());
      m_ui->layout_file->setText(                     configObj.value("layout-file").toString());      
      m_ui->ns_alias->setPlainText(                   getDataList(configObj, "ns-alias"));
      m_ui->bb_style_CB->setChecked(                  bbObj.value("bb-style").toBool());

      // tab 2 -programming
      m_ui->tcl_subst->setPlainText(                  getDataList(configObj, "tcl-subst"));
      m_ui->language_mapping->setPlainText(           getDataList(configObj, "language-mapping"));
      m_ui->built_in_stl_support_CB->setChecked(      configObj.value("built-in-stl-support").toBool());
      m_ui->cpp_cli_support_CB->setChecked(           configObj.value("cpp-cli-support").toBool());
      m_ui->sip_support_CB->setChecked(               configObj.value("sip-support").toBool());
      m_ui->idl_support_CB->setChecked(               configObj.value("idl-support").toBool());

      // tab 2 - messages
      m_ui->quiet_CB->setChecked(                     msgObj.value("quiet").toBool());
      m_ui->warnings_CB->setChecked(                  msgObj.value("warnings").toBool());
      m_ui->warn_undoc_CB->setChecked(                msgObj.value("warn-undoc").toBool());
      m_ui->warn_doc_error_CB->setChecked(            msgObj.value("warn-doc-error").toBool());
      m_ui->warn_undoc_param_CB->setChecked(          msgObj.value("warn-undoc-param").toBool());
      m_ui->warn_format->setText(                     msgObj.value("warn-format").toString());
      m_ui->warn_logfile->setText(                    msgObj.value("warn-logfile").toString());

      // tab 2 -input src
      m_ui->input_source->setPlainText(               getDataList(inputObj, "input-source"));
      m_ui->input_patterns->setPlainText(             getDataList(inputObj, "input-patterns"));
      m_ui->input_encoding->setText(                  inputObj.value("input-encoding").toString());     
      m_ui->input_recursive_CB->setChecked(           inputObj.value("input-recursive").toBool());

      m_ui->exclude_files->setPlainText(              getDataList(inputObj, "exclude-files"));
      m_ui->exclude_symlinks_CB->setChecked(          inputObj.value("exclude-symlinks").toBool());
      m_ui->exclude_patterns->setPlainText(           getDataList(inputObj, "exclude-patterns"));
      m_ui->exclude_symbols->setPlainText(            getDataList(inputObj, "exclude-symbols"));

      // tab 2 - input other
      m_ui->example_source->setPlainText(             getDataList(inputObj, "example-source"));
      m_ui->example_patterns->setPlainText(           getDataList(inputObj, "example-patterns"));
      m_ui->example_recursive_CB->setChecked(         inputObj.value("example-recursive").toBool());

      m_ui->image_path->setPlainText(                 getDataList(inputObj, "image-path"));
      m_ui->mdfile_mainpage->setText(                 inputObj.value("mdfile-mainpage").toString());

      // tab 2 -index filters
      m_ui->filter_program->setText(                  inputObj.value("filter-program").toString());
      m_ui->filter_patterns->setPlainText(            getDataList(inputObj, "filter-patterns"));
      m_ui->filter_source_files_CB->setChecked(       inputObj.value("filter-source-files").toBool());
      m_ui->filter_source_patterns->setPlainText(     getDataList(inputObj, "filter-source-patterns"));

      // tab 2 -index page
      m_ui->alpha_index_CB->setChecked(               indexObj.value("alpha-index").toBool());
      m_ui->cols_in_index_SB->setValue(               indexObj.value("cols-in-index").toInt());
      m_ui->ignore_prefix->setPlainText(              getDataList(indexObj, "ignore-prefix"));    

      // tab 2 -source code
      m_ui->source_code_CB->setChecked(               sourceObj.value("source-code").toBool());
      m_ui->inline_source_CB->setChecked(             sourceObj.value("inline-source").toBool());
      m_ui->strip_code_comments_CB->setChecked(       sourceObj.value("strip-code-comments").toBool());
      m_ui->verbatim_headers_CB->setChecked(          sourceObj.value("verbatim-headers").toBool());
      m_ui->ref_by_relation_CB->setChecked(           sourceObj.value("ref-by-relation").toBool());
      m_ui->ref_relation_CB->setChecked(              sourceObj.value("ref-relation").toBool());
      m_ui->ref_link_source_CB->setChecked(           sourceObj.value("ref-link-source").toBool());
      m_ui->source_tooltips_CB->setChecked(           sourceObj.value("source-tooltips").toBool());
      m_ui->use_htags_CB->setChecked(                 sourceObj.value("use-htags").toBool());
      m_ui->suffix_source_navtree->setPlainText(      getDataList(sourceObj, "suffix-source-navtree"));
      m_ui->suffix_header_navtree->setPlainText(      getDataList(sourceObj, "suffix-header-navtree"));
      m_ui->suffix_exclude_navtree->setPlainText(     getDataList(sourceObj, "suffix-exclude-navtree"));
      m_ui->clang_parsing_CB->setChecked(             sourceObj.value("clang-parsing").toBool());
      m_ui->clang_flags->setPlainText(                getDataList(sourceObj, "clang-flags"));

      // tab 2 - preprocessor
      m_ui->enable_preprocessing_CB->setChecked(      ppObj.value("enable-preprocessing").toBool());
      m_ui->search_includes_CB->setChecked(           ppObj.value("search-includes").toBool());

      m_ui->include_path->setPlainText(               getDataList(ppObj,"include-path"));
      m_ui->include_patterns->setPlainText(           getDataList(ppObj,"include-patterns"));

      m_ui->macro_expansion_CB->setChecked(           ppObj.value("macro-expansion").toBool());      
      m_ui->expand_only_predefined_CB->setChecked(    ppObj.value("expand-only-predefined").toBool());
      m_ui->skip_function_macros_CB->setChecked(      ppObj.value("skip-function-macros").toBool());

      m_ui->predefined_macros->setPlainText(          getDataList(ppObj,"predefined-macros"));
      m_ui->expand_as_defined->setPlainText(          getDataList(ppObj,"expand-as-defined"));      

      // tab 2 - external
      m_ui->tag_files->setPlainText(                  getDataList(extObj,"tag-files"));
      m_ui->gen_tagfile->setText(                     extObj.value("generate-tagfile").toString());
      m_ui->all_externals_CB->setChecked(             extObj.value("all-externals").toBool());
      m_ui->external_groups_CB->setChecked(           extObj.value("external-groups").toBool());
      m_ui->external_pages_CB->setChecked(            extObj.value("external-pages").toBool());
      m_ui->perl_path->setText(                       extObj.value("perl-path").toString());

      // tab 2 - dot
      m_ui->class_diagrams_CB->setChecked(            dotObj.value("class-diagrams").toBool());
      m_ui->mscgen_path->setText(                     dotObj.value("mscgen-path").toString());
      m_ui->dia_path->setText(                        dotObj.value("dia-path").toString());
      m_ui->hide_undoc_relations_CB->setChecked(      dotObj.value("hide-undoc-relations").toBool());
      m_ui->have_dot_CB->setChecked(                  dotObj.value("have-dot").toBool());
      m_ui->dot_num_threads_SB->setValue(             dotObj.value("dot-num-threads").toInt());
      m_ui->dot_font_name->setText(                   dotObj.value("dot-font-name").toString());
      m_ui->dot_font_size_SB->setValue(               dotObj.value("dot-font-size").toInt());
      m_ui->dot_font_path->setText(                   dotObj.value("dot-font-path").toString());

      m_ui->group_graphs_CB->setChecked(              dotObj.value("group-graphs").toBool());
      m_ui->uml_look_CB->setChecked(                  dotObj.value("uml-look").toBool());
      m_ui->uml_limit_num_fields_SB->setValue(        dotObj.value("uml-limit-num-fields").toInt());
      m_ui->template_relations_CB->setChecked(        dotObj.value("template-relations").toBool());
      m_ui->directory_graph_CB->setChecked(           dotObj.value("directory-graph").toBool());

      index = m_ui->dot_image_format_CM->findText(    dotObj.value("dot-image-format").toString());
      m_ui->dot_image_format_CM->setCurrentIndex(index);

      m_ui->interactive_svg_CB->setChecked(           dotObj.value("interactive-svg").toBool());
      m_ui->dot_path->setText(                        dotObj.value("dot-path").toString());
      m_ui->dot_file_dirs->setPlainText(              getDataList(dotObj, "dot-file-dirs"));
      m_ui->msc_file_dirs->setPlainText(              getDataList(dotObj, "msc-file-dirs"));
      m_ui->dia_file_dirs->setPlainText(              getDataList(dotObj, "dia-file-dirs"));
      m_ui->plantuml_jar_path->setText(               dotObj.value("plantuml-jar-path").toString());
      m_ui->plantuml_inc_path->setPlainText(          getDataList(dotObj, "plantuml-inc-path"));

      m_ui->dot_graph_max_nodes_SB->setValue(         dotObj.value("dot-graph-max-nodes").toInt());
      m_ui->dot_graph_max_depth_SB->setValue(         dotObj.value("dot-graph-max-depth").toInt());
      m_ui->dot_transparent_CB->setChecked(           dotObj.value("dot-transparent").toBool());
      m_ui->dot_multiple_targets_CB->setChecked(      dotObj.value("dot-multiple-targets").toBool());
      m_ui->gen_legend_CB->setChecked(                dotObj.value("generate-legend").toBool());
      m_ui->dot_cleanup_CB->setChecked(               dotObj.value("dot-cleanup").toBool());

      //  ***
      // tab 3 - html
      m_ui->html_output->setText(                     htmlObj.value("html-output").toString());
      m_ui->html_file_extension->setText(             htmlObj.value("html-file-extension").toString());
      m_ui->html_header->setText(                     htmlObj.value("html-header").toString());
      m_ui->html_footer->setText(                     htmlObj.value("html-footer").toString());
      m_ui->html_stylesheets->setPlainText(           getDataList(htmlObj, "html-stylesheets"));
      m_ui->html_extra_files->setPlainText(           getDataList(htmlObj, "html-extra-files"));

      m_ui->html_colorstyle_hue->setValue(            htmlObj.value("html-colorstyle-hue").toInt());
      m_ui->html_colorstyle_sat->setValue(            htmlObj.value("html-colorstyle-sat").toInt());
      m_ui->html_colorstyle_gamma->setValue(          htmlObj.value("html-colorstyle-gamma").toInt());

      m_ui->html_timestamp_CB->setChecked(            htmlObj.value("html-timestamp").toBool());
      m_ui->html_dynamic_sections_CB->setChecked(     htmlObj.value("html-dynamic-sections").toBool());
      m_ui->html_index_num_entries_SB->setValue(      htmlObj.value("html-index-num-entries").toInt());

      m_ui->disable_index_CB->setChecked(             htmlObj.value("disable-index").toBool());
      m_ui->gen_treeview_CB->setChecked(              htmlObj.value("generate-treeview").toBool());
      m_ui->enum_values_per_line_SB->setValue(        htmlObj.value("enum-values-per-line").toInt());
      m_ui->treeview_width_SB->setValue(              htmlObj.value("treeview-width").toInt());
      m_ui->external_links_in_window_CB->setChecked(  htmlObj.value("external-links-in-window").toBool());

      m_ui->html_search_CB2->setChecked(              htmlObj.value("html-search").toBool());
      m_ui->search_server_based_CB->setChecked(       htmlObj.value("search-server-based").toBool());
      m_ui->search_external_CB->setChecked(           htmlObj.value("search-external").toBool());
      m_ui->search_external_url->setText(             htmlObj.value("search-external-url").toString());
      m_ui->search_data_file->setText(                htmlObj.value("search-data-file").toString());
      m_ui->search_external_id->setText(              htmlObj.value("search-external-id").toString());
      m_ui->search_mappings->setPlainText(            getDataList(htmlObj, "search-mappings"));

      m_ui->formula_fontsize_SB->setValue(            htmlObj.value("formula-fontsize").toInt());
      m_ui->formula_transparent_CB->setChecked(       htmlObj.value("formula-transparent").toBool());
      m_ui->ghostscript->setText(                     htmlObj.value("ghostscript").toString());
      m_ui->use_mathjax_CB->setChecked(               htmlObj.value("use-mathjax").toBool());

      index = m_ui->mathjax_format_CM->findText(      htmlObj.value("mathjax-format").toString());
      m_ui->mathjax_format_CM->setCurrentIndex(index);

      m_ui->mathjax_relpath->setText(                 htmlObj.value("mathjax-relpath").toString());
      m_ui->mathjax_extensions->setPlainText(         getDataList(htmlObj, "mathjax-extensions"));
      m_ui->mathjax_codefile->setText(                htmlObj.value("mathjax-codefile").toString());

      // tab 3 - chm
      m_ui->gen_chm_CB->setChecked(                   chmObj.value("generate-chm").toBool());
      m_ui->chm_file->setText(                        chmObj.value("chm-file").toString());
      m_ui->hhc_location->setText(                    chmObj.value("hhc-location").toString());
      m_ui->gen_chi_CB->setChecked(                   chmObj.value("generate-chi").toBool());
      m_ui->chm_index_encoding->setText(              chmObj.value("chm-index-encoding").toString());
      m_ui->binary_toc_CB->setChecked(                chmObj.value("binary-toc").toBool());
      m_ui->toc_expanded_CB->setChecked(              chmObj.value("toc-expanded").toBool());

      // tab 3 - docset
      m_ui->gen_docset_CB->setChecked(                docsetObj.value("generate-docset").toBool());
      m_ui->docset_feedname->setText(                 docsetObj.value("docset-feedname").toString());
      m_ui->docset_bundle_id->setText(                docsetObj.value("docset-bundle-id").toString());
      m_ui->docset_publisher_id->setText(             docsetObj.value("docset-publisher-id").toString());
      m_ui->docset_publisher_name->setText(           docsetObj.value("docset-publisher-name").toString());

      // tab 3 - eclipse
      m_ui->gen_eclipse_CB->setChecked(               eclipseObj.value("generate-eclipse").toBool());
      m_ui->eclipse_doc_id->setText(                  eclipseObj.value("eclipse-doc-id").toString());

      // tab 3 - latex
      m_ui->latex_output->setText(                    latexObj.value("latex-output").toString());
      m_ui->latex_cmd_name->setText(                  latexObj.value("latex-cmd-name").toString());
      m_ui->make_index_cmd_name->setText(             latexObj.value("make-index-cmd-name").toString());
      m_ui->latex_compact_CB->setChecked(             latexObj.value("latex-compact").toBool());

      index = m_ui->latex_paper_type_CM->findText(    latexObj.value("latex-paper-type").toString());
      m_ui->latex_paper_type_CM->setCurrentIndex(index);

      m_ui->latex_extra_packages->setPlainText(       getDataList(latexObj, "latex-extra-packages"));
      m_ui->latex_header->setText(                    latexObj.value("latex-header").toString());
      m_ui->latex_footer->setText(                    latexObj.value("latex-footer").toString());
      m_ui->latex_stylesheets->setPlainText(          getDataList(latexObj, "latex-stylesheets"));
      m_ui->latex_extra_files->setPlainText(          getDataList(latexObj, "latex-extra-files"));

      m_ui->latex_timestamp_CB->setChecked(           latexObj.value("latex-timestamp").toBool());
      m_ui->latex_hyper_pdf_CB->setChecked(           latexObj.value("latex-hyper-pdf").toBool());
      m_ui->latex_pdf_CB->setChecked(                 latexObj.value("latex-pdf").toBool());
      m_ui->latex_batch_mode_CB->setChecked(          latexObj.value("latex-batch-mode").toBool());
      m_ui->latex_hide_indices_CB->setChecked(        latexObj.value("latex-hide-indices").toBool());
      m_ui->latex_source_code_CB->setChecked(         latexObj.value("latex-source-code").toBool());

      // tab 3 - latex bib
      m_ui->latex_bib_style->setText(                 latexObj.value("latex-bib-style").toString());
      m_ui->cite_bib_files->setPlainText(             getDataList(latexObj, "cite-bib-files"));

      // tab 3 - man
      m_ui->man_output->setText(                      manObj.value("man-output").toString());
      m_ui->man_extension->setText(                   manObj.value("man-extension").toString());
      m_ui->man_subdir->setText(                      manObj.value("man-subdir").toString());
      m_ui->man_links_CB->setChecked(                 manObj.value("man-links").toBool());

      // tab 3 - perl modoule
      m_ui->gen_perl_CB->setChecked(                  perlObj.value("generate-perl").toBool());
      m_ui->perl_latex_CB->setChecked(                perlObj.value("perl-latex").toBool());
      m_ui->perl_pretty_CB->setChecked(               perlObj.value("perl-pretty").toBool());
      m_ui->perl_prefix->setText(                     perlObj.value("perl-prefix").toString());

      // tab 3 - qt help
      m_ui->gen_qthelp_CB->setChecked(                qhelpObj.value("generate-qthelp").toBool());
      m_ui->qch_file->setText(                        qhelpObj.value("qch-file").toString());
      m_ui->qhp_namespace->setText(                   qhelpObj.value("qhp-namespace").toString());
      m_ui->qhp_virtual_folder->setText(              qhelpObj.value("qhp-virtual-folder").toString());
      m_ui->qhp_cust_filter_name->setText(            qhelpObj.value("qhp-cust-filter-name").toString());
      m_ui->qhp_cust_attrib->setPlainText(            getDataList(qhelpObj, "qhp-cust-attrib"));
      m_ui->qhp_sect_attrib->setPlainText(            getDataList(qhelpObj, "qhp-sect-attrib"));
      m_ui->qthelp_gen_path->setText(                 qhelpObj.value("qthelp-gen-path").toString());

      // tab 3 - rtf
      m_ui->rtf_output->setText(                      rtfObj.value("rtf-output").toString());
      m_ui->rtf_compact_CB->setChecked(               rtfObj.value("rtf-compact").toBool());
      m_ui->rtf_hyperlinks_CB->setChecked(            rtfObj.value("rtf-hyperlinks").toBool());
      m_ui->rtf_stylesheet->setText(                  rtfObj.value("rtf-stylesheet").toString());
      m_ui->rtf_extension->setText(                   rtfObj.value("rtf-extension").toString());
      m_ui->rtf_source_code_CB->setChecked(           rtfObj.value("rtf-source-code").toBool());

      // tab 3 - xml
      m_ui->xml_output->setText(                      xmlObj.value("xml-output").toString());
      m_ui->xml_program_listing_CB->setChecked(       xmlObj.value("xml-program-listing").toBool());

      // tab 3 - docbook
      m_ui->docbook_output->setText(                  docbookObj.value("docbook-output").toString());
      m_ui->docbook_program_listing_CB->setChecked(   docbookObj.value("docbook-program-listing").toBool());
   }

   // final step
   finalLoad();
}

QByteArray MainWindow::json_SaveDoxy()
{
   QJsonObject object;

   QJsonObject projectObj;
   QJsonObject genObj;
   QJsonObject configObj;
   QJsonObject msgObj;
   QJsonObject inputObj;
   QJsonObject indexObj;
   QJsonObject sourceObj;
   QJsonObject ppObj;
   QJsonObject extObj;
   QJsonObject dotObj;
   QJsonObject bbObj;

   QJsonObject htmlObj;
   QJsonObject chmObj;
   QJsonObject docbookObj;
   QJsonObject docsetObj;
   QJsonObject eclipseObj;
   QJsonObject latexObj;
   QJsonObject manObj;
   QJsonObject perlObj;
   QJsonObject qhelpObj;
   QJsonObject rtfObj;
   QJsonObject xmlObj;

   m_doxypressFormat = DOXYPRESS_FORMAT;
   object.insert("doxypress-format",      m_doxypressFormat);
   object.insert("doxypress-updated",     QString{"2015-Oct-15"} );   

   // tab 1
   projectObj.insert("project-name",      m_ui->project_name->text());
   projectObj.insert("project-brief",     m_ui->project_brief->text());
   projectObj.insert("project-version",   m_ui->project_version->text());
   projectObj.insert("project-logo",      m_ui->project_logo->text());

   genObj.insert("output-dir",            m_ui->output_dir->text());
   genObj.insert("optimize-cplus",        m_ui->optimize_cplus_RB->isChecked());
   genObj.insert("optimize-java",         m_ui->optimize_java_RB->isChecked());
   genObj.insert("optimize-c",            m_ui->optimize_c_RB->isChecked());
   genObj.insert("optimize-fortran",      m_ui->optimize_fortran_RB->isChecked());

   htmlObj.insert("generate-html",        m_ui->gen_html_CB1->isChecked());

   latexObj.insert("generate-latex",      m_ui->gen_latex_CB1->isChecked());
   latexObj.insert("latex-hyper-pdf",     m_ui->latex_hyper_pdf_RB->isChecked());
   latexObj.insert("latex-pdf",           m_ui->latex_pdf_RB->isChecked());
   latexObj.insert("latex-ps",            m_ui->latex_ps_RB->isChecked());

   rtfObj.insert("generate-rtf",          m_ui->gen_rtf_CB1->isChecked());
   manObj.insert("generate-man",          m_ui->gen_man_CB1->isChecked());
   xmlObj.insert("generate-xml",          m_ui->gen_xml_CB1->isChecked());
   docbookObj.insert("generate-docbook",  m_ui->gen_docbook_CB1->isChecked());

   dotObj.insert("dot-class-graph",       m_ui->dot_class_graph_CB1->isChecked());
   dotObj.insert("dot-collaboration",     m_ui->dot_collaboration_CB1->isChecked());
   dotObj.insert("dot-hierarchy",         m_ui->dot_hierarchy_CB1->isChecked());
   dotObj.insert("dot-include",           m_ui->dot_include_CB1->isChecked());
   dotObj.insert("dot-included-by",       m_ui->dot_included_by_CB1->isChecked());
   dotObj.insert("dot-call",              m_ui->dot_call_CB1->isChecked());
   dotObj.insert("dot-called-by",         m_ui->dot_called_by_CB1->isChecked());

   //  ***
   // tab 2 - general
   genObj.insert("output-language",             m_ui->output_language_CM->currentText());
   genObj.insert("aliases",                     putDataList(m_ui->aliases->toPlainText()));
   genObj.insert("abbreviate-brief",            putDataList(m_ui->abbreviate_brief->toPlainText()));
   genObj.insert("strip-from-path",             putDataList(m_ui->strip_from_path->toPlainText()));
   genObj.insert("strip-from-inc-path",         putDataList(m_ui->strip_from_inc_path->toPlainText()));

   configObj.insert("full-path-names",          m_ui->full_path_names_CB->isChecked());
   configObj.insert("brief-member-desc",        m_ui->brief_member_desc_CB->isChecked());
   configObj.insert("repeat-brief",             m_ui->repeat_brief_CB->isChecked());
   configObj.insert("always-detailed-sec",      m_ui->always_detailed_sec_CB->isChecked());
   configObj.insert("javadoc-auto-brief",       m_ui->javadoc_auto_brief_CB->isChecked());
   configObj.insert("qt-auto-brief",            m_ui->qt_auto_brief_CB->isChecked());
   configObj.insert("multiline-cpp-brief",      m_ui->multiline_cpp_brief_CB->isChecked());

   genObj.insert("tab-size",                    m_ui->tab_size_SB->value());
   genObj.insert("lookup-cache-size",           m_ui->lookup_cache_size_SB->value());

   // tab 2 - build confg
   configObj.insert("extract-all",              m_ui->extract_all_CB->isChecked());
   configObj.insert("extract-private",          m_ui->extract_private_CB->isChecked());
   configObj.insert("extract-package",          m_ui->extract_package_CB->isChecked());
   configObj.insert("extract-static",           m_ui->extract_static_CB->isChecked());
   configObj.insert("extract-local-classes",    m_ui->extract_local_classes_CB->isChecked());
   configObj.insert("extract-local-methods",    m_ui->extract_local_methods_CB->isChecked());
   configObj.insert("extract-anon-namespaces",  m_ui->extract_anon_namespaces_CB->isChecked());
   configObj.insert("hide-undoc-members",       m_ui->hide_undoc_members_CB->isChecked());
   configObj.insert("hide-undoc-classes",       m_ui->hide_undoc_classes_CB->isChecked());
   configObj.insert("hide-friend-compounds",    m_ui->hide_friend_compounds_CB->isChecked());
   configObj.insert("hide-in-body-docs",        m_ui->hide_in_body_docs_CB->isChecked());
   configObj.insert("hide-scope-names",         m_ui->hide_scope_names_CB->isChecked());
   configObj.insert("hide-compound-ref",        m_ui->hide_compound_ref_CB->isChecked());
   configObj.insert("hide-navtree-members",     m_ui->hide_navtree_members_CB->isChecked());

   configObj.insert("show-include-files",       m_ui->show_include_files_CB->isChecked());
   configObj.insert("show-grouped-members-inc", m_ui->show_grouped_members_inc_CB->isChecked());
   configObj.insert("show-used-files",          m_ui->show_used_files_CB->isChecked());
   configObj.insert("show-file-page",           m_ui->show_file_page_CB->isChecked());
   configObj.insert("show-namespace-page",      m_ui->show_namespace_page_CB->isChecked());

   configObj.insert("inline-inherited-member",  m_ui->inline_inherited_member_CB->isChecked());
   configObj.insert("inline-info",              m_ui->inline_info_CB->isChecked());
   configObj.insert("inline-grouped-classes",   m_ui->inline_grouped_classes_CB->isChecked());
   configObj.insert("inline-simple-struct",     m_ui->inline_simple_struct_CB->isChecked());
   configObj.insert("use-typedef-name",         m_ui->use_typedef_name_CB->isChecked());

   configObj.insert("sort-member-docs",         m_ui->sort_member_docs_CB->isChecked());
   configObj.insert("sort-brief-docs",          m_ui->sort_brief_docs_CB->isChecked());
   configObj.insert("sort-constructors-first",  m_ui->sort_constructors_first_CB->isChecked());
   configObj.insert("sort-group-names",         m_ui->sort_group_names_CB->isChecked());
   configObj.insert("sort-by-scope-name",       m_ui->sort_by_scope_name_CB->isChecked());
   configObj.insert("sort-class-case-sensitive",m_ui->sort_class_case_sensitive_CB->isChecked());

   configObj.insert("generate-todo-list",       m_ui->gen_todo_list_CB->isChecked());
   configObj.insert("generate-test-list",       m_ui->gen_test_list_CB->isChecked());
   configObj.insert("generate-bug-list",        m_ui->gen_bug_list_CB->isChecked());
   configObj.insert("generate-deprecate-list",  m_ui->gen_deprecate_list_CB->isChecked());

   configObj.insert("short-names",              m_ui->short_names_CB->isChecked());
   configObj.insert("allow-unicode-names",      m_ui->allow_unicode_names_CB->isChecked());
   configObj.insert("create-subdirs",           m_ui->create_subdirs_CB->isChecked());
   configObj.insert("case-sensitive-fname",     m_ui->case_sensitive_fname_CB->isChecked());

   configObj.insert("markdown",                 m_ui->markdown_CB->isChecked());
   configObj.insert("auto-link",                m_ui->auto_link_CB->isChecked());
   configObj.insert("strict-sig-matching",      m_ui->strict_sig_matching_CB->isChecked());

   configObj.insert("internal-docs",            m_ui->internal_docs_CB->isChecked());
   configObj.insert("force-local-includes",     m_ui->force_local_includes_CB->isChecked());
   configObj.insert("inherit-docs",             m_ui->inherit_docs_CB->isChecked());

   configObj.insert("separate-member-pages",    m_ui->separate_member_pages_CB->isChecked());
   configObj.insert("allow-sub-grouping",       m_ui->allow_sub_grouping_CB->isChecked());
   configObj.insert("duplicate-docs",           m_ui->duplicate_docs_CB->isChecked());
   configObj.insert("group-nested-compounds",   m_ui->group_nested_compounds_CB->isChecked());

   // tab 2 -build options
   configObj.insert("max-init-lines",           m_ui->max_init_lines_SB->value());
   configObj.insert("enabled-sections",         putDataList(m_ui->enabled_sections->toPlainText()));   
   configObj.insert("file-version-filter",      m_ui->file_version_filter->text());
   configObj.insert("main-page-name",           m_ui->main_page_name->text());
   configObj.insert("main-page-omit",           m_ui->main_page_omit->isChecked());
   configObj.insert("layout-file",              m_ui->layout_file->text());           
   configObj.insert("ns-alias",                 putDataList(m_ui->ns_alias->toPlainText()));
   bbObj.insert("bb-style",                     m_ui->bb_style_CB->isChecked());

   // tab 2 -programming
   configObj.insert("tcl-subst",                putDataList(m_ui->tcl_subst->toPlainText()));
   configObj.insert("language-mapping",         putDataList(m_ui->language_mapping->toPlainText()));
   configObj.insert("built-in-stl-support",     m_ui->built_in_stl_support_CB->isChecked());
   configObj.insert("cpp-cli-support",          m_ui->cpp_cli_support_CB->isChecked());
   configObj.insert("sip-support",              m_ui->sip_support_CB->isChecked());
   configObj.insert("idl-support",              m_ui->idl_support_CB->isChecked());

   // tab 2 - messages
   msgObj.insert("quiet",                       m_ui->quiet_CB->isChecked());
   msgObj.insert("warnings",                    m_ui->warnings_CB->isChecked());
   msgObj.insert("warn-undoc",                  m_ui->warn_undoc_CB->isChecked());
   msgObj.insert("warn-doc-error",              m_ui->warn_doc_error_CB->isChecked());
   msgObj.insert("warn-undoc-param",            m_ui->warn_undoc_param_CB->isChecked());
   msgObj.insert("warn-format",                 m_ui->warn_format->text());
   msgObj.insert("warn-logfile",                m_ui->warn_logfile->text());

   // tab 2 - input src
   inputObj.insert("input-source",              putDataList(m_ui->input_source->toPlainText()));
   inputObj.insert("input-patterns",            putDataList(m_ui->input_patterns->toPlainText()));
   inputObj.insert("input-encoding",            m_ui->input_encoding->text());  
   inputObj.insert("input-recursive",           m_ui->input_recursive_CB->isChecked());

   inputObj.insert("exclude-files",             putDataList(m_ui->exclude_files->toPlainText()));
   inputObj.insert("exclude-symlinks",          m_ui->exclude_symlinks_CB->isChecked());
   inputObj.insert("exclude-patterns",          putDataList(m_ui->exclude_patterns->toPlainText()));
   inputObj.insert("exclude-symbols",           putDataList(m_ui->exclude_symbols->toPlainText()));

   // tab 2 - input other
   inputObj.insert("example-source",            putDataList(m_ui->example_source->toPlainText()));
   inputObj.insert("example-patterns",          putDataList(m_ui->example_patterns->toPlainText()));
   inputObj.insert("example-recursive",         m_ui->example_recursive_CB->isChecked());

   inputObj.insert("image-path",                putDataList(m_ui->image_path->toPlainText()));
   inputObj.insert("mdfile-mainpage",           m_ui->mdfile_mainpage->text());

   // tab 2 -index filters
   inputObj.insert("filter-program",            m_ui->filter_program->text());
   inputObj.insert("filter-patterns",           putDataList(m_ui->filter_patterns->toPlainText()));
   inputObj.insert("filter-source-files",       m_ui->filter_source_files_CB->isChecked());
   inputObj.insert("filter-source-patterns",    putDataList(m_ui->filter_source_patterns->toPlainText()));

   // tab 2 -index page
   indexObj.insert("alpha-index",               m_ui->alpha_index_CB->isChecked());
   indexObj.insert("cols-in-index",             m_ui->cols_in_index_SB->value());
   indexObj.insert("ignore-prefix",             putDataList(m_ui->ignore_prefix->toPlainText()));

   // tab 2 - source code
   sourceObj.insert("source-code",              m_ui->source_code_CB->isChecked());
   sourceObj.insert("inline-source",            m_ui->inline_source_CB->isChecked());
   sourceObj.insert("strip-code-comments",      m_ui->strip_code_comments_CB->isChecked());
   sourceObj.insert("verbatim-headers",         m_ui->verbatim_headers_CB->isChecked());
   sourceObj.insert("ref-by-relation",          m_ui->ref_by_relation_CB->isChecked());
   sourceObj.insert("ref-relation",             m_ui->ref_relation_CB->isChecked());
   sourceObj.insert("ref-link-source",          m_ui->ref_link_source_CB->isChecked());
   sourceObj.insert("source-tooltips",          m_ui->source_tooltips_CB->isChecked());
   sourceObj.insert("use-htags",                m_ui->use_htags_CB->isChecked());
   sourceObj.insert("suffix-source-navtree",    putDataList(m_ui->suffix_source_navtree->toPlainText()));
   sourceObj.insert("suffix-header-navtree",    putDataList(m_ui->suffix_header_navtree->toPlainText()));
   sourceObj.insert("suffix-exclude-navtree",   putDataList(m_ui->suffix_exclude_navtree->toPlainText()));
   sourceObj.insert("clang-parsing",            m_ui->clang_parsing_CB->isChecked());
   sourceObj.insert("clang-flags",              putDataList(m_ui->clang_flags->toPlainText()));

   // tab 2 - preprocess
   ppObj.insert("enable-preprocessing",         m_ui->enable_preprocessing_CB->isChecked());
   ppObj.insert("search-includes",              m_ui->search_includes_CB->isChecked());

   ppObj.insert("include-path",                 putDataList(m_ui->include_path->toPlainText()));
   ppObj.insert("include-patterns",             putDataList(m_ui->include_patterns->toPlainText()));

   ppObj.insert("macro-expansion",              m_ui->macro_expansion_CB->isChecked());
   ppObj.insert("expand-only-predefined",       m_ui->expand_only_predefined_CB->isChecked());
   ppObj.insert("skip-function-macros",         m_ui->skip_function_macros_CB->isChecked());

   ppObj.insert("predefined-macros",            putDataList(m_ui->predefined_macros->toPlainText()));
   ppObj.insert("expand-as-defined",            putDataList(m_ui->expand_as_defined->toPlainText()));

   // tab 2 - external
   extObj.insert("tag-files",                 putDataList(m_ui->tag_files->toPlainText()));
   extObj.insert("generate-tagfile",          m_ui->gen_tagfile->text());
   extObj.insert("all-externals",             m_ui->all_externals_CB->isChecked());
   extObj.insert("external-groups",           m_ui->external_groups_CB->isChecked());
   extObj.insert("external-pages",            m_ui->external_pages_CB->isChecked());
   extObj.insert("perl-path",                 m_ui->perl_path->text());

   // tab 2 - dot
   dotObj.insert("class-diagrams",            m_ui->class_diagrams_CB->isChecked());
   dotObj.insert("mscgen-path",               m_ui->mscgen_path->text());
   dotObj.insert("dia-path",                  m_ui->dia_path->text());
   dotObj.insert("hide-undoc-relations",      m_ui->hide_undoc_relations_CB->isChecked());
   dotObj.insert("have-dot",                  m_ui->have_dot_CB->isChecked());
   dotObj.insert("dot-num-threads",           m_ui->dot_num_threads_SB->value());
   dotObj.insert("dot-font-name",             m_ui->dot_font_name->text());
   dotObj.insert("dot-font-size",             m_ui->dot_font_size_SB->value());
   dotObj.insert("dot-font-path",             m_ui->dot_font_path->text());

   dotObj.insert("group-graphs",              m_ui->group_graphs_CB->isChecked());
   dotObj.insert("uml-look",                  m_ui->uml_look_CB->isChecked());
   dotObj.insert("uml-limit-num-fields",      m_ui->uml_limit_num_fields_SB->value());
   dotObj.insert("template-relations",        m_ui->template_relations_CB->isChecked());
   dotObj.insert("directory-graph",           m_ui->directory_graph_CB->isChecked());
   dotObj.insert("dot-image-format",          m_ui->dot_image_format_CM->currentText());

   dotObj.insert("interactive-svg",           m_ui->interactive_svg_CB->isChecked());
   dotObj.insert("dot-path",                  m_ui->dot_path->text());
   dotObj.insert("dot-file-dirs",             putDataList(m_ui->dot_file_dirs->toPlainText()));
   dotObj.insert("msc-file-dirs",             putDataList(m_ui->msc_file_dirs->toPlainText()));
   dotObj.insert("dia-file-dirs",             putDataList(m_ui->dia_file_dirs->toPlainText()));
   dotObj.insert("plantuml-jar-path",         m_ui->plantuml_jar_path->text());
   dotObj.insert("plantuml-inc-path",         putDataList(m_ui->plantuml_inc_path->toPlainText()));

   dotObj.insert("dot-graph-max-nodes",       m_ui->dot_graph_max_nodes_SB->value());
   dotObj.insert("dot-graph-max-depth",       m_ui->dot_graph_max_depth_SB->value());
   dotObj.insert("dot-transparent",           m_ui->dot_transparent_CB->isChecked());
   dotObj.insert("dot-multiple-targets",      m_ui->dot_multiple_targets_CB->isChecked());
   dotObj.insert("generate-legend",           m_ui->gen_legend_CB->isChecked());
   dotObj.insert("dot-cleanup",               m_ui->dot_cleanup_CB->isChecked());

   //  ***
   // tab 3 - html
   htmlObj.insert("html-output",              m_ui->html_output->text());
   htmlObj.insert("html-file-extension",      m_ui->html_file_extension->text());
   htmlObj.insert("html-header",              m_ui->html_header->text());
   htmlObj.insert("html-footer",              m_ui->html_footer->text());
   htmlObj.insert("html-stylesheets",         putDataList(m_ui->html_stylesheets->toPlainText()));
   htmlObj.insert("html-extra-files",         putDataList(m_ui->html_extra_files->toPlainText()));

   htmlObj.insert("html-colorstyle-hue",      m_ui->html_colorstyle_hue->value());
   htmlObj.insert("html-colorstyle-sat",      m_ui->html_colorstyle_sat->value());
   htmlObj.insert("html-colorstyle-gamma",    m_ui->html_colorstyle_gamma->value());

   htmlObj.insert("html-timestamp",           m_ui->html_timestamp_CB->isChecked());
   htmlObj.insert("html-dynamic-sections",    m_ui->html_dynamic_sections_CB->isChecked());;
   htmlObj.insert("html-index-num-entries",   m_ui->html_index_num_entries_SB->value());

   htmlObj.insert("disable-index",            m_ui->disable_index_CB->isChecked());
   htmlObj.insert("generate-treeview",        m_ui->gen_treeview_CB->isChecked());
   htmlObj.insert("enum-values-per-line",     m_ui->enum_values_per_line_SB->value());
   htmlObj.insert("treeview-width",           m_ui->treeview_width_SB->value());
   htmlObj.insert("external-links-in-window", m_ui->external_links_in_window_CB->isChecked());

   htmlObj.insert("html-search",              m_ui->html_search_CB2->isChecked());
   htmlObj.insert("search-server-based",      m_ui->search_server_based_CB->isChecked());
   htmlObj.insert("search-external",          m_ui->search_external_CB->isChecked());
   htmlObj.insert("search-external-url",      m_ui->search_external_url->text());
   htmlObj.insert("search-data-file",         m_ui->search_data_file->text());
   htmlObj.insert("search-external-id",       m_ui->search_external_id->text());
   htmlObj.insert("search-mappings",          putDataList(m_ui->search_mappings->toPlainText()));

   htmlObj.insert("formula-fontsize",         m_ui->formula_fontsize_SB->value());
   htmlObj.insert("formula-transparent",      m_ui->formula_transparent_CB->isChecked());
   htmlObj.insert("ghostscript",              m_ui->ghostscript->text());
   htmlObj.insert("use-mathjax",              m_ui->use_mathjax_CB->isChecked());
   htmlObj.insert("mathjax-format",           m_ui->mathjax_format_CM->currentText());
   htmlObj.insert("mathjax-relpath",          m_ui->mathjax_relpath->text());
   htmlObj.insert("mathjax-extensions",       putDataList(m_ui->mathjax_extensions->toPlainText()));
   htmlObj.insert("mathjax-codefile",         m_ui->mathjax_codefile->text());

   // tab 3 - chm
   chmObj.insert("generate-chm",              m_ui->gen_chm_CB->isChecked());
   chmObj.insert("chm-file",                  m_ui->chm_file->text());
   chmObj.insert("hhc-location",              m_ui->hhc_location->text());
   chmObj.insert("generate-chi",              m_ui->gen_chi_CB->isChecked());
   chmObj.insert("chm-index-encoding",        m_ui->chm_index_encoding->text());
   chmObj.insert("binary-toc",                m_ui->binary_toc_CB->isChecked());
   chmObj.insert("toc-expanded",              m_ui->toc_expanded_CB->isChecked());

   // tab 3 - docset
   docsetObj.insert("generate-docset",        m_ui->gen_docset_CB->isChecked());
   docsetObj.insert("docset-feedname",        m_ui->docset_feedname->text());
   docsetObj.insert("docset-bundle-id",       m_ui->docset_bundle_id->text());
   docsetObj.insert("docset-publisher-id",    m_ui->docset_publisher_id->text());
   docsetObj.insert("docset-publisher-name",  m_ui->docset_publisher_name->text());

   // tab 3 - eclipse
   eclipseObj.insert("generate-eclipse",      m_ui->gen_eclipse_CB->isChecked());
   eclipseObj.insert("eclipse-doc-id",        m_ui->eclipse_doc_id->text());

   // tab 3 - latex
   latexObj.insert("latex-output",            m_ui->latex_output->text());
   latexObj.insert("latex-cmd-name",          m_ui->latex_cmd_name->text());
   latexObj.insert("make-index-cmd-name",     m_ui->make_index_cmd_name->text());
   latexObj.insert("latex-compact",           m_ui->latex_compact_CB->isChecked());
   latexObj.insert("latex-paper-type",        m_ui->latex_paper_type_CM->currentText());

   latexObj.insert("latex-extra-packages",    putDataList(m_ui->latex_extra_packages->toPlainText()));
   latexObj.insert("latex-header",            m_ui->latex_header->text());
   latexObj.insert("latex-footer",            m_ui->latex_footer->text());
   latexObj.insert("latex-stylesheets",       putDataList(m_ui->latex_stylesheets->toPlainText()));
   latexObj.insert("latex-extra-files",       putDataList(m_ui->latex_extra_files->toPlainText()));

   latexObj.insert("latex-timestamp",         m_ui->latex_timestamp_CB->isChecked());
   latexObj.insert("latex-hyper-pdf",         m_ui->latex_hyper_pdf_CB->isChecked());
   latexObj.insert("latex-pdf",               m_ui->latex_pdf_CB->isChecked());
   latexObj.insert("latex-batch-mode",        m_ui->latex_batch_mode_CB->isChecked());
   latexObj.insert("latex-hide-indices",      m_ui->latex_hide_indices_CB->isChecked());
   latexObj.insert("latex-source-code",       m_ui->latex_source_code_CB->isChecked());

   // tab 3 - latex bib
   latexObj.insert("latex-bib-style",         m_ui->latex_bib_style->text());
   latexObj.insert("cite-bib-files",          putDataList(m_ui->cite_bib_files->toPlainText()));

   // tab 3 - man
   manObj.insert("man-output",                m_ui->man_output->text());
   manObj.insert("man-extension",             m_ui->man_extension->text());
   manObj.insert("man-subdir",                m_ui->man_subdir->text());
   manObj.insert("man-links",                 m_ui->man_links_CB->isChecked());

   // tab 3 - perl module
   perlObj.insert("generate-perl",            m_ui->gen_perl_CB->isChecked());
   perlObj.insert("perl-latex",               m_ui->perl_latex_CB->isChecked());
   perlObj.insert("perl-pretty",              m_ui->perl_pretty_CB->isChecked());
   perlObj.insert("perl-prefix",              m_ui->perl_prefix->text());

   // tab 3 - qt help
   qhelpObj.insert("generate-qthelp",         m_ui->gen_qthelp_CB->isChecked());
   qhelpObj.insert("qch-file",                m_ui->qch_file->text());
   qhelpObj.insert("qhp-namespace",           m_ui->qhp_namespace->text());
   qhelpObj.insert("qhp-virtual-folder",      m_ui->qhp_virtual_folder->text());
   qhelpObj.insert("qhp-cust-filter-name",    m_ui->qhp_cust_filter_name->text());
   qhelpObj.insert("qhp-cust-attrib",         putDataList(m_ui->qhp_cust_attrib->toPlainText()));
   qhelpObj.insert("qhp-sect-attrib",         putDataList(m_ui->qhp_sect_attrib->toPlainText()));
   qhelpObj.insert("qthelp-gen-path",         m_ui->qthelp_gen_path->text());

   // tab 3 - rtf
   rtfObj.insert("rtf-output",                m_ui->rtf_output->text());
   rtfObj.insert("rtf-compact",               m_ui->rtf_compact_CB->isChecked());
   rtfObj.insert("rtf-hyperlinks",            m_ui->rtf_hyperlinks_CB->isChecked());
   rtfObj.insert("rtf-stylesheet",            m_ui->rtf_stylesheet->text());
   rtfObj.insert("rtf-extension",             m_ui->rtf_extension->text());
   rtfObj.insert("rtf-source-code",           m_ui->rtf_source_code_CB->isChecked());

   // tab 3 - xml
   xmlObj.insert("xml-output",                m_ui->xml_output->text());
   xmlObj.insert("xml-program-listing",       m_ui->xml_program_listing_CB->isChecked());

   // tab 3 - docbook
   docbookObj.insert("docbook-output",           m_ui->docbook_output->text());
   docbookObj.insert("docbook-program-listing",  m_ui->docbook_program_listing_CB->isChecked());

   // save objects
   object.insert("project",         projectObj);
   object.insert("general",         genObj);
   object.insert("configuration",   configObj);
   object.insert("messages",        msgObj);
   object.insert("input",           inputObj);
   object.insert("index",           indexObj);
   object.insert("source",          sourceObj);
   object.insert("preprocessor",    ppObj);
   object.insert("external",        extObj);
   object.insert("dot",             dotObj);
   object.insert("bb",              bbObj);

   object.insert("output-html",     htmlObj);
   object.insert("output-chm",      chmObj);
   object.insert("output-docbook",  docbookObj);
   object.insert("output-docset",   docsetObj);
   object.insert("output-eclipse",  eclipseObj);
   object.insert("output-latex",    latexObj);
   object.insert("output-man",      manObj);
   object.insert("output-perl",     perlObj);
   object.insert("output-qhelp",    qhelpObj);
   object.insert("output-rtf",      rtfObj);
   object.insert("output-xml",      xmlObj);

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
