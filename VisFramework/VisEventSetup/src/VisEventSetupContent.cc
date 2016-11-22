#include "VisFramework/VisEventSetup/interface/VisEventSetupContent.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisEventSetup/interface/VisCaloGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisCSCGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisDTGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisRPCGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisTrackerGeometryTwig.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include <QString>

IG_DEFINE_STATE_ELEMENT (VisEventSetupContent, "Data/Framework/EventSetup");

VisEventSetupContent::VisEventSetupContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

void
VisEventSetupContent::init (void)
{    
  VisEventSetupService *esService = VisEventSetupService::get (state ());
  if (! esService)
  {
    esService = new VisEventSetupService (state ());	
  }
       
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *detTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");
  detTwig = new IgSimpleTwig (topTwig, "Reco Detector");
  detTwig->cascade (false);

  std::vector<std::string> twigs (4);
  twigs.push_back ("CaloGeometry");
  twigs.push_back ("CSCGeometry");
  twigs.push_back ("DTGeometry");
  twigs.push_back ("RPCGeometry");
  twigs.push_back ("TrackerGeometry");

  const edm::ParameterSet *pset = VisConfigurationService::pSet ();
  if (pset)
  {
    twigs = pset->getUntrackedParameter<std::vector<std::string> > ("VisEventSetup", twigs);
  }

  if (std::find (twigs.begin (), twigs.end (), "CaloGeometry") != twigs.end ()) 
  {
    VisCaloGeometryTwig *caloDetTwig = new VisCaloGeometryTwig (state (), detTwig, "Calo");
    buildCalo (caloDetTwig);
    caloDetTwig->cascade (false);
    caloDetTwig->selfVisible (true);
  }

  if (std::find (twigs.begin (), twigs.end (), "CSCGeometry") != twigs.end ()) 
  {
    VisCSCGeometryTwig *cscDetTwig = new VisCSCGeometryTwig (state (), detTwig, "CSCs");
    buildCSCs (cscDetTwig);
    cscDetTwig->cascade (false);
  }
    
  if (std::find (twigs.begin (), twigs.end (), "DTGeometry") != twigs.end ()) 
  {
    VisDTGeometryTwig *dtDetTwig = new VisDTGeometryTwig (state (), detTwig, "DTs");
    buildDTs (dtDetTwig);
    dtDetTwig->cascade (false);
  }
    
  if (std::find (twigs.begin (), twigs.end (), "RPCGeometry") != twigs.end ()) 
  {
    VisRPCGeometryTwig *rpcDetTwig = new VisRPCGeometryTwig (state (), detTwig, "RPCs");
    buildRPCs (rpcDetTwig);
    rpcDetTwig->cascade (false);
  }
    
  if (std::find (twigs.begin (), twigs.end (), "TrackerGeometry") != twigs.end ()) 
  {
    VisTrackerGeometryTwig *trackerDetTwig = new VisTrackerGeometryTwig (state (), detTwig, "Tracker");
    buildTracker (trackerDetTwig);
    trackerDetTwig->cascade (false);
  }
}

void
VisEventSetupContent::buildCalo (IgTwig *twig)
{    
  new VisCaloGeometryTwig (state (), twig, "Hcal Barrel", "Hcal", HcalBarrel);
  new VisCaloGeometryTwig (state (), twig, "Hcal Endcap", "Hcal", HcalEndcap);
  new VisCaloGeometryTwig (state (), twig, "Hcal Outer", "Hcal", HcalOuter);
  new VisCaloGeometryTwig (state (), twig, "Hcal Forward", "Hcal", HcalForward);

  new VisCaloGeometryTwig (state (), twig, "Ecal Barrel", "Ecal", EcalBarrel);
  new VisCaloGeometryTwig (state (), twig, "Ecal Endcap", "Ecal", EcalEndcap);
}

void
VisEventSetupContent::buildCSCs (IgTwig *twig)
{
  std::string ename ("Endcap ");
  std::string sname ("Station ");
  std::string rname ("Ring ");
  std::string cname ("Chamber ");
  int nChambers = 36;

  for (int e = 1; e <= 2; ++e)
  {
    VisCSCGeometryTwig *endcapDetTwig = new VisCSCGeometryTwig (state (), twig, ename + QString ("%1").arg (e).latin1 (), e);
    for (int s = 1; s <= 4; ++s)
    {
      VisCSCGeometryTwig *stationDetTwig = new VisCSCGeometryTwig (state (), endcapDetTwig, sname + QString ("%1").arg (s).latin1 (), e, s);
      int nRings = 4;
	    
      if (s == 2 || s == 3) nRings = 2;
      if (s == 4) nRings = 1;

      for (int r = 1; r <= nRings; ++r)
      {
	VisCSCGeometryTwig *ringDetTwig = new VisCSCGeometryTwig (state (), stationDetTwig, rname + QString ("%1").arg (r).latin1 (), e, s, r);

	((s == 2 && r == 1) || (s == 3 && r == 1) || (s == 4 && r == 1)) ? nChambers = 18 : nChambers = 36;
	
	for (int c = 1; c <= nChambers; ++c)
	{
	  new VisCSCGeometryTwig (state (), ringDetTwig, cname + QString ("%1").arg (c).latin1 (), e, s, r, c);
	}
	ringDetTwig->cascade (false);
      }
      stationDetTwig->cascade (false);
    }
    endcapDetTwig->cascade (false);
  }
}

