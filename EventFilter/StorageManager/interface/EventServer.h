#ifndef STOR_EVENT_SERVER_H
#define STOR_EVENT_SERVER_H

/**
 * This class manages the distribution of events to consumers from within
 * the storage manager.
 *
 * Two ways of throttling events are supported:
 * specifying a maximimum allowed rate of accepted events and specifying
 * a fixed prescale.  If the fixed prescale value is greater than zero,
 * it takes precendence.  That is, the maximum rate is ignored if the
 * prescale is in effect.
 *
 * 16-Aug-2006 - KAB  - Initial Implementation
 * $Id: EventServer.h,v 1.13 2009/07/21 06:44:31 mommsen Exp $
 */

#include <sys/time.h>
#include <string>
#include <vector>
#include <map>
#include "IOPool/Streamer/interface/MsgTools.h"
#include "IOPool/Streamer/interface/EventMessage.h"
#include "EventFilter/StorageManager/interface/ConsumerPipe.h"
#include "EventFilter/StorageManager/interface/ForeverCounter.h"
#include "EventFilter/StorageManager/interface/RollingIntervalCounter.h"
//#include "EventFilter/StorageManager/interface/RateLimiter.h"
#include "FWCore/Utilities/interface/CPUTimer.h"
#include "boost/random.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"

namespace stor
{
  class EventServer
  {
  public:
    enum STATS_TIME_FRAME { SHORT_TERM_STATS = 0, LONG_TERM_STATS = 1 };
    enum STATS_SAMPLE_TYPE { INPUT_STATS = 10, OUTPUT_STATS = 11,
                             UNIQUE_ACCEPT_STATS = 12 };
    enum STATS_TIMING_TYPE { CPUTIME = 20, REALTIME = 21 };

    EventServer(double maxEventRate, double maxDataRate,
                std::string hltOutputSelection, bool runFairShareAlgo = false);
    ~EventServer();

    void addConsumer(boost::shared_ptr<ConsumerPipe> consumer);
    std::map< uint32, boost::shared_ptr<ConsumerPipe> > getConsumerTable();
    boost::shared_ptr<ConsumerPipe> getConsumer(uint32 consumerId);

    void processEvent(const EventMsgView &eventView);
    boost::shared_ptr< std::vector<char> > getEvent(uint32 consumerId);
    void clearQueue();

    double getMaxEventRate() const { return maxEventRate_; }
    double getMaxDataRate() const { return maxDataRate_; }
    std::string getHLTOutputSelection() const { return hltOutputSelection_; }

    long long getEventCount(STATS_TIME_FRAME timeFrame = SHORT_TERM_STATS,
                            STATS_SAMPLE_TYPE sampleType = INPUT_STATS,
                            uint32 outputModuleId = 0,
                            double currentTime = ForeverCounter::getCurrentTime());
    double getEventRate(STATS_TIME_FRAME timeFrame = SHORT_TERM_STATS,
                        STATS_SAMPLE_TYPE sampleType = INPUT_STATS,
                        uint32 outputModuleId = 0,
                        double currentTime = ForeverCounter::getCurrentTime());
    double getDataRate(STATS_TIME_FRAME timeFrame = SHORT_TERM_STATS,
                       STATS_SAMPLE_TYPE sampleType = INPUT_STATS,
                       uint32 outputModuleId = 0,
                       double currentTime = ForeverCounter::getCurrentTime());
    double getDuration(STATS_TIME_FRAME timeFrame = SHORT_TERM_STATS,
                       STATS_SAMPLE_TYPE sampleType = INPUT_STATS,
                       uint32 outputModuleId = 0,
                       double currentTime = ForeverCounter::getCurrentTime());

    double getInternalTime(STATS_TIME_FRAME timeFrame = SHORT_TERM_STATS,
                           STATS_TIMING_TYPE timingType = CPUTIME,
                           double currentTime = ForeverCounter::getCurrentTime());
    double getTotalTime(STATS_TIME_FRAME timeFrame = SHORT_TERM_STATS,
                        STATS_TIMING_TYPE timingType = CPUTIME,
                        double currentTime = ForeverCounter::getCurrentTime());
    double getTimeFraction(STATS_TIME_FRAME timeFrame = SHORT_TERM_STATS,
                           STATS_TIMING_TYPE timingType = CPUTIME,
                           double currentTime = ForeverCounter::getCurrentTime());

  private:
    // data members for handling a maximum rate of accepted events
    double maxEventRate_;
    double maxDataRate_;
    std::string hltOutputSelection_;
    uint32 hltOutputModuleId_;
    bool runFairShareAlgo_;

    // new fair-share scheme
    //boost::shared_ptr<RateLimiter> rateLimiter_;

    // data members for deciding when to check for disconnected consumers
    int disconnectedConsumerTestCounter_;

    // consumer lists
    std::map< uint32, boost::shared_ptr<ConsumerPipe> > consumerTable_;
    //std::vector<boost::shared_ptr<ConsumerPipe>> vipConsumerList;

    // statistics
    std::map<uint32, boost::shared_ptr<ForeverCounter> > ltInputCounters_;
    std::map<uint32, boost::shared_ptr<RollingIntervalCounter> > stInputCounters_;
    std::map<uint32, boost::shared_ptr<ForeverCounter> > ltAcceptCounters_;
    std::map<uint32, boost::shared_ptr<RollingIntervalCounter> > stAcceptCounters_;
    std::map<uint32, boost::shared_ptr<ForeverCounter> > ltOutputCounters_;
    std::map<uint32, boost::shared_ptr<RollingIntervalCounter> > stOutputCounters_;
    edm::CPUTimer outsideTimer_;
    edm::CPUTimer insideTimer_;
    boost::shared_ptr<ForeverCounter> longTermInsideCPUTimeCounter_;
    boost::shared_ptr<RollingIntervalCounter> shortTermInsideCPUTimeCounter_;
    boost::shared_ptr<ForeverCounter> longTermInsideRealTimeCounter_;
    boost::shared_ptr<RollingIntervalCounter> shortTermInsideRealTimeCounter_;
    boost::shared_ptr<ForeverCounter> longTermOutsideCPUTimeCounter_;
    boost::shared_ptr<RollingIntervalCounter> shortTermOutsideCPUTimeCounter_;
    boost::shared_ptr<ForeverCounter> longTermOutsideRealTimeCounter_;
    boost::shared_ptr<RollingIntervalCounter> shortTermOutsideRealTimeCounter_;

    boost::mt19937 baseGenerator_;
    boost::shared_ptr< boost::uniform_01<boost::mt19937> > generator_;
  };
}

#endif
