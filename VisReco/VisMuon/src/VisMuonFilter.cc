#include "VisReco/VisMuon/interface/VisMuonFilter.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

using namespace std;
using namespace edm;

VisMuonFilter::VisMuonFilter (const ParameterSet &pset)
{
    m_muonLabel = pset.getUntrackedParameter<string>("StandAloneTrackCollectionLabel");

    m_numberOfEvents = 0;
    m_numberMuonTracks = 0;
}

VisMuonFilter::~VisMuonFilter (void) 
{}

bool 
VisMuonFilter::filter (Event &event, const EventSetup &eventSetup) 
{
    Handle<reco::TrackCollection> staTracks;
    event.getByLabel(m_muonLabel, staTracks);

    bool noTracks = false;
    m_numberOfEvents++;

    int numberMuonTracks = staTracks->size();

    if (! numberMuonTracks) 
    {
        m_numberMuonTracks++;
        cout << "Event no. " << m_numberOfEvents << " has no tracks (" 
	     << m_numberMuonTracks << " events with no tracks)." << endl;

        noTracks = true;
    }

    return noTracks;
}
