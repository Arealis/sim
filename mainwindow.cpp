//The flags for tables that SIM generated (they are unrelated to their named in the sqlite sim.db)
#define SIM_ITEMS 10
#define SIM_ITEM_DETAILS 11
#define SIM_ITEM_HISTORY 15
#define SIM_PROJECTS 20
#define SIM_PROJECT_DETAILS 21
#define SIM_PO 30
#define SIM_PO_DETAILS 31
#define SIM_SUPPLIERS 40
#define SIM_ADDRESSES 50
#define SIM_SHIPPERS 60
#define SIM_RCR 70
#define SIM_RCR_DETAILS 71
#define SIM_MRQ 80
#define SIM_MRQ_DETAILS 81

//The following do not function how flags should, there is too much complexity here.
//Refactor this code after the MVP is done
//The columns displayed in the SIM Items Table
#define SIM_ITEMS_ITEM_ID 0
#define SIM_ITEMS_PROJECT_ID 1
#define SIM_ITEMS_ITEM_NUM 2
#define SIM_ITEMS_ITEM_NAME 3
#define SIM_ITEMS_ITEM_TYPE 4
#define SIM_ITEMS_QTY 5
#define SIM_ITEMS_BIN 6
#define SIM_ITEMS_SHELF 7
#define SIM_ITEMS_PROJECT 8

//Columns in the SIM Item Details Table
#define SIM_ITEMD_PROJECT_ID 0
#define SIM_ITEMD_DATE 1
#define SIM_ITEMD_TRANS_TYPE 2
#define SIM_ITEMD_QTY 3
#define SIM_ITEMD_USED_FOR 4
#define SIM_ITEMD_MRQ 5
#define SIM_ITEMD_RCR 6
#define SIM_ITEMD_PO_NUM 7
#define SIM ITEMD_REQ_BY 8

//The columns displayed in the SIM Projects Table
#define SIM_PROJECTS_ID 0
#define SIM_PROJECTS_NAME 1

//The columns displsayed in the SIM Project Details Table

//The columns displayed in the SIM PO Table
#define SIM_PO_NUM 0
#define SIM_PO_CREATED 1
#define SIM_PO_SUPPLIER 2
#define SIM_PO_SHIP_TO 3
#define SIM_PO_SHIP_BY 4
#define SIM_PO_STATUS 5

//The columns displayed in the SIM PO Details Table
#define SIM_POD_ITEM_ID 0
#define SIM_POD_ITEM_NUM 1
#define SIM_POD_ITEM_NAME 2
#define SIM_POD_ITEM_TYPE 3
#define SIM_POD_QTY 4
#define SIM_POD_UNIT_PRICE 5
#define SIM_POD_TOTAL_PRICE 6

//RCR Table Column Flags
#define SIM_RCR_PROJECT_ID 0
#define SIM_RCR_NUM 1
#define SIM_RCR_DATE 2
#define SIM_RCR_PROJECT_NAME 3
#define SIM_RCR_ITEM_NUM 4
#define SIM_RCR_SUPPLIER 5
#define SIM_RCR_PO_NUM 6

//RCR Details Table Column Flags
#define SIM_RCRD_PROJECT_ID 0
#define SIM_RCRD_ITEM_ID 1
#define SIM_RCRD_SUPPLIER_ID 2
#define SIM_RCRD_ITEM_NUM 3
#define SIM_RCRD_ITEM_NAME 4
#define SIM_RCRD_QTY_RECD 5
#define SIM_RCRD_QTY_ORDERED 6
#define SIM_RCRD_PO_NUM 7
#define SIM_RCRD_PROJECT_NAME 8
#define SIM_RCRD_SUPPLIER_NAME 9

//MRQ Table Column Flags
#define SIM_MRQ_PROJECT_ID 0
#define SIM_MRQ_ITEM_ID 1
#define SIM_MRQ_SUPPLIER_ID 2
#define SIM_MRQ_NUM 3
#define SIM_MRQ_DATE 4
#define SIM_MRQ_ITEM_NUM 5
#define SIM_MRQ_ITEM_NAME 6
#define SIM_MRQ_SUPPLIER_NAME 7
#define SIM_MRQ_PROJECT_NAME 8
#define SIM_MRQ_REQUESTED_BY 9

//MRQ Details Table Column Flags
#define SIM_MRQD_PROJECT_ID 0
#define SIM_MRQD_ITEM_ID 1
#define SIM_MRQD_SUPPLIER_ID 2
#define SIM_MRQD_ITEM_NUM 3
#define SIM_MRQD_ITEM_NAME 4
#define SIM_MRQD_QTY 5
#define SIM_MRQD_SUPPLIER_NAME 6
#define SIM_MRQD_PROJECT_NAME 7

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "receiveitems.h"

