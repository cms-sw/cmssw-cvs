#ifndef IGUANA_GL_MODELS_IG_3D_BASE_REP_H
# define IGUANA_GL_MODELS_IG_3D_BASE_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLModels/interface/config.h"
# include "Iguana/Framework/interface/IgRep.h"
# include <Inventor/nodes/SoSeparator.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseModel;
class SoGroup;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IGUANA_GL_MODELS_API Ig3DBaseRep : public SoSeparator, public IgRep
{
public:
    Ig3DBaseRep (Ig3DBaseModel *model, Ig3DBaseRep *parent, Ig3DBaseRep *prev, SoGroup *node);
    ~Ig3DBaseRep (void);

    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;

    virtual SoGroup *		magic (void) const;
    virtual SoGroup *		magic (SoGroup *node);
    virtual SoNode *		findMagic (SbName name) const;

    virtual SoGroup *		node (void) const;
    virtual SoGroup *		node (SoGroup *node);

    virtual int			children (void) const;
    virtual Ig3DBaseRep *	child (int n) const;
    virtual Ig3DBaseRep *	child (SbString name) const;

    virtual void		clear (void);

    // FIXME: virtual void	pick (path, detail);
    // FIXME: remember last pick point/path detail?

    static Ig3DBaseRep *	asRep (SoNode *node);

protected:
    virtual void		context (IgRepContext *context);

    virtual void		zap (bool search = false, bool kill = true);

private:
    IgRepContext		*m_context;
    Ig3DBaseModel		*m_model;

    // undefined semantics
    Ig3DBaseRep (const Ig3DBaseRep &);
    Ig3DBaseRep &operator= (const Ig3DBaseRep &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_MODELS_IG_3D_BASE_REP_H
