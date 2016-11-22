#! /bin/bash

# clean
rm -f $CMSSW_BASE/src/FastSimulation/Configuration/python/HLT_{8E29,1E31,GRun}_cff.py
rm -f $CMSSW_BASE/src/FastSimulation/Configuration/python/blockHLT_{8E29,1E31,GRun}_cff.py

echo "Dumping GRun configuration"
./getFastSimHLT_GRun_cff.py $(head -1 HLTVersionGRun) "../python/HLT_GRun_cff.py" "../python/blockHLT_GRun_cff.py"

#echo "Dumping 8E29 configuration"
#./getFastSimHLT_8E29_cff.py $(head -1 HLTVersion8E29) "../python/HLT_8E29_cff.py" "../python/blockHLT_8E29_cff.py"

#echo "Dumping 1E31 configuration"
#./getFastSimHLT_1E31_cff.py $(head -1 HLTVersion1E31) "../python/HLT_1E31_cff.py" "../python/blockHLT_1E31_cff.py"
