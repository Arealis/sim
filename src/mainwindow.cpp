/*
    SIM - the Simple Inventory Manager
    Copyright (C) 2021 Arealis

    This file is part of Simple Inventory Manager.

    Simple Inventory Manager is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Simple Inventory Manager is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Simple Inventory Manager.  If not, see <https://www.gnu.org/licenses/>.
*/

//Login Flags (Creating new users, editing userdata, etc.)
#define SIM_LOGIN 0
#define SIM_NEW 1
#define SIM_EDIT 2

//The flags for tables that SIM generated (they are unrelated to their named in the sqlite sim.db)
#define SIM_ITEMS 10
#define SIM_ITEM_DETAILS 11
#define SIM_INVENTORY_HISTORY 15
#define SIM_PROJECTS 20
#define SIM_PROJECT_DETAILS 21
#define SIM_SUPPLIERS 30
#define SIM_SUPPLIERS_DETAILS 31
#define SIM_PO 40
#define SIM_POD 41
#define SIM_PR 50
#define SIM_PRD 51
#define SIM_ADDRESSES 60
#define SIM_SHIPPERS 70
#define SIM_RR 80
#define SIM_RRD 91
#define SIM_MR 100
#define SIM_MRD 101

//The following do not function how flags should, there is too much complexity here.
//Refactor this code after the MVP is done
//The columns displayed in the SIM Items Table
#define SIM_ITEMS_ITEM_ID 0
#define SIM_ITEMS_ITEM_NUM 1
#define SIM_ITEMS_ITEM_DESC 2
#define SIM_ITEMS_ITEM_CAT 3
#define SIM_ITEMS_QTY 4
#define SIM_ITEMS_UNIT 5
#define SIM_ITEMS_SHELF 6
#define SIM_ITEMS_BIN 7
#define SIM_ITEMS_PROJECT 8

//Columns in the SIM Item Details Table
#define SIM_ITEMD_SUPPLIER_ID 0
#define SIM_ITEMD_DOCTYPE_ID 1
#define SIM_ITEMD_CREATOR_ID 2
#define SIM_ITEMD_DATE 3
#define SIM_ITEMD_DOCTYPE 4
#define SIM_ITEMD_DOCNUM 5
#define SIM_ITEMD_CREATOR 6
#define SIM_ITEMD_UNIT 7
#define SIM_ITEMD_REQD 8
#define SIM_ITEMD_ORDERED 9
#define SIM_ITEMD_SUPPLIER 10
#define SIM_ITEMD_PROJECT 11
#define SIM_ITEMD_RECD 12
#define SIM_ITEMD_DIST 13

//The columns displayed in the SIM Projects Table
#define SIM_PROJECTS_NAME 0
#define SIM_PROJECTS_DESC 1
#define SIM_PROJECTS_GROUP 2
#define SIM_PROJECTS_MODEL 3
#define SIM_PROJECTS_SERIAL 4
#define SIM_PROJECTS_NOTES 5

//The columns displsayed in the SIM Project Details Table
#define SIM_PROJECTD_ITEM_ID 0
#define SIM_PROJECTD_SUPPLIER_ID 1
#define SIM_PROJECTD_CREATOR_ID 2
#define SIM_PROJECTD_CREATOR 3
#define SIM_PROJECTD_DATE 4
#define SIM_PROJECTD_DOCTYPE 5
#define SIM_PROJECTD_DOCNUM 6
#define SIM_PROJECTD_ITEM_NUM 7
#define SIM_PROJECTD_ITEM_DESC 8
#define SIM_PROJECTD_UNIT 9
#define SIM_PROJECTD_RECD 10
#define SIM_PROJECTD_DIST 11
#define SIM_PROJECTD_SUPPLIER_NAME 12

//Columns displayed in the Supplier Table
#define SIM_SUPPLIERS_ID 0
#define SIM_SUPPLIERS_NAME 1
#define SIM_SUPPLIERS_COUNTRY 2
#define SIM_SUPPLIERS_CONTACT 3
#define SIM_SUPPLIERS_EMAIL 4
#define SIM_SUPPLIERS_PHONE 5

//Columns displayed in the Suppliers Details Table
#define SIM_SUPPLIERD_ITEM_ID 0
#define SIM_SUPPLIERD_CREATOR_ID 1
#define SIM_SUPPLIERD_DATE 2
#define SIM_SUPPLIERD_CREATOR 3
#define SIM_SUPPLIERD_DOCTYPE 4
#define SIM_SUPPLIERD_DOCNAME 5
#define SIM_SUPPLIERD_PROJECT 6
#define SIM_SUPPLIERD_ITEM_NUM 7
#define SIM_SUPPLIERD_ITEM_DESC 8
#define SIM_SUPPLIERD_UNIT 9
#define SIM_SUPPLIERD_REQD 10
#define SIM_SUPPLIERD_ORDERED 11
#define SIM_SUPPLIERD_RECD 12
#define SIM_SUPPLIERD_UNIT_PRICE 13
#define SIM_SUPPLIERD_TOTAL 14

//PR Table Flags
#define SIM_PR_CREATOR_ID 0
#define SIM_PR_CREATOR 1
#define SIM_PR_DATE 2
#define SIM_PR_DATE_NEEDED 3
#define SIM_PR_NUM 4
#define SIM_PR_PROJECT 5
#define SIM_PR_SUPPLIER 6
#define SIM_PR_STATUS 7

//PRD Flags
#define SIM_PRD_SUPPLIER_ID 0
#define SIM_PRD_ITEM_ID 1
#define SIM_PRD_SUPPLIER 2
#define SIM_PRD_PROJECT 3
#define SIM_PRD_ITEM_NUM 4
#define SIM_PRD_ITEM_DESC 5
#define SIM_PRD_QTY 6
#define SIM_PRD_UNIT 7
#define SIM_PRD_EXPECTED_PRICE 8

//PO Table Flags
#define SIM_PO_CREATOR_ID 0
#define SIM_PO_SUPPLIER_ID 1
#define SIM_PO_CREATOR_NAME 2
#define SIM_PO_DATE 3
#define SIM_PO_NUM 4
#define SIM_PO_SUPPLIER 5
#define SIM_PO_PRICE 6
#define SIM_PO_SHIP_BY 7
#define SIM_PO_STATUS 8

//The columns displayed in the SIM PO Details Table
#define SIM_POD_ITEM_ID 0
#define SIM_POD_ITEM_NUM 1
#define SIM_POD_ITEM_NAME 2
#define SIM_POD_ITEM_TYPE 3
#define SIM_POD_QTY 4
#define SIM_POD_UNIT_PRICE 5
#define SIM_POD_TOTAL_PRICE 6

//RR Table Flags
#define SIM_RR_SUPPLIER_ID 0
#define SIM_RR_CREATOR_ID 1
#define SIM_RR_DATE_ARRIVED 2
#define SIM_RR_CREATOR 3
#define SIM_RR_DATE 4
#define SIM_RR_NUM 5
#define SIM_RR_PO_NUM 6
#define SIM_RR_SUPPLIER_NAME 7
#define SIM_RR_PO_STATUS 8

