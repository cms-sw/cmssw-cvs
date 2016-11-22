#ifndef IGUANA_MODELS_IG_SIMPLE_TEXT_REP_H
# define IGUANA_MODELS_IG_SIMPLE_TEXT_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Models/interface/IgTextRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_MODELS_API IgSimpleTextRep : public IgTextRep
{
public:
    IgSimpleTextRep (IgTextModel *model, const std::string &text);
    ~IgSimpleTextRep (void);
    // default copy constructor
    // default assignment operator

    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;

    virtual const std::string &	text (void) const;
    virtual void		setText (const std::string &text);

protected:
    virtual void		context (IgRepContext *context);

private:
    IgTextModel *		m_model;
    IgRepContext *		m_context;
    std::string			m_text;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_MODELS_IG_SIMPLE_TEXT_REP_H
