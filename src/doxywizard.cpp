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

#include <QFile>

#ifdef WIN32
#include <windows.h>
#endif

void MainWindow::clearAllFields()
{
   // BROOM - add more

   m_ui->project_name->setText("");
   m_ui->project_brief->setText("");
   m_ui->project_number->setText("");

   m_project_iconFN = "";

   m_ui->source_input->setText("");
   m_ui->source_output->setText("");

   m_ui->source_recursive_CB->setChecked(false);

   m_ui->genHtml_CB->setChecked(false);
   m_ui->genLatex_CB->setChecked(false);
   m_ui->genRtf_CB->setChecked(false);
   m_ui->genMan_CB->setChecked(false);
   m_ui->genXml_CB->setChecked(false);
   m_ui->genDocbook_CB->setChecked(false);

}

void MainWindow::convertDoxy(QByteArray data)
{
   QString tempStr;
   bool tempBool;
   int tempInt;

   int posBeg;
   int posEnd;

   posBeg = data.indexOf("PROJECT_NAME");
   if (posBeg > 0) {
      posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1);

      tempStr.replace("\"","");
      tempStr = tempStr.trimmed();

      m_ui->project_name->setText(tempStr);
   }

   posBeg = data.indexOf("PROJECT_BRIEF");
   if (posBeg > 0) {
      posEnd = data.indexOf("\n", posBeg);      
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1);

      tempStr.replace("\"","");
      tempStr = tempStr.trimmed();

      m_ui->project_brief->setText(tempStr);
   }

   tempStr = convert_Str(data,"PROJECT_NUMBER");
   m_ui->project_number->setText(tempStr);

   m_project_iconFN = convert_Str(data,"PROJECT_LOGO");

   tempStr = convert_Str(data,"INPUT");
   m_ui->source_input->setText(tempStr);

   tempStr = convert_Str(data,"OUTPUT_DIRECTORY");
   m_ui->source_output->setText(tempStr);

   tempBool = convert_Bool(data,"RECURSIVE");
   m_ui->source_recursive_CB->setChecked(tempBool);



/*
   m_ui->genHtml_CB->setChecked(false);
   m_ui->genLatex_CB->setChecked(false);
   m_ui->genRtf_CB->setChecked(false);
   m_ui->genMan_CB->setChecked(false);
   m_ui->genXml_CB->setChecked(false);
   m_ui->genDocbook_CB->setChecked(false);



DOXYFILE_ENCODING
OUTPUT_DIRECTORY
CREATE_SUBDIRS
ALLOW_UNICODE_NAMES
OUTPUT_LANGUAGE
BRIEF_MEMBER_DESC
REPEAT_BRIEF
ABBREVIATE_BRIEF
ALWAYS_DETAILED_SEC
INLINE_INHERITED_MEMB
FULL_PATH_NAMES
STRIP_FROM_PATH
STRIP_FROM_INC_PATH
SHORT_NAMES
JAVADOC_AUTOBRIEF
QT_AUTOBRIEF
MULTILINE_CPP_IS_BRIEF
INHERIT_DOCS
SEPARATE_MEMBER_PAGES
TAB_SIZE
ALIASES
TCL_SUBST
OPTIMIZE_OUTPUT_FOR_C
OPTIMIZE_OUTPUT_JAVA
OPTIMIZE_FOR_FORTRAN
OPTIMIZE_OUTPUT_VHDL
EXTENSION_MAPPING
MARKDOWN_SUPPORT
AUTOLINK_SUPPORT
BUILTIN_STL_SUPPORT
CPP_CLI_SUPPORT
SIP_SUPPORT
IDL_PROPERTY_SUPPORT
DISTRIBUTE_GROUP_DOC
SUBGROUPING
INLINE_GROUPED_CLASSES
INLINE_SIMPLE_STRUCTS
TYPEDEF_HIDES_STRUCT
LOOKUP_CACHE_SIZE
EXTRACT_ALL
EXTRACT_PRIVATE
EXTRACT_PACKAGE
EXTRACT_STATIC
EXTRACT_LOCAL_CLASSES
EXTRACT_LOCAL_METHODS
EXTRACT_ANON_NSPACES
HIDE_UNDOC_MEMBERS
HIDE_UNDOC_CLASSES
HIDE_FRIEND_COMPOUNDS
HIDE_IN_BODY_DOCS
INTERNAL_DOCS




*/

}

