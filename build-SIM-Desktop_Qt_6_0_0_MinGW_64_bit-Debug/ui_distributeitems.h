/********************************************************************************
** Form generated from reading UI file 'distributeitems.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISTRIBUTEITEMS_H
#define UI_DISTRIBUTEITEMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DistributeItems
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *MainWidget;
    QVBoxLayout *verticalLayout_10;
    QLabel *Title1_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_7;
    QLabel *Title2_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QWidget *ItemInfo1_2;
    QHBoxLayout *horizontalLayout_4;
    QWidget *UsedForWidget_2;
    QVBoxLayout *verticalLayout_11;
    QLabel *UsedForLabel_2;
    QLineEdit *UsedForText_2;
    QWidget *ItemInfo2_2;
    QHBoxLayout *horizontalLayout_5;
    QWidget *ItemIdWidget_2;
    QVBoxLayout *verticalLayout_13;
    QLabel *ItemIdLabel_2;
    QLineEdit *ItemIdText_2;
    QWidget *ItemTypeWidget_2;
    QVBoxLayout *verticalLayout_15;
    QLabel *ItemTypeLabel_2;
    QLineEdit *ItemTypeText_2;
    QWidget *QtyWidget_2;
    QVBoxLayout *verticalLayout_16;
    QLabel *QtyLabel_2;
    QLineEdit *QtyText_2;
    QWidget *DescriptionWidget_2;
    QVBoxLayout *verticalLayout_17;
    QLabel *DescLabel_2;
    QLineEdit *DescText_2;
    QWidget *Spacer_2;
    QWidget *ConfirmDiscard_2;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *Confirm_2;
    QPushButton *Discard_2;

    void setupUi(QDialog *DistributeItems)
    {
        if (DistributeItems->objectName().isEmpty())
            DistributeItems->setObjectName(QString::fromUtf8("DistributeItems"));
        DistributeItems->resize(400, 329);
        verticalLayout = new QVBoxLayout(DistributeItems);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        MainWidget = new QWidget(DistributeItems);
        MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        verticalLayout_10 = new QVBoxLayout(MainWidget);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        Title1_2 = new QLabel(MainWidget);
        Title1_2->setObjectName(QString::fromUtf8("Title1_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(Title1_2->sizePolicy().hasHeightForWidth());
        Title1_2->setSizePolicy(sizePolicy);

        verticalLayout_10->addWidget(Title1_2);

        widget = new QWidget(MainWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout_7 = new QHBoxLayout(widget);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, -1, 0, -1);
        Title2_2 = new QLabel(widget);
        Title2_2->setObjectName(QString::fromUtf8("Title2_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(Title2_2->sizePolicy().hasHeightForWidth());
        Title2_2->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(Title2_2);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setLayoutDirection(Qt::RightToLeft);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(label);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(lineEdit);


        verticalLayout_10->addWidget(widget);

        ItemInfo1_2 = new QWidget(MainWidget);
        ItemInfo1_2->setObjectName(QString::fromUtf8("ItemInfo1_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(3);
        sizePolicy4.setHeightForWidth(ItemInfo1_2->sizePolicy().hasHeightForWidth());
        ItemInfo1_2->setSizePolicy(sizePolicy4);
        horizontalLayout_4 = new QHBoxLayout(ItemInfo1_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 0, -1, 0);
        UsedForWidget_2 = new QWidget(ItemInfo1_2);
        UsedForWidget_2->setObjectName(QString::fromUtf8("UsedForWidget_2"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(5);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(UsedForWidget_2->sizePolicy().hasHeightForWidth());
        UsedForWidget_2->setSizePolicy(sizePolicy5);
        verticalLayout_11 = new QVBoxLayout(UsedForWidget_2);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(0, -1, 0, 0);
        UsedForLabel_2 = new QLabel(UsedForWidget_2);
        UsedForLabel_2->setObjectName(QString::fromUtf8("UsedForLabel_2"));

        verticalLayout_11->addWidget(UsedForLabel_2);

        UsedForText_2 = new QLineEdit(UsedForWidget_2);
        UsedForText_2->setObjectName(QString::fromUtf8("UsedForText_2"));

        verticalLayout_11->addWidget(UsedForText_2);


        horizontalLayout_4->addWidget(UsedForWidget_2);


        verticalLayout_10->addWidget(ItemInfo1_2);

        ItemInfo2_2 = new QWidget(MainWidget);
        ItemInfo2_2->setObjectName(QString::fromUtf8("ItemInfo2_2"));
        sizePolicy4.setHeightForWidth(ItemInfo2_2->sizePolicy().hasHeightForWidth());
        ItemInfo2_2->setSizePolicy(sizePolicy4);
        horizontalLayout_5 = new QHBoxLayout(ItemInfo2_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 0, -1, 0);
        ItemIdWidget_2 = new QWidget(ItemInfo2_2);
        ItemIdWidget_2->setObjectName(QString::fromUtf8("ItemIdWidget_2"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(6);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(ItemIdWidget_2->sizePolicy().hasHeightForWidth());
        ItemIdWidget_2->setSizePolicy(sizePolicy6);
        verticalLayout_13 = new QVBoxLayout(ItemIdWidget_2);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(0, 0, 0, 0);
        ItemIdLabel_2 = new QLabel(ItemIdWidget_2);
        ItemIdLabel_2->setObjectName(QString::fromUtf8("ItemIdLabel_2"));

        verticalLayout_13->addWidget(ItemIdLabel_2);

        ItemIdText_2 = new QLineEdit(ItemIdWidget_2);
        ItemIdText_2->setObjectName(QString::fromUtf8("ItemIdText_2"));

        verticalLayout_13->addWidget(ItemIdText_2);


        horizontalLayout_5->addWidget(ItemIdWidget_2);

        ItemTypeWidget_2 = new QWidget(ItemInfo2_2);
        ItemTypeWidget_2->setObjectName(QString::fromUtf8("ItemTypeWidget_2"));
        sizePolicy6.setHeightForWidth(ItemTypeWidget_2->sizePolicy().hasHeightForWidth());
        ItemTypeWidget_2->setSizePolicy(sizePolicy6);
        verticalLayout_15 = new QVBoxLayout(ItemTypeWidget_2);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        verticalLayout_15->setContentsMargins(0, 0, 0, 0);
        ItemTypeLabel_2 = new QLabel(ItemTypeWidget_2);
        ItemTypeLabel_2->setObjectName(QString::fromUtf8("ItemTypeLabel_2"));

        verticalLayout_15->addWidget(ItemTypeLabel_2);

        ItemTypeText_2 = new QLineEdit(ItemTypeWidget_2);
        ItemTypeText_2->setObjectName(QString::fromUtf8("ItemTypeText_2"));

        verticalLayout_15->addWidget(ItemTypeText_2);


        horizontalLayout_5->addWidget(ItemTypeWidget_2);

        QtyWidget_2 = new QWidget(ItemInfo2_2);
        QtyWidget_2->setObjectName(QString::fromUtf8("QtyWidget_2"));
        QtyWidget_2->setEnabled(true);
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(2);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(QtyWidget_2->sizePolicy().hasHeightForWidth());
        QtyWidget_2->setSizePolicy(sizePolicy7);
        verticalLayout_16 = new QVBoxLayout(QtyWidget_2);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        verticalLayout_16->setContentsMargins(0, 0, 0, 0);
        QtyLabel_2 = new QLabel(QtyWidget_2);
        QtyLabel_2->setObjectName(QString::fromUtf8("QtyLabel_2"));

        verticalLayout_16->addWidget(QtyLabel_2);

        QtyText_2 = new QLineEdit(QtyWidget_2);
        QtyText_2->setObjectName(QString::fromUtf8("QtyText_2"));

        verticalLayout_16->addWidget(QtyText_2);


        horizontalLayout_5->addWidget(QtyWidget_2);


        verticalLayout_10->addWidget(ItemInfo2_2);

        DescriptionWidget_2 = new QWidget(MainWidget);
        DescriptionWidget_2->setObjectName(QString::fromUtf8("DescriptionWidget_2"));
        sizePolicy4.setHeightForWidth(DescriptionWidget_2->sizePolicy().hasHeightForWidth());
        DescriptionWidget_2->setSizePolicy(sizePolicy4);
        verticalLayout_17 = new QVBoxLayout(DescriptionWidget_2);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(-1, 0, -1, 0);
        DescLabel_2 = new QLabel(DescriptionWidget_2);
        DescLabel_2->setObjectName(QString::fromUtf8("DescLabel_2"));

        verticalLayout_17->addWidget(DescLabel_2);

        DescText_2 = new QLineEdit(DescriptionWidget_2);
        DescText_2->setObjectName(QString::fromUtf8("DescText_2"));

        verticalLayout_17->addWidget(DescText_2);


        verticalLayout_10->addWidget(DescriptionWidget_2);

        Spacer_2 = new QWidget(MainWidget);
        Spacer_2->setObjectName(QString::fromUtf8("Spacer_2"));
        QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(6);
        sizePolicy8.setHeightForWidth(Spacer_2->sizePolicy().hasHeightForWidth());
        Spacer_2->setSizePolicy(sizePolicy8);
        Spacer_2->setMinimumSize(QSize(0, 0));

        verticalLayout_10->addWidget(Spacer_2);

        ConfirmDiscard_2 = new QWidget(MainWidget);
        ConfirmDiscard_2->setObjectName(QString::fromUtf8("ConfirmDiscard_2"));
        horizontalLayout_6 = new QHBoxLayout(ConfirmDiscard_2);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        Confirm_2 = new QPushButton(ConfirmDiscard_2);
        Confirm_2->setObjectName(QString::fromUtf8("Confirm_2"));

        horizontalLayout_6->addWidget(Confirm_2);

        Discard_2 = new QPushButton(ConfirmDiscard_2);
        Discard_2->setObjectName(QString::fromUtf8("Discard_2"));
        Discard_2->setStyleSheet(QString::fromUtf8("background-color: #a4a4a4;\n"
"color: black;\n"
""));

        horizontalLayout_6->addWidget(Discard_2);


        verticalLayout_10->addWidget(ConfirmDiscard_2);


        verticalLayout->addWidget(MainWidget);


        retranslateUi(DistributeItems);

        QMetaObject::connectSlotsByName(DistributeItems);
    } // setupUi

    void retranslateUi(QDialog *DistributeItems)
    {
        DistributeItems->setWindowTitle(QCoreApplication::translate("DistributeItems", "Dialog", nullptr));
        Title1_2->setText(QCoreApplication::translate("DistributeItems", "Distribution Report #: [last MRQ + 1] (AKA MRQ)", nullptr));
        Title2_2->setText(QCoreApplication::translate("DistributeItems", "Authorized by: [Current User]", nullptr));
        label->setText(QCoreApplication::translate("DistributeItems", "Requested by:", nullptr));
        UsedForLabel_2->setText(QCoreApplication::translate("DistributeItems", "Used For", nullptr));
        UsedForText_2->setPlaceholderText(QCoreApplication::translate("DistributeItems", "Eg. Volvo A20 ", nullptr));
        ItemIdLabel_2->setText(QCoreApplication::translate("DistributeItems", "Item ID", nullptr));
        ItemIdText_2->setText(QString());
        ItemIdText_2->setPlaceholderText(QCoreApplication::translate("DistributeItems", "Eg. F-2156", nullptr));
        ItemTypeLabel_2->setText(QCoreApplication::translate("DistributeItems", "Item Type", nullptr));
        ItemTypeText_2->setPlaceholderText(QCoreApplication::translate("DistributeItems", "Eg. Filter", nullptr));
        QtyLabel_2->setText(QCoreApplication::translate("DistributeItems", "Qty", nullptr));
        QtyText_2->setPlaceholderText(QCoreApplication::translate("DistributeItems", "Eg. 14", nullptr));
        DescLabel_2->setText(QCoreApplication::translate("DistributeItems", "Description", nullptr));
        DescText_2->setPlaceholderText(QCoreApplication::translate("DistributeItems", "Eg. 5/8\" Thick Carbon Filter", nullptr));
        Confirm_2->setText(QCoreApplication::translate("DistributeItems", "Confirm", nullptr));
        Discard_2->setText(QCoreApplication::translate("DistributeItems", "Discard", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DistributeItems: public Ui_DistributeItems {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISTRIBUTEITEMS_H
