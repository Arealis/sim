#include "userlogin.h"
#include "ui_userlogin.h"

#include <QSqlDatabase>
#include <QSqlQuery>

bool ok = 0;

UserLogin::UserLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogin)
{
    ui->setupUi(this);
    switch (loginFlag) {
    case 0: { //login
        ui->emailWidget->hide();
        ui->topWidget->hide();
        break;
    }
    case 1: { //new
        break;
    }
    case 2: { //edit
        ui->privelageWidget->hide();
        QSqlDatabase simdb = QSqlDatabase::database("sim", false);
        QSqlQuery q(simdb);
        simdb.open();
        q.exec(QString("SELECT name, email, user FROM userdata WHERE id = %1;").arg(currentUser));
        q.next();
        simdb.close();
        ui->nameLineEdit->setText(q.value(0).toString());
        ui->nameLineEdit->setDisabled(true);
        ui->emailLineEdit->setText(q.value(1).toString());
        ui->usrLineEdit->setText(q.value(2).toString());
        ok = 1;
        break;
    }
    }
}

UserLogin::~UserLogin()
{

    delete ui;
}

void UserLogin::on_acceptButton_clicked()
{
    switch(loginFlag) {
    case 2: { //edit
        if (ok)
        {
            if (ui->passLineEdit->text().isEmpty())
            {
                QSqlDatabase simdb = QSqlDatabase::database("sim", false);
                QSqlQuery q(simdb);
                simdb.open();
                q.exec(QString("UPDATE userdata SET name = '%1', user = '%2', email = '%3' WHERE id = %4;")
                       .arg(ui->nameLineEdit->text(), ui->usrLineEdit->text(), ui->emailLineEdit->text() ,currentUser));
                simdb.close();
                UserLogin::close();
            } else {
                QSqlDatabase simdb = QSqlDatabase::database("sim", false);
                QSqlQuery q(simdb);
                simdb.open();
                q.exec(QString("UPDATE userdata SET name = '%1', user = '%2', email = '%3', pass = '%5' WHERE id = %4;")
                       .arg(ui->nameLineEdit->text(), ui->usrLineEdit->text(), ui->emailLineEdit->text() ,currentUser, ui->passLineEdit->text()));
                simdb.close();
                UserLogin::close();
            }
        }
        break;
    }
    case 1: { //new
        if (!(ui->usrLineEdit->text().isEmpty()) && !(ui->passLineEdit->text().isEmpty()) && ok == true)
        {

            QSqlDatabase simdb = QSqlDatabase::database("sim", false);
            QSqlQuery q(simdb);
            simdb.open();
            q.exec(QString("INSERT INTO userdata (name, user, email, pass, privelage)"
            "VALUES ('%1', '%2', '%3', '%4', %5);")
                   .arg(ui->nameLineEdit->text()
                        ,ui->usrLineEdit->text()
                        ,ui->emailLineEdit->text()
                        ,ui->passLineEdit->text()
                        ,QString::number(ui->privelageComboBox->currentIndex())));
            q.exec(QString("SELECT id FROM userdata WHERE user = '%1';").arg(ui->usrLineEdit->text()));
            q.next();
            simdb.close();
            currentUser = q.value(0).toString();
            UserLogin::close();
        }
        break;
    }
    case 0: { //login
        QSqlDatabase simdb = QSqlDatabase::database("sim", false);
        QSqlQuery q(simdb);
        QString user = ui->usrLineEdit->text();
        QString pass = ui->passLineEdit->text();
        simdb.open();
        q.exec("SELECT user, pass, id FROM userdata WHERE user = '"+user+"';");
        q.next();
        simdb.close();
        if (q.value(0).toString() == user && q.value(1).toString() == pass)
        {
            currentUser = q.value(2).toString();
            UserLogin::close();
        }
        break;
    }
    }
}

void UserLogin::on_usrLineEdit_editingFinished()
{
    if (loginFlag > 0) //new or edit
    {
        QSqlDatabase simdb = QSqlDatabase::database("sim", false);
        QString user = ui->usrLineEdit->text();
        QSqlQuery q(simdb);
        simdb.open();
        q.exec(QString("SELECT user FROM userdata WHERE user = '%1' EXCEPT SELECT user FROM userdata WHERE id = %2;").arg(user,currentUser));
        q.next();
        simdb.close();
        if (q.value(0).toString() == user) {
            ui->usrLineEdit->setStyleSheet("QLineEdit#usrLineEdit {color:#AA4471; background-color:#ffe4e4}");
            ok = false;
        } else {
            ui->usrLineEdit->setStyleSheet("QLineEdit#UsedForText {color:#000000; background-color:#ffffff}");
            ok = true;
        }
    }
}

void UserLogin::on_cancelButton_clicked()
{
    if (loginFlag == 0) //if this is a Login
        currentUser = "0";
    UserLogin::close();
}
