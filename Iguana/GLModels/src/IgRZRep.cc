//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgRZModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgRZRep::IgRZRep (IgRZModel *model,
		  IgRZRep *parent,
		  IgRZRep *prev,
		  SoGroup *node)
    : Ig3DBaseRep (model, parent, prev, node)
{}
