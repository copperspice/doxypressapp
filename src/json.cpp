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

   QString temp;

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


   // tab 2 - project
   m_ui->project_encoding->setText( object.value("project-encoding").toString());
   m_ui->create_subdirs_CB->setChecked( object.value("create-subdirs").toBool());
   m_ui->allow_unicode_names_CB->setChecked( object.value("allow-unicode-names").toBool());


   // tab 2 - build


   m_ui->show_used_files_CB->setChecked( object.value("show-used-files").toBool());;
   m_ui->layout_file->setText( object.value("layout-file").toString());

   m_ui->file_version_filter->setText( object.value("file-version-filter").toString());


   // tab 2 - messages



   m_ui->warn_forrmat->setText( object.value("warn-forrmat").toString());
   m_ui->warn_logfile->setText( object.value("warn-logfile").toString());

   // tab 2 -input
   m_ui->input_source->setPlainText(getDataList(object, "source-input"));
   m_ui->input_encoding->setText( object.value("input-encoding").toString());
   m_ui->file_patterns->setPlainText(getDataList(object, "file-patterns"));
   m_ui->source_recursive_CB->setChecked( object.value("source-recursive").toBool());
   m_ui->exclude_files->setPlainText(getDataList(object, "exclude-files"));


//  m_ui->exclude_patterns->setPlainText("");
//  m_ui->exclude_symbols->setPlainText("");
    m_ui->exclude_symlinks_CB->setChecked( object.value("exclude-symlinks").toBool());

    m_ui->mdfile_mainpage->setText( object.value("mdfile-mainpage").toString());



   // tab 2 -browser


   // tab 2 -index

   // tab 2 - autogen

   // tab 2 - perlmod

   m_ui->perlmod_prefix->setText( object.value("perlmod-prefix").toString());


   // tab 2 - preprocess


   // tab 2 - external


   m_ui->generate_tagfile->setText( object.value("generate-tagfile").toString());
   m_ui->perl_path->setText( object.value("perl-path").toString());


   // tab 2 - dot



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
   QJsonArray list;

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

   // tab 2
   object.insert("source-recursive",   m_ui->source_recursive_CB->isChecked());

   list = QJsonArray();
   QStringList temp = m_ui->input_source->toPlainText().split(", ");

   for (auto s : temp) {
      list.append(s);
   }

   object.insert("source-input",  list);




   // save the data
   QJsonDocument doc(object);
   QByteArray data = doc.toJson();

   return data;
}

QString MainWindow::getDataList(QJsonObject &object, QString field)
{
   QString retval;
   QStringList dataList;

   QJsonArray list = object.value(field).toArray();
   int cnt = list.count();

   for (int k = 0; k < cnt; k++)  {
      dataList.append(list.at(k).toString());
   }

   retval = dataList.join(", ");

   return retval;
}



