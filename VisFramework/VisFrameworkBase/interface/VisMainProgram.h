#ifndef VIS_FRAMEWORK_BASE_VIS_MAIN_PROGRAM_H
# define VIS_FRAMEWORK_BASE_VIS_MAIN_PROGRAM_H

# include "FWCore/Utilities/interface/Exception.h"
# include <classlib/utils/Error.h>
# include <exception>
# include "boost/shared_ptr.hpp"

class IgState;
class QWidget;

/** edm::EventProcessor visualisation main program interface.  */
class VisMainProgram
{
public:
  VisMainProgram (IgState *state);

  void		startingProcessorCallback (void);
  void		stoppingProcessorCallback (void);
  void		processorStoppedCallback (void);
  void 		processorStartedCallback (void);
  void		exceptionCallback (std::exception *e);
  void		cmsExceptionCallback (cms::Exception *e);
  void		frameworkExceptionCallback (lat::Error *e);
  void		unhandledExceptionCallback (void);
  void		configFailureCallback (cms::Exception *e);
  void		statusMessageCallback (const std::string& msg);

private:
  void		init (void);
  void		quit (void);

  IgState		*m_state;	//< State context.
  QWidget		*m_mainWindow;
};

#endif // VIS_FRAMEWORK_BASE_VIS_MAIN_PROGRAM_H
