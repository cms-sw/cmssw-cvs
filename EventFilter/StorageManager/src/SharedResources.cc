/**
 * $Id: SharedResources.cc,v 1.8 2010/03/04 16:59:05 mommsen Exp $
/// @file: SharedResources.cc
 */

#include "EventFilter/StorageManager/interface/Configuration.h"
#include "EventFilter/StorageManager/interface/DiscardManager.h"
#include "EventFilter/StorageManager/interface/DiskWriterResources.h"
#include "EventFilter/StorageManager/interface/DQMEventProcessorResources.h"
#include "EventFilter/StorageManager/interface/InitMsgCollection.h"
#include "EventFilter/StorageManager/interface/RegistrationCollection.h"
#include "EventFilter/StorageManager/interface/SharedResources.h"
#include "EventFilter/StorageManager/interface/StateMachine.h"
#include "EventFilter/StorageManager/interface/StatisticsReporter.h"

#include "xcept/tools.h"

#include <fstream>
#include <iostream>
#include <unistd.h>


namespace stor
{

  void SharedResources::moveToFailedState( xcept::Exception& exception )
  {
    std::string errorMsg = "Failed to process FAIL exception: "
      + xcept::stdformat_exception_history(exception) + " due to ";

    try
    {
      _statisticsReporter->alarmHandler()->notifySentinel(AlarmHandler::FATAL, exception);
      _statisticsReporter->getStateMachineMonitorCollection().setStatusMessage( 
        xcept::stdformat_exception_history(exception)
      );
      event_ptr stMachEvent( new Fail() );
      // wait maximum 5 seconds until enqueuing succeeds
      if ( ! _commandQueue->enq_timed_wait( stMachEvent, 5 ) )
      {
        XCEPT_DECLARE_NESTED( stor::exception::StateTransition,
          sentinelException, "Failed to enqueue FAIL event", exception );
        _statisticsReporter->alarmHandler()->
          notifySentinel(AlarmHandler::FATAL, sentinelException);
      }
    }
    catch(xcept::Exception &e)
    {
      errorMsg += xcept::stdformat_exception_history(e);
      localDebug( errorMsg );
    }
    catch(std::exception &e)
    {
      errorMsg += e.what();
      localDebug( errorMsg );
    }
    catch( ... )
    {
      errorMsg += "an unknown exception.";
      localDebug( errorMsg );
    }
  }


  void SharedResources::localDebug( const std::string& message ) const
  {
    std::ostringstream fname_oss;
    fname_oss << "/tmp/storage_manager_debug_" << 
      _configuration->getDiskWritingParams()._smInstanceString <<
      "_" << getpid();
    const std::string fname = fname_oss.str();
    std::ofstream f( fname.c_str(), std::ios_base::ate | std::ios_base::out | std::ios_base::app );
    if( f.is_open() )
    {
      try
      {
        f << message << std::endl;
        f.close();
      }
      catch(...)
      {}
    }
  }

    
} // namespace stor

/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
