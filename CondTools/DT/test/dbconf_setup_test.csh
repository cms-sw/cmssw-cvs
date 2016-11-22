echo "============ build pool mapping ============"
source pool_build_conf_mapping_test.csh
echo "============ db init ============"
cmsRun dbtest_init.py
echo "============ db dump ============"
cmsRun dbtest_dump.py
