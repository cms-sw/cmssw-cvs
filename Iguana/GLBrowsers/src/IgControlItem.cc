#include "Iguana/GLBrowsers/interface/IgControlItem.h"
#include <QHBoxLayout>
#include <QWidget>

IgControlItem::IgControlItem(void)
  : m_editable (true),
    m_dirty (false)
{}

//////////////////////////////////////////////////////////////////////
bool
IgControlItem::isDirty (void) const
{ return m_dirty; }

void
IgControlItem::setDirty (bool state)
{
    if (state && ! m_dirty)
    {
	m_dirty = true;
	emit dirty ();
    }
    else if (! state && m_dirty)
    {
	m_dirty = false;
	emit clean ();
    }
}

void
IgControlItem::setDirty (void)
{ setDirty (true); }

void
IgControlItem::setClean (void)
{ setDirty (false); }

//////////////////////////////////////////////////////////////////////
void
IgControlItem::setEditable (bool value)
{
    m_editable = value;
    emit editable (value);
}

bool
IgControlItem::isEditable (void) const
{ return m_editable; }
//////////////////////////////////////////////////////////////////////
QWidget *
IgControlItem::makeSpacedBox (QWidget *parent, QWidget *content)
{ 
    QWidget *box = makeBox (parent, content);
    box->layout ()->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding));

    return box;
}

QWidget *
IgControlItem::makeBox (QWidget *parent, QWidget *content)
{
  QWidget *hbox = new QWidget(parent);
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(content);
  hbox->setLayout(layout);

  return hbox;
}
