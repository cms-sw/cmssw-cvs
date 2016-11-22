// -*- C++ -*-
// $Id: FWCaloTowerLegoHistProxyBuilder.h,v 1.3 2009/11/14 16:45:32 chrjones Exp $
//

#ifndef Fireworks_Calo_FWCaloTowerLegoHistProxyBuilder_h
#define Fireworks_Calo_FWCaloTowerLegoHistProxyBuilder_h


#include "Fireworks/Calo/plugins/FW3DLegoEveHistProxyBuilder.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

class TH2F;

class FWCaloTowerLegoHistBuilderBase : public FW3DLegoEveHistProxyBuilder
{
public:
   FWCaloTowerLegoHistBuilderBase() : FW3DLegoEveHistProxyBuilder(), m_towers(0), m_hist(0) {
   }
   virtual ~FWCaloTowerLegoHistBuilderBase(){
   }

   // ---------- const member functions ---------------------
   virtual const char* histName() const = 0;

protected:
   virtual void fillHist() = 0;
   // ---------- member data --------------------------------
   const CaloTowerCollection* m_towers;
   TH2F* m_hist;

private:
   virtual void applyChangesToAllModels();
   virtual void build(const FWEventItem* iItem,
                      TH2F** product);

   FWCaloTowerLegoHistBuilderBase(const FWCaloTowerLegoHistBuilderBase&); // stop default
   const FWCaloTowerLegoHistBuilderBase& operator=(const FWCaloTowerLegoHistBuilderBase&); // stop default


};

//
// ECal
//

class FWECalCaloTowerLegoHistBuilder : public FWCaloTowerLegoHistBuilderBase
{

public:
   FWECalCaloTowerLegoHistBuilder() : FWCaloTowerLegoHistBuilderBase(){
   }
   virtual ~FWECalCaloTowerLegoHistBuilder(){
   }

   // ---------- const member functions ---------------------
   virtual const char* histName() const {
      return "ECal";
   }

   REGISTER_PROXYBUILDER_METHODS();

protected:
   virtual void fillHist();

private:
   FWECalCaloTowerLegoHistBuilder(const FWECalCaloTowerLegoHistBuilder&);  // stop default
   // const FWECalCaloTowerLegoHistBuilder& operator=(const FWECalCaloTowerLegoHistBuilder&); // stop default
};


//
// HCal
//

class FWHCalCaloTowerLegoHistBuilder : public FWCaloTowerLegoHistBuilderBase
{

public:
   FWHCalCaloTowerLegoHistBuilder() : FWCaloTowerLegoHistBuilderBase() {
   }
   virtual ~FWHCalCaloTowerLegoHistBuilder(){
   }

   // ---------- const member functions ---------------------
   virtual const char* histName() const {
      return "HCal";
   }

   REGISTER_PROXYBUILDER_METHODS();

protected:
   virtual void fillHist();

private:
   FWHCalCaloTowerLegoHistBuilder(const FWHCalCaloTowerLegoHistBuilder&); // stop default
   const FWHCalCaloTowerLegoHistBuilder& operator=(const FWHCalCaloTowerLegoHistBuilder&); // stop default

};


#endif
