#!/bin/sh

function die { echo $1: status $2; exit $2; }

CONFIGS=("run_MockHadronizerGeneratorFilter_cfg.py")

for config in $CONFIGS; do
    cmsRun --parameter-set ${LOCAL_TEST_DIR}/${config} || die "Failed in ${config}" $?
done
