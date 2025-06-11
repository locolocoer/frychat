#include "applyfriendpage.h"
#include "ui_applyfriendpage.h"
#include "tcpmgr.h"
#include "usermgr.h"
#include "applyfriendlist.h"

#include <QPainter>
#include <QRandomGenerator>
ApplyFriendPage::ApplyFriendPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
    connect(ui->applyFriendList,&ApplyFriendList::sig_show_search,this,&ApplyFriendPage::sig_show_search);
    connect(tcpMgr::GetInstance().get(),&tcpMgr::sig_auth_rsp,this,&ApplyFriendPage::slot_auth_rsp);
    loadApplyList();
}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}

void ApplyFriendPage::addNewApply(std::shared_ptr<AddFriendApply> apply)
{
    int randValue = QRandomGenerator::global()->bounded(100);
    int head_i = randValue%heads.size();
    //int name_i = randValue%names.size();
    auto * applyItem = new ApplyFriendItem();
    auto applyinfo = std::make_shared<ApplyInfo>(heads[head_i],apply->_name,apply->_desc,"",apply->_from_uid,0,1);
    applyItem->setInfo(applyinfo);
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(applyItem->sizeHint());
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
    ui->applyFriendList->insertItem(0,item);
    ui->applyFriendList->setItemWidget(item, applyItem);
    applyItem->showBtn(true);
    connect(applyItem,&ApplyFriendItem::sig_auth_friend,this,[this](std::shared_ptr<ApplyInfo> apply_info){

    });
}

void ApplyFriendPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ApplyFriendPage::loadApplyList()
{
    auto apply_list = UserMgr::GetInstance()->GetApplyList();
    for(auto apply:apply_list){
        int randValue = QRandomGenerator::global()->bounded(100);
        int head_i = randValue%heads.size();
        apply->setIcon(heads[head_i]);
        auto applyitem = new ApplyFriendItem();
        applyitem->setInfo(apply);
        QListWidgetItem *item = new QListWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->applyFriendList->insertItem(0,item);
        ui->applyFriendList->setItemWidget(item, applyitem);
        if(apply->_status){
            applyitem->showBtn(false);
        }else{
            applyitem->showBtn(true);
            int uid =  apply->_uid;
            _unauth_items[uid]=applyitem;
        }
        connect(applyitem,&ApplyFriendItem::sig_auth_friend,this,[this](std::shared_ptr<ApplyInfo> apply_info){

        });
    }
    // 模拟假数据，创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *apply_item = new ApplyFriendItem();
        auto apply = std::make_shared<ApplyInfo>(heads[head_i],names[name_i], strs[str_i],
                                                  names[name_i], 0, 0, 1);
        apply_item->setInfo(apply);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->applyFriendList->addItem(item);
        ui->applyFriendList->setItemWidget(item, apply_item);
        //收到审核好友信号
        connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info){
            //            auto *authFriend =  new AuthenFriend(this);
            //            authFriend->setModal(true);
            //            authFriend->SetApplyInfo(apply_info);
            //            authFriend->show();
        });
    }
}

void ApplyFriendPage::slot_auth_rsp(std::shared_ptr<AuthRsp> rsp)
{
    int uid = rsp->_uid;
    auto find_iter = _unauth_items.find(uid);
    if(find_iter!=_unauth_items.end()){
        _unauth_items[uid]->showBtn(false);
    }
}
