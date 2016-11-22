source blob_preparation.txt
sed -i '{s/TagXX/Tag340V4/g}' *.py
sed -i '{s/TagXX/Tag340V4/g}' *.txt
sed -i '{s/TagXX/Tag340V4/g}' *.sh
cmsRun geometryxmlwriterExtended.py
cmsRun geometrywriter.py
cmsRun geometryxmlwriterExtendedGFlash.py
cmsRun geometryxmlwriterIdeal.py
cmsRun geometryxmlwriterIdealGFlash.py
#NOTE: the first one (ge.xml) is not written here ... it is written by geometrywriter.py above.
sed -i '{s/Extended/ExtendedGFlash/g}' xmlgeometrywriter.py
sed -i '{s/\/ge/\/geg/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
#cat xmlgeometrywriter.py
sed -i '{s/Extended/Ideal/g}' xmlgeometrywriter.py
sed -i '{s/\/geg/\/gig/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
#cat xmlgeometrywriter.py
sed -i '{s/IdealGFlash/Ideal/g}' xmlgeometrywriter.py
sed -i '{s/\/gig/\/gi/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
#cat xmlgeometrywriter.py
source cmscondExport.sh
source renameALL.sh

