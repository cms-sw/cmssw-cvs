#include "Iguana/GLBrowsers/interface/IgExaminerViewerCategory.h"
#include "Iguana/GLBrowsers/interface/IgQtRangeControlFloat.h"
#include "Iguana/GLBrowsers/interface/IgQtRangeControlUInt.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>

IG_DEFINE_STATE_ELEMENT (IgExaminerViewerCategory, "/ControlCentre/Categories/View Properties/Examiner Viewer");

IgExaminerViewerCategory::IgExaminerViewerCategory (IgState *state)
    : IgCommonViewCategory (state, "Examiner Viewer"),
      m_stillDrawStyle	(0),
      m_interactiveDrawStyle(0),
      m_bufferType	(0),
      m_seekTime	(0),
      m_stereoOffset	(0),
      m_feedbackSize	(0),
      m_autoClipping	(0),
      m_animation	(0),
      m_stereoViewing	(0),
      m_detailSeek	(0),
      m_cursor		(0)
{
    addExaminerViewerTab ();
    state->put (s_key, this);    
}

IgExaminerViewerCategory::~IgExaminerViewerCategory (void)
{ state()->detach (s_key); }

/****************************************************************************
** Form implementation generated from reading ui file 'IgExaminerViewerTab.ui'
**
** Created: Fri Nov 7 10:49:16 2003
**      by: The User Interface Compiler ($Id: IgExaminerViewerCategory.cc,v 1.3 2010/01/11 16:37:04 yana Exp $)
****************************************************************************/
void
IgExaminerViewerCategory::addExaminerViewerTab (void)
{
    QWidget	*exViewerOptions = new QWidget (topArea ());
    topLayout ()->addWidget (exViewerOptions);
    
    QVBoxLayout	*exViewerVLayout  = new QVBoxLayout (exViewerOptions);
    QGridLayout	*exViewerLayout   = new QGridLayout (exViewerVLayout);

    QGridLayout *layout14 = new QGridLayout (0, 1, 1, 0, 6, "layout14"); 

    QLabel *stillDrawLabel = new QLabel (exViewerOptions, "stillDrawLabel");
    stillDrawLabel->setText (tr ("Still Draw Style"));

    layout14->addWidget (stillDrawLabel, 0, 0);

    m_stillDrawStyle = new QComboBox (FALSE, exViewerOptions, "m_stillDrawStyle");
    m_stillDrawStyle->clear ();
    m_stillDrawStyle->insertItem (tr ("As is"));
    m_stillDrawStyle->insertItem (tr ("Hidden line"));
    m_stillDrawStyle->insertItem (tr ("Wireframe overlay"));
    m_stillDrawStyle->insertItem (tr ("No texture"));
    m_stillDrawStyle->insertItem (tr ("Low resolution"));
    m_stillDrawStyle->insertItem (tr ("Wireframe"));
    m_stillDrawStyle->insertItem (tr ("Points"));
    m_stillDrawStyle->insertItem (tr ("Bounding box (no depth)"));

    layout14->addWidget (m_stillDrawStyle, 0, 1);

    QLabel *interDrawLabel = new QLabel (exViewerOptions, "interDrawLabel");
    interDrawLabel->setText (tr ("Interactive Draw Style"));

    layout14->addWidget (interDrawLabel, 1, 0);

    m_interactiveDrawStyle = new QComboBox (FALSE, exViewerOptions, "m_interactiveDrawStyle");
    m_interactiveDrawStyle->clear ();
    m_interactiveDrawStyle->insertItem (tr ("Same as still"));
    m_interactiveDrawStyle->insertItem (tr ("No texture"));
    m_interactiveDrawStyle->insertItem (tr ("Low resolution"));
    m_interactiveDrawStyle->insertItem (tr ("Wireframe"));
    m_interactiveDrawStyle->insertItem (tr ("Low resolution wireframe (no depth)"));
    m_interactiveDrawStyle->insertItem (tr ("Points"));
    m_interactiveDrawStyle->insertItem (tr ("Low resolution points (no depth)"));
    m_interactiveDrawStyle->insertItem (tr ("Bounding box (no depth)"));

    layout14->addWidget (m_interactiveDrawStyle, 1, 1);

    QLabel *bufferLabel = new QLabel (exViewerOptions, "bufferLabel");
    bufferLabel->setText (tr ("Render Buffer Type"));

    layout14->addWidget (bufferLabel, 2, 0);

    m_bufferType = new QComboBox( FALSE, exViewerOptions, "m_bufferType" );
    m_bufferType->clear ();
    m_bufferType->insertItem (tr ("Single"));
    m_bufferType->insertItem (tr ("Double"));
    m_bufferType->insertItem (tr ("Interactive"));

    layout14->addWidget( m_bufferType, 2, 1 );

    QLabel *seekTimeLabel = new QLabel(exViewerOptions, "seekTimeLabel" );
    seekTimeLabel->setText (tr ("Seek Time"));

    layout14->addWidget (seekTimeLabel, 3, 0);

    m_seekTime = new IgQtRangeControlFloat (exViewerOptions,
				       IgQtRangeControl::StepperPart
				       | IgQtRangeControl::ValuePart); 
    m_seekTime->setMinValue (0.);
    m_seekTime->setLongStep (1.);
    m_seekTime->setSmallStep (.1);

    layout14->addWidget (m_seekTime->widget (), 3, 1);

    m_stereoViewing = new QCheckBox (exViewerOptions, "m_stereoViewing");
    m_stereoViewing->setText (tr ("Stereo Viewing Offset"));

    layout14->addWidget (m_stereoViewing, 4, 0);

    m_stereoOffset = new IgQtRangeControlFloat (exViewerOptions,
                                           IgQtRangeControl::StepperPart
				           | IgQtRangeControl::ValuePart);
    m_stereoOffset->setLongStep (1.);
    m_stereoOffset->setSmallStep (.1);
    m_stereoOffset->widget ()->setEnabled (false);

    layout14->addWidget (m_stereoOffset->widget (), 4, 1);

    QLabel *feedbackaxis = new QLabel(exViewerOptions, "feedbackaxis" );
    feedbackaxis->setText (tr ("Feedback Axis"));

    layout14->addWidget (feedbackaxis, 5, 0);
    m_feedbackSize = new IgQtRangeControlUInt (exViewerOptions,
					   IgQtRangeControl::StepperPart
					   | IgQtRangeControl::ValuePart);
    m_feedbackSize->setMinValue (1);
    m_feedbackSize->setLongStep (5);
    m_feedbackSize->setSmallStep (1);

    layout14->addWidget (m_feedbackSize->widget (), 5, 1);

    exViewerLayout->addLayout (layout14, 0, 0);

    QGridLayout *layout15 = new QGridLayout (0, 1, 1, 0, 6, "layout15"); 

    m_autoClipping = new QCheckBox (exViewerOptions, "m_autoClipping");
    m_autoClipping->setText (tr ("Auto Clipping"));

    layout15->addWidget (m_autoClipping, 0, 0);

    m_animation = new QCheckBox (exViewerOptions, "m_animation");
    m_animation->setText (tr ("3D Animation"));
    layout15->addWidget (m_animation, 0, 1);

    m_cursor = new QCheckBox (exViewerOptions, "m_cursor");
    m_cursor->setText (tr ("Mouse Cursor"));
    layout15->addWidget (m_cursor, 1, 0);

    m_detailSeek = new QCheckBox (exViewerOptions, "m_detailSeek");
    m_detailSeek->setText (tr ("Detail Seek"));
    layout15->addWidget (m_detailSeek, 1, 1);

    exViewerLayout->addLayout (layout15, 1, 0);
    exViewerVLayout->addItem (new QSpacerItem ( 0, 0, QSizePolicy::Expanding,
						    QSizePolicy::Expanding));

    connect (m_interactiveDrawStyle,	SIGNAL(activated (int)),
             this,		    	SLOT  (setInteractiveDrawStyle (int)));
    connect (m_stillDrawStyle,		SIGNAL(activated (int)),
             this,			SLOT  (setStillDrawStyle (int)));
    connect (m_bufferType,		SIGNAL(activated (int)),
             this,			SLOT  (setBufferType (int)));
    connect (m_seekTime,		SIGNAL(newValue (float)),
             this,			SLOT  (setSeekTime (float)));
    connect (m_stereoOffset,		SIGNAL(newValue (float)),
             this,			SLOT  (setStereoOffset (float)));
    connect (m_feedbackSize,		SIGNAL(newValue (unsigned int)),
             this,			SLOT  (setFeedbackSize (unsigned int)));
    connect (m_autoClipping,		SIGNAL(toggled (bool)),
             this,			SLOT  (setAutoClipping (bool)));
    connect (m_animation,		SIGNAL(toggled (bool)),
             this,			SLOT  (setAnimation (bool)));
    connect (m_stereoViewing,		SIGNAL(toggled (bool)),
             this,			SLOT  (setStereoViewing (bool)));
    connect (m_stereoViewing,		SIGNAL(toggled (bool)),
             m_stereoOffset->widget (),	SLOT  (setEnabled (bool)));
    connect (m_detailSeek,		SIGNAL(toggled (bool)),
             this,			SLOT  (setDetailSeek (bool)));
    connect (m_cursor,			SIGNAL(toggled (bool)),
             this,			SLOT  (setCursor (bool)));
}

