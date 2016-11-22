#include "Iguana/Studio/interface/IgStudioSetup.h"

/* 
 *  Constructs a IgStudioSetup as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgStudioSetup::IgStudioSetup(QWidget* parent)
    : QDialog(parent)
{
  setupUi(this);
}
