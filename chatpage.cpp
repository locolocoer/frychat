#include "chatpage.h"
#include "ui_chatpage.h"
#include "clickedbtn.h"
#include "clickedlabel.h"
#include "textbubble.h"
#include "picturebubble.h"

#include <QPainter>
#include <qstyleoption.h>
#include <QTextEdit>
#include "chatitembase.h"
ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    ui->recvBtn->setState("normal","hover","press");
    ui->sendBtn->setState("normal","hover","press");
    //设置图标样式
    ui->emoLab->SetState("normal","hover","press","normal","hover","press");
    ui->fileLab->SetState("normal","hover","press","normal","hover","press");
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatPage::on_sendBtn_clicked()
{
    auto pTextEdit = ui->chatEdit;
    ChatRole role = ChatRole::self;
    QString userName = QStringLiteral("恋恋风辰");
    const QString userIcon = ":/images/head_1.jpg";
    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    for(int i=0; i<msgList.size(); ++i)
    {
        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        QPixmap pixmap = QPixmap(userIcon);
        pChatItem->setUserIcon(pixmap);
        QWidget *pBubble = nullptr;
        if(type == "text")
        {
            pBubble = new TextBubble(role, msgList[i].content);
        }
        else if(type == "image")
        {
            pBubble = new PictureBubble(QPixmap(msgList[i].content) , role);
        }
        else if(type == "file")
        {
        }
        if(pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->chatDataWid->appendChatItem(pChatItem);
        }
    }
}

