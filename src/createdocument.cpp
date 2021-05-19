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

#include "createdocument.h"
#include "ui_createdocument.h"
#include "global.h"
#include "mainwindow.h"

#include <QtGlobal>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QCompleter>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStringBuilder>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>

#include <QDebug>
#include <QSqlError>

/*! A BRIEF OVERVIEW OF WHAT THIS SOURCE FILE DOES
 *
 *  1. Initialize an empty document. This is a document that only has
 *      default and null values.
 *  2. If this is an existing document [1]:
 *      a. Populate the table
 *      b. Populate the document info.
 *  3. Perform the appropriate action when one of the save buttons at
 *      the bottom is pressed. SIM will verify that all important
 *      fields are filled out before it allows you to write to the
 *      database.
 *
 *  [1] We populate the table first and then the document because of
 *  problems that can occur if the table tries to update the text of
 *  a Subtotal Label that does not exist.
 */

// ========================================
// == CLASS CONSTRUCTORS AND DESTRUCTORS ==
// ========================================

CreateDocument::CreateDocument(TableFlag tableFlag, QString docNum, User *userData, QString connectionName, QWidget *parent, bool authorizer) :
    QDialog(parent),
    ui(new Ui::CreateDocument)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    db = QSqlDatabase::database(connectionName, false);
    docnum = docNum;
    user = userData;
    tFlag = tableFlag;
    authorized = authorizer;
    QSqlQuery qry(db);

    customDetailCount = 0;
    addDetailButtonRow = 1; // The row in the gridLayout where the addCustom button lives. This will always be the final populated row.

    //      SHARED DOCUMENT INFO
    // Logo
    ui->logo->setPixmap(qApp->applicationDirPath()+"/logo/logo.png");
    ui->logo->setPixmap(ui->logo->pixmap().scaledToHeight(75));
    // Company Info
    db.open();
    qry.exec("SELECT name, info FROM company;");
    qry.next();
    db.close();
    ui->companyName->setText(qry.value(0).toString());
    ui->companyInfo->setText(qry.value(1).toString());

    // Although all documents use items, not all tables use the same item model.

    //      DOCUMENT SPECIFIC INFO
    switch(tFlag) {
    case PR: {
        //Titles
        setWindowTitle("Create Purchase Requisition");
        ui->doctype->setText("Purchase Requisition");
        docname = "pr";

        //Shared Read-Only Models (For Completers)
        initReadOnlyModel(ModelFlag::Item, db, this);
        initReadOnlyModel(ModelFlag::Supplier, db, this);

        //Create Table
        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Internal Details
        QLabel *projectLabel = new QLabel("Project", ui->internalWidget);
        QComboBox *project = initComboBox(initReadWriteModel(ModelFlag::Project, db, ui->internalWidget), ui->internalWidget);
        project->setObjectName("project");
        project->lineEdit()->setPlaceholderText("Select Project");

        ui->internalDetailsGrid->addWidget(projectLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(project, 0, 1);
        // May need a function to visually indicate whether or not a project exists when editing is finished.

        QLabel *neededByLabel = new QLabel("Needed by", ui->internalWidget);
        QDateTimeEdit *neededBy = initDateTimeEdit(ui->internalWidget);
        ui->internalDetailsGrid->addWidget(neededByLabel, 1, 0);
        ui->internalDetailsGrid->addWidget(neededBy, 1, 1);

        initTotals();
        break;
    }
    case QR: {
        //Titles
        setWindowTitle("Create Request for Quotation");
        ui->doctype->setText("Request for Quotation");
        docname = "qr";

        //Shared Read-Only Models (For Completers)
        QSqlQueryModel *itemModel = new QSqlQueryModel(this);
        itemModel->setObjectName(QString::number(ModelFlag::Item));
        db.open();
        itemModel->setQuery("SELECT i.id, i.num, i.desc, i.cat, i.unit, prd.id FROM prd JOIN items AS 'i' ON i.id = prd.item_id WHERE prd.status = 1 OR prd.status = 2;", db);
        while (itemModel->canFetchMore()) { itemModel->fetchMore(); }
        db.close();

        //Table
        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Info Boxes
        ui->infoLayout->addWidget(initDynamicInfoWidget(ui->info, BillingAddress, db));
        ui->infoLayout->addWidget(initDynamicInfoWidget(ui->info, ShippingAddress, db));
        ui->infoLayout->addWidget(initDynamicInfoWidget(ui->info, Supplier, db));

        //Internal Details
        QLabel *neededByLabel = new QLabel("Needed by", ui->internalWidget);
        QDateTimeEdit *neededBy = initDateTimeEdit(ui->internalWidget);
        ui->internalDetailsGrid->addWidget(neededByLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(neededBy, 0, 1);

        //QRs can only have items that exist in PRs
        break;
    }
    case PO: {
        //Titles
        setWindowTitle("Create Purchase Order");
        ui->doctype->setText("Purchase Order");
        docname = "po";

        //Shared Read-Only Models (For Completers)
        QSqlQueryModel *itemModel = new QSqlQueryModel(this);
        itemModel->setObjectName(QString::number(ModelFlag::Item));
        db.open();
        itemModel->setQuery("SELECT i.id, i.num, i.desc, i.cat, i.unit FROM prd JOIN items AS 'i' ON i.id = prd.item_id WHERE prd.status = 1;", db);
        while (itemModel->canFetchMore()) { itemModel->fetchMore(); }
        db.close();

        //Table
        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Info Boxes
        ui->infoLayout->addWidget(initDynamicInfoWidget(ui->info, BillingAddress, db));
        ui->infoLayout->addWidget(initDynamicInfoWidget(ui->info, ShippingAddress, db));
        ui->infoLayout->addWidget(initDynamicInfoWidget(ui->info, Supplier, db));

        //Internal Details
        QLabel *neededByLabel = new QLabel("Expected by:", ui->internalWidget);
        QDateTimeEdit *neededBy = initDateTimeEdit(ui->internalWidget);
        ui->internalDetailsGrid->addWidget(neededByLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(neededBy, 0, 1);

        QLabel *invoiceLabel = new QLabel("Invoice #", ui->internalWidget);
        QLineEdit *invoice = new QLineEdit(ui->internalWidget);
        invoice->setObjectName("invoice");
        ui->internalDetailsGrid->addWidget(invoiceLabel, 1, 0);
        ui->internalDetailsGrid->addWidget(invoice, 1, 1);

        initTotals();
        break;
    }
    case RR: {
        //Titles
        setWindowTitle("Create Receiving Report");
        ui->doctype->setText("Receiving Report");
        docname = "rr";

        //Shared Read-Only Models (For Completers)
        initReadOnlyModel(ModelFlag::Item, db, this);

        //Table
        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Info Widgets
        ui->infoLayout->addWidget(initDynamicInfoWidget(ui->info, Supplier, db));

        //Internal Details
        QLabel *arrivalDateLabel = new QLabel("Arrival Date:", ui->internalWidget);
        QDateTimeEdit *arrivalDate = initDateTimeEdit(ui->internalWidget);
        ui->internalDetailsGrid->addWidget(arrivalDateLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(arrivalDate, 0, 1);

        QLabel *deliveredByLabel = new QLabel("Delivered by:", ui->internalWidget);
        QLineEdit *deliveredBy = new QLineEdit(ui->internalWidget);
        deliveredBy->setObjectName("delivered_by");
        ui->internalDetailsGrid->addWidget(deliveredByLabel, 1, 0);
        ui->internalDetailsGrid->addWidget(deliveredBy, 1, 1);

        QStandardItemModel *poModel = new QStandardItemModel(ui->internalWidget);
        poModel->setObjectName("poModel");
        db.open();
        qry.exec("SELECT num FROM po WHERE status > 0 AND status < 5;");
        for (int i = 0; qry.next(); i++) {
            poModel->setItem(i, 0, new QStandardItem(qry.value(0).toString()));
        }
        db.close();
        QLabel *poComboBoxLabel = new QLabel("PO#", ui->internalWidget);
        QComboBox *poComboBox = initComboBox(poModel, ui->info);
        poComboBox->setObjectName("po");
        poComboBox->lineEdit()->setPlaceholderText("Select PO#");
        ui->internalDetailsGrid->addWidget(poComboBoxLabel, 2, 0);
        ui->internalDetailsGrid->addWidget(poComboBox, 2, 1);

        QLabel *invoiceLabel = new QLabel("Invoice", ui->info);
        QLineEdit *invoice = new QLineEdit(ui->info);
        invoice->setObjectName("invoice");
        invoice->setPlaceholderText("Select Invoice#");
        ui->internalDetailsGrid->addWidget(invoiceLabel, 3, 0);
        ui->internalDetailsGrid->addWidget(invoice, 3, 1);

        //Connections
        connect(poComboBox, &QComboBox::activated, this, [=] ()
        {
            clear();
            QString doc = poComboBox->currentText();
            db.open();
            fetchDetails(qry, doc, PO);
            insertRecurringCustomDetails(qry, &addDetailButtonRow);
            table->fetchRows(qry, doc, PO);
            db.close();
        });
        //TODO: Add invoice number functionality (I have not yet thought about how to allow for the selection of invoices that can occasionally have duplicate values).
        //TODO: Items should be able to be added to this document, but not removed.
        break;
    }
    case MR: {
        //Titles
        setWindowTitle("Create Material Requisition");
        ui->doctype->setText("Material Requisition");
        docname = "mr";

        //Shared Read-Only Models (For Completers)
        initReadOnlyModel(ModelFlag::Item, db, this); //This is good enough but could be better. The model should be different depending on whether the user is authorized or not.

        //Table
        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Internal Details
        QLabel *projectLabel = new QLabel("Project", this);
        QComboBox *project = initComboBox(initReadWriteModel(ModelFlag::Project, db, this), ui->internalWidget);
        project->setObjectName("project");
        project->lineEdit()->setPlaceholderText("Select Project");
        ui->internalDetailsGrid->addWidget(projectLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(project, 0, 1);

        QLabel *requestedByLabel = new QLabel("Requested by:", this);
        QLineEdit *requestedBy = new QLineEdit(this);
        requestedBy->setObjectName("requested_by");
        ui->internalDetailsGrid->addWidget(requestedByLabel, 1, 0);
        ui->internalDetailsGrid->addWidget(requestedBy, 1, 1);

        QLabel *collectedByLabel = new QLabel("Collected by:", this);
        QLineEdit *collectedBy = new QLineEdit(this);
        collectedBy->setObjectName("collected_by");
        ui->internalDetailsGrid->addWidget(collectedByLabel, 2, 0);
        ui->internalDetailsGrid->addWidget(collectedBy, 2, 1);

        if (!authorized) {
            project->setDisabled(true);
            requestedBy->setDisabled(true);
            collectedBy->setDisabled(true);
            //TODO: If the user it not authorized to edit, they should only be allowed to change quantities, and to swap items out for related items. Nothing else should be allowed.
        }
        break;
    }
    default:
        break;
    }

    //      ALL DOCUMENTS ARE NOW INITIALIZED. IT IS TIME TO FETCH INFO AND POPULATE.
    if (docnum.isEmpty()) //If New Draft
    {
        ui->docNumber->setText("DRAFT");
        ui->update->hide();
        ui->deleteDraft->hide();
        db.open();
        fetchDetails(qry, docnum);
        insertRecurringCustomDetails(qry, &addDetailButtonRow);
        db.close();
    }
    else if (docnum.toInt() < 10000) //If old draft
    {
        ui->docNumber->setText(docnum);
        ui->doctype->setText("DRAFT "%ui->doctype->text());
        ui->update->hide();
        db.open();
        table->fetchRows(qry,  docnum);
        fetchDetails(qry, docnum);
        fetchCustomDetails(qry, true);
        if (tFlag == PR || tFlag == PO)
            fetchTotals(qry);
        db.close();
    }
    else //If finalized document ----- I'm starting to think that finalized documents should use something different.
    {
        ui->docNumber->setText(docnum);
        ui->saveDraft->hide();
        ui->save->hide();
        ui->deleteDraft->hide();
        db.open();
        fetchDetails(qry, docnum);
        fetchCustomDetails(qry, false);
        table->fetchRows(qry, docnum);
        if (tFlag == PR || tFlag == PO)
            fetchTotals(qry);
        db.close();
        //I need to update the custom fields functions. There should be one to insert deletable custom fields, and one to insert constant custom fields.
    }

    //      EXTRA: Setting up the confirmation window when selecting delete
    QMessageBox *confirm = new QMessageBox(this);
    confirm->setText("Are you sure you would like to delete this document?");
    confirm->addButton("Confirm", QMessageBox::AcceptRole);
    confirm->addButton("Cancel", QMessageBox::RejectRole);
    confirm->setWindowTitle("Delete?");
    connect(ui->deleteDraft, &QPushButton::clicked, confirm, &QMessageBox::exec);
    connect(confirm, &QMessageBox::rejected, confirm, &QMessageBox::close);
    connect(confirm, &QMessageBox::accepted, confirm, [this, qry] () {
        db.open();
        CreateDocument::DeleteDocument(qry, docnum);
        db.close();
        CreateDocument::close();
    });

    exec();
}

ResizableTable::ResizableTable(TableFlag tableFlag, CreateDocument *parent) :
    QTableWidget(parent)
{
    // Initialize classes and variables
    parentDoc = parent;
    tflag = tableFlag;

    // Set up table headers and meta-info
    setRowCount(0);
    initializeColumns();
    setColumnCount(colNames.count());
    setHorizontalHeaderLabels(colNames);

    // Insert blank row for data entry
    appendRow();

    // Connect signals and slots
    connect(this, &ResizableTable::cellChanged, this,
            [=] (int row, int column)
    {
        if (row == finalRow && !item(row,column)->text().isEmpty())
            appendRow();
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    menu = new QMenu(this);
    deleteRow = new QAction(this);
    deleteRow->setText("Delete Row");
    menu->addAction(deleteRow);
    connect(this, &ResizableTable::customContextMenuRequested, this, [=] (const QPoint &pos) { ResizableTable::onCustomContextMenuRequested(pos); });
}

CreateDocument::~CreateDocument()
{
    delete ui;
    static_cast<MainWindow*>(parent())->refresh();
}

//  ====================================================
//  == FUNCTIONS FOR INITIALIZING THE RESIZABLE TABLE ==
//  ====================================================

//  RATIONALE - ResizableTable was subclassed from QTableWidget because in order to allow for have columns that would
//              resize themselves according to fixed ratios between their widths were not possible otherwise. This is the
//              reason that resizeEvent() is reimplemented. Also notable is that the ResizableTable is created before any
//              other widget in the document because changing many widgets will also change the table. In other words, the
//              widgets rely on the existance of the table, not the other way around.

void ResizableTable::initializeColumns()
{
    switch (tflag) {
    case PR: {
        colNames = {"id", "Item ID", "Description", "Category", "Unit", "Qty", "sid", "Recommended Supplier", "Tax?", "Expected Unit Price", "Total", "Status"};
        cid.supplierId = 6;
        cid.supplier = 7;
        cid.taxable = 8;
        cid.unitPrice = 9;
        cid.total = 10;
        cid.status = 11;
        break;
    }
    case QR: {
        colNames = {"id", "Item ID", "Description", "Category", "Unit", "Qty", "Notes", "Status", "prd.id"};
        cid.notes = 6;
        cid.status = 7;
        cid.prdId = 8;
        break;
    }
    case PO: {
        colNames = {"id", "Item ID", "Description", "Category", "Unit", "Qty", "Unit Price", "Tax?", "Total", "Status"};
        cid.unitPrice = 6;
        cid.taxable = 7;
        cid.total = 8;
        cid.status = 9;
        break;
    }
    case RR: {
        colNames = {"id", "Item ID", "Description", "Category", "Unit", "Qty", "Condition"};
        cid.condition = 6;
        break;
    }
    case MR: {
        colNames = {"id", "Item ID", "Description", "Category", "Unit", "Qty"};
        break;
    }
    default:
        break;
    }
}

void ResizableTable::onCustomContextMenuRequested(const QPoint &pos)
{
    menu->popup(viewport()->mapToGlobal(pos));
    connect(deleteRow, &QAction::triggered,
            [=] ()
            {
                removeRow(currentRow());
                finalRow--;
            });
}

//  =========================================================
//  == FUNCTIONS FOR POPULATING THE RESIZABLE TABLE WIDGET ==
//  =========================================================

void ResizableTable::resizeEvent(QResizeEvent *event) //This is called multiple times. Slow.
{
    int w = width();
    //Resizing windows based on tFlag
    switch (tflag) {
    case PR: {
        //{"id", "Item ID", "Description", "Qty", "Unit","sid", "Recommended Supplier", "taxable", "Expected Price", "total", "Status"};
        w = w / 22;
        setColumnWidth(cid.itemNum  ,w*3);
        setColumnWidth(cid.itemDesc ,w*5);
        setColumnWidth(cid.cat      ,w*2);
        setColumnWidth(cid.qty      ,w*1);
        setColumnWidth(cid.unit     ,w*2);
        setColumnWidth(cid.supplier ,w*3);
        setColumnWidth(cid.taxable  ,w*1);
        setColumnWidth(cid.unitPrice,w*2);
        setColumnWidth(cid.total    ,w*2);
        break;
    }
    case QR: {
        w = w / 19;
        setColumnWidth(cid.itemNum  ,w*3);
        setColumnWidth(cid.itemDesc ,w*5);
        setColumnWidth(cid.cat      ,w*2);
        setColumnWidth(cid.qty      ,w*1);
        setColumnWidth(cid.unit     ,w*2);
        setColumnWidth(cid.notes    ,w*5);
        break;
    }
    }
    horizontalHeader()->setStretchLastSection(true);
}

void ResizableTable::appendRow()
{
    finalRow = rowCount();
    setRowCount(rowCount()+1);
    for (int i = 0; i < columnCount(); i++)
    {
        setItem(finalRow,i, new QTableWidgetItem());
    }

    initLineEdit(finalRow, Item);
    setColumnHidden(cid.itemId,true);

    switch (tflag) {
    case PR: {
        initLineEdit(finalRow, Supplier);
        initTaxCheckBox(finalRow, cid.taxable);
        initLineEdit(finalRow, Disabled, cid.total);
        setColumnHidden(cid.supplierId, true);
        setColumnHidden(cid.status, true);
        break;
    }
    case QR: {
        setColumnHidden(cid.status, true);
        setColumnHidden(cid.prdId, true);
        break;
    }
    case PO: {
        setColumnHidden(cid.status, true);
        break;
    }
    default:
        break;
    }
}

void ResizableTable::fetchRows(QSqlQuery qry, QString docnum, TableFlag modifier)
{
    //Creating the query
    switch (tflag) {
    case PR: {
        qry.exec("SELECT prd.item_id, items.num, items.desc, items.cat, items.unit, prd.qty, prd.supplier_id, suppliers.name, prd.taxable, prd.expected_unit_price, prd.total, prd.status "
        "FROM prd "
        "LEFT JOIN items ON items.id = prd.item_id "
        "LEFT JOIN suppliers ON prd.supplier_id = suppliers.id "
        "WHERE prd.pr_num = "%docnum%";");
        break;
    }
    case QR: {
        qry.exec("SELECT qrd.item_id, items.num, items.desc, items.cat, items.unit, qrd.qty, qrd.notes, qrd.status, prd.id "
            "FROM qrd "
            "JOIN items ON items.id = qrd.item_id "
            "LEFT JOIN prd ON prd.id = qrd.prd_id "
            "WHERE qrd.qr_num = "%docnum%";");
        break;
    }
    case PO: {
        qry.exec("SELECT pod.item_id, items.num, items.desc, items.cat, items.unit, pod.qty, pod.unit_price, pod.taxable, pod.total, pod.status "
            "FROM pod "
            "LEFT JOIN items ON items.id = pod.item_id "
            "WHERE pod.po_num = "%docnum%";");
        break;
    }
    case RR: {
        if (modifier == PO) {
            qry.exec("SELECT pod.id, pod.item_id, items.num, items.desc, items.cat, items.unit, rrd.qty, null "
                "FROM pod "
                "JOIN items ON items.id = pod.item_id "
                "LEFT JOIN rrd ON rrd.pod_id = pod.id "
                "WHERE pod.po_num = "%docnum%";");
        } else {
            qry.exec("SELECT rrd.pod_id, rrd.item_id, items.num, items.desc, items.cat, items.unit, rrd.qty, rrd.condition "
                "FROM rrd "
                "LEFT JOIN items ON items.id = rrd.item_id "
                "WHERE rrd.rr_num = "%docnum%";");
        }
        break;
    }
    case MR: {
        qry.exec("SELECT mrd.item_id, items.num, items.desc, items.cat, items.unit, mrd.qty "
            "FROM mrd "
            "LEFT JOIN items ON items.id = mrd.item_id "
            "WHERE mrd.mr_num = "%docnum%";");
        break;
    }
    default:
        break;
    }

    //Inserting the details
    for (int i = 0; qry.next(); i++)
    {
        for (int j = 0; j < colNames.count(); j++)
        {
            if (QLineEdit *line = qobject_cast<QLineEdit*>(cellWidget(i,j))) {
                if (line->objectName() == "item" || line->objectName() == "supplier") {
                    line->setText(qry.value(j).toString());
                    line->completer()->setCompletionPrefix(qry.value(j).toString());
                    emit line->editingFinished();
                }
            }
            else if (QCheckBox *box = cellWidget(i,j)->findChild<QCheckBox*>("box")) {
                if (qry.value(j).toBool())
                    box->setChecked(true);
                else
                    box->setChecked(false);
            }
            else
                item(i,j)->setText(qry.value(j).toString());
        }
    }
}

void ResizableTable::initTaxCheckBox(int row, int column)
{
    QCheckBox *box = new QCheckBox(this);
    box->setObjectName("box");
    box->setChecked(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);

    QWidget *widget = new QWidget(this);

    widget->setLayout(layout);
    layout->addWidget(box);
    setCellWidget(row,column,widget);

    connect(box, &QCheckBox::stateChanged, this,
            [this, row] ()
    {
        calculateSubtotal(row);
    });

}

//This could be made quicker by subclassing a QLineEdit and creating a new property for checking whether it is valid or not
void ResizableTable::initCompanionLineEdit(int row, int column, QLineEdit *sourceLine, QLineEdit *line2, QLineEdit *line3, QLineEdit *line4, QString cssValid, QString cssInvalid)
{
    sourceLine->setFrame(false);
    sourceLine->setContentsMargins(1,1,1,1);
    sourceLine->setProperty("ok", 1);
    QStandardItemModel *model = new QStandardItemModel(1,2,sourceLine);
    model->setObjectName("model");
    model->setItem(0,0,new QStandardItem(""));
    model->setItem(0,1,new QStandardItem(""));
    QCompleter *completer = new QCompleter(model, sourceLine);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionColumn(1);

    QObject::connect(sourceLine, &QLineEdit::editingFinished, this,
           [=] ()
            {
                // This does not need a setCompleterPrefix function since there can only ever be one completion in this column
                if (completer->currentCompletion() == sourceLine->text())
                    sourceLine->setProperty("ok", 1);
                else
                    sourceLine->setProperty("ok", 0);

                //This doesn't check line2 because line2 will always be the itemNum line
                if (sourceLine->property("ok").toBool() && line3->property("ok").toBool() && line4->property("ok").toBool())
                {
                    item(row, cid.itemId)->setText(stringAt(completer,0));
                    sourceLine->setStyleSheet(cssValid);
                    line2->setStyleSheet(cssValid);
                    line3->setStyleSheet(cssValid);
                    line4->setStyleSheet(cssValid);
                } else {
                    item(row, cid.itemId)->setText("");
                    sourceLine->setStyleSheet(cssInvalid);
                    line2->setStyleSheet(cssInvalid);
                    line3->setStyleSheet(cssInvalid);
                    line4->setStyleSheet(cssInvalid);
                }
            });

    sourceLine->setCompleter(completer);
    emitSignalOnCompletion(sourceLine);
    setCellWidget(row, column, sourceLine);
    return;
}

void ResizableTable::changeCompletionModel(QLineEdit *line, QString itemId, QString value)
{
    line->findChild<QStandardItemModel*>("model")->item(0,0)->setText(itemId);
    line->findChild<QStandardItemModel*>("model")->item(0,1)->setText(value);
    line->completer()->setCompletionPrefix(value);
    line->setText(value);
}

void ResizableTable::initLineEdit(int row, lineType type, int column)
{
    QLineEdit *line = new QLineEdit(this);
    line->setFrame(false);
    line->setContentsMargins(1,1,1,1);
    //For now, these columns will always be in the same place.
    switch (type) {
    case Item: {
        line->setObjectName("item");
        QCompleter *completer = new QCompleter(parentDoc->findChild<QSqlQueryModel*>(QString::number(ModelFlag::Item)), line);
        completer->setCompletionColumn(1);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseSensitive); //I would like these completers to be case insensitive, but it causes problems right now when two different items are identical except for the cases.

        QLineEdit *descLine = new QLineEdit(this);
        QLineEdit *catLine = new QLineEdit(this);
        QLineEdit *unitLine = new QLineEdit(this);

        initCompanionLineEdit(row, cid.itemDesc, descLine, line, catLine, unitLine, CSS::Normal, CSS::Alert);
        initCompanionLineEdit(row, cid.unit, unitLine, line, descLine, catLine, CSS::Normal, CSS::Alert);
        initCompanionLineEdit(row, cid.cat, catLine, line, descLine, unitLine, CSS::Normal, CSS::Alert);

        QObject::connect(line, &QLineEdit::editingFinished, this,
                [=] ()
                {
                    // The below function is redundant, but it must be called for QCompleter to function as intended. QCompleter emits an
                    // activated signal when Enter/Return is pressed, but not when Tab or Backtab (Shift+Tab) is pressed. As a result, whenever
                    // there are multiple completions available and a user tabs out of the QCompleter, the QCompleter will default to the first
                    // completion for its comparison, even if the user has selected the 2nd, or 3rd, etc... The best solution would be to reimplement
                    // the QCompleter and have its event handler also include Tabs and Backtabs, but I don't know how to do that. The solution implemented
                    // below passes the appropriate text into the completer whenever editingFinished() is emited. Another solution is to have an
                    // event handler that sets the completer to the correct index whenever Tab or Backtab is pressed from within a QCompleter.
                    // I am not sure which is faster, but I have settled on the one that takes less code. Measure and refactor later.
                    completer->setCompletionPrefix(line->text());
                    if(completer->currentCompletion() == line->text()) {
                        item(row, cid.itemId)->setText(stringAt(completer,0));

                        changeCompletionModel(descLine, stringAt(completer, 0), stringAt(completer, 2));
                        changeCompletionModel(unitLine, stringAt(completer, 0), stringAt(completer, 3));
                        changeCompletionModel(catLine, stringAt(completer, 0), stringAt(completer, 4));

                        line->setStyleSheet(CSS::Normal);
                        descLine->setStyleSheet(CSS::Normal);
                        catLine->setStyleSheet(CSS::Normal);
                        unitLine->setStyleSheet(CSS::Normal);

                        if (tflag == QR) {
                            item(row, cid.prdId)->setText(stringAt(completer, 5));
                        }
                    } else {
                        item(row, cid.itemId)->setText("");

                        changeCompletionModel(descLine, "", "");
                        changeCompletionModel(unitLine, "", "");
                        changeCompletionModel(catLine, "", "");

                        line->setStyleSheet(CSS::Alert);
                        descLine->setStyleSheet(CSS::Alert);
                        catLine->setStyleSheet(CSS::Alert);
                        unitLine->setStyleSheet(CSS::Alert);
                        if (tflag == QR) { //QRs should never be allowed to reach this state
                            item(row, cid.prdId)->setText("");
                        }
                    }
                });

        line->setCompleter(completer);
        emitSignalOnCompletion(line);
        setCellWidget(row, cid.itemNum, line);
        break;
    }
    case Supplier: {
        line->setObjectName("supplier");
        QCompleter *completer = new QCompleter(parentDoc->findChild<QSqlQueryModel*>(QString::number(ModelFlag::Supplier)), line);
        completer->setCompletionColumn(1);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseSensitive);
        QObject::connect(line, &QLineEdit::editingFinished, this,
                [row, line, completer, this] ()
                {
                    completer->setCompletionPrefix(line->text());
                    if(completer->currentCompletion() == line->text())
                    {
                        item(row, cid.supplierId)->setText(stringAt(completer, 0));
                        line->setStyleSheet(CSS::Normal);
                    }
                    else
                    {
                        item(row, cid.supplierId)->setText("");
                        line->setStyleSheet(CSS::Alert);
                    }
                });
        setCellWidget(row, cid.supplier, line);
        line->setCompleter(completer);
        emitSignalOnCompletion(line);
        break;
    }
    case Disabled: {
        line->setObjectName("disabled");
        line->setDisabled(true);
        setCellWidget(row, column, line);
        break;
    }
    }
}

//  =================================================
//  == FUNCTIONS FOR INITIALIZING DOCUMENT WIDGETS ==
//  =================================================

//  RATIONALE - I don't mind having init functions for classes. If I were to reimplement the classes with
//              different constructors I'd have the exact same result with no reduction in code or complexity.
//              It sounds like two ways to tackle the same task. The one thing I'm not sure about is performace.
//              During the optimization stage, performance should be measured and action taken accordingly.

//      === HEADERS AND MODELS ===

QSqlQueryModel *initReadOnlyModel(ModelFlag modelFlag, QSqlDatabase db, QWidget *parent) //For completers
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setObjectName(QString::number(modelFlag));

    db.open();
    switch (modelFlag) {
    case ModelFlag::Item: {
        model->setQuery("SELECT id, num, desc, cat, unit FROM items;", db);
        break;
    }
    case ModelFlag::Supplier: {
        model->setQuery("SELECT id, name FROM suppliers;", db);
        break;
    }
    case ModelFlag::Project: {
        model->setQuery("SELECT name FROM projects;", db);
        break;
    }
    }
    while (model->canFetchMore()) { model->fetchMore(); }
    db.close();

    return model;
}

QStandardItemModel *initReadWriteModel(ModelFlag modelFlag, QSqlDatabase db, QWidget *parent) //For comboBoxes
{
    QStandardItemModel *model = new QStandardItemModel(parent);
    QSqlQuery qry(db);
    db.open();
    switch(modelFlag) {
    case ModelFlag::Project: {
        qry.exec("SELECT name FROM projects;");
        int i = 0;
        while (qry.next()) {
            model->setItem(i, 0, new QStandardItem(qry.value(0).toString()));
            i++;
        }
        break;
    }
    default:
        break;
    }
    db.close();
    return model;
}

QComboBox *initComboBox(QAbstractItemModel *model, QWidget *parent)
{
    QComboBox *combo = new QComboBox(parent);
    combo->setEditable(true);
    combo->setModel(model);
    return combo;
}

QLineEdit *initCompletedLineEdit(QSqlQueryModel *model, QWidget *parent)
{
    QLineEdit *line = new QLineEdit(parent);

    QCompleter *completer = new QCompleter(model, line);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    switch(static_cast<ModelFlag>(model->objectName().toInt())) {
    case Item: case Supplier: {
        completer->setCompletionColumn(1);
        break;
    }
    case Project: {
        completer->setCompletionColumn(0);
        break;
    }
    }

    line->setCompleter(completer);

    return line;
}

QDateTimeEdit *initDateTimeEdit(QWidget *parent)
{
    QDateTimeEdit *date = new QDateTimeEdit(QDate::currentDate(), parent);
    date->setDisplayFormat("yyyy/MM/dd");
    date->setCalendarPopup(true);
    date->setTimeSpec(Qt::UTC);
    date->setObjectName("date");

    return date;
}

QWidget *CreateDocument::initStaticInfoWidget(QWidget *parent, WidgetType widgetType, QString title)
{
    //A static info widget is uneditable by the user.
    //It is initialized as a blank Label and is filled in the fetchDetails() function.
    QWidget *widget = new QWidget(parent);
    widget->setObjectName(QString::number(widgetType));
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setSpacing(1);

    QLabel *name = new QLabel(title, widget);
    name->setFont(QFont("Consolas", 14, QFont::Bold));
    QLabel *address = new QLabel(widget);
    address->setObjectName("address");

    layout->addWidget(name);
    layout->addWidget(address);

    if (widgetType == Supplier) {
        QLabel *internal = new QLabel(widget);
        internal->setObjectName("internal");
        layout->addWidget(internal);
    }

    widget->setContentsMargins(0,0,0,0);
    return widget;
}

QWidget *CreateDocument::initDynamicInfoWidget(QWidget *parent, WidgetType widgetType, QSqlDatabase db, QString title)
{
    //Dynamic meaning the value can be changed by the user
    //This is a basic info widget for displaying simple items like addresses
    QWidget *widget = new QWidget(parent);
    widget->setObjectName(QString::number(widgetType));
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setSpacing(1);

    QLabel *name = new QLabel(widget);
    QPlainTextEdit *address = new QPlainTextEdit(widget);
    address->setPlaceholderText("Enter Address");
    address->setObjectName("address");

    layout->addWidget(name);
    layout->addWidget(address);

    switch(widgetType) {
    case Supplier: {
        title.isNull() ? name->setText("Supplier") : name->setText(title);

        QStandardItemModel *model = new QStandardItemModel(widget);
        QSqlQuery qry(db);
        model->setObjectName("supplierModel");
        db.open();
        qry.exec("SELECT suppliers.name, s.address, suppliers.info, suppliers.address_id, suppliers.id FROM suppliers LEFT JOIN addresses AS 's' ON s.id = suppliers.address_id;");
        for (int row = 0; qry.next(); row++) {
            qDebug() << qry.value(3).toString();
            model->setItem(row, 0, new QStandardItem(qry.value(0).toString()));
            model->setItem(row, 1, new QStandardItem(qry.value(1).toString()));
            model->setItem(row, 2, new QStandardItem(qry.value(2).toString()));
            model->setItem(row, 3, new QStandardItem(qry.value(3).toString()));
            model->setItem(row, 4, new QStandardItem(qry.value(4).toString()));
        }
        db.close();

        QComboBox *comboBox = new QComboBox(widget);
        comboBox->setObjectName("combo");
        comboBox->setEditable(true);
        comboBox->lineEdit()->setPlaceholderText("Select Supplier");
        layout->insertWidget(1, comboBox);

        QPlainTextEdit *internal = new QPlainTextEdit(widget);
        internal->setObjectName("internal");
        internal->setPlaceholderText("Enter Internal Info");
        layout->addWidget(internal);

        comboBox->setModel(model);

        connect(comboBox, &QComboBox::activated, widget, [address, internal, model] (int index) //This doesn't work when Tab is presed.
        {
            address->setPlainText(model->index(index, 1).data().toString());
            internal->setPlainText(model->index(index, 2).data().toString());
        });
        break;
    }
    case ShippingAddress: {
        title.isNull() ? name->setText("Shipping Address") : name->setText(title);
        break;
    }
    case BillingAddress: {
        title.isNull() ? name->setText("Billing Address") : name->setText(title);
        break;
    }
    }

    widget->setContentsMargins(0,0,0,0);
    return widget;
}

//      === TOTALS ===

void CreateDocument::initTotals() //Need to fix how QSqlQueries work in the program in general. It messes the database open()/close() flow often.
{
    /* This is the most complex part of the CreateDocument window, and I'd like to simplify it in the future. This is how it works for now.
     *
     * A subtotal will always be a label. The text in the label is generated from the everything previous
     * The following happens whenever a subtotal is triggered:
     *  1. All amounts (starting from the previous subtotal if it exists) are added together and brought down and the subtotal is updated.
     *  2. The subtotal will emit a signal that the next subtotal will read.
     *
     * The structure of the totals will change depending on whether
     *  1. All items in the table are taxable or not.
     *  2. The discount is before tax or after tax.
     *  3. The order has a discount on it (this is a smaller change).
     *
     * As a result, there are 4 main states that the totals can exist in.
     * (with the Working Subtotal column being hidden or not depending on whether it is the same as the subtotal column or not).
     * It looks as follows:
     *
     *      0   Taxable Subtotal
     *      1   Tax Exempt Subtotal
     *      2   Discount on Taxable (applied before tax)
     *      3   Discount on Tax Exempt (applied before tax)
     *      4   Working Taxable Subtotal
     *      5   Working Tax Exempt Subtotal
     *      6   Tax
     *      7   Discount (applied after Tax)
     *      8...Others (user inserted expenses or discounts - this spans for an unknown number of rows)
     *          Total
     *
     *  This allows for all 4 permutations to all do the same operations and get to the same result. As seen below:
     *
     *    +---------------------+-------------------------+---------------------+-------------------------+
     *    |             Discount BEFORE Tax               |            Discount AFTER Tax                 |
     *    +---------------------+-------------------------+---------------------+-------------------------+
     *    | All Items Taxable   | Some Items Exempt       | All Items Taxable   | Some Items Exempt       |
     *    +---------------------+-------------------------+---------------------+-------------------------+
     * 0  | Subtotal            | Taxable Subtotal        | Subtotal            | Taxable Subtotal        | <       (sum of taxable items)
     * 1  | 0                   | Tax E. Subtotal         | 0                   | Tax E. Subtotal         | <       (sum of tax exempt items) . . . . . . . . . . . . . . . . . . . [if == 0]
     * 2  | Discount            | Discount On Taxable     | 0                   | 0                       | < user: discount on taxable subtotal . . . . . . [if !discountBeforeTax]
     * 3  | 0                   | Discount on Tax E.      | 0                   | 0                       | < user: discount on taxable subtotal . . . . . . [if !discountBeforeTax] [if sum of tax E. == 0]
     * 4  | - - - - - - - - - - | - - - - - - - - - - - - | - - - - - - - - - - | - - - - - - - - - - - - |                                                  [if taxableDiscount == 0 && exemptDiscount == 0]
     * 5  | Taxable Amount      | Taxable Amount          | Taxable Amount      | Taxable Amount          | <       (taxable subtotal - discount on taxable) [if taxableDiscount == 0 && exemptDiscount == 0]
     * 6  | 0                   | Tax Exempt Amount       | 0                   | Tax Exempt Amount       | <       (tax E. subtotal - discount on tax E.) . [if taxableDiscount == 0 && exemptDiscount == 0]
     * 7  | Tax                 | Tax                     | Tax                 | Tax                     | < user: tax rate (rate * taxable amount)
     * 8  | 0                   | 0                       | Discount            | Discount                | < user: discount after tax . . . . . . . . . . . [if discountBeforeTax]
     * 9  | Other               | Other                   | Other               | Other                   | < user: other expenses or discounts
     * n  | ...                 | ...                     | ...                 | ...                     | < user: any number of others
     * n+1| - - - - - - - - - - | - - - - - - - - - - - - | - - - - - - - - - - | - - - - - - - - - - - - |
     * n+2| Total               | Total                   | Total               | Total                   | <       （sum of all rows > 3)
     *    +---------------------+-------------------------+---------------------+-------------------------+
     *      ^ This is default ^
     *
     *    There are 3 types of rows within the totals:
     *      - Expense   (QLineEdit)
     *      - Discounts (QLineEdit)
     *      - Subtotals (QLabel)
     */
    connect (table, &ResizableTable::cellChanged, table,
             [=] (int row, int column)
    {
        if ((column == table->cid.unitPrice || column == table->cid.qty) && row != table->finalRow)
            table->calculateSubtotal(row);
    });

    totalsColumns = "discount_before_tax, taxable_subtotal, tax_exempt_subtotal, "
        "discount_on_taxable_rate, discount_on_taxable, discount_on_tax_exempt_rate, discount_on_tax_exempt, tax_rate, tax, discount_after_tax_rate, discount_after_tax, total";
    customExpenseRowCount = 0;
    int nextOpenRow = 11;
    //No idea why, but I had to declare these booleans globally because it refused to be declared as anything other than a const. I need to edit it. I have no idea why that happens.
    taxExempt = 0;
    discBeforeTax = 1;

    discountBeforeTax = new QCheckBox("Discount Before Tax", ui->internalWidget);
    discountBeforeTax->setChecked(true);
    qDebug() << ui->internalDetailsGrid->rowCount();
    ui->internalDetailsGrid->addWidget(discountBeforeTax, ui->internalDetailsGrid->rowCount(), 0, 1, 2);

    //Declare everything so we can work from the top down
    total = createTotalsLabel(nullptr, &nextOpenRow, "Grand Total", ui->totalsWidget, ui->totalsGridLayout);
    line2 = createTotalsLine(&nextOpenRow, ui->totalsWidget, ui->totalsGridLayout);
    customExpenseRowStart = nextOpenRow;                                    //See this...
    addExpenseLabel = new QLabel("Add Expense", ui->totalsWidget);          //although this is only called once
    addExpenseLabel->setAlignment(Qt::AlignRight);                          //it could be placed inside of a function
    addExpenseButton = new QPushButton("+", ui->totalsWidget);              //so that the creation of everything would
    ui->totalsGridLayout->addWidget(addExpenseLabel, nextOpenRow, 2);       //be neater and clearer. Everything might
    ui->totalsGridLayout->addWidget(addExpenseButton, nextOpenRow, 3);      //look and work a little better. But maybe
    nextOpenRow--;                                                          //that's stupid.
    discountAfterTax = createTotalsLineEdit(total, &nextOpenRow, "Discount", ui->totalsWidget, ui->totalsGridLayout, 0);
    tax = createTotalsLineEdit(total, &nextOpenRow, "Tax", ui->totalsWidget, ui->totalsGridLayout);
    taxExemptAmount = createTotalsLabel(total, &nextOpenRow, "Tax Exempted Amount", ui->totalsWidget, ui->totalsGridLayout);
    taxableAmountRow = nextOpenRow;
    taxableAmount= createTotalsLabel(total, &nextOpenRow, "Taxable Amount", ui->totalsWidget, ui->totalsGridLayout);
    line1 = createTotalsLine(&nextOpenRow, ui->totalsWidget, ui->totalsGridLayout);
    discountOnTaxExempt = createTotalsLineEdit(taxExemptAmount, &nextOpenRow, "Discount on Tax Exempt", ui->totalsWidget, ui->totalsGridLayout, 0);
    discountOnTaxable = createTotalsLineEdit(taxableAmount, &nextOpenRow, "Discount on Taxable", ui->totalsWidget, ui->totalsGridLayout, 0);
    taxExemptSubtotal = createTotalsLabel(taxExemptAmount, &nextOpenRow, "Tax Exempt Subtotal", ui->totalsWidget, ui->totalsGridLayout);
    taxableSubtotal = createTotalsLabel(taxableAmount, &nextOpenRow, "Taxable Subtotal", ui->totalsWidget, ui->totalsGridLayout);


    //Hiding the appropriate rows for the default totals layout
    setTotalsRowHidden(ui->totalsGridLayout, line1, true);
    setTotalsRowHidden(ui->totalsGridLayout, taxableAmount, true);
    setTotalsRowHidden(ui->totalsGridLayout, taxExemptSubtotal, true);
    setTotalsRowHidden(ui->totalsGridLayout, discountOnTaxExempt, true);
    setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, true);
    setTotalsRowHidden(ui->totalsGridLayout, discountAfterTax, true);

    //Connecting the triggers to hide and unhide rows (the triggers to hide a row are labeled in the ASCII table above in the [square] brackets.
    connect(discountBeforeTax, &QCheckBox::stateChanged, ui->totalsGridLayout,
            [=] (int state)
    {
        if (state)
        {
            discountAfterTax->setText("0.00");
            setTotalsRowHidden(ui->totalsGridLayout, discountAfterTax, true);
            setTotalsRowHidden(ui->totalsGridLayout, discountOnTaxable, false);
            if (taxExempt)
            {
                setTotalsRowHidden(ui->totalsGridLayout, discountOnTaxExempt, false);
            }
            discBeforeTax = true;
        } else {
            discountOnTaxable->setText("0.00");
            discountOnTaxExempt->setText("0.00");
            setTotalsRowHidden(ui->totalsGridLayout, discountAfterTax, false);
            setTotalsRowHidden(ui->totalsGridLayout, discountOnTaxable, true);
            setTotalsRowHidden(ui->totalsGridLayout, discountOnTaxExempt, true);
            setTotalsRowHidden(ui->totalsGridLayout, taxableAmount, true);
            setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, true);
            setTotalsRowHidden(ui->totalsGridLayout, line1, true);
            discBeforeTax = false;
        }
        emit taxableAmount->linkActivated("");
    });

    connect(taxExemptSubtotal, &QLabel::linkActivated, ui->totalsGridLayout,
            [=] ()
    {
        if (taxExemptSubtotal->text().toDouble() == 0)
        {
            setTotalsRowHidden(ui->totalsGridLayout, taxExemptSubtotal, true);
            setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, true);
            discountOnTaxExempt->setText("0.00");
            setTotalsRowHidden(ui->totalsGridLayout, discountOnTaxExempt, true);
            taxExempt = false;
        } else {
            setTotalsRowHidden(ui->totalsGridLayout, taxExemptSubtotal, false);
            if (discBeforeTax)
                setTotalsRowHidden(ui->totalsGridLayout, discountOnTaxExempt, false);
            if (discountOnTaxable->text().toDouble() != 0)
                setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, false);
            taxExempt = true;
        }
        emit taxableAmount->linkActivated("");
    });

    connect(discountOnTaxable, &QLineEdit::textChanged, ui->totalsGridLayout,
            [=] (const QString &text)
    {
        if (text.toDouble() == 0 && discountOnTaxExempt->text().toDouble() == 0)
        {
            setTotalsRowHidden(ui->totalsGridLayout, line1, true);
            setTotalsRowHidden(ui->totalsGridLayout, taxableAmount, true);
            setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, true);
        } else {
            setTotalsRowHidden(ui->totalsGridLayout, line1, false);
            setTotalsRowHidden(ui->totalsGridLayout, taxableAmount, false);
            if (taxExempt)
                setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, false);
        }
        emit taxableAmount->linkActivated("");
    });

    connect(discountOnTaxExempt, &QLineEdit::textChanged, ui->totalsGridLayout,
            [=] (const QString &text)
    {
        if (text.toDouble() == 0 && discountOnTaxable->text().toDouble() == 0)
        {
            setTotalsRowHidden(ui->totalsGridLayout, line1, true);
            setTotalsRowHidden(ui->totalsGridLayout, taxableAmount, true);
            setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, true);
        } else {
            setTotalsRowHidden(ui->totalsGridLayout, line1, false);
            setTotalsRowHidden(ui->totalsGridLayout, taxableAmount, false);
            if (taxExempt)
                setTotalsRowHidden(ui->totalsGridLayout, taxExemptAmount, false);
        }
        emit taxableAmount->linkActivated("");
    });

    connect(addExpenseButton, &QPushButton::clicked, this,
            [=] ()
    //The following lambda function is based off of the addCustomField a function.
    //and the createTotals function. It is redundant in a lot of places, and could
    //probably be removed after some light tweaking
    {
        customExpenseRowCount++;

        int buttonRow, trash; //trash exists as a junk pointer that will never be read.
        ui->totalsGridLayout->getItemPosition(ui->totalsGridLayout->indexOf(addExpenseButton), &buttonRow, &trash, &trash, &trash);
        QCheckBox *box = new QCheckBox(ui->totalsWidget);

        QLineEdit *rate = new QLineEdit(ui->totalsWidget);
        rate->setAlignment(Qt::AlignCenter);
        rate->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        rate->setPlaceholderText("%");
        rate->setMaximumWidth(35);
        rate->setEnabled(false);

        QLineEdit *label = new QLineEdit(ui->totalsWidget);
        label->setAlignment(Qt::AlignRight);
        label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

        QLineEdit *amount = new QLineEdit(ui->totalsWidget);
        amount->setAlignment(Qt::AlignRight);
        amount->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        amount->setPlaceholderText("$");
        amount->setMaximumWidth(100);

        QPushButton *deleteExpenseButton = new QPushButton(ui->totalsWidget);
        deleteExpenseButton->setMaximumSize(QSize(16,16));
        deleteExpenseButton->setText("x");
        deleteExpenseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        deleteExpenseButton->setStyleSheet("background-color: rgb(255, 0, 4);");

        //You CANNOT use replaceWidget() here because it messes with the order of indices, which is vital.
        QLabel *totalLabel = qobject_cast<QLabel*>(ui->totalsGridLayout->itemAt(ui->totalsGridLayout->indexOf(total) - 1)->widget());
        ui->totalsGridLayout->removeWidget(line2);
        ui->totalsGridLayout->removeWidget(addExpenseLabel);
        ui->totalsGridLayout->removeWidget(addExpenseButton);
        ui->totalsGridLayout->removeWidget(totalLabel);
        ui->totalsGridLayout->removeWidget(total);

        ui->totalsGridLayout->addWidget(box, buttonRow, 0);
        ui->totalsGridLayout->addWidget(rate, buttonRow, 1);
        ui->totalsGridLayout->addWidget(label, buttonRow, 2);
        ui->totalsGridLayout->addWidget(amount, buttonRow, 3);
        ui->totalsGridLayout->addWidget(deleteExpenseButton, buttonRow, 4);

        ui->totalsGridLayout->addWidget(addExpenseLabel, buttonRow+1, 2);
        ui->totalsGridLayout->addWidget(addExpenseButton, buttonRow+1, 3);

        ui->totalsGridLayout->addWidget(line2, buttonRow+2, 2, 1, 2);

        ui->totalsGridLayout->addWidget(totalLabel, buttonRow+3, 2);
        ui->totalsGridLayout->addWidget(total, buttonRow+3, 3);

        connect(box, &QCheckBox::stateChanged,
                [rate, amount, this] (int state)
        {
            if (state)
            {
                rate->setEnabled(true);
                amount->setEnabled(false);
            } else {
                rate->setEnabled(false);
                amount->setEnabled(true);
                rate->clear();
            }
            emit total->linkActivated("");
        });

        connect(rate, &QLineEdit::textEdited, [this] () { emit total->linkActivated(""); });
        connect(amount, &QLineEdit::textEdited, [this] () { emit total->linkActivated(""); });

        connect(deleteExpenseButton, &QPushButton::clicked,
                [=] ()
        {
            //This function deletes all items that are no longer used and then moves every item below it up by one
            //First, delete all items
            int row, trash;
            ui->totalsGridLayout->getItemPosition(ui->totalsGridLayout->indexOf(box), &row, &trash, &trash, &trash);
            QLayoutItem *child;
            for (int column = 0; column < 5; column++)
            {
                child = ui->totalsGridLayout->itemAtPosition(row, column);
                ui->totalsGridLayout->removeItem(child);
                delete child->widget();
                delete child;
            }
            row++;

            //Second, move all custom items up by one (Fill in the vacuum left by deleted items
            for (int customExpenseRowEnd = customExpenseRowStart + customExpenseRowCount; row < customExpenseRowEnd; row++)
            {
                for (int column = 0; column < 5; column++)
                {
                    child = ui->totalsGridLayout->itemAtPosition(row, column);
                    ui->totalsGridLayout->removeItem(child);
                    ui->totalsGridLayout->addItem(child, row-1, column);
                }
            }

            //Last, move the expenseButton, expenseLabels, line, and total labels up by one
            for (int i = row+3, skip = row+1; row < i; row++) {
                for (int column = 2; column < 4; column++)
                {
                    if (row == skip) {
                        child = ui->totalsGridLayout->itemAtPosition(row, column);
                        ui->totalsGridLayout->removeItem(child);
                        ui->totalsGridLayout->addItem(child, row-1, column, 1, 2);
                        break;
                    }
                    else {
                        child = ui->totalsGridLayout->itemAtPosition(row, column);
                        ui->totalsGridLayout->removeItem(child);
                        ui->totalsGridLayout->addItem(child, row-1, column);
                    }
                }
            }
            customExpenseRowCount--;
            emit total->linkActivated("");
        });
    });
}

