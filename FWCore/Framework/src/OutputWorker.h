#ifndef Framework_OutputWorker_h
#define Framework_OutputWorker_h

/*----------------------------------------------------------------------
  
OutputWorker: The OutputModule as the schedule sees it.  The job of
this object is to call the output module.

According to our current definition, a single output module can only
appear in one worker.

$Id: OutputWorker.h,v 1.35 2008/07/29 02:17:36 wmtan Exp $
----------------------------------------------------------------------*/

#include <memory>

#include "boost/shared_ptr.hpp"

#include "FWCore/Framework/src/WorkerT.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

namespace edm {

  class OutputWorker : public WorkerT<OutputModule> {
  public:
    OutputWorker(std::auto_ptr<OutputModule> mod, 
		 ModuleDescription const&,
		 WorkerParams const&);

    virtual ~OutputWorker();

    // Call closeFile() on the controlled OutputModule.
    void closeFile();

    // Call shouldWeCloseFile() on the controlled OutputModule.
    bool shouldWeCloseFile() const;

    void openNewFileIfNeeded();

    bool wantAllEvents() const;

    void openFile(FileBlock const& fb);

    void writeRun(RunPrincipal const& rp);

    void writeLumi(LuminosityBlockPrincipal const& lbp);

    bool limitReached() const;

    void configure(OutputModuleDescription const& desc);
  };

}

#endif
