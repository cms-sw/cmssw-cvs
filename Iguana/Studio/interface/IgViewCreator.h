#ifndef IGUANA_STUDIO_IG_VIEW_CREATOR_H
# define IGUANA_STUDIO_IG_VIEW_CREATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Studio/interface/IgViewDB.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: this can probably be done with callbacks.

class IGUANA_STUDIO_API IgViewCreator : public QObject
{
    Q_OBJECT
public:
    IgViewCreator (IgViewDB::Iterator &i, IgPage *page);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
public slots:
void createView (void);
private:
    IgPage 		*m_page;
    IgViewDB::Iterator	m_iterator;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_VIEW_CREATOR_H
