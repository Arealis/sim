/********************************************************************************
** Form generated from reading UI file 'additem.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDITEM_H
#define UI_ADDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_additem
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *Info;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *additem)
    {
        if (additem->objectName().isEmpty())
            additem->setObjectName(QString::fromUtf8("additem"));
        additem->resize(400, 300);
        verticalLayout = new QVBoxLayout(additem);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        Info = new QWidget(additem);
        Info->setObjectName(QString::fromUtf8("Info"));
        verticalLayout_2 = new QVBoxLayout(Info);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lineEdit = new QLineEdit(Info);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout_2->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(Info);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        verticalLayout_2->addWidget(lineEdit_2);


        verticalLayout->addWidget(Info);

        buttonBox = new QDialogButtonBox(additem);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(additem);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, additem, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, additem, &QDialog::reject);

        QMetaObject::connectSlotsByName(additem);
    } // setupUi

    void retranslateUi(QDialog *additem)
    {
        additem->setWindowTitle(QCoreApplication::translate("additem", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class additem: public Ui_additem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDITEM_H
