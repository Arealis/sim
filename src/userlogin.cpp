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

#include "userlogin.h"
#include "ui_userlogin.h"
#include "global.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringBuilder>
#include <QCompleter>

UserLogin::UserLogin(QString databaseName, User *currentUser, LoginFlag flag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogin)
{
    db = QSqlDatabase::database(databaseName, false);
    qry = QSqlQuery(db);
    loginFlag = flag;
    user = currentUser;
    ui->setupUi(this);
    ok = false;

    switch (loginFlag) {
    case Login: {
        setWindowTitle("Login");
        ui->emailWidget->hide();
        ui->topWidget->hide();
        break;
    }
    case New: {
        setWindowTitle("Create New User");
        break;
    }
    case Edit: {
        setWindowTitle("Edit Userdata");
        ui->privelageComboBox->setDisabled(true);
        ui->nameLineEdit->setDisabled(true);

        ui->nameLineEdit->setText(user->name);
        ui->emailLineEdit->setText(user->email);
        ui->userLineEdit->setText(user->login);
        ui->privelageComboBox->setCurrentIndex(user->privelage);
        ok = true;
        break;
    }
    }

    exec();
}

UserLogin::~UserLogin()
{
    delete ui;
}

void UserLogin::on_acceptButton_clicked()
{
    switch(loginFlag) {
    case Edit: {
        if (ok) {
            qryString = "UPDATE userdata SET "
                     " name = "%escapeSql(ui->nameLineEdit->text())
                    %" ,user = "%escapeSql(ui->userLineEdit->text())
                    %" ,email = "%escapeSql(ui->emailLineEdit->text())
                    %returnStringINN(ui->passLineEdit->text(), ", pass = "%escapeSql(ui->passLineEdit->text()))
                    %" WHERE id = "%user->id%";";
            db.open();
            qry.exec(qryString);
            db.close();
            UserLogin::close();
        }
        break;
    }
    case New: {
        if (!(ui->userLineEdit->text().isEmpty()) && !(ui->passLineEdit->text().isEmpty()) && ok == true) {
            qryString = QString("INSERT INTO userdata (name, user, email, pass, privelage) "
                    "VALUES (%1, %2, %3, %4, %5);")
                    .arg(escapeSql(ui->nameLineEdit->text())
                         ,escapeSql(ui->userLineEdit->text())
                         ,escapeSql(ui->emailLineEdit->text())
                         ,escapeSql(ui->passLineEdit->text())
                         ,QString::number(ui->privelageComboBox->currentIndex()));
            db.open();
            qry.exec(qryString);
            qry.exec("SELECT id, user, name, email, privelage FROM userdata WHERE user = "%escapeSql(ui->userLineEdit->text())%";");
            qry.next();
            db.close();

            user->id = qry.value(0).toString();
            user->login = qry.value(1).toString();
            user->name = qry.value(2).toString();
            user->email = qry.value(3).toString();
            user->privelage = static_cast<Privelage>(qry.value(4).toInt());
            UserLogin::close();
        }
        break;
    }
    case Login: {
        QString login = ui->userLineEdit->text();
        QString password = ui->passLineEdit->text();
        db.open();
        qry.exec("SELECT id, user, name, email, pass, privelage FROM userdata WHERE user = "%escapeSql(login)%";");
        qry.next();
        db.close();
        if (qry.value(1).toString() == login && qry.value(4).toString() == password) {
            user->id = qry.value(0).toString();
            user->login = qry.value(1).toString();
            user->name = qry.value(2).toString();
            user->email = qry.value(3).toString();
            user->privelage = static_cast<Privelage>(qry.value(5).toInt());
            UserLogin::close();
        }
        break;
    }
    }
}

void UserLogin::on_cancelButton_clicked()
{
    if (loginFlag == Login)
        user->clear();
    UserLogin::close();
}

void UserLogin::on_userLineEdit_editingFinished()
{
    if (loginFlag != Login) {
        QString login = escapeSql(ui->userLineEdit->text());
        db.open();
        qry.exec("SELECT user FROM userdata WHERE user = "%login%" EXCEPT SELECT user FROM userdata WHERE id = "%user->id%";");
        qry.next();
        db.close();
        if (qry.value(0).toString() == login) {
            ui->userLineEdit->setStyleSheet(CSS::Alert);
            //TODO: Include "This name is already taken" or something.
            ok = false;
        } else {
            ui->userLineEdit->setStyleSheet(CSS::Normal);
            ok = true;
        }
    }
}
