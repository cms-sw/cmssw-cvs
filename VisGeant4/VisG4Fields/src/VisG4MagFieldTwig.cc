//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Fields/interface/VisG4MagFieldTwig.h"
#include "VisGeant4/VisG4Fields/interface/VisG4MagField.h"
#include "VisGeant4/VisG4Fields/interface/xtypeinfo.h"
#include "Iguana/GLModels/interface/IgCommon3DReps.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/xtypeinfo.h"
#include "Iguana/Models/interface/IgTextModel.h"
#include "Iguana/Models/interface/IgSimpleTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include "Iguana/Inventor/interface/IgSoFieldPlane.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/GLBrowsers/interface/Ig3DFieldPlanesCategory.h"
#include <G4TransportationManager.hh>
#include <G4FieldManager.hh>
#include <G4VisExtent.hh>
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

VisG4MagFieldTwig::VisG4MagFieldTwig (IgTwig *parent, IgState *state)
    : IgSimpleTwig (parent, "G4 Magnetic Field", true, false, true),
      m_fieldProxy (new VisG4MagField),
      m_fieldPlane (new IgSoFieldPlane)
{
    Ig3DFieldPlanesCategory *fieldPlanesCategory = Ig3DFieldPlanesCategory::get (state);
    if (! fieldPlanesCategory)
	fieldPlanesCategory = new Ig3DFieldPlanesCategory (state);
    fieldPlanesCategory->registerMe (state);
    m_fieldPlane->ref();
    fieldPlanesCategory->addOne (m_fieldPlane);
}

IgSoFieldPlane*
VisG4MagFieldTwig::getFieldPlane ()
{ return m_fieldPlane; }

bool
VisG4MagFieldTwig::fieldProxy (IgSbField *&field, SbBox3f &worldbox)
{
    // Get the current magnetic field
    G4FieldManager	*fieldMgr = G4TransportationManager
				    ::GetTransportationManager ()
				    ->GetFieldManager ();
    const G4Field	*baseField = fieldMgr ? fieldMgr->GetDetectorField () : 0;
    G4VPhysicalVolume	*worldVolume = G4TransportationManager
				       ::GetTransportationManager ()
				       ->GetNavigatorForTracking ()
				       ->GetWorldVolume ();

    if (baseField && worldVolume)
    {
	m_fieldProxy->setField (baseField);

	G4VisExtent	dims (worldVolume->GetLogicalVolume ()
			      ->GetSolid ()->GetExtent ());
	SbBox3f		world (dims.GetXmin () / m,
			       dims.GetYmin () / m,
			       dims.GetZmin () / m,
			       dims.GetXmax () / m,
			       dims.GetYmax () / m,
			       dims.GetZmax () / m);

	field = m_fieldProxy;
	worldbox = world;
    }
    else
	field = 0;

    return field != 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4MagFieldTwig *twig, Ig3DRep *rep, unsigned mask))
{
    // Let base class implementation handle the basics.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    // Get the current magnetic field if twig is visible
    IgSbField           *field = 0;
    SbBox3f             worldbox;
    
    if (! twig->visible () || ! twig->fieldProxy (field, worldbox))
        return;
    
    IgSoFieldPlane      *repnode = twig->getFieldPlane ();
    repnode->field (field, worldbox);
    rep->node ()->addChild (repnode);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4MagFieldTwig *, IgTextRep *rep, unsigned))
{
    std::ostringstream text;
    text << "<table width='100%'><tr><td>Name</td><td>"
	 << "G4 Magnetic Field</td></tr></table>\n";

    rep->setText (text.str ());
}
