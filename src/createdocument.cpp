#include "createdocument.h"
#include "ui_createdocument.h"
#include "global.h"

#include <QtGlobal>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QCompleter>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStringBuilder>

#include <QDebug>
#include <QSqlError>

QString docname;

QString returnStringINN(QVariant sqlValue, QString ifNotNull = "%1\n", QString ifNull = "")
{
    if (sqlValue.isNull())
        return ifNull;
    else
        return ifNotNull.arg(sqlValue.toString());
}

CreateDocument::CreateDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDocument)
{
    ui->setupUi(this);
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);

    cfRow = 1; // The row in the gridLayout where the first non-rcf (recurring custom field) custom row lives
    customButtonRow = 1; // The row in the gridLayout where the addCustom button lives
    //! FIRST: Set up the elements that are shared between all documents
    //!     (ie. The header: company name, logo, address)

    //Change the logo to be user editable, auto-resizing (with fixed aspect ratio),
    ui->logo->setPixmap(qApp->applicationDirPath()+"/logo/logo.png");
    ui->logo->setPixmap(ui->logo->pixmap().scaledToHeight(75));

    simdb.open();
    qry.exec("SELECT company.name, company.phone, company.fax, company.email, company.website, shipping.address, billing.address "
        "FROM company "
        "INNER JOIN addresses AS 'shipping' ON shipping.id = company.shipping_address_id "
        "INNER JOIN addresses AS 'billing' ON billing.id = company.billing_address_id "
    ";");
    qry.next();
    simdb.close();
    ui->companyName->setText(qry.value(0).toString());
    ui->companyAddress->setText(QString("%1%2%3%4%5").arg(qry.value(6).toString()
                                                          ,returnStringINN(qry.value(1),"\nPhone: %1")
                                                          ,returnStringINN(qry.value(2)," | Fax: %1")
                                                          ,returnStringINN(qry.value(3),"\n%1")
                                                          ,returnStringINN(qry.value(4),"\n%1")));

    //      SECOND: Set up elements and/or hide elements that are document specific
    switch (tflag.toInt()) {
    case 0: { //If PR
        ui->shipToLabel->hide();
        ui->shippingAddress->hide();
        ui->supplierLabel->hide();
        ui->supplierAddress->hide();

        ui->prLabel->hide();
        ui->prnum->hide();
        ui->qrLabel->hide();
        ui->qrnum->hide();
        ui->poLabel->hide();
        ui->ponum->hide();
        ui->invoiceLabel->hide();
        ui->invoicenum->hide();

        ui->doctype->setText("Purchase Requisition #");
        docname = "pr";
        break;
    }
    case 1: { //If QR
        ui->qrLabel->hide();
        ui->qrnum->hide();
        ui->poLabel->hide();
        ui->ponum->hide();
        ui->invoiceLabel->hide();
        ui->invoicenum->hide();

        ui->doctype->setText("Quotation Request #");
        docname = "qr";
        break;
    }
    case 2: { //If PO
        ui->shippingAddress->setText(qry.value(5).toString());
        ui->doctype->setText("Purchase Order #");
        docname = "po";
        break;
    }
    case 3: { //If RR
        ui->shipToWidget->hide();
        ui->doctype->setText("Receiving Report #");
        docname = "rr";
        break;
    }
    case 4: { //If MR
        ui->doctype->setText("Material Requisition #");
        docname = "mr";
        break;
    }
    }

    //     THIRD: Initialize the table
    table = new ResizableTable(this);
    ui->verticalLayout_3->addWidget(table);

    //      LAST: Import elements from an existing document, or set defaults if new draft
    if (docnum.isEmpty()) //If New Draft
    {
        ui->docNumber->setText("DRAFT");
        ui->update->hide();
        ui->deleteDraft->hide();
        simdb.open();
        qry.exec("SELECT date('now', 'localtime'), name, email FROM userdata WHERE id = "+currentUser+";");
        qry.next();
        ui->detailsNames->setText(QString("Date:\nCreated by:%1").arg(returnStringINN(qry.value(2),"\nEmail:")));
        ui->detailsValues->setText(QString("%1%2%3")
                                   .arg(qry.value(0).toString()+"\n"
                                        ,qry.value(1).toString()
                                        ,returnStringINN(qry.value(2),"\n%1")));
        insertRecurringCustomDetails(qry, tflag, &customButtonRow);
        simdb.close();
    }
    else if (docnum.toInt() < 10000) //If old draft
    {
        ui->docNumber->setText(docnum);
        ui->doctype->setText("DRAFT "+ui->doctype->text());
        ui->update->hide();
        simdb.open();
        fetchDetails(qry, tflag.toInt(), docnum);
        fetchCustomDetails(qry, &customButtonRow, tflag, docnum, true);
        table->fetchRows(qry, tflag.toInt(), docnum);
        simdb.close();
    }
    else //If finalized document
    {
        ui->docNumber->setText(docnum);
        ui->saveDraft->hide();
        ui->save->hide();
        ui->deleteDraft->hide();
        simdb.open();
        fetchDetails(qry, tflag.toInt(), docnum);
        fetchCustomDetails(qry, &customButtonRow, tflag, docnum, false);
        table->fetchRows(qry, tflag.toInt(), docnum);
        simdb.close();
        //I need to update the custom fields functions. There should be one to insert deletable custom fields, and one to insert constant custom fields.
    }

    //      EXTRA: Create delete Message Box
    QMessageBox *confirm = new QMessageBox(this);
    confirm->setText("Are you sure you would like to delete this document?");
    confirm->addButton("Confirm", QMessageBox::AcceptRole);
    confirm->addButton("Cancel", QMessageBox::RejectRole);
    confirm->setWindowTitle("Delete?");
    connect(ui->deleteDraft, &QPushButton::clicked, confirm, &QMessageBox::exec);
    connect(confirm, &QMessageBox::accepted, this, &CreateDocument::DeleteDocument);
    connect(confirm, &QMessageBox::rejected, confirm, &QMessageBox::close);
}

