#ifndef DQM_HCALMONITORTASKS_HCALDETDIAGLASERMONITOR_H
#define DQM_HCALMONITORTASKS_HCALDETDIAGLASERMONITOR_H

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQM/HcalMonitorTasks/interface/HcalBaseMonitor.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"

#include <math.h>
using namespace edm;
using namespace std;
// this is to retrieve HCAL digi's
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
// to retrive trigger information (local runs only)
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"

#include "CalibCalorimetry/HcalAlgos/interface/HcalLogicalMapGenerator.h"
#include "CondFormats/HcalObjects/interface/HcalLogicalMap.h"

/** \class HcalDetDiagLEDMonitor
  *  
  * $Date: 2009/07/06 19:09:04 $
  * $Revision: 1.5 $
  * \author D. Vishnevskiy
  */

class HcalDetDiagLaserData{
public: 
   HcalDetDiagLaserData(){ 
	     IsRefetence=false;
	     status=0;
	     reset();
	  }
   void   reset(){
             Xe=XXe=Xt=XXt=n=0;
	     overflow=0;
	     undeflow=0;
          }
   void   add_statistics(double *data,int nTS){
 	     double e=GetEnergy(data,nTS);
	     double t=GetTime(data,nTS);
             if(e<20) undeflow++; else if(e>10000) overflow++; else{
	        n++; Xe+=e; XXe+=e*e; Xt+=t; XXt+=t*t;
	     } 	   
	  }
   void   set_reference(float val,float rms,float time,float time_rms){
             ref_amp=val; ref_rms=rms;
	     ref_time=time; ref_time_rms=time_rms;
	     IsRefetence=true;
          }	  
   void   change_status(int val){
             status|=val;
          }	  
   int    get_status(){
             return status;
          }	  
   bool   get_reference(double *val,double *rms,double *time,double *time_rms){
             *val=ref_amp; *rms=ref_rms;
             *time=ref_time; *time_rms=ref_time_rms;
	     return IsRefetence;
          }	  
   bool   get_average_amp(double *ave,double *rms){
	     if(n>0){ *ave=Xe/n; *rms=sqrt(XXe/n-(Xe*Xe)/(n*n));} else return false;
             return true; 
          }
   bool   get_average_time(double *ave,double *rms){
             if(n>0){ *ave=Xt/n; *rms=sqrt(XXt/n-(Xt*Xt)/(n*n));} else return false;
             return true; 
          }
   int    get_statistics(){
	     return (int)n;
	  } 
   int    get_overflow(){
             return overflow;
          }   
   int    get_undeflow(){
             return undeflow;
          }   
private:   
   double GetEnergy(double *data,int n){
             int MaxI=0; double Energy,MaxE=0;
             for(int j=0;j<n;++j) if(MaxE<data[j]){ MaxE=data[j]; MaxI=j; }
             Energy=data[MaxI];
             if(MaxI>0) Energy+=data[MaxI-1];
             if(MaxI>1) Energy+=data[MaxI-2];
             if(MaxI<(n-1)) Energy+=data[MaxI+1];
             if(MaxI<(n-2)) Energy+=data[MaxI+2];
             return Energy;
          }
   double GetTime(double *data,int n=10){
             int MaxI=-100; double Time=-9999,SumT=0,MaxT=-10;
             for(int j=0;j<n;++j) if(MaxT<data[j]){ MaxT=data[j]; MaxI=j; }
	     if (MaxI>=0) // dummy protection so that compiler doesn't think MaxI=-100
	       {
		 Time=MaxI*data[MaxI];
		 SumT=data[MaxI];
		 if(MaxI>0){ Time+=(MaxI-1)*data[MaxI-1]; SumT+=data[MaxI-1]; }
		 if(MaxI<(n-1)){ Time+=(MaxI+1)*data[MaxI+1]; SumT+=data[MaxI+1]; }
		 Time=Time/SumT;
	       }
             return Time;
         }      
   int   overflow;
   int   undeflow;
   double Xe,XXe,Xt,XXt,n;
   bool  IsRefetence;
   float ref_amp;
   float ref_rms;
   float ref_time;
   float ref_time_rms;
   int   status;
};

class HcalDetDiagLaserMonitor:public HcalBaseMonitor {
public:
  HcalDetDiagLaserMonitor(); 
  ~HcalDetDiagLaserMonitor(); 

  void setup(const edm::ParameterSet& ps, DQMStore* dbe);
  void processEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup, const HcalDbService& cond);
  void done();
  void reset();
  void clearME(); 
  void fillHistos();
  int  GetStatistics(){ return ievt_; }
private:
  const HcalElectronicsMap  *emap;
  // in principle functions below shold use DB interface (will be modefied when DB will be ready...)  
  void SaveReference();
  void LoadReference();

  int         ievt_;
  int         run_number;
  int         dataset_seq_number;
  bool        IsReference;
  bool        UseDB;
   
  std::string ReferenceData;
  std::string ReferenceRun;
  std::string OutputFilePath;

  MonitorElement *meEVT_;
  MonitorElement *RefRun_;
  MonitorElement *hbheEnergy;
  MonitorElement *hbheTime;
  MonitorElement *hbheEnergyRMS;
  MonitorElement *hbheTimeRMS;
  MonitorElement *hoEnergy;
  MonitorElement *hoTime;
  MonitorElement *hoEnergyRMS;
  MonitorElement *hoTimeRMS;
  MonitorElement *hfEnergy;
  MonitorElement *hfTime;
  MonitorElement *hfEnergyRMS;
  MonitorElement *hfTimeRMS;
  
  
  MonitorElement *Time2Dhbhehf;
  MonitorElement *Time2Dho;
  MonitorElement *Energy2Dhbhehf;
  MonitorElement *Energy2Dho;
  MonitorElement *refTime2Dhbhehf;
  MonitorElement *refTime2Dho;
  MonitorElement *refEnergy2Dhbhehf;
  MonitorElement *refEnergy2Dho;

  MonitorElement *Raddam[56];
    
  HcalDetDiagLaserData hb_data[85][72][4];
  HcalDetDiagLaserData he_data[85][72][4];
  HcalDetDiagLaserData ho_data[85][72][4];
  HcalDetDiagLaserData hf_data[85][72][4];
};

#endif
