/********************************************************************************
** Form generated from reading UI file 'receiveitems.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECEIVEITEMS_H
#define UI_RECEIVEITEMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReceiveItems
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *MainWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *Title1;
    QLabel *Title2;
    QWidget *ItemInfo1;
    QHBoxLayout *horizontalLayout_2;
    QWidget *UsedForWidget;
    QVBoxLayout *verticalLayout_7;
    QLabel *UsedForLabel;
    QLineEdit *UsedForText;
    QWidget *POWidget;
    QVBoxLayout *verticalLayout_4;
    QLabel *POText;
    QComboBox *POComboBox;
    QWidget *ItemInfo2;
    QHBoxLayout *horizontalLayout;
    QWidget *ItemIdWidget;
    QVBoxLayout *verticalLayout_5;
    QLabel *ItemIdLabel;
    QLineEdit *ItemIdText;
    QWidget *ItemIdWidget2;
    QVBoxLayout *verticalLayout_9;
    QLabel *ItemIdLabel2;
    QComboBox *ItemIdComboBox;
    QWidget *ItemTypeWidget;
    QVBoxLayout *verticalLayout_6;
    QLabel *ItemTypeLabel;
    QLineEdit *ItemTypeText;
    QWidget *QtyWidget;
    QVBoxLayout *verticalLayout_3;
    QLabel *QtyLabel;
    QLineEdit *QtyText;
    QWidget *DescriptionWidget;
    QVBoxLayout *verticalLayout_8;
    QLabel *DescLabel;
    QLineEdit *DescText;
    QWidget *Spacer;
    QWidget *ConfirmDiscard;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *Confirm;
    QPushButton *Discard;

    void setupUi(QDialog *ReceiveItems)
    {
        if (ReceiveItems->objectName().isEmpty())
            ReceiveItems->setObjectName(QString::fromUtf8("ReceiveItems"));
        ReceiveItems->resize(335, 361);
        QPalette palette;
        QBrush brush(QColor(62, 62, 62, 128));
        brush.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush);
#endif
        ReceiveItems->setPalette(palette);
        verticalLayout = new QVBoxLayout(ReceiveItems);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        MainWidget = new QWidget(ReceiveItems);
        MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        verticalLayout_2 = new QVBoxLayout(MainWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        Title1 = new QLabel(MainWidget);
        Title1->setObjectName(QString::fromUtf8("Title1"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(Title1->sizePolicy().hasHeightForWidth());
        Title1->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(Title1);

        Title2 = new QLabel(MainWidget);
        Title2->setObjectName(QString::fromUtf8("Title2"));
        sizePolicy.setHeightForWidth(Title2->sizePolicy().hasHeightForWidth());
        Title2->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(Title2);

        ItemInfo1 = new QWidget(MainWidget);
        ItemInfo1->setObjectName(QString::fromUtf8("ItemInfo1"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(3);
        sizePolicy1.setHeightForWidth(ItemInfo1->sizePolicy().hasHeightForWidth());
        ItemInfo1->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(ItemInfo1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        UsedForWidget = new QWidget(ItemInfo1);
        UsedForWidget->setObjectName(QString::fromUtf8("UsedForWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(5);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(UsedForWidget->sizePolicy().hasHeightForWidth());
        UsedForWidget->setSizePolicy(sizePolicy2);
        verticalLayout_7 = new QVBoxLayout(UsedForWidget);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, -1, 0, 0);
        UsedForLabel = new QLabel(UsedForWidget);
        UsedForLabel->setObjectName(QString::fromUtf8("UsedForLabel"));

        verticalLayout_7->addWidget(UsedForLabel);

        UsedForText = new QLineEdit(UsedForWidget);
        UsedForText->setObjectName(QString::fromUtf8("UsedForText"));

        verticalLayout_7->addWidget(UsedForText);


        horizontalLayout_2->addWidget(UsedForWidget);

        POWidget = new QWidget(ItemInfo1);
        POWidget->setObjectName(QString::fromUtf8("POWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(2);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(POWidget->sizePolicy().hasHeightForWidth());
        POWidget->setSizePolicy(sizePolicy3);
        verticalLayout_4 = new QVBoxLayout(POWidget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, -1, 0, 0);
        POText = new QLabel(POWidget);
        POText->setObjectName(QString::fromUtf8("POText"));

        verticalLayout_4->addWidget(POText);

        POComboBox = new QComboBox(POWidget);
        POComboBox->setObjectName(QString::fromUtf8("POComboBox"));

        verticalLayout_4->addWidget(POComboBox);


        horizontalLayout_2->addWidget(POWidget);


        verticalLayout_2->addWidget(ItemInfo1);

        ItemInfo2 = new QWidget(MainWidget);
        ItemInfo2->setObjectName(QString::fromUtf8("ItemInfo2"));
        sizePolicy1.setHeightForWidth(ItemInfo2->sizePolicy().hasHeightForWidth());
        ItemInfo2->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(ItemInfo2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, 0);
        ItemIdWidget = new QWidget(ItemInfo2);
        ItemIdWidget->setObjectName(QString::fromUtf8("ItemIdWidget"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(6);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(ItemIdWidget->sizePolicy().hasHeightForWidth());
        ItemIdWidget->setSizePolicy(sizePolicy4);
        verticalLayout_5 = new QVBoxLayout(ItemIdWidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        ItemIdLabel = new QLabel(ItemIdWidget);
        ItemIdLabel->setObjectName(QString::fromUtf8("ItemIdLabel"));

        verticalLayout_5->addWidget(ItemIdLabel);

        ItemIdText = new QLineEdit(ItemIdWidget);
        ItemIdText->setObjectName(QString::fromUtf8("ItemIdText"));

        verticalLayout_5->addWidget(ItemIdText);


        horizontalLayout->addWidget(ItemIdWidget);

        ItemIdWidget2 = new QWidget(ItemInfo2);
        ItemIdWidget2->setObjectName(QString::fromUtf8("ItemIdWidget2"));
        sizePolicy4.setHeightForWidth(ItemIdWidget2->sizePolicy().hasHeightForWidth());
        ItemIdWidget2->setSizePolicy(sizePolicy4);
        verticalLayout_9 = new QVBoxLayout(ItemIdWidget2);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        ItemIdLabel2 = new QLabel(ItemIdWidget2);
        ItemIdLabel2->setObjectName(QString::fromUtf8("ItemIdLabel2"));

        verticalLayout_9->addWidget(ItemIdLabel2);

        ItemIdComboBox = new QComboBox(ItemIdWidget2);
        ItemIdComboBox->setObjectName(QString::fromUtf8("ItemIdComboBox"));

        verticalLayout_9->addWidget(ItemIdComboBox);


        horizontalLayout->addWidget(ItemIdWidget2);

        ItemTypeWidget = new QWidget(ItemInfo2);
        ItemTypeWidget->setObjectName(QString::fromUtf8("ItemTypeWidget"));
        sizePolicy4.setHeightForWidth(ItemTypeWidget->sizePolicy().hasHeightForWidth());
        ItemTypeWidget->setSizePolicy(sizePolicy4);
        verticalLayout_6 = new QVBoxLayout(ItemTypeWidget);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        ItemTypeLabel = new QLabel(ItemTypeWidget);
        ItemTypeLabel->setObjectName(QString::fromUtf8("ItemTypeLabel"));

        verticalLayout_6->addWidget(ItemTypeLabel);

        ItemTypeText = new QLineEdit(ItemTypeWidget);
        ItemTypeText->setObjectName(QString::fromUtf8("ItemTypeText"));

        verticalLayout_6->addWidget(ItemTypeText);


        horizontalLayout->addWidget(ItemTypeWidget);

        QtyWidget = new QWidget(ItemInfo2);
        QtyWidget->setObjectName(QString::fromUtf8("QtyWidget"));
        QtyWidget->setEnabled(true);
        sizePolicy3.setHeightForWidth(QtyWidget->sizePolicy().hasHeightForWidth());
        QtyWidget->setSizePolicy(sizePolicy3);
        verticalLayout_3 = new QVBoxLayout(QtyWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        QtyLabel = new QLabel(QtyWidget);
        QtyLabel->setObjectName(QString::fromUtf8("QtyLabel"));

        verticalLayout_3->addWidget(QtyLabel);

        QtyText = new QLineEdit(QtyWidget);
        QtyText->setObjectName(QString::fromUtf8("QtyText"));

        verticalLayout_3->addWidget(QtyText);


        horizontalLayout->addWidget(QtyWidget);


        verticalLayout_2->addWidget(ItemInfo2);

        DescriptionWidget = new QWidget(MainWidget);
        DescriptionWidget->setObjectName(QString::fromUtf8("DescriptionWidget"));
        sizePolicy1.setHeightForWidth(DescriptionWidget->sizePolicy().hasHeightForWidth());
        DescriptionWidget->setSizePolicy(sizePolicy1);
        verticalLayout_8 = new QVBoxLayout(DescriptionWidget);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(-1, 0, -1, 0);
        DescLabel = new QLabel(DescriptionWidget);
        DescLabel->setObjectName(QString::fromUtf8("DescLabel"));

        verticalLayout_8->addWidget(DescLabel);

        DescText = new QLineEdit(DescriptionWidget);
        DescText->setObjectName(QString::fromUtf8("DescText"));

        verticalLayout_8->addWidget(DescText);


        verticalLayout_2->addWidget(DescriptionWidget);

        Spacer = new QWidget(MainWidget);
        Spacer->setObjectName(QString::fromUtf8("Spacer"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(6);
        sizePolicy5.setHeightForWidth(Spacer->sizePolicy().hasHeightForWidth());
        Spacer->setSizePolicy(sizePolicy5);
        Spacer->setMinimumSize(QSize(0, 0));

        verticalLayout_2->addWidget(Spacer);

        ConfirmDiscard = new QWidget(MainWidget);
        ConfirmDiscard->setObjectName(QString::fromUtf8("ConfirmDiscard"));
        horizontalLayout_3 = new QHBoxLayout(ConfirmDiscard);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        Confirm = new QPushButton(ConfirmDiscard);
        Confirm->setObjectName(QString::fromUtf8("Confirm"));

        horizontalLayout_3->addWidget(Confirm);

        Discard = new QPushButton(ConfirmDiscard);
        Discard->setObjectName(QString::fromUtf8("Discard"));
        Discard->setStyleSheet(QString::fromUtf8("background-color: #a4a4a4;\n"
"color: black;\n"
""));

        horizontalLayout_3->addWidget(Discard);


        verticalLayout_2->addWidget(ConfirmDiscard);


        verticalLayout->addWidget(MainWidget);


        retranslateUi(ReceiveItems);

        POComboBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(ReceiveItems);
    } // setupUi

    void retranslateUi(QDialog *ReceiveItems)
    {
        ReceiveItems->setWindowTitle(QCoreApplication::translate("ReceiveItems", "Dialog", nullptr));
        Title1->setText(QCoreApplication::translate("ReceiveItems", "Receiving Report #: [last rcr + 1]", nullptr));
        Title2->setText(QCoreApplication::translate("ReceiveItems", "Received by: [current user]", nullptr));
        UsedForLabel->setText(QCoreApplication::translate("ReceiveItems", "Used For", nullptr));
        UsedForText->setPlaceholderText(QCoreApplication::translate("ReceiveItems", "Eg. Volvo A20 ", nullptr));
        POText->setText(QCoreApplication::translate("ReceiveItems", "Associated PO#", nullptr));
        POComboBox->setPlaceholderText(QCoreApplication::translate("ReceiveItems", "None", nullptr));
        ItemIdLabel->setText(QCoreApplication::translate("ReceiveItems", "Item ID", nullptr));
        ItemIdText->setText(QString());
        ItemIdText->setPlaceholderText(QCoreApplication::translate("ReceiveItems", "Eg. F-2156", nullptr));
        ItemIdLabel2->setText(QCoreApplication::translate("ReceiveItems", "Item ID", nullptr));
        ItemTypeLabel->setText(QCoreApplication::translate("ReceiveItems", "Item Type", nullptr));
        ItemTypeText->setPlaceholderText(QCoreApplication::translate("ReceiveItems", "Eg. Filter", nullptr));
        QtyLabel->setText(QCoreApplication::translate("ReceiveItems", "Qty", nullptr));
        QtyText->setPlaceholderText(QCoreApplication::translate("ReceiveItems", "Eg. 14", nullptr));
        DescLabel->setText(QCoreApplication::translate("ReceiveItems", "Description", nullptr));
        DescText->setPlaceholderText(QCoreApplication::translate("ReceiveItems", "Eg. 5/8\" Thick Carbon Filter", nullptr));
        Confirm->setText(QCoreApplication::translate("ReceiveItems", "Confirm", nullptr));
        Discard->setText(QCoreApplication::translate("ReceiveItems", "Discard", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReceiveItems: public Ui_ReceiveItems {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECEIVEITEMS_H
