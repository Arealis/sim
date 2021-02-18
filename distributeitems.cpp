#include "distributeitems.h"
#include "ui_distributeitems.h"

DistributeItems::DistributeItems(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DistributeItems)
{
    ui->setupUi(this);
}

DistributeItems::~DistributeItems()
{
    delete ui;
}
