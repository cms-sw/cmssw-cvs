//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4VolumeTwig.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Navigator.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4VolumePicker.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialCacheFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialMiscFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialDensityFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFalseColorFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Filter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4FilterCache.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialNameFilter.h"
#include "VisGeant4/VisG4Volumes/interface/xtypeinfo.h"
#include "VisGeant4/VisG4Core/interface/VisG4Path.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisSceneHandler.h"
#include "Iguana/GLModels/interface/IgCommon3DReps.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiModel.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZModel.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/xtypeinfo.h"
#include "Iguana/Models/interface/IgTextModel.h"
#include "Iguana/Models/interface/IgSimpleTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

#include <Inventor/nodes/SoMaterial.h>

#include <G4ModelingParameters.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Material.hh>
#include <G4VSensitiveDetector.hh>
#include <G4VSolid.hh>
#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4Torus.hh>
#include <G4Trap.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>

#include <sstream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

extern lat::logflag LFG4vis;

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4VolumeTwig::VisG4VolumeTwig (IgTwig	  *parent,
				G4VPhysicalVolume *volume,
				bool		  traverse /* = false */,
				bool		  visible /* = false */,
				bool		  open /* = false */,
				bool		  logic /* = true */)
    : IgCompoundTwig (parent),
      m_volume (volume)
{
    ASSERT (parent);
    ASSERT (m_volume);
    
    selfTraverse (traverse);
    selfVisible (visible);
    cascade (open);
    logical (logic);
    if (m_materialFilter == 0)
    {
	m_materialFilter = new VisG4MaterialCacheFilter (true);
	VisG4MaterialFilter *currentFilter = m_materialFilter;

	VisG4MaterialFilter *tmpFilter;
	
	if (! getenv ("DISABLE_LUT_FILTER"))
	{
	    tmpFilter = new VisG4MaterialLUTFilter (true);	   
	    currentFilter->appendFilter (tmpFilter);
	    currentFilter = tmpFilter;
	}
	
	if (getenv ("ENABLE_DENSITY_FILTER"))
	{
	    tmpFilter = new VisG4MaterialDensityFilter (true);
	    currentFilter->appendFilter (tmpFilter);
	    currentFilter = tmpFilter;
	}
	
	if (getenv ("ENABLE_FALSE_COLOR_FILTER"))
	{
	    tmpFilter = new VisG4MaterialFalseColorFilter (true);
	    currentFilter->appendFilter (tmpFilter);
	    currentFilter = tmpFilter;
	}
    } 
}

VisG4MaterialFilter *VisG4VolumeTwig::m_materialFilter = 0;
VisG4Filter *VisG4VolumeTwig::m_visualizationFilter = 0;

std::string
VisG4VolumeTwig::name (void) const
{ return m_volume->GetLogicalVolume ()->GetName (); }

void
VisG4VolumeTwig::name (const std::string &)
{ ASSERT (false); }

bool
VisG4VolumeTwig::expand (void)
{
    if (IgCompoundTwig::expand ())
	return true;

    VisG4Navigator::build (1, m_volume, this);
    return false;
}

#if 0
VisG4Path
VisG4VolumeTwig::path (void) const
{
    if (const IgTwig *p = parent ())
	return VisG4Path (p->path (), p->indexOf (this), 0);
    else
	return VisG4Path ();
}
#endif

VisG4VolumeTwig *
VisG4VolumeTwig::select (const VisG4Path &path, bool autoexpand /* = false */)
{
    if (path.empty ())
	return this;

    VisG4VolumeTwig *c = select (path.prefix (), autoexpand);

    if (! c->children () && autoexpand)
	c->expand ();

    unsigned n = path.daughter ();
    if (n < c->children ())
    {
	IgTwig		*sub = c->child (n);
	VisG4VolumeTwig	*subvol = static_cast<VisG4VolumeTwig *> (sub);
	ASSERT (dynamic_cast<VisG4VolumeTwig *> (sub));
	return subvol;
    }
    else
	return 0;
}

G4VPhysicalVolume *
VisG4VolumeTwig::volume (void) const
{ return m_volume; }

bool
VisG4VolumeTwig::selfTraverse (void) const
{ return flag (TRAVERSE); }

void
VisG4VolumeTwig::selfTraverse (bool value)
{ flag (TRAVERSE, value); }

bool
VisG4VolumeTwig::selfVisible (void) const
{ return flag (VISIBLE); }

