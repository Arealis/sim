#ifndef DISTRIBUTEITEMS_H
#define DISTRIBUTEITEMS_H

#include <QDialog>

namespace Ui {
class DistributeItems;
}

class DistributeItems : public QDialog
{
    Q_OBJECT

public:
    explicit DistributeItems(QWidget *parent = nullptr);
    ~DistributeItems();

private:
    Ui::DistributeItems *ui;
};

#endif // DISTRIBUTEITEMS_H
