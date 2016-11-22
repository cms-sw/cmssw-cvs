import FWCore.ParameterSet.Config as cms

from RecoTauTag.RecoTau.TauDiscriminatorTools import requireLeadPion

pfRecoTauDiscriminationAgainstMuon = cms.EDProducer("PFRecoTauDiscriminationAgainstMuon",
    
    # tau collection to discriminate
    PFTauProducer = cms.InputTag('pfRecoTauProducer'),

    # Require leading pion ensures that:
    #  1) these is at least one track above threshold (0.5 GeV) in the signal cone
    #  2) a track OR a pi-zero in the signal cone has pT > 5 GeV
    Prediscriminants = requireLeadPion,

    # algorithm parameters
    a = cms.double(0.5), ## user definde 2D Cut. Reject tau if calo * a + seg * b < 0 

    b = cms.double(0.5),
    c = cms.double(0.0),
    discriminatorOption = cms.string('noSegMatch'), ## available options are; noSegMatch, twoDCut, merePresence, combined

)


