#!/bin/tcsh

cmsenv
rehash

set rawLHC = L1RePack
set rawSIM = DigiL1Raw

echo
echo "Existing cfg files:"
ls -l On*.py

echo
echo "Creating ONLINE cfg files adding the HLTAnalyzerEndpath:"

foreach gtag ( Data Line )
  set GTAG = ` echo $gtag | tr "[a-z]" "[A-Z]" `
  if ( $GTAG == LINE ) then
    set GTAG = STARTUP
  endif
  foreach table ( GRun HIon )
    set oldfile = On${gtag}_HLT_${table}.py
    set newfile = ONLINE_HLT_${table}_${GTAG}.py
    rm -f $newfile
    cp $oldfile $newfile
    cat >> $newfile <<EOF
#
    if not ('HLTAnalyzerEndpath' in process.__dict__) :
        from HLTrigger.Configuration.HLT_FULL_cff import hltL1GtTrigReport,hltTrigReport
        process.hltL1GtTrigReport = hltL1GtTrigReport
        process.hltTrigReport = hltTrigReport
        process.hltTrigReport.HLTriggerResults = cms.InputTag( 'TriggerResults','',process.name_() )
        process.HLTAnalyzerEndpath = cms.EndPath(process.hltL1GtTrigReport + process.hltTrigReport)
#
EOF
  end
end

echo
echo "Created ONLINE cfg files:"
ls -l ON*.py

echo
echo "Creating offline cfg files with cmsDriver"
echo "./cmsDriver.sh"
time  ./cmsDriver.sh

echo
echo "Running selected cfg files from:"
pwd

foreach gtag ( STARTUP DATA )

  foreach table ( GRun HIon )

    if ($gtag == DATA) then
      set base = RelVal_${rawLHC}
    else
      set base = RelVal_${rawSIM}
    endif

#   run workflows

    set base = ( $base ONLINE_HLT RelVal_HLT RelVal_HLT2 )

    if ( $gtag == STARTUP ) then
      if ( $table == GRun) then
        set base = ( $base FastSim_GenToHLT )
      endif
    endif

    foreach task ( $base )

      echo
      set name = ${task}_${table}_${gtag}
      rm -f $name.{log,root}
      echo "cmsRun $name.py >& $name.log"
#     ls -l        $name.py
      time  cmsRun $name.py >& $name.log
      echo "exit status: $?"

      if ( ( $task == RelVal_${rawLHC} ) || ( $task == RelVal_${rawSIM} ) ) then
#       link to input file for subsequent steps
        rm -f              RelVal_Raw_${table}_${gtag}.root
        ln -s ${name}.root RelVal_Raw_${table}_${gtag}.root
      endif

    end

  end

end

# special fastsim integration test

foreach task ( IntegrationTestWithHLT_cfg )

  echo
  set name = ${task}
  rm -f $name.{log,root}

  if ( -f $CMSSW_BASE/src/FastSimulation/Configuration/test/$name.py ) then  
    echo "cmsRun "'$CMSSW_BASE'"/src/FastSimulation/Configuration/test/$name.py >& $name.log"
#   ls -l         $CMSSW_BASE/src/FastSimulation/Configuration/test/$name.py
    time  cmsRun  $CMSSW_BASE/src/FastSimulation/Configuration/test/$name.py >& $name.log
    echo "exit status: $?"
  else
    echo "cmsRun "'$CMSSW_RELEASE_BASE'"/src/FastSimulation/Configuration/test/$name.py >& $name.log"
#   ls -l         $CMSSW_RELEASE_BASE/src/FastSimulation/Configuration/test/$name.py
    time  cmsRun  $CMSSW_RELEASE_BASE/src/FastSimulation/Configuration/test/$name.py >& $name.log
    echo "exit status: $?"
  endif

end

# separate hlt+reco workflows to run last

foreach gtag ( STARTUP DATA )

  foreach table ( GRun HIon )

    foreach task ( RelVal_HLT_RECO )

      echo
      set name = ${task}_${table}_${gtag}
      rm -f $name.{log,root}
      echo "cmsRun $name.py >& $name.log"
#     ls -l        $name.py
      time  cmsRun $name.py >& $name.log
      echo "exit status: $?"

    end

  end

end

#

echo
echo "Resulting log files:"
ls -lt *.log
