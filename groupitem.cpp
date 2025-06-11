#include "groupitem.h"
#include "ui_groupitem.h"

GroupItem::GroupItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::GroupItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::GroupTip);
}

GroupItem::~GroupItem()
{
    delete ui;
}

void GroupItem::SetGroupTip(QString tip)
{
    ui->grouptip->setText(tip);
}
