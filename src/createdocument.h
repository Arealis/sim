#ifndef CREATEDOCUMENT_H
#define CREATEDOCUMENT_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>
#include <QCompleter>
#include <QLineEdit>
#include <QKeyEvent>
#include <QSqlQueryModel>

class CreateDocument;

namespace Ui {
class CreateDocument;
}

class ResizableTable : public QTableWidget {
    Q_OBJECT

public:
    explicit ResizableTable(CreateDocument* parent = nullptr);

    int finalRow;
    int cfRow, customButtonRow;
    int priceCol, qtyCol;

    QStringList colNames;

    QSqlQueryModel *itemModel, *projectModel, *supplierModel;

    void fetchRows(QSqlQuery qry, int tFlag, QString docnum);

    void appendRow();

    void resizeEvent(QResizeEvent *event);

    enum lineType {Item, Project, Supplier};
    void customLineEdit(int row, lineType type);

    void initializeColumns(int tflag);
};

class CreateDocument : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDocument(QWidget *parent = nullptr);
    ~CreateDocument();

    int cfRow, customButtonRow;

    ResizableTable *table;

    void setTotal(double total);

private slots:
    void on_addCustom_clicked();

    void on_save_clicked();

    void on_cancel_clicked();

    void on_saveDraft_clicked();

    void storeTable(int tableFlag, QString oldDocNum, QString newDocNum);

    void insertRecurringCustomDetails(QSqlQuery qry, QString tableFlag, int *row);

    void fetchCustomDetails(QSqlQuery qry, int *row, QString tableFlag, QString docNum, bool editable);

    void deleteCustomDetail();

    void fetchDetails(QSqlQuery qry, int tFlag, QString docnum);

    void DeleteDocument();
private:
    Ui::CreateDocument *ui;
};

QString concatAddress(QSqlQuery sqlQuery, int startsAt, int length);

QString returnStringINN(QVariant sqlValue, QString ifNotNull, QString ifNull);


extern QString tflag, currentUser, docnum;

inline QString stringAt(QCompleter *completer, int column)
{
    return completer->currentIndex().siblingAtColumn(column).data().toString();
}

inline QString cssclear1 = "color: #000000; background-color: #ffffff;"; //red
inline QString cssalert1 = "color: #aa4471; background-color: #c7c7c7;"; //grey

#endif // CREATEDOCUMENT_H
