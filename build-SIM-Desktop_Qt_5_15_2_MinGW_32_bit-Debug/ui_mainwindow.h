/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionImport_From_Existing_Database;
    QAction *actionItems_Table_Default;
    QAction *actionBackup_Database;
    QAction *actionPurchase_Orders;
    QAction *actionGenerate_Report_For_Current_Table;
    QAction *actionSelect_Database_Location;
    QWidget *centralwidget;
    QTableView *ItemTable;
    QMenuBar *menubar;
    QMenu *menuSimple_Inventory_Manager;
    QMenu *menuTables;
    QMenu *menuReports;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1366, 768);
        MainWindow->setMinimumSize(QSize(50, 10));
        MainWindow->setBaseSize(QSize(1366, 786));
        MainWindow->setWindowOpacity(1.000000000000000);
        actionImport_From_Existing_Database = new QAction(MainWindow);
        actionImport_From_Existing_Database->setObjectName(QString::fromUtf8("actionImport_From_Existing_Database"));
        actionItems_Table_Default = new QAction(MainWindow);
        actionItems_Table_Default->setObjectName(QString::fromUtf8("actionItems_Table_Default"));
        actionBackup_Database = new QAction(MainWindow);
        actionBackup_Database->setObjectName(QString::fromUtf8("actionBackup_Database"));
        actionPurchase_Orders = new QAction(MainWindow);
        actionPurchase_Orders->setObjectName(QString::fromUtf8("actionPurchase_Orders"));
        actionGenerate_Report_For_Current_Table = new QAction(MainWindow);
        actionGenerate_Report_For_Current_Table->setObjectName(QString::fromUtf8("actionGenerate_Report_For_Current_Table"));
        actionSelect_Database_Location = new QAction(MainWindow);
        actionSelect_Database_Location->setObjectName(QString::fromUtf8("actionSelect_Database_Location"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        ItemTable = new QTableView(centralwidget);
        ItemTable->setObjectName(QString::fromUtf8("ItemTable"));
        ItemTable->setGeometry(QRect(20, 15, 1280, 768));
        ItemTable->setMinimumSize(QSize(1280, 768));
        ItemTable->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        ItemTable->setDefaultDropAction(Qt::IgnoreAction);
        ItemTable->setAlternatingRowColors(true);
        ItemTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
        ItemTable->setSortingEnabled(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1366, 21));
        menuSimple_Inventory_Manager = new QMenu(menubar);
        menuSimple_Inventory_Manager->setObjectName(QString::fromUtf8("menuSimple_Inventory_Manager"));
        menuTables = new QMenu(menubar);
        menuTables->setObjectName(QString::fromUtf8("menuTables"));
        menuReports = new QMenu(menubar);
        menuReports->setObjectName(QString::fromUtf8("menuReports"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSimple_Inventory_Manager->menuAction());
        menubar->addAction(menuTables->menuAction());
        menubar->addAction(menuReports->menuAction());
        menuSimple_Inventory_Manager->addAction(actionImport_From_Existing_Database);
        menuSimple_Inventory_Manager->addAction(actionBackup_Database);
        menuSimple_Inventory_Manager->addAction(actionGenerate_Report_For_Current_Table);
        menuSimple_Inventory_Manager->addAction(actionSelect_Database_Location);
        menuTables->addAction(actionItems_Table_Default);
        menuTables->addAction(actionPurchase_Orders);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Simple Inventory Manager", nullptr));
        actionImport_From_Existing_Database->setText(QCoreApplication::translate("MainWindow", "Import From Existing Database", nullptr));
        actionItems_Table_Default->setText(QCoreApplication::translate("MainWindow", "Items (Default)", nullptr));
        actionBackup_Database->setText(QCoreApplication::translate("MainWindow", "Backup Database", nullptr));
        actionPurchase_Orders->setText(QCoreApplication::translate("MainWindow", "Purchase Orders", nullptr));
        actionGenerate_Report_For_Current_Table->setText(QCoreApplication::translate("MainWindow", "Generate Report For Current Table", nullptr));
        actionSelect_Database_Location->setText(QCoreApplication::translate("MainWindow", "Select Database Location", nullptr));
#if QT_CONFIG(tooltip)
        actionSelect_Database_Location->setToolTip(QCoreApplication::translate("MainWindow", "Simple Inventory Manager can read from any sim.db file. If you would like to change which database file SIM uses, select this.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        actionSelect_Database_Location->setWhatsThis(QCoreApplication::translate("MainWindow", "This will allow you to change which sim.db database that Simple Inventory Manager reads from.", nullptr));
#endif // QT_CONFIG(whatsthis)
        menuSimple_Inventory_Manager->setTitle(QCoreApplication::translate("MainWindow", "Simple Inventory Manager", nullptr));
        menuTables->setTitle(QCoreApplication::translate("MainWindow", "Tables", nullptr));
        menuReports->setTitle(QCoreApplication::translate("MainWindow", "Reports", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