//RCR Details Table Column Flags
#define SIM_RRD_PROJECT_ID 0
#define SIM_RRD_ITEM_ID 1
#define SIM_RRD_SUPPLIER_ID 2
#define SIM_RRD_ITEM_NUM 3
#define SIM_RRD_ITEM_NAME 4
#define SIM_RRD_QTY_RECD 5
#define SIM_RRD_QTY_ORDERED 6
#define SIM_RRD_PO_NUM 7
#define SIM_RRD_PROJECT_NAME 8
#define SIM_RRD_SUPPLIER_NAME 9

//MRQ Table Column Flags
#define SIM_MR_CREATOR_ID 0
#define SIM_MR_CREATOR 1
#define SIM_MR_DATE 2
#define SIM_MR_NUM 3
#define SIM_MR_REQBY 4
#define SIM_MR_COLBY 5
#define SIM_MR_PROJECT 6

//MRQ Details Table Column Flags
#define SIM_MRD_PROJECT_ID 0
#define SIM_MRD_ITEM_ID 1
#define SIM_MRD_SUPPLIER_ID 2
#define SIM_MRD_ITEM_NUM 3
#define SIM_MRD_ITEM_NAME 4
#define SIM_MRD_QTY 5
#define SIM_MRD_SUPPLIER_NAME 6
#define SIM_MRD_PROJECT_NAME 7

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userlogin.h"
#include "createdocument.h"
#include "editcompanyinfo.h"

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
#include <QSqlError>
#include <QMessageBox>

#include <QSortFilterProxyModel>
#include <QIdentityProxyModel>

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

/*When any of these queries are editted, you should check to make sure that the
#definitions for what tables and columns are are also changed.

HOWEVER, this is tied to the search box, so in order to have search functionality
(that is easy to update in the codebase) this needs to stay as is in memory.
Refactor later.*/
QString itemQry, itemDetailsQry, projectQry, projectDetailsQry, poQry, rcrQry, reqQry;
QString poDetailsQry = "SELECT items.item_id, items.item_num AS 'Item Name', items.item_name AS 'Description'"
        ",items.item_type AS 'Category', linker.item_qty as 'Quantity', linker.item_price AS 'Unit Price'"
        ",(linker.item_price * linker.item_qty) as 'Total' "
        "FROM po "
        "LEFT JOIN suppliers ON po.supplier_id = suppliers.supplier_id "
        "LEFT JOIN linker ON po.po_num = linker.po_num "
        "LEFT JOIN items ON items.item_id = linker.item_id "
        "LEFT JOIN addresses ON po.ship_to = addresses.id "
        "LEFT JOIN shippers ON po.ship_by = shippers.id ";
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
QString reqDetailsQry = "SELECT trans.used_for, trans.item_id "
        ", items.item_num AS 'Item ID', items.item_name AS 'Description', projects.project_name AS 'Used For' "
        "FROM trans "
        "LEFT JOIN items ON items.item_id = trans.item_id "
        "LEFT JOIN projects ON projects.project_id = trans.used_for ";

//! Global variables like this could be refactored
//! It's not terrible or causing any memory leaks or anything,
//! but it could be more effecient

QSqlDatabase simdb;
char loginFlag;
int currentTable;
int dropdownIndex;
QString tflag;
QString docnum;
QString currentUser = "0";
QString currentPrivelage;
QString searchfor;
QSqlQueryModel *model = new QSqlQueryModel();
QSortFilterProxyModel *proxy = new QSortFilterProxyModel();
//! Using a proxy model to display data so the the QSqlQueryModel can have all the raw information stored within it
//! and the proxy model has the job of rendering it. That way no additional SQL queries have to be made and there is
//! a much lower CPU bottleneck. This will be a non-issue when using a DBMS that plans for simultaneous connections

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    simdb = QSqlDatabase::addDatabase("QSQLITE", "sim");
    simdb.setDatabaseName(qApp->applicationDirPath()+"/db/sim.db");
    ui->setupUi(this);
    ui->ItemTable->setModel(proxy);
    ui->ItemTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ItemTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_H),this,SLOT(goToHome()));
    ui->ItemTable->setContextMenuPolicy(Qt::CustomContextMenu);
    proxy->setSourceModel(model);
    loginFlag = SIM_LOGIN;
    MainWindow::Login();
    MainWindow::on_actionItems_Table_Default_triggered();
}
/*  The system is still vulnerable to SQL injections:
 *
 *  The following is a valid SQL injection
 *  %' UNION ALL SELECT supplier_id, supplier_name, address, e_mail, phone, phone2, fax, contact, email FROM suppliers--
 */

