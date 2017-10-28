#!/bin/tcsh

cmsenv
rehash

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
  foreach table ( GRun 5E33v4 HIon )
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
echo "Fixup of RelVal_L1RePack_HIon_DATA.py - should be done via cmsDriver's ConfigBuilder"
cat >> RelVal_L1RePack_HIon_DATA.py <<EOF
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.SimL1Emulator,"rawDataCollector","rawDataRepacker",False)
EOF

echo
echo "Running selected cfg files from:"
pwd

rm -f                         ./runOne.log 
time  ./runOne.sh DATA     >& ./runOne.log &
time  ./runOne.sh STARTUP

  set N = 0
  cp -f ./runOne.log ./runOne.tmp  
  grep -q Finished   ./runOne.tmp
  set F = $?

while ( $F )
  awk "{if (NR>$N) {print}}"  ./runOne.tmp
  set N = `cat ./runOne.tmp | wc -l`
  sleep 13
  cp -f ./runOne.log ./runOne.tmp  
  grep -q Finished   ./runOne.tmp
  set F = $?
end

wait

  awk "{if (NR>$N) {print}}"  ./runOne.log
  rm -f ./runOne.{log,tmp}

echo
echo "Resulting log files:"
ls -l *.log
