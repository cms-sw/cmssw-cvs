//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMagneticField/interface/VisCMSMagFieldTwig.h"
#include "VisReco/VisMagneticField/interface/VisCMSMagField.h"
#include "VisReco/VisMagneticField/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "Iguana/Inventor/interface/IgSoFieldPlane.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/GLBrowsers/interface/Ig3DFieldPlanesCategory.h"
#include "Iguana/GLModels/interface/IgCommon3DReps.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/xtypeinfo.h"
#include "Iguana/Models/interface/IgTextModel.h"
#include "Iguana/Models/interface/IgSimpleTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include <sstream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCMSMagFieldTwig::VisCMSMagFieldTwig (IgTwig *parent, IgState *state)
    : IgSimpleTwig (parent, "CMS Magnetic Field", true, false, true),
      m_fieldProxy (new VisCMSMagField),
      m_fieldPlane (new IgSoFieldPlane),
      m_done (false),
      m_fieldDone (false)
{
    Ig3DFieldPlanesCategory *fieldPlanesCategory = Ig3DFieldPlanesCategory::get (state);
    if (! fieldPlanesCategory)
	fieldPlanesCategory = new Ig3DFieldPlanesCategory (state);
    fieldPlanesCategory->registerMe (state);
    m_fieldPlane->ref ();
    fieldPlanesCategory->addOne (m_fieldPlane);
}

void
VisCMSMagFieldTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    LOG (1, trace, LFfwvis, "VisCMSMagFieldTwig[" << name ()
	 << "]::onNewEvent()\n");

    if (! m_done)
    {
	m_fieldProxy->setField (eventSetup);
	m_done = true;
    }

    IgRepSet::invalidate (this, SELF_MASK);
}

IgSoFieldPlane*
VisCMSMagFieldTwig::getFieldPlane ()
{ return m_fieldPlane; }

bool
VisCMSMagFieldTwig::fieldProxy (IgSbField *&field, SbBox3f &worldbox)
{
    // Get the current magnetic field
    // FIXME: Need to convert the cm to m somewhere.
    // The field is using cm.
    if (m_fieldProxy->valid ())
    {
	field = m_fieldProxy;
	//world dimensions: cube with each side 18+18 meters long
	SbBox3f world (-18.0,  // Xmin
		       -18.0,  // Ymin
		       -18.0,  // Zmin
		        18.0,  // Xmax
		        18.0,  // Ymax
		        18.0); // Zmax
	worldbox = world;
	m_fieldDone = true;
    }
    else
	field = 0;
    
    return field != 0;
}
 
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisCMSMagFieldTwig *twig, Ig3DRep *rep, unsigned mask))
{
    // Let base class implementation handle the basics.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    // Get the current magnetic field if twig is visible
    IgSbField           *field = 0;
    SbBox3f             worldbox;
    
    if ((! twig->visible () ) || (! twig->fieldProxy (field, worldbox)))
	return;

    IgSoFieldPlane      *repnode = twig->getFieldPlane ();
    repnode->field (field, worldbox);
    rep->node ()->addChild (repnode);
}
  
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisCMSMagFieldTwig *, IgTextRep *rep, unsigned))
{
    std::ostringstream text;
    text << "<table width='100%'><tr><td>Name</td><td>"
         << "CMS Magnetic Field</td></tr></table>\n";
    
    rep->setText (text.str ());
}