QLabel* CreateDocument::createTotalsLabel(QLabel *nextSubtotal, int *row, QString title, QWidget *parent, QGridLayout *grid)
{
    QLabel *label = new QLabel(title, parent);
    QLabel *amount = new QLabel("0.00", parent);

    label->setAlignment(Qt::AlignRight);
    amount->setAlignment(Qt::AlignRight);

    grid->addWidget(label, *row, 2);
    grid->addWidget(amount, *row, 3);

    //Since most of these only get called once, it may be better to just inline them into the initTotals function.
    connect(amount, &QLabel::linkActivated,
            [=] ()
    {
        int totalRow, trash;
        ui->totalsGridLayout->getItemPosition(ui->totalsGridLayout->indexOf(amount), &totalRow, &trash, &trash, &trash);

        if (nextSubtotal == nullptr) //This is the grand total.
        {
            double total = 0;
            double taxableAmount = qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(taxableAmountRow, 3)->widget())->text().toDouble();
            QLineEdit *line;
            total += taxableAmount;
            total += qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(taxableAmountRow+1, 3)->widget())->text().toDouble();
            for (int currentRow = (taxableAmountRow + 2); currentRow < (totalRow - 2); currentRow++)
            {
                line = qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(currentRow, 3)->widget());
                if(!line->isEnabled()) {
                    double newAmount;
                    newAmount = taxableAmount * qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(currentRow, 1)->widget())->text().toDouble();
                    line->setText(QString::number(newAmount, 'f', 2));
                }
                total += line->text().toDouble();
            }
            amount->setText(QString::number(total, 'f', 2));
        } else if (totalRow < taxableAmountRow) { //This means it's one of the first subtotals. No calculations needed
            emit nextSubtotal->linkActivated("");
        } else { //This is the taxable amount. The four rows before this are always the same, so we can do operations on constants.
            QLineEdit *line;
            double newAmount;

            line = qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(2, 3)->widget());
            if (!line->isEnabled())
            {
                newAmount = qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(0, 3)->widget())->text().toDouble()
                        * qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(2, 1)->widget())->text().toDouble();
                line->setText(QString::number(newAmount, 'f', 2));
            }
            newAmount = line->text().toDouble() + qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(0, 3)->widget())->text().toDouble();
            qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(5, 3)->widget())->setText(QString::number(newAmount, 'f', 2));

            line = qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(3, 3)->widget());
            if (!line->isEnabled())
            {
                newAmount = qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(1, 3)->widget())->text().toDouble()
                        * qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(3, 1)->widget())->text().toDouble();
                line->setText(QString::number(newAmount, 'f', 2));
            }
            newAmount = line->text().toDouble() + qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(1, 3)->widget())->text().toDouble();
            qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(6, 3)->widget())->setText(QString::number(newAmount, 'f', 2));

            emit nextSubtotal->linkActivated("");
        }
    });

    *row = *row - 1;
    return amount;
}

