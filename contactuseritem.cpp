#include "contactuseritem.h"
#include "ui_contactuseritem.h"

ContactUserItem::ContactUserItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ContactUserItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::ContactUser);
    ui->redPoint->raise();
    showRedPoint(false);
}

ContactUserItem::~ContactUserItem()
{
    delete ui;
}

void ContactUserItem::showRedPoint(bool show)
{
    if(show){
        ui->redPoint->show();
    }else{
        ui->redPoint->hide();
    }

}

QSize ContactUserItem::sizeHint() const
{
    return QSize(250,70);
}

void ContactUserItem::setInfo(std::shared_ptr<AuthInfo> auth_info)
{
    _info = std::make_shared<UserInfo>(auth_info);
    QPixmap pixmap(_info->_icon);
    ui->iconLab->setPixmap(pixmap.scaled(ui->iconLab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->userNameLab->setText(_info->_name);
}

void ContactUserItem::setInfo(std::shared_ptr<AuthRsp> auth_rsp)
{
    _info = std::make_shared<UserInfo>(auth_rsp);
    QPixmap pixmap(_info->_icon);
    ui->iconLab->setPixmap(pixmap.scaled(ui->iconLab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->userNameLab->setText(_info->_name);
}

void ContactUserItem::setInfo(int uid, QString name, QString icon)
{
    _info = std::make_shared<UserInfo>(uid,name,icon);
    QPixmap pixmap(_info->_icon);
    ui->iconLab->setPixmap(pixmap.scaled(ui->iconLab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->userNameLab->setText(_info->_name);
}
