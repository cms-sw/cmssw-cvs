#ifndef FastSimulation_Muons_FastL3MuonProducer__H
#define FastSimulation_Muons_FastL3MuonProducer_H

/**  \class FastL3MuonProducer
 * 
 *   Fast L3 muon reconstructor:
 *   reconstructs muons using DT, CSC, RPC and tracker
 *   information,<BR>
 *   starting from a L2 reonstructed muon and from pixel seeded
 *   tracks (by default - the latter is configuratble)
 *
 *   $Date: 2010/07/19 19:54:14 $
 *   $Revision: 1.4 $
 *   \author  P. Janot - CERN
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace edm {class ParameterSet; class Event; class EventSetup;}

class MuonServiceProxy;
class MuonTrackFinder;
// class FastL3MuonTrajectoryBuilder;
class L3MuonTrajectoryBuilder;

class FastL3MuonProducer : public edm::EDProducer {

 public:

  /// constructor with config
  FastL3MuonProducer(const edm::ParameterSet&);
  
  /// destructor
  virtual ~FastL3MuonProducer(); 
  
  /// reconstruct muons
  virtual void produce(edm::Event&, const edm::EventSetup&);

  
 private:
    
  /// Seed STA  (input)
  edm::InputTag theL2CollectionLabel;

  /// Tracker tracks (input)
  edm::InputTag theTrackerTrackCollectionLabel;

  /// Label for L2SeededTracks (output)
  std::string theL2SeededTkLabel;

  // The muon track finder (from STA and tracks)
  MuonTrackFinder* theTrackFinder;
  // FastL3MuonTrajectoryBuilder* l3mtb;
  L3MuonTrajectoryBuilder* l3mtb;

  bool theL2TrajectoryFlag;
  bool updatedAtVtx;

  /// the event setup proxy, it takes care the services update
  MuonServiceProxy *theService;
    
};

#endif
