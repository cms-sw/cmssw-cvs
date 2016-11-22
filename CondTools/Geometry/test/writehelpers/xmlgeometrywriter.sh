#!/bin/tcsh
cmsenv
if ($#argv == 0) then
   setenv mytag "${mytag}"
else
   setenv mytag `echo ${1}`
endif
echo ${mytag}

#source blob_preparation.txt
sed -i {s/TagXX/${mytag}/g} *.py
sed -i {s/TagXX/${mytag}/g} *.txt
sed -i {s/TagXX/${mytag}/g} *.sh
sed -i {s/ge${mytag}/geTagXX/g} geometrywriter.py
sed -i {s/ge${mytag}/geTagXX/g} geometryxmlwriter.py
sed -i {s/ge${mytag}/geTagXX/g} xmlgeometrywriter.py
cmsRun geometryxmlwriter.py
cmsRun geometrywriter.py
sed -i '{s/Extended/ExtendedGFlash/g}' geometryxmlwriter.py
sed -i '{s/\/ge/\/geg/g}' geometryxmlwriter.py
cmsRun geometryxmlwriter.py
#sed -i '{s/Extended/Ideal/g}' geometryxmlwriter.py
#sed -i '{s/\/geg/\/gig/g}' geometryxmlwriter.py
#cmsRun geometryxmlwriter.py
#sed -i '{s/IdealGFlash/Ideal/g}' geometryxmlwritergen.py
#sed -i '{s/\/gig/\/gi/g}' geometryxmlwriter.py
sed -i '{s/ExtendedGFlash/Ideal/g}' geometryxmlwriter.py
sed -i '{s/\/geg/\/gi/g}' geometryxmlwriter.py
cmsRun geometryxmlwriter.py
sed -i '{s/Ideal/ExtendedX0Min/g}' geometryxmlwriter.py
sed -i '{s/\/gi/\/gexmin/g}' geometryxmlwriter.py
cmsRun geometryxmlwriter.py
sed -i '{s/X0Min/X0Max/g}' geometryxmlwriter.py
sed -i '{s/\/gexmin/\/gexmax/g}' geometryxmlwriter.py
cmsRun geometryxmlwriter.py
sed -i '{s/X0Max/LiMin/g}' geometryxmlwriter.py
sed -i '{s/\/gexmax/\/gelimin/g}' geometryxmlwriter.py
cmsRun geometryxmlwriter.py
sed -i '{s/LiMin/LiMax/g}' geometryxmlwriter.py
sed -i '{s/\/gelimin/\/gelimax/g}' geometryxmlwriter.py
cmsRun geometryxmlwriter.py
#NOTE: the first one (ge.xml) is not written here ... it is written by geometrywriter.py above.
sed -i '{s/Extended/ExtendedGFlash/g}' xmlgeometrywriter.py
sed -i '{s/\/ge/\/geg/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
#sed -i '{s/Extended/Ideal/g}' xmlgeometrywriter.py
#sed -i '{s/\/geg/\/gig/g}' xmlgeometrywriter.py
#cmsRun xmlgeometrywriter.py
#sed -i '{s/IdealGFlash/Ideal/g}' xmlgeometrywriter.py
#sed -i '{s/\/gig/\/gi/g}' xmlgeometrywriter.py
sed -i '{s/ExtendedGFlash/Ideal/g}' xmlgeometrywriter.py
sed -i '{s/\/geg/\/gi/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
sed -i '{s/Ideal/ExtendedX0Min/g}' xmlgeometrywriter.py
sed -i '{s/\/gi/\/gexmin/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
sed -i '{s/X0Min/X0Max/g}' xmlgeometrywriter.py
sed -i '{s/\/gexmin/\/gexmax/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
sed -i '{s/X0Max/LiMin/g}' xmlgeometrywriter.py
sed -i '{s/\/gexmax/\/gelimin/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
sed -i '{s/LiMin/LiMax/g}' xmlgeometrywriter.py
sed -i '{s/\/gelimin/\/gelimax/g}' xmlgeometrywriter.py
cmsRun xmlgeometrywriter.py
#cat xmlgeometrywriter.py
source cmscondExport.sh
#source renameALL.sh

