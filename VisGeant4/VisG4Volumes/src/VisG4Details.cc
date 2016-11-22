//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"
#include "Iguana/Framework/interface/IgRep.h"


#include <G4Material.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4RepDetails::VisG4RepDetails (void)
{
}

VisG4RepDetails::VisG4RepDetails (const IgRep */*t*/)
{
}

VisG4RepDetails::VisG4RepDetails (const VisG4RepDetails &/*t*/)
{
}

bool 
VisG4RepDetails::less::operator() (const VisG4RepDetails& /*x*/, 
				   const VisG4RepDetails& /*y*/) const
{
    return false;	    
}

bool 
VisG4RepDetails::operator== (const VisG4RepDetails &t) const
{
    less comparison;
    return !(comparison (*this, t) || comparison (t, *this));    
}

bool 
VisG4RepDetails::operator< (const VisG4RepDetails &t) const
{
    less comparison;
    return comparison (*this, t);    
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


VisG4MaterialDetails::VisG4MaterialDetails (void)
{
}

VisG4MaterialDetails::VisG4MaterialDetails (const G4Material *t)
{
    m_name = t->GetName ();    
    m_density = t->GetDensity ();    
}

VisG4MaterialDetails::VisG4MaterialDetails (const VisG4MaterialDetails &t)
{
    m_name = t.m_name;
    m_density = t.m_density;    
}

std::string
VisG4MaterialDetails::name (void) const
{
    return m_name;	    
}
    
float 
VisG4MaterialDetails::density (void) const
{
    return m_density;	    
}


bool 
VisG4MaterialDetails::less::operator() (const VisG4MaterialDetails& x,
					const VisG4MaterialDetails& y) const
{
    if (x.name () == y.name ())
	return x.density () < y.density ();
    return x.name () < y.name ();    
}

bool
VisG4MaterialDetails::operator== (const VisG4MaterialDetails &t) const
{
    less comparison;
    return !(comparison (*this, t) || comparison (t, *this));    
}        

bool
VisG4MaterialDetails::operator< (const VisG4MaterialDetails &t) const
{
    less comparison;
    return comparison (*this, t);    
}        

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

VisG4PVDetails::VisG4PVDetails (void)
    :m_sensitive (false)
{
}

VisG4PVDetails::VisG4PVDetails (const VisG4PVDetails &t)
    :m_sensitive (t.sensitive ())
{    
}

VisG4PVDetails::VisG4PVDetails (const G4VPhysicalVolume *pv)
    :m_sensitive (pv->GetLogicalVolume ()->GetSensitiveDetector ())
{
}

bool 
VisG4PVDetails::sensitive (void) const
{
    return m_sensitive;
}

bool 
VisG4PVDetails::less::operator() (const VisG4PVDetails& x, 
				       const VisG4PVDetails& y)
{
    return x.sensitive () < y.sensitive ();
}		

bool 
VisG4PVDetails::operator== (const VisG4PVDetails &t) const
{
    less comparison;
    return !(comparison (*this, t) || comparison (t, *this));    
}

bool 
VisG4PVDetails::operator< (const VisG4PVDetails &t) const
{
    less comparison;
    return comparison (*this, t);    
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

VisSoMaterialDetails::VisSoMaterialDetails (void)
{
}

VisSoMaterialDetails &
VisSoMaterialDetails::operator<< (const VisG4RepDetails &detail)
{
    m_repDetails = detail;    
    return *this;    
}

VisSoMaterialDetails &
VisSoMaterialDetails::operator<< (const VisG4MaterialDetails &detail)
{
    m_g4MaterialDetails = detail;    
    return *this;    
}

VisSoMaterialDetails &
VisSoMaterialDetails::operator<< (const VisG4PVDetails &detail)
{
    m_pvDetails = detail;    
    return *this;    
}

VisSoMaterialDetails &
VisSoMaterialDetails::operator<< (const VisG4PathDetails &detail)
{
    m_pathDetails = detail;    
    return *this;    
}

VisG4RepDetails
VisSoMaterialDetails::repDetails (void) const
{
    return m_repDetails;
}

VisG4MaterialDetails
VisSoMaterialDetails::g4MaterialDetails (void) const
{
    return m_g4MaterialDetails;
}

VisG4PVDetails 
VisSoMaterialDetails::pvDetails (void) const
{
    return m_pvDetails;
}

VisG4PathDetails 
VisSoMaterialDetails::pathDetails (void) const
{
    return m_pathDetails;
}

bool 
VisSoMaterialDetails::less::operator() (const VisSoMaterialDetails& x, const VisSoMaterialDetails& y)
{	
    if (x.m_repDetails == y.m_repDetails)
	if (x.m_pathDetails == y.m_pathDetails)
	    if (x.m_g4MaterialDetails == y.m_g4MaterialDetails)
		return x.m_pvDetails < y.m_pvDetails;
	    else
		return x.m_g4MaterialDetails < y.m_g4MaterialDetails;
	else
	    return x.m_pathDetails < y.m_pathDetails;
    else
	return x.m_repDetails < y.m_repDetails;
}

bool 
VisSoMaterialDetails::operator== (const VisSoMaterialDetails &t) const
{
    less comparison;
    
    return !(comparison (t, *this) || comparison (*this, t));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

VisG4PathDetails::VisG4PathDetails (void)
    :m_path ("")
{
}

VisG4PathDetails::VisG4PathDetails (const VisG4PathDetails &t)
{
    this->m_path = t.path ();	    
}        

VisG4PathDetails::VisG4PathDetails (const std::string &path)
{
    this->m_path = path;	    
}

const std::string &
VisG4PathDetails::path (void) const
{
    return m_path;
}    

bool 
VisG4PathDetails::less::operator() (const VisG4PathDetails& x, 
				    const VisG4PathDetails& y)
{
    if (x.path () < y.path ())
	return true;
    return false;
}
    
bool 
VisG4PathDetails::operator== (const VisG4PathDetails &t) const
{
    less comparison;
    return !(comparison (t, *this) || comparison (*this, t));
}

bool 
VisG4PathDetails::operator< (const VisG4PathDetails &t) const
{
    less comparison;
    return comparison (*this, t);
}
    
