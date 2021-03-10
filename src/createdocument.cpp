#include "createdocument.h"
#include "ui_createdocument.h"

#include <QSqlQuery>
#include <QHeaderView>
#include <QCompleter>

#include <QDebug>
#include <QSqlError>

QString docnum;
QString docname;
QStringList colNames;
ResizableTable *table;
int finalRow;
int customBegins, customRow;
int priceCol;

QString returnStringINN(QVariant sqlValue, QString ifNotNull = "%1\n", QString ifNull = "")
{
    if (sqlValue.isNull())
        return ifNull;
    else
        return ifNotNull.arg(sqlValue.toString());
}

ResizableTable::ResizableTable(QWidget *parent) : QTableWidget(parent) {/*could have constuctor here*/}

CreateDocument::CreateDocument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDocument)
{
    ui->setupUi(this);
    customRow = 1; // The row in the gridLayout where the addCustom button lives
    customBegins = 1; // The row in the gridLayout where the first (non-repeated) custom value lives
    finalRow = 0; //The final row in the table.
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);

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
        ui->doctype->setText("Purchase Requisition #");
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

        docname = "pr";
        colNames = {"id", "Item ID", "Description", "Qty", "Unit", "Project", "sid", "Recommended Supplier", "Expected Price", "Status"};
        priceCol = 8;
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
        colNames = {"id", "Item ID", "Description", "Qty", "Unit", "Unit Price", "Total", "Status"};
        break;
    }
    case 2: { //If PO
        ui->shippingAddress->setText(qry.value(5).toString());
        ui->doctype->setText("Purchase Order #");
        docname = "po";
        colNames = {"id", "Item ID", "Description", "Qty", "Unit", "Unit Price", "Tax?", "Total", "Status"};
        break;
    }
    case 3: { //If RR
        ui->shipToWidget->hide();
        ui->doctype->setText("Receiving Report #");
        docname = "rr";
        colNames = {"id", "Item ID", "Description", "Qty", "Unit", "Condition"};
        break;
    }
    case 4: { //If MR
        ui->doctype->setText("Material Requisition #");
        docname = "mr";
        colNames = {"id", "Item ID", "Description", "Qty", "Unit"};
        break;
    }
    }

    //      THIRD: Import elements from an existing document, or initialize them as an unsaved DRAFT
    if (docnum.isEmpty()) //If New Draft
    {
        ui->docNumber->setText("DRAFT");
        ui->update->hide();
        simdb.open();
        qry.exec("SELECT date('now', 'localtime'), name, email FROM userdata WHERE id = "+currentUser+";");
        qry.next();
        simdb.close();
        ui->defaultExtras->setText(QString("Date:\nCreated by:\n%1").arg(returnStringINN(qry.value(2),"Email:")));
        ui->defaultValues->setText(QString("%1%2%3")
                                   .arg(qry.value(0).toString()+"\n"
                                        ,qry.value(1).toString()+"\n"
                                        ,returnStringINN(qry.value(2).toString(),"%1")));
        CreateDocument::insertRecurringCustomFields(simdb, tflag, &customRow);

    } else if (docnum.toInt() < 10000) { //If old draft
        ui->docNumber->setText(docnum);
        ui->doctype->setText(ui->doctype->text()+"DRAFT");
        ui->update->hide();
        CreateDocument::insertRecurringCustomFields(simdb, tflag, &customRow);
        //Insert more extra info here
    } else {
        ui->docNumber->setText(docnum);
    }

    //      LAST: Initialize the table
    table = new ResizableTable(this);
    ui->verticalLayout_3->addWidget(table);
    table->setColumnCount(colNames.count());
    table->setHorizontalHeaderLabels(colNames);
    table->setRowCount(finalRow + 1);
    for (int i = 0; i < table->columnCount(); i++)
    {
        table->setItem(0,i, new QTableWidgetItem());
    }

    //      Hide the appropriate columns
    table->setColumnHidden(0,true);
    switch (tflag.toInt()) {
    case 0: { //PR
        table->setColumnHidden(6,true); //sid
        table->setColumnHidden(9,true); //status
        break;
    }
    case 1: { //QR
        table->setColumnHidden(6, true); //status
        break;
    }
    case 2: { //PO
        table->setColumnHidden(8, true); //status
        break;
    }
    }

    connect(table, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged(int,int)));
}


