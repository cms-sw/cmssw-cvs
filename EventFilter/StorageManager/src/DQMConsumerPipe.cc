/**
 * This class is used to manage the subscriptions, DQMevents, and
 * lost connections associated with a DQMevent consumer within the
 * DQMevent server part of the storage manager or SM Proxy Server.
 *
 * Initial Implementation based on Kurt's ConsumerPipe
 * make a common class later when all this works
 *
 * $Id: DQMConsumerPipe.cc,v 1.12 2010/08/06 20:24:30 wmtan Exp $
 */

#include "EventFilter/StorageManager/interface/DQMConsumerPipe.h"
#include "FWCore/Utilities/interface/DebugMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// keep this for debugging
//#include "IOPool/Streamer/interface/DumpTools.h"

using namespace std;
using namespace stor;
using namespace edm;

/**
 * Initialize the static value for the root consumer id.
 */
uint32_t DQMConsumerPipe::rootId_ = 1;

/**
 * Initialize the static lock used to control access to the root ID.
 */
boost::mutex DQMConsumerPipe::rootIdLock_;

/**
 * DQMConsumerPipe constructor.
 */
DQMConsumerPipe::DQMConsumerPipe(std::string name, std::string priority,
                                 int activeTimeout, int idleTimeout,
                                 std::string folderName, std::string hostName,
                                 int queueSize):
  han_(curl_easy_init()),
  headers_(),
  consumerName_(name),consumerPriority_(priority),
  topFolderName_(folderName),
  hostName_(hostName),
  events_(0),
  pushEventFailures_(0),
  maxQueueSize_(queueSize)
{
  // initialize the time values we use for defining "states"
  timeToIdleState_ = activeTimeout;
  timeToDisconnectedState_ = activeTimeout + idleTimeout;
  lastEventRequestTime_ = time(NULL);
  initializationDone = false;
  pushMode_ = false;
  if(consumerPriority_.compare("PushMode") == 0) pushMode_ = true;

  // determine if we're connected to a proxy server
  consumerIsProxyServer_ = false;
  //if (consumerName_ == PROXY_SERVER_NAME)
  if (consumerName_.find("urn") != std::string::npos &&
      consumerName_.find("xdaq") != std::string::npos &&
      consumerName_.find("pushEventData") != std::string::npos)
  {
    consumerIsProxyServer_ = true;
  }

  // assign the consumer ID
  boost::mutex::scoped_lock scopedLockForRootId(rootIdLock_);
  consumerId_ = rootId_;
  rootId_++;

  if(han_==0)
  {
    edm::LogError("DQMConsumerPipe") << "Could not create curl handle";
    //std::cout << "Could not create curl handle" << std::endl;
    // throw exception here when we can make the SM go to a fail state from
    // another thread
  } else {
    headers_ = curl_slist_append(headers_, "Content-Type: application/octet-stream");
    headers_ = curl_slist_append(headers_, "Content-Transfer-Encoding: binary");
    // Avoid the Expect: 100 continue automatic header that gives a 2 sec delay
    // for pthttp but we don't need the Expect: 100 continue anyway
    headers_ = curl_slist_append(headers_, "Expect:");
    setopt(han_, CURLOPT_HTTPHEADER, headers_);
    setopt(han_, CURLOPT_URL, consumerName_.c_str());
    setopt(han_, CURLOPT_WRITEFUNCTION, func);
    // debug options
    //setopt(han_,CURLOPT_VERBOSE, 1);
    //setopt(han_,CURLOPT_TCP_NODELAY, 1);
  }
}

/**
 * DQMConsumerPipe destructor.
 */
DQMConsumerPipe::~DQMConsumerPipe()
{
  FDEBUG(5) << "Executing destructor for DQM consumer pipe with ID = " <<
    consumerId_ << std::endl;
  curl_slist_free_all(headers_);
  curl_easy_cleanup(han_);
}

/**
 * Returns the consumer ID associated with this pipe.
 */
uint32_t DQMConsumerPipe::getConsumerId() const
{
  return consumerId_;
}

/**
 * Initializes the event selection for this consumer based on the
 * list of available triggers stored in the specified InitMsgView
 * and the request ParameterSet that was specified in the constructor.
 */
void DQMConsumerPipe::initializeSelection()
{
  FDEBUG(5) << "Initializing DQM consumer pipe, ID = " <<
    consumerId_ << std::endl;

  // no need for initialization yet
  initializationDone = true;

}

/**
 * Tests if the consumer is idle (as opposed to active).  The idle
 * state indicates that the consumer is still connected, but it hasn't
 * requested an event in some time.
 */
bool DQMConsumerPipe::isIdle() const
{
  time_t timeDiff = time(NULL) - lastEventRequestTime_;
  return (timeDiff >= timeToIdleState_ &&
          timeDiff <  timeToDisconnectedState_);
}

/**
 * Tests if the consumer has disconnected.
 */
bool DQMConsumerPipe::isDisconnected() const
{
  time_t timeDiff = time(NULL) - lastEventRequestTime_;
  return (timeDiff >= timeToDisconnectedState_);
}

/**
 * Tests if the consumer is ready for an event.
 */
bool DQMConsumerPipe::isReadyForEvent() const
{
  // 13-Oct-2006, KAB - we're not ready if we haven't been initialized
  if (! initializationDone) return false;

  // for now, just test if we are in the active state
  time_t timeDiff = time(NULL) - lastEventRequestTime_;
  return (timeDiff < timeToIdleState_);
}

/**
 * Tests if the consumer wants the specified event.
 */