QLineEdit* CreateDocument::createTotalsLineEdit(QLabel *nextSubtotal, int *row, QString title, QWidget *parent, QGridLayout *grid, bool expense)
{
    //The subtotal is what the subtotal this is currently working towards
    QCheckBox *box = new QCheckBox(parent);
    QLineEdit *rate = new QLineEdit(parent);
    QLabel *label = new QLabel(title, parent);
    QLineEdit *amount = new QLineEdit(parent);

    rate->setAlignment(Qt::AlignCenter);
    rate->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    rate->setPlaceholderText("%");
    rate->setMaximumWidth(35);
    rate->setEnabled(false);

    label->setAlignment(Qt::AlignRight);

    amount->setAlignment(Qt::AlignRight);
    amount->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    amount->setPlaceholderText("$");
    amount->setMaximumWidth(100);

    grid->addWidget(box, *row, 0);
    grid->addWidget(rate, *row, 1);
    grid->addWidget(label, *row, 2);
    grid->addWidget(amount, *row, 3);

    connect(box, &QCheckBox::stateChanged,
            [rate, amount] (int state)
    {
        if (state)
        {
            rate->setEnabled(true);
            amount->setEnabled(false);
        } else {
            rate->setEnabled(false);
            amount->setEnabled(true);
            rate->clear();
        }
    });

    connect(rate, &QLineEdit::textEdited, [nextSubtotal] () { emit nextSubtotal->linkActivated(""); });
    connect(amount, &QLineEdit::textEdited, [nextSubtotal] () { emit nextSubtotal->linkActivated(""); });
    //signaling the subtotal to recalculates everything may not be necessary. Optimization opportunity.

    *row = *row - 1;
    return amount;
}

