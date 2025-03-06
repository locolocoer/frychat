#include "chatwidget.h"
#include "ui_chatwidget.h"
#include <QAction>
#include <QRandomGenerator>
#include "chatuseritem.h"
#include <QListWidgetItem>
ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent),_state(ChatUIMode::ChatMode)
    , ui(new Ui::ChatWidget),_b_loading(false)
{
    ui->setupUi(this);
    ui->addBtn->setState("normal","hover","press");
    ui->searchEdit->setMaxLength(15);
    connect(ui->searchEdit,&CustomizeEdit::sig_clear_text,this,[this]{
        showSearch(false);
    });
    showSearch(false);
    addChatUserList();
    connect(ui->chatUserList,&ChatUserList::sig_load_user,this,&ChatWidget::slot_loading_user);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::showSearch(bool bsearch)
{
    if(bsearch){
        ui->chatUserList->hide();
        ui->searchList->show();
        ui->conUserList->hide();
        _mode = ChatUIMode::SearchMode;
    }else if(_state == ChatUIMode::ChatMode){
        ui->chatUserList->show();
        ui->searchList->hide();
        ui->conUserList->hide();
        _mode = ChatUIMode::ChatMode;
    }else if(_state == ChatUIMode::ContactMode){
        ui->conUserList->show();
        ui->searchList->hide();
        ui->chatUserList->hide();
        _mode = ChatUIMode::ContactMode;
    }
}

std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};
std::vector<QString> heads = {
    ":/images/head_1.jpg",
    ":/images/head_2.jpg",
    ":/images/head_3.jpg",
    ":/images/head_4.jpg",
    ":/images/head_5.jpg"
};
std::vector<QString> names = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};

void ChatWidget::addChatUserList()
{
    for(int i=0;i<13;i++){
        int randValue = QRandomGenerator::global()->bounded(100);
        int msg_i = randValue%strs.size();
        int name_i = randValue%names.size();
        int head_i = randValue%heads.size();
        QString msg = strs[msg_i];
        QString name = names[name_i];
        QString head = heads[head_i];

        auto chatUserItem = new ChatUserItem(this);
        chatUserItem->SetInfo(name,head,msg);
        auto item = new QListWidgetItem;
        item->setSizeHint(chatUserItem->sizeHint());
        ui->chatUserList->addItem(item);
        ui->chatUserList->setItemWidget(item,chatUserItem);
    }

}

void ChatWidget::slot_loading_user()
{
    if(_b_loading){
        return;
    }
    _b_loading = true;

    addChatUserList();
    _b_loading =false;
}
