#ifndef VIS_CSC_CONFIG_SERVICE_H
#define VIS_CSC_CONFIG_SERVICE_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"

/*!
 *    \class VisCSCConfigurationService
 *    \brief Configuration of materials and style for displaying the Muon CSC 
 *           related items.
 *   
 *    A direct copy of VisFramework/VisConfigurationService
 *
 *    \author G. Alverson, Northeastern University/CMS
 *    \date 2008/09/08
 *
 */

class VisCSCConfigurationService
{
public:
    VisCSCConfigurationService (const edm::ParameterSet& pset);
    ~VisCSCConfigurationService ();

    static const edm::ParameterSet* pSet ();

private:
    static edm::ParameterSet* m_pSet;
};

#endif // VIS_CSC_CONFIG_SERVICE_H
