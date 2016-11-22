#ifndef IGUANA_STUDIO_IG_QT_CALLBACKS_H
# define IGUANA_STUDIO_IG_QT_CALLBACKS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QObject;
class QTabWidget;
class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

typedef struct
{
    QObject *m_source;
    QObject *m_dest;
    std::string m_signal;
    std::string m_slot;    
} IgQtConnection;

typedef struct
{
    QTabWidget *m_tab;
    QWidget *m_panel;
} IgQtTabConnection;

void *createQtConnection (QObject *source,
			  const char *signal,
			  QObject *dest,
			  const char *slot);

void *createQtTabConnection (QTabWidget *tab,
			     QWidget *panel);

void qtConnectionCallback (void *data, bool focus);
void qtTabCallback (void *data, bool focus);
void qtShowHideCallback (void *data, bool focus);


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_CALLBACKS_H
