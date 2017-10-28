#!/bin/tcsh

cmsenv
rehash

set rawLHC = L1RePack
set rawSIM = DigiL1Raw

echo
echo Starting $0 $1

foreach gtag ( $1 )

  foreach table ( GRun 5E33v4 HIon )

    if ($gtag == DATA) then
      set base = RelVal_${rawLHC}
    else
      set base = RelVal_${rawSIM}
    endif

#   run workflows

    set base = ( $base ONLINE_HLT RelVal_HLT RelVal_HLT2 )

    if ( $gtag == STARTUP ) then
      if ( $table != HIon ) then
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

if ( $1 == STARTUP ) then
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
endif

# separate hlt+reco and reco+(validation)+dqm workflows

foreach gtag ( $1 )

  foreach table ( GRun 5E33v4 HIon )

    if ($gtag == DATA) then
      set base = ( RelVal_HLT_Reco                     RelVal_RECO )
    else
      set base = ( RelVal_HLT_Reco RelVal_DigiL1RawHLT RelVal_RECO )
    endif

    foreach task ( $base )

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

echo
echo Finished $0 $1
#
