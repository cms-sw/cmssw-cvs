#ifndef VIS_EVENT_SETUP_VIS_EVENT_SETUP_SERVICE_H
# define VIS_EVENT_SETUP_VIS_EVENT_SETUP_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisEventObserver.h"
# include "VisFramework/VisFrameworkBase/interface/VisPhiDelta.h"
# include "VisFramework/VisFrameworkBase/interface/VisPhiStart.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "FWCore/Framework/interface/ESWatcher.h"
# include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
# include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
# include "Geometry/CaloGeometry/interface/CaloGeometry.h"
# include "Geometry/CSCGeometry/interface/CSCGeometry.h"
# include "Geometry/DTGeometry/interface/DTGeometry.h"
# include "Geometry/RPCGeometry/interface/RPCGeometry.h"
# include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
# include "Geometry/Records/interface/CaloGeometryRecord.h"
# include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
# include "DataFormats/GeometryVector/interface/GlobalPoint.h"
# include "DataFormats/GeometryVector/interface/LocalPoint.h"
# include "DataFormats/TrackReco/interface/TrackFwd.h"
# include "MagneticField/Engine/interface/MagneticField.h"
# include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class GeomDet;
class DetId;
class SbVec3f;
class SoSeparator;
class TrackingRecHit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEventSetupService : public IgStateElement, public VisEventObserver
{
    IG_DECLARE_STATE_ELEMENT (VisEventSetupService);
public:
    VisEventSetupService (IgState *state);
    ~VisEventSetupService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);

    enum DetectorRep { FullDet, RZDet, RPhiDet };

    const GlobalPoint 	toGlobal (const LocalPoint point, const DetId detId);
    const GlobalPoint	globalPosition (const TrackingRecHit * rechit);
    const LocalPoint	localPosition (const TrackingRecHit * rechit);
    const GeomDet*	idToDet (const DetId det);
    
    float		phi4eta (float eta) const;
    unsigned int	getBinNumber (const DetId detId,
				      const unsigned numR, 
				      const unsigned numZ, 
				      const float radiusR,
				      const float radiusZ,
				      const float barrelMaxEta = 0.f, 
				      const float beamPipeTheta = 0.f, 
				      const float endcapMaxTheta = 0.f) const;
    unsigned int	getBinNumber (const float eta, const float phi,
				      const unsigned numR, 
				      const unsigned numZ, 
				      const float radiusR,
				      const float radiusZ,
				      const float barrelMaxEta = 0.f, 
				      const float beamPipeTheta = 0.f, 
				      const float endcapMaxTheta = 0.f) const;
    float 		etaToTheta (float eta) const;
    float 		thetaToEta (float theta) const;
    bool		present (const DetId detId) const;
    bool		emptyCorners (const DetId detId) const;
    const CaloCellGeometry::CornersVec& getCorners (const DetId detId) const;
    const GlobalPoint& 	getCellPosition (const DetId detId) const;

    const std::string  	cscTextRep (int chamber, int ring, int station, int endcap);
    const std::string  	dtTextRep (int station, int sector, int wheel);
    const std::string  	rpcTextRep (int region, int wheel, int sector, int station);
    const std::string  	trackerTextRep (const std::string name);
    const std::string  	caloTextRep (const std::string name, const int subdetn);

    SoSeparator*   	buildCSCChambers (DetectorRep type, int chamber, int ring, int station, int endcap);
    SoSeparator*   	buildDTChambers (DetectorRep type, int station, int sector, int wheel);
    SoSeparator*	buildRPCChambers (DetectorRep type, int region, int wheel, int sector, int station);
    SoSeparator*	buildTracker (DetectorRep type, const std::string name);
    SoSeparator*	buildDet (DetectorRep type, std::vector<const GeomDetUnit*> &dets);
    SoSeparator*	buildCaloDet (DetectorRep type, const std::string detId, const int subdetn);
    SoSeparator*	buildLegoDet (const double scale);   
    SoSeparator*	buildHitDets (DetectorRep type, std::vector<DetId> &dets);   

    SoSeparator*   	annotateCSCChambers (DetectorRep type, int chamber, int ring, int station, int endcap);
    SoSeparator*   	annotateDTChambers (DetectorRep type, int station, int sector, int wheel);
    SoSeparator*	annotateRPCChambers (DetectorRep type, int region, int wheel, int sector, int station);
    SoSeparator*	annotateTracker (DetectorRep type, const std::string name);
    SoSeparator*	annotateCaloDet (DetectorRep type, const std::string detId, int subdetn);
    
    const std::string 	trackerModuleName (DetId detId);
    void	       	addMaterial (SoSeparator *separator, DetectorRep type);
    
    SoSeparator*	refitTrack (reco::TrackRef track, double in, double out, double step);
    void 		localPositions (const TrackingRecHit *rechit, std::vector<LocalPoint> &points);

    bool		globalTrackingGeomChanged (void) { return m_globalTrackingGeomCanged; }
    bool 		trackerGeomChanged (void) { return m_trackerGeomCanged; }
    bool 		caloGeomChanged (void) { return m_caloGeomCanged; }    
    bool 		muonGeomChanged (void) { return m_muonGeomCanged; } 
    bool 		fieldChanged (void) { return m_fieldGeomCanged; }
	
private:
    bool	     	visible (const GlobalPoint & point, DetectorRep type, int subDetector);
    void		addDescription (SoSeparator *separator, const GeomDet *det, const std::string label);
    void	      	addWafer (SoSeparator *separator, const GeomDet *det, DetectorRep type);
    void	      	addBoxWafer (SoSeparator *separator, const GeomDet *det);
    void		createRotation (const GeomDet *det, SbVec3f & axis, float & angle);
    int			subDetector (const std::string &key);
    DetId::Detector	detectorId (const std::string &key);
    void 		invalidate (void) {};
    
    IgState	       *m_state;
    int			m_run;
    int			m_event;
    int			m_ls;
    int			m_orbit;
    int			m_bx;
    VisPhiDelta		m_phiDelta;
    VisPhiStart		m_phiStart;    

    bool	m_globalTrackingGeomCanged;
    bool	m_trackerGeomCanged;
    bool	m_caloGeomCanged;
    bool	m_muonGeomCanged;
    bool	m_fieldGeomCanged;

    edm::ESHandle<GlobalTrackingGeometry> m_globalTrackingGeom;
    edm::ESHandle<TrackerGeometry> 	  m_trackerGeom;
    edm::ESHandle<CaloGeometry>		  m_caloGeom;
    edm::ESHandle<CSCGeometry>  	  m_cscGeom;
    edm::ESHandle<DTGeometry>  		  m_dtGeom;
    edm::ESHandle<RPCGeometry>  	  m_rpcGeom;
    edm::ESHandle<MagneticField>	  m_field;

    edm::ESWatcher<GlobalTrackingGeometryRecord> m_watch_globalTrackingGeom;
    edm::ESWatcher<TrackerDigiGeometryRecord> m_watch_trackerGeom;
    edm::ESWatcher<CaloGeometryRecord> m_watch_caloGeom;
    edm::ESWatcher<MuonGeometryRecord> m_watch_muonGeom;
    edm::ESWatcher<IdealMagneticFieldRecord> m_watch_field;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_EVENT_SETUP_VIS_EVENT_SETUP_SERVICE_H
