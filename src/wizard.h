/******************************************************************************
 *
 *
 *
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby
 * granted. No representations are made about the suitability of this software
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 */

#ifndef WIZARD_H
#define WIZARD_H

#include <QSplitter>
#include <QHash>
#include <QDialog>

class Input;
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QGroupBox;
class QButtonGroup;
class Wizard;
class QImage;
class QLabel;

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

 public :
   CS_SLOT_1(Public, void setCol(int h, int s, int g))
   CS_SLOT_2(setCol)
   //void setCol(int h, int s);

 public:
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


class Step1 : public QWidget
{
   CS_OBJECT(Step1)

 public:
   Step1(Wizard *parent, const QHash<QString, Input *> &modelData);
   void init();

 private :
   CS_SLOT_1(Private, void selectSourceDir())
   CS_SLOT_2(selectSourceDir)
   CS_SLOT_1(Private, void selectDestinationDir())
   CS_SLOT_2(selectDestinationDir)
   CS_SLOT_1(Private, void selectProjectIcon())
   CS_SLOT_2(selectProjectIcon)
   CS_SLOT_1(Private, void setProjectName(const QString &name))
   CS_SLOT_2(setProjectName)
   CS_SLOT_1(Private, void setProjectBrief(const QString &desc))
   CS_SLOT_2(setProjectBrief)
   CS_SLOT_1(Private, void setProjectNumber(const QString &num))
   CS_SLOT_2(setProjectNumber)
   CS_SLOT_1(Private, void setSourceDir(const QString &dir))
   CS_SLOT_2(setSourceDir)
   CS_SLOT_1(Private, void setDestinationDir(const QString &dir))
   CS_SLOT_2(setDestinationDir)
   CS_SLOT_1(Private, void setRecursiveScan(int un_named_arg1))
   CS_SLOT_2(setRecursiveScan)

 private:
   QLineEdit *m_projName;
   QLineEdit *m_projBrief;
   QLineEdit *m_projNumber;
   QLineEdit *m_sourceDir;
   QLineEdit *m_destDir;
   QLabel    *m_projIconLab;
   QCheckBox *m_recursive;
   QPushButton *m_srcSelectDir;
   QPushButton *m_dstSelectDir;
   Wizard    *m_wizard;
   const QHash<QString, Input *> &m_modelData;
};

class Step2 : public QWidget
{
   CS_OBJECT(Step2)

 public:
   Step2(Wizard *parent, const QHash<QString, Input *> &modelData);
   void init();

 private :
   CS_SLOT_1(Private, void optimizeFor(int choice))
   CS_SLOT_2(optimizeFor)
   CS_SLOT_1(Private, void extractMode(int choice))
   CS_SLOT_2(extractMode)
   CS_SLOT_1(Private, void changeCrossRefState(int choice))
   CS_SLOT_2(changeCrossRefState)

 private:
   QGroupBox    *m_extractMode;
   QGroupBox    *m_optimizeLang;
   QButtonGroup *m_extractModeGroup;
   QButtonGroup *m_optimizeLangGroup;
   QCheckBox    *m_crossRef;
   Wizard       *m_wizard;
   const QHash<QString, Input *> &m_modelData;
};

class Step3 : public QWidget
{
   CS_OBJECT(Step3)

 public:
   Step3(Wizard *parent, const QHash<QString, Input *> &modelData);
   void init();

 private :
   CS_SLOT_1(Private, void setHtmlEnabled(bool un_named_arg1))
   CS_SLOT_2(setHtmlEnabled)
   CS_SLOT_1(Private, void setLatexEnabled(bool un_named_arg1))
   CS_SLOT_2(setLatexEnabled)
   CS_SLOT_1(Private, void setManEnabled(int un_named_arg1))
   CS_SLOT_2(setManEnabled)
   CS_SLOT_1(Private, void setRtfEnabled(int un_named_arg1))
   CS_SLOT_2(setRtfEnabled)
   CS_SLOT_1(Private, void setXmlEnabled(int un_named_arg1))
   CS_SLOT_2(setXmlEnabled)
   CS_SLOT_1(Private, void setSearchEnabled(int un_named_arg1))
   CS_SLOT_2(setSearchEnabled)
   CS_SLOT_1(Private, void setHtmlOptions(int un_named_arg1))
   CS_SLOT_2(setHtmlOptions)
   CS_SLOT_1(Private, void setLatexOptions(int un_named_arg1))
   CS_SLOT_2(setLatexOptions)
   CS_SLOT_1(Private, void tuneColorDialog())
   CS_SLOT_2(tuneColorDialog)

