#include "searchlist.h"
#include "qscrollbar.h"
#include "tcpmgr.h"
#include "adduseritem.h"
#include "customizeedit.h"
#include "findsucessdlg.h"
#include "loadingdlg.h"
#include <QJsonDocument>

SearchList::SearchList(QWidget *parent):QListWidget(parent),_find_dlg(nullptr),_search_edit(nullptr),_send_pending(false)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
    connect(this,&SearchList::itemClicked,this,&SearchList::slot_item_clicked);
    connect(tcpMgr::GetInstance().get(),&tcpMgr::sig_user_search,this,&SearchList::slot_user_search);
    addTipItem();
}

void SearchList::closeFindDlg()
{
    if(_find_dlg){
        _find_dlg->hide();
        _find_dlg=nullptr;
    }
}

void SearchList::SetSearchEdit(QWidget *edit)
{
    _search_edit = edit;
}

bool SearchList::eventFilter(QObject *watched, QEvent *event)
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

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);
}

void SearchList::waitPending(bool pending)
{
    if(pending){
        _loadingDlg=new LoadingDlg(this);
        _loadingDlg->show();
        _send_pending = pending;
    }else{
        _loadingDlg->hide();
        _send_pending = pending;
        _loadingDlg->deleteLater();
    }
}

void SearchList::addTipItem()
{
    auto *invalid_item = new QWidget();
    QListWidgetItem* item_temp = new QListWidgetItem();
    item_temp->setSizeHint(QSize(250,10));
    this->addItem(item_temp);
    invalid_item->setObjectName("invalidItem");
    this->setItemWidget(item_temp,invalid_item);

    auto *add_user_item = new AddUserItem();
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item,add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget* wid = this->itemWidget(item);
    if(!wid){
        return;
    }
    ListItemBase* itembase = static_cast<ListItemBase*>(wid);
    auto itemtype = itembase->GetItemType();
    if(itemtype==ListItemType::Invalid){
        return;
    }
    if(itemtype==ListItemType::AddUser){
        if(_send_pending){
            return;
        }
        waitPending(true);
        auto search_edit = dynamic_cast<CustomizeEdit*>(_search_edit);
        auto uid_str = search_edit->text();
        QJsonObject jsonObj;
        jsonObj["uid"]=uid_str;
        QJsonDocument doc(jsonObj);
        QByteArray jsonstr = doc.toJson(QJsonDocument::Indented);
        emit tcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USR_REQ,jsonstr);
    }

    closeFindDlg();
}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    waitPending(false);
    if(si==nullptr){
        _find_dlg = std::make_shared<FindFailedDlg>(this);
    }else{
        _find_dlg =  std::make_shared<FindSucessDlg>(this);
        auto find_sucess_dlg = std::dynamic_pointer_cast<FindSucessDlg>(_find_dlg);
        find_sucess_dlg->setSearchInfo(si);
    }
    return;
}
