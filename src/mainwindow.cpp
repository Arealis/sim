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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userlogin.h"
#include "createdocument.h"
#include "editcompanyinfo.h"
#include "global.h"

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

#include <QStringBuilder>

/*! A word on functions and database access:
 *
 *  If a function wants you to pass it a closed database,
 *  it will ask you for the database pointer (so that it can control
 *  the access.
 *  If a function wants you to pass it an open database,
 *  it will ask you for the query pointer (so that it can execute
 *  queries but is not able to control access
 */


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

/*  The problem:
 *      Each table has different columns, and the user needs to be able to search by any column.
 *      On top of that, unique tables need to be brought up for each column type.
 *
 *  Solution:
 *      Columns will have flags (of some type) so that SIM can identify them.
 *      Whenever a column is clicked, check the columnFlag and the tableFlag and
 *      the tableFlag and display the appropriate table in response.
 *
 *      When a search term is entered (ie, enter is pressed), do an SQL query on that column
 *
 *      ADDENDUM:
 *          The column 'flags' will simply be the strings that make up the header names.
 *          since the header names are set in the SQL query, if headers are consistently named
 *          across this entire cpp file, they can be used as flags.
 */

//! Using a proxy model to display data so the the QSqlQueryModel can have all the raw information stored within it
//! and the proxy model has the job of rendering it. That way no additional SQL queries have to be made and there is
//! a much lower CPU bottleneck. This will be a non-issue when using a DBMS that plans for simultaneous connections

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    simdb = QSqlDatabase::addDatabase("QSQLITE", "sim");
    simdb.setDatabaseName(qApp->applicationDirPath()+"/db/sim.db");
    qry = QSqlQuery(simdb);
    model = new QSqlQueryModel(this);
    headers = new QStringList;
    user = new User;
    //TODO: Set the modelQry here, and attempt to never reset the modelQry elsewhere in the program again. Only call update().

    ui->table->setModel(model);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_H),this,SLOT(goToHome()));
    ui->table->setContextMenuPolicy(Qt::CustomContextMenu);
    MainWindow::Login();
    MainWindow::on_actionItems_Table_Default_triggered();
}


bool MainWindow::checkPrivelage(TableFlag table, User *user)
{
    //Check whether the user has the correct permissions to create certain documents or not
    //NOTE: This could probably be made quicker by setting some permission flags at the login stage.
    //  (ie, defining bit flags for what these enums actually permit on a per table basis.).
    switch(table) {
    case PR: {
        if (user->privelage == Admin || user->privelage == InventoryManager || user->privelage == Mechanic)
            return true;
        break;
    }
    case QR: {
        if (user->privelage == Admin || user->privelage == Purchasing)
            return true;
        break;
    }
    case PO: {
        if (user->privelage== Admin || user->privelage== Purchasing)
            return true;
        break;
    }
    case RR: {
        if (user->privelage== Admin || user->privelage== InventoryManager)
            return true;
        break;
    }
    case MR: {
        if (user->privelage== Admin || user->privelage== InventoryManager || user->privelage== Mechanic)
            return true;
        break;
    }
    default:
        break;
    }

    return false;
}
void MainWindow::prepareBaseTable()
{
    ui->ExtraDetailsWidget->setHidden(1);
    ui->FooterWidget->setHidden(1);
    ui->tableLabel2->setHidden(1);
}
MainWindow::~MainWindow()
{
    delete user;
    delete headers;
    delete ui;
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
            QMessageBox message;
            message.setWindowTitle("No Previous Database Detected");
            message.setText("No sim.db file could be found. This is the file that stores all of the inventory and userdata of sim. If this is a fresh installation "
                "of SIM, then this is normal. A new database will be generated. If you already have an existing database file and are seeing this message, the database "
                "file was either deleted or moved. If you would like to restore your old database, simple replace "+qApp->applicationFilePath()+"/db/sim.db "
                "with your own file.");
            message.exec();
            MainWindow::createNewSIMDB();
        }

        simdb.open();
        QSqlQuery qry(simdb);
        qry.exec("SELECT COUNT(id) FROM userdata;");
        qry.next();
        simdb.close();
        if (qry.value(0).toInt() == 0)
            UserLogin userLogin("sim", user, LoginFlag::New, this);
        else
            UserLogin userLogin("sim", user, LoginFlag::Login, this);
    } else {
        qDebug("Could not open sim.db. It may not be in the correct location.");
    }

    if (user->id.isEmpty()) //Throws may not work depending on compiler, but we can't use a close because the ui may not be loaded yet
        throw std::invalid_argument("Valid username and password combo was not entered");
}