void
VisEventSetupContent::buildDTs (IgTwig *twig)
{
  std::string wname ("Wheel ");
  std::string stname ("Station ");
  std::string sename ("Sector ");

  VisDTGeometryTwig *wheelDetTwig;
  VisDTGeometryTwig *stationDetTwig;
  VisDTGeometryTwig *sectorDetTwig;

  for (int w = -2; w <= +2; ++w)
  {
    wheelDetTwig = new VisDTGeometryTwig (state (), twig, wname + QString ("%1").arg (w).latin1 (), w);
    for (int se = 1; se <= 14; ++se)
    {
      sectorDetTwig = new VisDTGeometryTwig (state (), wheelDetTwig, sename + QString ("%1").arg (se).latin1 (), w, se);
      for (int st = 1; st <= 4; ++st)
      {
	stationDetTwig = new VisDTGeometryTwig (state (), sectorDetTwig, stname + QString ("%1").arg (st).latin1 (), w, se, st);
	stationDetTwig->cascade (false);
      }
      sectorDetTwig->cascade (false);
    }
    wheelDetTwig->cascade (false);
  }
}

void
VisEventSetupContent::buildRPCs (IgTwig *twig)
{
  std::string bname ("Barrel ");
  std::string ename ("Endcap ");
  std::string wname ("Wheel ");
  std::string sename ("Sector ");
  std::string stname ("Station ");

  VisRPCGeometryTwig *barrelDetTwig = 0;
  VisRPCGeometryTwig *endcapDetTwig = 0;
  VisRPCGeometryTwig *rpcWheelDetTwig = 0;
  VisRPCGeometryTwig *rpcStationDetTwig = 0;
  VisRPCGeometryTwig *rpcSectorDetTwig = 0;

  int r = 0;
  barrelDetTwig = new VisRPCGeometryTwig (state (), twig, "Barrel", r);

  for (int w = -2; w <= +2; ++w)
  {
    rpcWheelDetTwig = new VisRPCGeometryTwig (state (), barrelDetTwig, wname + QString ("%1").arg (w).latin1 (), r, w);
    int nSector = 12;
	  
    for (int se = 1; se <= nSector; ++se)
    {
      rpcSectorDetTwig = new VisRPCGeometryTwig (state (), rpcWheelDetTwig, sename + QString ("%1").arg (se).latin1 (), r, w, se);
      for (int st = 1; st <= 4; ++st)
      {
	rpcStationDetTwig = new VisRPCGeometryTwig (state (), rpcSectorDetTwig, stname + QString ("%1").arg (st).latin1 (), r, w, se, st);
	rpcStationDetTwig->cascade (false);
      }	      
      rpcSectorDetTwig->cascade (false);
    }	  
    rpcWheelDetTwig->cascade (false);
  }
  barrelDetTwig->cascade (false);
    
  for (r = -1; r <=1; r += 2)
  {
    endcapDetTwig = new VisRPCGeometryTwig (state (), twig, ename + QString ("%1").arg (r).latin1 (), r);
    for (int w = 2; w <= 3; ++w)
    {
      rpcWheelDetTwig = new VisRPCGeometryTwig (state (), endcapDetTwig, wname + QString ("%1").arg (w).latin1 (), r, w);
      int nSector = 6;
      for (int se = 1; se <= nSector; ++se)
      {
	rpcSectorDetTwig = new VisRPCGeometryTwig (state (), rpcWheelDetTwig, sename + QString ("%1").arg (se).latin1 (), r, w, se);
	for (int st = 1; st <= 3; ++st)
	{
	  rpcStationDetTwig = new VisRPCGeometryTwig (state (), rpcSectorDetTwig, stname + QString ("%1").arg (st).latin1 (), r, w, se, st); 
	  rpcStationDetTwig->cascade (false);
	}
	rpcSectorDetTwig->cascade (false);
      }
      rpcWheelDetTwig->cascade (false);
    }
    endcapDetTwig->cascade (false);	
  }
}

void
VisEventSetupContent::buildTracker (IgTwig *twig)
{
  new VisTrackerGeometryTwig (state (), twig, "PixelBarrel");
  new VisTrackerGeometryTwig (state (), twig, "PixelEndcap");
  new VisTrackerGeometryTwig (state (), twig, "TIB");
  new VisTrackerGeometryTwig (state (), twig, "TOB");
  new VisTrackerGeometryTwig (state (), twig, "TID");
  new VisTrackerGeometryTwig (state (), twig, "TEC");
}
