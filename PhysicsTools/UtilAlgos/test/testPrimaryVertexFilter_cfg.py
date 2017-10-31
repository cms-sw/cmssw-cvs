import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    "/store/relval/CMSSW_5_0_0_pre6/RelValZTT/GEN-SIM-RECO/START50_V5-v1/0202/A0D83694-4917-E111-85E1-0026189438F2.root"
  )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.MessageLogger = cms.Service("MessageLogger")

## ---
## This is an example of the use of the EDFilterWrapper to exploit C++ selector classes as defined in the
## PhysicsTools/SelectorUtils package wrapping them into an EDFilter. You can find the implementation of
## the EDFilter in PhysicsTools/UtilAlgos/plugins/PrimaryVertexFilter.cc. You can find the EDfilterWrapper
## class in PhysicsTools/UtilAlgos/interface/EDFilterWrapper.h. The class that has been exploited here is 
## the PVSelector class of the PhysicsTools/SelectorUtils package, which makes implicit use of the
## PVObjectSelector class in the same package.You cna find the input parameters there. 
process.primaryVertexFilter = cms.EDFilter("PrimaryVertexFilter",
  pvSrc   = cms.InputTag("offlinePrimaryVertices"),
  minNdof = cms.double( 4 ),
  maxZ    = cms.double( 24.0 ),
  maxRho  = cms.double(2.0),
  NPV     = cms.int32(1)
)

process.p = cms.Path(process.primaryVertexFilter)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
