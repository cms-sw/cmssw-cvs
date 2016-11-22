//################## EcalCorrelatedNoiseExample.cc ####################
// B. Fabbro       07/06/2007
//
//

#include "CalibCalorimetry/EcalCorrelatedNoiseAnalysisAlgos/interface/TCnaViewEB.h"

#include "Riostream.h"
#include "TROOT.h"
#include "TRint.h"

extern void InitGui();
VoidFuncPtr_t initfuncs[] = { InitGui, 0 };
TROOT root("GUI","GUI test environnement", initfuncs);

#include <stdlib.h>
#include <string>
#include "TString.h"

using namespace std;

int main ( int argc, char **argv )
{
  cout << "*EcalCorrelatedNoiseExample> Starting ROOT session" << endl;
  TRint theApp("App", &argc, argv);
  //--------------------------------------------------------------------
  //                      Init
  //--------------------------------------------------------------------
  Int_t xCnew      = 0;
  Int_t xCdelete   = 0;

  TString    fTTBELL = "\007";

  //--------------------------------------------------------------------
  //                   view histos
  //--------------------------------------------------------------------

  TCnaViewEB* fView = 0;  
  if ( fView == 0 ){fView = new TCnaViewEB();       xCnew++;}
  fView->GetPathForResultsRootFiles();

  //.............. Declarations and default values

  TString    fKeyAnaType    = "CnP";      // Analysis name
  Int_t      fKeyRunNumber  = 17635;      // Run number
  Int_t      fKeyFirstEvt   =     0;      // First Event number (to be analyzed)
  Int_t      fKeyNbOfEvts   =   150;      // Number of required events (events to be analyzed)
  Int_t      fKeySuMoNumber =     6;      // Super-module number

  //================================== Plots

  //............................. MeanOfSampleSigmasOfCrystals
  fKeyAnaType    = "CnP";
  fKeyRunNumber  = 17635;  
  fKeyFirstEvt   =   150;
  fKeyNbOfEvts   =   150;
  fKeySuMoNumber =     6;
  fView->SetFile(fKeyAnaType,    fKeyRunNumber, fKeyFirstEvt, fKeyNbOfEvts,
		 fKeySuMoNumber);

  fView->SetHistoScaleY("LIN");
  
  fView->HistoSuperModuleMeanOfSampleSigmasOfCrystals("ONLYONE");

  //............................. MeanOfSampleSigmasDistribution for 3 gains
  fKeyAnaType    = "CnP";
  fKeyRunNumber  = 17635;
  fKeyNbOfEvts   =   150;
  fKeySuMoNumber =     6;

  fView->SetHistoScaleY("LOG");
  //.................................
  fKeyFirstEvt   =     0;
  fView->SetFile(fKeyAnaType,    fKeyRunNumber, fKeyFirstEvt, fKeyNbOfEvts,
		 fKeySuMoNumber);
  fView->HistoSuperModuleMeanOfSampleSigmasDistribution("SEVERAL");
  //.................................
  fKeyFirstEvt   =   150;
  fView->SetFile(fKeyAnaType,    fKeyRunNumber, fKeyFirstEvt, fKeyNbOfEvts,
		 fKeySuMoNumber);
  fView->HistoSuperModuleMeanOfSampleSigmasDistribution("SEVERAL");
  //.................................
  fKeyFirstEvt   =   300;
  fView->SetFile(fKeyAnaType,    fKeyRunNumber, fKeyFirstEvt, fKeyNbOfEvts,
		 fKeySuMoNumber);
  fView->HistoSuperModuleMeanOfSampleSigmasDistribution("SEVERAL");

  //.................................. MeanOfCorssDistribution for 5 crystals
  fKeyAnaType    = "CnP";
  fKeyRunNumber  = 17537;
  fKeyFirstEvt   =   300;  
  fKeyNbOfEvts   =   150;
  fKeySuMoNumber =     6;
  fView->SetFile(fKeyAnaType,    fKeyRunNumber, fKeyFirstEvt, fKeyNbOfEvts,
		 fKeySuMoNumber);

  fView->SetHistoScaleY("LIN"); 

  Int_t SMtower = 1;
  Int_t TowEcha = 0;

  SMtower = 38;

  TowEcha = 10;
  fView->HistoCrystalSigmasOfSamples(SMtower, TowEcha, "SEVERAL");
  TowEcha = 11;
  fView->HistoCrystalSigmasOfSamples(SMtower, TowEcha, "SEVERAL");
  TowEcha = 12;
  fView->HistoCrystalSigmasOfSamples(SMtower, TowEcha, "SEVERAL");
  TowEcha = 13;
  fView->HistoCrystalSigmasOfSamples(SMtower, TowEcha, "SEVERAL");
  TowEcha = 14;
  fView->HistoCrystalSigmasOfSamples(SMtower, TowEcha, "SEVERAL");

  //.................................... EtaPhiSuperModuleMeanOfSampleSigmas 
  fKeyAnaType    = "CnP";
  fKeyRunNumber  = 17500;
  fKeyFirstEvt   =   150;  
  fKeyNbOfEvts   =   150;
  fKeySuMoNumber =     6;
  fView->SetHistoScaleY("LIN"); 

  fView->SetFile(fKeyAnaType,    fKeyRunNumber, fKeyFirstEvt, fKeyNbOfEvts,
		 fKeySuMoNumber);     
  fView->EtaPhiSuperModuleMeanOfSampleSigmas();

  //..................................... Sigma stability
  fKeyAnaType    = "CnP";
  fKeyRunNumber  = 17635;  
  fKeyFirstEvt   =   150;
  fKeyNbOfEvts   =   150;
  fKeySuMoNumber =     6;

  fView->SetHistoScaleY("LIN");
  fView->PutYmax("EvolEvSig",(Double_t)2.5);
  fView->GetPathForListOfRunFiles();

  SMtower = 54;
  TowEcha = 12;

  TString run_par_file_name = "CnP_SM6_evol_burst1.ascii";  
  fView->HistimeCrystalMeanSigmas(run_par_file_name, SMtower, TowEcha, "SEVERAL");

  run_par_file_name = "CnP_SM6_evol_burst2.ascii"; 
  fView->HistimeCrystalMeanSigmas(run_par_file_name, SMtower, TowEcha, "SEVERAL");
  
  run_par_file_name = "CnP_SM6_evol_burst3.ascii";
  fView->HistimeCrystalMeanSigmas(run_par_file_name, SMtower, TowEcha, "SEVERAL");
  
  //.......................................................................

  delete fView;                          xCdelete++;

      cout << "!H4Cna(main)> End of the example. You can quit ROOT (.q)"  << endl;

  if ( xCnew != xCdelete )
    {
      cout << "!H4Cna(main)> WRONG MANAGEMENT OF ALLOCATIONS: xCnew = "
	   << xCnew << ", xCdelete = " << xCdelete << '\007' << endl;
    }
  else
    {
      //  cout << "*H4Cna(main)> BRAVO! GOOD MANAGEMENT OF ALLOCATIONS: xCnew = "
      //      << xCnew << ", xCdelete = " << xCdelete << endl;
    }

  Bool_t retVal = kTRUE;
  theApp.Run(retVal);
  cout << endl
       << "*EcalCorrelatedNoiseExample> Terminating ROOT session." << endl;
  theApp.Terminate(0);
  cout << "*EcalCorrelatedNoiseExample> Exiting main program." << endl;
  exit(0);
}
