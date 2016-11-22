#ifndef IGUANA_STUDIO_IG_QT_TWIG_REP_H
# define IGUANA_STUDIO_IG_QT_TWIG_REP_H

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgRep.h"
# include <QTreeWidget>
# include <QPixmap>
# include <vector>
# include <assert.h>

class IgQtTwigBrowser;

// FIXME: this should inherit from a twig to avoid repetitive coding!
class IGUANA_STUDIO_API IgQtTwigRep : public QTreeWidgetItem, public IgRep
{
public:
    static const int		VISIBLE_FIELD  = 1;
    static const int		TRAVERSE_FIELD = 2;
    static const int		CASCADE_FIELD  = 3;

    static IgQtTwigRep *	create (IgQtTwigBrowser *in,
					IgQtTwigRep *parent,
					IgQtTwigRep *before,
					QString name, bool traverse,
					bool visible, bool opened);

    IgQtTwigRep (IgQtTwigRep *parent, IgQtTwigRep *before,
		 QString name, bool traverse, bool visible, bool opened);
    IgQtTwigRep (IgQtTwigRep *parent,
		 QString name, bool traverse, bool visible, bool opened);
    IgQtTwigRep (IgQtTwigBrowser *in, IgQtTwigRep *before,
		 QString name, bool traverse, bool visible, bool opened);
    IgQtTwigRep (IgQtTwigBrowser *in,
		 QString name, bool traverse, bool visible, bool opened);
    ~IgQtTwigRep (void);

    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;

    bool			checked (int column) const;
    void			check (int column, bool value = true);

    bool			bold (void) const;
    void			bold (bool value);

    bool			italic (void) const;
    void			italic (bool value);

    bool			magic (void) const;
    void			magic (bool value);

    void			setCheck (int column, bool value, bool known);
    void			setOpen (bool opened);
    bool			traversed (void) const;

protected:
    virtual void		context (IgRepContext *context);

    void			init (bool traverse, bool visible, bool opened);
    QColorGroup			columnColor (int column, const QColorGroup &cg) const;

private:
    bool			flag (unsigned n) const;
    void			flag (unsigned n, bool value);

    static const unsigned int	CHECKABLE = 1;
    static const unsigned int	BOLD	  = 2;
    static const unsigned int	ITALIC	  = 4;
    static const unsigned int	MAGIC	  = 8;
    static const unsigned int	EXPANDED  = 16;
    static const unsigned int	TRAVERSE  = 32;
    static const unsigned int	VISIBLE	  = 64;
    static const unsigned int	AUTO_OPEN = 128;

    IgRepContext		*m_context;
    unsigned			m_flags;

    // undefined semantics
    IgQtTwigRep (const IgQtTwigRep &);
    IgQtTwigRep &operator= (const IgQtTwigRep &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline bool
IgQtTwigRep::flag (unsigned n) const
{ return m_flags & n; }

inline void
IgQtTwigRep::flag (unsigned n, bool value)
{ m_flags = (m_flags & ~n) | (n * value); }

inline bool
IgQtTwigRep::bold (void) const
{ return flag (BOLD); }

inline void
IgQtTwigRep::bold (bool value)
{ flag (BOLD, value); }

inline bool
IgQtTwigRep::italic (void) const
{ return flag (ITALIC); }

inline void
IgQtTwigRep::italic (bool value)
{ flag (ITALIC, value); }

inline bool
IgQtTwigRep::magic (void) const
{ return flag (MAGIC); }

inline void
IgQtTwigRep::magic (bool value)
{ flag (MAGIC, value); }

inline bool
IgQtTwigRep::checked (int column) const
{
    assert(column == TRAVERSE_FIELD || column == VISIBLE_FIELD);
    return flag (column == TRAVERSE_FIELD ? TRAVERSE : VISIBLE);
}

#endif // IGUANA_STUDIO_IG_QT_TWIG_REP_H
