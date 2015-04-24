/*************************************************************************
 *
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim
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

#include <QApplication>
#include <QMessageBox>
#include <QString>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
   // passed parameters
   QStringList flagList;

   for (int k = 0; k < argc; ++k)   {
      QString value = argv[k];

      if (value.left(2) == "--") {
         flagList.append(value);
      }
   }

   QApplication app(argc, argv);

   if (flagList.contains("--help", Qt::CaseInsensitive)) {
      QMessageBox msgBox;
      msgBox.setWindowIcon(QIcon("://resources/doxypress.png"));

      msgBox.setText(QString().sprintf("Usage: %s [config file]", argv[0]));
      msgBox.exec();

      exit(0);
   }

   try {
      MainWindow main;

      int index = flagList.indexOf("--config", Qt::CaseInsensitive);

      if (index > 0 && index + 1 < flagList.size()) {
         // name of config file as an argument
         main.openDoxy_Internal(flagList[index+1]);
      }

      main.show();
      return app.exec();

   } catch (std::exception &e) {

      const char *what = e.what();

      if (strcmp(what, "abort_no_message") == 0)  {
         // do nothing

      } else {
         QString errMsg = "Exception: " + QString(what);

         QMessageBox msgB;
         msgB.setWindowTitle("DoxyPressApp / Issue");
         msgB.setIcon(QMessageBox::NoIcon);
         msgB.setMinimumWidth(100);

         msgB.setText(errMsg);
         msgB.exec();
      }
   }
}