void MainWindow::createNewSIMDB()
{
    simdb.open();
    qry.exec("PRAGMA foreign_keys = 1;");
    qry.exec("CREATE TABLE userdata ("
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
    qry.exec("CREATE TABLE company ( "
                "name               TEXT "
                ",info              TEXT " //This is what shows up next to the logo on documents
                ",shipping_address  TEXT "
                ",billing_address   TEXT "
            ");");
    qry.exec("INSERT INTO company (name, info, shipping_address, billing_address) VALUES ('[No Company Name]', '[No Company Info]', '[No Shipping Address]', '[No Billing Address]');");
    qry.exec("CREATE TABLE suppliers ( "
                "id             INTEGER PRIMARY KEY "
                ",name          TEXT NOT NULL "
                ",address       TEXT "
                ",internal      TEXT "
            ");");
    qry.exec("CREATE TABLE projects ( " //This can be vehicles, machines, tasks, etc.
                "name       TEXT PRIMARY KEY " //Is is less confusing if project names are unique.
                ",desc      TEXT " //If further information about the project is needed
                ",isgroup   BOOL NOT NULL DEFAULT 0 " //Projects can also be groups of projects
                ",model     TEXT "
                ",serial    TEXT "
                ",notes     TEXT " //For huge amounts of information about the project
           ");");
    qry.exec("CREATE TABLE project_groups ( "
                "id         INTEGER PRIMARY KEY "
                ",name      REFERENCES projects (name) ON UPDATE CASCADE "
                ",project   REFERENCES projects (name) ON UPDATE CASCADE "
            ");");
    qry.exec("CREATE TABLE items ("
                "id     INTEGER PRIMARY KEY "
                ",num   TEXT "
                ",desc  TEXT NOT NULL "
                ",cat   TEXT DEFAULT 'Unknown' "
                ",qty   REAL NOT NULL DEFAULT 0 "
                ",unit  TEXT "  //Maybe this should be each by default
                ",shelf TEXT "	//Location of item in storage
                ",bin   TEXT "	//Location of item in storage
           ");");
           /*       A NOTE ABOUT UNITS AND CHILD ITEMS
            * If the units are not like NULL, EACH or EA, special operations can be performed on the item.
            * For example, if you buy a pack of 1000 screws, you may not want to always distribute all 1000
            * of those screws at once, so you can break up the pack of 1000 screws into individual screws.
            * A new item is created with the same item info, but has a different unit.
            * Now you can distribute 50 screws, and still have 950 remaining. Since there are so many different
            * types of units (there are 12 units in a dozen, but exactly how many units are in a box?),
            * the user is meant to break these up themselves. This could be a security vulnerability. */
    qry.exec("CREATE TABLE items_projects_linker ( "
                "id         INTEGER PRIMARY KEY "
                ",item_id   INTEGER REFERENCES items (id) ON UPDATE CASCADE "
                ",project   TEXT REFERENCES projects (name) ON UPDATE CASCADE "
            ");");
    qry.exec("CREATE TABLE relatives ("
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
    qry.exec("CREATE TABLE recurring_custom_details ("
                "id     INTEGER PRIMARY KEY "
                ",tflag INTEGER NOT NULL "
                ",name  TEXT NOT NULL "
            ");");
    qry.exec("CREATE TABLE custom_details ("
                "id     INTEGER PRIMARY KEY "
                ",tflag INTEGER NOT NULL "
                ",tnum  INTEGER NOT NULL "
                ",name  TEXT NOT NULL "
                ",value TEXT "
                ",flag  INTEGER DEFAULT 0 "
            ")");
    qry.exec("CREATE TABLE custom_expenses ("
                "id     INTEGER PRIMARY KEY "
                ",tflag INTEGER NOT NULL "
                ",tnum  INTEGER NOT NULL "
                ",name  TEXT "
                ",rate  REAL " //If the rate is 0, then the rate checkbox was not clicked
                ",value REAL NOT NULL "
            ");");
    qry.exec("CREATE TABLE default_notes ("
                "id     INTEGER PRIMARY KEY "
                ",tflag INTEGER NOT NULL"
                ",notes TEXT"
            ");");
    qry.exec("INSERT INTO default_notes (tflag, notes) VALUES "
            "(0, 'PR Default'), (1, 'QR Default'), (2, 'PO Default'), (3, 'RR Default'), (4, 'MR Default')"
            ";");
    /*                      CUSTOM / DEFAULT FIELDS ARE A LITTLE STRANGE
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
     *  4 = MR = material requisition
     * Custom Detail Flags (flag):
     *  0 = Not Editable
     *  1 = Editable
     *  2 = Internal Detail [This is not implemented] */
    qry.exec("CREATE TABLE pr ( "
                "num                            INTEGER PRIMARY KEY "
                ",date                          DATE "
                ",date_needed                   DATE "
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
     *      1 = New / Open
     *      2 = Partially Completed
     *      3 = Completed / Closed
     *      4 = Partially Completed (at least one rejected)
     *      5 = Completed (at least one rejected) */
    qry.exec("CREATE TABLE prd ( "
                "id                     INTEGER PRIMARY KEY "
                ",pr_num                INTEGER NOT NULL REFERENCES pr (num) ON UPDATE CASCADE "
                ",item_id               INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",qty                   REAL NOT NULL "
                ",qty_not_ordered       REAL "
                ",supplier_id           INTEGER REFERENCES suppliers (id) /*The suggested supplier*/ "
                ",expected_unit_price   REAL "
                ",taxable               BOOLEAN "
                ",total                 REAL "
                ",status                INTEGER NOT NULL DEFAULT 0 /*0=draft,1=open,2=QRSent,3=Ordered,4=Rejected,5=NotAvailable*/ "
            ");");
    qry.exec("CREATE TABLE qr ( "
                "num                INTEGER PRIMARY KEY "
                ",date              DATE "
                ",date_needed       DATE "
                ",requested_by      INTEGER NOT NULL REFERENCES userdata (id) "
                ",supplier_id       INTEGER NOT NULL REFERENCES supplier (id) "
                ",supplier_address  TEXT "
                ",supplier_internal TEXT "
                ",shipping_address  TEXT "
                ",billing_address   TEXT "
                ",notes             TEXT "
                ",status            INTEGER NOT NULL DEFAULT 0 /*0=draft, 1=open, 2=Closed (Quote Received), 3=Discarded*/ "
            ");");
    qry.exec("CREATE TABLE qrd ( "
                "id         INTEGER PRIMARY KEY "
                ",qr_num    INTEGER NOT NULL REFERENCES qr (num) "
                ",item_id   INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",qty       REAL NOT NULL "
                ",notes     TEXT "
                ",status    INTEGER NOT NULL DEFAULT 0 /*0=draft, 1=Open, 2=Quoted, 3=PriceNotAvailable, 4=Discarded*/"
            ");");
    qry.exec("CREATE TABLE prd_qrd_linker ("
                "prd_id INTEGER NOT NULL REFERENCES prd (id) "
                ",qrd_id INTEGER NOT NULL REFERENCES qrd (id) "
            ");");
    qry.exec("CREATE TABLE po ( "
                "num                            INTEGER PRIMARY KEY "
                ",invoice_num                   INTEGER "
                ",authorized_by                 INTEGER REFERENCES userdata (id) "
                ",date                          DATE "
                ",date_expected                 DATE "
                ",supplier_id                   INTEGER REFERENCES suppliers (id) NOT NULL "
                ",supplier_address              TEXT "
                ",supplier_internal             TEXT "
                ",shipping_address              TEXT "
                ",billing_address               TEXT "
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
                ",notes                         TEXT " //Any additional information or instructions
                ",status                        INTEGER NOT NULL DEFAULT 0 " // See below for status info
           ");");
            /*              status FLAGS ARE AS FOLLOWS
             * 0 = Draft................ The PO is still a draft.
             * 1 = Sent................. The PO has been sent out but the vendor has not accepted it yet.
             * 2 = Open................. The vendor has accepted the PO but it is not known whether the items have shipped yet.
             * 3 = Shipped.............. Items have been shipped, but none have been received. This is useful to note because of some vendor's FOB shipping policy.
             * 4 = Partially Received... Only some of the items from the PO have been been received.
             * 5 = Closed............... All items from the PO have been received.
             * 6 = Rejected............. The PO was not accepted by the vendor for some reason.
             * 7 = Forced Closed........ The PO was closed without all items being received. This may be useful in cases where the company has to cut their losses with a vendor.
             */
    qry.exec("CREATE TABLE pod ("
                "id                 INTEGER PRIMARY KEY "
                ",po_num            INTEGER REFERENCES po (num) ON UPDATE CASCADE "
                ",item_id           INTEGER REFERENCES items (id) ON UPDATE CASCADE NOT NULL "
                ",qty               REAL NOT NULL "
                ",qty_not_delivered REAL NOT NULL"
                ",unit_price        REAL NOT NULL "
                ",discount_rate     REAL " // This may not be shown, but may be useful internally
                ",discount          REAL " // Amount
                ",taxable           BOOL NOT NULL DEFAULT 1 " //Can tax be applied to this item?
                ",total             REAL NOT NULL " //qty * unit_price - discount
                ",status            INTEGER NOT NULL DEFAULT 0 " // See below for status info
                ",condition         TEXT " // This is a mirror of the condition in receiving report details
            ");");
            /*              POD status FLAGS
             *  0 = none received
             *  1 = partial quantity received
             *  2 = correct quantity received
             *  3 = too much received */
    qry.exec("CREATE TABLE prd_pod_linker ("
                "prd_id INTEGER NOT NULL REFERENCES prd (id) "
                ",pod_id INTEGER NOT NULL REFERENCES pod (id) "
            ");");
    qry.exec("CREATE TABLE rr ("
                "num                    INTEGER PRIMARY KEY "
                ",date                  DATE NOT NULL " //Date collected
                ",date_arrived          DATE NOT NULL "
                ",delivered_by          TEXT NOT NULL "
                ",inspected_by          INTEGER REFERENCES userdata (id) "
                ",po_num                INTEGER REFERENCES po (num) " //There may be no PO# in certain cases
                ",invoice_num           INTEGER REFERENCES po (invoice_num) "
                ",supplier_id           INTEGER REFERENCES suppliers (id) "
                ",supplier_address      TEXT "
                ",supplier_internal     TEXT "
                ",notes                 TEXT "
                ",status                INTEGER NOT NULL DEFAULT 0 "
            ");");
    qry.exec("CREATE TABLE rrd ("
                "id             INTEGER PRIMARY KEY "
                ",rr_num        INTEGER NOT NULL REFERENCES rr (num) ON UPDATE CASCADE "
                ",pod_id        INTEGER REFERENCES pod (id) ON UPDATE CASCADE "
                ",item_id       INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",qty           REAL NOT NULL " //This has to be checked against the PO
                ",unit          TEXT "
                ",condition     TEXT NOT NULL REFERENCES conditions (name) "
                ",notes         TEXT "
            ");");
    qry.exec("CREATE TABLE mr ("
                "num                INTEGER PRIMARY KEY "
                ",project           INTEGER REFERENCES projects (name) ON UPDATE CASCADE "
                ",date_requested    DATE NOT NULL "
                ",requested_by      TEXT NOT NULL "
                ",date_authorized   DATE "
                ",authorized_by     INTEGER REFERENCES userdata (id) "
                ",date_issued       DATE "
                ",issued_by         INTEGER REFERENCES userdata (id) "
                ",collected_by      TEXT "
                ",notes             TEXT "
            ");");
    qry.exec("CREATE TABLE mrd ( "
                "id         INTEGER PRIMARY KEY "
                ",mr_num    INTEGER NOT NULL REFERENCES mr (num) ON UPDATE CASCADE "
                ",item_id   INTEGER NOT NULL REFERENCES items (id) ON UPDATE CASCADE "
                ",qty       REAL NOT NULL "
            ");");
    /*                                  A WORD ABOUT DRAFTS
     *  Drafts will inhabit the bottom few numbers of POs, so if POs begin from 1001, drafts span the numbers
     *  1 - 999. This means that it is possible to run out of drafts if the users managing the database do not
     *  clear out drafts. This is intentional (a.) because it significantly less complex that including a separate
     *  table dedicated to only drafts and (b.) because it encourages the user to not have drafts sitting in SIM
     *  for a very long time. When drafts are cleared out, it creates space for the next draft to come in. */
    simdb.close();
}

void MainWindow::getHeaders() { //This function pulls the column (header) names from the QSqlQueryModel and inserts them into the headers QStringList in order
    headers->clear();
    ui->SearchForDropdown->clear();
    QString header;
    for (int column = 0; column < model->columnCount(); column++)
    {
        header = model->headerData(column, Qt::Horizontal).toString();
        ui->SearchForDropdown->addItem(header);
        headers->append(header);
    }
    return;
}

//Hide Columns variadic fuctions
//These function will also remove these headers from the search for combo box
inline void MainWindow::hideColumnsVariadic() {}

template <typename Current, typename ... Extras>
inline void MainWindow::hideColumnsVariadic(Current header, Extras ... moreHeaders)
{
    ui->table->hideColumn(headers->indexOf(header));
    ui->SearchForDropdown->removeItem(ui->SearchForDropdown->findText(header, Qt::MatchExactly));
    hideColumnsVariadic(moreHeaders...);
}

template <typename ... Headers>
inline void MainWindow::setHiddenColumns(Headers ... columnHeaders)
{
    //The reason this function has recently been emptied. Eventually this should be bypassed and removed.
    hideColumnsVariadic(columnHeaders...);
}

//The order of operations when creating a table will be explained here

/*
 * STEP 1:  Create the base table and set the headers.
 *      2:  Input the correct string into the query variable
 *      3:  Execute the query and set the model
 *      4:  hide the appropriate columns and fill out the search for comboBox
 */

void MainWindow::displayTable(QString title, QSqlDatabase database, QString query)
{
    ui->tableLabel->setText(title);
    ui->SearchBox->clear();

    database.open();
    qry.exec(query);
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    database.close();

    ui->tableLabel2->setHidden(1);
    ui->ExtraDetailsWidget->setHidden(1);
    ui->FooterWidget->setHidden(1);
    ui->tableLabel2->setHidden(1);

    for (int column = 0; column < model->columnCount(); column++)
        ui->table->showColumn(column);
    getHeaders();
    ui->table->update();

    qDebug() << title << qry.lastError();
}

/*  TODO: Additional functions needed:
 *          displayDetails()
 *          displayTotals()
 *          setDateRange()
 */

void MainWindow::on_SearchBox_returnPressed()
{
    if (!ui->SearchBox->text().isEmpty()) {
        //This query is dangerous, because the searchTerm is not escaped
        QString newQuery = "SELECT * FROM ("%query.chopped(1)%") WHERE `"%ui->SearchForDropdown->currentText()%"` LIKE "%escapeSql("%"%ui->SearchBox->text()%"%")%";";
        simdb.open();
        qry.exec(newQuery);
        model->setQuery(qry);
        while (model->canFetchMore()) {model->fetchMore();}
        simdb.close();
    } else {
        refresh();
    }
}

void MainWindow::refresh()
{
    simdb.open();
    qry.exec(query);
    model->setQuery(qry);
    while (model->canFetchMore()) { model->fetchMore(); }
    simdb.close();
    ui->table->update();
}

void MainWindow::on_actionItem_History_triggered() //Every transaction in the Inventory
{
    query = "SELECT prd.supplier_id, pr.requested_by, pr.date AS 'Date', 'Purchase Requisition' AS 'Document', prd.pr_num AS 'Doc#', userdata.name As 'Created by' "
    ",prd.qty AS 'Qty Reqd.', NULL AS 'Qty Quoted', NULL AS 'Qty Ordered', suppliers.name AS 'Supplier', pr.project AS 'Project', NULL AS 'Qty In', NULL AS 'Qty Out' "
    "FROM prd "
    "LEFT JOIN pr ON prd.pr_num = pr.num "
    "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
    "LEFT JOIN userdata ON userdata.id = pr.requested_by "
    "UNION "
    "SELECT qr.supplier_id, qr.requested_by, qr.date, 'Request for Quotation', qr.num, userdata.name, NULL , qrd.qty, NULL, suppliers.name, GROUP_CONCAT(pr.project), NULL, NULL "
    "FROM qrd "
    "LEFT JOIN qr ON qrd.qr_num = qr.num "
    "LEFT JOIN suppliers ON suppliers.id = qr.supplier_id "
    "LEFT JOIN userdata ON userdata.id = qr.requested_by "
    "LEFT JOIN prd_qrd_linker AS 'linker' ON linker.qrd_id = qrd.id "
    "LEFT JOIN prd ON prd.id = linker.prd_id "
    "LEFT JOIN pr ON pr.num = prd.pr_num "
    "GROUP BY qrd.id "
    "UNION "
    "SELECT po.supplier_id, po.authorized_by, po.date, 'Purchase Order', pod.po_num, userdata.name, NULL, NULL, pod.qty, suppliers.name, GROUP_CONCAT(pr.project), NULL, NULL "
    "FROM pod "
    "LEFT JOIN po ON pod.po_num = po.num "
    "LEFT JOIN suppliers ON suppliers.id = po.supplier_id "
    "LEFT JOIN userdata ON userdata.id = po.authorized_by "
    "LEFT JOIN prd_pod_linker AS 'linker' ON linker.pod_id = pod.id "
    "LEFT JOIN prd ON prd.id = linker.prd_id "
    "LEFT JOIN pr ON pr.num = prd.pr_num "
    "GROUP BY pod.id "
    "UNION "
    "SELECT rr.supplier_id, rr.inspected_by, rr.date, 'Receiving Report', rrd.rr_num, userdata.name, NULL, NULL, NULL, suppliers.name, GROUP_CONCAT(pr.project), rrd.qty, NULL "
    "FROM rrd "
    "LEFT JOIN rr ON rrd.rr_num = rr.num "
    "LEFT JOIN suppliers ON suppliers.id = rr.supplier_id "
    "LEFT JOIN userdata ON userdata.id = rr.inspected_by "
    "LEFT JOIN prd_pod_linker AS 'linker' ON linker.pod_id = rrd.pod_id "
    "LEFT JOIN prd ON prd.id = linker.prd_id "
    "LEFT JOIN pr ON pr.num = prd.pr_num "
    "GROUP BY rrd.id "
    "UNION "
    "SELECT NULL, mr.authorized_by, mr.date_authorized, 'Material Requisition', mrd.mr_num, userdata.name, NULL, NULL, NULL, NULL, mr.project, NULL, mrd.qty "
    "FROM mrd "
    "LEFT JOIN mr ON mrd.mr_num = mr.num "
    "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
    ";";

    displayTable("Inventory Movement History", simdb, query);

    setHiddenColumns("supplier_id", "requested_by");
}

void MainWindow::on_actionItems_Table_Default_triggered()
{
    query = "SELECT i.id AS 'item_id', i.num AS 'Item ID', i.desc AS 'Description', i.cat AS 'Category', i.qty AS 'Qty' "
            ",i.unit AS 'Unit', i.shelf AS 'Shelf', i.bin AS 'Bin', group_concat(DISTINCT ipl.project) AS 'Used For' "
            "FROM items AS 'i' "
            "LEFT JOIN items_projects_linker AS 'ipl' ON i.id = ipl.item_id "
            "GROUP BY i.id "
            "ORDER BY i.id DESC;";

    displayTable("Items", simdb, query);

    setHiddenColumns("item_id");
}

void MainWindow::createItem_Details_Table(QString itemId)
{
    query = QString("SELECT prd.supplier_id, pr.requested_by, pr.date AS 'Date', 'Purchase Requisition' AS 'Document', pr.num AS 'Doc#', userdata.name As 'Created by' "
    ",prd.qty AS 'Qty Reqd.', NULL AS 'Qty Quoted', NULL AS 'Qty Ordered', suppliers.name AS 'Supplier', pr.project AS 'Project', NULL AS 'Qty In', NULL AS 'Qty Out' "
    "FROM prd "
    "LEFT JOIN pr ON prd.pr_num = pr.num "
    "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
    "LEFT JOIN userdata ON userdata.id = pr.requested_by "
    "WHERE prd.item_id = %1 "
    "UNION "
    "SELECT qr.supplier_id, qr.requested_by, qr.date, 'Request for Quotation', qr.num, userdata.name, NULL , qrd.qty, NULL, suppliers.name, GROUP_CONCAT(pr.project), NULL, NULL "
    "FROM qrd "
    "LEFT JOIN qr ON qrd.qr_num = qr.num "
    "LEFT JOIN suppliers ON suppliers.id = qr.supplier_id "
    "LEFT JOIN userdata ON userdata.id = qr.requested_by "
    "LEFT JOIN prd_qrd_linker AS 'linker' ON linker.qrd_id = qrd.id "
    "LEFT JOIN prd ON prd.id = linker.prd_id "
    "LEFT JOIN pr ON pr.num = prd.pr_num "
    "WHERE qrd.item_id = %1 "
    "GROUP BY qrd.id "
    "UNION "
    "SELECT po.supplier_id, po.authorized_by, po.date, 'Purchase Order', pod.po_num, userdata.name, NULL, NULL, pod.qty, suppliers.name, GROUP_CONCAT(pr.project), NULL, NULL "
    "FROM pod "
    "LEFT JOIN po ON pod.po_num = po.num "
    "LEFT JOIN suppliers ON suppliers.id = po.supplier_id "
    "LEFT JOIN userdata ON userdata.id = po.authorized_by "
    "LEFT JOIN prd_pod_linker AS 'linker' ON linker.pod_id = pod.id "
    "LEFT JOIN prd ON prd.id = linker.prd_id "
    "LEFT JOIN pr ON pr.num = prd.pr_num "
    "WHERE pod.item_id = %1 "
    "GROUP BY pod.id "
    "UNION "
    "SELECT rr.supplier_id, rr.inspected_by, rr.date, 'Receiving Report', rrd.rr_num, userdata.name, NULL, NULL, NULL, suppliers.name, GROUP_CONCAT(pr.project), rrd.qty, NULL "
    "FROM rrd "
    "LEFT JOIN rr ON rrd.rr_num = rr.num "
    "LEFT JOIN suppliers ON suppliers.id = rr.supplier_id "
    "LEFT JOIN userdata ON userdata.id = rr.inspected_by "
    "LEFT JOIN prd_pod_linker AS 'linker' ON linker.pod_id = rrd.pod_id "
    "LEFT JOIN prd ON prd.id = linker.prd_id "
    "LEFT JOIN pr ON pr.num = prd.pr_num "
    "WHERE rrd.item_id = %1 "
    "GROUP BY rrd.id "
    "UNION "
    "SELECT NULL, mr.authorized_by, mr.date_authorized, 'Material Requisition', mrd.mr_num, userdata.name, NULL, NULL, NULL, NULL, mr.project, NULL, mrd.qty "
    "FROM mrd "
    "LEFT JOIN mr ON mrd.mr_num = mr.num "
    "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
    "WHERE mrd.item_id = %1 "
    ";").arg(itemId);
    //Should probably include more info in this query

    //NOTE: I don't like this very much.
    simdb.open();
    qry.exec("SELECT num, desc FROM items WHERE id = "%itemId%";");
    qry.next();
    simdb.close();
    QString itemName = qry.value(0).toString();
    QString itemDesc = qry.value(1).toString();

    displayTable("Movement History For "%itemName%" || "%itemDesc, simdb, query);
    qDebug() << qry.lastError();

    setHiddenColumns("supplier_id", "requested_by");
}

void MainWindow::on_actionProjects_triggered()
{
    query = "SELECT name AS 'Project', isgroup AS 'Group?', model as 'Model#', serial as 'Serial#', notes AS 'Notes' FROM projects;";

    displayTable("Projects", simdb, query);
}

void MainWindow::createProject_Details_Table(QString project)
{
    query = QString("SELECT prd.item_id, prd.supplier_id, pr.requested_by, userdata.name AS 'Creator', pr.date AS 'Date', 'PR' AS 'Doc', pr.num AS 'Doc#', items.num AS 'Item ID'"
        ", items.desc AS 'Description', items.unit AS 'Unit', prd.qty AS 'Qty Reqd.', NULL AS 'Qty Dist.', suppliers.name AS 'Supplier' "
        "FROM prd "
        "LEFT JOIN pr ON pr.num = prd.pr_num "
        "LEFT JOIN items ON items.id = prd.item_id "
        "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
        "LEFT JOIN userdata ON userdata.id = pr.requested_by "
        "WHERE pr.project = '%1' "
        "UNION "
        "SELECT mrd.item_id, NULL, mr.authorized_by, userdata.name, mr.date_authorized, 'MR', mr.num, items.num, items.desc, items.unit, NULL, mrd.qty, NULL "
        "FROM mrd "
        "LEFT JOIN mr ON mr.num = mrd.mr_num "
        "LEFT JOIN items ON items.id = mrd.item_id "
        "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
        "WHERE mr.project = '%1' "
        ";").arg(project);

    displayTable(project%" History", simdb, query);

    setHiddenColumns("item_id", "supplier_id", "requested_by");
}

void MainWindow::on_actionSuppliers_triggered()
{
    query = "SELECT id AS 'supplier_id', name AS 'Supplier' FROM suppliers;";

    displayTable("Suppliers", simdb, query);

    setHiddenColumns("supplier_id");
}

void MainWindow::createSuppliers_Details_Table(QString supplierId)
{
    query = QString("SELECT prd.item_id, pr.requested_by, pr.date AS 'Date', userdata.name AS 'Creator', 'PR' AS 'Doc', prd.pr_num AS 'Doc#', pr.project AS 'Project', items.num AS 'Item ID' "
        ", items.desc AS 'Description' , items.unit AS 'Unit', prd.qty AS 'Qty Reqd.', NULL AS 'Qty Ordered', NULL AS 'Qty Recd.', NULL As 'Unit Price', NULL AS 'Total' "
        "FROM prd "
        "LEFT JOIN pr ON pr.num = prd.pr_num "
        "LEFT JOIN items ON items.id = prd.item_id "
        "LEFT JOIN userdata ON userdata.id = pr.requested_by "
        "WHERE prd.supplier_id = %1 "
        "UNION "
        "SELECT pod.item_id, po.authorized_by, po.date, userdata.name, 'PO', pod.po_num, NULL, items.num, items.desc, items.unit, NULL, pod.qty, NULL, pod.unit_price, pod.total "
        "FROM pod "
        "LEFT JOIN po ON po.num = pod.po_num "
        "LEFT JOIN items ON items.id = pod.item_id "
        "LEFT JOIN userdata ON userdata.id = po.authorized_by "
        "WHERE po.supplier_id = %1 "
        "UNION "
        "SELECT rrd.item_id, rr.inspected_by, rr.date, userdata.name, 'RR', rrd.rr_num, NULL, items.num, items.desc, items.unit, NULL, NULL, rrd.qty "
        ", (SELECT pod.unit_price WHERE pod.item_id = rrd.item_id), (SELECT pod.unit_price WHERE pod.item_id = rrd.item_id) * rrd.qty "
        "FROM rrd "
        "LEFT JOIN rr ON rr.num = rrd.rr_num "
        "LEFT JOIN items ON items.id = rrd.item_id "
        "LEFT JOIN userdata ON userdata.id = rr.inspected_by "
        "LEFT JOIN pod ON pod.po_num = rr.po_num "
        "WHERE rr.supplier_id = %1 "
        ";").arg(supplierId);

    simdb.open();
    qry.exec("SELECT name FROM suppliers WHERE id = "%supplierId%";");
    qry.next();
    simdb.close();
    QString supplierName = qry.value(0).toString();

    displayTable("Transactions Involving "%supplierName, simdb, query);

    setHiddenColumns("item_id", "requested_by");
}

void MainWindow::on_actionPurchase_Requisitions_triggered()
{
    query = "SELECT pr.requested_by, userdata.name AS 'Requested By', pr.date AS 'Date Created', pr.date_needed AS 'Date Needed', pr.num AS 'PR#' "
        ",pr.project AS 'Project', group_concat(DISTINCT suppliers.id) AS 'supplier_id', group_concat(DISTINCT suppliers.name) AS 'Recommended Supplier(s)' "
        ",CASE pr.status "
            "WHEN 0 THEN 'Draft' "
            "WHEN 1 THEN 'Closed '"
            "WHEN 2 THEN 'Partial' "
            "WHEN 3 THEN 'Open' "
            "WHEN 4 THEN 'Partial (Item(s) Rejected)' "
            "WHEN 5 THEN 'Closed (Item(s) Rejected)' "
        "END AS 'Status' "
        "FROM pr "
        "LEFT JOIN prd ON prd.pr_num = pr.num "
        "LEFT JOIN userdata ON pr.requested_by = userdata.id "
        "LEFT JOIN suppliers ON suppliers.id = prd.supplier_id "
        "GROUP BY pr.num "
        "ORDER BY pr.num DESC "
    ";";

    displayTable("Purchase Requisitions", simdb, query);

    setHiddenColumns("requested_by", "supplier_id");
}

void MainWindow::createPR_Details(QString prnum)
{
    query = "SELECT prd.supplier_id, prd.item_id, suppliers.name AS 'Supplier', items.num AS 'Item ID' , items.desc AS 'Description'"
        ",items.cat AS 'Category', prd.qty AS 'Quantity', items.unit AS 'Unit', printf('%.2f', prd.expected_unit_price) AS 'Expected Price',  prd.total AS 'Total' "
        "FROM prd "
        "LEFT JOIN suppliers ON prd.supplier_id = suppliers.id "
        "LEFT JOIN items ON prd.item_id = items.id "
        "WHERE prd.pr_num = "%prnum%";";

    displayTable("Purchase Requisition #"%prnum, simdb, query);

    setHiddenColumns("supplier_id", "item_id");
}

void MainWindow::on_actionQuotation_Requests_triggered()
{
    query = "SELECT qr.requested_by, userdata.name AS 'Requested By', qr.date AS 'Date Created', qr.date_needed AS 'Date Needed', qr.num AS 'QR#' "
        ",qr.supplier_id, suppliers.name AS 'Supplier', qr.status AS 'Status' "
        "FROM qr "
        "LEFT JOIN userdata ON qr.requested_by = userdata.id "
        "LEFT JOIN suppliers ON qr.supplier_id = suppliers.id "
    ";";

    displayTable("Requests for Quotation", simdb, query);

    setHiddenColumns("requested_by", "supplier_id");
}

void MainWindow::createQR_Details(QString qrnum)
{
    query = "SELECT qrd.prd_id AS 'PR#', prd.project AS 'Needed For', prd.item_id, items.num AS 'Item ID', items.desc AS 'Description', items.cat AS 'Category', items.unit AS 'Unit' "
        ",qrd.qty AS 'Quantity', qrd.status AS 'Status', qrd.notes AS 'Notes' "
        "FROM qrd "
        "LEFT JOIN items ON items.id = qrd.item_id "
        "LEFT JOIN prd ON prd.id = qrd.prd_id "
        "WHERE qrd.qr_num = "%qrnum%";";

    displayTable("Request for Quotation #"%qrnum, simdb, query);

    setHiddenColumns("item_id");
}


void MainWindow::on_actionPurchase_Orders_triggered()
{
    query = "SELECT po.authorized_by, po.supplier_id, userdata.name AS 'Created By', po.date AS 'Date', po.num AS 'PO#' "
        ",suppliers.name AS 'Supplier', total AS 'Total Price' "
        ",CASE status "
            "WHEN 0 THEN 'DRAFT' "
            "WHEN 1 THEN 'Closed' "
            "WHEN 2 THEN 'Partial' "
            "WHEN 3 THEN 'Shipped' "
            "WHEN 5 THEN 'Sent' "
        "END as 'Status' "
        "FROM po "
        "LEFT JOIN userdata ON po.authorized_by = userdata.id "
        "LEFT JOIN suppliers ON suppliers.id = po.supplier_id "
    ";";

    displayTable("Purchase Orders", simdb, query);

    setHiddenColumns("authorized_by", "supplier_id");
}

void MainWindow::createPOD_Table(QString ponum)
{
    query = "SELECT po.supplier_id, po.authorized_by, po.date AS 'Date Created', po.date_expected AS 'Date Expected', userdata.name AS 'Created By' "
    ",suppliers.name AS 'Supplier', items.num AS 'Item Id', items.desc AS 'Description', pod.qty AS 'Qty', GROUP_CONCAT(pr.project) AS 'Used For' "
    "FROM pod "
        "LEFT JOIN po ON pod.po_num = po.num "
        "LEFT JOIN items ON items.id = pod.item_id "
        "LEFT JOIN suppliers ON suppliers.id = po.supplier_id "
        "LEFT JOIN userdata ON userdata.id = po.authorized_by "
        "LEFT JOIN prd_pod_linker AS 'linker' ON linker.pod_id = pod.id "
        "LEFT JOIN prd ON prd.id = linker.prd_id "
        "LEFT JOIN pr ON pr.num = prd.pr_num "
    "WHERE po.num = "%ponum%" "
    "GROUP BY pod.id"
    ";";

    displayTable("Purchase Order #"%ponum, simdb, query);

    setHiddenColumns("supplier_id", "authorized_by");
}

void MainWindow::on_actionReceived_triggered()
{
    query = "SELECT rr.supplier_id, rr.inspected_by, rr.date_arrived AS 'Arrival Date', userdata.name AS 'Inspected By', rr.date 'Collection Date', rr.num AS 'RR#', rr.po_num AS 'PO#'"
        ", suppliers.name AS 'Supplier'"
        ", CASE po.status "
            "WHEN 0 THEN 'Draft' "
            "WHEN 1 THEN 'Closed' "
            "WHEN 2 THEN 'Incomplete' "
        "END AS 'PO Status' "
        "FROM rr "
        "LEFT JOIN suppliers ON suppliers.id = rr.supplier_id "
        "LEFT JOIN po ON po.num = rr.po_num "
        "LEFT JOIN userdata ON userdata.name = rr.inspected_by"
        ";";

    displayTable("Receiving Reports", simdb, query);
}

void MainWindow::createReceived_Details_Table(QString rrnum)
{
    query = "SELECT trans.used_for, trans.item_id, po.supplier_id "
        ",items.item_num AS 'Item ID', items.item_name AS 'Description' "
        ",trans.qty_change AS 'Qty Recd.' "
        ",trans.po_num AS 'PO#', projects.project_name AS 'Used For' "
        ",suppliers.supplier_name AS 'Supplier' "
        "FROM trans "
        "LEFT JOIN po ON trans.po_num = po.po_num "
        "LEFT JOIN suppliers ON po.supplier_id = suppliers.supplier_id "
        "LEFT JOIN items ON items.item_id = trans.item_id "
        "LEFT JOIN projects ON projects.project_id = trans.used_for ";

    displayTable("Receiving Report #"%rrnum, simdb, query);
}

void MainWindow::on_actionRequested_Items_triggered()
{
    //This query is completely incorrect
    query = "SELECT mr.authorized_by, userdata.name AS 'Authorized By', mr.date_authorized AS 'Date', mr.num AS 'MR#'"
        ", mr.requested_by AS 'Requested By', mr.collected_by AS 'Collected By', group_concat(DISTINCT mrd.project) AS 'Project(s)' "
        "FROM mr "
        "LEFT JOIN userdata ON userdata.id = mr.authorized_by "
        "LEFT JOIN mrd ON mrd.mr_num = mr.num "
        "GROUP BY mr.num "
        "ORDER BY mr.num "
        ";";

    displayTable("Material Requisitions", simdb, query);
}

void MainWindow::createRequested_Details_Table(QString mrnum)
{
    //This query is completely incorrect
    query = "SELECT trans.used_for, trans.item_id "
        ", items.item_num AS 'Item ID', items.item_name AS 'Description', projects.project_name AS 'Used For' "
        "FROM trans "
        "LEFT JOIN items ON items.item_id = trans.item_id "
        "LEFT JOIN projects ON projects.project_id = trans.used_for ";

    displayTable("Material Requisition #"%mrnum, simdb, query);
}

void MainWindow::on_SearchForDropdown_currentIndexChanged(int index)
{
    if (!ui->SearchBox->text().isEmpty())
        on_SearchBox_returnPressed();
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

inline QString MainWindow::siblingAtHeader(const QModelIndex &index, QString headerName) //returns the index of the sibling column that has the specified header string
{
    //Just a convenience function so I don't have to type this out every time
    return index.siblingAtColumn(headers->indexOf(headerName)).data().toString();
}

void MainWindow::on_table_doubleClicked(const QModelIndex &index)
{
    // First:   Identify the column flag
    // Second:  If necessary, identify any special tables that would require a different columnFlag
    QString columnName = headers->at(index.column());
    switch(columnFlagsMap.at(columnName)) {
    case ColumnFlags::ItemId:
    case ColumnFlags::ItemNum:
    case ColumnFlags::ItemUnit:
    case ColumnFlags::ItemDesc: {
        createItem_Details_Table(siblingAtHeader(index, "item_id"));
        break;
    }
    case ColumnFlags::SupplierId:
    case ColumnFlags::SupplierName: {
        createSuppliers_Details_Table(siblingAtHeader(index, "supplier_id"));
        break;
    }
    case ColumnFlags::Project: {
        if (headers->indexOf("Project") < 0)
            createProject_Details_Table(siblingAtHeader(index, "Used For"));
        else
            createProject_Details_Table(siblingAtHeader(index, "Project"));
        break;
    }
    case ColumnFlags::ItemCat: {
        ui->SearchBox->setText(index.data().toString());
        ui->SearchForDropdown->setCurrentIndex(ui->SearchForDropdown->findText("Category"));
        break;
    }
    case ColumnFlags::ItemBin:
    case ColumnFlags::ItemShelf: {
        //Something needs to be done here after locations are reformed
        break;
    }
    case ColumnFlags::DocNum:
    case ColumnFlags::DocType: {
        switch(tableFlagsMap.at(siblingAtHeader(index, headers->contains("Document") ? "Document" : "Doc"))) {
        case PR: {
            if (siblingAtHeader(index, "Doc#").toInt() < 10000)
                CreateDocument doc(PR, siblingAtHeader(index, "Doc#"), user, "sim", this);
            else
                createPR_Details(siblingAtHeader(index, "Doc#"));
            break;
        }
        case QR: {
            if (siblingAtHeader(index, "Doc#").toInt() < 10000)
                CreateDocument doc(QR, siblingAtHeader(index, "Doc#"), user, "sim", this);
            else
                createQR_Details(siblingAtHeader(index, "Doc#"));
            break;
        }
        case PO: {
            if (siblingAtHeader(index, "Doc#").toInt() < 10000)
                CreateDocument doc(PO, siblingAtHeader(index, "Doc#"), user, "sim", this);
            else
                createPOD_Table(siblingAtHeader(index, "Doc#"));
            break;
        }
        case RR: {
            if (siblingAtHeader(index, "Doc#").toInt() < 10000)
                CreateDocument doc(RR, siblingAtHeader(index, "Doc#"), user, "sim", this);
            else
                createReceived_Details_Table(siblingAtHeader(index, "Doc#"));
            break;
        }
        case MR: {
            if (siblingAtHeader(index, "Doc#").toInt() < 10000)
                CreateDocument doc(MR, siblingAtHeader(index, "Doc#"), user, "sim", this);
            else
                createRequested_Details_Table(siblingAtHeader(index, "Doc#"));
            break;
        }
        default:
            qDebug() << "The document type could not be inferred.";
            break;
        }
        break;
    }
    //If I can get docnum and doctype workin right, I should be able to get rid of these 5 extra cases (and their enums)
    case ColumnFlags::PRNum : {
        if (siblingAtHeader(index, "Status") == "Draft")
            CreateDocument doc(PR, index.data().toString(), user, "sim", this);
        else
            createPR_Details(index.data().toString());
        break;
    }
    case ColumnFlags::QRNum: {
        if (siblingAtHeader(index, "Status") == "0")
            CreateDocument doc(QR, index.data().toString(), user, "sim", this);
        else
            createQR_Details(index.data().toString());
        break;
    }
    case ColumnFlags::PONum: {
        if (siblingAtHeader(index, "Status") == "DRAFT")
            CreateDocument doc(PO, index.data().toString(), user, "sim", this);
        else
            createPOD_Table(index.data().toString());
        break;
    }
    case ColumnFlags::RRNum: {
        //This function doesn't exist yet
        //createRR_Details(index.data().toString());
        break;
    }
    case ColumnFlags::MRNum: {
        //This function doesn't exist yet
        //createMR_Details(index.data().toString());
        break;
    }
    case ColumnFlags::CreatorName: {
        //Show everything done by this Creator
        break;
    }
    default:
        break;
    }
}

void MainWindow::on_ClearSearchButton_clicked()
{
    ui->SearchBox->clear();
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
        << QString("<title>%1</title>\n").arg("Current Table is: " + ui->tableLabel->text())
        << "</head>\n"
        "<body bgcolor=#ffffff link=#5000A0>\n"
        "<table border=1 cellspacing=0 cellpadding=2>\n";

    //Headers
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int c = 0; c < col; c++)
    {
        if (!ui->table->isColumnHidden(c))
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
            if(!ui->table->isColumnHidden(c))
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


void MainWindow::on_table_customContextMenuRequested(const QPoint &pos)
{
    //This is a good idea, but is poorly implemented. It should be more similar to the double click actions.

    /*
    switch (currentTable) {
    case PR:
    {
        // This is definitely a memory problem.
        QMenu *m = new QMenu(this);
        QAction *addToPO = new QAction(this);
        QAction *addToPR = new QAction(this);
        QAction *addToDist = new QAction(this);
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

        m->popup(ui->table->viewport()->mapToGlobal(pos));
        break;
    }
    default:
        break;
    }
    */
}

//The following are great ideas that haven't been implemented yet. It seems like it wil probably be the ifnal feature
void MainWindow::addItemToPO()
{

}
void MainWindow::addItemToPR()
{

}
void MainWindow::addItemToDist()
{

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
    UserLogin userLogin("sim", user, LoginFlag::New, this);
}

void MainWindow::on_actionChange_Current_User_triggered()
{
    UserLogin userLogin("sim", user, LoginFlag::Login, this);
}

void MainWindow::on_actionEdit_Current_Userdata_triggered()
{
    UserLogin userLogin("sim", user, LoginFlag::Edit, this);
}

void MainWindow::on_actionEdit_Company_Information_triggered()
{
    EditCompanyInfo e(this);
}

void MainWindow::on_actionCreate_Purchase_Requisition_triggered()
{
    if (checkPrivelage(PR, user))
        CreateDocument doc(PR, NULL, user, "sim", this);
        //Throw error message here.
}

void MainWindow::on_actionCreate_Quotation_Request_triggered()
{
    if (checkPrivelage(QR, user))
        CreateDocument doc(QR, NULL, user, "sim", this);
    //Throw error message here.
}

void MainWindow::on_actionCreate_Purchase_Order_triggered()
{
    if (checkPrivelage(PO, user))
        CreateDocument doc(PO, NULL, user, "sim", this);
        //Throw error message here.
}

void MainWindow::on_actionReceive_Items_triggered()
{
    if (checkPrivelage(RR, user))
        CreateDocument doc(RR, NULL, user, "sim", this);
        //Throw error message here.
}

void MainWindow::on_actionDistribute_Inventory_triggered()
{
    if (checkPrivelage(MR, user)) {
        if (user->privelage == Mechanic || user->privelage == Admin)
            CreateDocument(MR, NULL, user, "sim", this, true);
        else
            CreateDocument(MR, NULL, user, "sim", this, false);
    }
    //Throw error message here.
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
    simdb.close();
    refresh();
}
