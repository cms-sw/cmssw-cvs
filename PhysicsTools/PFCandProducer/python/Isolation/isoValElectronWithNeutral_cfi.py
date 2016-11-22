import FWCore.ParameterSet.Config as cms

isoValElectronWithNeutral = cms.EDProducer(
    "CandIsolatorFromDeposits",
    deposits = cms.VPSet(
    cms.PSet(
    src = cms.InputTag("isoDepElectronWithNeutral"),
    deltaR = cms.double(0.4),
    weight = cms.string('0.3333'),
    vetos = cms.vstring(),
    skipDefaultVeto = cms.bool(True),
    mode = cms.string('sum')
    )
    )
    )