void MainWindow::prepareBaseTable()
{
    ui->ExtraDetailsWidget->setHidden(1);
    ui->FooterWidget->setHidden(1);
    ui->ItemTableLabel2->setHidden(1);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::Login() //Login, edit data, basically the userdata manager.
{
    /* Sqlite truly has no security. It's not meant to, so while this provides the illusion of security,
     * it's truly only meant to store user data, preferences, and check who is logged into the databse.
     * This will be changed if and when this is converted to a more industrial DBMS */
    if (simdb.open())
    {
        bool dbEmpty = simdb.tables(QSql::Tables).isEmpty();
        simdb.close();
        if (dbEmpty)
        {
            QMessageBox *message = new QMessageBox(this);
            message->setWindowTitle("No Previous Database Detected");
            message->setText("No sim.db file could be found. This is the file that stores all of the inventory and userdata of sim. If this is a fresh installation "
                "of SIM, then this is normal. A new database will be generated. If you already have an existing database file and are seeing this message, the database "
                "file was either deleted or moved. If you would like to restore your old database, simple replace "+qApp->applicationFilePath()+"/db/sim.db "
                "with your own file.");
            message->exec();
            delete message;
            MainWindow::createNewSIMDB();
            loginFlag = SIM_NEW;
        }

        simdb.open();
        QSqlQuery qry(simdb);
        qry.exec("SELECT COUNT(id) FROM userdata;");
        qry.next();
        simdb.close();
        if (qry.value(0).toInt() == 0)
        {
            loginFlag = SIM_NEW;
        }
        class UserLogin a;
        switch (loginFlag) {
        case SIM_LOGIN:
            a.setWindowTitle("Login");
            break;
        case SIM_NEW:
            a.setWindowTitle("Create New User");
            break;
        case SIM_EDIT:
            a.setWindowTitle("Edit Userdata");
            break;
        }
        a.exec();
    } else {
        qDebug("Could not open sim.db. It may not be in the correct location.");
    }

    if (currentUser == "0") //Throws may not work depending on compiler, but we can't use a close because the ui may not be loaded yet
        throw std::invalid_argument("Valid username and password combo was not entered");
    simdb.open();
    QSqlQuery qry(simdb);
    qry.exec(QString("SELECT privelage FROM userdata WHERE id = %1;").arg(currentUser));
    qry.next();
    simdb.close();
    currentPrivelage = qry.value(0).toString();
}

void MainWindow::createNewSIMDB()
{
    simdb.open();
    QSqlQuery q(simdb);
    q.exec("PRAGMA foreign_keys = 1;");
    q.exec("CREATE TABLE userdata ("
                "id         INTEGER PRIMARY KEY "
                ",user      TEXT UNIQUE NOT NULL "
                ",pass      TEXT "
                ",name      TEXT NOT NULL "
                ",email     TEXT "
                ",theme     INTEGER DEFAULT 0 "
                ",privelage INTEGER NOT NULL DEFAULT 0 "
            ");");
    /*  PRIVELAGE FLAGS (L->H) ... Only the admin can create users
     *  0 - Viewer >> can only view data
     *  1 - Mechanic (authorized requestor) >> can create PR, MR
     *  2 - Purchasing >> can create QR, POs
     *  3 - Inventory Manager >> can create PR, MR, RR
     *  4 - Admin >> has full access to everything */
    q.exec("CREATE TABLE recurring_custom_fields ("
                "id     INTEGER PRIMARY KEY "
                ",tflag INTEGER NOT NULL "
                ",name  TEXT NOT NULL "
            ");");
    q.exec("CREATE TABLE custom_fields ("
                "id     INTEGER PRIMARY KEY "
                ",tflag INTEGER NOT NULL "
                ",tnum  INTEGER NOT NULL "
                ",name  TEXT NOT NULL "
                ",value TEXT "
            ")");
    /*                      CUSTOM FIELDS ARE A LITTLE STRANGE
     * A user can enter any number of custom fields. Qt will then generate
     * a custom field at an appropriate location within the appropriate table. Every time a new document
     * is created, it check recurring_custom_fields to see if that table has any recurring custom fields.
     * For each custom field, SIM autogenerates a custom field with the default value of NULL. The
     * rational behind why custom fields are implemented like this can be found in the documentation.
     * Table Flags (tflag):
     *  0 = PR - purchase requisition
     *  1 = QR - quotation request
     *  2 = PO - purchase order
     *  3 = RR - receiving report
     *  4 = MR = material requisition */
    q.exec("CREATE TABLE addresses ("
                "id         INTEGER PRIMARY KEY "
                ",address   TEXT NOT NULL "
            ");");
    q.exec("INSERT INTO addresses (address) VALUES ('Unknown Address');");
    q.exec("CREATE TABLE company (" //This is so similar to a supplier that it may be worth it to classify the company as a supplier.
                "name                   TEXT "
                ",shipping_address_id   TEXT "
                ",billing_address_id    TEXT "
                ",phone                 TEXT "
                ",phone2                TEXT "
                ",fax                   TEXT "
                ",email                 TEXT "
                ",website               TEXT "
            ");");
    q.exec("INSERT INTO company (name, shipping_address_id, billing_address_id) VALUES ('INSERT COMPANY NAME',1,1);");
    q.exec("CREATE TABLE suppliers ( "
                "id             INTEGER PRIMARY KEY "
                ",name          TEXT NOT NULL "
                ",address_id	INTEGER REFERENCES addresses (id) "
                ",email			TEXT "
                ",phone			TEXT "
                ",phone2    	TEXT "
                ",fax			TEXT "
                ",contact_name  TEXT "
            ");");
    q.exec("CREATE TABLE projects ( " //This can be vehicles, machines, tasks, etc.
                "name       TEXT PRIMARY KEY " //Is is less confusing if project names are unique.
                ",desc      TEXT " //If further information about the project is needed
                ",isgroup   BOOL NOT NULL DEFAULT 0 " //Projects can also be groups of projects
                ",model     TEXT "
                ",serial    TEXT "
                ",notes     TEXT " //For huge amounts of information about the project
           ");");
    q.exec("CREATE TABLE project_groups ( "
                "id         INTEGER PRIMARY KEY "
                ",name      REFERENCES projects (name) ON UPDATE CASCADE "
                ",project   REFERENCES projects (name) ON UPDATE CASCADE "
            ");");
    q.exec("CREATE TABLE items ("
                "id     INTEGER PRIMARY KEY "
                ",num   TEXT NOT NULL "
                ",desc  TEXT NOT NULL "
                ",cat   TEXT NOT NULL DEFAULT 'unknown' "
                ",qty   REAL NOT NULL DEFAULT 0 "
                ",unit  TEXT "
                ",shelf TEXT "	//Location of item in storage
                ",bin   TEXT "	//Location of item in storage
           ");");
           /*       A NOTE ABOUT UNITS AND CHILD ITEMS
            * If the units are not NULL, EACH or EA, special operations can be performed on the item.
            * For example, if you buy a pack of 1000 screws, you may not want to always distribute all 1000
            * of those screws at once, so you can break up the pack of 1000 screws into individual screws.
            * A new item is created with the same item info, but has a different unit.
            * Now you can distribute 50 screws, and still have 950 remaining. Since there are so many different
            * types of units (there are 12 units in a dozen, but exactly how many units are in a box?),
            * the user is meant to break these up themselves. This could be a security vulnerability. */
    q.exec("CREATE TABLE relatives ("
                "id         INTEGER PRIMARY KEY "
                ",original  INTEGER REFERENCES items (id) ON UPDATE CASCADE "
                ",relative  INTEGER REFERENCES items (id) ON UPDATE CASCADE "
            ");");
           /*           NEED TO THINK ABOUT HOW RELATIVES WILL WORK A LITTLE MORE
            * Explanation of relatives: Imagine a situation where a part for an old vehicle is no
            * longer manufactured, but there happens to be a 3rd party who produces an exact replica
            * of that part, but under a different part number. This is the exact situation siblings are
            * for. It is not to link 5 different colors of rag together, it is used exclusively for when
            * two different part numbers serve the exact same purpose, and for all intents and purposes,
            * are the exact same. This is its own table because these cases are very rare. Children are
            * also relatives. A whole family of items that do the exact same thing can be generated. */
    q.exec("CREATE TABLE items_projects_linker ( "
                "id         INTEGER PRIMARY KEY "
                ",item_id   INTEGER REFERENCES items (id) ON UPDATE CASCADE "
                ",project   TEXT REFERENCES projects (name) ON UPDATE CASCADE "
            ");");
    q.exec("CREATE TABLE custom_expenses ("
                "id     INTEGER PRIMARY KEY "
                ",tflag INTEGER NOT NULL "
                ",tnum  INTEGER NOT NULL "
                ",name  TEXT "
                ",rate  REAL " //If the rate is 0, then the rate checkbox was not clicked
                ",value REAL NOT NULL "
            ");");
    q.exec("CREATE TABLE pr ( "
                "num                            INTEGER PRIMARY KEY "
                ",date                          DATE "
                ",date_needed                   DATE /*The day by which this is needed. Enter ASAP if it's urgent.*/ "
                ",requested_by                  INTEGER NOT NULL REFERENCES userdata (id) "
                ",project                       TEXT NOT NULL REFERENCES projects (name) ON UPDATE CASCADE /*The project this is needed for*/ "
                ",discount_before_tax           BOOL DEFAULT 1 "
                ",taxable_subtotal              REAL " // This is just for storage. Whenever a document is
                ",tax_exempt_subtotal           REAL " // opened this will be calculated on the fly
                ",discount_on_taxable_rate      REAL "
                ",discount_on_taxable           REAL "
                ",discount_on_tax_exempt_rate   REAL "
                ",discount_on_tax_exempt        REAL "
                ",discount_after_tax_rate       REAL "
                ",discount_after_tax            REAL "
                ",tax_rate                      REAL "
                ",tax                           REAL "
                ",total                         REAL "
                ",notes                         TEXT "
                ",status                        INTEGER NOT NULL DEFAULT 0 "
            ");");
    /* STATUS FLAGS
     *      0 = DRAFT
     *      1 = completed
     *      2 = partially completed
     *      3 = new
     *      4 = partially completed (at least one rejected)
     *      5 = completed (at least one rejected) */
    q.exec("CREATE TABLE prd ( "
                "id                     INTEGER PRIMARY KEY "
                ",pr_num                INTEGER NOT NULL REFERENCES pr (num) ON UPDATE CASCADE "
                ",item_id               INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",supplier_id           INTEGER REFERENCES suppliers (id) /*The suggested supplier*/ "
                ",qty                   REAL NOT NULL "
                ",expected_unit_price   REAL "
                ",taxable               BOOLEAN "
                ",total                 REAL "
                ",status                INTEGER NOT NULL DEFAULT 1 /*0=closed,1=open,2=rejected*/ "
            ");");
    q.exec("CREATE TABLE qr ( "
                "num            INTEGER PRIMARY KEY "
                ",date          DATE "
                ",supplier_id   INTEGER NOT NULL REFERENCES supplier (id) "
                ",requested_by  INTEGER NOT NULL REFERENCES userdata (id) "
                ",notes         TEXT "
                ",status        INTEGER NOT NULL DEFAULT 2 /*0=Closed,1=Items-Missing,2=Open*/ "
            ");");
    q.exec("CREATE TABLE qrd ( "
                "id         INTEGER PRIMARY KEY "
                ",qr_num    INTEGER NOT NULL REFERENCES qr (num) "
                ",item_id   INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",unit      TEXT "
                ",qty       REAL NOT NULL "
                ""
            ");");
    q.exec("CREATE TABLE po ( "
                "num                    INTEGER PRIMARY KEY "
                ",invoice_num           INTEGER "
                ",date                  DATE "
                ",supplier_id           INTEGER REFERENCES suppliers (id) NOT NULL "
                ",supplier_address_id   INTEGER REFERENCES addresses (id) "
                ",shipping_address_id   INTEGER REFERENCES addresses (id) "
                ",billing_address_id    INTEGER REFERENCES addresses (id) "
                ",shipper               TEXT "
                ",authorized_by         INTEGER REFERENCES userdata (id) "
                ",subtotal              REAL " // sum of prices from pod
                ",taxable_subtotal      REAL " // sum of prices from pod where taxable = 1
                ",discount_rate         REAL "
                ",discount              REAL DEFAULT 0 "
                ",tax_rate              REAL " // Percentage tax
                ",tax                   REAL " // Amount tax
                ",shipping              REAL " // shipping & handling AKA freight
                ",other                 REAL " // Any uncategorized expenses. May be better to do a custom table here.
                ",total                 REAL " // subtotal + tax + shipping + other - discount
                ",notes                 TEXT " //Any additional information or instructions
                ",status                INTEGER NOT NULL DEFAULT 3 " // See below for status info
           ");");
            /*              status FLAGS ARE AS FOLLOWS
             * 0 = PO Closed. Ideally, items from PO have been received, verified.
             * 1 = PO Partially Received. Only some of the items from the PO have been received.
             * 2 = PO Items have shipped, but none received. This is useful to note because of some vendor's FOB shipping policy.
             * 3 = PO sent but items have not been received. */
    q.exec("CREATE TABLE pod (" //POD stand for purchase order details
                "id             INTEGER PRIMARY KEY "
                ",po_num        INTEGER REFERENCES po (num) ON UPDATE CASCADE "
                ",pr_num        INTEGER REFERENCES pr (num) " //This is to internally update items in the PR as they are completed
                ",item_id       INTEGER REFERENCES items (id) ON UPDATE CASCADE NOT NULL "
                ",qty           REAL NOT NULL "
                ",unit          TEXT "
                ",unit_price    REAL NOT NULL "
                ",discount_rate REAL " // This may not be shown, but may be useful internally
                ",discount      REAL " // Amount
                ",taxable       BOOL NOT NULL DEFAULT 1 " //Can tax be applied to this item?
                ",price         REAL NOT NULL " //qty * unit_price - discount
                ",status        INTEGER NOT NULL DEFAULT 2 " // See below for status info
                ",condition     TEXT " // This is a mirror of the condition in receiving report details
            ");");
            /*              POD status FLAGS
             *  0 = correct quantity received
             *  1 = partial quantity received
             *  2 = none received
             *  3 = too much received */
    q.exec("CREATE TABLE rr (" //rr - Receiving Report
                "num            INTEGER PRIMARY KEY "
                ",date          DATE NOT NULL " //Date collected
                ",date_arrived  DATE NOT NULL "
                ",shipped_by    TEXT NOT NULL "
                ",delivered_by  TEXT NOT NULL "
                ",inspected_by  TEXT REFERENCES userdata (id) "
                ",fob           TEXT "
                ",po_num        INTEGER REFERENCES po (num) " //There may be no PO# in certain cases
                ",invoice_num   INTEGER REFERENCES po (invoice_num) "
                ",supplier_id   INTEGER REFERENCES suppliers (id) "
                ",comments      TEXT "
            ");");
    q.exec("CREATE TABLE rrd ("
                "id             INTEGER PRIMARY KEY "
                ",rr_num        INTEGER NOT NULL REFERENCES rr (num) ON UPDATE CASCADE "
                ",item_id       INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",qty           REAL NOT NULL " //This has to be checked against the PO
                ",unit          TEXT "
                ",condition     TEXT NOT NULL REFERENCES conditions (name) "
                ",comments      TEXT "
            ");");
    q.exec("CREATE TABLE mr ("
                "num            INTEGER PRIMARY KEY "
                ",date          DATE NOT NULL "
                ",requested_by  TEXT NOT NULL "
                ",authorized_by INTEGER REFERENCES userdata (id) "
                ",collected_by  TEXT NOT NULL "
                ",issued_by     INTEGER REFERENCES userdata (id) "
                ",project       INTEGER REFERENCES projects (name) ON UPDATE CASCADE "
                ",comments      TEXT "
            ");");
    q.exec("CREATE TABLE mrd ( "
                "id         INTEGER PRIMARY KEY "
                ",mr_num    INTEGER NOT NULL REFERENCES mr (num) ON UPDATE CASCADE "
                ",item_id   INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",qty       REAL NOT NULL "
                ",unit      TEXT REFERENCES items (unit) "
            ");");
    /*                                  A WORD ABOUT DRAFTS
     *  Drafts will inhabit the bottom few numbers of POs, so if POs begin from 1001, drafts span the numbers
     *  1 - 999. This means that it is possible to run out of drafts if the users managing the database do not
     *  clear out drafts. This is intentional (a.) because it significantly less complex that including a separate
     *  table dedicated to only drafts and (b.) because it encourages the user to not have drafts sitting in SIM
     *  for a very long time. When drafts are cleared out, it creates space for the next draft to come in. */
    simdb.close();
}

void MainWindow::on_actionItem_History_triggered() //Every transaction in the Inventory
{
    currentTable = SIM_INVENTORY_HISTORY;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Inventory Movement History");
    ui->ItemTableLabel2->setHidden(0);
    ui->ItemTableLabel2->setText("[Insert Date Range Here]");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT prd.supplier_id, '0', pr.requested_by, pr.date AS 'Date', 'Purchase Requisition' AS 'Document Type', prd.pr_num AS 'Doc#', userdata.name As 'Created by' , prd.unit AS 'Unit'"
    ",prd.qty AS 'Qty Reqd.', NULL AS 'Qty Ordered', suppliers.name AS 'Supplier', pr.project AS 'Project', NULL AS 'Qty In', NULL AS 'Qty Out' "
    "FROM prd "
    "LEFT JOIN pr ON prd.pr_num = pr.num "
    "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
    "LEFT JOIN userdata ON userdata.id = pr.requested_by "
    "UNION "
    "SELECT po.supplier_id, '1', po.authorized_by, po.date, 'Purchase Order', pod.po_num, userdata.name, pod.unit, NULL, pod.qty, suppliers.name, NULL, NULL, NULL "
    "FROM pod "
    "LEFT JOIN po ON pod.po_num = po.num "
    "LEFT JOIN suppliers ON suppliers.id = po.supplier_id "
    "LEFT JOIN userdata ON userdata.id = po.authorized_by "
    "UNION "
    "SELECT rr.supplier_id, '2', rr.inspected_by, rr.date, 'Receiving Report', rrd.rr_num, userdata.name, rrd.unit, NULL, NULL, suppliers.name, NULL, rrd.qty, NULL "
    "FROM rrd "
    "LEFT JOIN rr ON rrd.rr_num = rr.num "
    "LEFT JOIN suppliers ON suppliers.id = rr.supplier_id "
    "LEFT JOIN userdata ON userdata.id = rr.inspected_by "
    "UNION "
    "SELECT NULL, '3', mr.authorized_by, mr.date, 'Material Requisition', mrd.mr_num, userdata.name, mrd.unit, NULL, NULL, NULL, mr.project, NULL, mrd.qty "
    "FROM mrd "
    "LEFT JOIN mr ON mrd.mr_num = mr.num "
    "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
    ";");
    model->setQuery(qry);
    while (model->canFetchMore()) {model->fetchMore();}
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_ITEMD_CREATOR_ID);
    ui->ItemTable->hideColumn(SIM_ITEMD_SUPPLIER_ID);
    ui->ItemTable->hideColumn(SIM_ITEMD_DOCTYPE_ID);
}