ResizableTable::ResizableTable(CreateDocument *parent) :
    QTableWidget(parent)
{

    // Initialize classes and variables
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    itemModel = new QSqlQueryModel(this);
    projectModel = new QSqlQueryModel(this);
    supplierModel = new QSqlQueryModel(this);

    simdb.open();
    itemModel->setQuery("SELECT id, num, desc, unit FROM items;", simdb);
    while (itemModel->canFetchMore()) {itemModel->fetchMore();}
    supplierModel->setQuery("SELECT id, name FROM suppliers;", simdb);
    while (supplierModel->canFetchMore()) {supplierModel->fetchMore();}
    projectModel->setQuery("SELECT name FROM projects;", simdb);
    while (projectModel->canFetchMore()) {projectModel->fetchMore();}
    simdb.close();

    setRowCount(0);

    initializeColumns(tflag.toInt());

    // Create base table
    setColumnCount(colNames.count());
    setHorizontalHeaderLabels(colNames);
    appendRow();

    // Connect signals and slots
    connect(this, &ResizableTable::cellChanged,
            [=] (int row, int column)
    {
        if (row == finalRow && !item(row,column)->text().isEmpty())
            appendRow();

        // This is the create totals fucntion. It will have to become dramatically more robust to account for all doctypes. This should probably not be a lambda function.
        if (column == cid.unitPrice || column == cid.qty)
        {
            double total = 0;
            for (int r = 0; r < finalRow; r++)
                total += (item(r, cid.unitPrice)->text().toDouble() * item(r, cid.qty)->text().toDouble());
            static_cast<CreateDocument*>(parent)->setTotal(total);
        }
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    menu = new QMenu(this);
    deleteRow = new QAction(this);
    deleteRow->setText("Delete Row");
    menu->addAction(deleteRow);
    connect(this, &ResizableTable::customContextMenuRequested, [=] (const QPoint &pos) { ResizableTable::onCustomContextMenuRequested(pos); });
}



void ResizableTable::initializeColumns(int tflag)
{
    switch (tflag) {
    case 0: {
        colNames = {"id", "Item ID", "Description", "Category", "Qty", "Unit", "Project", "sid", "Recommended Supplier", "Expected Unit Price", "Status"};
        cid.project = 6;
        cid.supplierId = 7;
        cid.supplier = 8;
        cid.unitPrice = 9;
        cid.status = 10;
        break;
    }
    case 1: {
        colNames = {"id", "Item ID", "Description", "Category", "Qty", "Unit", "Unit Price", "Total", "Status"};
        cid.unitPrice = 6;
        cid.total = 7;
        cid.status = 8;
        break;
    }
    case 2: {
        colNames = {"id", "Item ID", "Description", "Category", "Qty", "Unit", "Unit Price", "Tax?", "Total", "Status"};
        cid.unitPrice = 6;
        cid.tax = 7;
        cid.total = 8;
        cid.status = 9;
        break;
    }
    case 3: {
        colNames = {"id", "Item ID", "Description", "Category", "Qty", "Unit", "Condition"};
        cid.status = 6;
        break;
    }
    case 4: {
        colNames = {"id", "Item ID", "Description", "Category", "Qty", "Unit"};
        break;
    }
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

void CreateDocument::setTotal(double total)
{
    ui->totals->setText("Total: "%QString::number(total,'f',2));
}

CreateDocument::~CreateDocument()
{
    delete ui;
}

//!      ==============================================
//!      ===== INTERNAL/EXTERNAL & CUSTOM DETAILS =====
//!      ==============================================

void CreateDocument::fetchDetails(QSqlQuery qry, int tFlag, QString docnum)
{
    switch (tFlag) {
    case 0: {
        ui->detailsNames->setText("Date:\nCreated by:");
        qry.exec("SELECT pr.date, pr.date_needed, pr.notes, userdata.name FROM pr JOIN userdata ON userdata.id = pr.requested_by WHERE pr.num = "+docnum+";");
        qry.next();
        ui->detailsValues->setText(qry.value(0).toString()+"\n"+qry.value(3).toString());
        ui->dateNeeded->setDate(qry.value(1).toDate());
        ui->notes->setPlainText(qry.value(2).toString());
    }
    case 1: {

        break;
    }
    case 2: {
        break;
    }
    case 3: {
        break;
    }
    case 4: {
        break;
    }
    }
}

void CreateDocument::insertRecurringCustomDetails(QSqlQuery qry, QString tableFlag, int *row)
{
    ui->gridLayout->removeWidget(ui->addCustom);
    ui->gridLayout->removeWidget(ui->customLabel);
    qry.exec("SELECT name FROM recurring_custom_fields WHERE tflag = "%tableFlag%";");
    while (qry.next())
    {
        QLabel *label = new QLabel(ui->details);
        label->setAlignment(Qt::AlignRight);
        label->setText(qry.value(0).toString());
        ui->gridLayout->addWidget(label, *row, 0);
        QLineEdit *line = new QLineEdit(ui->details);
        line->setAlignment(Qt::AlignRight);
        line->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        ui->gridLayout->addWidget(line, *row, 1);
        *row += 1;
    }
    ui->gridLayout->addWidget(ui->customLabel, *row, 0);
    ui->gridLayout->addWidget(ui->addCustom, *row, 1);
    cfRow = *row;
}

void CreateDocument::fetchCustomDetails(QSqlQuery qry, int *row, QString tableFlag, QString docNum, bool editable)
{
    ui->gridLayout->removeWidget(ui->addCustom);
    ui->gridLayout->removeWidget(ui->customLabel);
    qry.exec("SELECT count(*) FROM recurring_custom_fields WHERE tflag = "%tableFlag%";");
    qry.next();
    int rcfCount = qry.value(0).toInt();
    qry.exec("SELECT name, value FROM custom_fields WHERE tflag = "%tableFlag%" AND tnum = "%docNum%";");
    for (int i = 0; i < rcfCount; i++)
    {
        qry.next();
        QLabel *label = new QLabel(ui->details);
        label->setAlignment(Qt::AlignRight);
        label->setText(qry.value(0).toString());
        ui->gridLayout->addWidget(label, *row, 0);
        QLineEdit *line = new QLineEdit(ui->details);
        line->setAlignment(Qt::AlignRight);
        line->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        line->setText(qry.value(1).toString());
        ui->gridLayout->addWidget(line, *row, 1);
        *row += 1;
    }
    cfRow = *row;
    while (qry.next())
    {
        QLineEdit *line1 = new QLineEdit(ui->details);
        line1->setAlignment(Qt::AlignRight);
        line1->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        line1->setText(qry.value(0).toString());
        ui->gridLayout->addWidget(line1, *row, 0);
        QLineEdit *line2 = new QLineEdit(ui->details);
        line2->setAlignment(Qt::AlignRight);
        line2->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        line2->setText(qry.value(1).toString());
        ui->gridLayout->addWidget(line2, *row, 1);
        if (editable)
        {
            QPushButton *deleteButton = new QPushButton;
            deleteButton->setMaximumSize(QSize(16,16));
            deleteButton->setText("x");
            deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            deleteButton->setStyleSheet("background-color: #ff0000;");
            ui->gridLayout->addWidget(deleteButton, *row, 2);
            connect(deleteButton, &QPushButton::clicked, this, &CreateDocument::deleteCustomDetail);
        }
        *row += 1;
    }
    ui->gridLayout->addWidget(ui->customLabel, *row, 0);
    ui->gridLayout->addWidget(ui->addCustom, *row, 1);
    if (!editable)
        cfRow = *row;
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

    ui->gridLayout->removeWidget(ui->addCustom);
    ui->gridLayout->removeWidget(ui->customLabel);
    ui->gridLayout->addWidget(custom1, customButtonRow, 0);
    ui->gridLayout->addWidget(custom2, customButtonRow, 1);
    ui->gridLayout->addWidget(deleteButton, customButtonRow, 2);
    ui->gridLayout->addWidget(ui->customLabel, customButtonRow+1, 0);
    ui->gridLayout->addWidget(ui->addCustom, customButtonRow+1, 1);

    customButtonRow++;
}

void CreateDocument::deleteCustomDetail()
{
    //This may cause memory leaks. Verify.
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int index = ui->gridLayout->indexOf(button);
    ui->gridLayout->itemAt(index-0)->widget()->~QWidget();
    ui->gridLayout->itemAt(index-0)->~QLayoutItem();
    ui->gridLayout->itemAt(index-1)->widget()->~QWidget();
    ui->gridLayout->itemAt(index-1)->~QLayoutItem();
    ui->gridLayout->itemAt(index-2)->widget()->~QWidget();
    ui->gridLayout->itemAt(index-2)->~QLayoutItem();
    customButtonRow--;
}

//!      ==========================
//!      ====== SAVE BUTTONS ======
//!      ==========================

void CreateDocument::on_cancel_clicked()
{
    CreateDocument::close();
}

void CreateDocument::on_save_clicked()
{
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("SELECT (SELECT num FROM "+docname+" ORDER BY num DESC LIMIT 1) + 1;");
    qry.next();
    simdb.close();

    if (docnum.toInt() == 0) //New Draft
    {
        CreateDocument::storeTable(tflag.toInt(), NULL, qry.value(0).toString());
    }
    else if (docnum.toInt() < 10000) //Old draft
    {
        CreateDocument::storeTable(tflag.toInt(), docnum, qry.value(0).toString());
    }
}

void CreateDocument::on_saveDraft_clicked() //Completed documents do not have this option
{
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);
    if (docnum.toInt() == 0) //New Draft
    {
        //Find a suitable docNum. storeTable() saves to the specified docNum
        simdb.open();
        qry.exec("SELECT CASE WHEN (SELECT num FROM "+docname+" WHERE num < 10000) IS NULL "
                        "THEN 1 "
                        "ELSE ((SELECT num FROM "+docname+" WHERE num < 10000 ORDER BY num DESC LIMIT 1) + 1) "
                    "END;");
        qry.next();
        simdb.close();
        docnum = qry.value(0).toString();
        CreateDocument::storeTable(tflag.toInt(), docnum, docnum);
        CreateDocument::close();
    }
    else //Old draft
    {
        CreateDocument::storeTable(tflag.toInt(), docnum, docnum);
        CreateDocument::close();
    }
}

void CreateDocument::storeTable(int tableFlag, QString oldDocNum, QString newDocNum)
{
    //This function saves the table as a draft that can be opened later
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);

    //Clear out any and all existing values for this table. This is a draft so any old data should be removed.
    simdb.open();
    qry.exec("DELETE FROM "%docname%"d WHERE "%docname%"_num = "%oldDocNum%";");
    qry.exec("DELETE FROM "%docname%" WHERE num = "%oldDocNum%";");
    qry.exec("DELETE FROM custom_fields WHERE tnum = "%oldDocNum%" AND tflag = "%QString::number(tableFlag)%";");
    switch (tableFlag) {
    //Check whether the columns are valid, and insert into the appropriate tables.
    case 0: { //PR
        qry.exec(QString("INSERT INTO pr (num, date, date_needed, requested_by, notes, status) VALUES (%1,datetime('now','localtime'),'%2',%3,%4,0);")
                 .arg(newDocNum
                      ,ui->dateNeeded->date().toString("yyyy-MM-dd")
                      ,currentUser
                      ,escapeSql(ui->notes->toPlainText())));
        table->storeTableDetails(qry, newDocNum, tableFlag);
        break;
    }
    case 1: { //QR
        break;
    }
    case 2: { //PO
        break;
    }
    }
    //Insert custom rows. This should become a function.
    QString customName, customValue;
    for (int i = 1; i < cfRow; i++)
    {
        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt(i*2)->widget()))
        {
            customName = escapeSql(label->text());
        } else {
            customName = escapeSql(qobject_cast<QLineEdit *>(ui->gridLayout->itemAt((i*2))->widget())->text());
        }
        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt((i*2)+1)->widget()))
        {
            customValue = escapeSql(label->text());
        } else {
            customValue = escapeSql(qobject_cast<QLineEdit *>(ui->gridLayout->itemAt(i*2+1)->widget())->text());
        }
        qry.exec(QString("INSERT INTO custom_fields (tflag, tnum, name, value) VALUES (%1, %2, %3, %4);")
                .arg(QString::number(tableFlag), newDocNum, customName, customValue));
    }
    for (int i = cfRow; i < customButtonRow; i++)
    {
        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt((i*2)+i-cfRow)->widget()))
            customName = escapeSql(label->text());
        else
            customName = escapeSql(qobject_cast<QLineEdit *>(ui->gridLayout->itemAt((i*2)+i-cfRow)->widget())->text());

        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt((i*2)+i-cfRow+1)->widget()))
            customValue = escapeSql(label->text());
        else
            customValue = escapeSql(qobject_cast<QLineEdit *>(ui->gridLayout->itemAt((i*2)+i-cfRow+1)->widget())->text());

        qry.exec(QString("INSERT INTO custom_fields (tflag, tnum, name, value) VALUES (%1, %2, %3, %4);")
                .arg(QString::number(tableFlag), newDocNum, customName, customValue));
    }
    simdb.close();
}

