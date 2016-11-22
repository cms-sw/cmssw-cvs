//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisEventIdTwig.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/SbLinear.h>
#include <qstring.h>
#include <sstream>
#include <iomanip>
#include <classlib/utils/Error.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <sys/time.h> 

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisEventIdTwig::VisEventIdTwig (IgState *state, IgTwig *parent,
				const std::string &name /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text ("no info")
{}

void
VisEventIdTwig::configChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisEventIdTwig::onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup)
{    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);
    const edm::Timestamp time = event.time ();

    timeval eventTime;
    eventTime.tv_sec = time.value () >> 32;
    eventTime.tv_usec = 0xFFFFFFFF & time.value ();

    boost::posix_time::ptime bt0 = boost::posix_time::from_time_t(0);
    boost::posix_time::ptime bt = bt0 + boost::posix_time::seconds(eventTime.tv_sec)
      + boost::posix_time::microseconds(eventTime.tv_usec);

    std::cout << "Time: " << bt << std::endl;

    std::stringstream oss;
    oss << bt;
    std::string contents (oss.str ());

    m_text = (QString ("%1 GMT: Run %2, Event %3, LS %4, Orbit %5, BX %6")
	      .arg (contents.c_str ())
	      .arg (event.id ().run ())
	      .arg (event.id ().event ())
	      .arg (event.luminosityBlock ())
	      .arg (event.orbitNumber ())
	      .arg (event.bunchCrossing ())
	      .latin1 ());

    VisQueuedTwig::onBaseInvalidate ();
}

void
VisEventIdTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->setText (m_text);
}

void
VisEventIdTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

// //     QWidget *w = SoQt::getTopLevelWidget (void);
// //     SbVec2s view = SoQt::getWidgetSize (w);
    
//     // Set the viewport in the OpenGL canvas. Dimensions are calculated
//     // as a percentage of the total canvas size.
//     SbVec2s view = PUBLIC(this)->getGLSize();
//     const int pixelarea =
// 	int(float(this->axiscrossSize)/100.0f * SoQtMin(view[0], view[1]));
// #if 0 // middle of canvas
//     SbVec2s origin(view[0]/2 - pixelarea/2, view[1]/2 - pixelarea/2);
// #endif // middle of canvas
// #if 1 // lower right of canvas
//     SbVec2s origin(view[0] - pixelarea, 0);
// #endif // lower right of canvas
//     glViewport(origin[0], origin[1], pixelarea, pixelarea);

    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-11.0,
			   8.0,
			   0.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}

void
VisEventIdTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;    

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-8.0,
			   8.0,
			   0.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}

void
VisEventIdTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;    

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-11.0,
			   8.0,
			   -12.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}

void
VisEventIdTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;    

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-11.0,
			   7.0,
			   0.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}
