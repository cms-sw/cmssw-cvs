import FWCore.ParameterSet.Config as cms

from PhysicsTools.PFCandProducer.pfMET_cfi  import *
from PhysicsTools.PFCandProducer.pfNoPileUp_cff  import *
from PhysicsTools.PFCandProducer.pfElectrons_cff import *
from PhysicsTools.PFCandProducer.pfMuons_cff import *
from PhysicsTools.PFCandProducer.pfJets_cff import *
from PhysicsTools.PFCandProducer.pfTaus_cff import *

# sequential top projection cleaning
from PhysicsTools.PFCandProducer.ParticleSelectors.pfSortByType_cff import *
from PhysicsTools.PFCandProducer.TopProjectors.pfNoMuon_cfi import * 
from PhysicsTools.PFCandProducer.TopProjectors.pfNoElectron_cfi import * 
from PhysicsTools.PFCandProducer.TopProjectors.pfNoJet_cfi import *
from PhysicsTools.PFCandProducer.TopProjectors.pfNoTau_cfi import *

# generator tools
from PhysicsTools.PFCandProducer.genForPF2PAT_cff import *

# To reconstruct genjets without the neutrinos
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *
genParticlesForJets.ignoreParticleIDs.append(14)
genParticlesForJets.ignoreParticleIDs.append(12)
genParticlesForJets.ignoreParticleIDs.append(16)
genParticlesForJets.excludeResonances = False

PF2PAT = cms.Sequence(
    pfMET +
    pfNoPileUpSequence + 
    # pfSortByTypeSequence +
    pfAllNeutralHadrons+
    pfAllChargedHadrons+
    pfAllPhotons+
    # pfAllMuons + in 'pfMuonSequence' 
    pfMuonSequence + 
    pfNoMuon +
    # pfAllElectrons + in 'pfElectronSequence' 
    pfElectronSequence +
    pfNoElectron + 
# when uncommenting, change the source of the jet clustering
    pfJetSequence +
    pfNoJet + 
    pfTauSequence +
    pfNoTau 
# now that we have real data, we leave it to the user
# or maybe to PAT? to run the gen sequence. 
    )
