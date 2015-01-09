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

#ifndef COLORS_H
#define COLORS_H

#include <QDialog>
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygon>
#include <QPushButton>
#include <QRect>
#include <QSpacerItem>
#include <QWidget>

enum OptLang     { Lang_Cpp, Lang_C, Lang_Java, Lang_CS };
enum HtmlStyle   { HS_Plain, HS_TreeView, HS_CHM };
enum TexStyle    { TS_PDFHyper, TS_PDF, TS_PS };
enum DiagramMode { DM_None, DM_Builtin, DM_Dot };

class TuneColorDialog : public QDialog
{
   CS_OBJECT(TuneColorDialog)

 public:
   TuneColorDialog(int hue, int sat, int gamma, QWidget *parent = 0);
   int getHue() const;
   int getSaturation() const;
   int getGamma() const;

 private :
   CS_SLOT_1(Private, void updateImage(int hue, int sat, int val))
   CS_SLOT_2(updateImage)

   QImage *m_image;
   QLabel *m_imageLab;
   int m_hue;
   int m_sat;
   int m_gam;
};

class ColorPicker : public QWidget
{
   CS_OBJECT(ColorPicker)

 public:
   enum Mode { Hue, Saturation, Gamma };

   ColorPicker(Mode m);
   ~ColorPicker();
 
   CS_SLOT_1(Public, void setCol(int h, int s, int g))
   CS_SLOT_2(setCol)

   CS_SIGNAL_1(Public, void newHsv(int h, int s, int g))
   CS_SIGNAL_2(newHsv, h, s, g)

 protected:
   void paintEvent(QPaintEvent *);
   void mouseMoveEvent(QMouseEvent *);
   void mousePressEvent(QMouseEvent *);

 private:
   enum { foff = 3, coff = 4 }; //frame and contents offset
   int y2hue(int y);
   int y2sat(int y);
   int y2gam(int y);
   int hue2y(int hue);
   int sat2y(int sat);
   int gam2y(int gamma);
   void setHue(int v);
   void setSat(int v);
   void setGam(int v);

   QPixmap *m_pix;
   Mode m_mode;
   int m_gam;
   int m_hue;
   int m_sat;

};

#endif
