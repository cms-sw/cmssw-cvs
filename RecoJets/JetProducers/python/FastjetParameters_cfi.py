import FWCore.ParameterSet.Config as cms

# Generic parameters for PU subtraction
# $Id: FastjetParameters_cfi.py,v 1.2 2008/04/21 03:28:19 rpw Exp $
FastjetParameters = cms.PSet(

)
FastjetNoPU = cms.PSet(
    Active_Area_Repeats = cms.int32(0),
    GhostArea = cms.double(1.0),
    Ghost_EtaMax = cms.double(0.0),
    UE_Subtraction = cms.string('no')
)
FastjetWithPU = cms.PSet(
    Active_Area_Repeats = cms.int32(5),
    GhostArea = cms.double(0.01),
    Ghost_EtaMax = cms.double(6.0),
    UE_Subtraction = cms.string('yes')
)