void MainWindow::on_actionItems_Table_Default_triggered() //Items
{
    currentTable = SIM_ITEMS;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Items Table"); 
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "Item ID");
    ui->SearchForDropdown->addItem("Description");
    ui->SearchForDropdown->addItem("Category");
    ui->SearchForDropdown->addItem("Project");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT i.id, i.num AS 'Item ID', i.desc AS 'Description', i.cat AS 'Category', i.qty AS 'Qty' "
            ",i.unit AS 'Unit', i.shelf AS 'Shelf', i.bin AS 'Bin', group_concat(DISTINCT ipl.project) AS 'Used For' "
            "FROM items AS 'i' "
            "LEFT JOIN items_projects_linker AS 'ipl' ON i.id = ipl.item_id "
            "GROUP BY i.id "
            "ORDER BY i.id DESC;");
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); } //this exists to allow the db to be closed. An SQLite specific quirk.
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_ITEMS_ITEM_ID, true);
}

void MainWindow::createItem_Details_Table(QString itemid)
{
    currentTable = SIM_ITEM_DETAILS;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel2->setHidden(0);
    ui->ItemTableLabel2->setText("[Insert Date Range Here]");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "PR#");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec(QString("SELECT num, desc FROM items WHERE id = %1;").arg(itemid));
    qry.next();
    ui->ItemTableLabel->setText(QString("Movement History For: %1 || %2 ").arg(qry.value(0).toString(), qry.value(1).toString()));
    qry.exec(QString("SELECT prd.supplier_id, '0', pr.requested_by, pr.date AS 'Date', 'Purchase Requisition' AS 'Document Type', pr.num AS 'Doc#', userdata.name As 'Created by' /*,items.unit AS 'Unit'*/"
    ",prd.qty AS 'Qty Reqd.', NULL AS 'Qty Ordered', suppliers.name AS 'Supplier', pr.project AS 'Project', NULL AS 'Qty In', NULL AS 'Qty Out' "
    "FROM prd "
    "LEFT JOIN pr ON prd.pr_num = pr.num "
    "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
    "LEFT JOIN userdata ON userdata.id = pr.requested_by "
    "WHERE prd.item_id = %1 "
    "UNION "
    "SELECT po.supplier_id, '1', po.authorized_by, po.date, 'Purchase Order', pod.po_num, userdata.name /*,items.unit*/, NULL, pod.qty, suppliers.name, NULL, NULL, NULL "
    "FROM pod "
    "LEFT JOIN po ON pod.po_num = po.num "
    "LEFT JOIN suppliers ON suppliers.id = po.supplier_id "
    "LEFT JOIN userdata ON userdata.id = po.authorized_by "
    "WHERE pod.item_id = %1 "
    "UNION "
    "SELECT rr.supplier_id, '2', rr.inspected_by, rr.date, 'Receiving Report', rrd.rr_num, userdata.name /*,items.unit*/, NULL, NULL, suppliers.name, NULL, rrd.qty, NULL "
    "FROM rrd "
    "LEFT JOIN rr ON rrd.rr_num = rr.num "
    "LEFT JOIN suppliers ON suppliers.id = rr.supplier_id "
    "LEFT JOIN userdata ON userdata.id = rr.inspected_by "
    "WHERE rrd.item_id = %1 "
    "UNION "
    "SELECT NULL, '3', mr.authorized_by, mr.date, 'Material Requisition', mrd.mr_num, userdata.name /*,items.unit*/, NULL, NULL, NULL, mr.project, NULL, mrd.qty "
    "FROM mrd "
    "LEFT JOIN mr ON mrd.mr_num = mr.num "
    "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
    "WHERE mrd.item_id = %1 "
    ";").arg(itemid));
    //Should probably include authorizer, siblings, units, and other info
    model->setQuery(qry);
    while (model->canFetchMore()) {model->fetchMore();}
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_ITEMD_SUPPLIER_ID);
    ui->ItemTable->hideColumn(SIM_ITEMD_DOCTYPE_ID);
    ui->ItemTable->hideColumn(SIM_ITEMD_CREATOR_ID);
}

