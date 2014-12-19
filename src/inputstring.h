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

#ifndef _INPUTSTRING_H
#define _INPUTSTRING_H

#include "input.h"

#include <QObject>
#include <QMap>
#include <QStringList>

class QLabel;
class QLineEdit;
class QToolBar;
class QComboBox;
class QGridLayout;

class InputString : public QObject, public Input
{
   CS_OBJECT(InputString)

 public:
   enum StringMode { StringFree = 0,
                     StringFile = 1,
                     StringDir = 2,
                     StringFixed = 3,
                     StringImage = 4
                   };

   InputString( QGridLayout *layout, int &row,
                const QString &id, const QString &s,
                StringMode m,
                const QString &docs,
                const QString &absPath = QString() );
   ~InputString();
   void addValue(QString s);
   void setDefault();

   // Input
   QVariant &value();
   void update();
   Kind kind() const {
      return String;
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
   void addDependency(Input *) {
      Q_ASSERT(false);
   }
   void setEnabled(bool);
   void updateDependencies() {}
   void writeValue(QTextStream &t, QTextCodec *codec);
   void setTemplateDocs(const QString &docs) {
      m_tdocs = docs;
   }

 public :
   CS_SLOT_1(Public, void reset())
   CS_SLOT_2(reset)
   CS_SLOT_1(Public, void setValue(const QString &un_named_arg1))
   CS_SLOT_2(setValue)

 public:
   CS_SIGNAL_1(Public, void changed())
   CS_SIGNAL_2(changed)
   CS_SIGNAL_1(Public, void showHelp(Input *un_named_arg1))
   CS_SIGNAL_2(showHelp, un_named_arg1)

 private :
   CS_SLOT_1(Private, void browse())
   CS_SLOT_2(browse)
   CS_SLOT_1(Private, void clear())
   CS_SLOT_2(clear)
   CS_SLOT_1(Private, void help())
   CS_SLOT_2(help)

 private:
   void updateDefault();
   QLabel       *m_lab;
   QLineEdit    *m_le;
   QLabel       *m_im;
   QToolBar     *m_br;
   QComboBox    *m_com;
   QString       m_str;
   QString       m_default;
   StringMode    m_sm;
   QStringList   m_values;
   int           m_index;
   QVariant      m_value;
   QString       m_docs;
   QString       m_id;
   bool          m_absPath;
   QString       m_tdocs;
};

#endif
