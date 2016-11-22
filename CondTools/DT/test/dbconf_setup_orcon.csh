echo "============ build pool mapping ============"
source pool_build_conf_mapping_orcon.csh
echo "============ db init ============"
cmsRun orcon_conf_init.py
echo "============ db dump ============"
cmsRun orcon_conf_dump.py
