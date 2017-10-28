#! /bin/tcsh

cmsenv

rehash

# gen sim input for MonteCarlo Simulation tests
#set InputFileGENSIM = file:/scratch/cms/TTbarGenSim3110.root
#set InputFileGENSIM = rfio:/castor/cern.ch/user/g/gruen/cms/TTbarGenSim3110.root
set InputFileGENSIM = root://castorcms//castor/cern.ch/user/g/gruen/cms/TTbarGenSim3110.root

# raw input for Real-Data tests (use same real-data raw file as in confdb.py!)
set InputFileRealData = /store/data/Run2011B/MinimumBias/RAW/v1/000/178/479/3E364D71-F4F5-E011-ABD2-001D09F29146.root

# global tags to be used for PP and HIon running
set GTAGPPUP = auto:startup
set GTAGPPMC = auto:mc
set GTAGPPRD = auto:hltonline
set GTAGHIUP = auto:starthi
set GTAGHIMC = auto:mc      # MC39_V4HI::All
set GTAGHIRD = auto:hltonline

set XL1T   = "" # syntax: tag,record[,connect,label]
set XL1TPP = "L1GtTriggerMenu_L1Menu_Collisions2012_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T"
set XL1THI = "L1GtTriggerMenu_L1Menu_CollisionsHeavyIons2011_v0_mc,L1GtTriggerMenuRcd,frontier://FrontierProd/CMS_COND_31X_L1T"
#set XL1TPP = "L1GtTriggerMenu_L1Menu_Collisions2012_v0_mc,L1GtTriggerMenuRcd,sqlite_file:/afs/cern.ch/user/g/ghete/public/L1Menu/L1Menu_Collisions2012_v0/sqlFile/L1Menu_Collisions2012_v0_mc.db"
#set XL1THI = "L1GtTriggerMenu_L1Menu_CollisionsHeavyIons2011_v0_mc,L1GtTriggerMenuRcd,sqlite_file:/afs/cern.ch/user/g/ghete/public/L1Menu/L1Menu_CollisionsHeavyIons2011_v0/sqlFile/L1Menu_CollisionsHeavyIons2011_v0_mc.db"

# specific workflows, first varying the globaltags, then the hlt tables