#include <QFileDialog>

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTableView>
#include <QItemSelectionModel>
#include <QShortcut>
#include <QMouseEvent>

#include <QPrinter>
#include <QTextDocument>
#include <QPainter>
#include <QPrintDialog>
#include <QFile>
#include <QTextBrowser>
#include <QColor>

#include <QtDebug>
#include <QMessageBox>

/*  This cpp file (in few words) has a very simple order of operations.
 *  It's essentially a library
 *  It could described in a flow chart:
 *      1: Draw a table by
 *          (a.) Find out which table is currently selected
 *          (b.) Draw it
 *      2: Apply a search term by
 *          (a.) Find out which table is being searched for
 *          (b.) Find out what column you are trying to search by
 */


//Maybe global variables should not be set here, because you can never delete them
QSqlQueryModel *model = new QSqlQueryModel();
int dropdownIndex;
int currentTable;
QString searchfor;

/*When any of these queries are editted, you should check to make sure that the
#definitions for what tables and columns are are also changed.

HOWEVER, this is tied to the search box, so in order to have search functionality
(that is easy to update in the codebase) this needs to stay as is in memory.
Refactor later.*/
QString itemQry = "SELECT items.item_id, group_concat(projects.project_id), items.item_num AS 'Item ID' "
        ", items.item_name AS 'Description', items.item_type AS 'Category' "
        ", items.qty AS 'Quantity', items.bin_num AS 'Bin#', items.shelf_num AS 'Shelf' "
        ", group_concat(projects.project_name) AS 'Project (Used For)' "
        "FROM items "
        "LEFT JOIN linker ON items.item_id = linker.item_id "
        "LEFT JOIN projects ON linker.project_id = projects.project_id ";
QString itemDetailsQry = "SELECT projects.project_id, trans.date AS 'Date', trans.trans_type AS 'TransType' "
        ", trans.qty_change AS 'QtyChange', projects.project_name AS 'Used For' "
        ", trans.mrq_num AS 'MREQ#', trans.rcr_num AS 'RCR#' "
        ", trans.po_num AS 'PO#', trans.requested_by AS 'Req by' "
        "FROM items "
        "LEFT JOIN trans ON items.item_id = trans.item_id "
        "LEFT JOIN projects on projects.project_id = trans.used_for ";
QString projectQry = "SELECT projects.project_id, projects.project_name AS 'Project' FROM projects ";
QString projectDetailsQry = "SELECT trans.date AS 'Date', trans.trans_type AS 'Trans Type' "
        ", items.item_num AS 'Item ID', items.item_name AS 'Item Desc.' "
        ", trans.qty_change as 'QtyChange', trans.mrq_num as 'MRQ#', trans.rcr_num AS 'RCR#' "
        ", trans.po_num as 'PO#', trans.requested_by AS 'Requested by' "
        "FROM projects "
        "LEFT JOIN trans ON trans.used_for = projects.project_id "
        "LEFT JOIN items ON items.item_id = trans.item_id ";
QString poQry = "SELECT po.po_num AS 'PO#', po.date AS 'Created On', suppliers.supplier_name AS 'Supplier', "
        "addresses.thoroughfare AS 'Ship To', shippers.name AS 'Ship By', po.po_status as 'STATUS' "
        "FROM po "
        "LEFT JOIN suppliers ON po.supplier_id = suppliers.supplier_id "
        "LEFT JOIN addresses ON po.ship_to = addresses.id "
        "LEFT JOIN shippers ON po.ship_by = shippers.id ";
        //This is the PO (in general) table, for detailed POs there is another table
        /* 0 = received
         * 1 = some received
         * 2 = none received
         * 3 = draft */
QString poDetailsQry = "SELECT items.item_id, items.item_num AS 'Item Name', items.item_name AS 'Description'"
        ",items.item_type AS 'Category', linker.item_qty as 'Quantity', linker.item_price AS 'Unit Price'"
        ",(linker.item_price * linker.item_qty) as 'Total' "
        "FROM po "
        "LEFT JOIN suppliers ON po.supplier_id = suppliers.supplier_id "
        "LEFT JOIN linker ON po.po_num = linker.po_num "
        "LEFT JOIN items ON items.item_id = linker.item_id "
        "LEFT JOIN addresses ON po.ship_to = addresses.id "
        "LEFT JOIN shippers ON po.ship_by = shippers.id ";
    //Include a column that records the user who received the items
