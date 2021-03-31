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

#include "editcompanyinfo.h"
#include "ui_editcompanyinfo.h"

#include <QSqlQuery>
#include <QSqlQueryModel>

//! This entire screen was created very haphazardly without much thought being put into it
//! It does not generate any additional lag during normal operations of the program, but is
//! not the best. This is not critical to improve, but could use some love.


QString formatStringForSQL(QString string)
// If a string is empty, return null. Else, return the string in a format for SQL
{
    if (string.isEmpty())
        return "null";
    else
        return "'"+string+"'";
}

EditCompanyInfo::EditCompanyInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCompanyInfo)
{
    ui->setupUi(this);
    ui->logoButton->setIcon(QIcon(qApp->applicationDirPath()+"/logo/logo.png"));
    ui->logoButton->setIconSize(ui->logoButton->iconSize().scaled(500,100,Qt::KeepAspectRatio));

    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);

    simdb.open();
    qry.exec("SELECT c.name, c.phone, c.phone2, c.fax, c.email, c.website, s.address, b.address "
        "FROM company AS 'c' "
        "JOIN addresses AS 's' ON c.shipping_address_id = s.id "
        "JOIN addresses AS 'b' ON c.billing_address_id = b.id "
        ";");
    qry.next();
    simdb.close();
    ui->company->setText(qry.value(0).toString());
    ui->phone->setText(qry.value(1).toString());
    ui->phone2->setText(qry.value(2).toString());
    ui->fax->setText(qry.value(3).toString());
    ui->email->setText(qry.value(4).toString());
    ui->website->setText(qry.value(5).toString());
    ui->shipping->setPlainText(qry.value(6).toString());
    ui->billing->setPlainText(qry.value(7).toString());

}

EditCompanyInfo::~EditCompanyInfo()
{
    delete ui;
}

void EditCompanyInfo::on_saveButton_clicked()
{
    QSqlDatabase simdb = QSqlDatabase::database("sim", false);
    QSqlQuery qry(simdb);

    //! Inserting plain text directly into a query causes the newline characters to get parsed by SQLite
    //! instead of being included as a part of the string. I do not know why. The most logical
    //! explanation is that newline characters are displayed as \n while using qDebug(), but are
    //! stored as the actual newline character (ie. 10 in ascii decimal)
    simdb.open();
    qry.exec("INSERT INTO addresses (address) VALUES ('"+ui->shipping->toPlainText()+"');");
    qry.exec("UPDATE company SET shipping_address_id = (SELECT id FROM addresses ORDER BY id DESC LIMIT 1);");

    qry.exec("INSERT INTO addresses (address) VALUES ('"+ui->billing->toPlainText()+"');");
    qry.exec("UPDATE company SET billing_address_id = (SELECT id FROM addresses ORDER BY id DESC LIMIT 1);");

    qry.exec(QString("UPDATE company SET name = %1, phone = %2, phone2 = %3, fax = %4,email = %5, website = %6;")
             .arg(formatStringForSQL(ui->company->text())
                  ,formatStringForSQL(ui->phone->text())
                  ,formatStringForSQL(ui->phone2->text())
                  ,formatStringForSQL(ui->fax->text())
                  ,formatStringForSQL(ui->email->text())
                  ,formatStringForSQL(ui->website->text())));
    simdb.close();
    EditCompanyInfo::close();
}

void EditCompanyInfo::on_cancelButton_clicked()
{
    EditCompanyInfo::close();
}
