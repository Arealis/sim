#ifndef RECEIVEITEMS_H
#define RECEIVEITEMS_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QCompleter>

namespace Ui {
class ReceiveItems;
}

class ReceiveItems : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveItems(QWidget *parent = nullptr);
    ~ReceiveItems();

    /*This is here because I need these classes to be able to be constructed and deleted from anywhere
    This isn't a good idea because (I believe) a destructor needs to be called for this dynamically allocated
    memory whenever the header file is included, but I don't know how to fix this right now, or whether
    that is even true. Refactoring is necessary.*/
    QCompleter *itemNumCompleter = new QCompleter();
    QCompleter *projNameCompleter = new QCompleter();
    QCompleter *itemTypeCompleter = new QCompleter();
    QSqlQueryModel *itemNumModel = new QSqlQueryModel();
    QSqlQueryModel *projNameModel = new QSqlQueryModel();
    QSqlQueryModel *itemTypeModel = new QSqlQueryModel();

private slots:
    void on_Discard_clicked();

    void on_Confirm_clicked();

    void on_ItemIdText_textChanged(const QString &arg1);

    void on_ItemIdText_editingFinished();

    void on_ItemIdText_returnPressed();

    void onItemIdTextCompleted();

    void on_POComboBox_currentIndexChanged(int index);

    void on_ItemIdComboBox_currentTextChanged(const QString &arg1);

    void on_UsedForText_textChanged(const QString &arg1);

    void onUsedForTextCompleted();

private:
    Ui::ReceiveItems *ui;
};

#endif // RECEIVEITEMS_H
