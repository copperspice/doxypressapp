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

#include "colors.h"

#include <math.h>

TuneColorDialog::TuneColorDialog(int hue, int sat, int gamma, QWidget *parent) : QDialog(parent)
{
   setWindowTitle(tr("Tune the colors for HTML output"));

   QGridLayout *layout = new QGridLayout(this);
   m_image    = new QImage(QString::fromAscii(":/resources/tunecolor.png"));
   m_imageLab = new QLabel;
   updateImage(hue, sat, gamma);

   layout->addWidget(m_imageLab,0,0);

   //
   QHBoxLayout *buttonsLayout = new QHBoxLayout;

   QPushButton *okButton = new QPushButton(tr("Ok"));
   connect(okButton, SIGNAL(clicked()), SLOT(accept()));
   okButton->setDefault(true);

   QPushButton *cancelButton = new QPushButton(tr("Cancel"));
   connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

   //
   ColorPicker *huePicker = new ColorPicker(ColorPicker::Hue);
   huePicker->setCol(hue, sat, gamma);
   huePicker->setFixedWidth(20);
   layout->addWidget(huePicker, 0, 1);

   ColorPicker *satPicker = new ColorPicker(ColorPicker::Saturation);
   satPicker->setCol(hue, sat, gamma);
   satPicker->setFixedWidth(20);
   layout->addWidget(satPicker, 0, 2);

   ColorPicker *gamPicker = new ColorPicker(ColorPicker::Gamma);
   gamPicker->setCol(hue, sat, gamma);
   gamPicker->setFixedWidth(20);
   layout->addWidget(gamPicker, 0, 3);

   QSpacerItem *spacer = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
   layout->addItem(spacer, 1, 0, 1, 4);

   buttonsLayout->addStretch();
   buttonsLayout->addWidget(okButton);
   buttonsLayout->addWidget(cancelButton);   
   buttonsLayout->addStretch();

   layout->addLayout(buttonsLayout, 2, 0, 1, 4);

   //
   connect(huePicker, SIGNAL(newHsv(int, int, int)), satPicker, SLOT(setCol(int, int, int)));
   connect(satPicker, SIGNAL(newHsv(int, int, int)), huePicker, SLOT(setCol(int, int, int)));
   connect(huePicker, SIGNAL(newHsv(int, int, int)), gamPicker, SLOT(setCol(int, int, int)));
   connect(satPicker, SIGNAL(newHsv(int, int, int)), gamPicker, SLOT(setCol(int, int, int)));
   connect(gamPicker, SIGNAL(newHsv(int, int, int)), satPicker, SLOT(setCol(int, int, int)));
   connect(gamPicker, SIGNAL(newHsv(int, int, int)), huePicker, SLOT(setCol(int, int, int)));
   connect(huePicker, SIGNAL(newHsv(int, int, int)), this, SLOT(updateImage(int, int, int)));
   connect(satPicker, SIGNAL(newHsv(int, int, int)), this, SLOT(updateImage(int, int, int)));
   connect(gamPicker, SIGNAL(newHsv(int, int, int)), this, SLOT(updateImage(int, int, int)));
}

