/*************************************************************************
 *
 * Copyright (C) 2014-2016 Barbara Geller & Ansel Sermersheim
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

#ifndef SYNTAX_H
#define SYNTAX_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegExp>

class Syntax : public QSyntaxHighlighter
{
   CS_OBJECT(Syntax)

   public:
      Syntax(QTextDocument *document);
      ~Syntax();

      void processSyntax();

   protected:
      void highlightBlock(const QString &text);

   private:
      struct HighlightingRule
      {
         QRegExp pattern;
         QTextCharFormat format;
      };

      QVector<HighlightingRule> highlightingRules;
};

#endif
