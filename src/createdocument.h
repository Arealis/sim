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
#include <QComboBox>

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
        char unit = 4;
        char qty = 5;
        char unitPrice;
        char supplierId;
        char supplier;
        char status;
        char total;
        char taxable;
        char project;
        char notes;
        char prdId;
        char condition;
        char podId;
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

    void calculateSubtotal(int row);

    void storeRows(QSqlQuery qry, QString docnum, int status);

    void fetchRows(QSqlQuery qry, QString docnum, TableFlag modifier = PR); //The modifier is set to PR by default because PR has no purpose as a modifier.

    void appendRow();

    bool itemEntryValid(int row);

    bool qtyEntryValid(int row);

    QString validateItemId(QSqlQuery qry, int row);

    void checkProject(QSqlQuery qry, int row);

    QString validateSupplierId(QSqlQuery qry, int row);

    void resizeEvent(QResizeEvent *event);

    void initCompanionLineEdit(int row, int column, QLineEdit *sourceLine, QLineEdit *line2, QLineEdit *line3, QLineEdit *line4, QString cssValid, QString cssInvalid);

    void changeCompletionModel(QLineEdit *line, QString itemId, QString value);

    enum lineType {Item, Supplier, Disabled};
    void initLineEdit(int row, lineType type, int column = 0);
    void initTaxCheckBox(int row, int column);

    void initializeColumns();

    void onCustomContextMenuRequested(const QPoint &pos);
};

class CreateDocument : public QDialog
{
private:
    Q_OBJECT

public:
    explicit CreateDocument(TableFlag tableFlag, QString docNum, User *userData, QString connectionName, QWidget *parent = nullptr, bool authorizer = true);
    ~CreateDocument();

    User *user;
    bool authorized;
    TableFlag tFlag;
    QSqlDatabase db;
    QString docname, docnum, totalsColumns;
    int customDetailRow, customDetailCount, addDetailButtonRow, recurringCustomDetailCount, customExpenseRowStart, customExpenseRowCount, taxableAmountRow;
    bool taxExempt, discBeforeTax;

    ResizableTable *table;

    //All of these are the totals
    QCheckBox *discountBeforeTax;
    QLabel *taxableSubtotal, *taxExemptSubtotal, *taxableAmount, *taxExemptAmount, *total, *addExpenseLabel;
    QLineEdit *discountAfterTax, *tax, *discountOnTaxExempt, *discountOnTaxable;
    QFrame *line1, *line2;
    QPushButton *addExpenseButton;

    void clear();

    //Finds the row of a model that matches the given string
    inline int matchingRow(QString matchString, QAbstractItemModel *model, int matchColumn = 1);

    bool documentEntryValid();
    inline bool supplierEntryValid();
    QString validateSupplierId(QSqlQuery qry, QString supplierId, QString supplierName, QString address = QString(), QString internal = QString());

    void initTotals();

    void setSubtotal(double total, bool isTaxExempt);

    enum WidgetType{ShippingAddress, BillingAddress, Supplier};
    QWidget *initStaticInfoWidget(QWidget *parent, WidgetType widgetType, QString title = QString());

    QWidget *initDynamicInfoWidget(QWidget *parent, WidgetType widgetType,  QSqlDatabase db, QString title = QString());

private slots:

    void fetchTotals(QSqlQuery qry);

    QString fetchTotalsValuesForSql();

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

    void storeCustomExpenses(QSqlQuery qry, QString docnum);

    void storeTable(QSqlQuery qry, QString oldDocNum, QString newDocNum, int status = 0);

    void storeCustomDetails(QSqlQuery qry, QString docnum);

    void insertRecurringCustomDetails(QSqlQuery qry, int *row);

    void fetchCustomDetails(QSqlQuery qry, bool editable);

    void deleteCustomDetail();

    void fetchDetails(QSqlQuery qry, QString docnum, TableFlag modifier = PR); //The modifier is set to PR by default because PR has no purpose as a modifier.

    void DeleteDocument(QSqlQuery qry, QString docnum);


private:
    Ui::CreateDocument *ui;
};

enum ModelFlag {Item, Supplier, Project};
QSqlQueryModel *initReadOnlyModel(ModelFlag modelFlag, QSqlDatabase db, QWidget *parent);
QStandardItemModel *initReadWriteModel(ModelFlag modelFlag, QSqlDatabase db, QWidget *parent);

QComboBox *initComboBox(QAbstractItemModel *model, QWidget *parent);
QDateTimeEdit *initDateTimeEdit(QWidget *parent);
QLineEdit *initCompletedLineEdit(QSqlQueryModel *model, QWidget *parent = nullptr);

inline void onCompletion(QLineEdit *line, void (*function)(QLineEdit*, bool) = nullptr);

inline bool POIsValidAndOpen(QSqlQuery qry, QString poNum);
QString validateProject(QSqlQuery qry, QString project);

QString insertNewSupplier(QSqlQuery qry, QString supplierName, QString address = QString(), QString internal = QString());
QString insertNewItem(QSqlQuery qry, QString itemNum, QString itemDesc, QString unit = QString(), QString category = QString());

void emitSignalOnCompletion(QLineEdit *line);

void appendErrorMessage(QString *orignalMessage, QString appendage, bool *error = nullptr);
void throwErrorMessage(QString message, QString title = "Error");
inline QString stringAt(QCompleter *completer, int column);

#endif // CREATEDOCUMENT_H
