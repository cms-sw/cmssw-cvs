#ifndef VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_TABLE_H
# define VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_TABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"
# include "VisGeant4/VisG4Volumes/interface/VisG4Filter.h"

# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4FilterTable
{
    typedef std::map<std::string, VisG4Filter *> FilterTable;
public:
    typedef FilterTable::iterator iterator;
    typedef FilterTable::const_iterator const_iterator;
    
    VisG4FilterTable (void);

    static VisG4FilterTable *instance (void);    
    void add (VisG4Filter *filter);
    const_iterator begin (void) const {return m_filterTable.begin ();};
    const_iterator end (void) const {return m_filterTable.end ();};
    iterator find (const char *name) {return m_filterTable.find (name);};    
    VisG4Filter *operator[] (const char *name) {return m_filterTable[std::string(name)];};    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    FilterTable m_filterTable;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_TABLE_H
