#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void Login();

    void createNewSIMDB();

    void on_SearchForDropdown_currentIndexChanged(int index);

    void on_actionPurchase_Orders_triggered();

    void on_actionItems_Table_Default_triggered();

    void on_ItemTable_doubleClicked(const QModelIndex &index);

    void on_SearchBox_textChanged(const QString &arg1);

    void on_ClearSearchButton_clicked();

    void on_actionProjects_triggered();

    void clearDropdown();

    void createPOD_Table(QString ponum);

    void createProject_Details_Table(QString project);

    void createItem_Details_Table(QString itemid);

    void goToHome();

    void on_actionReceived_triggered();

    void createReceived_Details_Table(QString rcr, QString date);

    void showAllColumns();

    void on_actionRequested_Items_triggered();

    void createRequested_Details_Table(QString mrq, QString date, QString reqby);

    void on_actionPrint_Current_Table_triggered();

    void on_actionItem_History_triggered();

    void on_actionReceive_Items_triggered();

    void on_ItemTable_customContextMenuRequested(const QPoint &pos);

    void addItemToPO();

    void addItemToPR();

    void addItemToDist();

    void on_actionPing_Databse_triggered();

    void on_actionClean_Database_triggered();

    void colorRows();

    void on_actionSuppliers_triggered();

    void createSuppliers_Details_Table(QString supplier_id, QString supplier_name);

    void on_actionCreate_New_User_triggered();

    void on_actionChange_Current_User_triggered();

    void on_actionPurchase_Requisitions_triggered();

    void createPR_Details(QString prnum);

    void prepareBaseTable();

    void on_actionEdit_Current_Userdata_triggered();

    void on_actionCreate_Purchase_Requisition_triggered();

    void on_actionEdit_Company_Information_triggered();

    void on_actionCreate_Quotation_Request_triggered();

    void on_actionCreate_Purchase_Order_triggered();

    void on_actionDistribute_Inventory_triggered();

private:
    Ui::MainWindow *ui;
};

/*
class QSqlQueryModelRW : public QSqlQueryModel{
public:
    using QSqlQueryModel::QSqlQueryModel;

    Qt::ItemFlags flags(const QModelIndex &index) const {
        return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) {
        if (index.isValid() && role == Qt::EditRole) {
            return true;
        }
        return false;
    }
};
*/

#endif // MAINWINDOW_H
