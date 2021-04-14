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

/*! The following is a breif overview of what should happen in this file
 *
 *  1. Initialize an empty version of the document first, then initialize
 *      an empty version of the table.
 *  2. If this is an existing document: First, populate the table. Then,
 *      populate the document.[1]
 *
 *  The program is now in the users hands. Change will only happen if one
 *  of the following occurs:
 *  - Save button is clicked (final or draft)
 *      Verify using SQL that all items are valid, all important fields
 *      have been filled out, and that there will be no overwriting of
 *      important data. Then store the document with the appropriate
 *      status (0 for draft, 1 for complete [as in open])
 *  - Delete button is clicked
 *      Delete everything in SIM that has a reference to this docnum
 *  - Cancel button is clicked
 *      Do nothing and close the windows
 *
 *  [1] We populate the table first and then the document because of
 *  problems that can occur if the table tries to update the text of
 *  a Subtotal Label that does not exist.
 */

CreateDocument::CreateDocument(TableFlag tableFlag, QString docnum, User *user, QString connectionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDocument)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    db = QSqlDatabase::database(connectionName, false);
    tFlag = tableFlag;
    QSqlQuery qry(db);

    customDetailCount = 0;
    customDetailRow = 1; // The row in the gridLayout where the first non-rcf (recurring custom field) custom row lives
    addDetailButtonRow = 1; // The row in the gridLayout where the addCustom button lives

    //      FIRST: Initialize a blank version of the document, which will be populated if necessary.
    // HEADER
    //  Logo
    ui->logo->setPixmap(qApp->applicationDirPath()+"/logo/logo.png");
    ui->logo->setPixmap(ui->logo->pixmap().scaledToHeight(75));
    // Company Info
    db.open();
    qry.exec("SELECT name, info FROM company;");
    qry.next();
    db.close();
    ui->companyName->setText(qry.value(0).toString());
    ui->companyInfo->setText(qry.value(1).toString());

    // DOCUMENT SPECIFIC INFO
    switch(tFlag) {
    case PR: {
        setWindowTitle("Create Purchase Requisition");
        ui->doctype->setText("Purchase Requisition");
        docname = "pr";

        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Thought about subclassing these instead of having init functions. Similar complexity, similar performance. Not worth changing right now.
        QLabel *projectLabel = new QLabel("Project", ui->internalWidget);
        QSqlQueryModel *projectModel = initModel(ModelFlag::Project, db, ui->internalWidget);
        QLineEdit *project = initCompletedLineEdit(projectModel, ui->internalWidget);
        project->setObjectName("project");
        onCompletion(project);
        QLabel *neededByLabel = new QLabel("Needed by", ui->internalWidget);
        QDateTimeEdit *neededBy = initDateTimeEdit(ui->internalWidget);

        ui->internalDetailsGrid->addWidget(projectLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(project, 0, 1);
        ui->internalDetailsGrid->addWidget(neededByLabel, 1, 0);
        ui->internalDetailsGrid->addWidget(neededBy, 1, 1);

        initTotals();
        break;
    }
    case QR: {
        setWindowTitle("Create Request for Quotation");
        ui->doctype->setText("Request for Quotation");
        docname = "qr";

        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Info boxes
        QSqlQueryModel *supplierModel = initModel(Supplier, db, ui->info);
        QWidget *supplierWidget = initInfoWidget(ui->info, "SUPPLIER", supplierModel);
        onCompletion(supplierWidget->findChild<QLineEdit*>("line"), &infoWidgetConnector);

        QWidget *shippingWidget = initInfoWidget(ui->info, "SHIPPING ADDRESS");
        QWidget *billingWidget = initInfoWidget(ui->info, "BILLING ADDRESS");

        ui->infoLayout->addWidget(billingWidget);
        ui->infoLayout->addWidget(shippingWidget);
        ui->infoLayout->addWidget(supplierWidget);

        //Internal Details
        QLabel *neededByLabel = new QLabel("Needed by", ui->internalWidget);
        QDateTimeEdit *neededBy = initDateTimeEdit(ui->internalWidget);
        ui->internalDetailsGrid->addWidget(neededByLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(neededBy, 0, 1);

        QSqlQueryModel *prModel = initModel(Custom, db, ui->info, "SELECT num FROM pr WHERE status = 1;");
        QLabel *prComboBoxLabel = new QLabel("PR#", ui->internalWidget);
        QComboBox *prComboBox = new QComboBox(ui->info);
        prComboBox->setModel(prModel);
        //TODO: Add function to connect models (so that the whole table is updated when they are activated.
        ui->internalDetailsGrid->addWidget(prComboBoxLabel, 1, 0);
        ui->internalDetailsGrid->addWidget(prComboBox, 1, 1);

        initTotals();
        break;
    }
    case PO: {
        setWindowTitle("Create Purchase Order");
        ui->doctype->setText("Purchase Order");
        docname = "po";

        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Info Boxes
        QSqlQueryModel *supplierModel = initModel(Supplier, db, ui->info);

        QWidget *supplierWidget = initInfoWidget(ui->info, "SUPPLIER", supplierModel);
        onCompletion(supplierWidget->findChild<QLineEdit*>("line"), &infoWidgetConnector);
        QWidget *shippingWidget = initInfoWidget(ui->info, "SHIPPING ADDRESS");
        QWidget *billingWidget = initInfoWidget(ui->info, "BILLING ADDRESS");

        ui->infoLayout->addWidget(billingWidget);
        ui->infoLayout->addWidget(shippingWidget);
        ui->infoLayout->addWidget(supplierWidget);

        //Internal Details
        QLabel *neededByLabel = new QLabel("Needed by", ui->internalWidget);
        QDateTimeEdit *neededBy = initDateTimeEdit(ui->internalWidget);
        ui->internalDetailsGrid->addWidget(neededByLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(neededBy, 0, 1);

        QSqlQueryModel *prModel = initModel(Custom, db, ui->info, "SELECT num FROM pr WHERE status = 1;");
        QLabel *prComboBoxLabel = new QLabel("PR#", ui->internalWidget);
        QComboBox *prComboBox = new QComboBox(ui->info);
        prComboBox->setModel(prModel);
        //TODO: Add function to connect models (so that the whole table is updated when they are activated.
        ui->internalDetailsGrid->addWidget(prComboBoxLabel, 1, 0);
        ui->internalDetailsGrid->addWidget(prComboBox, 1, 1);

        QSqlQueryModel *qrModel = initModel(Custom, db, ui->info, "SELECT num FROM pr WHERE status = 1;");
        QLabel *qrComboBoxLabel = new QLabel("PR#", ui->internalWidget);
        QComboBox *qrComboBox = new QComboBox(ui->info);
        qrComboBox->setModel(qrModel);
        //TODO: Add function to connect models (so that the whole table is updated when they are activated.
        ui->internalDetailsGrid->addWidget(qrComboBoxLabel, 2, 0);
        ui->internalDetailsGrid->addWidget(qrComboBox, 2, 1);

        initTotals();
        break;
    }
    case RR: {
        setWindowTitle("Create Receiving Report");
        ui->doctype->setText("Receiving Report");
        docname = "rr";

        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Info Boxes
        QSqlQueryModel *supplierModel = initModel(Supplier, db, ui->info);
        QWidget *supplierWidget = initInfoWidget(ui->info, "SUPPLIER", supplierModel);
        onCompletion(supplierWidget->findChild<QLineEdit*>("line"), &infoWidgetConnector);

        ui->infoLayout->addWidget(supplierWidget);

        //Internal Details
        QSqlQueryModel *poModel = initModel(Custom, db, ui->info, "SELECT num FROM pr WHERE status = 1;");
        QLabel *poComboBoxLabel = new QLabel("PR#", ui->internalWidget);
        QComboBox *poComboBox = new QComboBox(ui->info);
        poComboBox->setModel(poModel);
        //TODO: Add function to connect models (so that the whole table is updated when they are activated.
        ui->internalDetailsGrid->addWidget(poComboBoxLabel, 0, 0);
        ui->internalDetailsGrid->addWidget(poComboBox, 0, 1);

        //TODO: Add invoice number functionality (this can either be a QLineEdit or it can be a Combo Box that has an option to enter the invoice number into a QLineEdit)

        break;
    }
    case MR: {
        setWindowTitle("Create Material Requisition");
        ui->doctype->setText("Material Requisition");
        docname = "mr";

        table = new ResizableTable(tFlag, this);
        ui->verticalLayout_3->addWidget(table);

        //Internal Details
        //  TODO: check what this is in the database
        break;
    }
    }

    //      LAST: Import elements from an existing document, or set defaults if new draft
    //  This should become a POPULATE function
    if (docnum.isEmpty()) //If New Draft
    {
        ui->docNumber->setText("DRAFT");
        ui->update->hide();
        ui->deleteDraft->hide();
        db.open();
        qry.exec("SELECT date('now', 'localtime'), name, email FROM userdata WHERE id = "%user->id%";");
        qry.next();
        ui->detailsNames->setText("Date:\nCreated by:"%returnStringINN(qry.value(2).toString(),"\nEmail:"));
        ui->detailsValues->setText(QString("%1%2%3")
                                   .arg(qry.value(0).toString()
                                        ,"\n"%qry.value(1).toString()
                                        ,returnStringINN(qry.value(2).toString(),"\n"%qry.value(2).toString())));
        insertRecurringCustomDetails(qry, &addDetailButtonRow);
        db.close();
    }
    else if (docnum.toInt() < 10000) //If old draft
    {
        ui->docNumber->setText(docnum);
        ui->doctype->setText("DRAFT "%ui->doctype->text());
        ui->update->hide();
        db.open();
        fetchDetails(qry);
        fetchCustomDetails(qry, &addDetailButtonRow, true);
        table->fetchRows(qry,  docnum);
        if (tFlag == PR || tFlag == QR || tFlag == PO)
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
        fetchDetails(qry);
        fetchCustomDetails(qry, &addDetailButtonRow, false);
        table->fetchRows(qry, docnum);
        if (tFlag == PR || tFlag == QR || tFlag == PO)
            fetchTotals(qry);
        db.close();
        //I need to update the custom fields functions. There should be one to insert deletable custom fields, and one to insert constant custom fields.
    }

    //      EXTRA: Create delete Message Box
    QMessageBox *confirm = new QMessageBox(this);
    confirm->setText("Are you sure you would like to delete this document?");
    confirm->addButton("Confirm", QMessageBox::AcceptRole);
    confirm->addButton("Cancel", QMessageBox::RejectRole);
    confirm->setWindowTitle("Delete?");
    connect(ui->deleteDraft, &QPushButton::clicked, confirm, &QMessageBox::exec);
    connect(confirm, &QMessageBox::rejected, confirm, &QMessageBox::close);
    connect(confirm, &QMessageBox::accepted, [this, qry, docnum] () {
        db.open();
        CreateDocument::DeleteDocument(qry, docnum);
        db.close();
        CreateDocument::close();
    });



    exec();
}

QSqlQueryModel *initModel(ModelFlag modelFlag, QSqlDatabase db, QWidget *parent, QString query)
{
    QSqlQueryModel *model = new QSqlQueryModel(parent);
    model->setObjectName(QString::number(modelFlag));

    db.open();
    switch (modelFlag) {
    case Item: {
        model->setQuery("SELECT id, num, desc, cat, unit FROM items;", db);
        break;
    }
    case Supplier: {
        model->setQuery("SELECT id, name FROM suppliers;", db);
        break;
    }
    case Project: {
        model->setQuery("SELECT name FROM projects;", db);
        break;
    }
    case Custom: {
        model->setQuery(query, db);
        break;
    }
    }
    while (model->canFetchMore()) { model->fetchMore(); }
    db.close();

    return model;
}

QLineEdit *initCompletedLineEdit(QSqlQueryModel *model, QWidget *parent, int column)
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
    case Custom: {
        completer->setCompletionColumn(column);
        break;
    }
    }

    line->setCompleter(completer);

    return line;
}

