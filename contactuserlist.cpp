#include "contactuserlist.h"
#include "groupitem.h"
#include "qscrollbar.h"

#include <QEvent>
#include <QRandomGenerator>
#include <QWheelEvent>


ContactUserList::ContactUserList(QWidget *parent):QListWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->viewport()->installEventFilter(this);
    AddContactUserList();
    connect(this,&QListWidget::itemClicked,this,&ContactUserList::slot_item_clicked);
}

void ContactUserList::ShowRedpoint(bool bshow)
{
    _add_friend_item->showRedPoint(bshow);
}

bool ContactUserList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        // 检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10; // 每页加载的联系人数量

        if (maxScrollValue - currentValue <= 0) {
            // 滚动到底部，加载新的联系人
            qDebug()<<"load more contact user";
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loding_contact_user();
        }

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);
}

void ContactUserList::AddContactUserList()
{
    auto groupTip = new GroupItem();
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(groupTip->sizeHint());
    this->addItem(item);
    this->setItemWidget(item,groupTip);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto add_user_item = new ContactUserItem();
    add_user_item->setObjectName("new_friend_item");
    add_user_item->setInfo(0,tr("新的朋友"),":/images/add_friend.png");
    add_user_item->SetItemType(ListItemType::ApplyFriendType);

    QListWidgetItem* add_item = new QListWidgetItem();
    add_item->setSizeHint(add_user_item->sizeHint());
    this->addItem(add_item);
    this->setItemWidget(add_item,add_user_item);
    this->setCurrentItem(add_item);

    auto groupCon = new GroupItem();
    groupCon->SetGroupTip(tr("联系人"));
    QListWidgetItem* itemCon = new QListWidgetItem();
    itemCon->setSizeHint(groupCon->sizeHint());
    this->addItem(itemCon);
    this->setItemWidget(itemCon,groupCon);
    itemCon->setFlags(itemCon->flags() & ~Qt::ItemIsSelectable);

    for(int i=0;i<13;i++){
        int randvalue = QRandomGenerator::global()->bounded(100);
        int head_i = randvalue%heads.size();
        int name_i = randvalue%names.size();
        auto user_item = new ContactUserItem();
        user_item->setInfo(0,names[name_i],heads[head_i]);
        user_item->SetItemType(ListItemType::ContactUser);
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(user_item->sizeHint());
        this->addItem(item);
        this->setItemWidget(item,user_item);
        item->setFlags(item->flags() & Qt::ItemIsSelectable);
    }
}

void ContactUserList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item); // 获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::Invalid
        || itemType == ListItemType::GroupTip){
        qDebug()<< "slot invalid item clicked ";
        return;
    }

    if(itemType == ListItemType::ApplyFriendType){

        // 创建对话框，提示用户
        qDebug()<< "apply friend item clicked ";
        //跳转到好友申请界面
        emit sig_switch_friendinfo_page();
        return;
    }

    if(itemType == ListItemType::ContactUser){
        // 创建对话框，提示用户
        qDebug()<< "contact user item clicked ";
        //跳转到好友申请界面
        emit sig_switch_appyfriend_page();
        return;
    }
}
