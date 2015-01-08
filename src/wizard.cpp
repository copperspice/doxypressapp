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

#include "input.h"
#include "wizard.h"
#include "mainwindow.h"

#include <math.h>

void MainWindow::tuneColorDialog_PB()
{
   csMsg("display the tune color Dialog");

   //  TuneColorDialog tuneColor(0, 150, 200, this);


/*  BROOM

   int hue = getIntOption(m_modelData, STR_HTML_COLORSTYLE_HUE);
   int sat = getIntOption(m_modelData, STR_HTML_COLORSTYLE_SAT);
   int gam = getIntOption(m_modelData, STR_HTML_COLORSTYLE_GAMMA);

   TuneColorDialog tuneColor(hue, sat, gam, this);

   if (tuneColor.exec() == QDialog::Accepted) {
      updateIntOption(m_modelData, STR_HTML_COLORSTYLE_HUE, tuneColor.getHue());
      updateIntOption(m_modelData, STR_HTML_COLORSTYLE_SAT, tuneColor.getSaturation());
      updateIntOption(m_modelData, STR_HTML_COLORSTYLE_GAMMA, tuneColor.getGamma());
   }

*/


}



// options configurable via the wizard
#define STR_PROJECT_NAME          QString::fromAscii("PROJECT_NAME")
#define STR_PROJECT_LOGO          QString::fromAscii("PROJECT_LOGO")
#define STR_PROJECT_BRIEF         QString::fromAscii("PROJECT_BRIEF")
#define STR_INPUT                 QString::fromAscii("INPUT")
#define STR_OUTPUT_DIRECTORY      QString::fromAscii("OUTPUT_DIRECTORY")
#define STR_PROJECT_NUMBER        QString::fromAscii("PROJECT_NUMBER")
#define STR_RECURSIVE             QString::fromAscii("RECURSIVE")
#define STR_OPTIMIZE_OUTPUT_FOR_C QString::fromAscii("OPTIMIZE_OUTPUT_FOR_C")
#define STR_OPTIMIZE_OUTPUT_JAVA  QString::fromAscii("OPTIMIZE_OUTPUT_JAVA")
#define STR_OPTIMIZE_FOR_FORTRAN  QString::fromAscii("OPTIMIZE_FOR_FORTRAN")
#define STR_CPP_CLI_SUPPORT       QString::fromAscii("CPP_CLI_SUPPORT")
#define STR_HIDE_SCOPE_NAMES      QString::fromAscii("HIDE_SCOPE_NAMES")
#define STR_EXTRACT_ALL           QString::fromAscii("EXTRACT_ALL")
#define STR_SOURCE_BROWSER        QString::fromAscii("SOURCE_BROWSER")
#define STR_GENERATE_HTML         QString::fromAscii("GENERATE_HTML")
#define STR_GENERATE_LATEX        QString::fromAscii("GENERATE_LATEX")
#define STR_GENERATE_MAN          QString::fromAscii("GENERATE_MAN")
#define STR_GENERATE_RTF          QString::fromAscii("GENERATE_RTF")
#define STR_GENERATE_XML          QString::fromAscii("GENERATE_XML")
#define STR_GENERATE_HTMLHELP     QString::fromAscii("GENERATE_HTMLHELP")
#define STR_GENERATE_TREEVIEW     QString::fromAscii("GENERATE_TREEVIEW")
#define STR_USE_PDFLATEX          QString::fromAscii("USE_PDFLATEX")
#define STR_PDF_HYPERLINKS        QString::fromAscii("PDF_HYPERLINKS")
#define STR_SEARCHENGINE          QString::fromAscii("SEARCHENGINE")
#define STR_HAVE_DOT              QString::fromAscii("HAVE_DOT")
#define STR_CLASS_DIAGRAMS        QString::fromAscii("CLASS_DIAGRAMS")
#define STR_CLASS_GRAPH           QString::fromAscii("CLASS_GRAPH")
#define STR_COLLABORATION_GRAPH   QString::fromAscii("COLLABORATION_GRAPH")
#define STR_GRAPHICAL_HIERARCHY   QString::fromAscii("GRAPHICAL_HIERARCHY")
#define STR_INCLUDE_GRAPH         QString::fromAscii("INCLUDE_GRAPH")
#define STR_INCLUDED_BY_GRAPH     QString::fromAscii("INCLUDED_BY_GRAPH")
#define STR_CALL_GRAPH            QString::fromAscii("CALL_GRAPH")
#define STR_CALLER_GRAPH          QString::fromAscii("CALLER_GRAPH")
#define STR_HTML_COLORSTYLE_HUE   QString::fromAscii("HTML_COLORSTYLE_HUE")
#define STR_HTML_COLORSTYLE_SAT   QString::fromAscii("HTML_COLORSTYLE_SAT")
#define STR_HTML_COLORSTYLE_GAMMA QString::fromAscii("HTML_COLORSTYLE_GAMMA")

