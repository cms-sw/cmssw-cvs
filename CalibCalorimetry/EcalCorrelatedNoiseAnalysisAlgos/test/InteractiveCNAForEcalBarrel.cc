//-------- InteractiveCNAForEcalBarrel.cc ------------
//
//                   C.N.A.  dialog box (GUI)
// 
//         Update: 07/06/2007
//
//---------------------------------------------------------------
#include "Riostream.h"
#include "TROOT.h"
#include "TGClient.h"
#include "TRint.h"
#include <cstdlib>

// CMSSW include files

#include "CalibCalorimetry/EcalCorrelatedNoiseAnalysisAlgos/interface/TCnaDialogEB.h"

extern void InitGui();
VoidFuncPtr_t initfuncs[] = { InitGui, 0 };
TROOT root("GUI","GUI test environnement", initfuncs);

int main(int argc, char **argv)
{
  cout << "*InteractiveCNAForEcalBarrel> Starting ROOT session" << endl;
  TRint theApp("App", &argc, argv);
  cout << "*InteractiveCNAForEcalBarrel> Starting new CNA session. Opening dialog box" << endl;
  TCnaDialogEB* mainWin = new TCnaDialogEB(gClient->GetRoot(), 420, 800);
  cout << "*InteractiveCNAForEcalBarrel> CNA session: preparing to run application." << endl;
  Bool_t retVal = kTRUE;
  theApp.Run(retVal);
  //theApp.Run();
  cout << "*InteractiveCNAForEcalBarrel> End of CNA session. Closing dialog box." << endl;
  delete mainWin;
  cout << "*InteractiveCNAForEcalBarrel> Terminating ROOT session." << endl;
  theApp.Terminate(0);
  cout << "*InteractiveCNAForEcalBarrel> Exiting main program." << endl;
  exit(0);
}