QString rcrQry = "SELECT group_concat(DISTINCT trans.used_for), trans.rcr_num AS 'Receiving Report #', trans.date AS 'Date' "
        ", group_concat(DISTINCT projects.project_name) AS 'Used For', group_concat(DISTINCT items.item_num) AS 'Received' "
        ",group_concat(DISTINCT suppliers.supplier_name) AS 'Suppliers' , group_concat(DISTINCT trans.po_num) AS 'PO#' "
        "FROM trans "
        "LEFT JOIN projects ON projects.project_id = trans.used_for "
        "LEFT JOIN items ON items.item_id = trans.item_id "
        "LEFT JOIN po ON trans.po_num = po.po_num "
        "LEFT JOIN suppliers on po.supplier_id = suppliers.supplier_id ";
    //Include a math column that calculates how many have been received total, and from which POs, and total ordered from the PO
QString rcrDetailsQry = "SELECT trans.used_for, trans.item_id, po.supplier_id "
        ",items.item_num AS 'Item ID', items.item_name AS 'Description' "
        ",trans.qty_change AS 'Qty Recd.' "
        ",trans.po_num AS 'PO#', projects.project_name AS 'Used For' "
        ",suppliers.supplier_name AS 'Supplier' "
        "FROM trans "
        "LEFT JOIN po ON trans.po_num = po.po_num "
        "LEFT JOIN suppliers ON po.supplier_id = suppliers.supplier_id "
        "LEFT JOIN items ON items.item_id = trans.item_id "
        "LEFT JOIN projects ON projects.project_id = trans.used_for ";
//Include a table to request a supplier here
QString reqQry = "SELECT group_concat(DISTINCT trans.used_for), group_concat(DISTINCT trans.item_id), trans.mrq_num AS 'MRQ#' "
        ", group_concat(DISTINCT trans.date) AS 'Date' ,group_concat(DISTINCT items.item_num) AS 'Item ID' "
        ", group_concat(DISTINCT items.item_name) AS 'Description' "
        //",suppliers.supplier_name AS 'Supplier' "
        ",group_concat(DISTINCT projects.project_name) AS 'Used For', trans.requested_by AS 'Requested By' "
        "FROM trans "
        "LEFT JOIN items on items.item_id = trans.item_id "
        //"LEFT JOIN suppliers ON suppliers.supplier_id = linker.supplier_id "
        "LEFT JOIN projects ON projects.project_id = trans.used_for ";
QString reqDetailsQry = "SELECT trans.used_for, trans.item_id "
        ", items.item_num AS 'Item ID', items.item_name AS 'Description', projects.project_name AS 'Used For' "
        //supplier
        "FROM trans "
        "LEFT JOIN items ON items.item_id = trans.item_id "
        "LEFT JOIN projects ON projects.project_id = trans.used_for ";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::on_actionItems_Table_Default_triggered();
    ui->ItemTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ItemTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_H),this,SLOT(goToHome()));
    new QShortcut(Qt::BackButton,this,SLOT(goToHome()));
    new QShortcut(QKeySequence::StandardKey(Qt::BackButton),this,SLOT(goToHome()));
    ui->ItemTable->setContextMenuPolicy(Qt::CustomContextMenu);
}
/*  The system is still vulnerable to SQL injections:
 *
 *  The following is a valid SQL injection
 *  %' UNION ALL SELECT supplier_id, supplier_name, address, e_mail, phone, phone2, fax, contact, email FROM suppliers--
 */

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::on_actionImport_From_Existing_Database_triggered()
{
    system("converter-manual\\converter-manual.exe");
}

void MainWindow::on_actionSelect_Database_Location_triggered()
{
    //This currently does not work
    QString dbLoc = QFileDialog::getOpenFileName(this,"Select Folder For New Database (WIP)", NULL);
    QMessageBox::information(this, "Stock Title 43", dbLoc);
}

void MainWindow::on_SearchForDropdown_currentIndexChanged(int index)
{
    dropdownIndex = index;
    MainWindow::on_SearchBox_textChanged(searchfor);
}

void MainWindow::goToHome()
{
    switch(currentTable){
    case SIM_ITEM_DETAILS:{
        MainWindow::on_actionItems_Table_Default_triggered();
        break;
    }
    case SIM_PROJECT_DETAILS: {
        MainWindow::on_actionProjects_triggered();
        break;
    }
    case SIM_PO_DETAILS: {
        MainWindow::on_actionPurchase_Orders_triggered();
        break;
    }
    }
}

