#include "chatpage.h"
#include "ui_chatpage.h"
#include "clickedbtn.h"
#include "clickedlabel.h"

#include <QPainter>
#include <qstyleoption.h>
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