/*

  <tabstop>output_langugae_CM</tabstop>
  <tabstop>brief_member_desc_CB</tabstop>
  <tabstop>repeat_brief_CB</tabstop>
  <tabstop>abbreviate_brief</tabstop>
  <tabstop>abbreviate_brief_PB</tabstop>
  <tabstop>always_detailed_sec_CB</tabstop>
  <tabstop>inline_inherited_member_CB</tabstop>
  <tabstop>full_path_names_CB</tabstop>
  <tabstop>strip_from_path</tabstop>
  <tabstop>strip_from_path_PB</tabstop>
  <tabstop>strip_from_inc_path</tabstop>
  <tabstop>strip_from_inc_path_PB</tabstop>
  <tabstop>short_names_CB</tabstop>
  <tabstop>javadoc_auto_brief_CB</tabstop>
  <tabstop>qt_auto_brief_CB</tabstop>
  <tabstop>multiline_cpp_brief_CB</tabstop>
  <tabstop>inherit_docs_CB</tabstop>
  <tabstop>separate_member_pages_CB</tabstop>
  <tabstop>tab_size</tabstop>
  <tabstop>aliases</tabstop>
  <tabstop>aliases_PB</tabstop>
  <tabstop>tcl_subst</tabstop>
  <tabstop>tcl_subst_PB</tabstop>
  <tabstop>extension_mapping</tabstop>
  <tabstop>extension_mapping_PB</tabstop>
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
  <tabstop>extract__package</tabstop>
  <tabstop>extract_static</tabstop>
  <tabstop>extract_local_classes</tabstop>
  <tabstop>extract_local_methods</tabstop>
  <tabstop>extract_anon_namespaces</tabstop>
  <tabstop>hide_undoc_members</tabstop>
  <tabstop>hide_undoc_classes</tabstop>
  <tabstop>hide_friend_compounds</tabstop>
  <tabstop>hide_in_body_docs</tabstop>
  <tabstop>internal_docs</tabstop>
  <tabstop>case_sense_names</tabstop>
  <tabstop>checkBox_76</tabstop>
  <tabstop>checkBox_78</tabstop>
  <tabstop>checkBox_77</tabstop>
  <tabstop>checkBox_79</tabstop>
  <tabstop>checkBox_80</tabstop>
  <tabstop>checkBox_82</tabstop>
  <tabstop>checkBox_81</tabstop>
  <tabstop>checkBox_83</tabstop>
  <tabstop>checkBox_84</tabstop>
  <tabstop>checkBox_86</tabstop>
  <tabstop>checkBox</tabstop>
  <tabstop>checkBox_85</tabstop>
  <tabstop>checkBox_87</tabstop>
  <tabstop>checkBox_88</tabstop>
  <tabstop>checkBox_90</tabstop>
  <tabstop>enabled_sections</tabstop>
  <tabstop>show_used_files_CB</tabstop>
  <tabstop>show_files_CB</tabstop>
  <tabstop>show_namespaces_CB</tabstop>
  <tabstop>file_version_filter</tabstop>
  <tabstop>layout_file</tabstop>
  <tabstop>cite_bib_files</tabstop>

  // messages
  <tabstop>quiet_CB</tabstop>
  <tabstop>warnings_CB</tabstop>
  <tabstop>warn_undoc_CB</tabstop>
  <tabstop>warn_syntax_CB</tabstop>
  <tabstop>warn_undoc_parm_CB</tabstop>
  <tabstop>warn_forrmat</tabstop>
  <tabstop>warn_logfile</tabstop>
  <tabstop>warn_logfile_PB</tabstop>

  // input
  <tabstop>exclude_patterns</tabstop>
  <tabstop>exclude_patterns_PB</tabstop>
  <tabstop>exclude_symbols</tabstop>
  <tabstop>example_souce</tabstop>
  <tabstop>example_souce_PB</tabstop>
  <tabstop>example_patterns</tabstop>

  <tabstop>example_recursive_CB</tabstop>
  <tabstop>image_path</tabstop>
  <tabstop>input_filter</tabstop>
  <tabstop>filter_patterns</tabstop>
  <tabstop>filter_patterns_PB</tabstop>
  <tabstop>checkBox_401</tabstop>
  <tabstop>filter_source_patterns</tabstop>
  <tabstop>filter_source_patterns_PB</tabstop>
  <tabstop>mdfile_mainpage</tabstop>

  // browser
  <tabstop>source_browser_CB</tabstop>
  <tabstop>inline_sources_CB</tabstop>
  <tabstop>strip_code_comments_CB</tabstop>
  <tabstop>ref_by_relation_CB</tabstop>
  <tabstop>ref_relation_CB</tabstop>
  <tabstop>ref_link_source_CB</tabstop>
  <tabstop>source_tooltips_CB</tabstop>
  <tabstop>use_htags_CB</tabstop>
  <tabstop>verbatim_headers_CB</tabstop>
  <tabstop>clang_parsing_CB</tabstop>
  <tabstop>clang_options</tabstop>
  <tabstop>clang_options_PB</tabstop>

  // index
  <tabstop>alpha_index_CB</tabstop>
  <tabstop>cols_in_index_CM</tabstop>
  <tabstop>ignore_prefix</tabstop>
  <tabstop>ignore_prefix_PB</tabstop>

  // autogen
  <tabstop>gen_autogen_def_CB</tabstop>

  // perlmod
  <tabstop>gen_perl_CB</tabstop>
  <tabstop>perl_latex_CB</tabstop>
  <tabstop>perl_pretty_CB</tabstop>
  <tabstop>perlmod_prefix</tabstop>

  // preprocessor
  <tabstop>checkBox_608</tabstop>
  <tabstop>checkBox_609</tabstop>
  <tabstop>checkBox_607</tabstop>
  <tabstop>checkBox_610</tabstop>
  <tabstop>plainTextEdit_111</tabstop>
  <tabstop>source_input_PB_17</tabstop>
  <tabstop>plainTextEdit_112</tabstop>
  <tabstop>source_input_PB_18</tabstop>
  <tabstop>plainTextEdit_113</tabstop>
  <tabstop>source_input_PB_19</tabstop>
  <tabstop>plainTextEdit_114</tabstop>
  <tabstop>source_input_PB_20</tabstop>
  <tabstop>checkBox_611</tabstop>

  // external
  <tabstop>tag_files</tabstop>
  <tabstop>tag_files_PB</tabstop>
  <tabstop>generate_tagfile</tabstop>
  <tabstop>all_externals_CB</tabstop>
  <tabstop>external_groups_CB</tabstop>
  <tabstop>external_pages_CB</tabstop>
  <tabstop>perl_path</tabstop>
  <tabstop>perl_path_PB</tabstop>

  // dot
  <tabstop>checkBox_565</tabstop>
  <tabstop>destDir_118</tabstop>
  <tabstop>pushButton_35</tabstop>
  <tabstop>destDir_119</tabstop>
  <tabstop>pushButton_36</tabstop>
  <tabstop>checkBox_578</tabstop>
  <tabstop>checkBox_577</tabstop>
  <tabstop>spinBox_2</tabstop>
  <tabstop>destDir_30</tabstop>
  <tabstop>pushButton_2</tabstop>
  <tabstop>spinBox_3</tabstop>
  <tabstop>destDir_120</tabstop>
  <tabstop>pushButton_37</tabstop>
  <tabstop>checkBox_580</tabstop>
  <tabstop>checkBox_581</tabstop>
  <tabstop>checkBox_579</tabstop>
  <tabstop>checkBox_582</tabstop>
  <tabstop>spinBox_4</tabstop>
  <tabstop>checkBox_585</tabstop>
  <tabstop>checkBox_583</tabstop>
  <tabstop>checkBox_584</tabstop>
  <tabstop>checkBox_588</tabstop>
  <tabstop>checkBox_3</tabstop>
  <tabstop>checkBox_586</tabstop>
  <tabstop>checkBox_587</tabstop>
  <tabstop>comboBox_14</tabstop>
  <tabstop>checkBox_589</tabstop>
  <tabstop>destDir_121</tabstop>
  <tabstop>pushButton_41</tabstop>
  <tabstop>plainTextEdit_127</tabstop>
  <tabstop>pushButton_51</tabstop>
  <tabstop>plainTextEdit_128</tabstop>
  <tabstop>pushButton_50</tabstop>
  <tabstop>plainTextEdit_129</tabstop>
  <tabstop>pushButton_49</tabstop>
  <tabstop>destDir_180</tabstop>
  <tabstop>pushButton</tabstop>
  <tabstop>spinBox_14</tabstop>
  <tabstop>spinBox_15</tabstop>
  <tabstop>checkBox_690</tabstop>
  <tabstop>checkBox_688</tabstop>
  <tabstop>checkBox_689</tabstop>
  <tabstop>checkBox_687</tabstop>
  <tabstop>output_TreeWidget</tabstop>


  // tab 3 - html
  <tabstop>gen_html_CB2</tabstop>
  <tabstop>destDir_32</tabstop>
  <tabstop>pushButton_14</tabstop>
  <tabstop>destDir_35</tabstop>
  <tabstop>destDir_39</tabstop>
  <tabstop>pushButton_15</tabstop>
  <tabstop>destDir_40</tabstop>
  <tabstop>pushButton_16</tabstop>
  <tabstop>destDir_52</tabstop>
  <tabstop>pushButton_21</tabstop>
  <tabstop>plainTextEdit_50</tabstop>
  <tabstop>pushButton_34</tabstop>
  <tabstop>plainTextEdit_51</tabstop>
  <tabstop>pushButton_42</tabstop>
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
  <tabstop>pushButton_23</tabstop>
  <tabstop>destDir_60</tabstop>
  <tabstop>pushButton_24</tabstop>
  <tabstop>checkBox_625</tabstop>
  <tabstop>destDir_61</tabstop>
  <tabstop>checkBox_626</tabstop>
  <tabstop>checkBox_627</tabstop>
  <tabstop>checkBox_628</tabstop>
  <tabstop>destDir_62</tabstop>
  <tabstop>pushButton_25</tabstop>
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
  <tabstop>pushButton_43</tabstop>
  <tabstop>destDir_70</tabstop>
  <tabstop>checkBox_635</tabstop>
  <tabstop>checkBox_636</tabstop>
  <tabstop>checkBox_637</tabstop>
  <tabstop>destDir_76</tabstop>
  <tabstop>lineEdit</tabstop>
  <tabstop>destDir_77</tabstop>
  <tabstop>pushButton_44</tabstop>
  <tabstop>plainTextEdit_32</tabstop>
  <tabstop>pushButton_26</tabstop>

  // latex
  <tabstop>gen_latex_CB2</tabstop>
  <tabstop>destDir_33</tabstop>
  <tabstop>pushButton_29</tabstop>
  <tabstop>destDir_78</tabstop>
  <tabstop>pushButton_27</tabstop>
  <tabstop>destDir_79</tabstop>
  <tabstop>pushButton_28</tabstop>
  <tabstop>checkBox_638</tabstop>
  <tabstop>comboBox_2</tabstop>
  <tabstop>plainTextEdit_33</tabstop>
  <tabstop>pushButton_32</tabstop>
  <tabstop>destDir_80</tabstop>
  <tabstop>pushButton_30</tabstop>
  <tabstop>destDir_81</tabstop>
  <tabstop>pushButton_31</tabstop>
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
  <tabstop>rtf_output_PB</tabstop>
  <tabstop>compact_rtf_CB</tabstop>
  <tabstop>rtf_hyperlinks_CB</tabstop>
  <tabstop>rtf_stylesheet</tabstop>
  <tabstop>rtf_stylesheet_PB</tabstop>
  <tabstop>rtf_extension</tabstop>
  <tabstop>rtf_extension_PB</tabstop>

  // man
  <tabstop>gen_man_CB2</tabstop>
  <tabstop>man_output</tabstop>
  <tabstop>man_output_PB</tabstop>
  <tabstop>man_extension</tabstop>
  <tabstop>man_subdir</tabstop>
  <tabstop>man_links_CB</tabstop>
  <tabstop>gen_xml_CB2</tabstop>

  // xml
  <tabstop>xml_output</tabstop>
  <tabstop>xml_output_PB</tabstop>
  <tabstop>xml_program_llisting</tabstop>

  // docbook
  <tabstop>gen_docbook_CB2</tabstop>
  <tabstop>docbook_output</tabstop>
  <tabstop>docbook_output_PB</tabstop>
  <tabstop>docbook_program_listing_CB

*/
