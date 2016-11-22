#include "Iguana/GLBrowsers/interface/IgGLRenderCategory.h"
#include "Iguana/GLBrowsers/interface/IgQtRangeControlUInt.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>

IG_DEFINE_STATE_ELEMENT (IgGLRenderCategory, "/ControlCentre/Categories/View Properties/GL Render");

IgGLRenderCategory::IgGLRenderCategory (IgState *state)
    : IgCommonViewCategory (state, "GL Render"),
      m_transparencyType(0),
      m_redrawPriority	(0),
      m_antialiasingPasses(0),
      m_antialiasing	(0),
      m_autoRedraw	(0),
      m_passUpdate	(0)
{
    addGLRenderTab ();
    state->put (s_key, this);    
}

IgGLRenderCategory::~IgGLRenderCategory (void)
{ state()->detach (s_key); }

void
IgGLRenderCategory::addGLRenderTab (void)
{
    QWidget	*renderOptions = new QWidget (topArea ());
    topLayout ()->addWidget (renderOptions);

    QVBoxLayout	*renderOptionsVLayout  = new QVBoxLayout (renderOptions);
    QGridLayout	*renderOptionsLayout   = new QGridLayout (renderOptionsVLayout);

    QLabel *transpTypeLabel = new QLabel (renderOptions, "transpTypeLabel");
    transpTypeLabel->setText (tr ("Transparency Type"));
    renderOptionsLayout->addWidget (transpTypeLabel, 0, 0);

    QLabel *redrawPriorityLabel = new QLabel (renderOptions, "redrawPriorityLabel");
    redrawPriorityLabel->setText (tr ("Redraw Priority"));
    renderOptionsLayout->addWidget (redrawPriorityLabel, 1, 0);

    m_antialiasing = new QCheckBox (renderOptions, "m_antialiasing");
    m_antialiasing->setText (tr ("Antialiasing Passes"));
    renderOptionsLayout->addWidget (m_antialiasing, 2, 0);

    m_transparencyType = new QComboBox (FALSE, renderOptions, "m_transparencyType" );
    m_transparencyType->clear ();
    m_transparencyType->insertItem (tr ("Screen door"));
    m_transparencyType->insertItem (tr ("Add"));
    m_transparencyType->insertItem (tr ("Delayed add"));
    m_transparencyType->insertItem (tr ("Sorted object add"));
    m_transparencyType->insertItem (tr ("Blend"));
    m_transparencyType->insertItem (tr ("Delayed blend"));
    m_transparencyType->insertItem (tr ("Sorted object blend"));
    renderOptionsLayout->addWidget (m_transparencyType, 0, 1);

    m_redrawPriority = new IgQtRangeControlUInt (renderOptions,
 					     IgQtRangeControl::StepperPart
 					     | IgQtRangeControl::ValuePart);
					 
    m_redrawPriority->setMinValue (1);
    m_redrawPriority->setLongStep (5);
    m_redrawPriority->setSmallStep (1);
    renderOptionsLayout->addWidget (m_redrawPriority->widget (), 1, 1);

    m_antialiasingPasses = new IgQtRangeControlUInt (renderOptions,
						 IgQtRangeControl::StepperPart
						 | IgQtRangeControl::ValuePart);
					 
    m_antialiasingPasses->setMinValue (1);
    m_antialiasingPasses->setLongStep (5);
    m_antialiasingPasses->setSmallStep (1);
    m_antialiasingPasses->widget ()->setEnabled (false);
    renderOptionsLayout->addWidget (m_antialiasingPasses->widget (), 2, 1);

    m_autoRedraw = new QCheckBox (renderOptions, "m_autoRedraw");
    m_autoRedraw->setText (tr ("Auto Redraw"));
    renderOptionsVLayout->addWidget (m_autoRedraw);

    m_passUpdate = new QCheckBox (renderOptions, "m_passUpdate");
    m_passUpdate->setText (tr ("Render Pass Update"));
    renderOptionsVLayout->addWidget (m_passUpdate);

    renderOptionsVLayout->addItem (new QSpacerItem ( 0, 0, QSizePolicy::Expanding,
						    QSizePolicy::Expanding));

    connect (m_transparencyType,SIGNAL (activated (int)),
             this,		SLOT   (setTransparencyType (int)));
    connect (m_redrawPriority,	SIGNAL (newValue (unsigned int)),
             this,		SLOT   (setRedrawPriority (unsigned int)));
    connect (m_antialiasingPasses,SIGNAL(newValue (unsigned int)),
             this,		SLOT   (setAntialiasingPasses (unsigned int)));
    connect (m_antialiasing,	SIGNAL (toggled (bool)),
             this,		SLOT   (setAntialiasing(bool)));
    connect (m_antialiasing,	SIGNAL (toggled (bool)),
             m_antialiasingPasses->widget (),SLOT(setEnabled (bool)));
    connect (m_autoRedraw,	SIGNAL (toggled (bool)),
             this,		SLOT   (setAutoRedraw(bool)));
    connect (m_passUpdate,	SIGNAL (toggled (bool)),
             this,		SLOT   (setPassUpdate(bool)));
}

