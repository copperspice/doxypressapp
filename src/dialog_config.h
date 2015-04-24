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

#ifndef DIALOG_CONFIG_H
#define DIALOG_CONFIG_H

#include "ui_dialog_config.h"

#include <QDialog>
#include <QString>

class Dialog_Config : public QDialog
{     
   CS_OBJECT(Dialog_Config)

   public:
      Dialog_Config(QString fileName);
      ~Dialog_Config();

      QString get_newName();

   private:
      Ui::Dialog_Config *m_ui;

      CS_SLOT_1(Private, void createNew())
      CS_SLOT_2(createNew) 

      CS_SLOT_1(Private, void select())
      CS_SLOT_2(select) 

      CS_SLOT_1(Private, void rename())
      CS_SLOT_2(rename) 

      CS_SLOT_1(Private, void cancel())
      CS_SLOT_2(cancel) 
};

#endif
