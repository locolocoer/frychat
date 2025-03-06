#include "chatuseritem.h"
#include "ui_chatuseritem.h"
#include "global.h"
ChatUserItem::ChatUserItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ChatUserItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::ChatUser);
}

ChatUserItem::~ChatUserItem()
{
    delete ui;
}

void ChatUserItem::SetInfo(QString name, QString head, QString msg)
{
    _name = name;
    _head = head;
    _msg = msg;
    QPixmap pixmap(_head);
    ui->iconLab->setPixmap(pixmap.scaled(ui->iconLab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->iconLab->setScaledContents(true);
    ui->nameLab->setText(_name);
    _msg = _msg.left(15)+"...";
    ui->msgLab->setText(_msg);

}
