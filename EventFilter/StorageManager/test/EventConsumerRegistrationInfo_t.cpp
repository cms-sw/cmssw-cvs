// $Id: EventConsumerRegistrationInfo_t.cpp,v 1.6 2010/08/06 20:24:32 wmtan Exp $

#include <iostream>

#include "EventFilter/StorageManager/interface/EventConsumerRegistrationInfo.h"
#include "EventFilter/StorageManager/interface/QueueID.h"


typedef stor::EventConsumerRegistrationInfo ECRI;
using stor::QueueID;
using namespace std;

int main()
{

  ECRI::FilterList fl;
  fl.push_back( "DQM1" );
  fl.push_back( "DQM2" );

  std::string filters2 = "DQM1 || DQM2";

  QueueID id1(stor::enquing_policy::DiscardOld, 3);

  ECRI ecri( "Test Consumer",
	     filters2,
	     fl,
	     "hltOutputDQM",
             id1.index(),
             id1.policy(),
	     10,
	     "localhost" );
  ecri.setQueueID( id1 );
	     

  std::cout << ecri << std::endl;

  return 0;

}
