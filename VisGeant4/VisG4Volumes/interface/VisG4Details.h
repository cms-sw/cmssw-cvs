
#ifndef VIS_G4_VOLUMES_IG_SO_MATERIAL_DETAILS_H
# define VIS_G4_VOLUMES_IG_SO_MATERIAL_DETAILS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"

# include <string>
# include <functional>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRep;
class G4Material;
class G4VPhysicalVolume;

class VisG4RepDetails;
class VisG4MaterialDetails;
class VisG4PVDetails;
class VisG4PathDetails;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4RepDetails
{
public:
    VisG4RepDetails (void);    
    VisG4RepDetails (const IgRep *t);
    VisG4RepDetails (const VisG4RepDetails &t);

    struct less
    {
	bool operator() (const VisG4RepDetails& x, 
			 const VisG4RepDetails& y) const;	
    };
    
    bool operator== (const VisG4RepDetails &t) const;        
    bool operator< (const VisG4RepDetails &t) const;        
};

class VIS_G4_VOLUMES_API VisG4MaterialDetails
{
public:
    VisG4MaterialDetails (void);    
    VisG4MaterialDetails (const G4Material *t);
    VisG4MaterialDetails (const VisG4MaterialDetails &t);
    
    std::string name (void) const;
    float density (void) const;
    
    struct less
    {
	bool operator () (const VisG4MaterialDetails& x, 
			  const VisG4MaterialDetails& y) const;	
    };

    bool operator== (const VisG4MaterialDetails &t) const;        
    bool operator< (const VisG4MaterialDetails &t) const;        

private:
    std::string m_name;
    float m_density;    
};


class VIS_G4_VOLUMES_API VisG4PVDetails
{
public:
    VisG4PVDetails (void);    
    VisG4PVDetails (const G4VPhysicalVolume *pv);    
    VisG4PVDetails (const VisG4PVDetails &t);

    bool sensitive () const;
    
    struct less
    {
	bool operator() (const VisG4PVDetails& x, 
			 const VisG4PVDetails& y);	
    };

    bool operator== (const VisG4PVDetails &t) const;        
    bool operator< (const VisG4PVDetails &t) const;        
private:
    bool m_sensitive;    
};


class VIS_G4_VOLUMES_API VisG4PathDetails
{
public:
    VisG4PathDetails (void);
    VisG4PathDetails (const VisG4PathDetails &t);
    VisG4PathDetails (const std::string &path);
    
    const std::string &path (void) const;    

    struct less
    {
	bool operator() (const VisG4PathDetails& x, 
			 const VisG4PathDetails& y);
    };
        
    bool operator== (const VisG4PathDetails &t) const; 
    bool operator< (const VisG4PathDetails &t) const; 
private:
    std::string 	m_path;    
};


class VIS_G4_VOLUMES_API VisSoMaterialDetails
{
public:
    VisSoMaterialDetails (void);

    VisSoMaterialDetails &operator<< (const VisG4RepDetails &detail);
    VisSoMaterialDetails &operator<< (const VisG4MaterialDetails &detail);
    VisSoMaterialDetails &operator<< (const VisG4PVDetails &detail);
    VisSoMaterialDetails &operator<< (const VisG4PathDetails &detail);

    VisG4RepDetails repDetails (void) const;
    VisG4MaterialDetails g4MaterialDetails (void) const;
    VisG4PVDetails pvDetails (void) const;
    VisG4PathDetails pathDetails (void) const;

    struct less
    {
	bool operator() (const VisSoMaterialDetails& x,
			 const VisSoMaterialDetails& y);	
    };
    
    bool operator== (const VisSoMaterialDetails &a) const;    
private:
    VisG4RepDetails m_repDetails;
    VisG4MaterialDetails m_g4MaterialDetails;
    VisG4PVDetails m_pvDetails;
    VisG4PathDetails m_pathDetails;

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_IG_SO_MATERIAL_DETAILS_H