QFrame* CreateDocument::createTotalsLine(int *row, QWidget *parent, QGridLayout *grid)
{
    QFrame *line = new QFrame(parent);
    line->setGeometry(QRect());
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    grid->addWidget(line, *row, 2, 1, 2);

    *row = *row - 1;
    return line;
}

//      === CUSTOM DETAILS ===

void CreateDocument::insertRecurringCustomDetails(QSqlQuery qry, int *row)
{
    ui->detailsGrid->removeWidget(ui->addCustom);
    ui->detailsGrid->removeWidget(ui->customLabel);
    qry.exec("SELECT name FROM recurring_custom_details WHERE tflag = "%QString::number(tFlag)%";");
    while (qry.next())
    {
        QLabel *label = new QLabel(ui->details);
        label->setAlignment(Qt::AlignRight);
        label->setText(qry.value(0).toString());
        ui->detailsGrid->addWidget(label, *row, 0);
        QLineEdit *line = new QLineEdit(ui->details);
        line->setAlignment(Qt::AlignRight);
        line->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        ui->detailsGrid->addWidget(line, *row, 1);
        *row += 1;
    }
    ui->detailsGrid->addWidget(ui->customLabel, *row, 0);
    ui->detailsGrid->addWidget(ui->addCustom, *row, 1);
}

