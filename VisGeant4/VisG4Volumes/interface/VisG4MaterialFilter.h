#ifndef VIS_G4_VOLUMES_VIS_G4_MATERIAL_FILTER_H
# define VIS_G4_VOLUMES_VIS_G4_MATERIAL_FILTER_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"
# include <map>
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoMaterial;
class G4Material;
class SoMaterial;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Abstract class of a filter which performs the conversion between
   G4Material and SoMaterial.
 */
class VIS_G4_VOLUMES_API VisG4MaterialFilter
{
public:
    VisG4MaterialFilter (bool enabled = false);    
    virtual ~VisG4MaterialFilter (void) {};
 
    /* This is what we want it to do in the end. Caching this is not easy at all.
     * 
     * VisSoMaterialDetails list; // = DetailValue (= void *) of size [N], each detail has a fixed index
     * list.add (new VisG4RepDetail (object));
     * list.add (new VisG4MaterialDetail (mat));
     * list.add (new VisG4PVDetail (pv));
     * list.add (new VisG4PathDetail (path));
     * SoMaterial *mat = matfilter->compute (list);
     *
     * list = { RepDetail (object), MatDetail (mat), ... };
     *  matfilter->compute (IgSoMaterialDetails (object) << ... << ... );
     * 
     */
    
    virtual SoMaterial *getSoMaterial (const VisSoMaterialDetails &details, 
				       SoMaterial *soMat) = 0;
    /** Symbolic name for the filter. This will go away as soon as the
     * filters move to be full fledged plugins*/
    virtual const std::string name (void) = 0;
    
    /**Appends another filter to the filter chain*/
    void appendFilter (VisG4MaterialFilter *filter);
    SoMaterial *next (const VisSoMaterialDetails &details, SoMaterial *soMat);
    /**@return true if the filter is enabled*/
    bool enabled (void);
    void setEnabled (bool enabled);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    typedef std::list<VisG4MaterialFilter *> 	VisG4MaterialFilterList;
    
    static VisG4MaterialFilterList &filterList (void);    
    
private:
    VisG4MaterialFilter *m_nextFilter;    
    bool		m_enabled;    
    static VisG4MaterialFilterList		m_filterList;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4MATERIAL_FILTER_H
