#include "receiveitems.h"
#include "ui_receiveitems.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QCompleter>
#include <QSqlQueryModel>
#include <QColor>

#include <QtDebug>

/*Like most of the SQL in this program, inputs will not return a value if '' is inputed
 *
 *When an associated PO# is selected, the item_num dropdown should turn into a combo box,
 *so that only the appropriate value can be selected
 *
 *Known bug: If an item_num (or project_name) is entered manually, and its name is part of another
 *items (or project's) name, then the QCompleter will recognize that there is still another
 *string that it can complete to, and both conditionals to check if the item (or project)
 *exists already fail.
 */

//This exists only to hold back the ItemIdComboBox from executing too fast.
bool actionComplete = 1;

ReceiveItems::ReceiveItems(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReceiveItems)
{
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery q;
    itemNumCompleter->setModel(itemNumModel);
    projNameCompleter->setModel(projNameModel);
    itemTypeCompleter->setModel(itemTypeModel);
    qdb.open();
    q.exec("SELECT (SELECT rcr_num FROM trans ORDER BY rcr_num DESC LIMIT 1) + 1;");
    q.next();
    QString rcr = q.value(0).toString();
    QString recby = "debug WIP"; //This will be available in the usrdata db

    ui->setupUi(this);
    ui->Title1->setText(QString("Receiving Report #: %1").arg(rcr));
    ui->Title2->setText(QString("Received by: %1").arg(recby));
    q.exec("SELECT count(po_num) FROM po WHERE (po_status != 0 AND po_status != 3);");
    q.next();
    int openorders = q.value(0).toInt();
    q.exec("SELECT po_num FROM po WHERE (po_status != 0 AND po_status != 3);");
    for (int po = 0; po < openorders; po++)
    {
        q.next();
        ui->POComboBox->insertItem(po, q.value(0).toString());
    }
    ui->POComboBox->insertItem(0, "None");
    ui->POComboBox->setCurrentIndex(0);

    q.exec("SELECT item_num FROM items;");
    itemNumModel->setQuery(q);
    itemNumCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    itemNumCompleter->setFilterMode(Qt::MatchContains);
    ui->ItemIdText->setCompleter(itemNumCompleter);
    QObject::connect(itemNumCompleter, SIGNAL(activated(const QString &)),
                     this, SLOT(onItemIdTextCompleted()));
    q.exec("SELECT project_name FROM projects;");
    projNameModel->setQuery(q);
    projNameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    projNameCompleter->setFilterMode(Qt::MatchContains);
    ui->UsedForText->setCompleter(projNameCompleter);
    QObject::connect(projNameCompleter, SIGNAL(activated(const QString &)),
                     this, SLOT(onUsedForTextCompleted()));
    q.exec("SELECT DISTINCT item_type FROM items;");
    itemTypeModel->setQuery(q);
    itemTypeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    itemTypeCompleter->setFilterMode(Qt::MatchContains);
    ui->ItemTypeText->setCompleter(itemTypeCompleter);
    qdb.close();
}

ReceiveItems::~ReceiveItems()
{
    delete itemNumCompleter;
    delete projNameCompleter;
    delete itemNumModel;
    delete projNameModel;
    delete ui;
}

void ReceiveItems::on_Discard_clicked()
{
    this->close();
}

void ReceiveItems::on_Confirm_clicked()
{
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery q;
    qdb.open();
    //If project is new
    q.exec("INSERT INTO projects");
    //If item is new
    q.exec("INSERT INTO items (item_num, item_name");
    //Always insert into trans
    q.exec("INSERT INTO trans");
}

void ReceiveItems::on_ItemIdText_editingFinished()
{
    if (itemNumCompleter->currentCompletion() == ui->ItemIdText->text())
    {
        ReceiveItems::onItemIdTextCompleted();
    } else {
        ui->DescText->clear();
        ui->ItemTypeText->clear();
        ui->DescText->setDisabled(false);
        ui->ItemTypeText->setDisabled(false);
    }
}

void ReceiveItems::onItemIdTextCompleted()
{
    //This breaks if more than one item have the same number
    QSqlDatabase qdb = QSqlDatabase::database();
    QSqlQuery q;
    qdb.open();
    q.exec(QString("SELECT item_name, item_type FROM items WHERE item_num LIKE '%1'") \
           .arg(ui->ItemIdText->text()));
    q.next();
    qdb.close();
    ui->DescText->setText(q.value(0).toString());
    ui->ItemTypeText->setText(q.value(1).toString());
    ui->DescText->setDisabled(true);
    ui->ItemTypeText->setDisabled(true);
}

void ReceiveItems::on_ItemIdText_textChanged(const QString &arg1)
{
    if (!(itemNumCompleter->currentCompletion() == arg1))
    {
        ui->ItemIdText->setStyleSheet("QLineEdit#ItemIdText {color:#ff0000; background-color:#ffe4e4}");
    } else {
        ui->ItemIdText->setStyleSheet("QLineEdit#ItemIdText {color:#000000; background-color:#ffffff}");
    }
}

void ReceiveItems::on_ItemIdText_returnPressed()
{
    ReceiveItems::on_ItemIdText_textChanged(ui->ItemIdText->text());
}

void ReceiveItems::on_UsedForText_textChanged(const QString &arg1)
{
    if (!(projNameCompleter->currentCompletion() == arg1))
    {
        ui->UsedForText->setStyleSheet("QLineEdit#UsedForText {color:#ff0000; background-color:#ffe4e4}");
    } else {
        ui->UsedForText->setStyleSheet("QLineEdit#UsedForText {color:#000000; background-color:#ffffff}");
    }
}

void ReceiveItems::onUsedForTextCompleted()
{
    //Just reseting the text to reset the CSS with it. Check effeciency.
    ui->UsedForText->setText(ui->UsedForText->text());
}

void ReceiveItems::on_POComboBox_currentIndexChanged(int index)
{
    actionComplete = false;
    if (index != 0)
    {
        ui->ItemIdWidget->setHidden(true);
        ui->ItemIdWidget2->setHidden(false);
        ui->ItemIdComboBox->clear();
        QSqlDatabase qdb = QSqlDatabase::database();
        QSqlQuery q;
        qdb.open();
        q.exec(QString("SELECT items.item_num FROM linker LEFT JOIN items on items.item_id = linker.item_id "
                "WHERE linker.po_num LIKE '%1';")
               .arg(ui->POComboBox->currentText()));
        while(q.next()) {ui->ItemIdComboBox->insertItem(0,q.value(0).toString());}
        qdb.close();
        actionComplete = true;
        on_ItemIdComboBox_currentTextChanged(ui->ItemIdComboBox->currentText());
    } else {
        ui->ItemIdWidget->setHidden(false);
        ui->ItemIdWidget2->setHidden(true);
        actionComplete = true;
    }
}

void ReceiveItems::on_ItemIdComboBox_currentTextChanged(const QString &arg1)
{
    //This may cause problems with the database being open too often
    if (actionComplete)
    {
        QSqlDatabase::database().open();
        QSqlQuery q;
        q.exec(QString("SELECT item_name, item_type FROM items WHERE item_num LIKE '%1';").arg(arg1));
        q.next();
        QSqlDatabase::database().close();
        ui->DescText->setText(q.value(0).toString());
        ui->DescText->setDisabled(true);
        ui->ItemTypeText->setText(q.value(1).toString());
        ui->ItemTypeText->setDisabled(true);
    }
}


