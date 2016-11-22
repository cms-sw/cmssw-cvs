#!/bin/tcsh

cmsenv

rehash


#set InputFileGENSIM = file:/scratch/cms/TTbarGenSim31X.root
set InputFileGENSIM = rfio:/castor/cern.ch/user/g/gruen/cms/TTbarGenSim31X.root

# global tags to be used
#set GTAGUP = FrontierConditions_GlobalTag,START36_V1::All
#set GTAGMC = FrontierConditions_GlobalTag,MC_36Y_V1::All
set GTAGUP = auto:startup
set GTAGMC = auto:mc


# two-table workflows
foreach prod ( RelVal MCProd )

  if ( $prod == RelVal ) then
    set XEVT = FEVTDEBUGHLT
  else if ( $prod == MCProd ) then
    set XEVT = RAWDEBUG
  else
    set XEVT = FEVTDEBUGHLT
  endif

  echo " "
  echo "Creating ${prod}_8E29"
cmsDriver.py $prod --step=DIGI,L1,DIGI2RAW,HLT --conditions=$GTAGUP --filein=$InputFileGENSIM                       --fileout=RelVal_${prod}_8E29.root      --number=100 --mc --no_exec --datatier 'GEN-SIM-DIGI-RAW-HLT' --eventcontent=$XEVT --customise=HLTrigger/Configuration/customL1THLT_Options.py   --python_filename=RelVal_${prod}_8E29.py --processName=HLT8E29

  echo " "
  echo "Creating ${prod}_1E31"
cmsDriver.py $prod --step=DIGI,L1,DIGI2RAW,HLT --conditions=$GTAGMC --filein=file:RelVal_${prod}_8E29.root          --fileout=RelVal_${prod}_8E29_1E31.root --number=100 --mc --no_exec --datatier 'GEN-SIM-DIGI-RAW-HLT' --eventcontent=$XEVT --customise=HLTrigger/Configuration/customL1THLT_Options.py   --python_filename=RelVal_${prod}_1E31.py --processName=HLT

end


# specific workflows
foreach lumi ( 8E29 GRun 1E31 HIon ) 
  if ( $lumi == 8E29 ) then
    set XHLT = HLT
    set GTAG = $GTAGUP
  else if ( $lumi == GRun ) then
    set XHLT = HLT:GRun
    set GTAG = $GTAGUP
  else if ( $lumi == 1E31 ) then
    set XHLT = HLT
    set GTAG = $GTAGMC
  else if ( $lumi == HIon ) then
    set XHLT = HLT:HIon
    set GTAG = $GTAGMC
  else
    set XHLT = HLT
    set GTAG = $GTAGMC
  endif

  echo " "
  echo "Creating TTbarGenSim $lumi"
cmsDriver.py TTbar_Tauola.cfi --step=GEN,SIM                        --conditions=$GTAG                               --fileout=TTbarGenSim.root               --number=100 --mc --no_exec --datatier 'GEN-SIM'              --eventcontent=FEVTSIM      --customise=HLTrigger/Configuration/custom_Options.py         --python_filename=TTbarGenSim_$lumi.py

  echo " "
  echo "Creating TTbarGenHLT $lumi"
cmsDriver.py TTbar_Tauola.cfi --step=GEN,SIM,DIGI,L1,DIGI2RAW,$XHLT --conditions=$GTAG                               --fileout=TTbarGenHLT_$lumi.root         --number=100 --mc --no_exec --datatier 'GEN-SIM-DIGI-RAW-HLT' --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/customL1THLT_Options.py   --python_filename=TTbarGenHLT_$lumi.py

  echo " "
  echo "Creating DigiL1Raw $lumi"
cmsDriver.py RelVal --step=DIGI,L1,DIGI2RAW       --conditions=$GTAG --filein=$InputFileGENSIM                       --fileout=RelVal_DigiL1Raw_$lumi.root    --number=100 --mc --no_exec --datatier 'GEN-SIM-DIGI-RAW'     --eventcontent=RAW          --customise=HLTrigger/Configuration/customL1T_Options.py      --python_filename=RelVal_DigiL1Raw_$lumi.py

  echo " "
  echo "Creating DigiL1RawHLT $lumi"
cmsDriver.py RelVal --step=DIGI,L1,DIGI2RAW,$XHLT --conditions=$GTAG --filein=$InputFileGENSIM                       --fileout=RelVal_DigiL1RawHLT_$lumi.root --number=100 --mc --no_exec --datatier 'GEN-SIM-DIGI-RAW-HLT' --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/customL1THLT_Options.py   --python_filename=RelVal_DigiL1RawHLT_$lumi.py --processName=HLT

  echo " "
  echo "Creating HLT $lumi"
cmsDriver.py RelVal --step=$XHLT                  --conditions=$GTAG --filein=file:RelVal_DigiL1Raw_$lumi.root       --fileout=RelVal_HLT_$lumi.root          --number=100 --mc --no_exec --datatier 'RAW-HLT'              --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/customL1THLT_Options.py   --python_filename=RelVal_HLT_$lumi.py          --processName=HLT

  echo " "
  echo "Creating HLT2 (re-running HLT) $lumi"
cmsDriver.py RelVal --step=$XHLT                  --conditions=$GTAG --filein=file:RelVal_HLT_$lumi.root             --fileout=RelVal_HLT2_$lumi.root         --number=100 --mc --no_exec --datatier 'RAW-HLT'              --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/customL1THLT_Options.py   --python_filename=RelVal_HLT2_$lumi.py         --processName=HLT2

  echo " "
  echo "Creating L1HLT2 (re-running L1 and HLT - buggy!) $lumi"
cmsDriver.py RelVal --step=L1,$XHLT               --conditions=$GTAG --filein=file:RelVal_DigiL1RawHLT_$lumi.root    --fileout=RelVal_L1HLT2_$lumi.root       --number=100 --mc --no_exec --datatier 'GEN-SIM-DIGI-RAW-HLT' --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/customL1THLT2_Options.py  --python_filename=RelVal_L1HLT2_$lumi.py       --processName=HLT2

  echo " "
  echo "Creating Reco $lumi"
cmsDriver.py RelVal --step=RAW2DIGI,L1Reco,RECO   --conditions=$GTAG --filein=file:RelVal_DigiL1Raw_$lumi.root        --fileout=RelVal_Reco_$lumi.root        --number=100 --mc --no_exec --datatier 'RECO'                 --eventcontent=RECOSIM      --customise=HLTrigger/Configuration/custom_Options.py         --python_filename=RelVal_Reco_$lumi.py

end
