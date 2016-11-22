import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.CondDBCommon.connect = 'sqlite_file:JEC_Summer09_7TeV_ReReco332.db'


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )
process.source = cms.Source("EmptySource")

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
            process.CondDBCommon,
            toPut = cms.VPSet(
              cms.PSet(record = cms.string('L2Relative_AK5Calo'),
                       tag    = cms.string('L2Relative_AK5Calo'),
                       label  = cms.string('L2Relative_AK5Calo')
              ),
              cms.PSet(record = cms.string('L3Absolute_AK5Calo'),
                       tag    = cms.string('L3Absolute_AK5Calo'),
                       label  = cms.string('L3Absolute_AK5Calo')
              )
            )
)

process.dbWriterL2AK5Calo = cms.EDAnalyzer("JetCorrectorDBWriter",
  inputTxtFile = cms.untracked.string('Summer09_7TeV_ReReco332_L2Relative_AK5Calo.txt'),
  option       = cms.untracked.string(''),
  label        = cms.untracked.string('L2Relative_AK5Calo')
)

process.dbWriterL3AK5Calo = cms.EDAnalyzer("JetCorrectorDBWriter",
  inputTxtFile = cms.untracked.string('Summer09_7TeV_ReReco332_L3Absolute_AK5Calo.txt'),
  option       = cms.untracked.string(''),
  label        = cms.untracked.string('L3Absolute_AK5Calo')
)

process.p = cms.Path(process.dbWriterL2AK5Calo * process.dbWriterL3AK5Calo)
