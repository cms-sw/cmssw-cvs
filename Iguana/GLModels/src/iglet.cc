//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgCapabilityDB.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgModuleDef.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/GLModels/interface/Ig2DModel.h"
#include "Iguana/GLModels/interface/IgLegoModel.h"
#include "Iguana/GLModels/interface/IgRPhiModel.h"
#include "Iguana/GLModels/interface/IgRZModel.h"
#include "Iguana/GLModels/interface/IgSpareModel.h"
#include "Iguana/GLModels/interface/IgOIVBrowser.h"
#include "Iguana/GLModels/interface/IgSoTwig.h"
#include "Iguana/Models/interface/IgTextModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

DEFINE_IGUANA_MODULE ();
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgTextModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (Ig3DModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (Ig2DModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgLegoModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgRZModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgRPhiModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgSpareModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (Ig3DModel).name (),
			typeid (IgSoTwig).name ()));

DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgOIVBrowser, IgOIVBrowser::catalogLabel ());