void MainWindow::on_actionProjects_triggered() //Project
{
    currentTable = SIM_PROJECTS;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Projects");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "Project");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT name AS 'Project', isgroup AS 'Group?', model as 'Model#', serial as 'Serial#', notes AS 'Notes' FROM projects;");
    model->setQuery(qry);
    while (model->canFetchMore()) {model->fetchMore();}
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
}

void MainWindow::createProject_Details_Table(QString project)
{
    currentTable = SIM_PROJECT_DETAILS;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText(project + " Usage History");
    ui->ItemTableLabel2->setHidden(0);
    ui->ItemTableLabel2->setText("[Insert Date Range Here]");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0,"Item ID");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec(QString("SELECT prd.item_id, prd.supplier_id, pr.requested_by, userdata.name AS 'Creator', pr.date, 'PR' AS 'Doc', pr.num AS '#', items.num AS 'Item ID'"
        ", items.desc AS 'Description', items.unit AS 'Unit', prd.qty AS 'Qty Reqd.', NULL AS 'Qty Dist.', suppliers.name AS 'Supplier' "
        "FROM prd "
        "LEFT JOIN pr ON pr.num = prd.pr_num "
        "LEFT JOIN items ON items.id = prd.item_id "
        "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
        "LEFT JOIN userdata ON userdata.id = pr.requested_by "
        "WHERE pr.project = '%1' "
        "UNION "
        "SELECT mrd.item_id, NULL, mr.authorized_by, userdata.name, mr.date, 'MR', mr.num, items.num, items.desc, items.unit, NULL, mrd.qty, NULL "
        "FROM mrd "
        "LEFT JOIN mr ON mr.num = mrd.mr_num "
        "LEFT JOIN items ON items.id = mrd.item_id "
        "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
        "WHERE mr.project = '%1' "
        ";").arg(project));
    model->setQuery(qry);
    while (model->canFetchMore()) {model->fetchMore();}
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_PROJECTD_ITEM_ID);
    ui->ItemTable->hideColumn(SIM_PROJECTD_SUPPLIER_ID);
    ui->ItemTable->hideColumn(SIM_PROJECTD_CREATOR_ID);
}