void
VisG4VolumeTwig::selfVisible (bool value)
{ flag (VISIBLE, value); }

bool
VisG4VolumeTwig::cascade (void) const
{ return flag (CASCADE); }

void
VisG4VolumeTwig::cascade (bool value)
{ flag (CASCADE, value); }

// FIXME: Do these inherit?
bool
VisG4VolumeTwig::logical (void) const
{ return flag (LOGICAL); }

void
VisG4VolumeTwig::logical (bool value)
{
    if (flag (LOGICAL) != value)
    {
	// Flip representation
	destroy ();
	flag (LOGICAL, value);
	flag (EXPANDED, false);
    }
}

//////////////////////////////////////////////////////////////////////
unsigned
VisG4VolumeTwig::physicalChildren (void)
{
    if (! logical ())
	return children ();
    else if (flag (EXPANDED))
	return m_volume->GetLogicalVolume ()->GetNoDaughters ();
    else
	return 0;
}

IgTwig *
VisG4VolumeTwig::physicalChild (unsigned index)
{ return child (logical () ? VisG4Navigator::findLogical (m_volume, index)
		: index); }

//////////////////////////////////////////////////////////////////////
unsigned
VisG4VolumeTwig::paths (void)
{
    if (m_paths.empty ())
    {
	// We haven't yet computed the paths.  Do so now.
	if (VisG4VolumeTwig *p = dynamic_cast<VisG4VolumeTwig *> (parent ()))
	{
	    // Parent is a volume twig.  Expand on paths assigned to it.
	    G4VPhysicalVolume	*pvol = p->volume ();
	    int			nreps = 1;

	    if (m_volume->IsReplicated ())
	    {
		EAxis	 axis;
		G4double width, offset;
		G4bool	 consuming;
		m_volume->GetReplicationData (axis, nreps, width,
					      offset, consuming);
	    }

	    if (p->logical ())
		// Logical.  Push all subpaths with equal physical
		// volume, using G4 daughter volume index (each
		// replica).
		for (unsigned i = 0; i < p->paths (); ++i)
		{
		    VisG4Path		prefix = p->path (i);
		    G4LogicalVolume	*mother = pvol->GetLogicalVolume ();
		    G4LogicalVolume	*me = m_volume->GetLogicalVolume ();

		    for (int i = 0; i < mother->GetNoDaughters (); ++i)
			if (mother->GetDaughter (i)->GetLogicalVolume () == me)
			    for (int j = 0; j < nreps; ++j)
				m_paths.push_back (VisG4Path (prefix, i, j));
		}

	    else
		// Physical.  Push index of the G4 daughter volume
		// (each replica).
		for (unsigned i = 0, me = p->indexOf(this); i < p->paths();++i)
		    for (int j = 0; j < nreps; ++j)
			m_paths.push_back (VisG4Path (p->path (i), me, j));
	}
	else
	    m_paths.push_back (VisG4Path ());
    }

    // OK, now return the number of paths we have
    return m_paths.size ();
}

