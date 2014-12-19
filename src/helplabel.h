#ifndef HELPLABEL_H
#define HELPLABEL_H

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QPoint>

class HelpLabel : public QLabel
{
   CS_OBJECT(HelpLabel)

 public:
   HelpLabel(const QString &text) : QLabel(text) {
      setContextMenuPolicy(Qt::CustomContextMenu);
      connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
              this, SLOT(showMenu(const QPoint &)));
   }

   CS_SIGNAL_1(Public, void enter())
   CS_SIGNAL_2(enter)
   CS_SIGNAL_1(Public, void reset())
   CS_SIGNAL_2(reset)

 protected:
   void enterEvent( QEvent *event ) {
      enter();
      QLabel::enterEvent(event);
   }

 private :
   CS_SLOT_1(Private, void showMenu(const QPoint &p))
   CS_SLOT_2(showMenu)

};

inline void HelpLabel::showMenu(const QPoint &p)
{
   QMenu menu(this);
   QAction *a = menu.addAction(tr("Reset to default"));

   if (menu.exec(mapToGlobal(p)) == a) {
      reset();
   }
}

#endif
