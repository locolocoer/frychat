#include "chatitembase.h"

#include <QGridLayout>


ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent):_role(role),QWidget(parent)
{
    _nameLab = new QLabel();
    _nameLab->setObjectName("chatUserName");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    _nameLab->setFont(font);
    _nameLab->setFixedHeight(20);

    _icoLab = new QLabel();
    _icoLab->setScaledContents(true);
    _icoLab->setFixedSize(42,42);

    _bubbleWid = new QWidget();

    QGridLayout *pGLayout = new QGridLayout();
    pGLayout->setVerticalSpacing(3);
    pGLayout->setHorizontalSpacing(3);
    pGLayout->setContentsMargins(3,3,3,3);

    QSpacerItem *pSpacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Maximum);
    if(_role == ChatRole::self){
        _nameLab->setContentsMargins(0,0,8,0);
        _nameLab->setAlignment(Qt::AlignRight);
        pGLayout->addWidget(_nameLab,0,1,1,1);
        pGLayout->addWidget(_icoLab,0,2,2,1,Qt::AlignTop);
        pGLayout->addItem(pSpacer,1,0,1,1);
        pGLayout->addWidget(_bubbleWid,1,1,1,1);
        pGLayout->setColumnStretch(0,2);
        pGLayout->setColumnStretch(1,3);
    }else{
        _nameLab->setContentsMargins(8,0,0,0);
        _nameLab->setAlignment(Qt::AlignLeft);
        pGLayout->addWidget(_nameLab,0,1,1,1);
        pGLayout->addWidget(_icoLab,0,0,2,1,Qt::AlignTop);
        pGLayout->addItem(pSpacer,2,2,1,1);
        pGLayout->addWidget(_bubbleWid,1,1,1,1);
        pGLayout->setColumnStretch(2,2);
        pGLayout->setColumnStretch(1,3);
    }
    this->setLayout(pGLayout);
}

void ChatItemBase::setUserName(QString name)
{
    _nameLab->setText(name);
}

void ChatItemBase::setUserIcon(QPixmap &icon)
{
    _icoLab->setPixmap(icon);
}

void ChatItemBase::setWidget(QWidget *w)
{
    QGridLayout *pGLayout = (qobject_cast<QGridLayout*>)(this->layout());
    pGLayout->replaceWidget(_bubbleWid,w);
    delete _bubbleWid;
    _bubbleWid = w;
}
