#ifndef Fireworks_Core_FWEveLegoViewManager_h
#define Fireworks_Core_FWEveLegoViewManager_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEveLegoViewManager
//
/**\class FWEveLegoViewManager FWEveLegoViewManager.h Fireworks/Core/interface/FWEveLegoViewManager.h

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:
//         Created:  Sun Jan  6 22:01:21 EST 2008
// $Id: FWEveLegoViewManager.h,v 1.15 2009/07/20 12:50:51 amraktad Exp $
//

// system include files
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

// user include files
#include "Fireworks/Core/interface/FWViewManagerBase.h"
#include "Fireworks/Core/interface/FWEvePtr.h"

// forward declarations
class FW3DLegoDataProxyBuilder;
class FWEventItem;
class FWGUIManager;
class TGFrame;
class FWEveLegoView;
class FWViewBase;
class TEveCaloDataHist;
class TEveElementList;
class TEveSelection;
class FWSelectionManager;
class TEveCaloLego;
class TEveWindowSlot;
class TEveStraightLineSet;

class FWEveLegoViewManager : public FWViewManagerBase
{

public:
   FWEveLegoViewManager(FWGUIManager*);
   virtual ~FWEveLegoViewManager();

   // ---------- const member functions ---------------------
   FWTypeToRepresentations supportedTypesAndRepresentations() const;

   // ---------- static member functions --------------------

   // ---------- member functions ---------------------------

   virtual void newItem(const FWEventItem*);

   FWViewBase* buildView(TEveWindowSlot* iParent);

   //connect to ROOT signals
   void selectionAdded(TEveElement*);
   void selectionRemoved(TEveElement*);
   void selectionCleared();

protected:
   virtual void modelChangesComing();
   virtual void modelChangesDone();
   virtual void colorsChanged();

private:
   FWEveLegoViewManager(const FWEveLegoViewManager&);    // stop default

   const FWEveLegoViewManager& operator=(const FWEveLegoViewManager&);    // stop default

   void makeProxyBuilderFor(const FWEventItem* iItem);
   void beingDestroyed(const FWViewBase*);
   void initData();

   void setGridColors();
   // ---------- member data --------------------------------
   typedef  std::map<std::string,std::vector<std::string> > TypeToBuilders;
   TypeToBuilders m_typeToBuilders;

   std::vector<boost::shared_ptr<FW3DLegoDataProxyBuilder> > m_builders;
   std::vector<boost::shared_ptr<FWEveLegoView> > m_views;

   FWEvePtr<TEveElementList> m_elements;
   TEveCaloDataHist*         m_data;
   TEveCaloLego*             m_lego;
   TEveStraightLineSet*      m_boundaries;
   TEveSelection*            m_eveSelection;
   FWSelectionManager*       m_selectionManager;

   bool m_modelsHaveBeenMadeAtLeastOnce;
};

#endif