void CreateDocument::on_addCustom_clicked()
{
    QLineEdit *custom1 = new QLineEdit;
    custom1->setAlignment(Qt::AlignRight);
    custom1->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
    QLineEdit *custom2 = new QLineEdit;
    custom2->setAlignment(Qt::AlignRight);
    custom2->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
    QPushButton *deleteButton = new QPushButton;
    deleteButton->setMaximumSize(QSize(16,16));
    deleteButton->setText("x");
    deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    deleteButton->setStyleSheet("background-color: rgb(255, 0, 4);");

    connect(deleteButton, &QPushButton::clicked, this, &CreateDocument::deleteCustomDetail);

    ui->detailsGrid->removeWidget(ui->addCustom);
    ui->detailsGrid->removeWidget(ui->customLabel);
    ui->detailsGrid->addWidget(custom1, addDetailButtonRow, 0);
    ui->detailsGrid->addWidget(custom2, addDetailButtonRow, 1);
    ui->detailsGrid->addWidget(deleteButton, addDetailButtonRow, 2);
    ui->detailsGrid->addWidget(ui->customLabel, addDetailButtonRow+1, 0);
    ui->detailsGrid->addWidget(ui->addCustom, addDetailButtonRow+1, 1);

    customDetailCount++;
    addDetailButtonRow++;
}

void CreateDocument::deleteCustomDetail()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int row, trash;
    ui->detailsGrid->getItemPosition(ui->detailsGrid->indexOf(button), &row, &trash, &trash, &trash);
    QLayoutItem *child;

    //delete the selected row
    for (int i = 0; i < 3; i++) {
        child = ui->detailsGrid->itemAtPosition(row, i);
        ui->detailsGrid->removeItem(child);
        delete child->widget();
        delete child;
    }

    //move all other rows down
    for (; row < addDetailButtonRow; row++) {
        child = ui->detailsGrid->itemAtPosition(row+1, 0);
        ui->detailsGrid->removeItem(child);
        ui->detailsGrid->addItem(child, row, 0);

        child = ui->detailsGrid->itemAtPosition(row+1, 1);
        ui->detailsGrid->removeItem(child);
        ui->detailsGrid->addItem(child, row, 1);

        if (ui->detailsGrid->itemAtPosition(row+1, 2) != nullptr)
        {
            child = ui->detailsGrid->itemAtPosition(row+1, 2);
            ui->detailsGrid->removeItem(child);
            ui->detailsGrid->addItem(child, row, 2);
        }
    }

    customDetailCount--;
    addDetailButtonRow--;
}

//  ===============================================
//  == FUNCTIONS FOR POPULATING DOCUMENT WIDGETS ==
//  ===============================================

//      === GENERAL ===

void CreateDocument::clear()
{
    while (customDetailCount > 0)
        emit qobject_cast<QPushButton*>(ui->detailsGrid->itemAtPosition(addDetailButtonRow-1, 2)->widget())->clicked();
    while (customExpenseRowCount > 0)
        emit qobject_cast<QPushButton*>(ui->totalsGridLayout->itemAtPosition(customExpenseRowStart+customExpenseRowCount-1, 4)->widget())->clicked();
    table->clear();
    table->setRowCount(0);
    table->setHorizontalHeaderLabels(table->colNames);
    table->appendRow();
}