void 
IgGLRenderCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
    {
	IgCommonViewCategory::browserChanged (source);
	assert (m_source->getGLRenderAction ());
	SoGLRenderAction	*ra = m_source->getGLRenderAction ();
	SbBool antialising;
	int    numPass;
	m_source->getAntialiasing (antialising, numPass);
      
	updateTransparencyType (ra->getTransparencyType ());
	m_redrawPriority->setValue (m_source->getRedrawPriority ());
	m_antialiasingPasses->setValue (numPass);
	m_antialiasing->setChecked (antialising);
	m_autoRedraw->setChecked (m_source->isAutoRedraw ());
	m_passUpdate->setChecked (ra->isPassUpdate ());
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgGLRenderCategory::setTransparencyType (int type)
{
    assert (m_source->getGLRenderAction ());
    SoGLRenderAction	*ra = m_source->getGLRenderAction ();
    switch (type)
    {
    case 0:
        ra->setTransparencyType (SoGLRenderAction::SCREEN_DOOR);
        break;
    case 1:
        ra->setTransparencyType (SoGLRenderAction::ADD);
        break;
    case 2:
        ra->setTransparencyType (SoGLRenderAction::DELAYED_ADD);
        break;
    case 3:
        ra->setTransparencyType (SoGLRenderAction::SORTED_OBJECT_ADD);
        break;
    case 4:
        ra->setTransparencyType (SoGLRenderAction::BLEND);
        break;
    case 5:
        ra->setTransparencyType (SoGLRenderAction::DELAYED_BLEND);
        break;
    case 6:
        ra->setTransparencyType (SoGLRenderAction::SORTED_OBJECT_BLEND);
        break;
    default:
        assert (false);
    }
}

void
IgGLRenderCategory::updateTransparencyType (int type)
{
    switch (type)
    {
    case SoGLRenderAction::SCREEN_DOOR:
        m_transparencyType->setCurrentItem (0);
        break;
    case SoGLRenderAction::ADD:
        m_transparencyType->setCurrentItem (1);
        break;
    case SoGLRenderAction::DELAYED_ADD:
        m_transparencyType->setCurrentItem (2);
        break;
    case SoGLRenderAction::SORTED_OBJECT_ADD:
        m_transparencyType->setCurrentItem (3);
        break;
    case SoGLRenderAction::BLEND:
        m_transparencyType->setCurrentItem (4);
        break;
    case SoGLRenderAction::DELAYED_BLEND:
        m_transparencyType->setCurrentItem (5);
        break;
    case SoGLRenderAction::SORTED_OBJECT_BLEND:
        m_transparencyType->setCurrentItem (6);
        break;
    default:
        assert (false);
    }
}

void
IgGLRenderCategory::setRedrawPriority (unsigned int priority)
{ m_source->setRedrawPriority (int (priority)); }

void
IgGLRenderCategory::setAntialiasingPasses (unsigned int passes)
{ m_source->setAntialiasing (m_antialiasing->isChecked (), int(passes)); }

void
IgGLRenderCategory::setAntialiasing (bool option)
{ m_source->setAntialiasing (option, int (m_antialiasingPasses->value ())); }

void
IgGLRenderCategory::setAutoRedraw (bool option)
{ 
    bool old = m_source->isAutoRedraw ();
    if (old != option)
	m_source->setAutoRedraw (option);
}

void
IgGLRenderCategory::setPassUpdate (bool option)
{
    assert (m_source->getGLRenderAction ());
    SoGLRenderAction	*ra = m_source->getGLRenderAction ();
    bool old = ra->isPassUpdate ();
    if (old != option)
	ra->setPassUpdate (option);
}