void 
IgExaminerViewerCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
    {
	IgCommonViewCategory::browserChanged (source);
	updateStillDrawStyle (m_source->getDrawStyle (SoQtViewer::STILL));
	updateInteractiveDrawStyle (m_source->getDrawStyle (SoQtViewer::INTERACTIVE));
	updateBufferType (m_source->getBufferingType ());    
	m_seekTime->setValue (m_source->getSeekTime ());
	m_stereoOffset->setValue (m_source->getStereoOffset ());
	m_feedbackSize->setValue (m_source->getFeedbackSize ());
	m_autoClipping->setChecked (m_source->isAutoClipping ());
	m_animation->setChecked (m_source->isAnimationEnabled ());
	m_stereoViewing->setChecked (m_source->isStereoViewing ());
	m_detailSeek->setChecked (m_source->isDetailSeek ());
	m_cursor->setChecked (m_source->isCursorEnabled ());
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgExaminerViewerCategory::setStillDrawStyle (int style)
{
    switch (style)
    {
    case 0:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_AS_IS);
	break;
    case 1:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_HIDDEN_LINE);
        break;
    case 2:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_WIREFRAME_OVERLAY);
        break;
    case 3:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_NO_TEXTURE);
        break;
    case 4:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_LOW_COMPLEXITY);
        break;
    case 5:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_LINE);
        break;
    case 6:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_POINT);
        break;
    case 7:
        m_source->setDrawStyle (SoQtViewer::STILL,
	                        SoQtViewer::VIEW_BBOX);
        break;
    default:
        assert (false);
    }
}

