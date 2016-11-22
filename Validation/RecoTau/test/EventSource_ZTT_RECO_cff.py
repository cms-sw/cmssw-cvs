import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
      '/store/relval/CMSSW_3_3_0_pre5/RelValZTT/GEN-SIM-RECO/STARTUP31X_V7-v1/0004/F84CAD5C-70AB-DE11-86D7-001D09F29597.root',
      '/store/relval/CMSSW_3_3_0_pre5/RelValZTT/GEN-SIM-RECO/STARTUP31X_V7-v1/0004/BE3F743D-2EAB-DE11-A23B-0019B9F581C9.root',
      '/store/relval/CMSSW_3_3_0_pre5/RelValZTT/GEN-SIM-RECO/STARTUP31X_V7-v1/0004/BC2880D6-88AB-DE11-9493-001D09F2906A.root',
      '/store/relval/CMSSW_3_3_0_pre5/RelValZTT/GEN-SIM-RECO/STARTUP31X_V7-v1/0004/BA2ACD13-31AB-DE11-9CAD-001D09F29524.root',
      '/store/relval/CMSSW_3_3_0_pre5/RelValZTT/GEN-SIM-RECO/STARTUP31X_V7-v1/0004/B8D96A13-2DAB-DE11-9E0E-001D09F2906A.root',
      '/store/relval/CMSSW_3_3_0_pre5/RelValZTT/GEN-SIM-RECO/STARTUP31X_V7-v1/0004/38F5C43E-2FAB-DE11-95B9-001D09F28EA3.root' 
      ] )


secFiles.extend( [
   ] )

