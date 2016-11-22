#ifndef Fireworks_Core_FWGlimpseDataProxyBuilder_h
#define Fireworks_Core_FWGlimpseDataProxyBuilder_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWGlimpseDataProxyBuilder
//
/**\class FWGlimpseDataProxyBuilder FWGlimpseDataProxyBuilder.h Fireworks/Core/interface/FWGlimpseDataProxyBuilder.h

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:
//         Created:  Sat Jan  5 15:02:03 EST 2008
// $Id: FWGlimpseDataProxyBuilder.h,v 1.9 2010/03/04 21:38:41 chrjones Exp $
//

// system include files
#include <vector>

// user include files
#include "Fireworks/Core/interface/FWGlimpseDataProxyBuilderFactory.h"
#include "Fireworks/Core/interface/FWModelChangeSignal.h"
#include "Fireworks/Core/interface/FWEvePtr.h"
#include "Fireworks/Core/interface/FWModelIdFromEveSelector.h"

// forward declarations
class FWEventItem;
class TEveElementList;
class TEveElement;
class FWModelId;
class TEveCalo3D;
class FWEveValueScaler;

class FWGlimpseDataProxyBuilder
{

public:
   FWGlimpseDataProxyBuilder();
   virtual ~FWGlimpseDataProxyBuilder();

   // ---------- const member functions ---------------------

   // ---------- static member functions --------------------
   ///Used by the plugin system to determine how the proxy uses the data from FWEventItem
   static std::string typeOfBuilder();

   // ---------- member functions ---------------------------
   void setItem(const FWEventItem* iItem);
   void setHaveAWindow(bool iFlag);
   void build();

   void modelChanges(const FWModelIds&);
   void itemChanged(const FWEventItem*);

   TEveElement* usedInScene();

   void setScaler(FWEveValueScaler* iScaler) {
      m_scaler = iScaler;
   }

   FWEveValueScaler* scaler() const {
      return m_scaler;
   }
protected:
   const FWEventItem* item() const {
      return m_item;
   }

   std::vector<FWModelIdFromEveSelector>& ids() {
      return m_ids;
   }
private:
   virtual void build(const FWEventItem* iItem,
                      TEveElementList** product) = 0 ;


   //Override this if you need to special handle selection or other changes
   virtual void modelChanges(const FWModelIds&, TEveElement*);
   virtual bool specialModelChangeHandling(const FWModelId&, TEveElement*);
   virtual void applyChangesToAllModels(TEveElement* iElements);
   virtual void itemChangedImp(const FWEventItem*);

   virtual void itemBeingDestroyed(const FWEventItem*);

   void applyChangesToAllModels();

   FWGlimpseDataProxyBuilder(const FWGlimpseDataProxyBuilder&); // stop default

   const FWGlimpseDataProxyBuilder& operator=(const FWGlimpseDataProxyBuilder&);    // stop default

   // ---------- member data --------------------------------
   const FWEventItem* m_item;
   FWEvePtr<TEveElementList> m_elementHolder;   //Used as a smart pointer for the item created by the builder
   std::vector<FWModelIdFromEveSelector> m_ids;

   bool m_modelsChanged;
   bool m_haveViews;
   FWEveValueScaler* m_scaler;
   bool m_mustBuild;
};


#endif
