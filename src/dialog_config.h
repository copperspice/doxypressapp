/**************************************************************************
*
* Copyright (c) 2012-2015 Barbara Geller
* All rights reserved.
*
* This file is part of Diamond Editor.
*
* Diamond Editor is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License version 3
* as published by the Free Software Foundation.
*
* Diamond Editor is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Diamond Editor.  If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

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