foreach gtag ( STARTUP DATA )
#foreach gtag ( DATA STARTUP MC )
  if ( $gtag == DATA ) then
    set GTAG   = $GTAGPPRD
    set GTAGPP = $GTAGPPRD
    set GTAGHI = $GTAGHIRD
    set DATAMC = --data
  else  if ( $gtag == STARTUP ) then
    set GTAG   = $GTAGPPUP
    set GTAGPP = $GTAGPPUP
    set GTAGHI = $GTAGHIUP
    set DATAMC = --mc
  else if ( $gtag == MC ) then
    set GTAG   = $GTAGPPMC
    set GTAGPP = $GTAGPPMC
    set GTAGHI = $GTAGHIMC
    set DATAMC = --mc
  else
    # unsupported
    continue
  endif

  # workflows depending ONLY on the globaltag (and not on the HLT table)
  if ( $gtag != DATA ) then    
  echo
  echo "Creating TTbarGenSim $gtag"
    cmsDriver.py TTbar_Tauola_8TeV_cfi --step=GEN,SIM                        --conditions=$GTAG                                                                        --fileout=TTbarGenSim_$gtag.root         --number=100 $DATAMC --no_exec --datatier 'GEN-SIM'              --eventcontent=FEVTSIM      --customise=HLTrigger/Configuration/CustomConfigs.Base    --python_filename=TTbarGenSim_$gtag.py
  endif

  foreach table ( GRun HIon )

    set name = ${table}_${gtag}  
    set FILEIN = file:RelVal_Raw_$name.root

    if ( $table == GRun ) then
      set XL1T = $XL1TPP
      set XHLT = HLT:GRun
      set GTAG = $GTAGPP
    else if ( $table == HIon ) then
      set XL1T = $XL1THI
      set XHLT = HLT:HIon
      set GTAG = $GTAGHI
    else
      # unsupported
      continue
    endif

    if ( $gtag == DATA ) then
    echo
    echo "Creating L1RePack $name"
    cmsDriver.py RelVal                --step=L1REPACK                       --conditions=$GTAG --filein=$InputFileRealData                  --custom_conditions=$XL1T --fileout=RelVal_L1RePack_$name.root     --number=100 $DATAMC --no_exec --datatier 'GEN-SIM-DIGI-RAW'     --eventcontent=RAW          --customise=HLTrigger/Configuration/CustomConfigs.L1T     --python_filename=RelVal_L1RePack_$name.py

    else
    echo
    echo "Creating TTbarGenToHLT $name"
    cmsDriver.py TTbar_Tauola_8TeV_cfi --step=GEN,SIM,DIGI,L1,DIGI2RAW,$XHLT --conditions=$GTAG                                              --custom_conditions=$XL1T --fileout=TTbarGenHLT_$name.root         --number=100 $DATAMC --no_exec --datatier 'GEN-SIM-DIGI-RAW-HLT' --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/CustomConfigs.L1THLT  --python_filename=TTbarGenToHLT_$name.py

    echo
    echo "Creating DigiL1Raw $name"
    cmsDriver.py RelVal                --step=DIGI,L1,DIGI2RAW               --conditions=$GTAG --filein=$InputFileGENSIM                    --custom_conditions=$XL1T --fileout=RelVal_DigiL1Raw_$name.root    --number=100 $DATAMC --no_exec --datatier 'GEN-SIM-DIGI-RAW'     --eventcontent=RAW          --customise=HLTrigger/Configuration/CustomConfigs.L1T     --python_filename=RelVal_DigiL1Raw_$name.py

    echo
    echo "Creating DigiL1RawHLT $name"
    cmsDriver.py RelVal                --step=DIGI,L1,DIGI2RAW,$XHLT         --conditions=$GTAG --filein=$InputFileGENSIM                    --custom_conditions=$XL1T --fileout=RelVal_DigiL1RawHLT_$name.root --number=100 $DATAMC --no_exec --datatier 'GEN-SIM-DIGI-RAW-HLT' --eventcontent=RAW          --customise=HLTrigger/Configuration/CustomConfigs.L1THLT  --python_filename=RelVal_DigiL1RawHLT_$name.py  --processName=HLT

    if ( $table == GRun) then
    echo
    echo "Creating FastSim $name"
    cmsDriver.py TTbar_Tauola_8TeV_cfi --step GEN,FASTSIM,HLT:GRun           --conditions=$GTAG                                              --custom_conditions=$XL1T --fileout=FastSim_GenToHLT_$name.root    --number=100 $DATAMC --no_exec --datatier 'GEN-SIM-DIGI-RECO'    --eventcontent FEVTDEBUGHLT --customise=HLTrigger/Configuration/CustomConfigs.FASTSIM --python_filename=FastSim_GenToHLT_$name.py     --processName=HLT
    endif

    endif

    echo
    echo "Creating HLT $name"
    cmsDriver.py RelVal                --step=$XHLT                          --conditions=$GTAG --filein=$FILEIN                             --custom_conditions=$XL1T --fileout=RelVal_HLT_$name.root          --number=100 $DATAMC --no_exec --datatier 'RAW-HLT'              --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/CustomConfigs.L1THLT  --python_filename=RelVal_HLT_$name.py           --processName=HLT1

    echo
    echo "Creating HLT2 (re-running HLT) $name"
    cmsDriver.py RelVal                --step=$XHLT                          --conditions=$GTAG --filein=file:RelVal_HLT_$name.root          --custom_conditions=$XL1T --fileout=RelVal_HLT2_$name.root         --number=100 $DATAMC --no_exec --datatier 'RAW-HLT'              --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/CustomConfigs.L1THLT  --python_filename=RelVal_HLT2_$name.py          --processName=HLT2

    echo
    echo "Creating HLT+RECO $name"
    cmsDriver.py RelVal                --step=$XHLT,RAW2DIGI,L1Reco,RECO     --conditions=$GTAG --filein=$FILEIN                             --custom_conditions=$XL1T --fileout=RelVal_HLT_RECO_$name.root     --number=100 $DATAMC --no_exec --datatier 'RAW-HLT-RECO'         --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/CustomConfigs.L1THLT  --python_filename=RelVal_HLT_RECO_$name.py      --processName=HLT1

  end
end
