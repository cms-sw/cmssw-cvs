import FWCore.ParameterSet.Config as cms
import copy

from RecoTauTag.RecoTau.JPTRecoTauProducer_cff import *
from RecoTauTag.RecoTau.TCRecoTauProducer_cfi import *

from RecoTauTag.RecoTau.CaloRecoTauDiscriminationByLeadingTrackFinding_cfi import *
from RecoTauTag.RecoTau.CaloRecoTauDiscriminationByLeadingTrackPtCut_cfi import *
from RecoTauTag.RecoTau.CaloRecoTauDiscriminationByIsolation_cfi import *
from RecoTauTag.RecoTau.CaloRecoTauDiscriminationAgainstElectron_cfi import *
#from RecoTauTag.RecoTau.TCRecoTauDiscriminationAgainstHadronicJets_cfi import *

caloRecoTauProducer = copy.deepcopy(tcRecoTauProducer)

tautagging = cms.Sequence(jptRecoTauProducer*
                          caloRecoTauProducer *
                          caloRecoTauDiscriminationByLeadingTrackFinding *
                          caloRecoTauDiscriminationByLeadingTrackPtCut *
                          caloRecoTauDiscriminationByIsolation *
                          caloRecoTauDiscriminationAgainstElectron 
#                          tcRecoTauDiscriminationAgainstHadronicJets
)

