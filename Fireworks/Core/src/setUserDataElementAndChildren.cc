// -*- C++ -*-
//
// Package:     Core
// Class  :     setUserDataElementAndChildren
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Wed Nov 26 14:19:15 EST 2008
// $Id: setUserDataElementAndChildren.cc,v 1.2 2009/01/23 21:35:44 amraktad Exp $
//

// system include files
#include "TEveElement.h"

// user include files
#include "Fireworks/Core/interface/setUserDataElementAndChildren.h"


//
// constants, enums and typedefs
//
namespace fireworks {

   void
   setUserDataElementAndChildren(TEveElement* iElement,
                                 const void* iInfo)
   {
      iElement->SetUserData(const_cast<void*>(iInfo));
      for(TEveElement::List_i itElement = iElement->BeginChildren(),
                              itEnd = iElement->EndChildren();
          itElement != itEnd;
          ++itElement) {
         setUserDataElementAndChildren(*itElement, iInfo);
      }
   }
}
