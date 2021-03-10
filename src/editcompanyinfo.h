#ifndef EDITCOMPANYINFO_H
#define EDITCOMPANYINFO_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class EditCompanyInfo;
}

class EditCompanyInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditCompanyInfo(QWidget *parent = nullptr);
    ~EditCompanyInfo();

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::EditCompanyInfo *ui;
};

#endif // EDITCOMPANYINFO_H
