/************************************************************************
*
* Copyright (c) 2014-2023 Barbara Geller & Ansel Sermersheim
* Copyright (c) 1997-2014 by Dimitri van Heesch
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

#include "util.h"

#include <QApplication>
#include <QMessageBox>
#include <QString>

void csMsg(const QString &msg)
{
   QMessageBox msgB;

   msgB.setWindowTitle(QApplication::translate("csMsg", "Debug Information"));
   msgB.setWindowIcon(QIcon("://resources/doxypress.png"));

   msgB.setText(msg);
   msgB.exec();
}

void csMsg(QString msg, int value)
{
   msg = msg + "   " + QString::number(value);

   QMessageBox msgB;

   msgB.setWindowTitle(QApplication::translate("csMsg", "Debug Information"));
   msgB.setWindowIcon(QIcon("://resources/doxypress.png"));

   msgB.setText(msg);
   msgB.exec();
}

void csMsg(QWidget *parent, const QString &title, const QString &msg)
{
   QMessageBox msgB;

   if (parent) {
      msgB.setWindowModality(Qt::WindowModal);
   }

   msgB.setWindowTitle(title);
   msgB.setWindowIcon(QIcon("://resources/doxypress.png"));

   msgB.setText(msg);
   msgB.exec();
}

void csError(QString title, QString msg)
{
   QMessageBox msgB;

   msgB.setWindowTitle(title);
   msgB.setWindowIcon(QIcon("://resources/doxypress.png"));

   msgB.setIcon(QMessageBox::Warning);
   msgB.setText(msg);
   msgB.exec();
}

void showDialog(QDialog &t_Dialog)
{
   t_Dialog.show();
   t_Dialog.raise();
   t_Dialog.activateWindow();

   QApplication::processEvents();
}