CreateDocument::~CreateDocument()
{
    delete table;
    delete ui;
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
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCustom()));

    ui->gridLayout->removeWidget(ui->addCustom);
    ui->gridLayout->removeWidget(ui->customLabel);
    ui->gridLayout->addWidget(custom1, customRow, 0);
    ui->gridLayout->addWidget(custom2, customRow, 1);
    ui->gridLayout->addWidget(deleteButton, customRow, 2);
    ui->gridLayout->addWidget(ui->customLabel, customRow+1, 0);
    ui->gridLayout->addWidget(ui->addCustom, customRow+1, 1);

    customRow++;
    /*      CUSTOM ROWS BUG
     * There is a bug that occurs whenever the user attempts to delete custom rows.
     * If the rows are deleted out of order, it causes the row IDs to still use their old values
     * even if they are in their new position. So when it comes time to read data from them,
     * SIM cannot read it sequentially because if a user were to delete the first custom field,
     * and leave the second, there would be an unitialized item between the two valid ones.
     *  THE PROBLEM: There are void IDs left behind when items are deleted in any way that isn't FIFO
     *  SOLUTIONS:
     *      1.] Only allow items to be deleted in FIFO fashion
     *      2.] When an item is deleted, recreate the lower parts of the grid to ensure that numbers work correctly
     *      3.] On button creation, add a QGridLayout rowNumber flag to an array, which will be called when it
     *          is time to read. Delete the appropriate flag from the array when the delete function is called
     * BEST 4.] The index is always updated, so when saving, so create an algorithm to read the correct items from the index
                and one which also knows which indices to skip*/
}

void CreateDocument::deleteCustom()
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
    customRow--;
}

void CreateDocument::insertRecurringCustomFields(QSqlDatabase db, QString tableFlag, int *row)
{
    db.open();
    QSqlQuery tempQry(db); //This works, but could be more effecient.
    tempQry.exec("SELECT name FROM recurring_custom_fields WHERE tflag = "+tableFlag+";");
    for (int qryResult = 0; tempQry.next(); qryResult++)
    {
        ui->gridLayout->removeWidget(ui->addCustom);
        ui->gridLayout->removeWidget(ui->customLabel);
        ui->gridLayout->addWidget(ui->customLabel, *row+1, 0);
        ui->gridLayout->addWidget(ui->addCustom, *row+1, 1);
        QLabel *customLabel = new QLabel;
        customLabel->setAlignment(Qt::AlignRight);
        customLabel->setText(tempQry.value(0).toString());
        ui->gridLayout->addWidget(customLabel, *row, 0);
        QLineEdit *customLineEdit = new QLineEdit;
        customLineEdit->setAlignment(Qt::AlignRight);
        customLineEdit->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        ui->gridLayout->addWidget(customLineEdit, *row, 1);
        *row = *row + 1;
    }
    db.close();
    customBegins = *row;
}

