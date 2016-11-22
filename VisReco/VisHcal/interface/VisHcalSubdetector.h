#ifndef VIS_HCAL_VIS_HCAL_SUBDETECTOR_H
# define VIS_HCAL_VIS_HCAL_SUBDETECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisHcalSubdetector
{
public:
    static int subDet 		(const std::string &key);
    static int otherSubDet 	(const std::string &key);

    static const std::string subDetName 	(HcalSubdetector key);
    static const std::string otherSubDetName 	(HcalOtherSubdetector key);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_HCAL_VIS_HCAL_SUBDETECTOR_H
