#include "VisReco/VisMuonCSC/interface/VisCSCChamberDrawer.h"
#include "VisReco/VisMuonCSC/interface/VisCSCConfigurationService.h"

#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoCoordinate3.h>

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
const edm::ParameterSet*      VisCSCChamberDrawer::m_pset = 0;
 
VisCSCChamberDrawer::VisCSCChamberDrawer(SoMaterial *material, SoDrawStyle *style) :  m_lineColor(0x59595900), m_lineThickness(2.0F), m_chamberFlag(true)
{
        if (! m_pset)
        {
            edm::ParameterSet p;
            VisCSCConfigurationService visService(p);
            m_pset = visService.pSet ();
        }
        m_chamberFlag = m_pset->getUntrackedParameter<bool>("VisCSCChamberFlag",true);
	
	if (material)
	{
	    m_lineColor = (material->diffuseColor[0]).getPackedValue();
	}
	if (style)
	{
	    m_lineThickness = style->lineWidth.getValue();
	}
}

bool VisCSCChamberDrawer::getOutline(SoSeparator *sep, const edm::ESHandle<CSCGeometry>  pGeom, const CSCDetId id)
{
	if (!pGeom.isValid() || !m_chamberFlag) return false;
	
	SbColor lineColor;
        float transparency = 0.0f;
        lineColor.setPackedValue(static_cast<uint32_t>(m_lineColor), transparency);
        SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (lineColor); 
	mat->emissiveColor.setValue (lineColor); 
	SoDrawStyle *drawStyle = new SoDrawStyle;
	if (m_lineThickness != 0.F) drawStyle->lineWidth = m_lineThickness;
	drawStyle->style = SoDrawStyle::LINES;
	
	
	sep->addChild(mat);
	sep->addChild(drawStyle);
	
	const CSCLayer *pLayer = pGeom->layer(id);
	// FIXME: Not used:	const CSCChamber *pChamber = pGeom->chamber(id);
	if (!pLayer)
	{
	    const CSCChamber *chamber = pGeom->chamber(id);
	    pLayer = chamber->layer(4); // pick a default if not present
	    //assert (pLayer);
	}
	const CSCLayerGeometry *layerGeom = pLayer->geometry ();
	// params: half bottom edge, half top edge, half thickness, half apothem
//	const std::vector<float> trapezBounds = pChamber->specs()->parameters();
	const std::vector<float> trapezBounds = layerGeom->parameters ();
	const GeomDet *det = pGeom->idToDet(id);
	std::vector<Surface::GlobalPoint> crossPoint;
	// get vertices, converting from cm to meters
	crossPoint.push_back (det->surface ().toGlobal
			      (LocalPoint (-trapezBounds [0], -trapezBounds [3], 0.0)));
	crossPoint.push_back (det->surface ().toGlobal
			      (LocalPoint ( trapezBounds [0], -trapezBounds [3], 0.0)));
	crossPoint.push_back (det->surface ().toGlobal
			      (LocalPoint ( trapezBounds [1],  trapezBounds [3], 0.0)));
	crossPoint.push_back (det->surface ().toGlobal
			      (LocalPoint (-trapezBounds [1],  trapezBounds [3], 0.0)));
	std::vector<SbVec3f> pts(4);
	for (int i=0; i < 4; i++)
	{
		pts[i] = SbVec3f(crossPoint[i].x(), crossPoint[i].y(),	crossPoint[i].z())*0.01F;
	}	      
	SoCoordinate3 *vert = new SoCoordinate3;
	vert->point.setValues(0, 4, &pts[0]);
	sep->addChild(vert);
	SoFaceSet *trapez = new SoFaceSet;
	trapez->numVertices.setValue(4);
	sep->addChild (trapez);

    return true;
}