bool MainWindow::convert_Bool(QByteArray data, QString key)
{
   bool retval = false;
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();

      if (tempStr == "YES") {
         retval = true;
      }
   }

   return retval;
}

int MainWindow::convert_Int(QByteArray data, QString key)
{
   int retval;
   QString temp;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      temp = data.mid(posBeg, posEnd - posBeg);

      retval = temp.toInt();
   }

   return retval;
}

QString MainWindow::convert_Str(QByteArray data, QString key)
{
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();
   }

   return tempStr;
}

void MainWindow::importDoxy()
{
   QMessageBox quest;
   quest.setWindowTitle(tr("CS Doxygen Import"));
   quest.setText( tr("Convert Doxygen configuration file to CS Doxygen format"));
   quest.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   quest.setDefaultButton(QMessageBox::No);

   int retval = quest.exec();

   if (retval == QMessageBox::Yes) {

      while (true) {

         QString fname = QFileDialog::getOpenFileName(this, tr("Open (old) Doxygen Config File"), m_struct.pathPrior);

         if (fname.isEmpty()) {
            csError(tr("File Open"), tr("No file name was provided"));
            break;
         }

         QFile file(fname);
         if (! file.open(QIODevice::ReadOnly)) {
            csError(tr("Error Opening: ") + fname, tr("Unable to open: ") + file.error());
            break;
         }

         QByteArray data;

         data = file.readAll();
         file.close();

         // strip comments
         int posBeg;
         int posEnd;

         while (true) {
            posBeg = data.indexOf("#");

            if (posBeg == -1) {
               break;
            }

            posEnd = data.indexOf("\n",posBeg);
            data.remove(posBeg, posEnd-posBeg);
         }

/*
         // *** get the file - internal usage
         QString path = "z:\\doxygen_wizard\\deploy";

         QFile fileX(path + "/dox_fields.txt");

         if (! fileX.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, tr("Error Saving: ") + m_ConfigFile, tr("Unable to save: ") + file.error());
            break;
         }

         fileX.write(data);
         fileX.close();

         csMsg("File Saved : " + path + "/dox_fields.txt");
         break;
*/

         // verify a few fields
         if (! data.contains("PROJECT_NAME") || ! data.contains("OUTPUT_DIRECTORY"))  {
            csError(tr("CS Doxygen Import"), tr("The configuration file is missing project information, import canceled"));
            break;
         }

         // get new file config name
         fname = QFileDialog::getSaveFileName(this, tr("Select name for CS Doxygen Config"), m_struct.pathPrior);

         if (fname.isEmpty()) {
            // are you sure? might want to loop around
            csError(tr("CS Doxygen Import"), tr("No configuraton file name, import canceled"));
            break;

         } else {
            m_ConfigFile = fname;

            clearAllFields();
            convertDoxy(data);

            saveDox_Internal();

            json_Write(PATH_PRIOR);
//          if (! m_struct.XXX.contains(m_ConfigFile)) {
//            addRecentFile(m_ConfigFile);
//          }

         }

         // all done, everything is fine
         break;
      }
   }
}

void MainWindow::newDoxy()
{
   csMsg("Create New Doxy file - pending");
}

void MainWindow::openDoxy()
{
   if (querySave()) {
      QString fname = QFileDialog::getOpenFileName(this, tr("Open CS Doxygen Config"), m_struct.pathPrior);

      if (! fname.isEmpty()) {
         openDoxy_Internal(fname);
      }
   }
}

void MainWindow::openDoxy_Internal(const QString fname)
{
   m_ConfigFile = fname;

   QFile file(m_ConfigFile);

   if (! file.open(QIODevice::ReadOnly)) {
      QMessageBox::warning(this, tr("Error Opening: ") + m_ConfigFile, tr("Unable to open: ") + file.error());
      return;
   }

   QByteArray data;

   data = file.readAll();
   file.close();

   json_Write(PATH_PRIOR);
//   if (! m_struct.XXX.contains(m_ConfigFile)) {
//      addRecentFile(m_ConfigFile);
//   }

   json_OpenDoxy(data);

   initTabs();
   updateLaunchButtonState();

   setDoxygenTitle(false);
}

