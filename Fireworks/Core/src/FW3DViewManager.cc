// -*- C++ -*-
//
// Package:     Core
// Class  :     FW3DViewManager
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:
//         Created:  Sun Jan  6 22:01:27 EST 2008
// $Id: FW3DViewManager.cc,v 1.15 2010/01/21 21:01:36 amraktad Exp $
//

// system include files
#include <iostream>
#include <boost/bind.hpp>
#include <algorithm>
#include "TView.h"
#include "TList.h"
#include "TEveManager.h"
#include "TClass.h"
#include "TColor.h"
#include "TRootEmbeddedCanvas.h"
#include "TEveCaloData.h"
#include "TEveElement.h"
#include "TEveWindow.h"
#include "TEveCalo.h"
#include "TEveScalableStraightLineSet.h"

// user include files
#include "Fireworks/Core/interface/FW3DViewManager.h"
#include "Fireworks/Core/interface/FW3DView.h"
#include "Fireworks/Core/interface/FW3DDataProxyBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWGUIManager.h"

#include "Fireworks/Core/interface/FWSelectionManager.h"
#include "Fireworks/Core/interface/FWColorManager.h"

#include "Fireworks/Core/interface/FW3DDataProxyBuilderFactory.h"

#include "Fireworks/Core/interface/FWEDProductRepresentationChecker.h"
#include "Fireworks/Core/interface/FWSimpleRepresentationChecker.h"
#include "Fireworks/Core/interface/FWTypeToRepresentations.h"
#include "Fireworks/Core/interface/TEveElementIter.h"


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FW3DViewManager::FW3DViewManager(FWGUIManager* iGUIMgr) :
   FWViewManagerBase(),
   m_elements( new TEveElementList("3D")),
   m_caloData(0),
   m_calo3d(0),
   m_selectionManager(0)
{
   FWGUIManager::ViewBuildFunctor f;
   f=boost::bind(&FW3DViewManager::buildView,
                 this, _1);
   iGUIMgr->registerViewBuilder(FW3DView::staticTypeName(), f);

   //create a list of the available ViewManager's
   std::set<std::string> builders;

   std::vector<edmplugin::PluginInfo> available = FW3DDataProxyBuilderFactory::get()->available();
   std::transform(available.begin(),
                  available.end(),
                  std::inserter(builders,builders.begin()),
                  boost::bind(&edmplugin::PluginInfo::name_,_1));

   if(edmplugin::PluginManager::get()->categoryToInfos().end()!=edmplugin::PluginManager::get()->categoryToInfos().find(FW3DDataProxyBuilderFactory::get()->category())) {
      available = edmplugin::PluginManager::get()->categoryToInfos().find(FW3DDataProxyBuilderFactory::get()->category())->second;
      std::transform(available.begin(),
                     available.end(),
                     std::inserter(builders,builders.begin()),
                     boost::bind(&edmplugin::PluginInfo::name_,_1));
   }

   for(std::set<std::string>::iterator it = builders.begin(), itEnd=builders.end();
       it!=itEnd;
       ++it) {
      std::string::size_type first = it->find_first_of('@')+1;
      std::string purpose = it->substr(first,it->find_last_of('@')-first);
      m_typeToBuilders[purpose].push_back(*it);
   }

}

FW3DViewManager::~FW3DViewManager()
{
}

//
// member functions
//
FWViewBase*
FW3DViewManager::buildView(TEveWindowSlot* iParent)
{
   TEveManager::TRedrawDisabler disableRedraw(gEve);
   boost::shared_ptr<FW3DView> view( new FW3DView(iParent, m_elements.get()) );
   view->setBackgroundColor(colorManager().background());

   m_views.push_back(view);
   //? pView->resetCamera();
   if(1 == m_views.size()) {
      for(std::vector<boost::shared_ptr<FW3DDataProxyBuilder> >::iterator it
             =m_builders.begin(), itEnd = m_builders.end();
          it != itEnd;
          ++it) {
         (*it)->setHaveAWindow(true);
      }
   }
   view->setGeometry( detIdToGeo() );
   view->beingDestroyed_.connect(boost::bind(&FW3DViewManager::beingDestroyed,this,_1));
   return view.get();
}
void
FW3DViewManager::beingDestroyed(const FWViewBase* iView)
{

   if(1 == m_views.size()) {
      for(std::vector<boost::shared_ptr<FW3DDataProxyBuilder> >::iterator it
             =m_builders.begin(), itEnd = m_builders.end();
          it != itEnd;
          ++it) {
         (*it)->setHaveAWindow(false);
      }
   }
   for(std::vector<boost::shared_ptr<FW3DView> >::iterator it=
          m_views.begin(), itEnd = m_views.end();
       it != itEnd;
       ++it) {
      if(it->get() == iView) {
         m_views.erase(it);
         return;
      }
   }
}

