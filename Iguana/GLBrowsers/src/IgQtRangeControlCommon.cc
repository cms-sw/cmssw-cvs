#include "Iguana/GLBrowsers/interface/IgQtRangeControlCommon.h"
#include "Iguana/Studio/interface/IgQtLineEdit.h"
#include "Iguana/Studio/interface/IgQtPixButton.h"
#include "Iguana/GLBrowsers/pixmaps/gotoMin.xpm"
#include "Iguana/GLBrowsers/pixmaps/fastRev.xpm"
#include "Iguana/GLBrowsers/pixmaps/rev.xpm"
#include "Iguana/GLBrowsers/pixmaps/fwd.xpm"
#include "Iguana/GLBrowsers/pixmaps/fastFwd.xpm"
#include "Iguana/GLBrowsers/pixmaps/gotoMax.xpm"
#include <QFrame>
#include <QSlider>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QVBoxLayout>

IgQtRangeControlCommon::IgQtRangeControlCommon (QWidget *parent, int /*parts*/,
				                const char *name /* = 0 */)
    : m_stepType   (ADD),
      m_fwdEnabled (true),
      m_revEnabled (true),
      m_widget	   (new QFrame (parent, name)),
      m_layout	   (0),
      m_row        (0),
      m_revMin	   (0),
      m_revLong	   (0),
      m_revSmall   (0),
      m_text       (0),
      m_fwdSmall   (0),
      m_fwdLong    (0),
      m_fwdMax     (0),
      m_slider     (0),
      m_resetChangedValue (0),
      m_valueEditable (true),
      m_stepEditable (true),
      m_rangeEditable  (true),
      m_precisionEditable (true),
      m_emitSettingsChanged (true)
{ }