   QGroupBox *m_texOptions;
   QButtonGroup *m_texOptionsGroup;
   QGroupBox *m_htmlOptions;
   QButtonGroup *m_htmlOptionsGroup;
   QCheckBox *m_htmlEnabled;
   QCheckBox *m_latexEnabled;
   QCheckBox *m_manEnabled;
   QCheckBox *m_rtfEnabled;
   QCheckBox *m_xmlEnabled;
   QCheckBox *m_searchEnabled;
   QPushButton *m_tuneColor;
   Wizard    *m_wizard;
   const QHash<QString, Input *> &m_modelData;
};

class Step4 : public QWidget
{
   CS_OBJECT(Step4)

 public:
   Step4(Wizard *parent, const QHash<QString, Input *> &modelData);
   void init();

 private :
   CS_SLOT_1(Private, void diagramModeChanged(int un_named_arg1))
   CS_SLOT_2(diagramModeChanged)
   CS_SLOT_1(Private, void setClassGraphEnabled(int state))
   CS_SLOT_2(setClassGraphEnabled)
   CS_SLOT_1(Private, void setCollaborationGraphEnabled(int state))
   CS_SLOT_2(setCollaborationGraphEnabled)
   CS_SLOT_1(Private, void setGraphicalHierarchyEnabled(int state))
   CS_SLOT_2(setGraphicalHierarchyEnabled)
   CS_SLOT_1(Private, void setIncludeGraphEnabled(int state))
   CS_SLOT_2(setIncludeGraphEnabled)
   CS_SLOT_1(Private, void setIncludedByGraphEnabled(int state))
   CS_SLOT_2(setIncludedByGraphEnabled)
   CS_SLOT_1(Private, void setCallGraphEnabled(int state))
   CS_SLOT_2(setCallGraphEnabled)
   CS_SLOT_1(Private, void setCallerGraphEnabled(int state))
   CS_SLOT_2(setCallerGraphEnabled)


   QGroupBox *m_diagramMode;
   QButtonGroup *m_diagramModeGroup;
   QGroupBox *m_dotGroup;
   QCheckBox *m_dotClass;
   QCheckBox *m_dotCollaboration;
   QCheckBox *m_dotInclude;
   QCheckBox *m_dotIncludedBy;
   QCheckBox *m_dotInheritance;
   QCheckBox *m_dotCall;
   QCheckBox *m_dotCaller;
   Wizard    *m_wizard;
   const QHash<QString, Input *> &m_modelData;
};

class Wizard : public QSplitter
{
   CS_OBJECT(Wizard)
 public:
   Wizard(const QHash<QString, Input *> &modelData, QWidget *parent = 0);
   ~Wizard();

 public :
   CS_SLOT_1(Public, void refresh())
   CS_SLOT_2(refresh)

 private :
   CS_SLOT_1(Private, void activateTopic(QTreeWidgetItem *item, QTreeWidgetItem *un_named_arg2))
   CS_SLOT_2(activateTopic)
   CS_SLOT_1(Private, void nextTopic())
   CS_SLOT_2(nextTopic)
   CS_SLOT_1(Private, void prevTopic())
   CS_SLOT_2(prevTopic)

 public:
   CS_SIGNAL_1(Public, void done())
   CS_SIGNAL_2(done)

 private:
   const QHash<QString, Input *> &m_modelData;
   QTreeWidget    *m_treeWidget;
   QStackedWidget *m_topicStack;
   Step1          *m_step1;
   Step2          *m_step2;
   Step3          *m_step3;
   Step4          *m_step4;
   QPushButton    *m_next;
   QPushButton    *m_prev;
};

#endif