void
FW3DViewManager::makeProxyBuilderFor(const FWEventItem* iItem)
{
   if(0==m_selectionManager) {
      //std::cout <<"got selection manager"<<std::endl;
      m_selectionManager = iItem->selectionManager();
   }
   TypeToBuilders::iterator itFind = m_typeToBuilders.find(iItem->purpose());
   if(itFind != m_typeToBuilders.end()) {
      for ( std::vector<std::string>::const_iterator builderName = itFind->second.begin();
            builderName != itFind->second.end(); ++builderName )
      {
         FW3DDataProxyBuilder* builder = FW3DDataProxyBuilderFactory::get()->create(*builderName);
         if(0!=builder) {
            boost::shared_ptr<FW3DDataProxyBuilder> pB( builder );
            builder->setItem(iItem);
            builder->setHaveAWindow(!m_views.empty());
            builder->addToScene(*m_elements,&m_caloData);
            //NOTE: this is a very bad design, we should pass in TEveCalo3D*& to addToScene or handle the construction
            // of m_calo3d ourselves
            TEveCalo3D* calo3d =  dynamic_cast<TEveCalo3D*>(m_elements->LastChild());
            if(0!=calo3d) {
               m_calo3d = calo3d;
               m_calo3d->SetMainColor(colorManager().geomColor(kFWCalo3DFrameColorIndex));
            }
            m_builders.push_back(pB);
         }
      }
   }
}

void
FW3DViewManager::newItem(const FWEventItem* iItem)
{
   makeProxyBuilderFor(iItem);
}

void
FW3DViewManager::modelChangesComing()
{
   gEve->DisableRedraw();
}
void
FW3DViewManager::modelChangesDone()
{
   gEve->EnableRedraw();
}
void
FW3DViewManager::colorsChanged()
{
   for(std::vector<boost::shared_ptr<FW3DView> >::iterator it=
       m_views.begin(), itEnd = m_views.end();
       it != itEnd;
       ++it) {
      (*it)->setBackgroundColor(colorManager().background());
   }

    if (m_calo3d)  m_calo3d->SetMainColor(colorManager().geomColor(kFWCalo3DFrameColorIndex));
}

FWTypeToRepresentations
FW3DViewManager::supportedTypesAndRepresentations() const
{
   FWTypeToRepresentations returnValue;
   const std::string kSimple("simple#");

   for(TypeToBuilders::const_iterator it = m_typeToBuilders.begin(), itEnd = m_typeToBuilders.end();
       it != itEnd;
       ++it) {
      for ( std::vector<std::string>::const_iterator builderName = it->second.begin();
            builderName != it->second.end(); ++builderName )
      {
         if(builderName->substr(0,kSimple.size()) == kSimple) {
            returnValue.add(boost::shared_ptr<FWRepresentationCheckerBase>(
                               new FWSimpleRepresentationChecker( builderName->substr(kSimple.size(), builderName->find_first_of('@')-kSimple.size()),
                                                                  it->first)));
         } else {
            returnValue.add(boost::shared_ptr<FWRepresentationCheckerBase>(
                               new FWEDProductRepresentationChecker( builderName->substr(0,builderName->find_first_of('@')),
                                                                     it->first)));
         }
      }

   }
   return returnValue;
}

void
FW3DViewManager::eventEnd()
{
   if (m_calo3d)
   {
      double scale = m_calo3d->GetValToHeight();
      TEveElementIter child(m_elements.get());
      while ( TEveElement* el = child.current() )
      {
         if ( TEveScalableStraightLineSet* line = dynamic_cast<TEveScalableStraightLineSet*>(el) )
         {
            line->SetScale( scale );
            line->ElementChanged();
         }
         child.next();
      }
   }
}
