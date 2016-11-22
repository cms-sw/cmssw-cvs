#ifndef VIS_G4_VOLUMES_VIS_G4_VOLUME_TWIG_H
# define VIS_G4_VOLUMES_VIS_G4_VOLUME_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"
# include "VisGeant4/VisG4Core/interface/VisG4Path.h"
# include "Iguana/Framework/interface/IgTwig.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class G4VPhysicalVolume;
class VisG4MaterialFilter;
class VisG4Filter;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Twig representing a Geant4 detector volume.  */
class VIS_G4_VOLUMES_API VisG4VolumeTwig : public IgCompoundTwig
{
public:
    VisG4VolumeTwig (IgTwig		*parent,
		    G4VPhysicalVolume	*in,
		    bool		traverse = false,
		    bool		visible = false,
		    bool		open = false,
		    bool		logical = true);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    //VisG4Path			path (void) const;
    unsigned			paths (void);
    VisG4Path			path (unsigned index);

    G4VPhysicalVolume *		volume (void) const;
    VisG4VolumeTwig *		select (const VisG4Path &path,
					bool autoexpand = false);

    unsigned			physicalChildren (void);
    IgTwig *			physicalChild (unsigned index);

    virtual std::string		name (void) const;
    virtual void		name (const std::string &value);

    virtual bool		expand (void);

    virtual bool		selfTraverse (void) const;
    virtual void		selfTraverse (bool value);

    virtual bool		selfVisible (void) const;
    virtual void		selfVisible (bool value);

    virtual bool		cascade (void) const;
    virtual void		cascade (bool value);

    virtual bool		logical (void) const;
    virtual void		logical (bool value);

    static VisG4MaterialFilter		*m_materialFilter;    
    static VisG4Filter 			*m_visualizationFilter;    
private:
    static const unsigned int	TRAVERSE = 2;
    static const unsigned int	VISIBLE = 4;
    static const unsigned int	CASCADE = 8;
    static const unsigned int	LOGICAL = 16;

    G4VPhysicalVolume		*m_volume;
    std::vector<VisG4Path>	m_paths;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4_VOLUME_TWIG_H
