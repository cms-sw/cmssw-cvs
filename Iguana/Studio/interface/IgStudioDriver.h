#ifndef IGUANA_STUDIO_IG_STUDIO_DRIVER_H
# define IGUANA_STUDIO_IG_STUDIO_DRIVER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgDriver.h"
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioSetupExtension;
class IgSite;
class QString;
class QMainWindow;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgStudioDriver : public IgDriver
{
public:
    IgStudioDriver (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual int		run (void);
    virtual IgSite *	mainSite (void) const;
    virtual QMainWindow *mainWindow (void) const;
    virtual IgState *	state (void) const;

    static const char *	catalogLabel (void);

protected:
    virtual void	loadSettings (void);
    virtual void	setupMainWindow (void);
    virtual bool	setupSession (QString sessionType);

private:
    std::vector<std::string> getAliases (const std::string& name,
                                         const std::string& type);
    IgState		*m_state;
    IgStudioSetupExtension *m_setup;
    IgSite		*m_mainSite;
    QMainWindow		*m_mainWindow;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_STUDIO_DRIVER_H