void
IgExaminerViewerCategory::updateStillDrawStyle (int style)
{
    switch (style)
    {
    case SoQtViewer::VIEW_AS_IS:
        m_stillDrawStyle->setCurrentItem (0);
        break;
    case SoQtViewer::VIEW_HIDDEN_LINE:
        m_stillDrawStyle->setCurrentItem (1);
        break;
    case SoQtViewer::VIEW_WIREFRAME_OVERLAY:
        m_stillDrawStyle->setCurrentItem (2);
        break;
    case SoQtViewer::VIEW_NO_TEXTURE:
        m_stillDrawStyle->setCurrentItem (3);
        break;
    case SoQtViewer::VIEW_LOW_COMPLEXITY:
        m_stillDrawStyle->setCurrentItem (4);
        break;
    case SoQtViewer::VIEW_LINE:
        m_stillDrawStyle->setCurrentItem (5);
        break;
    case SoQtViewer::VIEW_POINT:
        m_stillDrawStyle->setCurrentItem (6);
        break;
    case SoQtViewer::VIEW_BBOX:
        m_stillDrawStyle->setCurrentItem (7);
        break;
    default:
        assert (false);
    }
}

void
IgExaminerViewerCategory::setInteractiveDrawStyle (int style)
{
    switch (style)
    {
    case 0:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_SAME_AS_STILL);
        break;
    case 1:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_NO_TEXTURE);
        break;
    case 2:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_LOW_COMPLEXITY);
        break;
    case 3:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_LINE);
        break;
    case 4:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_LOW_RES_LINE);
        break;
    case 5:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_POINT);
        break;
    case 6:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_LOW_RES_POINT);
        break;
    case 7:
        m_source->setDrawStyle (SoQtViewer::INTERACTIVE,
	                        SoQtViewer::VIEW_BBOX);
        break;
    default:
        assert (false);
    }
}

