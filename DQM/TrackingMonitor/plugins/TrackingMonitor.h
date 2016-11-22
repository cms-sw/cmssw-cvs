#ifndef TrackingMonitor_H
#define TrackingMonitor_H
// -*- C++ -*-
//
// Package:    TrackingMonitor
// Class:      TrackingMonitor
// 
/**\class TrackingMonitor TrackingMonitor.cc DQM/TrackerMonitorTrack/src/TrackingMonitor.cc
Monitoring source for general quantities related to tracks.
*/
// Original Author:  Suchandra Dutta, Giorgia Mila
//         Created:  Thu 28 22:45:30 CEST 2008
// $Id: TrackingMonitor.h,v 1.7 2010/05/12 07:14:47 dutta Exp $

#include <memory>
#include <fstream>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"

class DQMStore;
class TrackAnalyzer;
class TrackBuildingAnalyzer;
class TProfile;
class GenericTriggerEventFlag;

class TrackingMonitor : public edm::EDAnalyzer 
{
    public:
        explicit TrackingMonitor(const edm::ParameterSet&);
        ~TrackingMonitor();
        virtual void beginJob(void);
        virtual void endJob(void);

        virtual void beginLuminosityBlock(const edm::LuminosityBlock& lumi, const edm::EventSetup&  eSetup);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void beginRun(const edm::Run&, const edm::EventSetup&); 
        virtual void endRun(const edm::Run&, const edm::EventSetup&);

    private:
        void doProfileX(TH2 * th2, MonitorElement* me);
        void doProfileX(MonitorElement * th2m, MonitorElement* me);


        // ----------member data ---------------------------

        std::string histname;  //for naming the histograms according to algorithm used

        DQMStore * dqmStore_;
        edm::ParameterSet conf_;

        // the track analyzer
        edm::InputTag bsSrc;

        TrackAnalyzer * theTrackAnalyzer;
        TrackBuildingAnalyzer  * theTrackBuildingAnalyzer;

        // Tracks 
        MonitorElement * NumberOfTracks;
        MonitorElement * NumberOfMeanRecHitsPerTrack;
        MonitorElement * NumberOfMeanLayersPerTrack;  

        // Track Seeds 
        MonitorElement * NumberOfSeeds;

        // Track Candidates
        MonitorElement * NumberOfTrackCandidates;

        // Cluster Properties
        MonitorElement* NumberOfPixelClus;
        MonitorElement* NumberOfStripClus;
        MonitorElement* RatioOfPixelAndStripClus;
        MonitorElement* NumberOfStripVsStripClus;
        MonitorElement* NumberOfTrkVsClus;

        std::string builderName;
        edm::ESHandle<TransientTrackingRecHitBuilder> theTTRHBuilder;
 
        bool doTrackerSpecific_; 
        bool doLumiAnalysis;

        GenericTriggerEventFlag* genTriggerEventFlag_;
};

#endif //define TrackingMonitor_H
