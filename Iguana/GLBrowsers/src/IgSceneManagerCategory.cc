#include "Iguana/GLBrowsers/interface/IgSceneManagerCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
#include "Iguana/Framework/interface/IgState.h"
#include <QCheckBox>
#include <QColorDialog>
#include <QFrame>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>

IG_DEFINE_STATE_ELEMENT (IgSceneManagerCategory, "/ControlCentre/Categories/View Properties/Scene Manager");

IgSceneManagerCategory::IgSceneManagerCategory (IgState *state)
    : IgCommonViewCategory (state, "Scene Manager"),
      m_background	(0),
      m_rgbMode		(0),
      m_realTimeUpdate	(0)
{
    addSceneManagerTab ();
    state->put (s_key, this);    
}

IgSceneManagerCategory::~IgSceneManagerCategory (void)
{ state()->detach (s_key); }

void
IgSceneManagerCategory::addSceneManagerTab (void)
{
    QWidget	*sceneManagerOptions = new QWidget (topArea ());
    topLayout ()->addWidget (sceneManagerOptions);
    
    QVBoxLayout	*sceneManagerVLayout  = new QVBoxLayout (sceneManagerOptions);
    QGridLayout	*sceneManagerLayout   = new QGridLayout (sceneManagerVLayout);

    QVBoxLayout *bodyLayout = new QVBoxLayout (0, 0, 6, "bodyLayout"); 

    QHBoxLayout *topLayout = new QHBoxLayout (0, 0, 6, "topLayout"); 

    //Scene Manager background color
    QLabel *bgColLabel = new QLabel (sceneManagerOptions, "bgColLabel");
    bgColLabel->setText (tr ("Background Color"));
    topLayout->addWidget (bgColLabel);

    m_background = new QFrame (sceneManagerOptions, "m_background");
    m_background->setMinimumSize (QSize (77, 0 ));
    m_background->setFrameShape (QFrame::StyledPanel);
    m_background->setFrameShadow (QFrame::Sunken);
    m_background->setBackgroundRole(QPalette::Window);
    m_background->setAutoFillBackground(true);
    QSpacerItem* spacer1 = new QSpacerItem (21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topLayout->addItem (spacer1);
    topLayout->addWidget (m_background);
    QSpacerItem* spacer2 = new QSpacerItem (21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topLayout->addItem (spacer2);

    QPushButton *selectColor = new QPushButton (sceneManagerOptions, "selectColor" );
    selectColor->setText (tr ("&Select"));
    selectColor->setAccel (QKeySequence (tr ("Alt+S")));
    topLayout->addWidget (selectColor);
    bodyLayout->addLayout (topLayout);

    QVBoxLayout *bottomLayout = new QVBoxLayout (0, 0, 6, "bottomLayout"); 

    // Scene Manager RGB Mode
    m_rgbMode = new QCheckBox (sceneManagerOptions, "m_rgbMode");
    m_rgbMode->setText (tr ("RGB Mode"));
    bottomLayout->addWidget (m_rgbMode);

    // Real Time Update while animating
    m_realTimeUpdate = new QCheckBox (sceneManagerOptions, "m_realTimeUpdate");
    m_realTimeUpdate->setText (tr ("Real Time Update"));
    bottomLayout->addWidget (m_realTimeUpdate);
    bodyLayout->addLayout (bottomLayout);

    sceneManagerLayout->addLayout (bodyLayout, 0, 0);    
    QSpacerItem *spacer = new QSpacerItem (20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);
    sceneManagerVLayout->addItem (spacer);

    connect (selectColor,      	SIGNAL (clicked (void)),
	     this,		SLOT   (setBackgroundColor(void)));
    connect (m_rgbMode,		SIGNAL (toggled (bool)),
	     this,		SLOT   (setRGBMode(bool)));
    connect (m_realTimeUpdate,	SIGNAL (toggled (bool)),
	     this,		SLOT   (setRealTimeUpdate(bool)));
}

void 
IgSceneManagerCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
    {
	IgCommonViewCategory::browserChanged (source);
	SbColor color (m_source->getSceneManager ()->getBackgroundColor ());
	QColor color1;
	color1.setRgb (int (color [0] * 255),
		       int (color [1] * 255),
		       int (color [2] * 255));
	//m_background->setPaletteBackgroundColor (color1);
	QPalette newPalette;
	newPalette.setBrush(QPalette::Window, QBrush(color1));
	m_background->setPalette(newPalette);
	m_rgbMode->setChecked (m_source->getSceneManager ()->isRGBMode ());
	m_realTimeUpdate->setChecked (m_source->getSceneManager ()->isRealTimeUpdateEnabled ());    
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgSceneManagerCategory::setBackgroundColor (void)
{
    QColor color = 
	QColorDialog::getColor (m_background->paletteBackgroundColor (),
				m_source->getShellWidget (),
				"Select color");
    if (color.isValid ())
    {
	m_background->setPaletteBackgroundColor (color);
	setBackgroundColor (SbColor (color.red   () / 255.,
				     color.green () / 255.,
				     color.blue  () / 255.));
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgSceneManagerCategory::setBackgroundColor (const SbColor& color)
{ m_source->getSceneManager ()->setBackgroundColor (color); }

void
IgSceneManagerCategory::setRGBMode (bool option)
{
    bool old = m_source->getSceneManager ()->isRGBMode ();
    if (old != option)
	m_source->getSceneManager ()->setRGBMode (option);
}

void
IgSceneManagerCategory::setRealTimeUpdate (bool option)
{
    bool old = m_source->getSceneManager ()->isRealTimeUpdateEnabled ();
    if (old != option)
	m_source->getSceneManager ()->enableRealTimeUpdate (option);
}
