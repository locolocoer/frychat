#include "chatuserlist.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
ChatUserList::ChatUserList(QWidget *parent):QListWidget(parent)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->viewport()){
        if(event->type() == QEvent::Enter){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }else if(event->type() == QEvent::Leave){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
        if(event->type() == QEvent::Wheel){
            QWheelEvent* wheel = static_cast<QWheelEvent*>(event);
            int numDegrees = wheel->angleDelta().y() / 8;
            int numSteps = numDegrees / 15; // 计算滚动步数
            // 设置滚动幅度
            this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);
            QScrollBar* scrollbar = this->verticalScrollBar();
            int maxValue = scrollbar->maximum();
            int currentValue = scrollbar->value();
            if(maxValue-currentValue<=0){
                qDebug()<<"load more";
                emit sig_load_user();
            }
        }

    }
    return QListWidget::eventFilter(watched,event);
}