void MainWindow::on_actionSuppliers_triggered() //Suppliers
{
    currentTable = SIM_SUPPLIERS;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Suppliers");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0,"Supplier Name");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT s.id, s.name AS 'Supplier', s.contact_name AS 'Contact', s.email AS 'email', s.phone AS 'Phone' "
        "FROM suppliers AS 's' "
        ";");
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_SUPPLIERS_ID);
}

void MainWindow::createSuppliers_Details_Table(QString supplier_id, QString supplier_name)
{
    currentTable = SIM_SUPPLIERS_DETAILS;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Transactions Made Involving " + supplier_name);
    ui->ItemTableLabel2->setHidden(0);
    ui->ItemTableLabel2->setText("[Insert Date Range Here]");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "Creator");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec(QString("SELECT prd.item_id, pr.requested_by, pr.date AS 'Date', userdata.name AS 'Creator', 'PR' AS 'Doc', prd.pr_num AS '#', pr.project AS 'Project', items.num AS 'Item ID' "
        ", items.desc AS 'Description' , items.unit AS 'Unit', prd.qty AS 'Qty Reqd.', NULL AS 'Qty Ordered', NULL AS 'Qty Recd.', NULL As 'Unit Price', NULL AS 'Total' "
        "FROM prd "
        "LEFT JOIN pr ON pr.num = prd.pr_num "
        "LEFT JOIN items ON items.id = prd.item_id "
        "LEFT JOIN userdata ON userdata.id = pr.requested_by "
        "WHERE prd.supplier_id = %1 "
        "UNION "
        "SELECT pod.item_id, po.authorized_by, po.date, userdata.name, 'PO', pod.po_num, NULL, items.num, items.desc, items.unit, NULL, pod.qty, NULL, pod.unit_price, pod.price "
        "FROM pod "
        "LEFT JOIN po ON po.num = pod.po_num "
        "LEFT JOIN items ON items.id = pod.item_id "
        "LEFT JOIN userdata ON userdata.id = po.authorized_by "
        "WHERE po.supplier_id = '%1' "
        "UNION "
        "SELECT rrd.item_id, rr.inspected_by, rr.date, userdata.name, 'RR', rrd.rr_num, NULL, items.num, items.desc, items.unit, NULL, NULL, rrd.qty "
        ", (SELECT pod.unit_price WHERE pod.item_id = rrd.item_id), (SELECT pod.unit_price WHERE pod.item_id = rrd.item_id) * rrd.qty "
        "FROM rrd "
        "LEFT JOIN rr ON rr.num = rrd.rr_num "
        "LEFT JOIN items ON items.id = rrd.item_id "
        "LEFT JOIN userdata ON userdata.id = rr.inspected_by "
        "LEFT JOIN pod ON pod.po_num = rr.po_num "
        "WHERE rr.supplier_id = '%1' "
        ";").arg(supplier_id));
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_SUPPLIERD_ITEM_ID);
    ui->ItemTable->hideColumn(SIM_SUPPLIERD_CREATOR_ID);
}

void MainWindow::on_actionPurchase_Requisitions_triggered() //PR
{
    currentTable = SIM_PR;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Purchase Requisitions");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0,"PR#");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT pr.requested_by, userdata.name AS 'Requested By', pr.date AS 'Date Created', pr.date_needed AS 'Date Needed', pr.num AS 'PR#' "
        ",group_concat(DISTINCT pr.project) AS 'Project(s)', group_concat(DISTINCT suppliers.name) AS 'Recommended Supplier(s)' "
        ",CASE pr.status "
            "WHEN 0 THEN 'Draft' "
            "WHEN 1 THEN 'Closed '"
            "WHEN 2 THEN 'Partial' "
            "WHEN 3 THEN 'Open' "
            "WHEN 4 THEN 'Partial (Item(s) Rejected)' "
            "WHEN 5 THEN 'Closed (Item(s) Rejected)' "
        "END as 'Status' "
        "FROM pr "
        "LEFT JOIN prd ON prd.pr_num = pr.num "
        "LEFT JOIN userdata ON pr.requested_by = userdata.id "
        "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
        "GROUP BY pr.num "
        "ORDER BY pr.num DESC "
    ";");
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_PR_CREATOR_ID);
}