void
IgQtRangeControlCommon::initialize (int parts)
{
    QSizePolicy	expexp (QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (!m_layout)
      m_layout = new QVBoxLayout (m_widget, 1, 0);

    m_widget->setFrameStyle (QFrame::Plain | QFrame::Box);
    m_widget->setFixedHeight (35);

    m_row = new QWidget (m_widget);
    QHBoxLayout	*rowLayout = new QHBoxLayout (m_row, 0, 0);
    m_layout->addWidget (m_row);
    
    // Add backward buttons
    if (parts & StepperPart)
    {
	m_revMin   = new IgQtPixButton (gotoMin_xpm, m_row);
	m_revLong  = new IgQtPixButton (fastRev_xpm, m_row);
	m_revSmall = new IgQtPixButton (rev_xpm, m_row);

	rowLayout->addWidget (m_revMin);
	rowLayout->addWidget (m_revLong);
	rowLayout->addWidget (m_revSmall);

	connect (m_revMin,   SIGNAL (clicked ()), this, SLOT (revMin ()));
	connect (m_revLong,  SIGNAL (clicked ()), this, SLOT (revLong ()));
	connect (m_revSmall, SIGNAL (clicked ()), this, SLOT (revSmall ()));
    }

    // Add text edit for the value (FIXME: optional label vs. text)
    if (parts & ValuePart)
    {
	m_text = new IgQtLineEdit (m_row);
	m_text->setSizePolicy (expexp);
	m_text->setReadOnly (false);
	m_text->setFocusPolicy (Qt::StrongFocus);
	m_text->setAlignment (Qt::AlignRight);
	m_text->setFixedWidth (75);
	m_text->setText ("0");

	rowLayout->addWidget (m_text);
	connect (m_text, SIGNAL (valueChanged (const QString&)),
		 this,   SLOT (textChanged (const QString&)));
    }

    // Add forward buttons
    if (parts & StepperPart)
    {
	m_fwdSmall = new IgQtPixButton (fwd_xpm, m_row);
	m_fwdLong  = new IgQtPixButton (fastFwd_xpm, m_row);
	m_fwdMax   = new IgQtPixButton (gotoMax_xpm, m_row);

	rowLayout->addWidget (m_fwdSmall);
	rowLayout->addWidget (m_fwdLong);
	rowLayout->addWidget (m_fwdMax);

	connect (m_fwdSmall, SIGNAL (clicked ()), this, SLOT (fwdSmall ()));
	connect (m_fwdLong,  SIGNAL (clicked ()), this, SLOT (fwdLong ()));
	connect (m_fwdMax,   SIGNAL (clicked ()), this, SLOT (fwdMax ()));
    }

    // Add slider below the row
    if (parts & SliderPart)
    {
	m_slider = new QSlider (Qt::Horizontal, m_widget);
	m_slider->setSizePolicy (expexp);
	m_slider->setFixedHeight (8);
	m_slider->setLineStep (1);
	m_layout->addWidget (m_slider);

	connect (m_slider, SIGNAL (valueChanged (int)),
		 this,     SLOT (sliderChanged (int)));
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/** Set the stepping fashion to @a value.  */
void
IgQtRangeControlCommon::setStepType (StepType value)
{ m_stepType = value; }

/** Get the current way stepping is done.  */
IgQtRangeControlCommon::StepType
IgQtRangeControlCommon::getStepType (void) const
{ return m_stepType; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/** Slider has changed to position @a value small steps from the
    minimum.  Adjust current control value. */
void
IgQtRangeControlCommon::sliderChanged (int /*value*/)
{ } 

void
IgQtRangeControlCommon::resetChangedValue (bool value)
{ m_resetChangedValue = value; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::revMin (void)
{ }

void
IgQtRangeControlCommon::revLong (void)
{ }

void
IgQtRangeControlCommon::revSmall (void)
{ }

void
IgQtRangeControlCommon::fwdSmall (void)
{ }

void
IgQtRangeControlCommon::fwdLong (void)
{ }

void
IgQtRangeControlCommon::fwdMax (void)
{ }

void
IgQtRangeControlCommon::emitSignals (void)
{ }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
IgQtRangeControlCommon::textChanged (const QString& /*newval*/)
{ }

void
IgQtRangeControlCommon::setDirty (bool dirty)
{ IgControlItem::setDirty (dirty); }

void
IgQtRangeControlCommon::fwdEnable (bool value)
{
    if (m_fwdSmall && m_fwdEnabled != value)
    {
	m_fwdSmall->setEnabled (value);
	m_fwdLong->setEnabled (value);
	m_fwdMax->setEnabled (value);
	m_fwdEnabled = value;
    }
}

void
IgQtRangeControlCommon::revEnable (bool value)
{
    if (m_revSmall && m_revEnabled != value)
    {
	m_revSmall->setEnabled (value);
	m_revLong->setEnabled (value);
	m_revMin->setEnabled (value);
	m_revEnabled = value;
    }
}

void
IgQtRangeControlCommon::addFwdButton (QWidget *w)
{
    addFwdWidget (w);
    connect (w, SIGNAL(clicked()), this, SLOT (fwdSmall ()));
}

void
IgQtRangeControlCommon::addRevButton (QWidget *w)
{
    addRevWidget (w);
    connect (w, SIGNAL(clicked()), this, SLOT (revSmall ()));
}

void
IgQtRangeControlCommon::addFwdWidget (QWidget *w)
{ insertWidget (w, -1); }

void
IgQtRangeControlCommon::addRevWidget (QWidget *w)
{ insertWidget (w, 0); }

void
IgQtRangeControlCommon::insertWidget (QWidget *w, int index /* = -1 */)
{
    w->reparent (m_row, QPoint (0, 0));
    ((QBoxLayout *) m_row->layout ())->insertWidget (index, w);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QWidget *
IgQtRangeControlCommon::widget (void) const
{ return m_widget; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::show (void)
{ m_widget->show (); }

void
IgQtRangeControlCommon::hide (void)
{ m_widget->hide (); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::editSettingsChanged (void)
{ if (isEmitSettingsChanged ()) emit settingsChanged (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::valueEditable (bool editable /*= true*/)
{ m_valueEditable = editable; }
    
void
IgQtRangeControlCommon::rangeEditable (bool editable /*= true*/)
{ m_rangeEditable = editable; }
    
void
IgQtRangeControlCommon::stepEditable (bool editable /*= true*/)
{ m_stepEditable = editable; }

void
IgQtRangeControlCommon::precisionEditable (bool editable /*= true*/)
{ m_precisionEditable = editable; }

bool
IgQtRangeControlCommon::isValueEditable (void) const
{ return m_valueEditable; }
    
bool
IgQtRangeControlCommon::isRangeEditable (void) const
{ return m_rangeEditable; }
    
bool
IgQtRangeControlCommon::isStepEditable (void) const
{ return m_stepEditable; }

bool
IgQtRangeControlCommon::isPrecisionEditable (void) const
{ return m_precisionEditable; }

bool
IgQtRangeControlCommon::isEmitSettingsChanged (void) const
{ return m_emitSettingsChanged; }

void
IgQtRangeControlCommon::setEmitSettingsChanged (bool value /*=true*/)
{ m_emitSettingsChanged = value; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QString
IgQtRangeControlCommon::getSettingItem (QString &setting)
{
    QString item ("");
    int index = setting.find(":");
    if (index == -1)
    {
      item = setting;
      setting = "";
    }
    else
    {
      item = setting.left(index);
      setting = setting.right (setting.length()-index-1);
    }
    return item;
}

void
IgQtRangeControlCommon::addSettingItem (const QString &item, QString &setting)
{
    if (setting.isEmpty ())
      setting = item;
    else
      setting += ":" + item;
}
