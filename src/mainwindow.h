#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QComboBox>

#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    User *user;
    QSqlDatabase simdb;
    QSqlQuery qry;
    QSqlQueryModel *model;

    QString query;

    QStringList *headers;
    void refresh();
    void getHeaders();

    void displayTable(QString title, QSqlDatabase database, QString query);

    QString openClosedDraftCases(QString column);

    inline QString siblingAtHeader(const QModelIndex &index, QString headerName);

    inline void hideColumnsVariadic();
    template <typename Current, typename ... Extras>
    inline void hideColumnsVariadic(Current header, Extras ... moreHeaders);
    template <typename ... Headers>
    inline void setHiddenColumns(Headers ... headers);

    enum class ColumnFlags : uint8_t {
        Date, Address, CreatorName,
        SupplierId, SupplierName, Project,
        ItemId, ItemNum, ItemDesc, ItemUnit, ItemCat, ItemBin, ItemShelf, Qty,
        DocType, DocNum, Status, PRNum, QRNum, PONum, RRNum, MRNum,
        };
    const std::map<QString, ColumnFlags> columnFlagsMap {
        {"Creator", ColumnFlags::CreatorName},
        {"Created by", ColumnFlags::CreatorName},
        {"Date", ColumnFlags::Date},
        {"Date Created", ColumnFlags::Date},
        {"Date Needed", ColumnFlags::Date},

        {"Doc", ColumnFlags::DocType},
        {"Document", ColumnFlags::DocType},
        {"Doc#", ColumnFlags::DocNum},

        {"PR#", ColumnFlags::PRNum},
        {"QR#", ColumnFlags::QRNum},
        {"PO#", ColumnFlags::PONum},
        {"RR#", ColumnFlags::RRNum},
        {"MR#", ColumnFlags::MRNum},

        {"Project", ColumnFlags::Project},
        {"Used For", ColumnFlags::Project},
        {"supplier_id", ColumnFlags::SupplierId},
        {"Supplier", ColumnFlags::SupplierName},
        {"Recommended Supplier(s)", ColumnFlags::SupplierName},
        {"Address", ColumnFlags::Address},


        {"item_id", ColumnFlags::ItemId},
        {"Item ID", ColumnFlags::ItemNum},
        {"ID", ColumnFlags::ItemNum},
        {"Description", ColumnFlags::ItemDesc},
        {"Unit", ColumnFlags::ItemUnit},
        {"Category", ColumnFlags::ItemCat},
        {"Cat", ColumnFlags::ItemCat},

        /* Bins and shelves need to change. This is not a good way to deal with location */
        {"Bin", ColumnFlags::ItemBin},
        {"Shelf", ColumnFlags::ItemShelf},


        {"Qty", ColumnFlags::Qty},
        {"Qty Reqd.", ColumnFlags::Qty},
        {"Qty Quoted", ColumnFlags::Qty},
        {"Qty Ordered", ColumnFlags::Qty},
        {"Qty In", ColumnFlags::Qty},
        {"Qty Out", ColumnFlags::Qty},
        {"Qty Dist.", ColumnFlags::Qty},
        {"Quantity", ColumnFlags::Qty},
    };
    const std::map<QString, TableFlag> tableFlagsMap {
        {"Purchase Requisition", TableFlag::PR},
        {"PR", TableFlag::PR},

        {"Quotation Request", TableFlag::QR},
        {"Request for Quotation", TableFlag::QR},
        {"QR", TableFlag::QR},

        {"Purchase Order", TableFlag::PO},
        {"PO", TableFlag::PO},

        {"Receiving Report", TableFlag::RR},
        {"RR", TableFlag::RR},

        {"Material Requisition", TableFlag::MR},
        {"MR", TableFlag::MR}
    };

private slots:

    bool checkPrivelage(TableFlag table, User *user);

    void Login();

    void createNewSIMDB();

    void on_SearchForDropdown_currentIndexChanged(int index);

    void on_actionQuotation_Requests_triggered();

    void createQR_Details(QString qrnum);

    void on_actionPurchase_Orders_triggered();

    void on_actionItems_Table_Default_triggered();

    void on_table_doubleClicked(const QModelIndex &index);

    void on_ClearSearchButton_clicked();

    void on_actionProjects_triggered();

    void createPOD_Table(QString ponum);

    void createProject_Details_Table(QString project);

    void createItem_Details_Table(QString itemId);

    void goToHome();

    void on_actionReceived_triggered();

    void createReceived_Details_Table(QString rrnum);

    void on_actionRequested_Items_triggered();

    void createRequested_Details_Table(QString mrnum);

    void on_actionPrint_Current_Table_triggered();

    void on_actionItem_History_triggered();

    void on_actionReceive_Items_triggered();

    void on_table_customContextMenuRequested(const QPoint &pos);

    void addItemToPO();

    void addItemToPR();

    void addItemToDist();

    void on_actionPing_Databse_triggered();

    void on_actionClean_Database_triggered();

    void colorRows();

    void on_actionSuppliers_triggered();

    void createSuppliers_Details_Table(QString supplierId);

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

    void on_SearchBox_returnPressed();

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
