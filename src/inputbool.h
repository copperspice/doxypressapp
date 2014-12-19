/******************************************************************************
 *
 *
 *
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby
 * granted. No representations are made about the suitability of this software
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 */

#ifndef _INPUTBOOL_H
#define _INPUTBOOL_H

#include "input.h"
#include <QObject>

class QCheckBox;
class QGridLayout;
class QLabel;

class InputBool : public QObject, public Input
{
   CS_OBJECT(InputBool)

 public:
   InputBool(QGridLayout *layout, int &row, const QString &id,
             bool enabled, const QString &docs );

   // Input
   QVariant &value();
   void update();
   Kind kind() const {
      return Bool;
   }
   QString docs() const {
      return m_docs;
   }
   QString id() const {
      return m_id;
   }
   QString templateDocs() const {
      return m_tdocs;
   }
   void addDependency(Input *option) {
      m_dependencies += option;
   }
   void setEnabled(bool);
   void updateDependencies();
   void writeValue(QTextStream &t, QTextCodec *codec);
   void setTemplateDocs(const QString &docs) {
      m_tdocs = docs;
   }

 public :
   CS_SLOT_1(Public, void reset())
   CS_SLOT_2(reset)
   CS_SLOT_1(Public, void setValue(bool un_named_arg1))
   CS_SLOT_2(setValue)

 public:
   CS_SIGNAL_1(Public, void changed())
   CS_SIGNAL_2(changed)
   CS_SIGNAL_1(Public, void toggle(QString un_named_arg1, bool un_named_arg2))
   CS_SIGNAL_2(toggle, un_named_arg1, un_named_arg2)
   CS_SIGNAL_1(Public, void showHelp(Input *un_named_arg1))
   CS_SIGNAL_2(showHelp, un_named_arg1)

 private :
   CS_SLOT_1(Private, void help())
   CS_SLOT_2(help)

 private:
   void updateDefault();
   bool m_state;
   bool m_default;
   bool m_enabled;
   QVariant m_value;
   QCheckBox *m_cb;
   QString m_docs;
   QList<Input *> m_dependencies;
   QString m_id;
   QLabel *m_lab;
   QString m_tdocs;
};

#endif
