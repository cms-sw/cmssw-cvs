#include "VisReco/VisDetector/interface/VisDetContent.h"
#include "VisReco/VisDetector/interface/VisDetIvTwig.h"
#include "VisReco/VisDetector/interface/VisDetTextureTwig.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/GLBrowsers/interface/Ig3DShapeManager.h"

IG_DEFINE_STATE_ELEMENT (VisDetContent, "Data/Reco/Detector");

/** Initialise the CMS detector data proxy. */
VisDetContent::VisDetContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the detector twig to the
    document data tree.  */
void
VisDetContent::init (void)
{
  IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ());
  ASSERT (sbar);
  sbar->setMessage ("Starting CMS detector construction...");

  Ig3DShapeManager *shapeManager = Ig3DShapeManager::get (state ());
  if (! shapeManager) shapeManager = new Ig3DShapeManager (state ());
  ASSERT (shapeManager);

  IgTwig *rootTwig = IgDocumentData::get (state ())->root ("", true);
    
  IgTwig * detTwig = new IgSimpleTwig (rootTwig, "Detector");
  new VisDetTextureTwig (state (), detTwig, "CMSskin");
    
  sbar->setMessage ("Drift Tubes...");
  new VisDetIvTwig (state (), detTwig, "Drift Tubes", "driftTubes-3d.gzip.iv", "driftTubes-rphi.gzip.iv", "driftTubes-rz.gzip.iv");
  sbar->setMessage ("Drift Tubes Superlayers...");
  new VisDetIvTwig (state (), detTwig, "Drift Tubes Superlayers", "none.iv", "driftTubes-superlayers-rphi.gzip.iv", "driftTubes-superlayers-rz.gzip.iv");
  sbar->setMessage ("Drift Tubes Superlayers (details)...");
  new VisDetIvTwig (state (), detTwig, "Drift Tubes Superlayers (details)", "none.iv", "driftTubes-superlayer-details-rphi.gzip.iv", "none.iv");
  sbar->setMessage ("RPCs in barrel...");
  new VisDetIvTwig (state (), detTwig, "Barrel RPCs", "rpc-barrel-3d.gzip.iv", "rpc-barrel-rphi.gzip.iv", "rpc-barrel-rz.gzip.iv");
  sbar->setMessage ("Absorber in barrel...");
  new VisDetIvTwig (state (), detTwig, "Barrel Absorber", "absorber-barrel-3d.gzip.iv", "absorber-barrel-rphi.gzip.iv", "absorber-barrel-rz.gzip.iv");
  sbar->setMessage ("CSCs...");
  new VisDetIvTwig (state (), detTwig, "CSCs", "csc-3d.gzip.iv", "none.iv", "csc-rz.gzip.iv");
  sbar->setMessage ("RPCs in the endcaps...");
  new VisDetIvTwig (state (), detTwig, "Endcap RPCs", "rpc-endcap-3d.gzip.iv", "none.iv", "rpc-endcap-rz.gzip.iv");
  sbar->setMessage ("Absorber in the endcaps...");
  new VisDetIvTwig (state (), detTwig, "Endcap Absorber", "absorber-endcap-3d.gzip.iv", "none.iv", "absorber-endcap-rz.gzip.iv");
  sbar->setMessage ("ECAL barrel...");
  new VisDetIvTwig (state (), detTwig, "ECAL Barrel", "ecal-barrel-3d.gzip.iv", "ecal-barrel-rphi.gzip.iv", "ecal-rz.gzip.iv");
  sbar->setMessage ("ECAL forward...");
  new VisDetIvTwig (state (), detTwig, "ECAL Forward", "ecal-forward-3d.gzip.iv", "none.iv", "none.iv");
  sbar->setMessage ("Preshower...");
  new VisDetIvTwig (state (), detTwig, "ECAL Preshower", "ecal-preshower-3d.gzip.iv", "none.iv", "none.iv");
  sbar->setMessage ("HCAL...");
  new VisDetIvTwig (state (), detTwig, "HCAL Barrel", "hcal-barrel-3d.gzip.iv", "hcal-barrel-rphi.gzip.iv", "hcal-barrel-rz.gzip.iv");
  sbar->setMessage ("HCAL barrel layers...");
  new VisDetIvTwig (state (), detTwig, "HCAL Barrel Layers", "none.iv", "hcal-barrel-layers-rphi.gzip.iv", "hcal-barrel-layers-rz.gzip.iv");
  sbar->setMessage ("HCAL barrel towers...");
  new VisDetIvTwig (state (), detTwig, "HCAL Barrel Towers", "none.iv", "hcal-barrel-towers-rphi.gzip.iv", "none.iv");
  sbar->setMessage ("HCAL endcap...");
  new VisDetIvTwig (state (), detTwig, "HCAL Endcap", "hcal-endcap-3d.gzip.iv", "none.iv", "hcal-endcap-rz.gzip.iv");
  sbar->setMessage ("HCAL forward...");
  new VisDetIvTwig (state (), detTwig, "HCAL Forward", "hcal-forward-3d.gzip.iv", "none.iv", "hcal-forward-rz.gzip.iv");
  sbar->setMessage ("HCAL outer...");
  new VisDetIvTwig (state (), detTwig, "HCAL Outer", "hcal-outer-3d.gzip.iv", "none.iv", "none.iv");
  sbar->setMessage ("Tracker...");
  new VisDetIvTwig (state (), detTwig, "Tracker", "none.iv", "tracker-barrel-rphi.gzip.iv", "tracker-rz.gzip.iv");
  sbar->setMessage ("Beampipe...");
  new VisDetIvTwig (state (), detTwig, "Beampipe", "beampipe.gzip.iv", "none.iv", "beampipe-rz.gzip.iv");
  sbar->setMessage ("Magnet...");
  new VisDetIvTwig (state (), detTwig, "Magnet", "magnet-3d.gzip.iv", "magnet-rphi.gzip.iv", "magnet-rz.gzip.iv");

  sbar->setMessage ("CMS detector is loaded.");
}
