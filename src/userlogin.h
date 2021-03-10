#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QDialog>

namespace Ui {
class UserLogin;
}

class UserLogin : public QDialog
{
    Q_OBJECT

public:
    explicit UserLogin(QWidget *parent = nullptr);
    ~UserLogin();

private slots:
    void on_acceptButton_clicked();

    void on_cancelButton_clicked();

    void on_usrLineEdit_editingFinished();

private:
    Ui::UserLogin *ui;
};

extern QString currentUser;
extern char loginFlag;


#endif // USERLOGIN_H