QDateTimeEdit *initDateTimeEdit(QWidget *parent)
{
    QDateTimeEdit *date = new QDateTimeEdit(parent);
    date->setDisplayFormat("yyyy/MM/dd");
    date->setCalendarPopup(true);
    date->setTimeSpec(Qt::UTC);
    date->setObjectName("date");

    return date;
}

inline void onCompletion(QLineEdit *line, void (*function)(QLineEdit*, bool))
{
    //  TODO: Make this comment make sense and name this function better
    //  The reason the following exists (and is so janky) is because signal is because of signal and emitter problems with a QCompleter
    //  and a QLineEdit. I cannot remember the exact details, but this takes care of it. It also has problems with case sensitivity.

    // Just because the lineEdit is finished, that does not mean that there will be a completion. This lambda checks for that.
    QCompleter *completer = line->completer();
    QObject::connect(line, &QLineEdit::editingFinished, line, [line, completer, function] () {
        bool hasCompletion = false;
        for (int row = 0; completer->setCurrentRow(row); row++) {
            if (completer->currentCompletion() == line->text()) {
                hasCompletion = true;
                line->setStyleSheet(CSS::Normal);
                if (function != nullptr)
                    function(line, hasCompletion);
                break;
            }
        }
        if (!hasCompletion)
            line->setStyleSheet(CSS::Alert);
    });

    //If the completer is activated, there is a completion and it (hopefully) has been activated.
    QObject::connect(completer, static_cast<void(QCompleter::*)(const QString &text)>(&QCompleter::activated), line, [line, function] () {
        line->setStyleSheet(CSS::Normal);
        if (function != nullptr)
            function(line, true);
    });
}