void MainWindow::createPR_Details(QString prnum)
{
    currentTable = SIM_PRD;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel2->setHidden(false);
    ui->ExtraDetailsWidget->setHidden(false);
    ui->FooterWidget->setHidden(false);
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "Item ID");
    QString extra2;

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec(QString("SELECT userdata.name, pr.date, pr.date_needed, pr.project "
        ",CASE pr.status "
            "WHEN 0 THEN 'Draft' "
            "WHEN 1 THEN 'Closed '"
            "WHEN 2 THEN 'Partial' "
            "WHEN 3 THEN 'Open' "
            "WHEN 4 THEN 'Partial (Item(s) Rejected)' "
            "WHEN 5 THEN 'Closed (Item(s) Rejected)' "
        "END as 'Status' "
        "FROM pr "
        "LEFT JOIN userdata ON userdata.id = pr.requested_by "
        "WHERE pr.num = %1;").arg(prnum));
    qry.next();
    ui->ItemTableLabel->setText(QString("Purchase Requisition #%1 requested by %2")
                                .arg(prnum,qry.value(0).toString()));
    ui->ItemTableLabel2->setText(qry.value(1).toString());
    extra2 = QString("Status: %1\n").arg(qry.value(2).toString());
    if (!(qry.value(4).toString() == ""))
        extra2 = QString("%1Needed by: %2\n").arg(extra2, qry.value(4).toString());
    ui->ExtraDetails2->setText(extra2);
    qry.exec(QString("SELECT printf('%.2f', SUM(prd.expected_unit_price * prd.qty)) FROM prd WHERE prd.pr_num = %1;").arg(prnum));
    qry.next();
    ui->Totals->setText(QString("Total $%1").arg(qry.value(0).toString()));

    qry.exec(QString("SELECT prd.supplier_id, prd.item_id, suppliers.name AS 'Supplier', items.num 'Item ID' , items.desc AS 'Description'"
        ", items.cat AS 'Category', prd.qty AS 'Quantity', items.unit AS 'Unit', printf('%.2f', prd.expected_unit_price) AS 'Expected Price',  prd.total AS 'Total' "
        "FROM prd "
        "LEFT JOIN suppliers ON prd.supplier_id = suppliers.id "
        "LEFT JOIN items ON prd.item_id = items.id "
        "WHERE prd.pr_num = %1"
        ";").arg(prnum));
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_PRD_ITEM_ID);
    ui->ItemTable->hideColumn(SIM_PRD_SUPPLIER_ID);
}

void MainWindow::on_actionPurchase_Orders_triggered() //PO
{
    currentTable = SIM_PO;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Purchase Orders");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0,"PO#");
    ui->SearchForDropdown->addItem("Date");
    ui->SearchForDropdown->addItem("Supplier");
    ui->SearchForDropdown->addItem("Ship By");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT po.authorized_by, po.supplier_id, userdata.name AS 'Created By', po.date AS 'Date', po.num AS 'PO#' "
        ",suppliers.name AS 'Supplier', total AS 'Total Price', ship_by AS 'Shipper' "
        ",CASE status "
            "WHEN 0 THEN 'Closed '"
            "WHEN 1 THEN 'Partial' "
            "WHEN 2 THEN 'Shipped' "
            "WHEN 3 THEN 'Sent' "
            "WHEN 4 THEN 'DRAFT' "
        "END as 'Status' "
        "FROM po "
        "LEFT JOIN userdata ON po.authorized_by = userdata.id "
        "LEFT JOIN suppliers ON suppliers.id = po.supplier_id "
    ";");
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_PO_CREATOR_ID);
    ui->ItemTable->hideColumn(SIM_PO_SUPPLIER_ID);
}

void MainWindow::createPOD_Table(QString ponum)
{
    currentTable = SIM_POD;
    MainWindow::prepareBaseTable();
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0, "Item");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec(QString("SELECT po.date, suppliers.name FROM po JOIN suppliers ON suppliers.id = po.supplier_id WHERE po.num = %1;").arg(ponum));
    ui->ItemTableLabel->setText(QString("Purchase Order %1 from %2").arg(ponum, qry.value(0).toString()));
    ui->ItemTableLabel2->setHidden(false);
    ui->ItemTableLabel2->setText(qry.value(1).toString());
    qry.exec();
    model->setQuery(qry);
    while (model->canFetchMore()) {model->fetchMore();}
    simdb.close();

    ui->ItemTable->update();
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

void MainWindow::on_actionReceived_triggered() //RR
{
    currentTable = SIM_RR;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Receiving Reports");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0,"RR#");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT rr.supplier_id, rr.inspected_by, rr.date_arrived AS 'Arrival Date', userdata.name AS 'Inspected By', rr.date 'Collection Date', rr.num AS 'RR#', rr.po_num AS 'PO#'"
        ", suppliers.name AS 'Supplier'"
        ", CASE po.status "
            "WHEN 0 THEN 'Closed' "
            "WHEN 1 THEN 'Incomplete' "
            "ELSE NULL "
        "END AS 'PO Status' "
        "FROM rr "
        "LEFT JOIN suppliers ON suppliers.id = rr.supplier_id "
        "LEFT JOIN po ON po.num = rr.po_num "
        "LEFT JOIN userdata ON userdata.name = rr.inspected_by"
        ";");
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    simdb.close();

    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_RR_SUPPLIER_ID);
    ui->ItemTable->hideColumn(SIM_RR_CREATOR_ID);
}

void MainWindow::on_actionRequested_Items_triggered() //MR
{
    currentTable = SIM_MR;
    MainWindow::prepareBaseTable();
    ui->ItemTableLabel->setText("Material Requisitions");
    MainWindow::clearDropdown();
    ui->SearchForDropdown->setItemText(0,"MR#");

    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT mr.authorized_by, userdata.name AS 'Authorized By', mr.date AS 'Date', mr.num AS 'MR#'"
        ", mr.requested_by AS 'Requested By', mr.collected_by AS 'Collected By', group_concat(DISTINCT mrd.project) AS 'Project(s)' "
        "FROM mr "
        "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
        "LEFT JOIN mrd ON mrd.mr_num = mr.num "
        "GROUP BY mr.num "
        "ORDER BY mr.num "
        ";");
    model->setQuery(qry);
    while (model->canFetchMore()) {model->fetchMore();}
    simdb.close();

    ui->ItemTable->update();
    MainWindow::showAllColumns();
    ui->ItemTable->hideColumn(SIM_MR_CREATOR_ID);
}






void MainWindow::on_SearchForDropdown_currentIndexChanged(int index)
{
    dropdownIndex = index;
    /* Update this function
    MainWindow::on_SearchBox_textChanged(searchfor);
    */
}

void MainWindow::goToHome()
{
    /* Update this function
    switch(currentTable){
    case SIM_ITEM_DETAILS:{
        MainWindow::on_actionItems_Table_Default_triggered();
        break;
    }
    case SIM_PROJECT_DETAILS: {
        MainWindow::on_actionProjects_triggered();
        break;
    }
    case SIM_POD: {
        MainWindow::on_actionPurchase_Orders_triggered();
        break;
    }
    }
    */
}





void MainWindow::createRequested_Details_Table(QString mrq, QString date, QString reqby)
{
    currentTable = SIM_MRD;
    QSqlQuery query;
    QSqlDatabase qdb = QSqlDatabase::database();
    query.exec(reqDetailsQry + "WHERE trans.mrq_num = '" + mrq + "';");
    model->setQuery(query);
    while (model->canFetchMore()) {model->fetchMore();}
    qdb.close();

    model->insertColumn(SIM_MRD_SUPPLIER_ID);
    model->insertColumn(SIM_MRD_QTY);
    model->insertColumn(SIM_MRD_SUPPLIER_NAME);
    ui->ItemTable->update();
    ui->ItemTableLabel->setText("Requisition #: " + mrq + " || " + date + " || Requested by: " + reqby);
    MainWindow::showAllColumns();
    ui->ItemTable->setColumnHidden(SIM_MRD_PROJECT_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRD_ITEM_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRD_SUPPLIER_ID, true);
    ui->ItemTable->setColumnHidden(SIM_MRD_SUPPLIER_NAME, true);
}

