/********************************************************************************
** Form generated from reading ui file 'IgStudioSetup.ui'
**
** Created: Tue Jan 12 08:56:17 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGSTUDIOSETUP_H
#define UI_IGSTUDIOSETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_IgStudioSetupDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *m_label;
    QDialogButtonBox *buttonBox;
    QListWidget *m_configs;

    void setupUi(QDialog *IgStudioSetupDialog)
    {
        if (IgStudioSetupDialog->objectName().isEmpty())
            IgStudioSetupDialog->setObjectName(QString::fromUtf8("IgStudioSetupDialog"));
        IgStudioSetupDialog->resize(408, 205);
        gridLayout = new QGridLayout(IgStudioSetupDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_label = new QLabel(IgStudioSetupDialog);
        m_label->setObjectName(QString::fromUtf8("m_label"));

        gridLayout->addWidget(m_label, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(IgStudioSetupDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 0, 1, 2, 1);

        m_configs = new QListWidget(IgStudioSetupDialog);
        m_configs->setObjectName(QString::fromUtf8("m_configs"));

        gridLayout->addWidget(m_configs, 1, 0, 1, 1);


        retranslateUi(IgStudioSetupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), IgStudioSetupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), IgStudioSetupDialog, SLOT(reject()));
        QObject::connect(m_configs, SIGNAL(itemDoubleClicked(QListWidgetItem*)), IgStudioSetupDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(IgStudioSetupDialog);
    } // setupUi

    void retranslateUi(QDialog *IgStudioSetupDialog)
    {
        IgStudioSetupDialog->setWindowTitle(QApplication::translate("IgStudioSetupDialog", "IGUANA Setup", 0, QApplication::UnicodeUTF8));
        m_label->setText(QApplication::translate("IgStudioSetupDialog", "Please, select configuation", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(IgStudioSetupDialog);
    } // retranslateUi

};

namespace Ui {
    class IgStudioSetupDialog: public Ui_IgStudioSetupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGSTUDIOSETUP_H