void MainWindow::on_actionItems_Table_Default_triggered()
{
    currentTable = SIM_ITEMS;
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(itemQry + "GROUP BY items.item_id;");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();
    ui->ItemTable->setModel(model);
    ui->ItemTableLabel->setText("Items Table");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "Item ID");
    ui->SearchForDropdown->addItem("Desription");
    ui->SearchForDropdown->addItem("Category");
    ui->SearchForDropdown->addItem("Project");
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_ITEMS_ITEM_ID, true);
    ui->ItemTable->setColumnHidden(SIM_ITEMS_PROJECT_ID, true);

    /*      WIDTH SIZING will come later
    double i = (ui->ItemTable->width()-8) / 16;
    ui->ItemTable->setColumnWidth(SIM_ITEMS_ITEM_NUM, 2*i);
    ui->ItemTable->setColumnWidth(SIM_ITEMS_ITEM_NAME, 4*i);
    ui->ItemTable->setColumnWidth(SIM_ITEMS_ITEM_TYPE, 2*i);
    ui->ItemTable->setColumnWidth(SIM_ITEMS_QTY, 1*i);
    ui->ItemTable->setColumnWidth(SIM_ITEMS_BIN, 2*i);
    ui->ItemTable->setColumnWidth(SIM_ITEMS_SHELF, 2*i);
    ui->ItemTable->setColumnWidth(SIM_ITEMS_PROJECT, 3*i);
    */
}

void MainWindow::createItem_Details_Table(QString itemid, QString itemname, QString itemdesc)
{
    currentTable = SIM_ITEM_DETAILS;
    ui->ItemTableLabel->setText("Movement History For: " + itemname + " || " + itemdesc);
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(itemDetailsQry + "WHERE items.item_id = '" + itemid + "';");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_ITEMD_PROJECT_ID, true);

}

void MainWindow::on_actionItem_History_triggered()
{
    currentTable = SIM_ITEM_HISTORY;
    ui->ItemTableLabel->setText("Inventory Movement History");
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(itemDetailsQry + ";");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_ITEMD_PROJECT_ID, true);
}

void MainWindow::on_actionPurchase_Orders_triggered()
{
    currentTable = SIM_PO;

    QSqlDatabase::database().open();
    QSqlQuery query;
    query.exec(poQry + "UNION SELECT 'DRAFT', date, supplier_id, null, null, '3'"
"FROM drafts WHERE type = '1' ORDER BY po_num DESC" + ";");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    QSqlDatabase::database().close();
    ui->ItemTable->update();

    ui->ItemTableLabel->setText("Purchase Orders");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "PO#");
    ui->SearchForDropdown->addItem("Date");
    ui->SearchForDropdown->addItem("Supplier");
    ui->SearchForDropdown->addItem("Ship By");
    MainWindow::showAllColumns();
    /*  SIZING comes later
    float i = (ui->ItemTable->width()-50) / 14;
    ui->ItemTable->setColumnWidth(SIM_PO_NUM, 2*i);
    ui->ItemTable->setColumnWidth(SIM_PO_CREATED, 3*i);
    ui->ItemTable->setColumnWidth(SIM_PO_SUPPLIER, 4*i);
    ui->ItemTable->setColumnWidth(SIM_PO_SHIP_TO, 2*i);
    ui->ItemTable->setColumnWidth(SIM_PO_SHIP_BY, 2*i);
    ui->ItemTable->setColumnWidth(SIM_PO_STATUS, 1*i);
    */
    MainWindow::colorRows();

}

void MainWindow::createPO_Details_Table(QString ponum, QString date, QString supplier)
{
    currentTable = SIM_PO_DETAILS;
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(poDetailsQry + "WHERE po.po_num LIKE '" + ponum + "';");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    ui->ItemTableLabel->setText("Purchase Order #: " + ponum + "  |  Ordered From: " + supplier + " on " + date);
    //Grand totals are not compatable with QSqlQueryModel, because that model is read only.
    //To implement grand totals, you have to switch to something like QTableModel
    ui->ItemTable->update();
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "Item");
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_POD_ITEM_ID, true);
    /* SIZING COMES LATER
    double i = ui->ItemTable->width() / 21;
    ui->ItemTable->setColumnWidth(SIM_POD_ITEM_NUM,3*i);
    ui->ItemTable->setColumnWidth(SIM_POD_ITEM_NAME,7*i);
    ui->ItemTable->setColumnWidth(SIM_POD_ITEM_TYPE,3*i);
    ui->ItemTable->setColumnWidth(SIM_POD_QTY,2*i);
    ui->ItemTable->setColumnWidth(SIM_POD_UNIT_PRICE,3*i);
    ui->ItemTable->setColumnWidth(SIM_POD_TOTAL_PRICE,3*i);
    */
}

