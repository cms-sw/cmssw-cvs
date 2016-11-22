#!/bin/tcsh

cmsenv

rehash

echo " "
echo "Removing prescales from online configs"
foreach task ( OnData OnLine)
foreach lumi ( 8E29 GRun 1E31 HIon )
cat >> ${task}_HLT_${lumi}.py <<EOF
#
# Removing prescales
if 'PrescaleService' in process.__dict__:
    process.PrescaleService.prescaleTable = cms.VPSet( )
# 
EOF
end
end

echo " "
echo "Creating offline configs with cmsDriver"
echo "./cmsDriver.sh"
time  ./cmsDriver.sh

#foreach lumi ( GRun )
#foreach task ( OnLine_HLT RelVal_HLT RelVal_HLT2 ) 
#cat >> ${task}_${lumi}.py <<EOF
##
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
##
#EOF
#end
#end

# GRun = 8E29+MWGR
foreach lumi ( 8E29 GRun 1E31 HIon )
# foreach task ( RelVal_DigiL1Raw RelVal_HLT OnLine_HLT RelVal_DigiL1RawHLT RelVal_HLT2 RelVal_L1HLT2 RelVal_Reco )
  foreach task ( RelVal_DigiL1Raw RelVal_HLT OnLine_HLT RelVal_HLT2 OnData_HLT )
    echo " "
    set name = ${task}_${lumi}
    foreach ext (log root)
	/bin/rm $name.$ext
    end
    echo "cmsRun $name.py >& $name.log"
    time  cmsRun $name.py >& $name.log
  end
end

foreach lumi ( 8E29 1E31 )
  foreach task ( RelVal_Reco )
    echo " "
    set name = ${task}_${lumi}
    foreach ext (log root)
	/bin/rm $name.$ext
    end
    echo "cmsRun $name.py >& $name.log"
    time  cmsRun $name.py >& $name.log
  end
end