void CreateDocument::onCellChanged(int row, int column)
{
    if (row == finalRow && !table->item(row,column)->text().isEmpty())
    {
        finalRow++;
        table->setRowCount(finalRow+1);
        for (int i = 0; i < table->columnCount(); i++)
        {
            table->setItem(finalRow,i, new QTableWidgetItem());
        }
    }
    if (column == priceCol)
    {
        double total = 0;
        for (int r = 0; r < finalRow; r++)
        {
            total += table->item(r, priceCol)->text().toDouble();
        }
        ui->totals->setText("Total: "+QString::number(total,'f',2));
    }
}

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
    qry.exec("DELETE FROM prd WHERE pr_num = "+oldDocNum+";");
    qry.exec("DELETE FROM pr WHERE num = "+oldDocNum+";");
    switch (tableFlag) {
    //Check whether the columns are valid, and insert into the appropriate tables.
    case 0: { //PR
        qry.exec(QString("INSERT INTO pr (num, date, date_needed, requested_by, notes) VALUES (%1,datetime('now','localtime'),'%2',%3,'%4');")
                 .arg(newDocNum
                      ,ui->dateNeeded->date().toString("yyyy-MM-dd")
                      ,currentUser
                      ,ui->notes->toPlainText()));
        for (int i = 0; i < finalRow; i++)
        {
            if (table->item(i,0)->text().isEmpty()) //Ensure item is valid
            {
                qry.exec(QString("INSERT INTO items (num, desc, unit) VALUES ('%1', '%2', '%3');")
                         .arg(table->item(i,1)->text()
                              ,table->item(i,2)->text()
                              ,table->item(i,4)->text()));
                qry.exec("SELECT id FROM items ORDER BY id DESC LIMIT 1;");
                qry.next();
                table->item(i,0)->setText(qry.value(0).toString());
            }
            if (table->item(i,6)->text().isEmpty()) //Ensure supplier is valid
            {
                qry.exec(QString("INSERT INTO suppliers (name) VALUES ('%1');")
                         .arg(table->item(i,7)->text()));
                qry.exec("SELECT id FROM suppliers ORDER BY id DESC LIMIT 1;");
                qry.next();
                QString sid = qry.value(0).toString();
                for (int j = i; j < finalRow; j++)
                {
                    if (table->item(j,7)->text() == table->item(i,7)->text())
                        table->item(j,6)->setText(sid);
                }
            }
            //Ensure project is valid
            qry.exec("SELECT name FROM projects WHERE name = '"+table->item(i,3)->text()+"';");
            qry.next();
            if (qry.value(0).isNull())
                qry.exec("INSERT INTO projects (name) VALUES ('"+table->item(i,3)->text()+"';");
            qry.exec(QString("INSERT INTO prd (pr_num, supplier_id, project, item_id, qty, unit, expected_price) "
                "VALUES (%1, %2, '%3', %4, %5, '%6', %7);")
                     .arg(newDocNum
                          ,table->item(i,6)->text()
                          ,table->item(i,5)->text()
                          ,table->item(i,0)->text()
                          ,table->item(i,3)->text()
                          ,table->item(i,4)->text()
                          ,table->item(i,8)->text()));
        }
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
    for (int i = 1; i < customBegins; i++)
    {
        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt(i*2)->widget()))
        {
            customName = label->text();
        } else {
            customName = qobject_cast<QLineEdit *>(ui->gridLayout->itemAt((i*2))->widget())->text();
        }
        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt((i*2)+1)->widget()))
        {
            customValue = label->text();
        } else {
            customValue = qobject_cast<QLineEdit *>(ui->gridLayout->itemAt(i*2+1)->widget())->text();
        }
        qry.exec(QString("INSERT INTO custom_fields (tflag, tnum, name, value) VALUES (%1, %2,'%3','%4');")
                .arg(QString::number(tableFlag), newDocNum, customName, customValue));
    }
    for (int i = customBegins; i < customRow; i++)
    {
        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt((i*2)+i-customBegins)->widget()))
        {
            customName = label->text();
        } else {
            customName = qobject_cast<QLineEdit *>(ui->gridLayout->itemAt((i*2)+i-customBegins)->widget())->text();
        }
        if (QLabel *label = qobject_cast<QLabel *>(ui->gridLayout->itemAt((i*2)+i-customBegins+1)->widget()))
        {
            customValue = label->text();
        } else {
            customValue = qobject_cast<QLineEdit *>(ui->gridLayout->itemAt((i*2)+i-customBegins+1)->widget())->text();
        }
        qry.exec(QString("INSERT INTO custom_fields (tflag, tnum, name, value) VALUES (%1, %2,'%3','%4');")
                .arg(QString::number(tableFlag), newDocNum, customName, customValue));
    }
    simdb.close();
}

/*  HOW TO QCompleter
 *
 * itemNumModel->setQuery(q);
    itemNumCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    itemNumCompleter->setFilterMode(Qt::MatchContains);
    ui->ItemIdText->setCompleter(itemNumCompleter);
    QObject::connect(itemNumCompleter, SIGNAL(activated(const QString &)),
                     this, SLOT(onItemIdTextCompleted()));
 */

void ResizableTable::resizeEvent(QResizeEvent *event) //This is called multiple times. Slow.
{
    int w = table->width();
    //Resizing windows based on tFlag
    switch (tflag.toInt()) {
    case 0: {
        //{"id", "Item ID", "Description", "Qty", "Unit", "Project", "sid", "Recommended Supplier", "Expected Price", "Status"};
        w = w / 14;
        table->setColumnWidth(1,w*2);
        table->setColumnWidth(2,w*3);
        table->setColumnWidth(3,w*1);
        table->setColumnWidth(4,w*1);
        table->setColumnWidth(5,w*2);
        table->setColumnWidth(7,w*3);
        table->setColumnWidth(8,w*1);
        table->horizontalHeader()->setStretchLastSection(true);
    }
    }
}
