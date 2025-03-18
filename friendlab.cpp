#include "friendlab.h"
#include "ui_friendlab.h"

FriendLab::FriendLab(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FriendLab)
{
    ui->setupUi(this);
    ui->closeLab->SetState("normal","hover","press","selected_normal","selected_hover","selected_press");
    connect(ui->closeLab,&ClickedLabel::clicked,this,&FriendLab::slot_close);
}

FriendLab::~FriendLab()
{
    delete ui;
}

void FriendLab::SetText(QString text)
{
    _text = text;
    ui->tipLab->setText(text);
    ui->tipLab->adjustSize();
    QFontMetrics fontMetrics(ui->tipLab->font());
    //auto textWidth = fontMetrics.lineWidth();
    auto textHeight = fontMetrics.height();
    //qDebug()<<"tiplab"<<ui->tipLab->width()<<"closelab"<<ui->closeLab->width();
    this->setFixedWidth(ui->tipLab->width()+ui->closeWid->width()+5);
    this->setFixedHeight(textHeight+2);
    _width = this->Width();
    _height = this->Height();
}

int FriendLab::Width()
{
    return _width;
}

int FriendLab::Height()
{
    return _height;
}

QString FriendLab::Text()
{
    return _text;
}

void FriendLab::slot_close()
{
    emit sig_close(_text);
}
