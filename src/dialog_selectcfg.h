

#ifndef DIALOG_SELECTCFG_H
#define DIALOG_SELECTCFG_H

#include <QDialog>

#include "mainwindow.h"
#include "ui_dialog_selectcfg.h"

class Dialog_SelectCfg : public QDialog
{
   CS_OBJECT(Dialog_SelectCfg)

   public:
      explicit Dialog_SelectCfg(MainWindow *parent);
      ~Dialog_SelectCfg();

      enum Result {
         SysDefault,
         Pick,
         Existing,
         Cancel
      };

   private:
      Ui::Dialog_SelectCfg *m_ui;

      CS_SLOT_1(Private, void sysDefault())
      CS_SLOT_2(sysDefault)

      CS_SLOT_1(Private, void pick())
      CS_SLOT_2(pick)

      CS_SLOT_1(Private, void existing())
      CS_SLOT_2(existing)

      CS_SLOT_1(Private, void cancel())
      CS_SLOT_2(cancel)
};

#endif
