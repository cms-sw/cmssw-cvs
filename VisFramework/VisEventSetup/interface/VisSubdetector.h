#ifndef VIS_EVENT_SETUP_VIS_SUBDETECTOR_H
# define VIS_EVENT_SETUP_VIS_SUBDETECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "DataFormats/DetId/interface/DetId.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisSubdetector
{
public:
    static const std::string detName	(DetId::Detector key);
    static const std::string subDetName (DetId::Detector det, int key);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_EVENT_SETUP_VIS_SUBDETECTOR_H
