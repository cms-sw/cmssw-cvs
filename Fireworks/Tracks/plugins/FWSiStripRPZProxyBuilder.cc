// -*- C++ -*-
//
// Package:     Core
// Class  :     FWSiStripRPZProxyBuilder
//
/**\class FWSiStripRPZProxyBuilder FWSiStripRPZProxyBuilder.h Fireworks/Core/interface/FWSiStripRPZProxyBuilder.h

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:
//         Created:  Thu Dec  6 18:01:21 PST 2007
// $Id: FWSiStripRPZProxyBuilder.cc,v 1.3 2009/12/03 04:34:04 latb Exp $
//

// system include files
#include "TEveManager.h"
#include "TEveCompound.h"
#include "TEveGeoNode.h"
#include "TEveStraightLineSet.h"

// user include files
#include "Fireworks/Tracks/interface/TrackUtils.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWRPZDataProxyBuilder.h"

#include "Fireworks/Core/interface/BuilderUtils.h"
#include "Fireworks/Core/src/CmsShowMain.h"
#include "Fireworks/Core/src/changeElementAndChildren.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"

class FWSiStripRPZProxyBuilder : public FWRPZDataProxyBuilder
{

public:
   FWSiStripRPZProxyBuilder() {
   }
   virtual ~FWSiStripRPZProxyBuilder() {
   }
   REGISTER_PROXYBUILDER_METHODS();
private:
   virtual void build(const FWEventItem* iItem, TEveElementList** product);

   FWSiStripRPZProxyBuilder(const FWSiStripRPZProxyBuilder&);    // stop default

   const FWSiStripRPZProxyBuilder& operator=(const FWSiStripRPZProxyBuilder&);    // stop default
   void modelChanges(const FWModelIds& iIds, TEveElement* iElements);
   void applyChangesToAllModels(TEveElement* iElements);
};

void FWSiStripRPZProxyBuilder::build(const FWEventItem* iItem, TEveElementList** product)
{
   TEveElementList* tList = *product;

   if(0 == tList) {
      tList =  new TEveElementList(iItem->name().c_str(),"SiStripCluster",true);
      *product = tList;
      tList->SetMainColor(iItem->defaultDisplayProperties().color());
      gEve->AddElement(tList);
   } else {
      tList->DestroyElements();
   }

   const edmNew::DetSetVector<SiStripCluster>* clusters=0;
   iItem->get(clusters);

   if(0 == clusters ) return;
   std::set<DetId> modules;
   int index=0;
   for(edmNew::DetSetVector<SiStripCluster>::const_iterator set = clusters->begin();
       set != clusters->end(); ++set,++index) {
      const unsigned int bufSize = 1024;
      char title[bufSize];
      char name[bufSize];
      unsigned int id = set->detId();
      snprintf(name,  bufSize,"module%d",index);
      snprintf(title, bufSize,"Module %d",id);
      TEveCompound* list = new TEveCompound(name, title);
      list->OpenCompound();
      //guarantees that CloseCompound will be called no matter what happens
      boost::shared_ptr<TEveCompound> sentry(list,boost::mem_fn(&TEveCompound::CloseCompound));
      list->SetRnrSelf(     iItem->defaultDisplayProperties().isVisible() );
      list->SetRnrChildren( iItem->defaultDisplayProperties().isVisible() );

      if (iItem->getGeom()) {
         // const TGeoHMatrix* matrix = iItem->getGeom()->getMatrix( id );
         TEveGeoShape* shape = iItem->getGeom()->getShape( id );
         if(0!=shape) {
            shape->SetMainTransparency(75);
            shape->SetMainColor( iItem->defaultDisplayProperties().color() );
            shape->SetPickable(true);
            list->AddElement(shape);
         }
      }

      gEve->AddElement(list,tList);
/////////////////////////////////////////////////////	   
//LatB
	   static int C2D=0;
	   static int PRINT=0;
	   if (C2D) {
		   if (PRINT) std::cout<<"SiStripCluster  "<<index<<", "<<title<<std::endl;
		   TEveStraightLineSet *scposition = new TEveStraightLineSet(title);
		   for(edmNew::DetSet<SiStripCluster>::const_iterator ic = set->begin (); ic != set->end (); ++ic) { 
			   short fs = (*ic).firstStrip ();
			   double bc = (*ic).barycenter();
			   TVector3 point, pointA, pointB; fireworks::localSiStrip(point, pointA, pointB, bc, id, iItem);
			   if (PRINT) std::cout<<"SiStripCluster first strip "<<fs<<", bary center "<<bc<<", phi "<<point.Phi()<<std::endl;
			   scposition->AddLine(pointA.X(), pointA.Y(), pointA.Z(), pointB.X(), pointB.Y(), pointB.Z());
			   scposition->SetLineColor(kRed);
		   }
		   gEve->AddElement(scposition,tList);
	   }
/////////////////////////////////////////////////////	   
   }
}

void
FWSiStripRPZProxyBuilder::modelChanges(const FWModelIds& iIds, TEveElement* iElements)
{
   applyChangesToAllModels(iElements);
}

void
FWSiStripRPZProxyBuilder::applyChangesToAllModels(TEveElement* iElements)
{
   if(0!=iElements && item() && item()->size()) {
      //make the bad assumption that everything is being changed indentically
      const FWEventItem::ModelInfo info(item()->defaultDisplayProperties(),false);
      changeElementAndChildren(iElements, info);
      iElements->SetRnrSelf(info.displayProperties().isVisible());
      iElements->SetRnrChildren(info.displayProperties().isVisible());
      iElements->ElementChanged();
   }
}
REGISTER_FWRPZDATAPROXYBUILDERBASE(FWSiStripRPZProxyBuilder,edmNew::DetSetVector<SiStripCluster>,"SiStrip");
