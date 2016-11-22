// $Id: FRDFileHandler.h,v 1.10.4.1 2010/05/21 10:22:02 mommsen Exp $
/// @file: FRDFileHandler.h 

#ifndef StorageManager_FRDFileHandler_h
#define StorageManager_FRDFileHandler_h

#include "EventFilter/StorageManager/interface/FileHandler.h"
#include "IOPool/Streamer/interface/FRDEventFileWriter.h"

#include <stdint.h>
#include <boost/scoped_ptr.hpp>


namespace stor {
  
  /**
   * Represents a file holding HLT error events in the
   * FED Raw Data (FRD) format.
   *
   * $Author: mommsen $
   * $Revision: 1.10.4.1 $
   * $Date: 2010/05/21 10:22:02 $
   */
  
  class FRDFileHandler : public FileHandler
  {
  public:
    FRDFileHandler
    (
      FilesMonitorCollection::FileRecordPtr,
      const DbFileHandlerPtr,
      const DiskWritingParams&,
      const unsigned long long& maxFileSize
    );

    /**
     * Returns true if the file has not seen any recent events
     */
    virtual bool tooOld(utils::time_point_t currentTime = utils::getCurrentTime());

    /**
     * Error events do not belong to a lumi section
     */
    virtual bool isFromSuperLumiSection(const uint32_t superLumiSection)
    { return false; }

    /**
     * Close the file
     */
    virtual void closeFile(const FilesMonitorCollection::FileRecord::ClosingReason&);
    
  private:
    
    /**
     * Write the I2OChain to the file
     */
    virtual void do_writeEvent(const I2OChain&);
    
    boost::scoped_ptr<FRDEventFileWriter> _writer; // writes FED Raw Data file
  };
  
} // stor namespace

#endif // StorageManager_FRDFileHandler_h


/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
