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

#ifndef EXPERT_H
#define EXPERT_H

#include <QtGui>
#include <QDomElement>
#include <QDomDocument>
#include <QHash>
#include <QSplitter>

#include "docintf.h"

class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
class QSettings;
class QTextBrowser;
class QTextCodec;
class QPushButton;
class Input;

class Expert : public QSplitter, public DocIntf
{
   CS_OBJECT(Expert)

 public:
   Expert();
   ~Expert();

   void saveSettings(QSettings *);
   void loadConfig(const QString &fileName);
   bool writeConfig(QTextStream &t, bool brief);
   QByteArray saveInnerState () const;
   bool restoreInnerState ( const QByteArray &state );
   const QHash<QString, Input *> &modelData() const {
      return m_options;
   }
   void resetToDefaults();
   bool htmlOutputPresent(const QString &workingDir) const;
   bool pdfOutputPresent(const QString &workingDir) const;
   QString getHtmlOutputIndex(const QString &workingDir) const;

   // DocIntf methods
   void setHeader(const char *name);
   void add(const char *name, const char *doc);

 public :
   CS_SLOT_1(Public, void activateTopic(QTreeWidgetItem *un_named_arg1, QTreeWidgetItem *un_named_arg2))
   CS_SLOT_2(activateTopic)
   CS_SLOT_1(Public, QWidget *createTopicWidget(QDomElement &elem))
   CS_SLOT_2(createTopicWidget)
   CS_SLOT_1(Public, void refresh())
   CS_SLOT_2(refresh)

 private :
   CS_SLOT_1(Private, void showHelp(Input *un_named_arg1))
   CS_SLOT_2(showHelp)
   CS_SLOT_1(Private, void nextTopic())
   CS_SLOT_2(nextTopic)
   CS_SLOT_1(Private, void prevTopic())
   CS_SLOT_2(prevTopic)

 public:
   CS_SIGNAL_1(Public, void changed())
   CS_SIGNAL_2(changed)
   CS_SIGNAL_1(Public, void done())
   CS_SIGNAL_2(done)

 private:
   void createTopics(const QDomElement &);
   void saveTopic(QTextStream &t, QDomElement &elem, QTextCodec *codec, bool brief);

   QSplitter               *m_splitter;
   QTextBrowser            *m_helper;
   QTreeWidget             *m_treeWidget;
   QStackedWidget          *m_topicStack;
   QHash<QString, QWidget *> m_topics;
   QHash<QString, QObject *> m_optionWidgets;
   QHash<QString, Input *>   m_options;
   QPushButton             *m_next;
   QPushButton             *m_prev;
   QDomElement              m_rootElement;
   bool                     m_inShowHelp;
   QString                  m_header;
};

#endif
