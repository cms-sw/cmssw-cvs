#ifndef STOR_DQMEVENT_SERVER_H
#define STOR_DQMEVENT_SERVER_H

/**
 * This class manages the distribution of DQMevents to consumers from within
 * the storage manager or SM Proxy Server
 *
 * Two ways of throttling events are supported:
 * specifying a maximimum allowed rate of accepted events and specifying
 * a fixed prescale.  If the fixed prescale value is greater than zero,
 * it takes precendence.  That is, the maximum rate is ignored if the
 * prescale is in effect.
 *
 * Initial Implementation based on Kurt's EventServer
 * we can think about a common class later...
 *
 * $Id: DQMEventServer.h,v 1.5 2010/05/17 15:59:09 mommsen Exp $
 *
 * Note: this class is no longer used in the StorageManager, but is still
 * required by the SMProxyServer (Remi Mommsen, May 5, 2009)
 */

#include <sys/time.h>
#include <string>
#include <vector>
#include "IOPool/Streamer/interface/MsgTools.h"
#include "IOPool/Streamer/interface/DQMEventMessage.h"
#include "EventFilter/StorageManager/interface/DQMConsumerPipe.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"

namespace stor
{
  class DQMEventServer
  {
  public:
    DQMEventServer(double maximumRate);
    ~DQMEventServer();

    void addConsumer(boost::shared_ptr<DQMConsumerPipe> consumer);
    boost::shared_ptr<DQMConsumerPipe> getConsumer(uint32_t consumerId);
    void processDQMEvent(const DQMEventMsgView &eventView);
    boost::shared_ptr< std::vector<char> > getDQMEvent(uint32_t consumerId);
    void clearQueue();
    std::map< uint32_t, boost::shared_ptr<DQMConsumerPipe> > getConsumerTable()
    { return(consumerTable);}

  private:

    // data members for handling a maximum rate of accepted events
    static const double MAX_ACCEPT_INTERVAL;
    double minTimeBetweenEvents_;  // seconds
    struct timeval lastAcceptedEventTime_;

    // data members for deciding when to check for disconnected consumers
    int disconnectedConsumerTestCounter_;

    // consumer lists
    std::map< uint32_t, boost::shared_ptr<DQMConsumerPipe> > consumerTable;
    //std::vector<boost::shared_ptr<DQMConsumerPipe>> vipConsumerList;
  };
}

#endif
/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
