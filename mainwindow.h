#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_actionImport_From_Existing_Database_triggered();

    void on_actionSelect_Database_Location_triggered();

    void on_SearchForDropdown_currentIndexChanged(int index);

    void on_actionPurchase_Orders_triggered();

    void on_actionItems_Table_Default_triggered();

    void on_ItemTable_doubleClicked(const QModelIndex &index);

    void on_SearchBox_textChanged(const QString &arg1);

    void on_ClearSearchButton_clicked();

    void on_actionProjects_triggered();

    void clearDropdown();

    void createPO_Details_Table(QString ponum, QString date, QString supplier);

    void createProject_Details_Table(QString projectid, QString projectname);

    void createItem_Details_Table(QString itemid, QString itemname, QString itemdesc);

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

    void colorRows();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