void CreateDocument::fetchDetails(QSqlQuery qry, QString docnum, TableFlag modifier)
{
    //This will give an incorrect email if the user changes their historical email. To fix this, emails have to be stored separately and referenced in the documents.
    //For complete accuracy this should happen, but that isn't necessary right now.
    // - fetch Details, internal details, infoboxes
    switch (tFlag) {
    case PR: {
        if (docnum.isEmpty()) {
            qry.exec("SELECT date('now', 'localtime'), "%escapeSql(user->name)%","%escapeSql(user->email)%", date('now', 'localtime'), null, notes "
                "FROM default_notes WHERE tflag = "%QString::number(PR)%";");
        } else {
            qry.exec("SELECT date(pr.date,'localtime'), userdata.name, userdata.email, date(pr.date_needed,'localtime'), pr.project, pr.notes "
                "FROM pr JOIN userdata ON userdata.id = pr.requested_by WHERE pr.num = "%docnum%";");
        }
        qry.next();
        ui->detailsNames->setText("Date:\nCreator:"%returnStringINN(qry.value(2).toString(),"\nEmail"));
        ui->detailsValues->setText(qry.value(0).toString()%"\n"%qry.value(1).toString()%returnStringINN(qry.value(2).toString(),"\n"%qry.value(2).toString()));
        ui->internalWidget->findChild<QDateTimeEdit*>("date")->setDate(qry.value(3).toDate());
        findChild<QComboBox*>("project")->setEditText(qry.value(4).toString());
        ui->notes->setPlainText(qry.value(5).toString());
        break;
    }
    case QR: {
        if (docnum.isEmpty()) {
            qry.exec("SELECT date('now', 'localtime'), "%escapeSql(user->name)%","%escapeSql(user->email)%", date('now', 'localtime'), default_notes.notes "
                ",null, null, null, shipping.address, billing.address FROM default_notes "
                    "JOIN company ON company.rowid = 1 "
                    "JOIN addresses AS 'shipping' ON shipping.id = company.shipping_address_id "
                    "JOIN addresses AS 'billing' ON billing.id = company.billing_address_id "
                "WHERE tflag = "%QString::number(QR)%";");
        } else {
            qry.exec("SELECT date(qr.date,'localtime'), userdata.name, userdata.email, date(qr.date_needed,'localtime'), qr.notes "
                ",qr.supplier_id, suppliers.info, supplier_address.address, shipping.address, billing.address FROM qr "
                    "JOIN userdata ON userdata.id = qr.requested_by "
                    "JOIN addresses AS 'shipping' ON shipping.id = qr.shipping_address_id "
                    "JOIN addresses AS 'billing' ON billing.id = qr.billing_address_id "
                    "JOIN addresses AS 'supplier_address' ON qr.supplier_address_id = supplier_address.id "
                    "JOIN suppliers ON qr.supplier_id = suppliers.id "
                "WHERE qr.num = "%docnum%";");
        }
        qry.next();
        ui->detailsNames->setText("Date:\nCreator:"%returnStringINN(qry.value(2).toString(),"\nEmail"));
        ui->detailsValues->setText(qry.value(0).toString()%"\n"%qry.value(1).toString()%returnStringINN(qry.value(2).toString(),"\n"%qry.value(2).toString()));
        ui->internalWidget->findChild<QDateTimeEdit*>("date")->setDate(qry.value(3).toDate());
        ui->notes->setPlainText(qry.value(4).toString());

        if (qry.value(5).isNull()) {
            ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo")->setEditText(qry.value(5).toString());
        } else {
            QComboBox *combo = ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo");
            int row = matchingRow(qry.value(5).toString(), combo->model(), 4);
            combo->setCurrentIndex(row);
        }

        ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QPlainTextEdit*>("internal")->setPlainText(qry.value(6).toString());
        ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QPlainTextEdit*>("address")->setPlainText(qry.value(7).toString());
        ui->info->findChild<QWidget*>(QString::number(ShippingAddress))->findChild<QPlainTextEdit*>("address")->setPlainText(qry.value(8).toString());
        ui->info->findChild<QWidget*>(QString::number(BillingAddress))->findChild<QPlainTextEdit*>("address")->setPlainText(qry.value(9).toString());
        break;
    }
    case PO: {
        if (docnum.isEmpty()) {
            qry.exec("SELECT date('now', 'localtime'), "%escapeSql(user->name)%","%escapeSql(user->email)%", date('now', 'localtime'), default_notes.notes "
                ",null, null, null, shipping.address, billing.address "
                "FROM default_notes "
                    "JOIN company ON company.rowid = 1 "
                    "JOIN addresses AS 'shipping' ON shipping.id = company.shipping_address_id "
                    "JOIN addresses AS 'billing' ON billing.id = company.billing_address_id "
                "WHERE tflag = "%QString::number(PO)%";");
        } else {
            qry.exec("SELECT date(po.date,'localtime'), userdata.name, userdata.email date(po.date_needed,'localtime'), po.notes "
                ",po.supplier_id, suppliers.info, supplier_address.address, shipping.address, billing.address "
                "FROM po "
                    "JOIN userdata ON userdata.id = po.requested_by "
                    "JOIN addresses AS 'shipping' ON shipping.id = po.shipping_address_id "
                    "JOIN addresses AS 'billing' ON billing.id = po.billing_address_id "
                    "JOIN addresses AS 'supplier_address' ON po.supplier_address_id = supplier_address.id "
                    "JOIN suppliers ON po.supplier_id = suppliers.id "
                "WHERE po.num = "%docnum%";");
        }
        qry.next();
        ui->detailsNames->setText("Date:\nCreator:"%returnStringINN(qry.value(2).toString(),"\nEmail"));
        ui->detailsValues->setText(qry.value(0).toString()%"\n"%qry.value(1).toString()%returnStringINN(qry.value(2).toString(),"\n"%qry.value(2).toString()));
        ui->internalWidget->findChild<QDateTimeEdit*>("date")->setDate(qry.value(3).toDate());
        ui->notes->setPlainText(qry.value(4).toString());

        if (qry.value(5).isNull()) {
            ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo")->setPlaceholderText("Select Supplier");
        } else {
            QComboBox *combo = ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo");
            combo->setCurrentIndex(matchingRow(qry.value(5).toString(), combo->model()));
        }

        ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QPlainTextEdit*>("internal")->setPlainText(qry.value(6).toString());
        ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QPlainTextEdit*>("address")->setPlainText(qry.value(7).toString());
        ui->info->findChild<QWidget*>(QString::number(ShippingAddress))->findChild<QPlainTextEdit*>("address")->setPlainText(qry.value(8).toString());
        ui->info->findChild<QWidget*>(QString::number(BillingAddress))->findChild<QPlainTextEdit*>("address")->setPlainText(qry.value(9).toString());
        break;
    } //11762
    case RR: {
        bool validPO = POIsValidAndOpen(qry, docnum);
        //date, name, email, date_arrived, notes, po_num, invoice_num, delivered_by, supplier_id, supplier_address_id
        if (modifier == PO) {
            if (validPO) {
                qry.exec("SELECT date('now', 'localtime'), userdata.name, userdata.email, date(po.date_expected, 'localtime'), default_notes.notes "
                    ",po.num, po.invoice_num, po.supplier_id, addresses.address, null "
                    "FROM po "
                        "LEFT JOIN default_notes ON default_notes.tflag = "%QString::number(RR)%" "
                        "LEFT JOIN userdata ON po.authorized_by = userdata.id"
                    "WHERE po.num = "%docnum%";");
            } else {
                qry.exec("SELECT date('now', 'localtime'), "%escapeSql(user->name)%","%escapeSql(user->email)%", date('now', 'localtime'), notes "
                    ","%escapeSql(docnum)%", null, null, null, null "
                    "FROM default_notes WHERE tflag = "%QString::number(RR)%";");
            }
        } else if (docnum.isEmpty()) {
            qry.exec("SELECT date('now', 'localtime'), "%escapeSql(user->name)%","%escapeSql(user->email)%", date('now', 'localtime'), notes "
                ",null, null, null, null, null "
                "FROM default_notes WHERE tflag = "%QString::number(RR)%";");
        } else {
            qry.exec("SELECT date(rr.date,'localtime'), userdata.name, userdata.email, date(rr.date_arrived, 'localtime'), rr.notes "
                ",rr.po_num, rr.invoice_num, rr.supplier_id, rr.supplier_address_id, rr.delivered_by "
                "FROM rr "
                    "JOIN userdata ON userdata.id = rr.inspected_by "
                    "WHERE rr.num = "%docnum%";");
        }
        qry.next();
        ui->detailsNames->setText("Date:\nInspector:"%returnStringINN(qry.value(2).toString(),"\nEmail"));
        ui->detailsValues->setText(qry.value(0).toString()%"\n"%qry.value(1).toString()%returnStringINN(qry.value(2).toString(),"\n"%qry.value(2).toString()));
        ui->internalWidget->findChild<QDateTimeEdit*>("date")->setDate(qry.value(3).toDate());
        ui->notes->setPlainText(qry.value(4).toString());
        if (qry.value(5).isNull()) {
            ui->internalDetails->findChild<QComboBox*>("po")->setPlaceholderText("Select PO");
        } else if (!validPO) {
            ui->internalDetails->findChild<QComboBox*>("po")->setEditText(docnum);
        } else {
            QComboBox *combo = ui->internalDetails->findChild<QComboBox*>("po");
            combo->setCurrentIndex(matchingRow(qry.value(5).toString(), combo->model()));
            ui->internalDetails->findChild<QLineEdit*>("invoice")->setText(qry.value(6).toString());
        }
        if (qry.value(7).isNull()) {
            ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo")->setPlaceholderText("Select Supplier");
        } else {
            QComboBox *combo = ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo");
            combo->setCurrentIndex(matchingRow(qry.value(7).toString(), combo->model()));
        }
        if (!qry.value(8).isNull())
            ui->internalDetails->findChild<QWidget*>(QString::number(Supplier))->findChild<QPlainTextEdit*>("address")->setPlainText(qry.value(8).toString());
        ui->internalDetails->findChild<QLineEdit*>("delivered_by")->setText(qry.value(9).toString());
        break;
    }
    case MR: {
        if (docnum.isEmpty() && authorized) {
            qry.exec("SELECT date('now', 'localtime'), null, null, null, null "
                ", null, null, null, null, null, notes "
                    "FROM default_notes "
                    "JOIN userdata ON userdata.id = "%user->id%
                    " WHERE tflag = "%QString::number(MR)%";");
        } else {
            //An issuer cannot create MRs, they can only issue them.
            qry.exec("SELECT date(mr.date_requested,'localtime'), mr.requested_by, date(mr.date_authorized, 'localtime'), authorizer.name, authorizer.email "
                ",date(mr.date_issued, 'localtime'), issuer.name, issuer.email, mr.collected_by, mr.project, mr.notes "
                    "FROM mr "
                    "LEFT JOIN userdata AS 'authorizer' ON userdata.id = mr.authorized_by"
                    "LEFT JOIN userdata AS 'issuer' ON userdata.id = mr.issued_by "
                    "WHERE mr.num = "%docnum%";");
        }
        qry.next();
        //REMOVE EMAILS
        ui->detailsNames->setText("Date Requested:\nDate Authorized:\nAuthorized by:"%returnStringINN(qry.value(4).toString(), "\nAuthorizer Email:")%
                                  "\nDate Issued:\nIssued by:"%returnStringINN(qry.value(7).toString(), "\nIssuer Email:"));
        ui->detailsValues->setText(qry.value(0).toString()
                                   %"\n"%qry.value(2).toString()
                                   %"\n"%(authorized ? returnStringINN(qry.value(3).toString(), qry.value(3).toString(), user->name) : qry.value(3).toString())
                                   %returnStringINN(qry.value(4).toString(), "\n"%qry.value(4).toString())
                                   %"\n"%qry.value(5).toString()
                                   %"\n"%(authorized ? qry.value(6).toString() : returnStringINN(qry.value(6).toString(), qry.value(6).toString(), user->name))
                                   %returnStringINN(qry.value(7).toString(), "\n"%qry.value(7).toString()));
        ui->internalDetails->findChild<QLineEdit*>("requested_by")->setText(qry.value(1).toString());
        ui->internalDetails->findChild<QLineEdit*>("collected_by")->setText(qry.value(8).toString());
        if (qry.value(9).isNull()) {
            ui->internalDetails->findChild<QComboBox*>("project")->setPlaceholderText("Select Project");
        } else {
            QComboBox *combo = ui->info->findChild<QComboBox*>("project");
            combo->setCurrentIndex(matchingRow(qry.value(9).toString(), combo->model()));
        }
        ui->notes->setPlainText(qry.value(10).toString());
        break;
    }
    default:
        break;
    }
}

void CreateDocument::fetchCustomDetails(QSqlQuery qry, bool editable)
{
    ui->detailsGrid->removeWidget(ui->addCustom);
    ui->detailsGrid->removeWidget(ui->customLabel);
    qry.exec("SELECT count(*) FROM recurring_custom_details WHERE tflag = "%QString::number(tFlag)%";");
    qry.next();
    int rcdCount = qry.value(0).toInt(); //recurring Custom Details Count
    qry.exec("SELECT name, value FROM custom_details WHERE tflag = "%QString::number(tFlag)%" AND tnum = "%docnum%";");
    for (int row = 0; row < rcdCount; row++)
    {
        qry.next();

        QLabel *label = new QLabel;
        label->setAlignment(Qt::AlignRight);
        label->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        label->setText(qry.value(0).toString());
        QLineEdit *line = new QLineEdit;
        line->setAlignment(Qt::AlignRight);
        line->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        line->setText(qry.value(1).toString());
        QPushButton *deleteButton = new QPushButton;
        deleteButton->setMaximumSize(QSize(16,16));
        deleteButton->setText("x");
        deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        deleteButton->setStyleSheet("background-color: rgb(255, 0, 4);");
        connect(deleteButton, &QPushButton::clicked, this, &CreateDocument::deleteCustomDetail);

        ui->detailsGrid->addWidget(label, addDetailButtonRow, 0);
        ui->detailsGrid->addWidget(line, addDetailButtonRow, 1);
        ui->detailsGrid->addWidget(deleteButton, addDetailButtonRow, 2);

        deleteButton->setHidden(true);
        customDetailCount++;
        addDetailButtonRow++;
    }
    while (qry.next())
    {
        on_addCustom_clicked();
        qobject_cast<QLineEdit*>(ui->detailsGrid->itemAtPosition(addDetailButtonRow-1, 0)->widget())->setText(qry.value(0).toString());
        qobject_cast<QLineEdit*>(ui->detailsGrid->itemAtPosition(addDetailButtonRow-1, 1)->widget())->setText(qry.value(1).toString());
        if (!editable)
            qobject_cast<QPushButton*>(ui->detailsGrid->itemAtPosition(addDetailButtonRow-1, 2)->widget())->setHidden(true);
    }
    ui->detailsGrid->addWidget(ui->customLabel, addDetailButtonRow, 0);
    ui->detailsGrid->addWidget(ui->addCustom, addDetailButtonRow, 1);
}

//      === TOTALS ===

