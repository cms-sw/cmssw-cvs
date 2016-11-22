#include "Iguana/Studio/interface/IgQtPixButton.h"
#include <QEvent>
#include <QCursor>
#include <QMouseEvent>
#include <QPixmap>

IgQtPixButton::IgQtPixButton (QWidget *parent, const char *name)
    : QToolButton (parent, name)
{ initialise (QPixmap ()); }

IgQtPixButton::IgQtPixButton (QPixmap pixmap, QWidget *parent,const char *name)
    : QToolButton (parent, name)
{ initialise (pixmap); }

void
IgQtPixButton::initialise (QPixmap pixmap)
{
    setAutoRepeat (true);
    setAutoRaise (true);
    if (!pixmap.isNull ())
      setPixmap (pixmap);
}