void MainWindow::on_actionProjects_triggered()
{
    currentTable = SIM_PROJECTS;
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(projectQry + ";");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    ui->ItemTable->update();
    ui->ItemTableLabel->setText("Projects");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "ProjectName");
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_PROJECTS_ID, true);
}

void MainWindow::createProject_Details_Table(QString projectid, QString projectname)
{
    currentTable = SIM_PROJECT_DETAILS;
    QSqlDatabase qdb = QSqlDatabase::database();
    ui->ItemTableLabel->setText(projectname + " Usage History");
    QSqlQuery query;
    query.exec(projectDetailsQry + "WHERE projects.project_id = '" + projectid + "';");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();
    ui->ItemTable->update();
    MainWindow::showAllColumns();
}

void MainWindow::on_actionRequested_Items_triggered()
{
    currentTable = SIM_MRQ;
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(reqQry + "WHERE trans.mrq_num IS NOT NULL GROUP BY trans.mrq_num ORDER BY trans.mrq_num;");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    model->insertColumn(SIM_MRQ_SUPPLIER_ID);
    model->insertColumn(SIM_MRQ_SUPPLIER_NAME);
    ui->ItemTable->update();
    ui->ItemTableLabel->setText("Requested Items");
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_MRQ_PROJECT_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRQ_ITEM_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRQ_SUPPLIER_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRQ_SUPPLIER_NAME, true);
}

void MainWindow::createRequested_Details_Table(QString mrq, QString date, QString reqby)
{
    currentTable = SIM_MRQ_DETAILS;
    QSqlQuery query;
    QSqlDatabase qdb = QSqlDatabase::database();
    query.exec(reqDetailsQry + "WHERE trans.mrq_num = '" + mrq + "';");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    model->insertColumn(SIM_MRQD_SUPPLIER_ID);
    model->insertColumn(SIM_MRQD_QTY);
    model->insertColumn(SIM_MRQD_SUPPLIER_NAME);
    ui->ItemTable->update();
    ui->ItemTableLabel->setText("Requisition #: " + mrq + " || " + date + " || Requested by: " + reqby);
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_MRQD_PROJECT_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRQD_ITEM_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRQD_SUPPLIER_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRQD_SUPPLIER_NAME, true);
}

void MainWindow::on_actionReceived_triggered()
{
    currentTable = SIM_RCR;
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(rcrQry + "WHERE trans.rcr_num IS NOT NULL GROUP BY trans.rcr_num ORDER BY trans.rcr_num;");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    ui->ItemTable->update();
    ui->ItemTableLabel->setText("Received Items");
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_RCR_PROJECT_ID,true);
    MainWindow::clearDropdown();
}

void MainWindow::createReceived_Details_Table(QString rcr, QString date)
{
    currentTable = SIM_RCR_DETAILS;
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    query.exec(rcrDetailsQry + "WHERE trans.rcr_num = '" + rcr + "';");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    //model->insertColumn(SIM_RCRD_QTY_RECD);
    ui->ItemTable->update();
    ui->ItemTableLabel->setText("Receiving Report #" + rcr + " || " + date);
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_RCRD_ITEM_ID, true);
    ui->ItemTable->setColumnHidden(SIM_RCRD_PROJECT_ID, true);
    ui->ItemTable->setColumnHidden(SIM_RCRD_SUPPLIER_ID, true);

}

