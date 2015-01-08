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

#include "mainwindow.h"

#include <QStringList>

void MainWindow::setupLimits()
{
   QString temp = "Afrikaans, Arabic, Armenian, Brazilian, Catalan, Chinese, "
         "Chinese-Traditional, Croatian, Czech, Danish, Dutch, English (United States), "
         "Esperanto, Farsi (Persian), Finnish, French, German, Greek, Hungarian, "
         "Indonesian, Italian, Japanese, Japanese-en (Japanese with English messages), "
         "Korean, Korean-en (Korean with English messages), Latvian, Lithuanian, "
         "Macedonian, Norwegian, Persian (Farsi), Polish, Portuguese, Romanian, Russian, "
         "Serbian, Serbian-Cyrillic, Slovak, Slovene, Spanish, Swedish, Turkish, Ukrainian, Vietnamese";

   QStringList lang = temp.split(", ");

   m_ui->output_langugae_CM->addItems(lang);
   m_ui->output_langugae_CM->setEditText("English");



   // 4
   m_ui->tab_size->setMinimum(1);
   m_ui->tab_size->setMaximum(16);
   // m_ui->tab_size->setValue(4);

   // 0
   // m_ui->look_up_cache->setMinimum(0);
   // m_ui->look_up_cache->setMaximum(9);

   // 30
   // m_ui->max_init_lines->setMinimum(0);
   // m_ui->max_init_lines->setMaximum(10000);


   // file patterns

   // cols in alph index

}
