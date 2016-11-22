#include "Iguana/Studio/interface/IgQtAppExtensions.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include "Iguana/Studio/interface/IgQtAppLoopService.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppToolBarService.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/Studio/interface/IgQtAppDebugService.h"
#include "Iguana/Studio/interface/IgQtAppHelpService.h"
#include "Iguana/Studio/interface/IgQtAppCrashAlertService.h"

// Install common Qt main window services:
//   - main loop service
//   - main menu service
//   - tool bar service
//   - status bar service
//   - current user attention context
//   - debugging menu (debugging builds only)
//   - help menu
//   - crash alert service  (<-- Rob's great name invention!)

void
IgQtAppExtensions::install (IgState *state, QWidget *mainWindow)
{
    new IgQtAppWindowService (state, mainWindow);
    new IgQtAppLoopService (state);
    new IgQtAppMenuService (state, mainWindow);
    new IgQtAppToolBarService (state, mainWindow);
    new IgQtAppStatusBarService (state, mainWindow);
    new IgQtAppContextService (state, mainWindow);
    new IgQtAppDebugService (state, mainWindow);
    new IgQtAppHelpService (state, mainWindow);
    new IgQtAppCrashAlertService (state, mainWindow);
}
