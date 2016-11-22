#! /bin/bash

echo
echo "Removing prescales from online configs"
for task in OnData OnLine; do
  for lumi in 8E29 GRun 1E31 HIon; do
    cat >> ${task}_HLT_${lumi}.py <<EOF

# Removing prescales
if 'PrescaleService' in process.__dict__:
    process.PrescaleService.prescaleTable = cms.VPSet( )

EOF
  done
done

echo
echo "Creating offline configs with cmsDriver"
echo "./cmsDriver.sh"
      ./cmsDriver.sh

#for lumi in GRun; do
#  for task in OnLine_HLT RelVal_HLT RelVal_HLT2; do 
#  cat >> ${task}_${lumi}.py <<EOF
#
## L1 not yet in GlobalTag - add by hand!
#from CondCore.DBCommon.CondDBSetup_cfi import *
#process.newL1menu = cms.ESSource(
#   "PoolDBESSource",CondDBSetup,
#   connect = cms.string("frontier://FrontierProd/CMS_COND_31X_L1T"),
#   toGet = cms.VPSet(cms.PSet(record = cms.string("L1GtTriggerMenuRcd"),
#                              tag = cms.string("L1GtTriggerMenu_STARTUP_v6"))
#                     )
#   )
#process.es_prefer_newL1menu = cms.ESPrefer("PoolDBESSource","newL1menu")
#
#EOF
#  done
#done

# GRun = 8E29+MWGR
for lumi in 8E29 GRun 1E31 HIon; do
# for task in RelVal_DigiL1Raw RelVal_HLT OnLine_HLT RelVal_DigiL1RawHLT RelVal_HLT2 RelVal_L1HLT2 RelVal_Reco; do
  for task in RelVal_DigiL1Raw RelVal_HLT OnLine_HLT RelVal_HLT2 OnData_HLT; do
    echo
    name=${task}_${lumi}
    rm -f $name.{log,root}
    echo "cmsRun $name.py >& $name.log"
          cmsRun $name.py >& $name.log
  done &        # run the different lumi in parallel
done

wait            # wail for all process to complete

for lumi in 8E29 1E31; do
  for task in RelVal_Reco; do
    echo
    name=${task}_${lumi}
    rm -f $name.{log,root}
    echo "cmsRun $name.py >& $name.log"
          cmsRun $name.py >& $name.log
  done &        # run the different lumi in parallel
done

wait            # wail for all process to complete
