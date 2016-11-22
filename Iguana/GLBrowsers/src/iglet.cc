//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/Ig2DWindow.h"
#include "Iguana/GLBrowsers/interface/Ig2DBrowser.h"
#include "Iguana/GLBrowsers/interface/Ig3DWindow.h"
#include "Iguana/GLBrowsers/interface/Ig3DBrowser.h"
#include "Iguana/GLBrowsers/interface/IgLegoWindow.h"
#include "Iguana/GLBrowsers/interface/IgLegoBrowser.h"
#include "Iguana/GLBrowsers/interface/IgRPhiWindow.h"
#include "Iguana/GLBrowsers/interface/IgRPhiBrowser.h"
#include "Iguana/GLBrowsers/interface/IgRZWindow.h"
#include "Iguana/GLBrowsers/interface/IgRZBrowser.h"
#include "Iguana/GLBrowsers/interface/IgSpareWindow.h"
#include "Iguana/GLBrowsers/interface/IgSpareBrowser.h"
#include "Iguana/GLBrowsers/interface/IgStudioCentre.h"
#include "Iguana/GLBrowsers/interface/Ig3DSystem.h"
#include "Iguana/GLBrowsers/interface/IgOIVExtension.h"
#include "Iguana/GLBrowsers/interface/IgIVView.h"
#include "Iguana/Studio/interface/IgViewDB.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include "Iguana/Framework/interface/IgCapabilityDB.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include "Iguana/Inventor/interface/IgSoShapeKit.h"
#include "Iguana/Inventor/interface/IgSo2DArrow.h"
#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include "Iguana/Inventor/interface/IgSoCalHit.h"
#include "Iguana/Inventor/interface/IgSoCircleArc.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include "Iguana/Inventor/interface/IgSoCube.h"
#include "Iguana/Inventor/interface/IgSoEllipsoid.h"
#include "Iguana/Inventor/interface/IgSoFieldPlane.h"
#include "Iguana/Inventor/interface/IgSoFieldPlaneMap.h"
#include "Iguana/Inventor/interface/IgSoG4Box.h"
#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include "Iguana/Inventor/interface/IgSoG4Trd.h"
#include "Iguana/Inventor/interface/IgSoGrid.h"
#include "Iguana/Inventor/interface/IgSoHits.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgSoJet.h"
#include "Iguana/Inventor/interface/IgSoLegoPlot.h"
#include "Iguana/Inventor/interface/IgSoLegoTowers.h"
#include "Iguana/Inventor/interface/IgSoPcon.h"
#include "Iguana/Inventor/interface/IgSoPolyVol.h"
#include "Iguana/Inventor/interface/IgSoQuad.h"
#include "Iguana/Inventor/interface/IgSoRectHist.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include "Iguana/Inventor/interface/IgSoRotSolid.h"
#include "Iguana/Inventor/interface/IgSoRZHist.h"
#include "Iguana/Inventor/interface/IgSoSiStrips.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgSoSphereHit.h"
#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Inventor/interface/IgSoTowerRadii.h"
#include "Iguana/Inventor/interface/IgSoXYZGrid.h"
#include "Iguana/Inventor/interface/IgSoAnimator.h"
#include "Iguana/Inventor/interface/IgSoClipPlane.h"
#include "Iguana/Inventor/interface/IgSoSlicer.h"
#include "Iguana/Inventor/interface/IgSoSlicerEngine.h"
#include "Iguana/Inventor/interface/IgSoViewpoint.h"
#include "Iguana/Inventor/interface/IgSoPlaneManip.h"
#include "Iguana/Inventor/interface/IgSoGL2PSAction.h"
#include "Iguana/Inventor/interface/IgSoGridPlane.h"
#include "Iguana/Inventor/interface/IgSoGridPlaneMap.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

static void initNodes (void)
{
    Ig3DSystem::initAction (lat::CreateCallback (&IgParticleChar::initParticles));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoShapeKit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSo2DArrow::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSo3DErrorBar::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoArrow::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoAxis::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCalHit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCircleArc::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCircularHist::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCoordinateAxis::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCrystalHit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCube::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoEllipsoid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoFieldPlane::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoFieldPlaneMap::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoG4Box::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoG4Trap::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoG4Trd::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoGrid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoHits::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoIdealTrack::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoJet::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoLegoPlot::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoLegoTowers::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoPcon::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoPolyVol::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoQuad::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoRectHist::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoRectColHist::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoRotSolid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoRZHist::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSiStrips::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSimpleTrajectory::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSphereHit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSplineTrack::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoTower::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoTowerRadii::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoXYZGrid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoAnimator::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoClipPlane::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSlicer::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSlicerEngine::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoViewpoint::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoPlaneManip::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoGL2PSAction::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoGridPlane::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoGridPlaneMap::initClass));   
}

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgBrowserDB, Ig2DBrowser, Ig2DBrowser::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgBrowserDB, Ig3DBrowser, Ig3DBrowser::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgLegoBrowser, IgLegoBrowser::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgRPhiBrowser, IgRPhiBrowser::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgRZBrowser, IgRZBrowser::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgSpareBrowser, IgSpareBrowser::catalogLabel ());

DEFINE_IGUANA_PLUGIN (IgExtensionDB, IgStudioCentre, IgStudioCentre::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgViewDB, Ig2DWindow, "2D Window");
DEFINE_IGUANA_PLUGIN (IgViewDB, Ig3DWindow, "3D Window");
DEFINE_IGUANA_PLUGIN (IgViewDB, IgLegoWindow, "Lego Window");
DEFINE_IGUANA_PLUGIN (IgViewDB, IgRPhiWindow, "RPhi Window");
DEFINE_IGUANA_PLUGIN (IgViewDB, IgRZWindow, "RZ Window");
DEFINE_IGUANA_PLUGIN (IgViewDB, IgSpareWindow, "Spare Window");
DEFINE_IGUANA_PLUGIN (IgExtensionDB, IgOIVExtension, IgOIVExtension::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgViewDB, IgIVView, "Open Inventor View");

DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, "Services/Inventor/NodeKits/IgOpenInventor");

IG_MODULE_ATTACH_ACTION ((&initNodes));
