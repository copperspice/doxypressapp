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

#include <qdrawutil.h>

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
      void updateImage(int hue, int sat, int val);

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

   void setCol(int h, int s, int g);

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
