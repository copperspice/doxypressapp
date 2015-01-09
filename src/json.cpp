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
   QJsonDocument doc = QJsonDocument::fromJson(data);

   QJsonObject object = doc.object();   
   QJsonArray list;   

   QString temp;
   QStringList dataList;
   int cnt;

   // tab 1
   m_ui->project_name->setText( object.value("project-name").toString());
   m_ui->project_brief->setText( object.value("project-brief").toString());
   m_ui->project_number->setText( object.value("project-number").toString());

   m_project_iconFN = object.value("project-icon").toString();
   m_ui->output_dir->setText( object.value("output-dir").toString());

   m_ui->gen_html_CB1->setChecked( object.value("generate-html").toBool());
   m_ui->gen_latex_CB1->setChecked( object.value("generate-latex").toBool());
   m_ui->gen_rtf_CB1->setChecked( object.value("generate-rtf").toBool());
   m_ui->gen_man_CB1->setChecked( object.value("generate-man").toBool());
   m_ui->gen_xml_CB1->setChecked( object.value("generate-xml").toBool());
   m_ui->gen_docbook_CB1->setChecked( object.value("generate-docbook").toBool());

   m_ui->gen_html_CB2->setChecked(m_ui->gen_html_CB1->isChecked());
   m_ui->gen_latex_CB2->setChecked(m_ui->gen_latex_CB1->isChecked());
   m_ui->gen_rtf_CB2->setChecked(m_ui->gen_rtf_CB1->isChecked());
   m_ui->gen_man_CB2->setChecked(m_ui->gen_man_CB1->isChecked());
   m_ui->gen_xml_CB2->setChecked(m_ui->gen_xml_CB1->isChecked());
   m_ui->gen_docbook_CB2->setChecked(m_ui->gen_docbook_CB1->isChecked());

//   m_ui->html_RB->setChecked( object.value("html-rb1").toBool());
//   m_ui->html_RB->setChecked( object.value("html-rb2").toBool());
//   m_ui->html_RB->setChecked( object.value("html-rb3").toBool());

   m_ui->searchEnabled_CB->setChecked( object.value("search-enabled").toBool());
   m_ui->html_colorstyle_hue->setValue( object.value("html-colorstyle-hue").toInt());
   m_ui->html_colorstyle_sat->setValue( object.value("html-colorstyle-sat").toInt());
   m_ui->html_colorstyle_gamma->setValue( object.value("html-colorstyle-gamma").toInt());

/*
   m_ui->latex_RB1->setChecked( object.value("latex-rb1").toBool());
   m_ui->latex_RB2->setChecked( object.value("latex-rb2").toBool());
   m_ui->latex_RB3->setChecked( object.value("latex-rb3").toBool());

   m_ui->diagram_RB1->setChecked( object.value("diagram-rb1").toBool());
   m_ui->diagram_RB2->setChecked( object.value("diagram-rb2").toBool());
   m_ui->diagram_RB3->setChecked( object.value("diagram-rb3").toBool());
*/

   m_ui->dot_class_CB->setChecked( object.value("dot-class").toBool());
   m_ui->dot_collaboration_CB->setChecked( object.value("dot-collaboration").toBool());
   m_ui->dot_overall_CB->setChecked( object.value("dot-overall").toBool());
   m_ui->dot_include_CB->setChecked( object.value("dot-include").toBool());
   m_ui->dot_included_by_CB->setChecked( object.value("dot-included-by").toBool());
   m_ui->dot_call_CB->setChecked( object.value("dot-call").toBool());
   m_ui->dot_called_by_CB->setChecked( object.value("dot-called-by").toBool());

   // tab 2
   m_ui->project_encoding->setText( object.value("project-encoding").toString());
   m_ui->source_recursive_CB->setChecked( object.value("source-recursive").toBool());
   m_ui->show_used_files_CB->setChecked( object.value("show-used-files").toBool());;
   m_ui->file_version_filter->setText( object.value("file-version-filter").toString());
   m_ui->layout_file->setText( object.value("layout-file").toString());
   m_ui->warn_logfile->setText( object.value("warn-logfile").toString());
   m_ui->warn_forrmat->setText( object.value("warn-forrmat").toString());
   m_ui->input_encoding->setText( object.value("input-encoding").toString());
   m_ui->mdfile_mainpage->setText( object.value("mdfile-mainpage").toString());
   m_ui->perlmod_prefix->setText( object.value("perlmod-prefix").toString());
   m_ui->generate_tagfile->setText( object.value("generate-tagfile").toString());
   m_ui->perl_path->setText( object.value("perl-path").toString());

   m_ui->exclude_symlinks_CB->setChecked( object.value("exclude-symlinks").toBool());

