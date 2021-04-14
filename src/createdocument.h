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
#include <QPlainTextEdit>

#include "global.h"

//delete this later. It is redundant and stupid.
class CreateDocument;

namespace Ui {
class CreateDocument;
}

class ResizableTable : public QTableWidget {
    Q_OBJECT

public:
    explicit ResizableTable(TableFlag tableFlag, CreateDocument* parent = nullptr);
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

    QSqlDatabase db;
    CreateDocument *parentDoc;
    columnId cid;
    TableFlag tflag;

    QStringList colNames;
    QString docname;

    int finalRow;
    double taxableSubtotal, taxExemptSubtotal;


    QMenu *menu;
    QAction *deleteRow;


    void updateRowTotal(int row);

    void storeRows(QSqlQuery qry, QString docnum);

    void fetchRows(QSqlQuery qry, QString docnum);

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

    void initializeColumns();

    void onCustomContextMenuRequested(const QPoint &pos);
};

class CreateDocument : public QDialog
{
private:
    Q_OBJECT

public:
    explicit CreateDocument(TableFlag tableFlag, QString docnum, User *user, QString databaseName, QWidget *parent = nullptr);
    ~CreateDocument();

    User *user;
    TableFlag tFlag;
    QSqlDatabase db;
    QString docname, docnum;
    int customDetailRow, customDetailCount, addDetailButtonRow, recurringCustomDetailCount, customExpenseRowStart, customExpenseRowCount, taxableAmountRow;
    bool taxExempt, discBeforeTax;

    ResizableTable *table;

    //All of these are the totals
    QCheckBox *discountBeforeTax;
    QLabel *taxableSubtotal, *taxExemptSubtotal, *taxableAmount, *taxExemptAmount, *total, *addExpenseLabel;
    QLineEdit *discountAfterTax, *tax, *discountOnTaxExempt, *discountOnTaxable;
    QFrame *line1, *line2;
    QPushButton *addExpenseButton;

    void initTotals();

    void updateTotal(double total, bool isTaxExempt);

private slots:

    void fetchTotals(QSqlQuery qry);

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

    void on_saveDraft_clicked(QSqlDatabase db);

    void storeCustomExpenses(QSqlQuery qry, QString docnum);

    void storeTable(QSqlQuery qry, QString oldDocNum, QString newDocNum, int status = 0);

    void storeCustomDetails(QSqlQuery qry, QString docnum);

    void insertRecurringCustomDetails(QSqlQuery qry, int *row);

    void fetchCustomDetails(QSqlQuery qry, int *row, bool editable);

    void deleteCustomDetail();

    void deleteCustomExpense();

    void fetchDetails(QSqlQuery qry);

    void DeleteDocument(QSqlQuery qry, QString docnum);

private:
    Ui::CreateDocument *ui;
};

enum ModelFlag {Item, Supplier, Project, Custom};
QSqlQueryModel *initModel(ModelFlag modelFlag, QSqlDatabase db, QWidget *parent, QString query = NULL);

QDateTimeEdit *initDateTimeEdit(QWidget *parent);
QLineEdit *initCompletedLineEdit(QSqlQueryModel *model, QWidget *parent = nullptr, int column = 0);
QWidget *initInfoWidget(QWidget *parent, QString title);
QWidget *initInfoWidget(QWidget *parent, QString title, QSqlQueryModel *model);

void infoWidgetConnector(QLineEdit *line, bool success);
inline void onCompletion(QLineEdit *line, void (*function)(QLineEdit*, bool) = nullptr);

bool validateProject(QSqlQuery qry, QString project);
QString validateSupplier(QSqlQuery qry, QString supplier, bool *newEntry);
QString validateItem(QSqlQuery qry, QString itemNum, QString itemDesc, QString unit, QString category);

inline QString insertNewSupplier(QSqlQuery qry, QString supplier);
inline QString insertNewItem(QSqlQuery qry, QString itemNum, QString itemDesc, QString unit, QString category);

void emitSignalOnCompletion(QLineEdit *line);

QString returnStringINN(QVariant sqlValue, QString ifNotNull, QString ifNull);

QString stringAt(QCompleter *completer, int column);

#endif // CREATEDOCUMENT_H