void MainWindow::on_ItemTable_doubleClicked(const QModelIndex &index)
{
    //First, identify table. Second, identify row. Third, switch table.
    switch (currentTable) {
    case SIM_ITEMS: {
        switch (index.column()) {
        case SIM_ITEMS_ITEM_NUM:
        case SIM_ITEMS_ITEM_NAME:
        case SIM_ITEMS_QTY: {
            //Go through the history of this particular item
            QString itemid = index.siblingAtColumn(SIM_ITEMS_ITEM_ID).data().toString();
            QString itemnum = index.siblingAtColumn(SIM_ITEMS_ITEM_NUM).data().toString();
            QString itemname = index.siblingAtColumn(SIM_ITEMS_ITEM_NAME).data().toString();
            createItem_Details_Table(itemid,itemnum,itemname);
            break;
        }
        case SIM_ITEMS_ITEM_TYPE: {
            //Insert a search for all items of this type
            QString item_type = index.data().toString();
            ui->SearchBox->setText(item_type);
            ui->SearchForDropdown->setCurrentIndex(2);
            break;
        }
        case SIM_ITEMS_BIN:
        case SIM_ITEMS_SHELF: {
            //allow these to be editted
            break;
        }
        case SIM_ITEMS_PROJECT: {
            //bring up this particular project's history.
            QString projectid = index.siblingAtColumn(SIM_ITEMS_PROJECT_ID).data().toString();
            QString projectname = index.siblingAtColumn(SIM_ITEMS_PROJECT).data().toString();
            MainWindow::createProject_Details_Table(projectid,projectname);
            break;
        }
        }
        break;
    }
    case SIM_ITEM_DETAILS: {
        switch (index.column()) {
        case SIM_ITEMD_PO_NUM: {
            QString ponum = index.data().toString();
            QSqlQuery q;
            q.exec("SELECT po.date, suppliers.supplier_name FROM po "
                "LEFT JOIN suppliers ON suppliers.supplier_id = po.supplier_id "
                "WHERE po.po_num = '" + ponum + "';");
            q.next();
            QString date = q.value(0).toString();
            QString supplier = q.value(1).toString();
            MainWindow::createPO_Details_Table(ponum,date,supplier);
            break;
        }
        case SIM_ITEMD_USED_FOR: {
            QString projectid = index.siblingAtColumn(SIM_ITEMD_PROJECT_ID).data().toString();
            QString projectname = index.siblingAtColumn(SIM_ITEMD_USED_FOR).data().toString();
            MainWindow::createProject_Details_Table(projectid, projectname);
            break;
        }
        }
        break;
    }
    case SIM_PROJECTS: {
        if (index.siblingAtColumn(SIM_PO_STATUS).data().toInt() == 3)
        {
            break;
        } else {
            QString projectid = index.siblingAtColumn(SIM_PROJECTS_ID).data().toString();
            QString projectname = index.siblingAtColumn(SIM_PROJECTS_NAME).data().toString();
            MainWindow::createProject_Details_Table(projectid,projectname);
            break;
        }
    }
    case SIM_PO: {
        switch(index.column()) {
        //PO, created, supplier, ship to, ship by, status
        case SIM_PO_NUM:
        case SIM_PO_CREATED:
        case SIM_PO_SUPPLIER:
        case SIM_PO_SHIP_TO:
        case SIM_PO_SHIP_BY:
        case SIM_PO_STATUS:
        {
            QString ponum = index.siblingAtColumn(0).data().toString();
            QString date = index.siblingAtColumn(1).data().toString();
            QString supplier = index.siblingAtColumn(2).data().toString();
            MainWindow::createPO_Details_Table(ponum, date, supplier);
            break;
        }
        }
        break;
    }
    case SIM_PO_DETAILS: {
        switch(index.column()) {
        case SIM_POD_ITEM_ID:
        case SIM_POD_ITEM_NUM:
        case SIM_POD_ITEM_NAME:
        {
            QString itemid = index.siblingAtColumn(SIM_POD_ITEM_ID).data().toString();
            QString itemnum = index.siblingAtColumn(SIM_POD_ITEM_NUM).data().toString();
            QString itemname = index.siblingAtColumn(SIM_POD_ITEM_NAME).data().toString();
            MainWindow::createItem_Details_Table(itemid,itemnum,itemname);
            break;
        }
        case SIM_POD_ITEM_TYPE:
        case SIM_POD_QTY:
        case SIM_POD_UNIT_PRICE:
        case SIM_POD_TOTAL_PRICE:
        {
            break;
        }
        }
        break;
    }
    case SIM_RCR: {
        //Make more detailed
        QString rcrnum = index.siblingAtColumn(SIM_RCR_NUM).data().toString();
        QString date = index.siblingAtColumn(SIM_RCR_DATE).data().toString();
        MainWindow::createReceived_Details_Table(rcrnum, date);
        break;
    }
    case SIM_MRQ:
    {
        QString mrqnum = index.siblingAtColumn(SIM_MRQ_NUM).data().toString();
        QString date = index.siblingAtColumn(SIM_MRQ_DATE).data().toString();
        QString reqby= index.siblingAtColumn(SIM_MRQ_REQUESTED_BY).data().toString();
        MainWindow::createRequested_Details_Table(mrqnum, date, reqby);
    }


    }
}

