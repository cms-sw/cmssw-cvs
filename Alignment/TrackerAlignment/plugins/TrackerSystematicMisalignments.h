#ifndef Alignment_TrackerAlignment_TrackerSystematicMisalignments_h
#define Alignment_TrackerAlignment_TrackerSystematicMisalignments_h

/** \class TrackerSystematicMisalignments
 *
 *  Class to misaligned tracker from DB.
 *
 *  $Date: 2009/12/17 20:46:50 $
 *  $Revision: 1.3 $
 *  \author Chung Khim Lae
 */
// user include files

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


class AlignableSurface;
class Alignments;


class TrackerSystematicMisalignments:
public edm::EDAnalyzer
{
public:
	
	TrackerSystematicMisalignments(
								   const edm::ParameterSet&
								   );
	
	/// Read ideal tracker geometry from DB
	virtual void beginJob();
	
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	
private:
	
	void applySystematicMisalignment( Alignable* ); 
	align::GlobalVector findSystematicMis( align::PositionType );
	
	AlignableTracker* theAlignableTracker;
	
	
	
	// configurables needed for the systematic misalignment
	bool m_fromDBGeom;
	
	double m_radialEpsilon;
	double m_telescopeEpsilon;
	double m_layerRotEpsilon;
	double m_bowingEpsilon;
	double m_zExpEpsilon;
	double m_twistEpsilon;
	double m_ellipticalEpsilon;
	double m_skewEpsilon;
	double m_saggitaEpsilon;
	
};

#endif
