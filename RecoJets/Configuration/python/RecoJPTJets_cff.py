
# $Id: RecoJPTJets_cff.py,v 1.1 2009/10/19 17:53:21 stadie Exp $
from JetMETCorrections.Configuration.JetPlusTrackCorrections_cff import *
from JetMETCorrections.Configuration.ZSPJetCorrections219_cff import *

#define jetPlusTrackZSPCorJet sequences
jetPlusTrackZSPCorJetIcone5   = cms.Sequence(ZSPJetCorrectionsIcone5+JetPlusTrackCorrectionsIcone5)
jetPlusTrackZSPCorJetSiscone5 = cms.Sequence(ZSPJetCorrectionsSisCone5+JetPlusTrackCorrectionsSisCone5)
jetPlusTrackZSPCorJetAntiKt5  = cms.Sequence(ZSPJetCorrectionsAntiKt5+JetPlusTrackCorrectionsAntiKt5)
 
recoJPTJets=cms.Sequence(jetPlusTrackZSPCorJetIcone5+jetPlusTrackZSPCorJetSiscone5+jetPlusTrackZSPCorJetAntiKt5)
