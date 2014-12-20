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
#include <QPushButton>
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

      // BROOM
//    m_struct.autoLoad       = object.value("autoLoad").toBool();

      m_struct.pathPrior      = object.value("pathPrior").toString();

      // screen fonts
      m_struct.fontNormal     = json_SetFont(object.value("font-normal").toString());

      // colors
      m_struct.colorText      = json_SetColor(object.value("color-text").toString());


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

         case AUTOLOAD:
//BROOM     object.insert("autoLoad", m_struct.autoLoad);
            break;

         case CLOSE:
            object.insert("color-text",  json_GetRGB(m_struct.colorText) );
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

         case COLORS:
            object.insert("color-text",     json_GetRGB(m_struct.colorText) );            
            break;           

         case FONT:           
            {
               QString temp = m_struct.fontNormal.toString();
               object.insert("font-normal", temp);

               break;
            }

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
   m_jsonFname = homePath + "/.config/CS_Doxygen/config.json";
   
   return;

#elif defined(Q_OS_MAC)
   if (m_appPath.contains(".app/Contents/MacOS")) {
      QString homePath = QDir::homePath();      
      m_jsonFname = homePath + "/Library/CS_Doxygen/config.json";
     
      return;
   }
#endif         

   QString selectedFilter;
   QFileDialog::Options options;

   QMessageBox quest;
   quest.setWindowTitle(tr("CS Doxygen"));
   quest.setText(tr("CS Doxygen configuration file is missing.\n\n"
                    "Selet an option to (a) create the configuration file in the system default location, "
                    "(b) pick a folder location, or (c) select an existing CS Doxygen Configuration file.\n"));

   QPushButton *createDefault  = quest.addButton("Default Location",     QMessageBox::AcceptRole );
   QPushButton *createNew      = quest.addButton("Pick Folder Location", QMessageBox::AcceptRole );
   QPushButton *selectExist    = quest.addButton("Select Existing File", QMessageBox::AcceptRole );

   quest.setStandardButtons(QMessageBox::Cancel);
   quest.setDefaultButton(QMessageBox::Cancel);

   quest.exec();

   if (quest.clickedButton() == createDefault) {
      m_jsonFname = m_appPath + "/config.json";

#ifdef Q_OS_WIN
      QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
      m_jsonFname  = path + "/config.json";
#endif

   } else if (quest.clickedButton() == createNew) {
      QString fname = m_appPath + "/config.json";

      // force windows 7 and 8 to honor initial path
      options = QFileDialog::ForceInitialDir_Win7;

      m_jsonFname = QFileDialog::getSaveFileName(this, tr("Create new Configuration File"),
            fname, tr("Json Files (*.json)"), &selectedFilter, options);                  

   } else if (quest.clickedButton() == selectExist) {

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
   QJsonArray list;

   object.insert("pos-x",       400);
   object.insert("pos-y",       200);
   object.insert("size-width",  800);
   object.insert("size-height", 600);

   object.insert("autoLoad",    true);

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
 
   //
   value = QJsonValue(QString("Courier,12,-1,5,50,0,0,0,0,0"));
   object.insert("font-normal", value);

   value = QJsonValue(QString("0,0,0"));         // black
   object.insert("color-text", value);

   value = QJsonValue(QString("255,255,255"));   // white
   object.insert("color-back", value);

   
   list.append(0);
   list.append(true);
   list.append(0);
  
   // save the data
   QJsonDocument doc(object);
   QByteArray data = doc.toJson();

   bool ok = json_SaveFile(data);

   return ok;
}


// **
QString MainWindow::get_xxFile(QString title, QString fname, QString filter)
{
   QString selectedFilter;
   QFileDialog::Options options;

   // force windows 7 and 8 to honor initial path
   options = QFileDialog::ForceInitialDir_Win7;

   fname = m_appPath + "/" + fname;

   QString file = QFileDialog::getOpenFileName(this, "Select " + title,
         fname, filter, &selectedFilter, options);

   return file;
}

QFont MainWindow::json_SetFont(QString value)
{
   QFont temp;
   temp.fromString(value);

   return temp;
}

QColor MainWindow::json_SetColor(QString values)
{
   QStringList list = values.split(",");

   int red   = 255;
   int green = 255;
   int blue  = 255;

   if (list.count() > 2 ) {
      red   = list[0].toInt();
      green = list[1].toInt();
      blue  = list[2].toInt();
   }

   QColor color(red,green,blue);

   return color;
}

QString MainWindow::json_GetRGB(QColor color)
{
   int red   = color.red();
   int green = color.green();
   int blue  = color.blue();

   QStringList list;
   list.append(QString::number(red));
   list.append(QString::number(green));
   list.append(QString::number(blue));

   QString values = list.join(",");

   return values;
}

// **
void MainWindow::move_ConfigFile()
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
                  m_appPath + "/config.json", tr("Json Files (*.json)"), &selectedFilter, options);

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

void MainWindow::save_ConfigFile()
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




/* BROOM

   QVariant state        = m_settings.value(QString::fromAscii("main/state"),    QVariant::Invalid);
   QVariant wizState     = m_settings.value(QString::fromAscii("wizard/state"),  QVariant::Invalid);
   QVariant loadSettings = m_settings.value(QString::fromAscii("wizard/loadsettings"),  QVariant::Invalid);

   if (state     != QVariant::Invalid) {
      restoreState   (state.toByteArray());
   }

   if (wizState  != QVariant::Invalid) {
      m_wizard->restoreState(wizState.toByteArray());
   }

   if (loadSettings != QVariant::Invalid && loadSettings.toBool()) {


      QHashIterator<QString, Input *> i(m_options);

      while (i.hasNext()) {
         i.next();

         QVariant var = m_settings->value(SA("config/") + i.key());

         if (i.value()) {
            //printf("Loading key %s: type=%d value='%s'\n",qPrintable(i.key()),var.type(),qPrintable(var.toString()));
            i.value()->value() = var;
            i.value()->update();
         }
      }

   }

   for (int i = 0; i < RECENT_FILES_MAX; i++) {
      QString entry = m_settings.value(QString().sprintf("recent/config%d", i)).toString();
      if (!entry.isEmpty() && QFileInfo(entry).exists()) {
         addRecentFile(entry);
      }
   }
*/


