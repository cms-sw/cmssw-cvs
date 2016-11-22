#ifndef VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_H
# define VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"

# include <string>

class VisSoMaterialDetails;

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4Filter
{
public:
    enum CompositionOperations
    {
	AND,
	OR,
	XOR
    };    
    
    VisG4Filter (CompositionOperations op = OR);
    virtual ~VisG4Filter (void) {};

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual std::string name (void) = 0;    
    virtual bool isToBeDrawn (const VisSoMaterialDetails &details) = 0;
    virtual bool compute (const VisSoMaterialDetails &details);
    void addFilter (VisG4Filter *filter);
    void setCompositionOperation (CompositionOperations op);
    int getCompositionOperation (void);
    VisG4Filter &operator<< (VisG4Filter &filter);
    void finalizeInitialization (void);    
protected:
    VisG4Filter *m_nextFilter;
private:    
    int m_op;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_H