static bool g_optimizeMapping[6][6] = {
   // A: OPTIMIZE_OUTPUT_FOR_C
   // B: OPTIMIZE_OUTPUT_JAVA
   // C: OPTIMIZE_FOR_FORTRAN
   // D: OPTIMIZE_OUTPUT_VHDL
   // E: CPP_CLI_SUPPORT
   // F: HIDE_SCOPE_NAMES
   // A     B     C     D     E      F
   { false, false, false, false, false, false }, // 0: C++
   { false, false, false, false, true, false }, // 1: C++/CLI
   { false, true, false, false, false, false }, // 2: C#/Java
   { true, false, false, false, false, true  }, // 3: C/PHP
   { false, false, true, false, false, false }, // 4: Fortran
   { false, false, false, true, false, false }, // 5: VHDL
};

static QString g_optimizeOptionNames[6] = {
   STR_OPTIMIZE_OUTPUT_FOR_C,
   STR_OPTIMIZE_OUTPUT_JAVA,
   STR_OPTIMIZE_FOR_FORTRAN,  
   STR_CPP_CLI_SUPPORT,
   STR_HIDE_SCOPE_NAMES
};

static bool stringVariantToBool(const QVariant &v)
{
   QString s = v.toString().toLower();
   return s == QString::fromAscii("yes") || s == QString::fromAscii("true") || s == QString::fromAscii("1");
}

static bool getBoolOption(
   const QHash<QString, Input *> &model, const QString &name)
{
   Input *option = model[name];
   Q_ASSERT(option != 0);
   return stringVariantToBool(option->value());
}

static int getIntOption(
   const QHash<QString, Input *> &model, const QString &name)
{
   Input *option = model[name];
   Q_ASSERT(option != 0);
   return option->value().toInt();
}

static QString getStringOption(
   const QHash<QString, Input *> &model, const QString &name)
{
   Input *option = model[name];
   Q_ASSERT(option != 0);
   return option->value().toString();
}

static void updateBoolOption(
   const QHash<QString, Input *> &model, const QString &name, bool bNew)
{
   Input *option = model[name];
   Q_ASSERT(option != 0);
   bool bOld = stringVariantToBool(option->value());
   if (bOld != bNew) {
      option->value() = QString::fromAscii(bNew ? "true" : "false");
      option->update();
   }
}

static void updateIntOption(
   const QHash<QString, Input *> &model, const QString &name, int iNew)
{
   Input *option = model[name];
   Q_ASSERT(option != 0);
   int iOld = option->value().toInt();
   if (iOld != iNew) {
      option->value() = QString::fromAscii("%1").arg(iNew);
      option->update();
   }
}

static void updateStringOption(
   const QHash<QString, Input *> &model, const QString &name, const QString &s)
{
   Input *option = model[name];
   Q_ASSERT(option != 0);
   if (option->value().toString() != s) {
      option->value() = s;
      option->update();
   }
}

//==========================================================================