void MainWindow::on_SearchBox_textChanged(const QString &arg1)
{
    searchfor = arg1;
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery query;
    QString searchbyString;
    QString qry;

    switch (currentTable) {
    case SIM_ITEMS: {
        qry = itemQry;
        switch (dropdownIndex) {
        case 0: {
            searchbyString = "items.item_num";
            break;
        }
        case 1: {
            searchbyString = "items.item_name";
            break;
        }
        case 2: {
            searchbyString = "items.item_type";
            break;
        }
        case 3: {
            searchbyString = "projects.project_name";
            break;
        }
        }
        qdb.open();
        query.exec(qry + "WHERE " + searchbyString + " LIKE '%" + arg1 + "%' GROUP BY items.item_id;");
        model->setQuery(query);
        while (model->canFetchMore()) {model->fetchMore();}
        qdb.close();
        ui->ItemTable->update();
        break;
    }
    case SIM_PO: {
        qry = poQry;
        switch (dropdownIndex) {
        case 0: {
            searchbyString = "po.po_num";
            break;
        }
        case 1: {
            searchbyString = "po.date";
            break;
        }
        case 2: {
            searchbyString = "suppliers.supplier_name";
            break;
        }
        case 3: {
            searchbyString = "shippers.name";
            break;
        }
        }
        qdb.open();
        query.exec(qry + "WHERE " + searchbyString + " LIKE '%" + arg1 + "%' UNION SELECT 'DRAFT', date, supplier_id, null, null, '3'"
"FROM drafts WHERE type = '1' ORDER BY po_num DESC;");
        model->setQuery(query);
        while (model->canFetchMore()) {model->fetchMore();}
        qdb.close();
        ui->ItemTable->setModel(model);
        break;
    }
    case SIM_PROJECTS: {
        qry = projectQry;
        searchbyString = "projects.project_name";
        qdb.open();
        query.exec(qry + "WHERE " + searchbyString + " LIKE '%" + arg1 + "%';");
        model->setQuery(query);
        while (model->canFetchMore()) {model->fetchMore();}
        qdb.close();
        ui->ItemTable->setModel(model);
        break;
    }
    case SIM_ITEM_DETAILS:
    case SIM_PO_DETAILS:
    case SIM_PROJECT_DETAILS:
    {
        break;
    }
    }
}

void MainWindow::on_ClearSearchButton_clicked()
{
    ui->SearchBox->setText("");
}

void MainWindow::showAllColumns()
{
    ui->ItemTable->setColumnHidden(0, false);
    ui->ItemTable->setColumnHidden(1, false);
    ui->ItemTable->setColumnHidden(2, false);
    ui->ItemTable->setColumnHidden(3, false);
    ui->ItemTable->setColumnHidden(4, false);
    ui->ItemTable->setColumnHidden(5, false);
    ui->ItemTable->setColumnHidden(6, false);
    ui->ItemTable->setColumnHidden(7, false);
    ui->ItemTable->setColumnHidden(8, false);
    ui->ItemTable->setColumnHidden(9, false);
}

void MainWindow::clearDropdown() {
    ui->SearchForDropdown->removeItem(3);
    ui->SearchForDropdown->removeItem(2);
    ui->SearchForDropdown->removeItem(1);
}

