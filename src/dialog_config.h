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

#ifndef DIALOG_CONFIG_H
#define DIALOG_CONFIG_H

#include <QDialog>
#include <QString>

#include "ui_dialog_config.h"

class Dialog_Config : public QDialog
{     
   CS_OBJECT(Dialog_Config)

   public:
      Dialog_Config(QString fileName);
      ~Dialog_Config();

      QString get_newName();

   private:
      Ui::Dialog_Config *m_ui;

      void createNew();
      void select();
      void rename();
      void cancel();
};

#endif
