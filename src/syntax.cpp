/************************************************************************
*
* Copyright (c) 2014-2025 Barbara Geller & Ansel Sermersheim
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

#include "syntax.h"

Syntax::Syntax(QTextDocument *document)
   : QSyntaxHighlighter(document)
{
}

Syntax::~Syntax()
{
}

void Syntax::processSyntax()
{
   QStringList keyWords;
   keyWords.append("^Parsing\\b");
   keyWords.append("^Processing\\b");
   keyWords.append("^Reading\\b");

   QStringList errorWords;
   errorWords.append("Error:");
   errorWords.append("Warning:");
   errorWords.append("^bibtex command failed:.*");
   errorWords.append("^.* is not recognized as an internal or external command");

   HighlightingRule rule;

   for (auto item : keyWords) {
      rule.format.setFontWeight(QFont::Bold);
      rule.format.setFontItalic(false);
      rule.format.setForeground(Qt::blue);

      rule.pattern = QRegularExpression(item, QPatternOption::CaseInsensitiveOption);
      highlightingRules.append(rule);
   }

   for (auto item : errorWords) {
      rule.format.setFontWeight(QFont::Bold);
      rule.format.setFontItalic(false);
      rule.format.setForeground(Qt::red);

      rule.pattern = QRegularExpression(item, QPatternOption::CaseInsensitiveOption);
      highlightingRules.append(rule);
   }

   // redo the current document
   rehighlight();
}

void Syntax::highlightBlock(const QString &text)
{
   for (auto &rule : highlightingRules) {
      QRegularExpressionMatch match = rule.pattern.match(text);

      while (match.hasMatch()) {
         int index  = match.capturedStart(0) - text.begin();
         int length = match.capturedLength();

         setFormat(index, length, rule.format);

         // get new match
         match = rule.pattern.match(text, match.capturedEnd(0));
      }
   }
}