void hsl2rgb(double h, double s, double l, double *pRed, double *pGreen, double *pBlue)
{
   double v;
   double r, g, b;

   r = l;   // default to gray
   g = l;
   b = l;
   v = (l <= 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
   if (v > 0) {
      double m;
      double sv;
      int sextant;
      double fract, vsf, mid1, mid2;

      m       = l + l - v;
      sv      = (v - m ) / v;
      h      *= 6.0;
      sextant = (int)h;
      fract   = h - sextant;
      vsf     = v * sv * fract;
      mid1    = m + vsf;
      mid2    = v - vsf;
      switch (sextant) {
         case 0:
            r = v;
            g = mid1;
            b = m;
            break;
         case 1:
            r = mid2;
            g = v;
            b = m;
            break;
         case 2:
            r = m;
            g = v;
            b = mid1;
            break;
         case 3:
            r = m;
            g = mid2;
            b = v;
            break;
         case 4:
            r = mid1;
            g = m;
            b = v;
            break;
         case 5:
            r = v;
            g = m;
            b = mid2;
            break;
      }
   }
   *pRed   = r;
   *pGreen = g;
   *pBlue  = b;
}

void TuneColorDialog::updateImage(int hue, int sat, int gam)
{
   QImage coloredImg(m_image->width(), m_image->height(), QImage::Format_RGB32);
   uint *srcPixel = (uint *)m_image->scanLine(0);
   uint *dstPixel = (uint *)coloredImg.scanLine(0);
   uint nrPixels = coloredImg.width() * coloredImg.height();

   for (uint i = 0; i < nrPixels; i++, srcPixel++, dstPixel++) {
      QColor c = QColor::fromRgb(*srcPixel);
      double r, g, b;
      hsl2rgb(hue / 359.0, sat / 255.0, pow(c.green() / 255.0, gam / 100.0), &r, &g, &b);
      *dstPixel = qRgb((int)(r * 255.0), (int)(g * 255.0), (int)(b * 255.0));
   }

   m_imageLab->setPixmap(QPixmap::fromImage(coloredImg));
   m_hue = hue;
   m_sat = sat;
   m_gam = gam;
}

int TuneColorDialog::getHue() const
{
   return m_hue;
}

int TuneColorDialog::getSaturation() const
{
   return m_sat;
}

int TuneColorDialog::getGamma() const
{
   return m_gam;
}

ColorPicker::ColorPicker(Mode m)
{
   m_hue = 220;
   m_gam = 100;
   m_sat = 100;
   m_mode = m;
   m_pix = 0;
}

ColorPicker::~ColorPicker()
{
   delete m_pix;
}

void ColorPicker::paintEvent(QPaintEvent *)
{
   int w = width() - 5;

   QRect r(0, foff, w, height() - 2 * foff);

   int wi = r.width() - 2;
   int hi = r.height() - 2;

   if (!m_pix || m_pix->height() != hi || m_pix->width() != wi) {
      delete m_pix;

      QImage img(wi, hi, QImage::Format_RGB32);

      int y;
      uint *pixel = (uint *) img.scanLine(0);
      for (y = 0; y < hi; y++) {
         const uint *end = pixel + wi;
         int yh = y2hue(y + coff);
         int ys = y2sat(y + coff);
         int yg = y2gam(y + coff);
         while (pixel < end) {
            QColor c;
            c.setHsv(yh, ys, (int)(255 * pow(0.7, yg / 100.0)));
            *pixel = c.rgb();
            ++pixel;
         }
      }
      m_pix = new QPixmap(QPixmap::fromImage(img));
   }

   QPainter p(this);
   p.drawPixmap(1, coff, *m_pix);
   const QPalette &g = palette();
   qDrawShadePanel(&p, r, g, true);
   p.setPen(g.foreground().color());
   p.setBrush(g.foreground());

   QPolygon a;

   int y = m_mode == Hue ?        hue2y(m_hue) :
           m_mode == Saturation ? sat2y(m_sat) :
           gam2y(m_gam);

   a.setPoints(3, w, y, w + 5, y + 5, w + 5, y - 5);
   p.eraseRect(w, 0, 5, height());
   p.drawPolygon(a);
}

void ColorPicker::mouseMoveEvent(QMouseEvent *m)
{
   if (m_mode == Hue) {
      setHue(y2hue(m->y()));

   } else if (m_mode == Saturation) {
      setSat(y2sat(m->y()));

   } else {
      setGam(y2gam(m->y()));
   }
}

void ColorPicker::mousePressEvent(QMouseEvent *m)
{
   if (m_mode == Hue) {
      setHue(y2hue(m->y()));

   } else if (m_mode == Saturation) {
      setSat(y2sat(m->y()));

   } else {
      setGam(y2gam(m->y()));
   }
}

void ColorPicker::setHue(int h)
{
   if (h == m_hue) {
      return;
   }
   m_hue = qMax(0, qMin(h, 359));
   delete m_pix;
   m_pix = 0;
   repaint();
   emit newHsv(m_hue, m_sat, m_gam);
}

void ColorPicker::setSat(int s)
{
   if (s == m_sat) {
      return;
   }
   m_sat = qMax(0, qMin(s, 255));
   delete m_pix;
   m_pix = 0;
   repaint();
   emit newHsv(m_hue, m_sat, m_gam);
}

void ColorPicker::setGam(int g)
{
   if (g == m_gam) {
      return;
   }
   m_gam = qMax(40, qMin(g, 240));
   delete m_pix;
   m_pix = 0;
   repaint();
   emit newHsv(m_hue, m_sat, m_gam);
}

void ColorPicker::setCol(int h, int s, int g)
{
   if (m_hue != h || m_sat != s || m_gam != g) {
      m_hue = h;
      m_sat = s;
      m_gam = g;
      delete m_pix;
      m_pix = 0;
      repaint();
   }
}

int ColorPicker::y2hue(int y)
{
   int d = height() - 2 * coff - 1;
   return m_mode == Hue ? (y - coff) * 359 / d : m_hue;
}

int ColorPicker::hue2y(int v)
{
   int d = height() - 2 * coff - 1;
   return coff + v * d / 359;
}

int ColorPicker::y2sat(int y)
{
   int d = height() - 2 * coff - 1;
   return m_mode == Saturation ? 255 - (y - coff) * 255 / d : m_sat;
}

int ColorPicker::sat2y(int v)
{
   int d = height() - 2 * coff - 1;
   return coff + (255 - v) * d / 255;
}

int ColorPicker::y2gam(int y)
{
   int d = height() - 2 * coff - 1;
   return m_mode == Gamma ? 240 - (y - coff) * 200 / d : m_gam;
}

int ColorPicker::gam2y(int g)
{
   int d = height() - 2 * coff - 1;
   return coff + (240 - g) * d / 200;
}