QWidget *initInfoWidget(QWidget *parent, QString title)
{
    //This is a basic info widget for displaying simple items like addresses
    QWidget *widget = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setSpacing(1);

    QLabel *name = new QLabel(title, widget);
    QPlainTextEdit *info = new QPlainTextEdit(widget);
    info->setObjectName("info");
    layout->addWidget(name);
    layout->addWidget(info);

    widget->setContentsMargins(2,0,2,0);

    return widget;
}

//The model this takes is very specific: id=0, name=1, info=2, internal=3
QWidget *initInfoWidget(QWidget *parent, QString title, QSqlQueryModel *model)
{
    //This is an extended info widget
    QWidget *widget = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setSpacing(1);

    QLabel *name = new QLabel(title, widget);
    QLineEdit *line = initCompletedLineEdit(model);
    line->setObjectName("line");

    QPlainTextEdit *info = new QPlainTextEdit(widget);
    info->setPlaceholderText("Address");
    info->setObjectName("info");
    QObject::connect(info, &QPlainTextEdit::textChanged, info, [line, info] ()
    {
        if (line->completer()->currentIndex().siblingAtColumn(2).data().toString() != info->toPlainText())
            info->setStyleSheet(CSS::Alert);
        else
            info->setStyleSheet(CSS::Normal);
    });

    QPlainTextEdit *internal = new QPlainTextEdit(widget);
    internal->setPlaceholderText("Internal Information");
    internal->setObjectName("internal");
    QObject::connect(info, &QPlainTextEdit::textChanged, info, [line, info] ()
    {
        if (line->completer()->currentIndex().siblingAtColumn(3).data().toString() != info->toPlainText())
            info->setStyleSheet(CSS::Alert);
        else
            info->setStyleSheet(CSS::Normal);
    });

    layout->addWidget(name);
    layout->addWidget(line);
    layout->addWidget(info);
    layout->addWidget(internal);

    widget->setContentsMargins(2,0,2,0);

    return widget;
    //TODO: Add a property of the widget that marks whether it has been modified or not.
}

