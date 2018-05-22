/*************************************************************************
 *
 * Copyright (C) 2014-2018 Barbara Geller & Ansel Sermersheim
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

