//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisCustomTracker/interface/VisCuDetectorRep.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"
#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include "Iguana/Inventor/interface/IgSoCube.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Transform and add the DetUnit to the scene graph */
void
VisCuDetectorRep::addWafer (SoSeparator *separator, const GeomDetUnit *det, DetectorRep type)
{    
  // position of the wafer's center
  float posx = det->surface ().position ().x () / 100.0;  // cm -> m
  float posy = det->surface ().position ().y () / 100.0;  // cm -> m
  float posz = det->surface ().position ().z () / 100.0;  // cm -> m
  if (type == RZDet) 
  {
    posx = 0;
    posy = sqrt (det->surface ().position ().x () * det->surface ().position ().x () 
		 + det->surface ().position ().y () * det->surface ().position ().y ()) / 100.0; 
    if (det->surface ().position ().y () < 0.0) posy = - posy;
  }
  float angle;
  SbVec3f axis;
  createRotation (det, axis, angle);
  SoTransform *transform = new SoTransform;
  transform->translation.setValue (posx, posy, posz);
  if (type == RZDet)
  {
    int subdet = det->geographicalId ().subdetId (); 
    if (subdet == 1 || subdet == 3 || subdet == 5) //barrel
    {
      SbMatrix sbm (1, 0, 0, 0,
		    0, 0, -1, 0,
		    0, 1, 0, 0,
		    0, 0, 0, 1);
      SbRotation sbr (sbm);
      sbr.getValue (axis, angle);
      transform->rotation.setValue (axis, angle);
    }  
    else 
      transform->rotation.setValue (axis, angle);
  }  
  else 
    transform->rotation.setValue (axis, angle);

  // Trapezoidal
  const Bounds *b = &((det->surface ()).bounds ());
  if (dynamic_cast<const TrapezoidalPlaneBounds *> (b))
  {
    const TrapezoidalPlaneBounds *b2 = dynamic_cast<const TrapezoidalPlaneBounds *> (b);
    std::vector< float > par = b2->parameters ();
	
    // these parameters are half-lengths, as in CMSIM/GEANT3
    float hBottomEdge = par [0];
    float hTopEdge    = par [1];
    float thickness   = par [2];
    float apothem     = par [3];

    IgSoG4Trap *g4Trap = new IgSoG4Trap ();
    g4Trap->dz = thickness / 100.; 
    g4Trap->dy1 = apothem / 100.;
    g4Trap->dy2 = apothem / 100.;
    g4Trap->dx1 = hBottomEdge / 100.;
    g4Trap->dx2 = hTopEdge / 100.;
    g4Trap->dx3 = hBottomEdge / 100.;
    g4Trap->dx4 = hTopEdge / 100.;
    g4Trap->alp1 = 0.0;
    g4Trap->alp2 = 0.0;
    separator->addChild (transform);
    separator->addChild (g4Trap);
  }
  if (dynamic_cast<const RectangularPlaneBounds *> (b))
  {
    IgSoCube *cube = new IgSoCube ();
	    
    // Rectangular
    float length = det->surface ().bounds ().length () / 100.0; // cm -> m
    float width = det->surface ().bounds ().width () / 100.0;   // cm -> m
    float thickness = det->surface ().bounds ().thickness () / 100.0;  // cm -> m
	
    transform->scaleFactor.setValue (width, length, thickness);
    separator->addChild (transform);
    separator->addChild (cube);
  }
}

/** Get rotation of a DetUnit */ 
void
VisCuDetectorRep::createRotation (const GeomDetUnit *det, SbVec3f & axis, float & angle)
{    
  // Extract the coeff of the rotation matrix
  // with a projection on the basis vectors
  TkRotation< float > rot = det->surface ().rotation ();
  Basic3DVector< float > nx (1., 0., 0.); // x basis vector
  Basic3DVector< float > ny (0., 1., 0.); // y basis vector
  Basic3DVector< float > nz (0., 0., 1.); // z basis vector
  Basic3DVector< float > px = rot * nx; // Rxx Ryx Rzx
  Basic3DVector< float > py = rot * ny; // Rxy Ryy Rzy
  Basic3DVector< float > pz = rot * nz; // Rxz Ryz Rzz
  /* the rotation matrix is
   *	Rxx Rxy Rxz
   *	Ryx Ryy Ryz
   *	Rzx Rzy Rzz
   */
  // SbMatrix in row major order.
  // The 4th dimension is set empiricly set to
  // the identity
  SbMatrix sbm (px.x (), py.x (), pz.x (), 0,
		px.y (), py.y (), pz.y (), 0,
		px.z (), py.z (), pz.z (), 0,
		0, 0, 0, 1);
  SbRotation sbr (sbm);

  float x;
  float y;
  float z;

  sbr.getValue (axis, angle);
  axis.getValue (x, y, z);
}
