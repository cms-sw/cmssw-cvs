#ifndef VIS_CSC_CHAMBER_DRAWER_H
#define VIS_CSC_CHAMBER_DRAWER_H

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

class SoSeparator;
class SoMaterial;
class SoDrawStyle;
/*!
 *
 *   class VisCSCChamberDrawer
 *   \brief draws the outline of a chamber
 *
 *   \date 15-SEP-2008
 */
 
class VisCSCChamberDrawer
{
public:
    /*! Constructor:
     * \param material: used to supply color parameters. Only uses diffuse value.
     * \param line: used to supply line parameters: Only thickness used for now.
     */
    explicit VisCSCChamberDrawer(SoMaterial *material=0, SoDrawStyle *style=0);
    /*! method to attach the chamber outline to the passed node
     * \param sep: the passed node
     * \param pGeom: the handle to the geometry for the chambers
     * \param id: id for this chamber
     */
    bool getOutline(SoSeparator *sep, const edm::ESHandle<CSCGeometry>  pGeom,
    const CSCDetId id); //! returns the outline of the layer
private:
        const static edm::ParameterSet* m_pset;
	uint32_t m_lineColor;
	float m_lineThickness;
	bool m_chamberFlag; //! true to enable the outlines
};

#endif //VIS_CSC_CHAMBER_DRAWER_H