void ResizableTable::storeTableDetails(QSqlQuery qry, QString docnum, int tableFlag)
{
    switch (tableFlag) {
    case 0: {
        for (int i = 0; i < finalRow; i++)
        {
            // Ensure item is valid
            if (item(i,cid.itemId)->text().isEmpty())
            {
                qry.exec(QString("INSERT INTO items (num, desc, unit, cat) VALUES (%1, %2, %3, %4);")
                         .arg(escapeSql(qobject_cast<QLineEdit*>(cellWidget(i,cid.itemNum))->text())
                              ,escapeSql(qobject_cast<QLineEdit*>(cellWidget(i,cid.itemDesc))->text())
                              ,escapeSql(item(i,cid.unit)->text())
                              ,escapeSql(item(i,cid.cat)->text())));
                qry.exec("SELECT id FROM items ORDER BY id DESC LIMIT 1;");
                qry.next();
                item(i,cid.itemId)->setText(qry.value(0).toString());
            }
            // Ensure supplier is valid
            if (item(i,cid.itemId)->text().isEmpty())
            {
                QString supplierName = qobject_cast<QLineEdit*>(cellWidget(i,cid.supplier))->text();
                qry.exec("INSERT INTO suppliers (name) VALUES ("%escapeSql(supplierName)%");");
                qry.exec("SELECT id FROM suppliers ORDER BY id DESC LIMIT 1;");
                qry.next();
                for (int j = i; j < finalRow; j++) //Loop so repeated supplier names in the doc will all have the same number
                {
                    if (qobject_cast<QLineEdit*>(cellWidget(j,cid.supplier))->text() == supplierName)
                        item(j,cid.supplierId)->setText(qry.value(0).toString());
                }
            }
            // Ensure project is valid
            qry.exec("SELECT name FROM projects WHERE name = "%escapeSql(qobject_cast<QLineEdit*>(cellWidget(i,cid.project))->text())%";");
            qry.next();
            if (qry.value(0).isNull())
                qry.exec("INSERT INTO projects (name) VALUES ("%escapeSql(qobject_cast<QLineEdit*>(cellWidget(i,cid.project))->text())%");");

            qry.exec(QString("INSERT INTO prd (pr_num, supplier_id, project, item_id, qty, unit, expected_price) "
                "VALUES (%1, %2, %3, %4, %5, %6, %7);")
                     .arg(docnum
                          ,item(i,cid.supplierId)->text()
                          ,escapeSql(qobject_cast<QLineEdit*>(cellWidget(i,cid.project))->text())
                          ,item(i,cid.itemId)->text()
                          ,escapeSql(item(i,cid.qty)->text())
                          ,escapeSql(item(i,cid.unit)->text())
                          ,escapeSql(item(i,cid.unitPrice)->text())));
        }
        break;
    }
    case 1: {
        for (int i = 0; i < finalRow; i++)
        {

        }
        break;
    }
    }
}