void CreateDocument::fetchTotals(QSqlQuery qry)
{
    qry.exec("SELECT discount_on_taxable_rate, discount_on_taxable, discount_on_tax_exempt_rate, discount_on_tax_exempt "
        ",tax_rate, tax, discount_after_tax_rate, discount_after_tax, discount_before_tax "
        "FROM "%docname%" WHERE num = "%docnum%";");
    qry.next();
    discountBeforeTax->setChecked(qry.value(8).toBool());
    fetchRateAndAmount(qry, 0, discountOnTaxable, ui->totalsGridLayout);
    fetchRateAndAmount(qry, 2, discountOnTaxExempt, ui->totalsGridLayout);
    fetchRateAndAmount(qry, 4, tax, ui->totalsGridLayout);
    fetchRateAndAmount(qry, 6, discountAfterTax, ui->totalsGridLayout);

    int row = customExpenseRowStart;
    qry.exec("SELECT name, rate, value FROM custom_expenses WHERE tflag = "%QString::number(tFlag)%" AND tnum = "%docnum%";");
    while (qry.next())
    {
        emit addExpenseButton->clicked();
        fetchRateAndAmount(qry, 1, qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(row, 3)->widget()), ui->totalsGridLayout, 1);
        row++;
    }
    emit taxableSubtotal->linkActivated("");
}

void CreateDocument::fetchRateAndAmount(QSqlQuery qry, int rateIndex, QLineEdit *line, QGridLayout *grid, bool custom) //This is a part of fetchTotals
{
    int row, trash;
    grid->getItemPosition(grid->indexOf(line), &row, &trash, &trash, &trash);

    if (qry.value(rateIndex).toDouble() == 0)
    {
        qobject_cast<QCheckBox*>(grid->itemAtPosition(row, 0)->widget())->setChecked(false);
        line->setText(qry.value(rateIndex+1).toString());
    } else {
        qobject_cast<QCheckBox*>(grid->itemAtPosition(row, 0)->widget())->setChecked(true);
        qobject_cast<QLineEdit*>(grid->itemAtPosition(row, 1)->widget())->setText(qry.value(rateIndex).toString());
    }
    if (custom)
        qobject_cast<QLineEdit*>(grid->itemAtPosition(row, 2)->widget())->setText(qry.value(rateIndex-1).toString());
}

void ResizableTable::calculateSubtotal(int row)
{
    //In my eyes it is preferrable to recalculate totals every time because floating point arithmetic is jank, but I can test again
    //Set the correct item total
    double total;
    total = item(row, cid.qty)->text().toDouble() * item(row, cid.unitPrice)->text().toDouble();
    qobject_cast<QLineEdit*>(cellWidget(row, cid.total))->setText(QString::number(total, 'f', 2));

    //Set the correct subtotals
    total = 0;
    double taxExemptTotal = 0;
    for (row = 0; row < finalRow; row++)
    {
        if (cellWidget(row, cid.taxable)->findChild<QCheckBox*>("box")->isChecked())
        {
            total +=  qobject_cast<QLineEdit*>(cellWidget(row, cid.total))->text().toDouble();
        } else {
            taxExemptTotal +=  qobject_cast<QLineEdit*>(cellWidget(row, cid.total))->text().toDouble();
        }
    }
    static_cast<CreateDocument*>(parentDoc)->setSubtotal(total, 0);
    static_cast<CreateDocument*>(parentDoc)->setSubtotal(taxExemptTotal, 1);
}

void CreateDocument::setSubtotal(double total, bool isTaxExempt) //This should only be accessed by the calculateSubtotal function. Perhaps this should be declared as private
{
    if (isTaxExempt)
    {
        QLabel *taxExemptSubtotal = qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(1,3)->widget());
        taxExemptSubtotal->setText(QString::number(total,'f',2));
        emit taxExemptSubtotal->linkActivated("");
    } else {
        QLabel *taxableSubtotal = qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(0,3)->widget());
        taxableSubtotal->setText(QString::number(total,'f',2));
        emit taxableSubtotal->linkActivated("");
    }
}

void CreateDocument::setTotalsRowHidden(QGridLayout *grid, QLabel *widget, bool hidden)
{
    int row, trash;
    grid->getItemPosition(grid->indexOf(widget), &row, &trash, &trash, &trash);
    qobject_cast<QLabel*>(grid->itemAtPosition(row, 2)->widget())->setHidden(hidden);
    qobject_cast<QLabel*>(grid->itemAtPosition(row, 3)->widget())->setHidden(hidden);
}
void CreateDocument::setTotalsRowHidden(QGridLayout *grid, QLineEdit *widget, bool hidden)
{
    int row, trash;
    grid->getItemPosition(grid->indexOf(widget), &row, &trash, &trash, &trash);
    qobject_cast<QCheckBox*>(grid->itemAtPosition(row, 0)->widget())->setHidden(hidden);
    qobject_cast<QLineEdit*>(grid->itemAtPosition(row, 1)->widget())->setHidden(hidden);
    qobject_cast<QLabel*>(grid->itemAtPosition(row, 2)->widget())->setHidden(hidden);
    qobject_cast<QLineEdit*>(grid->itemAtPosition(row, 3)->widget())->setHidden(hidden);
}
void CreateDocument::setTotalsRowHidden(QGridLayout *grid, QFrame *widget, bool hidden)
{
    int row, trash;
    grid->getItemPosition(grid->indexOf(widget), &row, &trash, &trash, &trash);
    qobject_cast<QFrame*>(grid->itemAtPosition(row, 3)->widget())->setHidden(hidden);
}

//  =====================================
//  == FUNCTIONS FOR SAVING / DELETING == These functions are the only ones that should actually be allowed to edit the database.
//  =====================================

void CreateDocument::DeleteDocument(QSqlQuery qry, QString docnum)
{
    QString tflag = QString::number(tFlag);
    qry.exec("DELETE FROM "%docname%"d WHERE "%docname%"_num = "%docnum%";");
    qry.exec("DELETE FROM "%docname%" WHERE num = "%docnum%";");
    qry.exec("DELETE FROM custom_details WHERE tnum = "%docnum%" AND tflag = "%tflag%";");
    qry.exec("DELETE FROM custom_expenses WHERE tnum = "%docnum%" AND tflag = "%tflag%";");
}

void CreateDocument::on_cancel_clicked()
{
    CreateDocument::close();
}

void CreateDocument::on_save_clicked() //Completed documents do not have this option.
{
    if (documentEntryValid()) {
        QSqlQuery qry(db);
        //Find a suitable docnum for a finalized document
        db.open();
        qry.exec("SELECT CASE WHEN (SELECT num FROM "%docname%" WHERE num > 10000) IS NULL "
                    "THEN 10001 "
                    "ELSE (SELECT (SELECT num FROM "%docname%" ORDER BY num DESC LIMIT 1) + 1) "
                 "END;");
        qry.next();
        db.close();

        if (docnum.toInt() == 0) //New Draft
            CreateDocument::storeTable(qry, NULL, qry.value(0).toString(), 1);
        else //Old draft
            CreateDocument::storeTable(qry, docnum, qry.value(0).toString(), 1);

        CreateDocument::close();
    }
}

void CreateDocument::on_saveDraft_clicked() //Completed documents do not have this option
{
    if (documentEntryValid()) {
        QSqlQuery qry(db);
        db.open();
        if (docnum.toInt() == 0) { //New Draft
            //Find a suitable docnum for a new draft
            qry.exec("SELECT CASE WHEN (SELECT num FROM "%docname%" WHERE num < 10000) IS NULL "
                        "THEN 1 "
                        "ELSE ((SELECT num FROM "%docname%" WHERE num < 10000 ORDER BY num DESC LIMIT 1) + 1) "
                    "END;");
            qry.next();
            docnum = qry.value(0).toString();
            CreateDocument::storeTable(qry, NULL, docnum);
        }
        else //Old draft
            CreateDocument::storeTable(qry, docnum, docnum);
        db.close();
        CreateDocument::close();
    }
}

void CreateDocument::storeCustomDetails(QSqlQuery qry, QString docnum)
{
    //custom details always start from row 1
    int row = 1;
    QString name, value;
    for (; row < recurringCustomDetailCount; row ++) {
        name = escapeSql(qobject_cast<QLabel*>(ui->detailsGrid->itemAtPosition(row, 0)->widget())->text());
        value = escapeSql(qobject_cast<QLineEdit*>(ui->detailsGrid->itemAtPosition(row, 1)->widget())->text());
        qry.exec("INSERT INTO custom_details (tflag, tnum, name, value, flag) VALUES ("%QString::number(tFlag)%","%docnum%","%name%","%value%",0);");
    }
    for (; row < addDetailButtonRow; row ++) {
        name = escapeSql(qobject_cast<QLineEdit*>(ui->detailsGrid->itemAtPosition(row, 0)->widget())->text());
        value = escapeSql(qobject_cast<QLineEdit*>(ui->detailsGrid->itemAtPosition(row, 1)->widget())->text());
        qry.exec("INSERT INTO custom_details (tflag, tnum, name, value, flag) VALUES ("%QString::number(tFlag)%","%docnum%","%name%","%value%",1);");
    }
}

void CreateDocument::storeCustomExpenses(QSqlQuery qry, QString docnum)
{
    for (int row = 0; row < customExpenseRowCount; row++)
    {
        qry.exec("INSERT INTO custom_expenses (tflag, tnum, name, rate, value) VALUES ("
                 %QString::number(tFlag)
                 %","%docnum
                 %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(customExpenseRowStart+row,2)->widget())->text())
                 %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(customExpenseRowStart+row,1)->widget())->text())
                 %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(customExpenseRowStart+row,3)->widget())->text())
            %");");
    }
}

QString CreateDocument::fetchTotalsValuesForSql()
{
    return QString::number(qobject_cast<QCheckBox*>(ui->internalDetailsGrid->itemAtPosition(ui->internalDetailsGrid->rowCount()-1,0)->widget())->isChecked())
            %","%escapeSql(qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(0,3)->widget())->text())
            %","%escapeSql(qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(1,3)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(2,1)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(2,3)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(3,1)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(3,3)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(7,1)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(7,3)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(8,1)->widget())->text())
            %","%escapeSql(qobject_cast<QLineEdit*>(ui->totalsGridLayout->itemAtPosition(8,3)->widget())->text())
            %","%escapeSql(qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(customExpenseRowStart+customExpenseRowCount+2,3)->widget())->text());
}

void CreateDocument::storeTable(QSqlQuery qry, QString oldDocNum, QString newDocNum, int status)
{
    //This function saves the table to the specified document number
    db.open();

    //FIRST: Clear out all existing data for this document. Everything will be overwritten.
    DeleteDocument(qry, oldDocNum);

    //Check whether the columns are valid, and insert into the appropriate tables.
    switch (tFlag) {
    case PR: {
        QString project = validateProject(qry, findChild<QComboBox*>("project")->currentText());
        qry.exec("INSERT INTO pr (num, date, date_needed, requested_by, notes, status, project, "%totalsColumns%") VALUES ("
                 %newDocNum
                 %",datetime('now')"
                  ",'"%ui->internalWidget->findChild<QDateTimeEdit*>("date")->date().toString("yyyy-MM-dd")%"'"
                  ","%user->id
                 %","%escapeSql(ui->notes->toPlainText())
                 %","%QString::number(status)
                 %","%escapeSql(project)
                 %","%fetchTotalsValuesForSql()
            %");");
        storeCustomExpenses(qry, newDocNum);
        break;
    }
    case QR: {
        QWidget *supplierWidget = ui->info->findChild<QWidget*>(QString::number(Supplier));
        QComboBox *supplierCombo = supplierWidget->findChild<QComboBox*>("combo");
        QString supplierName = supplierCombo->currentText();
        QString supplierAddressId = validateAddressId(qry, supplierWidget->findChild<QPlainTextEdit*>("address")->toPlainText());
        QString supplierInfo = supplierWidget->findChild<QPlainTextEdit*>("internal")->toPlainText();
        QString supplierId;
        if (supplierWidget->findChild<QStandardItemModel*>("supplierModel")->item(supplierCombo->currentIndex(), 3) != nullptr) {
            supplierId = validateSupplierId(qry, supplierWidget->findChild<QStandardItemModel*>("supplierModel")->item(supplierCombo->currentIndex(), 3)->text(), supplierName, supplierAddressId, supplierInfo);
        } else {
            supplierId = validateSupplierId(qry, QString(), supplierName, supplierAddressId, supplierInfo);
        }
        QString shippingAddressId = validateAddressId(qry, ui->info->findChild<QWidget*>(QString::number(ShippingAddress))->findChild<QPlainTextEdit*>("address")->toPlainText());
        QString billingAddressId = validateAddressId(qry, ui->info->findChild<QWidget*>(QString::number(BillingAddress))->findChild<QPlainTextEdit*>("address")->toPlainText());
        qry.exec("INSERT INTO qr (num, date, date_needed, requested_by, supplier_id, supplier_address_id, shipping_address_id, billing_address_id, notes, status) "
            "VALUES ("
                %newDocNum
                %",datetime('now')"
                 ",'"%ui->internalWidget->findChild<QDateTimeEdit*>("date")->date().toString("yyyy-MM-dd")%"'"
                 ","%user->id
                %","%supplierId
                %","%supplierAddressId
                %","%shippingAddressId
                %","%billingAddressId
                %","%escapeSql(ui->notes->toPlainText())
                %","%QString::number(status)
            %");");
        qDebug() << "INSERT INTO qr (num, date, date_needed, requested_by, supplier_id, supplier_address_id, shipping_address_id, billing_address_id, notes, status) "
            "VALUES ("
                %newDocNum
                %",datetime('now')"
                 ",'"%ui->internalWidget->findChild<QDateTimeEdit*>("date")->date().toString("yyyy-MM-dd")%"'"
                 ","%user->id
                %","%supplierId
                %","%supplierAddressId
                %","%shippingAddressId
                %","%billingAddressId
                %","%escapeSql(ui->notes->toPlainText())
                %","%QString::number(status)
            %");" << qry.lastError();
        break;
    }
    case PO: {
        break;
    }
    default:
        break;
    }

    storeCustomDetails(qry, newDocNum);
    table->storeRows(qry, newDocNum, QString::number(status));
}

