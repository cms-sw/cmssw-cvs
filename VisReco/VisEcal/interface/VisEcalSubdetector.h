#ifndef VIS_ECAL_VIS_ECAL_SUBDETECTOR_H
# define VIS_ECAL_VIS_ECAL_SUBDETECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEcalSubdetector
{
public:
    static int 			subDet (const std::string &key);
    static const std::string 	subDetName (EcalSubdetector key);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_ECAL_VIS_ECAL_SUBDETECTOR_H
