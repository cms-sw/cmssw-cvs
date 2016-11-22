//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLModels/interface/IgSpareRep.h"
#include "Iguana/GLModels/interface/IgSpareModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSpareRep::IgSpareRep (IgSpareModel *model,
		      IgSpareRep *parent,
		      IgSpareRep *prev,
		      SoGroup *node)
    : Ig3DBaseRep (model, parent, prev, node)
{}