void ResizableTable::storeRows(QSqlQuery qry, QString docnum, QString status)
{
    switch (tflag) {
    case PR: {
        for (int row = 0; row < finalRow; row++)
        {
            QString itemId = validateItemId(qry, row);
            QString supplierId = item(row, cid.supplier)->text().isEmpty() ? "null" : validateSupplierId(qry, row);
            qry.exec(QString("INSERT INTO prd (pr_num, supplier_id, item_id, qty, taxable, expected_unit_price, total, status) "
                "VALUES (%1, %2, %3, %4, %5, %6, %7, %8);")
                     .arg(docnum
                          ,supplierId
                          ,itemId
                          ,escapeSql(item(row,cid.qty)->text())
                          ,QString::number(cellWidget(row,cid.taxable)->findChild<QCheckBox*>("box")->isChecked())
                          ,escapeSql(item(row,cid.unitPrice)->text())
                          ,escapeSql(item(row,cid.total)->text())
                          ,status));
        }
        break;
    }
    case QR: {
        for (int row = 0; row < finalRow; row++)
        {
            QString itemId = validateItemId(qry, row);
            qry.exec(QString("INSERT INTO qrd (qr_num, item_id, prd_id, qty, notes) "
                "VALUES (%1, %2, %3, %4, %5);")
                     .arg(docnum
                          ,itemId
                          ,item(row, cid.prdId)->text()
                          ,escapeSql(item(row, cid.qty)->text())
                          ,escapeSql(item(row, cid.notes)->text())));
            qry.exec("UPDATE prd SET status = 2 WHERE id = "%item(row, cid.prdId)->text()%";");
        }
        break;
    }
    default:
        break;
    }
}

//      === VALIDATION FUNCTIONS === These functions will check whether an entry exists. If the entry does not exist, it will insert it. They always return the entry's ID.

bool CreateDocument::documentEntryValid()
{
    bool errorFlag = false;
    QString errorMessage = "This document cannot be saved because of the following error(s):";

    //Check for invalid entries in the document
    if (table->finalRow == 0)
        appendErrorMessage(&errorMessage, "At least one row must have an Item ID, Item Description, and Quantity", &errorFlag);

    switch (tFlag) {
    case PR: {
        if (ui->internalDetails->findChild<QComboBox*>("project")->currentText().isEmpty())
            appendErrorMessage(&errorMessage, "A project is required.", &errorFlag);
        break;
    }
    case QR: {
        if (ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo")->currentText().isEmpty())
            appendErrorMessage(&errorMessage, "A supplier must have a name.", &errorFlag);
        if (ui->info->findChild<QWidget*>(QString::number(ShippingAddress))->findChild<QPlainTextEdit*>("address")->toPlainText().isEmpty())
            appendErrorMessage(&errorMessage, "This document must have a shipping address", &errorFlag);
        if (ui->info->findChild<QWidget*>(QString::number(BillingAddress))->findChild<QPlainTextEdit*>("address")->toPlainText().isEmpty())
            appendErrorMessage(&errorMessage, "This document must have a billing address", &errorFlag);
        break;
    }
    case PO: {
        if (ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo")->currentText().isEmpty())
            appendErrorMessage(&errorMessage, "A supplier must have a name.", &errorFlag);
        if (ui->info->findChild<QWidget*>(QString::number(ShippingAddress))->findChild<QPlainTextEdit*>("address")->toPlainText().isEmpty())
            appendErrorMessage(&errorMessage, "This document must have a shipping address", &errorFlag);
        if (ui->info->findChild<QWidget*>(QString::number(BillingAddress))->findChild<QPlainTextEdit*>("address")->toPlainText().isEmpty())
            appendErrorMessage(&errorMessage, "This document must have a billing address", &errorFlag);
        break;
    }
    case RR: {
        if (ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("combo")->currentText().isEmpty())
            appendErrorMessage(&errorMessage, "A supplier must have a name.", &errorFlag);
        break;
    }
    case MR: {
        if (ui->internalDetails->findChild<QLineEdit*>("requested_by")->text().isEmpty())
            appendErrorMessage(&errorMessage, "The 'Requested By' field must not be empty.", &errorFlag);
        if (!authorized && ui->internalDetails->findChild<QLineEdit*>("collected_by")->text().isEmpty())
            appendErrorMessage(&errorMessage, "The 'Collected By' field must not be empty.", &errorFlag);
        break;
    }
    }

    //Check for invalid entries in the table
    for (int row = 0; row < table->finalRow; row++) {
        if (!table->itemEntryValid(row))
            appendErrorMessage(&errorMessage, "The Item in row "%QString::number(row+1)%" is missing either an ID or Description.", &errorFlag);
        if (!table->qtyEntryValid(row))
            appendErrorMessage(&errorMessage, "The Quantity in row "%QString::number(row+1)%" is not a valid decimal number.", &errorFlag);
        switch (tFlag) {
        case PO: {
            bool okay;
            table->item(row, table->cid.unitPrice)->text().toDouble(&okay);
            if (!okay || table->item(row, table->cid.unitPrice)->text().isEmpty())
                appendErrorMessage(&errorMessage, "The Unit Price in row "%QString::number(row+1)%" is not a valid decimal number.", &errorFlag);
        }
        default:
            break;
        }
    }

    //Return error message if possible
    if (errorFlag)
        throwErrorMessage(errorMessage, "Cannot Save Document");

    return !errorFlag;
}

inline bool CreateDocument::supplierEntryValid()
{
    return !(ui->info->findChild<QWidget*>(QString::number(Supplier))->findChild<QComboBox*>("name")->currentText().isEmpty());
}


QString validateProject(QSqlQuery qry, QString project)
{
    qry.exec("SELECT name FROM projects WHERE name = "%escapeSql(project)%";");
    qry.next();
    if (qry.value(0).isNull())
        qry.exec("INSERT INTO projects (name) VALUES ("%escapeSql(project)%");");
    return project;
}

QString validateAddressId(QSqlQuery qry, QString address, QString addressId)
{
    if (addressId.isNull())
        qry.exec("SELECT id FROM addresses WHERE address = "%escapeSql(address)%" LIMIT 1;");
    else
        qry.exec("SELECT id FROM addresses WHERE id = "%addressId%" AND address = "%escapeSql(address)%" LIMIT 1;");
    qry.next();
    if (qry.value(0).isNull())
        return insertNewAddress(qry, address);
    else
        return qry.value(0).toString();
}

QString CreateDocument::validateSupplierId(QSqlQuery qry, QString supplierId, QString supplierName, QString addressId, QString info)
{
    qry.exec("SELECT id FROM suppliers WHERE "
        "id = "%supplierId%
    "LIMIT 1;");
    qry.next();
    if (qry.value(0).isNull()) {
        supplierId = insertNewSupplier(qry, supplierName, addressId, info);
    }
    return supplierId;
}

QString ResizableTable::validateSupplierId(QSqlQuery qry, int row)
{
    if (item(row, cid.supplierId)->text().isEmpty()) {
        QString supplierName = qobject_cast<QLineEdit*>(cellWidget(row,cid.supplier))->text();
        QString supplierId = insertNewSupplier(qry, supplierName);
        for (int i = row; i < finalRow; i++) { //Loop so repeated supplier names in the doc will all have the same supplierId
            if (qobject_cast<QLineEdit*>(cellWidget(i,cid.supplier))->text() == supplierName)
                item(i,cid.supplierId)->setText(supplierId);
        }
        return supplierId;
    } else {
        return item(row, cid.supplierId)->text();
    }
}

bool ResizableTable::itemEntryValid(int row)
{
    if (qobject_cast<QLineEdit*>(cellWidget(row, cid.itemNum))->text().isEmpty() || qobject_cast<QLineEdit*>(cellWidget(row, cid.itemDesc))->text().isEmpty())
        return false;
    else
        return true;
}

bool ResizableTable::qtyEntryValid(int row)
{
    bool okay;
    item(row, cid.qty)->text().toDouble(&okay);
    return okay;
}

QString ResizableTable::validateItemId(QSqlQuery qry, int row)
{
    if (item(row,cid.itemId)->text().isEmpty())
    {
        QString itemId = insertNewItem(qry
                                       ,qobject_cast<QLineEdit*>(cellWidget(row,cid.itemNum))->text()
                                       ,qobject_cast<QLineEdit*>(cellWidget(row,cid.itemDesc))->text()
                                       ,qobject_cast<QLineEdit*>(cellWidget(row,cid.unit))->text()
                                       ,qobject_cast<QLineEdit*>(cellWidget(row,cid.cat))->text());
        item(row,cid.itemId)->setText(itemId);
        return itemId;
    }
    return item(row,cid.itemId)->text();
}

QString insertNewAddress(QSqlQuery qry, QString address)
{
    qry.exec("INSERT INTO addresses (address) VALUES ("%escapeSql(address)%");");
    qry.exec("SELECT id FROM addresses ORDER BY id DESC LIMIT 1;");
    qry.next();
    return qry.value(0).toString();
}

QString insertNewSupplier(QSqlQuery qry, QString supplierName, QString addressId, QString info)
{
    qry.exec("INSERT INTO suppliers (name"%returnStringINN(addressId, ",address_id")%returnStringINN(info, ",info")%")"
        "VALUES ("%escapeSql(supplierName)%returnStringINN(addressId, ","%addressId)%returnStringINN(info, escapeSql(info))%");");
    qry.exec("SELECT id FROM suppliers ORDER BY id DESC LIMIT 1;");
    qry.next();
    return qry.value(0).toString();
}

QString insertNewItem(QSqlQuery qry, QString itemNum, QString itemDesc, QString unit, QString category)
{
    qry.exec(QString("INSERT INTO items (num, desc, unit, cat) VALUES (%1, %2, %3, %4);")
             .arg(escapeSql(itemNum)
                  ,escapeSql(itemDesc)
                  ,escapeSql(unit)
                  ,escapeSql(category)));
    qry.exec("SELECT id FROM items ORDER BY id DESC LIMIT 1;");
    qry.next();
    return qry.value(0).toString();
}

//  ===========================
//  == CONVENIENCE FUNCTIONS ==
//  ===========================

void appendErrorMessage(QString *orignalMessage, QString appendage, bool *error)
{
    *error = true;
    orignalMessage->append("\n\t- "%appendage);
}

void throwErrorMessage(QString message, QString title) //This works, but it is obtrusive and accusatory. The main problem is that you have to press a button to make the message go away, which is annoying.
{
    QMessageBox *messageBox = new QMessageBox();
    messageBox->setWindowTitle(title);
    messageBox->setText(message);
    messageBox->exec();
    delete messageBox;
}

inline void emitSignalOnCompletion(QLineEdit *line) //This exists because editingFinished is not emitted when QCompleter is activated by clicking
{
    QObject::connect(line->completer(), static_cast<void(QCompleter::*)(const QModelIndex &index)>(&QCompleter::activated), line,
            [line] () { emit line->editingFinished(); } );
}

inline bool POIsValidAndOpen(QSqlQuery qry, QString poNum)
{
    qry.exec("SELECT * FROM po WHERE num = "%escapeSql(poNum)%" AND status > 0 AND status < 5;");
    qry.next();
    if (qry.value(0).isNull())
        return false;
    else
        return true;
}

inline QString stringAt(QCompleter *completer, int column)
{
    return completer->currentIndex().siblingAtColumn(column).data().toString();
}

inline int CreateDocument::matchingRow(QString matchString, QAbstractItemModel *model, int matchColumn) //Returns the first row that matches the given string in the ItemModel
{
    return model->match(model->index(0,matchColumn), Qt::DisplayRole, matchString, 1, Qt::MatchExactly).at(0).row();
}
