#ifndef CREATEDOCUMENT_H
#define CREATEDOCUMENT_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>
#include <QCompleter>
#include <QLineEdit>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QMenu>
#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>

class CreateDocument;

namespace Ui {
class CreateDocument;
}

class ResizableTable : public QTableWidget {
    Q_OBJECT

public:
    explicit ResizableTable(CreateDocument* parent = nullptr);

    struct columnId {
        //! This is all the data from the column headers so that they can be referenced in the program
        //! without having to do expensive look ups by passing in string arguments into a QTableWidget function
        //! ... though I have not measured the speed impact that this has. Certainly, this cannot be the best way,
        //! and could use a refactor
        char itemId = 0;
        char itemNum = 1;
        char itemDesc = 2;
        char cat = 3;
        char qty = 4;
        char unit = 5;
        char unitPrice;
        char supplierId;
        char supplier;
        char status;
        char total;
        char taxable;
        char project;
    };

    QMenu *menu;
    QAction *deleteRow;
    columnId cid;

    CreateDocument *parentDoc;

    int finalRow;
    int cfRow, customButtonRow;
    int priceCol, qtyCol;

    double taxableSubtotal, taxExemptSubtotal;

    QStringList colNames;

    QSqlQueryModel *itemModel, *projectModel, *supplierModel;

    void updateTotal(int row);

    void storeTableDetails(QSqlQuery qry, QString docnum, int tableFlag);

    void fetchRows(QSqlQuery qry, int tFlag, QString docnum);

    void appendRow();

    void checkItem(QSqlQuery qry, int row);

    void checkProject(QSqlQuery qry, int row);

    void checkSupplier(QSqlQuery qry, int row);

    void resizeEvent(QResizeEvent *event);

    void initCompanionLineEdit(int row, int column, QLineEdit *sourceLine, QLineEdit *line2, QLineEdit *line3, QLineEdit *line4, QString cssValid, QString cssInvalid);

    void changeCompletionModel(QLineEdit *line, QString itemId, QString value);

    enum lineType {Item, Supplier, Blank};
    void customLineEdit(int row, lineType type, int column = 0);
    void customCheckBox(int row, int column);

    void initializeColumns(int tflag);

    void onCustomContextMenuRequested(const QPoint &pos);
};

class CreateDocument : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDocument(QWidget *parent = nullptr);
    ~CreateDocument();

    int cfRow, customButtonRow, customExpenseRowStart, customExpenseRowCount, taxableAmountRow;
    bool taxExempt, discBeforeTax;

    ResizableTable *table;

    void initializeTotals(int tnum = 0);

    void updateTotal(double total, bool isTaxExempt);

private slots:

    void fetchRateAndAmount(QSqlQuery qry, int rateIndex, QLineEdit *line, QGridLayout *grid, bool custom = 0);

    QLabel* createTotalsLabel(QLabel *nextSubtotal, int *row, QString title, QWidget *parent, QGridLayout *grid);
    QLineEdit* createTotalsLineEdit(QLabel *nextSubtotal, int *row, QString title, QWidget *parent, QGridLayout *grid, bool expense = 1);
    QFrame* createTotalsLine(int *row, QWidget *parent, QGridLayout *grid);

    void setTotalsRowHidden(QGridLayout *grid, QFrame *widget, bool hidden);
    void setTotalsRowHidden(QGridLayout *grid, QLineEdit *widget, bool hidden);
    void setTotalsRowHidden(QGridLayout *grid, QLabel *widget, bool hidden);

    void on_addCustom_clicked();

    void on_save_clicked();

    void on_cancel_clicked();

    void on_saveDraft_clicked();

    void storeCustomExpenses(QSqlQuery qry);

    void storeTable(int tableFlag, QString oldDocNum, QString newDocNum);

    void insertRecurringCustomDetails(QSqlQuery qry, QString tableFlag, int *row);

    void fetchCustomDetails(QSqlQuery qry, int *row, QString tableFlag, QString docNum, bool editable);

    void deleteCustomDetail();

    void deleteCustomExpense();

    void fetchDetails(QSqlQuery qry, int tFlag, QString docnum);

    void DeleteDocument();

private:
    Ui::CreateDocument *ui;
};

bool validateProject(QSqlQuery qry, QString project);
QString validateSupplier(QSqlQuery qry, QString supplier, bool *newEntry);
QString validateItem(QSqlQuery qry, QString itemNum, QString itemDesc, QString unit, QString category);

inline QString insertNewSupplier(QSqlQuery qry, QString supplier);
inline QString insertNewItem(QSqlQuery qry, QString itemNum, QString itemDesc, QString unit, QString category);

void connectCompleterToLine(QLineEdit *line);

QString returnStringINN(QVariant sqlValue, QString ifNotNull, QString ifNull);

extern QString tflag, currentUser, docnum;

inline QString stringAt(QCompleter *completer, int column)
{
    return completer->currentIndex().siblingAtColumn(column).data().toString();
}

inline QString cssclear1 = "color: #000000; background-color: #ffffff;"; //red
inline QString cssalert1 = "color: #aa4471; background-color: #c7c7c7;"; //grey

#endif // CREATEDOCUMENT_H
