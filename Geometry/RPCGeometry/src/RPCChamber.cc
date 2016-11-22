/** \file
 *
 *  $Date: 2008/01/22 21:31:36 $
 *  $Revision: 1.4 $
 *  Author: Raffaello Trentadue - UniversitÓ Bari 
 *  Mail:     <raffaello.trentadue@ba.infn.it>
 */

/* This Class Header */
#include "Geometry/RPCGeometry/interface/RPCChamber.h"

/* Collaborating Class Header */
#include "Geometry/RPCGeometry/interface/RPCRoll.h"


/* C++ Headers */
#include <iostream>

/* ====================================================================== */

/* Constructor */ 
RPCChamber::RPCChamber(RPCDetId id, 
		       const ReferenceCountingPointer<BoundPlane> & plane) :
  GeomDet(plane), theId(id)
{}

/* Destructor */ 
RPCChamber::~RPCChamber() {
}

DetId 
RPCChamber::geographicalId() const
{
  return theId;
}

RPCDetId
RPCChamber::id() const
{
  return theId;
}

/* Operations */ 

bool 
RPCChamber::operator==(const RPCChamber& ch) const {
  return this->id()==ch.id();
}



void 
RPCChamber::add(RPCRoll* rl) {
  theRolls.push_back(rl);
}



std::vector<const GeomDet*> 
RPCChamber::components() const {
  return  std::vector<const GeomDet*>(theRolls.begin(), theRolls.end());
}



const GeomDet* 
RPCChamber::component(DetId id) const {
  return roll(RPCDetId(id.rawId()));
}


const std::vector<const RPCRoll*>& 
RPCChamber::rolls() const 
{
  return theRolls;
}

const int RPCChamber::nrolls() const
{
  return theRolls.size();
}

const RPCRoll* 
RPCChamber::roll(RPCDetId id) const
{
  if (id.chamberId()!=theId) return 0; // not in this Roll!
  return roll(id.roll());
}


const RPCRoll* 
RPCChamber::roll(int isl) const 
{
  for (std::vector<const RPCRoll*>::const_iterator i = theRolls.begin();
       i!= theRolls.end(); ++i) {
    if ((*i)->id().roll()==isl) 
      return (*i);
  }
  return 0;
}

