/************************************************************************
*
* Copyright (C) 2014-2020 Barbara Geller & Ansel Sermersheim
* Copyright (C) 1997-2014 by Dimitri van Heesch
*
* DoxyPressApp is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* DoxyPressApp is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* Documents produced by DoxyPress are derivative works derived from the
* input used in their production; they are not affected by this license.
*
*************************************************************************/

#include <QApplication>
#include <QMessageBox>
#include <QString>

#include "doxy_build_info.h"
#include "mainwindow.h"

#include <stdlib.h>

static void convert(QString fromFile, QString toFile);
static void useage();

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   app.setOrganizationName("CS");
   app.setApplicationName("DoxyPressApp");

   // passed parameters
   QStringList argList;

   for (int k = 0; k < argc; ++k)   {
      QString value = QString::fromLatin1(argv[k]);
      argList.append(value);
   }

   if (argList.contains("--convert", Qt::CaseInsensitive)) {

      int index = argList.indexOf("--convert", Qt::CaseInsensitive);

      QString fromFile;
      QString toFile;

      if (index + 1 < argList.size()) {
         fromFile = argList[index+1];
      }

      if (index + 2 < argList.size()) {
         toFile   = argList[index+2];
      }

      convert(fromFile, toFile);
      exit(0);

   } else if (argList.contains("--help", Qt::CaseInsensitive) ||
              argList.contains("--version", Qt::CaseInsensitive)) {

      useage();
      exit(0);
   }

   try {
      MainWindow main;

      int index = argList.indexOf("--project", Qt::CaseInsensitive);

      if (index > 0 && index + 1 < argList.size()) {
         // name of project file as an argument
         main.openDoxy_Internal(argList[index+1]);
      }

      main.show();
      return app.exec();

   } catch (std::exception &e) {
      const QString what = QString::fromUtf8(e.what());

      if (what == "abort_no_message")  {
         // do nothing

      } else {
         QString errMsg = "Exception: " + what;

         QMessageBox msgB;
         msgB.setWindowTitle("DoxyPressApp / Issue");
         msgB.setIcon(QMessageBox::NoIcon);
         msgB.setMinimumWidth(100);

         msgB.setText(errMsg);
         msgB.exec();
      }
   }

   return 0;
}

static void useage()
{
   // console issues on windows, tell user to redirect

   printf("\n");
   printf("DoxyPressApp: Version %s\n", versionString);
   printf("email: info@copperspice.com\n");

   printf("\n");
   printf("Usage: DoxyPressApp [OPTIONS] [project file name]\n");

   printf("\n\n");
   printf("Convert Doxygen project file to DoxyPress format: \n");
   printf("   --convert  existing-doxygen-fileName  new-doxypress-fileName \n");
}

static void convert(QString fromFile, QString toFile)
{
   // console issues on windows, tell user to redirect
   bool ok = true;

   if (fromFile.isEmpty())  {
      printf("Existing doxygen project file was not supplied\n");
      ok = false;
   }

   if (toFile.isEmpty())   {
      printf("New DoxyPress project file was not not supplied\n");
      ok = false;
   }

   if (ok) {
      MainWindow main;
      main.autoConvert(fromFile, toFile);
   } else {
      printf("Convert aborted");
   }
}
