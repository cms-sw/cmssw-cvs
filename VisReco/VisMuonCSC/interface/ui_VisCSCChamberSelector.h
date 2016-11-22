/********************************************************************************
** Form generated from reading ui file 'VisCSCChamberSelector.ui'
**
** Created: Fri Jul 24 11:05:36 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VISCSCCHAMBERSELECTOR_H
#define UI_VISCSCCHAMBERSELECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VisCSCChamberSelector
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *ChamberSpin;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpinBox *RingSpin;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpinBox *StationSpin;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QSpinBox *EndcapSpin;
    QRadioButton *ApplyFilter;

    void setupUi(QWidget *VisCSCChamberSelector)
    {
        if (VisCSCChamberSelector->objectName().isEmpty())
            VisCSCChamberSelector->setObjectName(QString::fromUtf8("VisCSCChamberSelector"));
        VisCSCChamberSelector->resize(151, 208);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VisCSCChamberSelector->sizePolicy().hasHeightForWidth());
        VisCSCChamberSelector->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(VisCSCChamberSelector);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(VisCSCChamberSelector);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        ChamberSpin = new QSpinBox(groupBox);
        ChamberSpin->setObjectName(QString::fromUtf8("ChamberSpin"));
        ChamberSpin->setMaximum(36);

        horizontalLayout->addWidget(ChamberSpin);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        RingSpin = new QSpinBox(groupBox);
        RingSpin->setObjectName(QString::fromUtf8("RingSpin"));
        RingSpin->setMaximum(4);

        horizontalLayout_2->addWidget(RingSpin);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        StationSpin = new QSpinBox(groupBox);
        StationSpin->setObjectName(QString::fromUtf8("StationSpin"));
        StationSpin->setMaximum(4);

        horizontalLayout_3->addWidget(StationSpin);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        EndcapSpin = new QSpinBox(groupBox);
        EndcapSpin->setObjectName(QString::fromUtf8("EndcapSpin"));
        EndcapSpin->setMaximum(2);

        horizontalLayout_4->addWidget(EndcapSpin);


        verticalLayout->addLayout(horizontalLayout_4);

        ApplyFilter = new QRadioButton(groupBox);
        ApplyFilter->setObjectName(QString::fromUtf8("ApplyFilter"));

        verticalLayout->addWidget(ApplyFilter);


        verticalLayout_2->addWidget(groupBox);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(ChamberSpin);
        label_2->setBuddy(RingSpin);
        label_3->setBuddy(StationSpin);
        label_4->setBuddy(EndcapSpin);
#endif // QT_NO_SHORTCUT

        retranslateUi(VisCSCChamberSelector);

        QMetaObject::connectSlotsByName(VisCSCChamberSelector);
    } // setupUi

    void retranslateUi(QWidget *VisCSCChamberSelector)
    {
        VisCSCChamberSelector->setWindowTitle(QApplication::translate("VisCSCChamberSelector", "CSC Chamber Selector", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        VisCSCChamberSelector->setWhatsThis(QApplication::translate("VisCSCChamberSelector", "Controls visibility of the strip readout. A zero value implies all.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        groupBox->setTitle(QApplication::translate("VisCSCChamberSelector", "CSC Chamber Selector", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        label->setWhatsThis(QApplication::translate("VisCSCChamberSelector", "Limit displayed strips", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label->setText(QApplication::translate("VisCSCChamberSelector", "Chamber", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("VisCSCChamberSelector", "Ring", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("VisCSCChamberSelector", "Station", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("VisCSCChamberSelector", "Endcap", 0, QApplication::UnicodeUTF8));
        ApplyFilter->setText(QApplication::translate("VisCSCChamberSelector", "Selector On", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(VisCSCChamberSelector);
    } // retranslateUi

};

namespace Ui {
    class VisCSCChamberSelector: public Ui_VisCSCChamberSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISCSCCHAMBERSELECTOR_H
