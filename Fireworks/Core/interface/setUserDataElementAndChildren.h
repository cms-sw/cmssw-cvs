#ifndef Fireworks_Core_setUserDataElementAndChildren_h
#define Fireworks_Core_setUserDataElementAndChildren_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     setUserDataElementAndChildren
//
/**\class setUserDataElementAndChildren setUserDataElementAndChildren.h Fireworks/Core/interface/setUserDataElementAndChildren.h

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:  Chris Jones
//         Created:  Wed Nov 26 14:10:33 EST 2008
// $Id: setUserDataElementAndChildren.h,v 1.2 2009/01/23 21:35:42 amraktad Exp $
//

// system include files

// user include files

// forward declarations
class TEveElement;

namespace fireworks {
   void
   setUserDataElementAndChildren(TEveElement* iElement,
                                 const void* iInfo);
}

#endif
