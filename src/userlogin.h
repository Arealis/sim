#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QDialog>
#include <QSqlQuery>
#include "global.h"

enum LoginFlag {Login, New, Edit};

namespace Ui {
class UserLogin;
}

class UserLogin : public QDialog
{
    Q_OBJECT

public:
    //NOTE TO SELF: compare the performance of pointers to the performance of raw variables. It'd just be interesting to see.
    explicit UserLogin(QString databaseName, User *currentUser, LoginFlag flag, QWidget *parent = nullptr);
    ~UserLogin();

    QSqlDatabase db;
    QSqlQuery qry;
    LoginFlag loginFlag;
    User *user;

    bool ok;
    QString qryString;
private slots:
    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

    void on_userLineEdit_editingFinished();

private:
    Ui::UserLogin *ui;
};


#endif // USERLOGIN_H