/*
   m_ui->file_patterns->setPlainText("");
   m_ui->exclude_files->setPlainText("");
   m_ui->exclude_patterns->setPlainText("");
   m_ui->exclude_symbols->setPlainText("");
*/

   //
   list = object.value("source-input").toArray();
   cnt  = list.count();

   for (int k = 0; k < cnt; k++)  {
      dataList.append(list.at(k).toString());
   }

   temp = dataList.join(", ");
   m_ui->input_source->setPlainText(temp);


   // tab 3 

}

QByteArray MainWindow::json_SaveDoxy()
{
   QJsonObject object;
   QJsonArray list;
// QJsonValue value;

   object.insert("project-name",       m_ui->project_name->text());
   object.insert("project-brief",      m_ui->project_brief->text());
   object.insert("project-number",     m_ui->project_number->text());

   object.insert("project-icon",       m_project_iconFN);  
   object.insert("output-dir",         m_ui->output_dir->text());

   object.insert("generate-html",      m_ui->gen_html_CB1->isChecked());
   object.insert("generate-latex",     m_ui->gen_latex_CB1->isChecked());
   object.insert("generate-rtf",       m_ui->gen_rtf_CB1->isChecked());
   object.insert("generate-man",       m_ui->gen_man_CB1->isChecked());
   object.insert("generate-xml",       m_ui->gen_xml_CB1->isChecked());
   object.insert("generate-docbook",   m_ui->gen_docbook_CB1->isChecked());

   object.insert("search-enabled",        m_ui->searchEnabled_CB->isChecked());
   object.insert("html-colorstyle-hue",   m_ui->html_colorstyle_hue->value());
   object.insert("html-colorstyle-sat",   m_ui->html_colorstyle_sat->value());
   object.insert("html-colorstyle-gamma", m_ui->html_colorstyle_gamma->value());

/*


   object.insert("html-rb1",           m_ui->html_RB1->isChecked());
   object.insert("html-rb2",           m_ui->html_RB2->isChecked());
   object.insert("html-rb3",           m_ui->html_RB3->isChecked());

   m_ui->latex_RB1->setChecked( object.value("latex-rb1").toBool());
   m_ui->latex_RB2->setChecked( object.value("latex-rb2").toBool());
   m_ui->latex_RB3->setChecked( object.value("latex-rb3").toBool());

   m_ui->diagram_RB1->setChecked( object.value("diagram-rb1").toBool());
   m_ui->diagram_RB2->setChecked( object.value("diagram-rb2").toBool());
   m_ui->diagram_RB3->setChecked( object.value("diagram-rb3").toBool());

   m_ui->dot_class_CB->setChecked( object.value("dot-class").toBool());
   m_ui->dot_collaboration_CB->setChecked( object.value("dot-collaboration").toBool());
   m_ui->dot_overall_CB->setChecked( object.value("dot-overall").toBool());
   m_ui->dot_include_CB->setChecked( object.value("dot-include").toBool());
   m_ui->dot_included_by_CB->setChecked( object.value("dot-included-by").toBool());
   m_ui->dot_call_CB->setChecked( object.value("dot-call").toBool());
   m_ui->dot_called_by_CB->setChecked( object.value("dot-called-by").toBool());

*/


   // tab 2
   object.insert("source-recursive",   m_ui->source_recursive_CB->isChecked());

   list = QJsonArray();
   QStringList temp = m_ui->input_source->toPlainText().split(", ");

   for (auto s : temp) {
      list.append(s);
   }

   object.insert("source-input",  list);


/*
   value = QJsonValue(QString("MM/dd/yyyy"));
   object.insert("formatDate", value);

   value = QJsonValue(m_appPath);
   object.insert("pathPrior", value);  
*/

   // save the data
   QJsonDocument doc(object);
   QByteArray data = doc.toJson();

   return data;
}