//This currently has a 256 item limit (because the sql query loads information in 256 row increments
void MainWindow::on_actionPrint_Current_Table_triggered()
{   //This writes an html table with all of the information in it, and then prints that. Because it's easier.
    //Thanks windows.

    const QString htmlFilepath = QString("%1/html/%2").arg(qApp->applicationDirPath(),"output.html");
    QFile htmlTable(htmlFilepath);

    if (!htmlTable.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Can't create file at " << htmlFilepath;
    }

    //QString htmlStr;
    QTextStream out(&htmlTable);

    const int col = model->columnCount();
    const int row = model->rowCount();

    qDebug() << col << ", " << row;

    //Setup Table
    out << "<html>\n"
        "<head>\n"
        "<meta content =\"Text/html; charset=Windows-1251\">\n"
        << QString("<title>%1</title>\n").arg("Current Table is: " + ui->ItemTableLabel->text())
        << "</head>\n"
        "<body bgcolor=#ffffff link=#5000A0>\n"
        "<table border=1 cellspacing=0 cellpadding=2>\n";

    //Headers
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int c = 0; c < col; c++)
    {
        if (!ui->ItemTable->isColumnHidden(c))
        {
            out << QString("<th>%1</th>").arg(model->headerData(c, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    //Data
    for (int r = 0; r < row; r++)
    {
        out << "<tr>";
        for (int c = 0; c < col; c++)
        {
            if(!ui->ItemTable->isColumnHidden(c))
            {
                QString data = model->index(r,c).data().toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg(!data.isEmpty() ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out << "</table>\n"
        "</body>\n"
        "</html>\n";

    htmlTable.close();

    /* This generates a solid HTML table right now, but that's it.
     * The print functionality still does not work. This project cannot be considered
     * functional until it can print.
     *
     * This should probably be the last issue tackles before release:
     *
     *  QTextBrowser *textBrowser = new QTextBrowser;
     *  QTextBrowser *editor = static_cast<QTextBrowser* >(textBrowser);
     *  QPrinter printer;
     *
     *  QPrintDialog *dialog = new QPrintDialog(&printer, this);
     *  dialog->setWindowTitle(tr("Print Document"));
     *  if (editor->textCursor().hasSelection())
     *      dialog->setOption(QAbstractPrintDialog::PrintSelection);
     *  if (dialog->exec() != QDialog::Accepted)
     *      return;
     *
     *  editor->print(&printer);
     */
}

void MainWindow::on_actionReceive_Items_triggered()
{
    ReceiveItems a;
    a.setWindowTitle("Receive Items");;
    a.exec();
}

void MainWindow::on_ItemTable_customContextMenuRequested(const QPoint &pos)
{
    switch (currentTable) {
    case SIM_ITEMS:
    {
        //Currently leaking memory. Refactor.
        QMenu *m = new QMenu();
        QAction *addToPO = new QAction();
        QAction *addToPR = new QAction();
        QAction *addToDist = new QAction();
        addToPO->setText("Add to PO for this item's (most recent) Supplier"); //A purchase order can only have one supplier
        addToPR->setText("Add to most recent Purchase Request form");
        addToDist->setText("Add to most recent Distribution form");
        connect(addToPO, SIGNAL(triggered()),
                this, SLOT(addItemToPO()));
        connect(addToPR, SIGNAL(triggered()),
                this, SLOT(addItemToPR()));
        connect(addToDist, SIGNAL(triggered()),
                this, SLOT(addItemToDist()));
        m->addAction(addToPO);
        m->addAction(addToPR);
        m->addAction(addToDist);

        m->popup(ui->ItemTable->viewport()->mapToGlobal(pos));
        break;
    }
    }

}

//The following three options are similar enough to be consolidated,
//or perhaps they are so simple that they need to be expanded upon. Refactor.
void MainWindow::addItemToPO()
{
    QSqlDatabase::database().open();
    QSqlQuery q;
    q.exec(QString("INSERT INTO drafts (type, date, item_id) VALUES (1, datetime('now'), %1);")
           .arg(ui->ItemTable->currentIndex().siblingAtColumn(SIM_ITEMS_ITEM_ID).data().toString()));
    QSqlDatabase::database().close();
}
void MainWindow::addItemToPR()
{
    QSqlDatabase::database().open();
    QSqlQuery q;
    q.exec(QString("INSERT INTO drafts (type, date, item_id) VALUES (2, datetime('now'), %1);")
           .arg(ui->ItemTable->currentIndex().siblingAtColumn(SIM_ITEMS_ITEM_ID).data().toString()));
    QSqlDatabase::database().close();
}
void MainWindow::addItemToDist()
{
    QSqlDatabase::database().open();
    QSqlQuery q;
    q.exec(QString("INSERT INTO drafts (type, date, item_id) VALUES (0, datetime('now'), %1);")
           .arg(ui->ItemTable->currentIndex().siblingAtColumn(SIM_ITEMS_ITEM_ID).data().toString()));
    QSqlDatabase::database().close();
}

void MainWindow::on_actionPing_Databse_triggered()
{
    qDebug() << QSqlDatabase::database().isOpen();
}

void MainWindow::colorRows()
{
    /*  This is hell without the Proxy Table Model. Forget it for now.
     *  That is a task for the refactor rounds.
     *  But the color values (for when it is that time) can be seen here:
     *      #AA4471 - Dark Magenta   | Red
     *      #BB6088 - Light Magenta  |
     *      #DDCC77 - Dark Yellow    | Yellow
     *      #E8D98F - Light Yellow   |
     *      #C7C7C7 - Dark Grey      | Grey
     *      #D8D8D8 - Light Grey     |
     *      #88CCEE - Dark Cyan      | Blue
     *      #A1D5Ef - Light Cyan     |
     *      #509287 - Dark Turquoise | Green
     *      #65A096 - Light Turqoise |
     *  Try to avoid mixing red and green... although they should be slightly different
     *  shades selected to minimize the impact on the colorblind, they are still very similar.
     *  It's worth avoiding if at all possible.
     */
}
