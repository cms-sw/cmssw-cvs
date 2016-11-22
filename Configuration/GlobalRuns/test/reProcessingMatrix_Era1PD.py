
def Era_1PDs():
    alcaMap={}
    alcaMap['MinimumBias']='SiStripCalMinBias+SiStripCalZeroBias+TkAlMinBias+TkAlMuonIsolated+MuAlCalIsolatedMu+MuAlOverlaps+HcalCalIsoTrk+HcalCalDijets+DtCalib+EcalCalElectron'
    return alcaMap

com='cmsDriver.py reco -s RAW2DIGI,L1Reco,RECO,DQM%s  --data --magField AutoFromDBCurrent --scenario pp --datatier RECO --eventcontent RECO --customise Configuration/GlobalRuns/customise_Collision_36X.py --no_exec --python_filename=rereco_%sCollision_36X.py --conditions %s'

import os

GT='GR_R_36X_V11A::All'
alcaMap=Era_1PDs()
for PD in alcaMap.keys():
    if (PD==''):
        alca=''
        spec=''
    else:
        alca=',ALCA:%s'%(alcaMap[PD],)
        spec=PD+"_"
    os.system(com%(alca,spec,GT))