VisG4Path
VisG4VolumeTwig::path (unsigned index)
{
    ASSERT (m_paths.size () > index);
    return m_paths [index];
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
drawSolidParts (VisG4VisSceneHandler *handler, G4VSolid *solid, int depth = 0)
{
    G4VSolid *b0, *b1;

    if ((b0 = solid->GetConstituentSolid (0)))
    {
	VERIFY (b1 = solid->GetConstituentSolid (1));
	drawSolidParts (handler, b0, depth+1);
	drawSolidParts (handler, b1, depth+1);
    }

    if (depth > 0)
	solid->DescribeYourselfTo (*handler);
}

void
drawVolumeTwig (VisG4VolumeTwig *twig, Ig3DRep *rep)
{

    // Process each path assigned to the twig.  We create exactly one
    // SoSeparator for each path so it is easy to correlate the two
    // later on.  Note that the volume has a separate path for each
    // replica associated with it, not just path for every physical
    // volume copy.

    // Render the twig.  We create a temporary volume model and render
    // that to our special scene handler.  FIXME: Check that G4 thread
    // is available!  NB: This is executed in the GUI thread.

    // Zap any existing representations
    rep->clear ();

    // Dig out some common variables.
    VisG4VolumeTwig	*root = twig;
    while (VisG4VolumeTwig *x = dynamic_cast<VisG4VolumeTwig *> (root->parent()))
	root = x;

    VisG4VisSceneHandler	*handler = VisG4VisSceneHandler::instance ();
    G4VPhysicalVolume	*volume = root->volume ();

    ASSERT (handler);
    ASSERT (volume);
    ASSERT (root);

    // FIXME: This isn't really true -- there could be something else
    // in the view volume beyond the volumes.  At the moment we need
    // this only for G4Scale and markers, neither of which are likely
    // to be used, so this isn't a catastrophy.  Need to rethink this
    // part entirely.  (FIXME: Move to geometry update?)
    handler->defineBounds (volume->GetLogicalVolume ()
			   ->GetSolid ()->GetExtent ());

    for (unsigned i = 0; i < twig->paths (); ++i)
    {
	bool			replica = false;
	G4VPhysicalVolume	*pv = volume;
	G4LogicalVolume		*lv = 0;
	G4VSolid		*solid = 0;
	G4Transform3D		pos;

	VisG4VolumePicker::apply (twig->path (i), pv, lv, solid, pos, replica);

	ASSERT (solid);
	ASSERT (pv);
	ASSERT (lv);

	// Create a VisSoMaterialDetails structure which describes the current twig
	VisSoMaterialDetails details;
	details << VisG4MaterialDetails (lv->GetMaterial ());
	details << VisG4PVDetails (pv);
	details << VisG4PathDetails (twig->fullName ());	

	// Skip if the filter rejects
	if (VisG4VolumeTwig::m_visualizationFilter
	    && ! VisG4VolumeTwig::m_visualizationFilter->compute (details))
	    continue;

	SoSeparator *sep = new SoSeparator;
	rep->node ()->addChild (sep);

	// Set object context.
	handler->beginObject (sep, replica ? 0 : pv);

	// Add transformation for this object.
	handler->addTransform (pos);

	// To show boolean constituents in white wireframe:
	//   handler->AddStyle (G4ViewParameters::wireframe);
	//   handler->AddMaterial (G4Colour (1., 1., 1., 1.));
	//   drawSolidParts (handler, solid);
    
	// Add colour and styles.  (FIXME: Move to and use a real
	// visualisation preferences service, and make it cache
	// materials and other style information as much as possible
	// -- switching materials is very bad for render performance,
	// and what's worse, we tend to reuse the same material lot.)
	handler->addStyle (lv->GetVisAttributes ());

	// Get the G4 material and check if there is a SoMaterial already in the cache. 
	handler->addMaterial (VisG4VolumeTwig::m_materialFilter->getSoMaterial (details, 0));	

	// Draw the solid itself.
	solid->DescribeYourselfTo (*handler);
	
	// Quit object context.
	handler->endObject ();
    }
}

void
drawVolumeTwig (VisG4VolumeTwig *twig, IgRPhiRep *rep)
{

    // Process each path assigned to the twig.  We create exactly one
    // SoSeparator for each path so it is easy to correlate the two
    // later on.  Note that the volume has a separate path for each
    // replica associated with it, not just path for every physical
    // volume copy.

    // Render the twig.  We create a temporary volume model and render
    // that to our special scene handler.  FIXME: Check that G4 thread
    // is available!  NB: This is executed in the GUI thread.

    // Zap any existing representations
    rep->clear ();

    // Dig out some common variables.
    VisG4VolumeTwig	*root = twig;
    while (VisG4VolumeTwig *x = dynamic_cast<VisG4VolumeTwig *> (root->parent()))
	root = x;

    VisG4VisSceneHandler	*handler = VisG4VisSceneHandler::instance ();
    G4VPhysicalVolume	*volume = root->volume ();

    ASSERT (handler);
    ASSERT (volume);
    ASSERT (root);

    // FIXME: This isn't really true -- there could be something else
    // in the view volume beyond the volumes.  At the moment we need
    // this only for G4Scale and markers, neither of which are likely
    // to be used, so this isn't a catastrophy.  Need to rethink this
    // part entirely.  (FIXME: Move to geometry update?)
    handler->defineBounds (volume->GetLogicalVolume ()
			   ->GetSolid ()->GetExtent ());

    for (unsigned i = 0; i < twig->paths (); ++i)
    {
	bool			replica = false;
	G4VPhysicalVolume	*pv = volume;
	G4LogicalVolume		*lv = 0;
	G4VSolid		*solid = 0;
	G4Transform3D		pos;

	VisG4VolumePicker::apply (twig->path (i), pv, lv, solid, pos, replica);

	ASSERT (solid);
	ASSERT (pv);
	ASSERT (lv);

	// Create a VisSoMaterialDetails structure which describes the current twig
	VisSoMaterialDetails details;
	details << VisG4MaterialDetails (lv->GetMaterial ());
	details << VisG4PVDetails (pv);
	details << VisG4PathDetails (twig->fullName ());	

	// Skip if the filter rejects
	if (VisG4VolumeTwig::m_visualizationFilter
	    && ! VisG4VolumeTwig::m_visualizationFilter->compute (details))
	    continue;

	SoSeparator *sep = new SoSeparator;
	rep->node ()->addChild (sep);

	// Set object context.
	handler->beginObject (sep, replica ? 0 : pv);

	// Add transformation for this object.
	handler->addTransform (pos);

	// To show boolean constituents in white wireframe:
	//   handler->AddStyle (G4ViewParameters::wireframe);
	//   handler->AddMaterial (G4Colour (1., 1., 1., 1.));
	//   drawSolidParts (handler, solid);
    
	// Add colour and styles.  (FIXME: Move to and use a real
	// visualisation preferences service, and make it cache
	// materials and other style information as much as possible
	// -- switching materials is very bad for render performance,
	// and what's worse, we tend to reuse the same material lot.)
	handler->addStyle (lv->GetVisAttributes ());

	// Get the G4 material and check if there is a SoMaterial already in the cache. 
	handler->addMaterial (VisG4VolumeTwig::m_materialFilter->getSoMaterial (details, 0));	

	// Draw the solid itself.
	solid->DescribeYourselfTo (*handler);
	
	// Quit object context.
	handler->endObject ();
    }
}

void
drawVolumeTwig (VisG4VolumeTwig *twig, IgRZRep *rep)
{

    // Process each path assigned to the twig.  We create exactly one
    // SoSeparator for each path so it is easy to correlate the two
    // later on.  Note that the volume has a separate path for each
    // replica associated with it, not just path for every physical
    // volume copy.

    // Render the twig.  We create a temporary volume model and render
    // that to our special scene handler.  FIXME: Check that G4 thread
    // is available!  NB: This is executed in the GUI thread.

    // Zap any existing representations
    rep->clear ();

    // Dig out some common variables.
    VisG4VolumeTwig	*root = twig;
    while (VisG4VolumeTwig *x = dynamic_cast<VisG4VolumeTwig *> (root->parent()))
	root = x;

    VisG4VisSceneHandler	*handler = VisG4VisSceneHandler::instance ();
    G4VPhysicalVolume	*volume = root->volume ();

    ASSERT (handler);
    ASSERT (volume);
    ASSERT (root);

    // FIXME: This isn't really true -- there could be something else
    // in the view volume beyond the volumes.  At the moment we need
    // this only for G4Scale and markers, neither of which are likely
    // to be used, so this isn't a catastrophy.  Need to rethink this
    // part entirely.  (FIXME: Move to geometry update?)
    handler->defineBounds (volume->GetLogicalVolume ()
			   ->GetSolid ()->GetExtent ());

    for (unsigned i = 0; i < twig->paths (); ++i)
    {
	bool			replica = false;
	G4VPhysicalVolume	*pv = volume;
	G4LogicalVolume		*lv = 0;
	G4VSolid		*solid = 0;
	G4Transform3D		pos;

	VisG4VolumePicker::apply (twig->path (i), pv, lv, solid, pos, replica);

	ASSERT (solid);
	ASSERT (pv);
	ASSERT (lv);

	// Create a VisSoMaterialDetails structure which describes the current twig
	VisSoMaterialDetails details;
	details << VisG4MaterialDetails (lv->GetMaterial ());
	details << VisG4PVDetails (pv);
	details << VisG4PathDetails (twig->fullName ());	

	// Skip if the filter rejects
	if (VisG4VolumeTwig::m_visualizationFilter
	    && ! VisG4VolumeTwig::m_visualizationFilter->compute (details))
	    continue;

	SoSeparator *sep = new SoSeparator;
	rep->node ()->addChild (sep);

	// Set object context.
	handler->beginObject (sep, replica ? 0 : pv);

	// Add transformation for this object.
	handler->addTransform (pos);

	// To show boolean constituents in white wireframe:
	//   handler->AddStyle (G4ViewParameters::wireframe);
	//   handler->AddMaterial (G4Colour (1., 1., 1., 1.));
	//   drawSolidParts (handler, solid);
    
	// Add colour and styles.  (FIXME: Move to and use a real
	// visualisation preferences service, and make it cache
	// materials and other style information as much as possible
	// -- switching materials is very bad for render performance,
	// and what's worse, we tend to reuse the same material lot.)
	handler->addStyle (lv->GetVisAttributes ());

	// Get the G4 material and check if there is a SoMaterial already in the cache. 
	handler->addMaterial (VisG4VolumeTwig::m_materialFilter->getSoMaterial (details, 0));	

	// Draw the solid itself.
	solid->DescribeYourselfTo (*handler);
	
	// Quit object context.
	handler->endObject ();
    }
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4VolumeTwig *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).
    if (twig->visible () &&
	(((mask & IgTwig::FLAGS_MASK)
	  && rep->node ()->getNumChildren () == 0)
	 || (mask & IgTwig::SELF_MASK)))
	drawVolumeTwig (twig, rep);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4VolumeTwig *twig, IgRPhiRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).
    if (twig->visible () &&
	(((mask & IgTwig::FLAGS_MASK)
	  && rep->node ()->getNumChildren () == 0)
	 || (mask & IgTwig::SELF_MASK)))
	drawVolumeTwig (twig, rep);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4VolumeTwig *twig, IgRZRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).
    if (twig->visible () &&
	(((mask & IgTwig::FLAGS_MASK)
	  && rep->node ()->getNumChildren () == 0)
	 || (mask & IgTwig::SELF_MASK)))
	drawVolumeTwig (twig, rep);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4VolumeTwig *twig, IgTextRep *rep, unsigned))
{
    // FIXME: HTML-quote text!
    // FIXME: Add physical volume information
    G4LogicalVolume	*lv = twig->volume ()->GetLogicalVolume ();
    G4VSolid		*solid = lv->GetSolid ();
    G4Material		*material = lv->GetMaterial ();
    G4RotationMatrix	*r = twig->volume ()->GetRotation ();
    G4ThreeVector	t = twig->volume ()->GetTranslation ();

    std::ostringstream text;

    text << "<table width='100%'><tr><td>Name</td><td>"
	 << lv->GetName ()
	 << "</td><tr><td>Copy #</td><td>"
	 << twig->volume ()->GetCopyNo ()
	 << "</td><tr><td>Replicated</td><td>"
	 << (twig->volume ()->IsReplicated () ? "Yes" : "No")
	// FIXME: show parametrisation info

	 << "</td><tr><td>Position</td><td>"
	 << "(" << t.x () << ", " << t.y () << ", " << t.z () << ")"
	 << "</td><tr><td>Rotation</td><td>";

    if (r)
	r->print (text);
    else
	text << "(None)";

    text << "</td><tr><td>Material</td><td>"
	 << (material ? material->GetName () : G4String ("(None)"))
	 << "</td></tr><tr><td>Density</td><td>"
	 << (material ? material->GetDensity () / g*cm*cm*cm : 0.)
	 << "</td></tr><tr><td>Sensitive</td><td>"
	 << (lv->GetSensitiveDetector ()
	     ? lv->GetSensitiveDetector ()->GetFullPathName ()
	     : G4String ("(not sensitive)"))
	 << "</td></tr><tr><td>Solid Type</td><td>"
	 << solid->GetEntityType ()
	 << "</td></tr><tr><td>Solid Name</td><td>"
	 << solid->GetName ()
	 << "</td></tr><tr><td>Solid Parameters</td><td>&nbsp;</td></tr>";

    // FIXME: This should be encapsulated, and use something like
    // Martin Liendl's SolidAnalyser or (why not?) a multimethod.
    if (G4Box *box = dynamic_cast<G4Box *> (solid))
	text << "<tr><td>&nbsp;&nbsp;x/2</td><td>"
	     << box->GetXHalfLength () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;y/2</td><td>"
	     << box->GetYHalfLength () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;z/2</td><td>"
	     << box->GetZHalfLength () << "</td></tr>";
    else if (G4Cons *cons = dynamic_cast<G4Cons *> (solid))
	text << "<tr><td>&nbsp;&nbsp;z/2</td><td>"
	     << cons->GetZHalfLength () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;rInZ-</td><td>"
	     << cons->GetInnerRadiusMinusZ () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;rInZ+</td><td>"
	     << cons->GetInnerRadiusPlusZ () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;rOutZ-</td><td>"
	     << cons->GetOuterRadiusMinusZ () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;rOutZ+</td><td>"
	     << cons->GetOuterRadiusPlusZ () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;startPhi</td><td>"
	     << cons->GetStartPhiAngle () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;deltaPhi</td><td>"
	     << cons->GetDeltaPhiAngle () << "</td></tr>";
    else if (G4Polycone *pcon = dynamic_cast<G4Polycone *> (solid))
    {
	int nr = pcon->GetNumRZCorner();   
	text << "<tr><td>&nbsp;&nbsp;startPhi</td><td>"
	     << pcon->GetStartPhi () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;endPhi</td><td>"
	     << pcon->GetEndPhi () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;nrRZ</td><td>"
	     << nr << "</td></tr>";

	for (int i = 0; i < nr; ++i)
	    text << "<tr><td>&nbsp;&nbsp;z" << i << "</td><td>"
		 << pcon->GetCorner (i).z << "</td></tr>"
		 << "<tr><td>&nbsp;&nbsp;r" << i << "</td><td>"
		 << pcon->GetCorner (i).r << "</td></tr>";
    }
    else if (G4Polyhedra *phed = dynamic_cast<G4Polyhedra *> (solid))
    {
	int nr = phed->GetNumRZCorner();   
	text << "<tr><td>&nbsp;&nbsp;startPhi</td><td>"
	     << phed->GetStartPhi () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;endPhi</td><td>"
	     << phed->GetEndPhi () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;sides</td><td>"
	     << phed->GetNumSide () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;nrRZ</td><td>"
	     << nr << "</td></tr>";

	for (int i = 0; i < nr; ++i)
	    text << "<tr><td>&nbsp;&nbsp;z" << i << "</td><td>"
		 << phed->GetCorner (i).z << "</td></tr>"
		 << "<tr><td>&nbsp;&nbsp;r" << i << "</td><td>"
		 << phed->GetCorner (i).r << "</td></tr>";
    }
    else if (G4Torus *torus = dynamic_cast<G4Torus *> (solid))
	text << "<tr><td>&nbsp;&nbsp;R min</td><td>"
	     << torus->GetRmin () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;R max</td><td>"
	     << torus->GetRmax () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;R tor</td><td>"
	     << torus->GetRtor () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;start Phi (radians)</td><td>"
	     << torus->GetSPhi () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;Delta angle of the segment (radians)</td><td>"
	     << torus->GetDPhi () << "</td></tr>";
    else if (G4Trap *trap = dynamic_cast<G4Trap *> (solid))
	text << "<tr><td>&nbsp;&nbsp;z/2</td><td>"
	     << trap->GetZHalfLength () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;x1/2</td><td>"
	     << trap->GetXHalfLength1 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;x2/2</td><td>"
	     << trap->GetXHalfLength2 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;y1/2</td><td>"
	     << trap->GetYHalfLength1 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;tanAlpha_1</td><td>"
	     << trap->GetTanAlpha1 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;x3/2</td><td>"
	     << trap->GetXHalfLength3 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;x4/2</td><td>"
	     << trap->GetXHalfLength4 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;y2/2</td><td>"
	     << trap->GetYHalfLength2 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;tanAlpha_2</td><td>"
	     << trap->GetTanAlpha2 () << "</td></tr>";
    else if (G4Trd *trd = dynamic_cast<G4Trd *> (solid))
	text << "<tr><td>&nbsp;&nbsp;z/2</td><td>"
	     << trd->GetZHalfLength () << "</td></tr>"

	     << "<tr><td>&nbsp;&nbsp;x1/2</td><td>"
	     << trd->GetXHalfLength1 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;x2/2</td><td>"
	     << trd->GetXHalfLength2 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;y1/2</td><td>"
	     << trd->GetYHalfLength1 () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;y2/2</td><td>"
	     << trd->GetYHalfLength2 () << "</td></tr>";
    else if (G4Tubs *tubs = dynamic_cast<G4Tubs *> (solid))
	text << "<tr><td>&nbsp;&nbsp;z/2</td><td>"
	     << tubs->GetZHalfLength () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;rIn</td><td>"
	     << tubs->GetInnerRadius () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;rOut</td><td>"
	     << tubs->GetOuterRadius () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;startPhi</td><td>"
	     << tubs->GetStartPhiAngle () << "</td></tr>"
	     << "<tr><td>&nbsp;&nbsp;deltaPhi</td><td>"
	     << tubs->GetDeltaPhiAngle () << "</td></tr>";
    else
	text << "<tr><td>&nbsp;&nbsp;(not known)</td><td>&nbsp;</td></tr>";

    text << "</table>";

    rep->setText (text.str ());
}