void MainWindow::reloadDoxy()
{
   clearAllFields();

   if (! m_ConfigFile.isEmpty()) {
      openDoxy_Internal(m_ConfigFile);
   }
}

void MainWindow::saveDox_Internal()
{
   QByteArray data = json_SaveDoxy();

   QFile file(m_ConfigFile);

   if (! file.open(QIODevice::WriteOnly)) {
      QMessageBox::warning(this, tr("Error Saving: ") + m_ConfigFile, tr("Unable to save: ") + file.error());
      return;
   }

   file.write(data);
   file.close();

   setDoxygenTitle(false);
}

bool MainWindow::saveDoxyAs()
{    
   m_ConfigFile = QFileDialog::getSaveFileName(this, tr("Select name for CS Doxygen Config"), m_struct.pathPrior);

   if (m_ConfigFile.isEmpty()) {     
      return false;

   } else {
      saveDox_Internal();

      json_Write(PATH_PRIOR);
 //   if (! m_struct.XXX.contains(m_ConfigFile)) {
 //      addRecentFile(m_ConfigFile);
 //   }

   }

   return true;
}

void MainWindow::saveDoxy()
{
   if (m_ConfigFile.isEmpty()) {
      saveDoxyAs();

   } else {
      saveDox_Internal();
   }
}

void MainWindow::saveSettings()
{
  json_Write(CLOSE);
}

// setup

void MainWindow::initTabs()
{
   icon_PB("load");




/*
   if (getBoolOption(m_modelData, STR_HAVE_DOT)) {
      // Dot
      m_diagramModeGroup->button(2)->setChecked(true);

   } else if (getBoolOption(m_modelData, STR_CLASS_DIAGRAMS)) {
      // Builtin diagrams
      m_diagramModeGroup->button(1)->setChecked(true);

   } else {
      // no diagrams
      m_diagramModeGroup->button(0)->setChecked(true);
   }

   m_dotClass->setChecked(getBoolOption(m_modelData, STR_CLASS_GRAPH));
   m_dotCollaboration->setChecked(getBoolOption(m_modelData, STR_COLLABORATION_GRAPH));
   m_dotInheritance->setChecked(getBoolOption(m_modelData, STR_GRAPHICAL_HIERARCHY));
   m_dotInclude->setChecked(getBoolOption(m_modelData, STR_INCLUDE_GRAPH));
   m_dotIncludedBy->setChecked(getBoolOption(m_modelData, STR_INCLUDED_BY_GRAPH));
   m_dotCall->setChecked(getBoolOption(m_modelData, STR_CALL_GRAPH));
   m_dotCaller->setChecked(getBoolOption(m_modelData, STR_CALLER_GRAPH));

*/
}


// ** Run Options
void MainWindow::runDoxygen()
{
   if (!m_running) {
      QString doxygenPath;

#if defined(Q_OS_MACX)
      doxygenPath = qApp->applicationDirPath() + QString::fromAscii("/../Resources/");
      qDebug() << tr("CS Doxygen path: ") << doxygenPath;

      if ( !QFile(doxygenPath + QString::fromAscii("doxygen")).exists() ) {

         // No doygen binary in the resources, if there is a system doxygen binary, use that instead         
         if ( QFile(QString::fromAscii("/usr/local/bin/cs_doxygen")).exists() ) {
            doxygenPath = QString::fromAscii("/usr/local/bin/");

         } else {
            qDebug() << tr("Unable to find the doxygen command, make sure it is in your $$PATH");
            doxygenPath = QString::fromAscii("");
         }
      }
      qDebug() << tr("Getting doxygen from: ") << doxygenPath;
#endif

      QString destDir = m_ui->source_output->text();

      m_runProcess->setReadChannel(QProcess::StandardOutput);
      m_runProcess->setProcessChannelMode(QProcess::MergedChannels);
      m_runProcess->setWorkingDirectory(destDir);

      QStringList env = QProcess::systemEnvironment();

      // set PWD environment variable
      env.replaceInStrings(QRegExp(QString::fromAscii("^PWD=(.*)"), Qt::CaseInsensitive),
                           QString::fromAscii("PWD=") + destDir);

      m_runProcess->setEnvironment(env);

      QStringList args;
      args << QString::fromAscii("-b"); // make stdout unbuffered
      args << QString::fromAscii("-");  // read config from stdin

      m_outputLog->clear();
      m_runProcess->start(doxygenPath + QString::fromAscii("doxygen"), args);

      if (!m_runProcess->waitForStarted()) {
         m_outputLog->append(QString::fromAscii("*** Failed to run doxygen\n"));
         return;
      }

      QTextStream t(m_runProcess);
      m_expert->writeConfig(t, false);
      m_runProcess->closeWriteChannel();

      if (m_runProcess->state() == QProcess::NotRunning) {
         m_outputLog->append("*** Failed to run CS Doxygen\n");

      } else {
         m_saveLog->setEnabled(false);

         m_running = true;

         m_ui->run_PB->setText(tr("Stop CS Doxygen"));
         m_ui->runStatus->setText(tr("CS Doxygen: Running"));

         m_timer->start(1000);
      }

   } else {
      m_running = false;      

      m_ui->run_PB->setText(tr("Run CS Doxygen"));
      m_ui->runStatus->setText(tr("CS Doxygen is Idle"));

      m_runProcess->kill();
      m_timer->stop();      
   }
}

