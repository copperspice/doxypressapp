/************************************************************************
*
* Copyright (c) 2014-2026 Barbara Geller & Ansel Sermersheim
*
* Copyright (c) 1997-2014 by Dimitri van Heesch
*
* DoxyPressApp is free software. You can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
* DoxyPressApp is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* Documents produced by DoxyPress are derivative works derived from the
* input used in their production; they are not affected by this license.
*
* https://www.gnu.org/licenses/
*
*************************************************************************/

#ifndef SYNTAX_H
#define SYNTAX_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextDocument>

class Syntax : public QSyntaxHighlighter
{
   CS_OBJECT(Syntax)

 public:
   Syntax(QTextDocument *document);
   ~Syntax();

   void processSyntax();

 protected:
   void highlightBlock(const QString &text) override;

 private:
   struct HighlightingRule
   {
      QRegularExpression pattern;
      QTextCharFormat format;
   };

   QVector<HighlightingRule> highlightingRules;
};

#endif