//!      ==================================
//!      ===== RESIZABLE TABLE WIDGET =====
//!      ==================================

void ResizableTable::resizeEvent(QResizeEvent *event) //This is called multiple times. Slow.
{
    int w = width();
    //Resizing windows based on tFlag
    switch (tflag.toInt()) {
    case 0: {
        //{"id", "Item ID", "Description", "Qty", "Unit", "Project", "sid", "Recommended Supplier", "Expected Price", "Status"};
        w = w / 15;
        setColumnWidth(cid.itemNum  ,w*2);
        setColumnWidth(cid.itemDesc ,w*3);
        setColumnWidth(cid.cat      ,w*1);
        setColumnWidth(cid.qty      ,w*1);
        setColumnWidth(cid.unit     ,w*2);
        setColumnWidth(cid.project  ,w*3);
        setColumnWidth(cid.supplier ,w*2);
        setColumnWidth(cid.unitPrice,w*1);
        horizontalHeader()->setStretchLastSection(true);
    }
    }
}

void CreateDocument::DeleteDocument()
{
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);
    simdb.open();
    qry.exec("DELETE FROM "%docname%" WHERE num = "%docnum%";");
    qry.exec("DELETE FROM "%docname%"d WHERE "%docname%"_num = "%docnum%";");
    qry.exec("DELETE FROM custom_fields WHERE tflag = "%tflag%" AND tid = "%docnum%";");
    simdb.close();
    CreateDocument::close();
}

