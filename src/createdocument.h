#ifndef CREATEDOCUMENT_H
#define CREATEDOCUMENT_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>

namespace Ui {
class CreateDocument;
}


class CreateDocument : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDocument(QWidget *parent = nullptr);
    ~CreateDocument();

private slots:
    void on_addCustom_clicked();

    void on_save_clicked();

    void on_cancel_clicked();

    void on_saveDraft_clicked();

    void storeTable(int tableFlag, QString oldDocNum, QString newDocNum);

    void insertRecurringCustomFields(QSqlDatabase db, QString tableFlag, int *row);

    void onCellChanged(int row, int column);

    void deleteCustom();

private:
    Ui::CreateDocument *ui;
};

class ResizableTable : public QTableWidget {
    Q_OBJECT
public:
    explicit ResizableTable(QWidget* parent = nullptr);

    void resizeEvent(QResizeEvent *event);


};

QString concatAddress(QSqlQuery sqlQuery, int startsAt, int length);

QString returnStringINN(QVariant sqlValue, QString ifNotNull, QString ifNull);


extern QString tflag;
extern QString currentUser;

#endif // CREATEDOCUMENT_H