void
IgExaminerViewerCategory::updateInteractiveDrawStyle (int style)
{
    switch (style)
    {
    case SoQtViewer::VIEW_SAME_AS_STILL:
        m_interactiveDrawStyle->setCurrentItem (0);
        break;
    case SoQtViewer::VIEW_NO_TEXTURE:
        m_interactiveDrawStyle->setCurrentItem (1);
        break;
    case SoQtViewer::VIEW_LOW_COMPLEXITY:
        m_interactiveDrawStyle->setCurrentItem (2);
        break;
    case SoQtViewer::VIEW_LINE:
        m_interactiveDrawStyle->setCurrentItem (3);
        break;
    case SoQtViewer::VIEW_LOW_RES_LINE:
        m_interactiveDrawStyle->setCurrentItem (4);
        break;
    case SoQtViewer::VIEW_POINT:
        m_interactiveDrawStyle->setCurrentItem (5);
        break;
    case SoQtViewer::VIEW_LOW_RES_POINT:
        m_interactiveDrawStyle->setCurrentItem (6);
        break;
    case SoQtViewer::VIEW_BBOX:
        m_interactiveDrawStyle->setCurrentItem (7);
        break;
    default:
        assert (false);
    }
}

void
IgExaminerViewerCategory::setBufferType (int type)
{
    switch (type)
    {
    case 0:
        m_source->setBufferingType (SoQtViewer::BUFFER_SINGLE);
        break;
    case 1:
        m_source->setBufferingType (SoQtViewer::BUFFER_DOUBLE);
        break;
    case 2:
        m_source->setBufferingType (SoQtViewer::BUFFER_INTERACTIVE);
        break;
    default:
        assert (false);
    }
}

void
IgExaminerViewerCategory::updateBufferType (int type)
{
    switch (type)
    {
    case SoQtViewer::BUFFER_SINGLE:
        m_bufferType->setCurrentItem (0);
        break;
    case SoQtViewer::BUFFER_DOUBLE:
        m_bufferType->setCurrentItem (1);
        break;
    case SoQtViewer::BUFFER_INTERACTIVE:
        m_bufferType->setCurrentItem (2);
        break;
    default:
        assert (false);
    }
}

void
IgExaminerViewerCategory::setSeekTime (float newValue)
{ m_source->setSeekTime (newValue); }

void
IgExaminerViewerCategory::setStereoOffset (float newValue)
{ m_source->setStereoOffset (newValue); }


void
IgExaminerViewerCategory::setFeedbackSize (unsigned int newValue)
{ m_source->setFeedbackSize (int (newValue)); }

void
IgExaminerViewerCategory::setAutoClipping (bool option)
{ 
    bool old = m_source->isAutoClipping ();
    if (old != option)
	m_source->setAutoClipping (option);
}

void
IgExaminerViewerCategory::setAnimation (bool option)
{ 
    bool old = m_source->isAnimationEnabled ();
    if (old != option)
	m_source->setAnimationEnabled (option);
}

void
IgExaminerViewerCategory::setStereoViewing (bool option)
{ 
    bool old = m_source->isStereoViewing ();
    if (old != option)
	m_source->setStereoViewing (option);
}

void
IgExaminerViewerCategory::setDetailSeek (bool option)
{ 
    bool old = m_source->isDetailSeek ();
    if (old != option)
	m_source->setDetailSeek (option);
}

void
IgExaminerViewerCategory::setCursor (bool option)
{ 
    bool old = m_source->isCursorEnabled ();
    if (old != option)
	m_source->setCursorEnabled (option);
}
