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
#include "global.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringBuilder>

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
    qry.exec("SELECT company.name, company.info , company.shipping_address, company.billing_address FROM company;");
    qry.next();
    simdb.close();

    ui->name->setText(qry.value(0).toString());
    ui->info->setPlainText(qry.value(1).toString());
    ui->shipping->setPlainText(qry.value(2).toString());
    ui->billing->setPlainText(qry.value(3).toString());
    setWindowTitle("Edit Company Information");
    exec();
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
    //! stored as the actual newline character (ie. 10 in ascii decimal). This is a happy coincidence.
    simdb.open();
    qry.exec("UPDATE company SET name = "%escapeSql(ui->name->text())%
             ",info = "%escapeSql(ui->info->toPlainText())%
             ",shipping_address = "%escapeSql(ui->shipping->toPlainText())%
             ",billing_address = "%escapeSql(ui->billing->toPlainText())%";");
    simdb.close();
    EditCompanyInfo::close();
}

void EditCompanyInfo::on_cancelButton_clicked()
{
    EditCompanyInfo::close();
}