bool DQMConsumerPipe::wantsDQMEvent(DQMEventMsgView const& eventView) const
{
  // for now, only allow one top folder selection or "*"
  std::string meansEverything = "*";
  if(topFolderName_.compare(meansEverything) == 0) return true;
  else return (topFolderName_.compare(eventView.topFolderName()) == 0);
}

/**
 * Adds the specified event to this consumer pipe.
 */
void DQMConsumerPipe::putDQMEvent(boost::shared_ptr< std::vector<char> > bufPtr)
{
  // update the local pointer to the most recent event
  boost::mutex::scoped_lock scopedLockForEventQueue(eventQueueLock_);
  eventQueue_.push_back(bufPtr);
  while (eventQueue_.size() > maxQueueSize_) {
    eventQueue_.pop_front();
  }
  // actually push out DQM data if this is a push mode consumer (SMProxyServer)
  if(pushMode_) {
    bool success = pushEvent();
    // update the time of the most recent successful transaction
    if(!success) ++pushEventFailures_;
    else 
    {
      lastEventRequestTime_ = time(NULL);
      ++events_;
    }
  }
}

/**
 * Fetches the next event from this consumer pipe.
 * If there are no events in the pipe, an empty shared_ptr will be returned
 * (ptr.get() == NULL).
 */
boost::shared_ptr< std::vector<char> > DQMConsumerPipe::getDQMEvent()
{
  // clear out any stale event(s)
  if (isIdle() || isDisconnected())
  {
    this->clearQueue();
  }

  // fetch the most recent event
  boost::shared_ptr< std::vector<char> > bufPtr;
  {
    boost::mutex::scoped_lock scopedLockForEventQueue(eventQueueLock_);
    if (! eventQueue_.empty())
    {
      bufPtr = eventQueue_.front();
      eventQueue_.pop_front();
      ++events_;
    }
  }

  // update the time of the most recent request
  lastEventRequestTime_ = time(NULL);

  // return the event
  return bufPtr;
}

bool DQMConsumerPipe::pushEvent()
{
  // fetch the most recent event
  boost::shared_ptr< std::vector<char> > bufPtr;
  {
    boost::mutex::scoped_lock scopedLockForEventQueue(eventQueueLock_);
    if (! eventQueue_.empty())
    {
      bufPtr = eventQueue_.front();
      eventQueue_.pop_front();
    }
  }
  if (bufPtr.get() == NULL)
  {
    edm::LogError("pushEvent") << "========================================";
    edm::LogError("pushEvent") << "pushEvent called with empty event queue!";
    return false;
  }

  // push the next event out to a push mode consumer (SMProxyServer)
  FDEBUG(5) << "pushing out DQMevent to " << consumerName_ << std::endl;
  stor::ReadData data;

  data.d_.clear();
  // check if curl handle was obtained (at ctor) if not try again
  if(han_==0)
  {
    han_ = curl_easy_init();
    if(han_==0)
    {
      edm::LogError("pushEvent") << "Could not create curl handle";
      return false;
    }
    headers_ = curl_slist_append(headers_, "Content-Type: application/octet-stream");
    headers_ = curl_slist_append(headers_, "Content-Transfer-Encoding: binary");
    // Avoid the Expect: 100 continue automatic header that gives a 2 sec delay
    headers_ = curl_slist_append(headers_, "Expect:");
    setopt(han_, CURLOPT_HTTPHEADER, headers_);
    setopt(han_, CURLOPT_URL, consumerName_.c_str());
    setopt(han_, CURLOPT_WRITEFUNCTION, func);
  }
  setopt(han_,CURLOPT_WRITEDATA,&data);

  // build the event message
  DQMEventMsgView msgView(&(*bufPtr)[0]);

  // add the request message as a http post
  setopt(han_, CURLOPT_POSTFIELDS, msgView.startAddress());
  setopt(han_, CURLOPT_POSTFIELDSIZE, msgView.size());

  // send the HTTP POST, read the reply
  // explicitly close connection when using pthttp transport or sometimes it hangs
  // because somtimes curl does not see that the connection was closed and tries to reuse it
  setopt(han_,CURLOPT_FORBID_REUSE, 1);
  CURLcode messageStatus = curl_easy_perform(han_);

  if(messageStatus!=0)
  {
    std::cerr << "curl perform failed for pushDQMEvent" << std::endl;
    edm::LogError("pushEvent") << "curl perform failed for pushDQMEvent. "
        << "Could not register: probably XDAQ not running on Storage Manager"
        << " at " << consumerName_;
    return false;
  }
  // should really read the message to see if okay (if we had made one!)
  if(data.d_.length() == 0)
  {
    return true;
  } else {
    if(data.d_.length() > 0) {
      std::vector<char> buf(1024);
      int len = data.d_.length();
      buf.resize(len);
      for (int i=0; i<len ; i++) buf[i] = data.d_[i];
      const unsigned int MAX_DUMP_LENGTH = 1000;
      edm::LogError("pushEvent") << "========================================";
      edm::LogError("pushEvent") << "Unexpected pushDQMEvent response!";
      if (data.d_.length() <= MAX_DUMP_LENGTH) {
        edm::LogError("pushEvent") << "Here is the raw text that was returned:";
        edm::LogError("pushEvent") << data.d_;
      }
      else {
        edm::LogError("pushEvent") << "Here are the first " << MAX_DUMP_LENGTH <<
          " characters of the raw text that was returned:";
        edm::LogError("pushEvent") << (data.d_.substr(0, MAX_DUMP_LENGTH));
      }
      edm::LogError("pushEvent") << "========================================";
    }
  }
  return false;
}

void DQMConsumerPipe::clearQueue()
{
  boost::mutex::scoped_lock scopedLockForEventQueue(eventQueueLock_);
  eventQueue_.clear();
}