void ResizableTable::appendRow()
{
    finalRow = rowCount();
    setRowCount(rowCount()+1);
    for (int i = 0; i < columnCount(); i++)
    {
        setItem(finalRow,i, new QTableWidgetItem());
    }

    customLineEdit(finalRow, Item);
    setColumnHidden(cid.itemId,true);

    switch (tflag.toInt()) {
    case 0: { //PR
        customLineEdit(finalRow, Supplier);
        customLineEdit(finalRow, Project);
        setColumnHidden(cid.supplierId,true); //sid
        setColumnHidden(cid.status,true); //status
        break;
    }
    case 1: { //QR
        setColumnHidden(cid.status, true); //status
        break;
    }
    case 2: { //PO
        setColumnHidden(cid.status, true); //status
        break;
    }
    }
}

void ResizableTable::fetchRows(QSqlQuery qry, int tFlag, QString docnum)
{
    switch (tFlag) {
    case 0: {
        qry.exec("SELECT prd.item_id, items.num, items.desc, items.cat, prd.qty, items.unit, prd.project, prd.supplier_id, suppliers.name, prd.expected_price, prd.is_open "
        "FROM prd "
        "LEFT JOIN items ON items.id = prd.item_id "
        "LEFT JOIN suppliers ON prd.supplier_id = suppliers.id "
        "WHERE prd.pr_num = "+docnum+";");
        break;
    }
    case 1: {
        qry.exec("SELECT qrd.item_id, items.num, items.desc, items.cat, qrd.qty, qrd.unit, qrd.unit_price, qrd.total, qrd.status "
            "FROM qrd "
            "LEFT JOIN items ON items.id = qrd.item_id "
            "WHERE qrd.qr_num = "+docnum+";");
        break;
    }
    case 2: {
        qry.exec("SELECT pod.item_id, items.num, items.desc, items.cat, pod.unit, pod.unit_price, pod.taxable, pod.total, pod.status "
            "FROM pod "
            "LEFT JOIN items ON items.id = pod.item_id "
            "WHERE pod.po_num = "+docnum+";");
        break;
    }
    case 3: {
        qry.exec("SELECT rrd.item_id, items.num, items.desc, items.cat, rrd.qty, rrd.unit, rrd.condition "
            "FROM rrd "
            "LEFT JOIN items ON items.id = rrd.item_id "
            "WHERE rrd.rr_num = "+docnum+";");
        break;
    }
    case 4: {
        qry.exec("SELECT mrd.item_id, items.num, items.desc, items.cat, mrd.qty, mrd.unit "
            "FROM mrd "
            "LEFT JOIN items ON items.id = mrd.item_id "
            "WHERE mrd.mr_num = "+docnum+";");
        break;
    }
    }
    //Table items can have a line edit as their primary display. The following loop accounts for this.
    for (int i = 0; qry.next(); i++)
    {
        for (int j = 0; j < colNames.count(); j++)
        {
            if (QLineEdit *line = qobject_cast<QLineEdit*>(cellWidget(i,j)))
            {
                line->setText(qry.value(j).toString());
                line->completer()->setCompletionPrefix(qry.value(j).toString());
                emit line->editingFinished();
            }
            else
                item(i,j)->setText(qry.value(j).toString());
        }
    }
}

