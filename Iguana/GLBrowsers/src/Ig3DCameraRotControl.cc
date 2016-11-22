#include "Iguana/GLBrowsers/interface/Ig3DCameraRotControl.h"

Ig3DCameraRotControl::Ig3DCameraRotControl (IgControlCategory *pane,
					    QString label)
    : Ig3DRotationControl (pane, label)
{}

void
Ig3DCameraRotControl::applyValue (void)
{
    SbVec3f fp = getFocalPoint ();
    Ig3DRotationControl::applyValue ();
    setFocalPoint (fp);
}
