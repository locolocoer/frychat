#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ApplyFriendItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::ApplyFriend);
    ui->addBtn->setState("normal","hover","press");
    ui->addBtn->hide();
    connect(ui->addBtn,&ClickedBtn::clicked,this,[this](){
        emit sig_auth_friend(_apply_info);
    });
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

void ApplyFriendItem::setInfo(std::shared_ptr<ApplyInfo> info)
{
    _apply_info = info;
    QPixmap pixmap(info->_icon);
    ui->iconLab->setPixmap(pixmap.scaled(ui->iconLab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->userNameLab->setText(info->_name);
    ui->userChatLab->setText(info->_desc);
}

void ApplyFriendItem::showBtn(bool b_show)
{
    if(b_show){
        ui->addBtn->show();
    }
    ui->addBtn->hide();
}

int ApplyFriendItem::getUid()
{
    return _apply_info->_uid;
}
