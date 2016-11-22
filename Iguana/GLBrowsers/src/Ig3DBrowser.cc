#include "Iguana/GLBrowsers/interface/Ig3DBrowser.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"

const char *
Ig3DBrowser::catalogLabel (void)
{ return "3D"; }

Ig3DBrowser::Ig3DBrowser (IgState *state, 
			  IgSite *site, 
			  Ig3DModel *model /* = 0 */)
    : Ig3DBaseBrowser (state, 
		       site, 
		       model ? model : new Ig3DModel (state))
{ }
