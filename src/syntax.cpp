/*************************************************************************
 *
 * Copyright (C) 2014-2017 Barbara Geller & Ansel Sermersheim
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

   HighlightingRule rule;

   for (auto pattern : keyWords) {
      rule.format.setFontWeight(QFont::Bold);
      rule.format.setFontItalic(false);
      rule.format.setForeground(Qt::blue);

      rule.pattern = QRegExp(pattern);
      rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);

      highlightingRules.append(rule);
   }

   for (auto pattern : errorWords) {
      rule.format.setFontWeight(QFont::Bold);
      rule.format.setFontItalic(false);
      rule.format.setForeground(Qt::red);

      rule.pattern = QRegExp(pattern);
      rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);

      highlightingRules.append(rule);
   }

   // redo the current document
   rehighlight();
}

void Syntax::highlightBlock(const QString &text)
{
   for (auto &rule : highlightingRules) {
      QRegExp expression(rule.pattern);
      int index = expression.indexIn(text);

      while (index >= 0) {
         int length = expression.matchedLength();
         setFormat(index, length, rule.format);
         index = expression.indexIn(text, index + length);
      }
   }

}

