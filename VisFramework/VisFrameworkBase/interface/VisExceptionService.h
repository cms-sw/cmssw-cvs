#ifndef VIS_FRAMEWORK_BASE_VIS_EXCEPTION_SERVICE_H
# define VIS_FRAMEWORK_BASE_VIS_EXCEPTION_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"
# include "VisFramework/VisFrameworkBase/interface/VisExceptionMessage.h"
# include "FWCore/Utilities/interface/Exception.h"
# include <classlib/utils/Error.h>
# include <exception>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisExceptionService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (VisExceptionService);
public:
    VisExceptionService (IgState *state);
    ~VisExceptionService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    void	exceptionCallback (std::exception *e);
    void        errorCallback (lat::Error *e);
    void	cmsExceptionCallback (cms::Exception *e);
    void        unhandledExceptionCallback (void);

private:
    virtual void        msgChanged (void);

    IgState    *m_state;       //< State context.
    QWidget    *m_mainWindow;
    VisExceptionMessage	m_msg;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_EXCEPTION_SERVICE_H
