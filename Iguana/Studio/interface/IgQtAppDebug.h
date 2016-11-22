#ifndef IGUANA_STUDIO_IG_QT_APP_DEBUG_H
# define IGUANA_STUDIO_IG_QT_APP_DEBUG_H

# include "Iguana/Studio/interface/config.h"
# include <QDialog>
# include <QObject>
# include <QTextStream>

class QTreeWidget;
class QTreeWidgetItem;
class QTimer;
class IgState;

class IGUANA_STUDIO_API IgQtAppDebugInfo : public QDialog
{
    Q_OBJECT
public:
    IgQtAppDebugInfo (QWidget *parent = 0, bool autoRefresh = true);
    ~IgQtAppDebugInfo (void);

public slots:
    void		info (void);
    void		exportList (void);
    virtual void	update (void) = 0;

protected:
    QTreeWidget *	list (void);
    void                hideEvent (QHideEvent*);

private:
  void		start (void);
  void 		iterateTreeWidget(QTextStream &out, QTreeWidgetItem *parent = 0);

    bool		m_autoRefresh;
    QTreeWidget		*m_data;
    QTimer		*m_timer;

    // undefined semantics
    IgQtAppDebugInfo (const IgQtAppDebugInfo &);
    IgQtAppDebugInfo &operator= (const IgQtAppDebugInfo &);
};

class IGUANA_STUDIO_API IgQtAppDebugResourceInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDebugResourceInfo (QWidget *parent);

protected:
    virtual void	update (void);
};

class IGUANA_STUDIO_API IgQtAppDebugLibrariesInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDebugLibrariesInfo (QWidget *parent);

protected:
    virtual void	update (void);
};

class IGUANA_STUDIO_API IgQtAppDebugPluginsInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDebugPluginsInfo (IgState *state, QWidget *parent);

protected:
    virtual void	update (void);

private:
    IgState		*m_state;
};

class IGUANA_STUDIO_API IgQtAppDumpStateInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDumpStateInfo (IgState *state, QWidget *parent);

protected:
    virtual void	update (void);
    virtual void 	populate (IgState* state, QTreeWidgetItem *item);
    
private:
    IgState		*m_state;
};

class IGUANA_STUDIO_API IgQtAppDebug : public QObject
{
    Q_OBJECT
public:
    IgQtAppDebug (IgState *state, QWidget *parent = 0);
    ~IgQtAppDebug (void);

public slots:
    void		resources (void);
    void		libraries (void);
    void		plugins (void);
    void		debugme (void);
    void		dumpState (void);
    void		rundebug (void);
    void		showProfiler (void);

private:
    static bool		m_profiler;

    IgState		*m_state;
    QWidget		*m_parent;
    IgQtAppDebugInfo	*m_info [4];

    // undefined semantics
    IgQtAppDebug (const IgQtAppDebug &);
    IgQtAppDebug &operator= (const IgQtAppDebug &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_DEBUG_H







