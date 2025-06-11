#include "chatwidget.h"
#include "ui_chatwidget.h"
#include <QAction>
#include <QPainter>
#include <QRandomGenerator>
#include "chatuseritem.h"
#include <QListWidgetItem>
#include "global.h"
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

    QPixmap pixmap(":/images/head_1.jpg");
    QPixmap scaledPixmap = pixmap.scaled(ui->sideHeadLab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->sideHeadLab->setPixmap(scaledPixmap);
    ui->sideChatLab->setProperty("state","normal");
    ui->sideChatLab->SetState("normal","hover","press","selected_normal","selected_hover","selected_press");
    ui->sideContactLab->SetState("normal","hover","press","selected_normal","selected_hover","selected_press");
    addLbGroup(ui->sideChatLab);
    addLbGroup(ui->sideContactLab);
    connect(ui->sideChatLab,&StateWidget::clicked,this,&ChatWidget::slot_chatLab_click);
    connect(ui->sideContactLab,&StateWidget::clicked,this,&ChatWidget::slot_contactLab_click);

    connect(ui->searchEdit,&CustomizeEdit::textChanged,this,&ChatWidget::slot_text_changed);
    this->installEventFilter(this);
    ui->sideChatLab->SetSelected(true);


    QAction* searchAction = new QAction(this);
    searchAction->setIcon(QIcon(":/images/search.png"));
    ui->searchEdit->addAction(searchAction,QLineEdit::LeadingPosition);
    ui->searchEdit->setPlaceholderText("搜索");
    QAction* clearAction = new QAction(this);
    clearAction->setIcon(QIcon(":/images/close_transparent.png"));
    ui->searchEdit->addAction(clearAction,QLineEdit::TrailingPosition);
    connect(ui->searchEdit,&CustomizeEdit::textChanged,this,[clearAction](const QString& text){
        if(!text.isEmpty()){
            clearAction->setIcon(QIcon(":/images/close_search.png"));
        }else{
            clearAction->setIcon(QIcon(":/images/close_transparent.png"));
        }
    });
    connect(clearAction,&QAction::triggered,this,[this,clearAction](){
        ui->searchEdit->clear();
        clearAction->setIcon(QIcon(":/images/close_transparent.png"));
        ui->searchEdit->clearFocus();
        //emit sig_clear_text();
    });
    ui->searchList->SetSearchEdit(ui->searchEdit);
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

void ChatWidget::addLbGroup(StateWidget *lb)
{
    _lb_group.push_back(lb);
}

void ChatWidget::clearOtherSelectState(StateWidget *lb)
{
    for(auto item:_lb_group){
        if(lb!=item){
            item->ClearState();
        }
    }
}

void ChatWidget::handleGlobalMouseEvent(QMouseEvent* ev)
{
    if(_mode!=ChatUIMode::SearchMode){
        return;
    }

    QPoint po = ui->searchList->mapFromGlobal(ev->globalPos());

    if(!ui->searchList->rect().contains(po)){
        ui->searchEdit->clear();
        showSearch(false);
    }
}

bool ChatWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMouseEvent(mouseEvent);

    }
    return QWidget::eventFilter(watched,event);
}

void ChatWidget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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

void ChatWidget::slot_chatLab_click()
{
    clearOtherSelectState(ui->sideChatLab);
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    _state = ChatUIMode::ChatMode;
    ui->searchEdit->clear();
    showSearch(false);
}

void ChatWidget::slot_contactLab_click()
{
     clearOtherSelectState(ui->sideContactLab);
    ui->stackedWidget->setCurrentWidget(ui->frientApplyPage);
     _state = ChatUIMode::ContactMode;
    ui->searchEdit->clear();
    showSearch(false);
}

void ChatWidget::slot_text_changed(const QString &text)
{
    if(!text.isEmpty()){
        showSearch(true);
    }else{
        showSearch(false);
    }
}
