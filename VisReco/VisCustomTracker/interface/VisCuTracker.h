#ifndef VIS_CUTRACKER_VIS_TRACKER_H
# define VIS_CUTRACKER_VIS_TRACKER_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Iguana/Framework/interface/IgTwig.h"
# include "VisReco/VisCustomTracker/interface/VisCuDetectorRep.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
# include "DataFormats/GeometryVector/interface/GlobalPoint.h"
# include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
# include "DataFormats/SiStripDetId/interface/TIBDetId.h"
# include "DataFormats/SiStripDetId/interface/TOBDetId.h"
# include "DataFormats/SiStripDetId/interface/TECDetId.h"
# include "DataFormats/SiStripDetId/interface/TIDDetId.h"
# include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
# include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
# include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

# include <string>
# include <iostream>  
# include <fstream>
# include <sstream>


//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class GeomDetUnit;
class SbVec3f;
class SoSeparator;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCuTracker
{
public:
  VisCuTracker (void);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor

  SoSeparator	       *buildDet (VisCuDetectorRep::DetectorRep type, TrackerGeometry* pDD, int subDetector);
  int			subDetector (const std::string &key);

  static int nlayer(int det,int part,int lay){
    if(det==3 && part==1) return lay;
    if(det==2 && part==1) return lay+9;
    if(det==1 && part==1) return lay+12;
    if(det==1 && part==3) return lay+15;
    if(det==2 && part==3) return lay+18;
    if(det==3 && part==3) return lay+21;
    if(det==1 && part==2) return lay+30;
    if(det==2 && part==2) return lay+33;
    if(det==3 && part==2) return lay+37;
    return -1;
  }
  static  std::string getModuleName(DetId detid1){
    std::ostringstream outs;
    std::string name0,name1,name2;
    switch( detid1.subdetId())
    {
    case PixelSubdetector::PixelBarrel:
      {
	PXBDetId detid(detid1);  
	outs <<detid<<" "<<detid1.rawId();
	break;
      }

    case PixelSubdetector::PixelEndcap:
      {
	PXFDetId detid(detid1);  
	outs <<detid<<" "<<detid1.rawId();
	break;
      }

    case StripSubdetector::TIB: 
      {
	TIBDetId detid(detid1);  
	outs <<detid;
	break;
      }

    case StripSubdetector::TID: 
      {
	TIDDetId detid(detid1);  
	outs <<detid;
	break;
      }

    case StripSubdetector::TOB: 
      {
	TOBDetId detid(detid1);  
	outs <<detid;
	break;
      }

    case StripSubdetector::TEC: 
      {
	TECDetId detid(detid1);  
	outs <<detid;
	break;
      }

    }
    return outs.str();
    
  }

private:
  bool	     	visible(const GlobalPoint & point, VisCuDetectorRep::DetectorRep type, int subDetector);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_CUTRACKER_VIS_TRACKER_H