void infoWidgetConnector(QLineEdit *line, bool success)
{
    if (success)
    {
        line->parent()->findChild<QPlainTextEdit*>("info")->setPlainText(line->completer()->currentIndex().siblingAtColumn(2).data().toString());
        line->parent()->findChild<QPlainTextEdit*>("internal")->setPlainText(line->completer()->currentIndex().siblingAtColumn(3).data().toString());
    } //TEST: See whether it is more intuitive to have a clear here or not.
}

ResizableTable::ResizableTable(TableFlag tableFlag, CreateDocument *parent) :
    QTableWidget(parent)
{    
    // Initialize classes and variables
    parentDoc = parent;
    tflag = tableFlag;

    setRowCount(0);

    initializeColumns();

    // Create base table
    setColumnCount(colNames.count());
    setHorizontalHeaderLabels(colNames);
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

QString stringAt(QCompleter *completer, int column)
{
    return completer->currentIndex().siblingAtColumn(column).data().toString();
}

// ========================================
// ===== CREATING AND MANAGING TOTALS =====
// ========================================

void CreateDocument::updateTotal(double total, bool isTaxExempt)
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

void ResizableTable::updateRowTotal(int row)
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
    static_cast<CreateDocument*>(parentDoc)->updateTotal(total, 0);
    static_cast<CreateDocument*>(parentDoc)->updateTotal(taxExemptTotal, 1);
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


void CreateDocument::fetchRateAndAmount(QSqlQuery qry, int rateIndex, QLineEdit *line, QGridLayout *grid, bool custom)
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
            table->updateRowTotal(row);
    });

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
    addExpenseLabel = new QLabel("Add Expense", ui->totalsWidget);          //this here
    addExpenseButton = new QPushButton("%", ui->totalsWidget);              //could be outsourced
    ui->totalsGridLayout->addWidget(addExpenseLabel, nextOpenRow, 2);       //so it could
    ui->totalsGridLayout->addWidget(addExpenseButton, nextOpenRow, 3);      //look and work
    nextOpenRow--;                                                          //a little better
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
    connect(discountBeforeTax, &QCheckBox::stateChanged,
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

    connect(taxExemptSubtotal, &QLabel::linkActivated,
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

    connect(discountOnTaxable, &QLineEdit::textChanged,
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

    connect(discountOnTaxExempt, &QLineEdit::textChanged,
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

    connect(addExpenseButton, &QPushButton::clicked,
            [=] ()
    //The following lambda function is based off of the addCustomField a function.
    //and the createTotals function. It is redundant in a lot of places, and could
    //probably be removed after some light tweaking
    {
        customExpenseRowCount++;

        int buttonIndex, buttonRow, trash; //trash exists as a junk pointer that will never be read.
        ui->totalsGridLayout->getItemPosition(ui->totalsGridLayout->indexOf(addExpenseButton), &buttonRow, &trash, &trash, &trash);
        buttonIndex = ui->totalsGridLayout->indexOf(addExpenseButton);

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
void CreateDocument::deleteCustomExpense()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QLayoutItem *child;
    int index = ui->gridLayout->indexOf(button);
    for (int i = 0; i < 3; i++)
    {
        child = ui->totalsGridLayout->takeAt(index - i);
        delete child->widget();
        delete child;
    }
    addDetailButtonRow--;
}

// ======================================
// == INITIALIZING THE RESIZABLE TABLE ==
// ======================================

void ResizableTable::initializeColumns()
{
    switch (tflag) {
    case 0: {
        colNames = {"id", "Item ID", "Description", "Category", "Qty", "Unit", "sid", "Recommended Supplier", "Tax?", "Expected Unit Price", "Total", "Status"};
        cid.supplierId = 6;
        cid.supplier = 7;
        cid.taxable = 8;
        cid.unitPrice = 9;
        cid.total = 10;
        cid.status = 11;
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
        cid.taxable = 7;
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


CreateDocument::~CreateDocument()
{
    delete ui;
}

//!      ==============================================================
//!      ===== CREATING AND FETCHING INTERNAL / EXTERNAL DETAILS ======
//!      ==============================================================

void CreateDocument::fetchDetails(QSqlQuery qry)
{
    switch (tFlag) {
    case PR: {
        ui->detailsNames->setText("Date:\nCreated by:");
        qry.exec("SELECT pr.date, pr.date_needed, pr.notes, userdata.name, pr.project FROM pr JOIN userdata ON userdata.id = pr.requested_by WHERE pr.num = "%docnum%";");
        qry.next();
        ui->detailsValues->setText(qry.value(0).toString()%"\n"%qry.value(3).toString());
        ui->internalWidget->findChild<QDateTimeEdit*>("date")->setDate(qry.value(1).toDate());
        ui->notes->setPlainText(qry.value(2).toString());
        findChild<QLineEdit*>("project")->setText(qry.value(4).toString());
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

void CreateDocument::insertRecurringCustomDetails(QSqlQuery qry, int *row)
{
    ui->gridLayout->removeWidget(ui->addCustom);
    ui->gridLayout->removeWidget(ui->customLabel);
    qry.exec("SELECT name FROM recurring_custom_details WHERE tflag = "%QString::number(tFlag)%";");
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
    customDetailRow = *row;
}

void CreateDocument::fetchCustomDetails(QSqlQuery qry, int *row, bool editable)
{
    ui->gridLayout->removeWidget(ui->addCustom);
    ui->gridLayout->removeWidget(ui->customLabel);
    qry.exec("SELECT count(*) FROM recurring_custom_details WHERE tflag = "%QString::number(tFlag)%";");
    qry.next();
    int rcdCount = qry.value(0).toInt(); //recurring Custom Details Count
    qry.exec("SELECT name, value FROM custom_details WHERE tflag = "%QString::number(tFlag)%" AND tnum = "%docnum%";");
    for (int i = 0; i < rcdCount; i++)
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
    customDetailRow = *row;
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
        customDetailRow = *row;
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
    ui->gridLayout->addWidget(custom1, addDetailButtonRow, 0);
    ui->gridLayout->addWidget(custom2, addDetailButtonRow, 1);
    ui->gridLayout->addWidget(deleteButton, addDetailButtonRow, 2);
    ui->gridLayout->addWidget(ui->customLabel, addDetailButtonRow+1, 0);
    ui->gridLayout->addWidget(ui->addCustom, addDetailButtonRow+1, 1);

    customDetailCount++;
    addDetailButtonRow++;
}

void CreateDocument::deleteCustomDetail()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int row, trash;
    ui->gridLayout->getItemPosition(ui->gridLayout->indexOf(button), &row, &trash, &trash, &trash);
    QLayoutItem *child;

    //delete the selected row
    for (int i = 0; i < 3; i++) {
        child = ui->gridLayout->itemAtPosition(row, i);
        ui->gridLayout->removeItem(child);
        delete child->widget();
        delete child;
    }

    //move all other rows down
    for (; row < addDetailButtonRow; row++) {
        child = ui->gridLayout->itemAtPosition(row+1, 0);
        ui->gridLayout->removeItem(child);
        ui->gridLayout->addItem(child, row, 0);

        child = ui->gridLayout->itemAtPosition(row+1, 1);
        ui->gridLayout->removeItem(child);
        ui->gridLayout->addItem(child, row, 1);

        if (ui->gridLayout->itemAtPosition(row+1, 2) != nullptr)
        {
            child = ui->gridLayout->itemAtPosition(row+1, 2);
            ui->gridLayout->removeItem(child);
            ui->gridLayout->addItem(child, row, 2);
        }
    }

    customDetailCount--;
    addDetailButtonRow--;
}

//!      ==========================
//!      =====     SAVING     =====
//!      ==========================

void CreateDocument::on_cancel_clicked()
{
    CreateDocument::close();
}

void CreateDocument::on_save_clicked()
{
    //Completed documents do not have this option.
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

void CreateDocument::on_saveDraft_clicked(QSqlDatabase db) //Completed documents do not have this option
{
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

//The validate functions will check to see whether a certain item exists, and insert it if it does not.
bool validateProject(QSqlQuery qry, QString project)
{
    qry.exec("SELECT name FROM projects WHERE name = "%escapeSql(project)%";");
    qry.next();
    if (qry.value(0).isNull())
    {
        qry.exec("INSERT INTO projects (name) VALUES ("%escapeSql(project)%");");
        return true;
    }
    else
        return false;
}

inline QString insertNewSupplier(QSqlQuery qry, QString supplier)
{
    qry.exec("INSERT INTO suppliers (name) VALUES ("%escapeSql(supplier)%");");
    qry.exec("SELECT id FROM suppliers ORDER BY id DESC LIMIT 1;");
    qry.next();
    return qry.value(0).toString();
}

//Returns the id. Has a side effect of changing the newEntry bool
QString validateSupplier(QSqlQuery qry, QString supplier, bool *newEntry)
{
    qry.exec("SELECT id FROM suppliers WHERE name = "%escapeSql(supplier)%";");
    qry.next();
    if (qry.value(0).isNull())
    {
        *newEntry = true;
        return insertNewSupplier(qry, supplier);
    }
    else
    {
        *newEntry = false;
        return qry.value(0).toString();
    }
}

inline QString insertNewItem(QSqlQuery qry, QString itemNum, QString itemDesc, QString unit = "", QString category = "")
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

void ResizableTable::checkItem(QSqlQuery qry, int row)
{
    if (item(row,cid.itemId)->text().isEmpty())
    {
        item(row,cid.itemId)->setText(insertNewItem(qry
                                                  ,qobject_cast<QLineEdit*>(cellWidget(row,cid.itemNum))->text()
                                                  ,qobject_cast<QLineEdit*>(cellWidget(row,cid.itemDesc))->text()
                                                  ,qobject_cast<QLineEdit*>(cellWidget(row,cid.unit))->text()
                                                  ,qobject_cast<QLineEdit*>(cellWidget(row,cid.cat))->text()));
    }
}

void ResizableTable::checkSupplier(QSqlQuery qry, int row)
{
    if (item(row,cid.supplierId)->text().isEmpty())
    {
        QString supplierName = qobject_cast<QLineEdit*>(cellWidget(row,cid.supplier))->text();
        QString supplierId = insertNewSupplier(qry, supplierName);
        for (int i = row; i < finalRow; i++) //Loop so repeated supplier names in the doc will all have the same supplierId
        {
            if (qobject_cast<QLineEdit*>(cellWidget(i,cid.supplier))->text() == supplierName)
                item(i,cid.supplierId)->setText(supplierId);
        }
    }
}

QString validateItem(bool *newEntry, QSqlQuery qry, QString itemNum, QString itemDesc, QString unit = "", QString category = "")
{
    qry.exec("SELECT id FROM items WHERE num = "%escapeSql(itemNum)%" AND desc = "%escapeSql(itemDesc)%"'");
    qry.next();
    if (qry.value(0).isNull())
    {
        *newEntry = true;
        return insertNewItem(qry, itemNum, itemDesc, unit, category);
    }
    else
    {
        *newEntry = false;
        return qry.value(0).toString();
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

void CreateDocument::storeTable(QSqlQuery qry, QString oldDocNum, QString newDocNum, int status)
{
    //This function saves the table to the specified document number
    db.open();

    //FIRST: Clear out all existing data for this document. Everything will be overwritten.
    DeleteDocument(qry, oldDocNum);

    //Check whether the columns are valid, and insert into the appropriate tables.
    switch (tFlag) {
    case 0: { //PR
        validateProject(qry, findChild<QLineEdit*>("project")->text());
        qry.exec("INSERT INTO pr (num, date, date_needed, requested_by, notes, status, project, discount_before_tax, taxable_subtotal, tax_exempt_subtotal, discount_on_taxable_rate"
            ",discount_on_taxable, discount_on_tax_exempt_rate, discount_on_tax_exempt, tax_rate, tax, discount_after_tax_rate, discount_after_tax, total) VALUES ("
                 %newDocNum
                 %",datetime('now','localtime')"
                 %",'"%ui->internalWidget->findChild<QDateTimeEdit*>("date")->date().toString("yyyy-MM-dd")%"'"
                 %","%user->id
                 %","%escapeSql(ui->notes->toPlainText())
                 %","%QString::number(status)
                 %","%escapeSql(findChild<QLineEdit*>("project")->text())
                 %","%QString::number(qobject_cast<QCheckBox*>(ui->internalDetailsGrid->itemAtPosition(ui->internalDetailsGrid->rowCount()-1,0)->widget())->isChecked())
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
                 %","%escapeSql(qobject_cast<QLabel*>(ui->totalsGridLayout->itemAtPosition(customExpenseRowStart+customExpenseRowCount+2,3)->widget())->text())
            %");");
        storeCustomExpenses(qry, newDocNum);
        table->storeRows(qry, newDocNum);
        break;
    }
    case 1: { //QR
        break;
    }
    case 2: { //PO
        break;
    }
    }

    storeCustomDetails(qry, newDocNum);

}

void CreateDocument::storeCustomDetails(QSqlQuery qry, QString docnum)
{
    //custom details always start from row 1
    int row = 1;
    QString name, value;
    for (; row < recurringCustomDetailCount; row ++) {
        name = escapeSql(qobject_cast<QLabel*>(ui->gridLayout->itemAtPosition(row, 0)->widget())->text());
        value = escapeSql(qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(row, 1)->widget())->text());
        qry.exec("INSERT INTO custom_details (tflag, tnum, name, value, flag) VALUES ("%QString::number(tFlag)%","%docnum%","%name%","%value%",0);");
    }
    for (; row < addDetailButtonRow; row ++) {
        name = escapeSql(qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(row, 0)->widget())->text());
        value = escapeSql(qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(row, 1)->widget())->text());
        qry.exec("INSERT INTO custom_details (tflag, tnum, name, value, flag) VALUES ("%QString::number(tFlag)%","%docnum%","%name%","%value%",1);");
    }
}

void ResizableTable::storeRows(QSqlQuery qry, QString docnum)
{
    switch (tflag) {
    case PR: {
        for (int row = 0; row < finalRow; row++)
        {
            checkItem(qry, row);
            checkSupplier(qry, row);
            qry.exec(QString("INSERT INTO prd (pr_num, supplier_id, item_id, qty, taxable, expected_unit_price, total) "
                "VALUES (%1, %2, %3, %4, %5, %6, %7);")
                     .arg(docnum
                          ,item(row,cid.supplierId)->text()
                          ,item(row,cid.itemId)->text()
                          ,escapeSql(item(row,cid.qty)->text())
                          ,QString::number(cellWidget(row,cid.taxable)->findChild<QCheckBox*>("box")->isChecked())
                          ,escapeSql(item(row,cid.unitPrice)->text())
                          ,escapeSql(item(row,cid.total)->text())));
        }
        break;
    }
    case QR: {
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
        horizontalHeader()->setStretchLastSection(true);
    }
    }
}

void CreateDocument::DeleteDocument(QSqlQuery qry, QString docnum)
{
    QString tflag = QString::number(tFlag);
    qry.exec("DELETE FROM "%docname%"d WHERE "%docname%"_num = "%docnum%";");
    qry.exec("DELETE FROM "%docname%" WHERE num = "%docnum%";");
    qry.exec("DELETE FROM custom_details WHERE tnum = "%docnum%" AND tflag = "%QString::number(tFlag)%";");
    qry.exec("DELETE FROM custom_expenses WHERE tnum = "%docnum%" AND tflag = "%QString::number(tFlag)%";");
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

    switch (tflag) {
    case PR: {
        customLineEdit(finalRow, Supplier);
        customCheckBox(finalRow, cid.taxable);
        customLineEdit(finalRow, Blank, cid.total);
        setColumnHidden(cid.supplierId,true); //sid
        setColumnHidden(cid.status,true); //status
        break;
    }
    case QR: {
        setColumnHidden(cid.status, true); //status
        break;
    }
    case PO: {
        setColumnHidden(cid.status, true); //status
        break;
    }
    }
}

void ResizableTable::fetchRows(QSqlQuery qry, QString docnum)
{
    switch (tflag) {
    case PR: {
        qry.exec("SELECT prd.item_id, items.num, items.desc, items.cat, prd.qty, items.unit, prd.supplier_id, suppliers.name, prd.taxable, prd.expected_unit_price, prd.total, prd.status "
        "FROM prd "
        "LEFT JOIN items ON items.id = prd.item_id "
        "LEFT JOIN suppliers ON prd.supplier_id = suppliers.id "
        "WHERE prd.pr_num = "%docnum%";");
        break;
    }
    case QR: {
        qry.exec("SELECT qrd.item_id, items.num, items.desc, items.cat, qrd.qty, qrd.unit, qrd.unit_price, qrd.total, qrd.status "
            "FROM qrd "
            "LEFT JOIN items ON items.id = qrd.item_id "
            "WHERE qrd.qr_num = "%docnum%";");
        break;
    }
    case PO: {
        qry.exec("SELECT pod.item_id, items.num, items.desc, items.cat, pod.unit, pod.unit_price, pod.taxable, pod.total, pod.status "
            "FROM pod "
            "LEFT JOIN items ON items.id = pod.item_id "
            "WHERE pod.po_num = "%docnum%";");
        break;
    }
    case RR: {
        qry.exec("SELECT rrd.item_id, items.num, items.desc, items.cat, rrd.qty, rrd.unit, rrd.condition "
            "FROM rrd "
            "LEFT JOIN items ON items.id = rrd.item_id "
            "WHERE rrd.rr_num = "%docnum%";");
        break;
    }
    case MR: {
        qry.exec("SELECT mrd.item_id, items.num, items.desc, items.cat, mrd.qty, mrd.unit "
            "FROM mrd "
            "LEFT JOIN items ON items.id = mrd.item_id "
            "WHERE mrd.mr_num = "%docnum%";");
        break;
    }
    }
    //Table items can have a line edit as their primary display. The following loop accounts for this.
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

void ResizableTable::customCheckBox(int row, int column)
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
        updateRowTotal(row);
    });

}

//The following function exists because editingFinished is not emitted when QCompleter is activated by clicking
inline void emitSignalOnCompletion(QLineEdit *line)
{
    QObject::connect(line->completer(), static_cast<void(QCompleter::*)(const QModelIndex &index)>(&QCompleter::activated), line,
            [line] () { emit line->editingFinished(); } );
}

//This could be made quicker by subclassing a QLineEdit and creating a new property for checking whether it is valid or not
inline void ResizableTable::initCompanionLineEdit(int row, int column, QLineEdit *sourceLine, QLineEdit *line2, QLineEdit *line3, QLineEdit *line4, QString cssValid, QString cssInvalid)
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

    QObject::connect(sourceLine, &QLineEdit::editingFinished,
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

void ResizableTable::customLineEdit(int row, lineType type, int column)
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
                    if(completer->currentCompletion() == line->text())
                    {
                        item(row, cid.itemId)->setText(stringAt(completer,0));

                        changeCompletionModel(descLine, stringAt(completer, 0), stringAt(completer, 2));
                        changeCompletionModel(unitLine, stringAt(completer, 0), stringAt(completer, 3));
                        changeCompletionModel(catLine, stringAt(completer, 0), stringAt(completer, 4));

                        line->setStyleSheet(CSS::Normal);
                        descLine->setStyleSheet(CSS::Normal);
                        catLine->setStyleSheet(CSS::Normal);
                        unitLine->setStyleSheet(CSS::Normal);
                    }
                    else
                    {
                        item(row, cid.itemId)->setText("");

                        changeCompletionModel(descLine, "", "");
                        changeCompletionModel(unitLine, "", "");
                        changeCompletionModel(catLine, "", "");

                        line->setStyleSheet(CSS::Alert);
                        descLine->setStyleSheet(CSS::Alert);
                        catLine->setStyleSheet(CSS::Alert);
                        unitLine->setStyleSheet(CSS::Alert);
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
    case Blank: {
        line->setObjectName("blank");
        line->setDisabled(true);
        setCellWidget(row, column, line);
        break;
    }
    }
}
