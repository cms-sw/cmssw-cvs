#ifndef IGSTUDIOSETUP_H
#define IGSTUDIOSETUP_H

# include "Iguana/Studio/interface/ui_IgStudioSetup.h"

class IgStudioSetup : public QDialog, public  Ui::IgStudioSetupDialog
{
    Q_OBJECT

public:
    IgStudioSetup(QWidget* parent = 0);
};

#endif // IGSTUDIOSETUP_H
