#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include <QLabel>
#include <qwidget.h>
#include "global.h"



class ChatItemBase:public QWidget
{
    Q_OBJECT
public:
    ChatItemBase(ChatRole role,QWidget* parrent=nullptr);
    void setUserName(QString name);
    void setUserIcon(QPixmap &icon);
    void setWidget(QWidget* w);
private:
    ChatRole _role;
    QLabel* _icoLab;
    QLabel* _nameLab;
    QWidget* _bubbleWid;
};

#endif // CHATITEMBASE_H
