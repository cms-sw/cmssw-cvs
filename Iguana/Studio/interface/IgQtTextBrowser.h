#ifndef IGUANA_STUDIO_IG_QT_TEXT_BROWSER_H
# define IGUANA_STUDIO_IG_QT_TEXT_BROWSER_H

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgBrowser.h"
# include <QTextBrowser>

class IgState;
class IgTextRep;
class IgSimpleTextModel;
class IgTextModelEvent;
class IgSite;
class IgSelectionMessage;

class IGUANA_STUDIO_API IgQtTextBrowser : public QTextBrowser,
					  public IgBrowser
{
public:
    IgQtTextBrowser (IgState *state, IgSite *site);
    ~IgQtTextBrowser (void);

    virtual void	browse (IgRepresentable *object);

    static const char *	catalogLabel (void);

protected:
    virtual void	textChanged (IgTextModelEvent event);
    virtual void	selectMessage (IgSelectionMessage message);
    virtual void	select (IgTextRep *rep);

private:
    IgState		*m_state;
    IgSite		*m_site;
    IgSimpleTextModel	*m_model;

    // undefined semantics
    IgQtTextBrowser (const IgQtTextBrowser &);
    IgQtTextBrowser &operator= (const IgQtTextBrowser &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_TEXT_BROWSER_H