TuneColorDialog::TuneColorDialog(int hue, int sat, int gamma, QWidget *parent) : QDialog(parent)
{
   setWindowTitle(tr("Tune the color of the HTML output"));
   QGridLayout *layout = new QGridLayout(this);
   m_image = new QImage(QString::fromAscii(":/images/tunecolor.png"));
   m_imageLab = new QLabel;
   updateImage(hue, sat, gamma);
   layout->addWidget(new QLabel(tr("Example output: use the sliders on the right to adjust the color")), 0, 0);
   layout->addWidget(m_imageLab, 1, 0);
   QHBoxLayout *buttonsLayout = new QHBoxLayout;

   QPushButton *okButton = new QPushButton(tr("Ok"));
   connect(okButton, SIGNAL(clicked()), SLOT(accept()));
   okButton->setDefault(true);
   QPushButton *cancelButton = new QPushButton(tr("Cancel"));
   connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

   ColorPicker *huePicker = new ColorPicker(ColorPicker::Hue);
   huePicker->setCol(hue, sat, gamma);
   huePicker->setFixedWidth(20);
   layout->addWidget(huePicker, 1, 1);
   ColorPicker *satPicker = new ColorPicker(ColorPicker::Saturation);
   satPicker->setCol(hue, sat, gamma);
   satPicker->setFixedWidth(20);
   layout->addWidget(satPicker, 1, 2);
   ColorPicker *gamPicker = new ColorPicker(ColorPicker::Gamma);
   gamPicker->setCol(hue, sat, gamma);
   gamPicker->setFixedWidth(20);
   layout->addWidget(gamPicker, 1, 3);

   connect(huePicker, SIGNAL(newHsv(int, int, int)), satPicker, SLOT(setCol(int, int, int)));
   connect(satPicker, SIGNAL(newHsv(int, int, int)), huePicker, SLOT(setCol(int, int, int)));
   connect(huePicker, SIGNAL(newHsv(int, int, int)), gamPicker, SLOT(setCol(int, int, int)));
   connect(satPicker, SIGNAL(newHsv(int, int, int)), gamPicker, SLOT(setCol(int, int, int)));
   connect(gamPicker, SIGNAL(newHsv(int, int, int)), satPicker, SLOT(setCol(int, int, int)));
   connect(gamPicker, SIGNAL(newHsv(int, int, int)), huePicker, SLOT(setCol(int, int, int)));
   connect(huePicker, SIGNAL(newHsv(int, int, int)), this, SLOT(updateImage(int, int, int)));
   connect(satPicker, SIGNAL(newHsv(int, int, int)), this, SLOT(updateImage(int, int, int)));
   connect(gamPicker, SIGNAL(newHsv(int, int, int)), this, SLOT(updateImage(int, int, int)));  

   buttonsLayout->addStretch();
   buttonsLayout->addWidget(okButton);
   buttonsLayout->addWidget(cancelButton);
   layout->addLayout(buttonsLayout, 5, 0, 1, 4);
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

//==========================================================================

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

//==========================================================================


/*
   connect(m_projName,     SIGNAL(textChanged(const QString &)), SLOT(setProjectName(const QString &)));
   connect(m_projBrief,    SIGNAL(textChanged(const QString &)), SLOT(setProjectBrief(const QString &)));
   connect(m_projNumber,   SIGNAL(textChanged(const QString &)), SLOT(setProjectNumber(const QString &)));
   connect(m_sourceDir,    SIGNAL(textChanged(const QString &)), SLOT(setSourceDir(const QString &)));
   connect(m_recursive,    SIGNAL(stateChanged(int)),            SLOT(setRecursiveScan(int)));
   connect(m_destDir,      SIGNAL(textChanged(const QString &)), SLOT(setDestinationDir(const QString &)));


   connect(m_diagramModeGroup, SIGNAL(buttonClicked(int)),
           this, SLOT(diagramModeChanged(int)));
   connect(m_dotClass, SIGNAL(stateChanged(int)),
           this, SLOT(setClassGraphEnabled(int)));
   connect(m_dotCollaboration, SIGNAL(stateChanged(int)),
           this, SLOT(setCollaborationGraphEnabled(int)));
   connect(m_dotInheritance, SIGNAL(stateChanged(int)),
           this, SLOT(setGraphicalHierarchyEnabled(int)));
   connect(m_dotInclude, SIGNAL(stateChanged(int)),
           this, SLOT(setIncludeGraphEnabled(int)));
   connect(m_dotIncludedBy, SIGNAL(stateChanged(int)),
           this, SLOT(setIncludedByGraphEnabled(int)));
   connect(m_dotCall, SIGNAL(stateChanged(int)),
           this, SLOT(setCallGraphEnabled(int)));
   connect(m_dotCaller, SIGNAL(stateChanged(int)),
           this, SLOT(setCallerGraphEnabled(int)));



*/




/*
   connect(m_crossRef, SIGNAL(stateChanged(int)), SLOT(changeCrossRefState(int)));
   connect(m_optimizeLangGroup, SIGNAL(buttonClicked(int)), SLOT(optimizeFor(int)));
   connect(m_extractModeGroup, SIGNAL(buttonClicked(int)), SLOT(extractMode(int)));
*/


void Step2::optimizeFor(int choice)
{
   for (int i = 0; i < 6; i++) {
      updateBoolOption(m_modelData,
                       g_optimizeOptionNames[i],
                       g_optimizeMapping[choice][i]);
   }
}

void Step2::extractMode(int choice)
{
   updateBoolOption(m_modelData, STR_EXTRACT_ALL, choice == 1);
}

void Step2::changeCrossRefState(int choice)
{
   updateBoolOption(m_modelData, STR_SOURCE_BROWSER, choice == Qt::Checked);
}

void Step2::init()
{
   m_extractModeGroup->button(
      getBoolOption(m_modelData, STR_EXTRACT_ALL) ? 1 : 0)->setChecked(true);
   m_crossRef->setChecked(getBoolOption(m_modelData, STR_SOURCE_BROWSER));

   int x = 0;
   if (getBoolOption(m_modelData, STR_CPP_CLI_SUPPORT)) {
      x = 1;
   } else if (getBoolOption(m_modelData, STR_OPTIMIZE_OUTPUT_JAVA)) {
      x = 2;
   } else if (getBoolOption(m_modelData, STR_OPTIMIZE_OUTPUT_FOR_C)) {
      x = 3;
   } else if (getBoolOption(m_modelData, STR_OPTIMIZE_FOR_FORTRAN)) {
      x = 4;   
   }

   m_optimizeLangGroup->button(x)->setChecked(true);
}

Step3::Step3(Wizard *wizard, const QHash<QString, Input *> &modelData)
   : m_wizard(wizard), m_modelData(modelData)
{
      m_htmlOptions = new QGroupBox(tr("HTML"));
      m_htmlOptions->setCheckable(true);

      // GENERATE_HTML
      m_htmlOptionsGroup = new QButtonGroup(m_htmlOptions);

      QRadioButton *r = new QRadioButton(tr("plain HTML"));
      r->setChecked(true);     
      m_htmlOptionsGroup->addButton(r, 0);

      r = new QRadioButton(tr("with navigation panel"));
      m_htmlOptionsGroup->addButton(r, 1);

      r = new QRadioButton(tr("prepare for compressed HTML (.chm)"));
      m_htmlOptionsGroup->addButton(r, 2);


   //gbox->setRowStretch(6, 1);
   connect(m_htmlOptions, SIGNAL(toggled(bool)), SLOT(setHtmlEnabled(bool)));
   connect(m_texOptions, SIGNAL(toggled(bool)), SLOT(setLatexEnabled(bool)));
   connect(m_manEnabled, SIGNAL(stateChanged(int)), SLOT(setManEnabled(int)));
   connect(m_rtfEnabled, SIGNAL(stateChanged(int)), SLOT(setRtfEnabled(int)));
   connect(m_xmlEnabled, SIGNAL(stateChanged(int)), SLOT(setXmlEnabled(int)));
   connect(m_searchEnabled, SIGNAL(stateChanged(int)), SLOT(setSearchEnabled(int)));
   connect(m_htmlOptionsGroup, SIGNAL(buttonClicked(int)), SLOT(setHtmlOptions(int)));
   connect(m_texOptionsGroup, SIGNAL(buttonClicked(int)), SLOT(setLatexOptions(int)));

}

/*
void Step3::setHtmlOptions(int id)
{
   if (id == 0) { // plain HTML
      updateBoolOption(m_modelData, STR_GENERATE_HTMLHELP, false);
      updateBoolOption(m_modelData, STR_GENERATE_TREEVIEW, false);
   } else if (id == 1) { // with navigation tree
      updateBoolOption(m_modelData, STR_GENERATE_HTMLHELP, false);
      updateBoolOption(m_modelData, STR_GENERATE_TREEVIEW, true);
   } else if (id == 2) { // with compiled help
      updateBoolOption(m_modelData, STR_GENERATE_HTMLHELP, true);
      updateBoolOption(m_modelData, STR_GENERATE_TREEVIEW, false);
   }
}

void Step3::setLatexOptions(int id)
{
   if (id == 0) { // hyperlinked PDF
      updateBoolOption(m_modelData, STR_USE_PDFLATEX, true);
      updateBoolOption(m_modelData, STR_PDF_HYPERLINKS, true);
   } else if (id == 1) { // PDF
      updateBoolOption(m_modelData, STR_USE_PDFLATEX, true);
      updateBoolOption(m_modelData, STR_PDF_HYPERLINKS, false);
   } else if (id == 2) { // PostScript
      updateBoolOption(m_modelData, STR_USE_PDFLATEX, false);
      updateBoolOption(m_modelData, STR_PDF_HYPERLINKS, false);
   }
}

*/

void Step3::init()
{
   m_htmlOptions->setChecked(getBoolOption(m_modelData, STR_GENERATE_HTML));
   m_texOptions->setChecked(getBoolOption(m_modelData, STR_GENERATE_LATEX));
   m_manEnabled->setChecked(getBoolOption(m_modelData, STR_GENERATE_MAN));
   m_rtfEnabled->setChecked(getBoolOption(m_modelData, STR_GENERATE_RTF));
   m_xmlEnabled->setChecked(getBoolOption(m_modelData, STR_GENERATE_XML));
   m_searchEnabled->setChecked(getBoolOption(m_modelData, STR_SEARCHENGINE));
   if (getBoolOption(m_modelData, STR_GENERATE_HTMLHELP)) {
      m_htmlOptionsGroup->button(2)->setChecked(true); // compiled help
   } else if (getBoolOption(m_modelData, STR_GENERATE_TREEVIEW)) {
      m_htmlOptionsGroup->button(1)->setChecked(true); // navigation tree
   } else {
      m_htmlOptionsGroup->button(0)->setChecked(true); // plain HTML
   }
   if (!getBoolOption(m_modelData, STR_USE_PDFLATEX)) {
      m_texOptionsGroup->button(2)->setChecked(true); // PostScript
   } else if (!getBoolOption(m_modelData, STR_PDF_HYPERLINKS)) {
      m_texOptionsGroup->button(1)->setChecked(true); // Plain PDF
   } else {
      m_texOptionsGroup->button(0)->setChecked(true); // PDF with hyperlinks
   }
}

//==========================================================================

Step4::Step4(Wizard *wizard, const QHash<QString, Input *> &modelData)
   : m_wizard(wizard), m_modelData(modelData)
{
   QRadioButton *rb = new QRadioButton(tr("No diagrams"));
   m_diagramModeGroup->addButton(rb, 0);

   QGridLayout *gbox = new QGridLayout();


   gbox->addWidget(rb, 1, 0);
   // CLASS_DIAGRAMS = NO, HAVE_DOT = NO
   rb->setChecked(true);
   rb = new QRadioButton(tr("Use built-in class diagram generator"));
   m_diagramModeGroup->addButton(rb, 1);
   // CLASS_DIAGRAMS = YES, HAVE_DOT = NO
   gbox->addWidget(rb, 2, 0);
   rb = new QRadioButton(tr("Use dot tool from the GraphViz package"));
   m_diagramModeGroup->addButton(rb, 2);
   gbox->addWidget(rb, 3, 0);
   // CLASS_DIAGRAMS = NO, HAVE_DOT = YES



   m_dotGroup = new QGroupBox(tr("Dot graphs to generate"));
   QVBoxLayout *vbox = new QVBoxLayout;
   m_dotClass = new QCheckBox(tr("Class diagrams"));
   // CLASS_GRAPH
   m_dotCollaboration = new QCheckBox(tr("Collaboration diagrams"));
   // COLLABORATION_GRAPH
   m_dotInheritance = new QCheckBox(tr("Overall Class hierarchy"));
   // GRAPHICAL_HIERARCHY
   m_dotInclude = new QCheckBox(tr("Include dependency graphs"));
   // INCLUDE_GRAPH
   m_dotIncludedBy = new QCheckBox(tr("Included by dependency graphs"));
   // INCLUDED_BY_GRAPH
   m_dotCall = new QCheckBox(tr("Call graphs"));
   // CALL_GRAPH
   m_dotCaller = new QCheckBox(tr("Called by graphs"));
   // CALLER_GRAPH
   vbox->addWidget(m_dotClass);
   vbox->addWidget(m_dotCollaboration);
   vbox->addWidget(m_dotInheritance);
   vbox->addWidget(m_dotInclude);
   vbox->addWidget(m_dotIncludedBy);
   vbox->addWidget(m_dotCall);
   vbox->addWidget(m_dotCaller);
   vbox->addStretch(1);
   m_dotGroup->setLayout(vbox);
   m_dotClass->setChecked(true);
   m_dotGroup->setEnabled(false);
   gbox->addWidget(m_dotGroup, 4, 0);

   m_dotInclude->setChecked(true);
   m_dotCollaboration->setChecked(true);
   gbox->setRowStretch(5, 1);
}


/*
void Step4::diagramModeChanged(int id)
{
   if (id == 0) { // no diagrams
      updateBoolOption(m_modelData, STR_HAVE_DOT, false);
      updateBoolOption(m_modelData, STR_CLASS_DIAGRAMS, false);

   } else if (id == 1) { // builtin diagrams
      updateBoolOption(m_modelData, STR_HAVE_DOT, false);
      updateBoolOption(m_modelData, STR_CLASS_DIAGRAMS, true);

   } else if (id == 2) { // dot diagrams
      updateBoolOption(m_modelData, STR_HAVE_DOT, true);
      updateBoolOption(m_modelData, STR_CLASS_DIAGRAMS, false);
   }
   m_dotGroup->setEnabled(id == 2);
}

*/

