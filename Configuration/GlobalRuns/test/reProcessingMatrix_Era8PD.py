def Era_8PDs():
    alcaMap={}
    alcaMap['']=''
    alcaMap['ZeroBias']='SiStripCalZeroBias'
    alcaMap['MinimumBias']='SiStripCalZeroBias+SiStripCalMinBias+TkAlMinBias+HcalCalIsoTrk'
    alcaMap['EG']='EcalCalElectron'
    alcaMap['Mu']='MuAlCalIsolatedMu+MuAlOverlaps+TkAlMuonIsolated+DtCalib'
    alcaMap['JetMETTau']='HcalCalDijets'
    #alcaMap['AlCaP0']='EcalCalPi0Calib+EcalCalEtaCalib'
    #alcaMap['AlCaPhiSymEcal']='EcalCalPhiSym+DQM'
    #alcaMap['HcalNZS']='HcalCalMinBias'
    #alcaMap['Cosmics']='TkAlBeamHalo+MuAlBeamHaloOverlaps+MuAlBeamHalo+TkAlCosmics0T+MuAlStandAloneCosmics+MuAlGlobalCosmics+MuAlCalIsolatedMu+HcalCalHOCosmics'
    return alcaMap

def Era_2PDs():
    alcaMap={}
    alcaMap['']=''
    alcaMap['ZeroBias']='SiStripCalZeroBias'
    alcaMap['MinimumBias']='SiStripCalMinBias+SiStripCalZeroBias+TkAlMinBias+TkAlMuonIsolated+MuAlCalIsolatedMu+MuAlOverlaps+HcalCalIsoTrk+HcalCalDijets+DtCalib+EcalCalElectron'
    return alcaMap

com='cmsDriver.py reco -s RAW2DIGI,L1Reco,RECO,DQM%s  --data --magField AutoFromDBCurrent --scenario pp --datatier RECO --eventcontent RECO --customise Configuration/GlobalRuns/customise_Collision_37X.py --no_exec --python_filename=rereco_%sCollision_37X.py --conditions %s'

import os

GT='GR_R_37X_V5A::All'
alcaMap=Era_8PDs()
for PD in alcaMap.keys():
    if (PD==''):
        alca=''
        spec=''
    else:
        alca=',ALCA:%s'%(alcaMap[PD],)
        spec=PD+"_"
    os.system(com%(alca,spec,GT))


