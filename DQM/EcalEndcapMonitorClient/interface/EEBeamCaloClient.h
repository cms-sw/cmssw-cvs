#ifndef EEBeamCaloClient_H
#define EEBeamCaloClient_H

/*
 * \file EEBeamCaloClient.h
 *
 * $Date: 2010/01/25 21:12:25 $
 * $Revision: 1.26 $
 * \author G. Della Ricca
 * \author A. Ghezzi
 *
*/

#include <vector>
#include <string>

#include "TROOT.h"
#include "TProfile2D.h"
#include "TProfile.h"
#include "TH1F.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQM/EcalEndcapMonitorClient/interface/EEClient.h"

class MonitorElement;
class DQMStore;
#ifdef WITH_ECAL_COND_DB
class EcalCondDBInterface;
class MonRunIOV;
#endif

class EEBeamCaloClient : public EEClient {

public:

/// Constructor
EEBeamCaloClient(const edm::ParameterSet& ps);

/// Destructor
virtual ~EEBeamCaloClient();

/// Analyze
void analyze(void);

/// BeginJob
void beginJob(void);

/// EndJob
void endJob(void);

/// BeginRun
void beginRun(void);

/// EndRun
void endRun(void);

/// Setup
void setup(void);

/// Cleanup
void cleanup(void);

#ifdef WITH_ECAL_COND_DB
/// WriteDB
bool writeDb(EcalCondDBInterface* econn, RunIOV* runiov, MonRunIOV* moniov, bool& status);
#endif

/// Get Functions
inline int getEvtPerJob() { return ievt_; }
inline int getEvtPerRun() { return jevt_; }

private:

const static int cryInArray_ = 9;

int ievt_;
int jevt_;

bool cloneME_;

bool verbose_;
bool debug_;

std::string prefixME_;

bool enableCleanup_;

std::vector<int> superModules_;

DQMStore* dqmStore_;

//specific task me
std::vector<int> checkedSteps_;
float prescaling_;
//specific task me

TH1F* hBGains_[cryInArray_];
TProfile* hBpulse_[cryInArray_];

TH1F* hBEne1_;

//TH1F* hBGainsMoving_[cryInArray_];
//TH1F* hBEne1Moving_;

TH1F* hBAllNeededCry_;

TH1F* hBNumReadCry_;

TH1F* hBE3x3_;

TH1F* hBE3x3Moving_;

TH2F* hBCryOnBeam_;

TH2F* hBMaxEneCry_;

TH1F* hBReadCryErrors_;

TProfile* hBE1vsCry_;

TProfile* hBE3x3vsCry_;

TH1F* hBEntriesvsCry_;

TH1F* hBcryDone_;

TH2F* hBBeamCentered_;

TH1F* hbTBmoving_;

TH1F* hbE1MaxCry_;

TH1F* hbDesync_;

TProfile* pBCriInBeamEvents_;

MonitorElement* meEEBCaloRedGreen_;
MonitorElement* meEEBCaloRedGreenReadCry_;
MonitorElement* meEEBCaloRedGreenSteps_;

// quality check parameters

int minEvtNum_;

float aveEne1_;
float E1Th_;
float aveEne3x3_;
float E3x3Th_;
float RMSEne3x3_;
float ReadCryErrThr_;

};

#endif
