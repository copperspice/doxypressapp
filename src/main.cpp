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

#include <QApplication>
#include <QMessageBox>
#include <QString>

#include "doxywizard.h"

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
      msgBox.setText(QString().sprintf("Usage: %s [config file]", argv[0]));
      msgBox.exec();

      exit(0);
   }

   MainWindow main;

   if (flagList.contains("--config", Qt::CaseInsensitive)) {
      // name of config file as an argument
//    main.loadConfigFromFile(QString::fromLocal8Bit(argv[1]));
   }

   main.show();
   return app.exec();
}
