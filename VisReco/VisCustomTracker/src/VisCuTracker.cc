//<<<<<< INCLUDES                                                       >>>>>>
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "VisReco/VisCustomTracker/interface/VisCuTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "DataFormats/GeometrySurface/interface/TkRotation.h"
#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include "Iguana/Inventor/interface/IgSoCube.h"
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <map>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCuTracker::VisCuTracker (void)
{
}

SoSeparator*
VisCuTracker::buildDet(VisCuDetectorRep::DetectorRep type, TrackerGeometry* pDD, int subDetector)
{
    SoSeparator *sep = new SoSeparator;

    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.88445997, 0.840042, 0.53582799);
    mat->diffuseColor.setValue (0.66334498, 0.630032, 0.401871);
    mat->specularColor.setValue (0.49999201, 0.49999201, 0.49999201);
    mat->emissiveColor.setValue (0, 0, 0);
    mat->shininess = 0.1;
    mat->transparency = 0;
    sep->addChild (mat);
    
    for (TrackingGeometry::DetUnitContainer::const_iterator idet = pDD->detUnits().begin();
         idet != pDD->detUnits().end(); idet++)
    {
      VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[(*idet)];
      if(mod->isVisible())
        {

          if (visible ((*idet)->surface ().position (), type, subDetector))
          {

              SoSeparator* separator = new SoSeparator;
              VisCuDetectorRep::addWafer (separator, *idet, type);
              sep->addChild (separator);
              }
        }
    }
    return sep;
}

bool
VisCuTracker::visible (const GlobalPoint & point, VisCuDetectorRep::DetectorRep type, int subDetector)
{
    bool visible = false;
    double width = 1.0;
    double length = 10.0;
    if (subDetector == GeomDetEnumerators::TOB) width = 10.0;
    if (subDetector == GeomDetEnumerators::TIB) width = 5.5;
    if (subDetector == GeomDetEnumerators::TID) width = 5.0;
    if (subDetector == GeomDetEnumerators::TEC) width = 10.0;
    if (subDetector == GeomDetEnumerators::CSC) width = 55.0;
    
    switch (type)
    {
    case VisCuDetectorRep::FullDet:
	visible = true;
	break;
    case VisCuDetectorRep::RZDet:
        visible = true;
	//fabs (point.x ()) < width ? visible = true : visible = false;
	break;
    case VisCuDetectorRep::RPhiDet:
	fabs (point.z ()) < length ? visible = true : visible = false;
	break;
    default:
	break;
    }

    return visible;
}

int
VisCuTracker::subDetector (const std::string &key) 
{ 
    std::map<std::string, GeomDetEnumerators::SubDetector> type; 

    type ["PixelBarrel"] = GeomDetEnumerators::PixelBarrel;
    type ["TIB"] = GeomDetEnumerators::TIB;
    type ["TOB"] = GeomDetEnumerators::TOB;
    type ["PixelEndcap"] = GeomDetEnumerators::PixelEndcap;
    type ["TID"] = GeomDetEnumerators::TID;
    type ["TEC"] = GeomDetEnumerators::TEC;

    return type [key]; 
}