void MainWindow::readStdout()
{
   if (m_running) {      
      QByteArray data = m_runProcess->readAllStandardOutput();
      QString text = QString::fromUtf8(data);

      if (! text.isEmpty()) {
         m_outputLog->append(text.trimmed());
      }
   }
}

void MainWindow::runComplete()
{
   if (m_running) {
      m_outputLog->append(tr("*** CS Doxygen has completed\n"));
   } else {
      m_outputLog->append(tr("*** CS Doxygen was cancelled\n"));
   }

   m_outputLog->ensureCursorVisible();

   m_run->setText(tr("Run CS Doxygen"));
   m_runStatus->setText(tr("Status: CS Doxygen is not running"));

   m_running = false;

   updateLaunchButtonState();

   m_saveLog->setEnabled(true);
}


void MainWindow::updateLaunchButtonState()
{        
   bool ok = false;           //  BROOM m_expert->htmlOutputPresent(m_ui->output->text());
   m_ui->display_PB->setEnabled(ok);
}

bool MainWindow::htmlOutputPresent(const QString &workingDir) const
{      
   bool retval = false;

/*
   bool generateHtml = getBoolOption(m_options, QString::fromAscii("GENERATE_HTML"));

   if (! generateHtml || workingDir.isEmpty()) {
      return retval;
   }

   QString indexFile = getHtmlOutputIndex(workingDir);
   QFileInfo fi(indexFile);

   retval = (fi.exists() && fi.isFile());

*/

   return retval;
}

void MainWindow::showHtmlOutput()
{
   QString indexFile = m_expert->getHtmlOutputIndex(m_ui->source_output->text());
   QFileInfo fi(indexFile);

   // TODO: the following does not seem to work with IE

#ifdef WIN32   
   ShellExecute(NULL, L"open", (LPCWSTR)fi.absoluteFilePath().utf16(), NULL, NULL, SW_SHOWNORMAL);

#else
   QString indexUrl(QString::fromAscii("file://"));
   indexUrl += fi.absoluteFilePath();
   QDesktopServices::openUrl(QUrl(indexUrl));

#endif

}

void MainWindow::saveLog()
{
   QString fn = QFileDialog::getSaveFileName(this, tr("Save log file"),
                                             m_ui->source_output->text() + QString::fromAscii("/doxy_log.txt"));

   if (! fn.isEmpty()) {
      QFile f(fn);

      if (f.open(QIODevice::WriteOnly)) {
         QTextStream t(&f);
         t << m_outputLog->toPlainText();

         statusBar()->showMessage(tr("Output log saved"));

      } else {
         QMessageBox::warning(this, tr("Error Saving: ") + fn, tr("Unable to save file: ") + f.error());

      }
   }
}

void MainWindow::showSettings()
{
   QString text;
   QTextStream t(&text);

   m_expert->writeConfig(t, true);
   m_outputLog->clear();
   m_outputLog->append(text);
   m_outputLog->ensureCursorVisible();
   m_saveLog->setEnabled(true);
}


