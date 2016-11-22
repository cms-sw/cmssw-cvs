import FWCore.ParameterSet.Config as cms

# Jets
# Correct Reco jets
from JetMETCorrections.Configuration.JetCorrectionServicesHLT_cff import *

hltMCJetCorJetIcone5 = cms.EDProducer( "CaloJetCorrectionProducer",
  src = cms.InputTag( "hltIterativeCone5CaloJets" ),
  alias = cms.untracked.string( "MCJetCorJetIcone5" ),
  correctors = cms.vstring( 'MCJetCorrectorIcone5' )
)
#Muons
# Clone Reco muons
CloneRecoMuons = cms.EDProducer("MuonShallowCloneProducer",
    src = cms.InputTag("muons")
)

# Select Reco muons
SelectRecoMuons = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("CloneRecoMuons"),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(5.0),
    ptMin = cms.double(10.0)
)

# Electrons
# Clone Reco electrons 
CloneRecoElec = cms.EDProducer("ElectronShallowCloneProducer",
#    src = cms.InputTag("siStripElectronToTrackAssociator","siStripElectrons")
     src = cms.InputTag("photons")
)

# Select Reco electrons
SelectRecoElec = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("CloneRecoElec"),
    etaMin = cms.double(-3.0),
    etaMax = cms.double(3.0),
    ptMin = cms.double(10.0)
)

# Clone Reco jets
CloneRecoJets = cms.EDProducer("CaloJetShallowCloneProducer",
##    src = cms.InputTag("MCJetCorJetIcone5")
    src = cms.InputTag("iterativeCone5CaloJets")
)

# Select Reco Cen jets
SelectRecoCenJets = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("CloneRecoJets"),
    etaMin = cms.double(-3.0),
    etaMax = cms.double(3.0),
    ptMin = cms.double(5.0)
)

# Select Reco For jets
SelectRecoForJets = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("CloneRecoJets"),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(-3.0),
    ptMin = cms.double(5.0)
)

# Make tau jet collection
TauJets = cms.EDProducer("TauCaloJetProducer",
    src = cms.InputTag("coneIsolationTauJetTags"),
    disMin = cms.double(0.0)
)

# Clone Reco tau jets
CloneRecoTauJets = cms.EDProducer("CaloJetShallowCloneProducer",
    src = cms.InputTag("TauJets")
)

# Select Reco tau jets
SelectRecoTauJets = cms.EDFilter("EtaPtMinCandSelector",
    src = cms.InputTag("CloneRecoTauJets"),
    etaMin = cms.double(-3.0),
    etaMax = cms.double(3.0),
    ptMin = cms.double(5.0)
)

# Missing Et
# Clone Reco Met
CloneRecoMet = cms.EDProducer("CaloMETShallowCloneProducer",
    src = cms.InputTag("met")
)

# Select Reco Met
SelectRecoMet = cms.EDFilter("PtMinCandSelector",
    src = cms.InputTag("CloneRecoMet"),
    ptMin = cms.double(10.0)
)

RecoMuonSelection = cms.Sequence(CloneRecoMuons*SelectRecoMuons)
RecoElecSelection = cms.Sequence(CloneRecoElec*SelectRecoElec)
RecoCenJetSelection = cms.Sequence(hltMCJetCorJetIcone5*CloneRecoJets*SelectRecoCenJets)
RecoForJetSelection = cms.Sequence(hltMCJetCorJetIcone5*CloneRecoJets*SelectRecoForJets)
RecoTauJetSelection = cms.Sequence(TauJets*CloneRecoTauJets*SelectRecoTauJets)
RecoMetSelection = cms.Sequence(CloneRecoMet*SelectRecoMet)