void MainWindow::createReceived_Details_Table(QString rcr, QString date)
{
    currentTable = SIM_RRD;
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

}

void MainWindow::on_ItemTable_doubleClicked(const QModelIndex &index)
{
    //First, identify table. Second, identify row. Third, switch table.
    switch (currentTable) {
    case SIM_ITEMS: {
        switch (index.column()) {
        case SIM_ITEMS_ITEM_NUM:
        case SIM_ITEMS_ITEM_DESC:
        case SIM_ITEMS_QTY: {
            //Go through the history of this particular item
            MainWindow::createItem_Details_Table(index.siblingAtColumn(SIM_ITEMS_ITEM_ID).data().toString());
            break;
        }
        case SIM_ITEMS_ITEM_CAT: {
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
            QString project = index.siblingAtColumn(SIM_ITEMS_PROJECT).data().toString();
            MainWindow::createProject_Details_Table(project);
            break;
        }
        }
        break;
    }
    case SIM_ITEM_DETAILS: {
        switch (index.column()) {
        case SIM_ITEMD_DOCTYPE_ID: {
            MainWindow::createPOD_Table(index.data().toString());
            break;
        }
        case SIM_ITEMD_PROJECT: {
            QString project = index.siblingAtColumn(SIM_ITEMD_PROJECT).data().toString();
            MainWindow::createProject_Details_Table(project);
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
            QString project = index.siblingAtColumn(SIM_PROJECTS_NAME).data().toString();
            MainWindow::createProject_Details_Table(project);
            break;
        }
    }
    case SIM_SUPPLIERS:
    {
        QString supplierid = index.siblingAtColumn(SIM_SUPPLIERS_ID).data().toString();
        QString suppliername = index.siblingAtColumn(SIM_SUPPLIERS_NAME).data().toString();
        MainWindow::createSuppliers_Details_Table(supplierid,suppliername);
        break;
    }
    case SIM_PR: {
        if (index.siblingAtColumn(SIM_PR_STATUS).data().toString() == "Draft")
        {
            docnum = index.siblingAtColumn(SIM_PR_NUM).data().toString();
            tflag = "0";
            CreateDocument h;
            h.setWindowTitle("Edit Purchase Order Draft #"+docnum);
            h.setWindowFlags(Qt::Window);
            h.exec();
        } else
            createPR_Details(index.siblingAtColumn(SIM_PR_NUM).data().toString());
        break;
    }
    case SIM_PO: {
        switch(index.column()) {
        //PO, created, supplier, ship to, ship by, status
        case SIM_PO_NUM:
        case SIM_PO_SUPPLIER:
        case SIM_PO_SHIP_BY:
        case SIM_PO_STATUS:
        {
            MainWindow::createPOD_Table(index.siblingAtColumn(0).data().toString());
            break;
        }
        }
        break;
    }
    case SIM_POD: {
        switch(index.column()) {
        case SIM_POD_ITEM_ID:
        case SIM_POD_ITEM_NUM:
        case SIM_POD_ITEM_NAME:
        {
            MainWindow::createItem_Details_Table(index.siblingAtColumn(SIM_POD_ITEM_ID).data().toString());
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
    case SIM_RR: {
        //Make more detailed
        QString rcrnum = index.siblingAtColumn(SIM_RR_NUM).data().toString();
        QString date = index.siblingAtColumn(SIM_RR_DATE).data().toString();
        MainWindow::createReceived_Details_Table(rcrnum, date);
        break;
    }
    case SIM_MR:
    {
        QString mrqnum = index.siblingAtColumn(SIM_MR_NUM).data().toString();
        QString date = index.siblingAtColumn(SIM_MR_DATE).data().toString();
        QString reqby= index.siblingAtColumn(SIM_MR_REQBY).data().toString();
        MainWindow::createRequested_Details_Table(mrqnum, date, reqby);
    }


    }
}

//Removing Searching for now. This will be one of the final features dealt with.
void MainWindow::on_SearchBox_textChanged(const QString &arg1)
{
    /*
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
    case SIM_POD:
    case SIM_PROJECT_DETAILS:
    {
        break;
    }
    }
    */
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


void MainWindow::on_ItemTable_customContextMenuRequested(const QPoint &pos)
{
    switch (currentTable) {
    case SIM_ITEMS:
    {
        // Is delete ever called for this? Almost definitely leaking memory.
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



void MainWindow::on_actionCreate_New_User_triggered()
{
    loginFlag = SIM_NEW;
    MainWindow::Login();
}

void MainWindow::on_actionChange_Current_User_triggered()
{
    loginFlag = SIM_LOGIN;
    MainWindow::Login();
}


void MainWindow::on_actionEdit_Current_Userdata_triggered()
{
    loginFlag = SIM_EDIT;
    MainWindow::Login();
}

void MainWindow::on_actionEdit_Company_Information_triggered()
{
    EditCompanyInfo e;
    e.setWindowTitle("Edit Company Information");
    e.exec();
}

void MainWindow::on_actionCreate_Purchase_Requisition_triggered()
{
    tflag = "0";
    docnum.clear();
    CreateDocument d;
    d.setWindowTitle("Create Purchase Requisition");
    d.setWindowFlags(Qt::Window);
    d.exec();
}

void MainWindow::on_actionCreate_Quotation_Request_triggered()
{
    tflag = "1";
    docnum.clear();
    CreateDocument d;
    d.setWindowTitle("Create Request for Quotation");
    d.setWindowFlags(Qt::Window);
    d.exec();
}

void MainWindow::on_actionCreate_Purchase_Order_triggered()
{
    tflag = "2";
    docnum.clear();
    CreateDocument d;
    d.setWindowTitle("Create Purchase Order");
    d.setWindowFlags(Qt::Window);
    d.exec();
}


void MainWindow::on_actionReceive_Items_triggered()
{
    tflag = "3";
    docnum.clear();
    CreateDocument d;
    d.setWindowTitle("Create Receiving Report");
    d.setWindowFlags(Qt::Window);
    d.exec();
}

void MainWindow::on_actionDistribute_Inventory_triggered()
{
    tflag = "4";
    docnum.clear();
    CreateDocument d;
    d.setWindowTitle("Create Material Requisition");
    d.setWindowFlags(Qt::Window);
    d.exec();
    ui->ItemTable->update();
}

void MainWindow::on_actionClean_Database_triggered()
{
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("DELETE FROM items WHERE id NOT IN ( "
        "SELECT DISTINCT prd.item_id FROM prd "
        "UNION SELECT DISTINCT qrd.item_id FROM qrd "
        "UNION SELECT DISTINCT pod.item_id FROM pod "
        "UNION SELECT DISTINCT rrd.item_id FROM rrd "
        "UNION SELECT DISTINCT mrd.item_id FROM mrd "
    ");");
    qDebug() << qry.lastError();
    simdb.close();
}
