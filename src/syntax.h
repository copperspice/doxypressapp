/************************************************************************
*
* Copyright (c) 2014-2021 Barbara Geller & Ansel Sermersheim
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

#ifndef SYNTAX_H
#define SYNTAX_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>

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
         QRegularExpression pattern;
         QTextCharFormat format;
      };

      QVector<HighlightingRule> highlightingRules;
};

#endif