void ResizableTable::customLineEdit(int row, lineType type)
{
    QLineEdit *line = new QLineEdit(this);
    line->setFrame(false);
    //For now, these columns will always be in the same place.
    //Add documentation here
    switch (type) {
    case Item: {
        QCompleter *completer = new QCompleter(itemModel, line);
        completer->setCompletionColumn(1);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseInsensitive);

        QLineEdit *descLine = new QLineEdit(this);
        descLine->setFrame(false);
        QStandardItemModel *descModel = new QStandardItemModel(1,2,descLine);
        descModel->setItem(0,0,new QStandardItem(""));
        descModel->setItem(0,1,new QStandardItem(""));
        QCompleter *descCompleter = new QCompleter(descModel, descLine);
        descCompleter->setFilterMode(Qt::MatchContains);
        descCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        descCompleter->setCompletionColumn(1);

        QObject::connect(line, &QLineEdit::editingFinished,
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
                    if(completer->currentCompletion() == line->text())
                    {
                        item(row, cid.itemId)->setText(stringAt(completer,0));
                        descModel->item(0,1)->setText(stringAt(completer,2));
                        descModel->item(0,0)->setText(stringAt(completer,0));
                        descLine->setText(stringAt(completer,2));
                        descCompleter->setCompletionPrefix(stringAt(completer,2));
                        item(row,cid.unit)->setText(stringAt(completer,3));
                        line->setStyleSheet(cssclear1);
                        descLine->setStyleSheet(cssclear1);
                    }
                    else
                    {
                        item(row, cid.itemId)->setText("");
                        descModel->item(0,1)->setText("");
                        descModel->item(0,0)->setText("");
                        item(row, cid.unit)->setText("");
                        line->setStyleSheet(cssalert1);
                        descLine->setStyleSheet(cssalert1);
                    }
                });
        QObject::connect(descLine, &QLineEdit::editingFinished,
               [=] ()
                {
                    // This does not need a setCompleterPrefix function since there can only ever be one completion in this column
                    if (descCompleter->currentCompletion() == descLine->text())
                    {
                        item(row, cid.itemId)->setText(stringAt(descCompleter,0));
                        line->setStyleSheet(cssclear1);
                        descLine->setStyleSheet(cssclear1);
                    }
                    else
                    {
                        item(row, cid.itemId)->setText("");
                        line->setStyleSheet(cssalert1);
                        descLine->setStyleSheet(cssalert1);
                    }
                });

        line->setCompleter(completer);
        descLine->setCompleter(descCompleter);
        this->setCellWidget(row, cid.itemNum, line);
        this->setCellWidget(row, cid.itemDesc, descLine);
        break;
    }
    case Supplier: {
        QCompleter *completer = new QCompleter(supplierModel, line);
        completer->setCompletionColumn(1);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        QObject::connect(line, &QLineEdit::editingFinished,
                [=] ()
                {
                    completer->setCompletionPrefix(line->text());
                    if(completer->currentCompletion() == line->text())
                    {
                        item(row, cid.supplierId)->setText(stringAt(completer, 0));
                        line->setStyleSheet(cssclear1);
                    }
                    else
                    {
                        item(row, cid.supplierId)->setText("");
                        line->setStyleSheet(cssalert1);
                    }
                });
        setCellWidget(row, cid.supplier, line);
        line->setCompleter(completer);
        break;
    }
    case Project: {
        QCompleter *completer = new QCompleter(projectModel, line);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        QObject::connect(line, &QLineEdit::editingFinished,
                [=] ()
                {
                    completer->setCompletionPrefix(line->text());
                    if(completer->currentCompletion() == line->text())
                        line->setStyleSheet(cssclear1);
                    else
                        line->setStyleSheet(cssalert1);
                });
        setCellWidget(row, cid.project, line);
        line->setCompleter(completer);
        break;
    }
    }
    //Pressing tab does not emit a QCompleter activated signal. This needs to be corrected.
}
