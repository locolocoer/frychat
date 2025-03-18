#include "bubbleframe.h"
#include <QPainter>


const int WIDTH_TRIANGLE = 8;
BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent):QFrame(parent),_role(role),_margin(3)
{
    _pHLayout = new QHBoxLayout();
    if(_role==ChatRole::self){
        _pHLayout->setContentsMargins(_margin,_margin,_margin+WIDTH_TRIANGLE,_margin);
    }else{
        _pHLayout->setContentsMargins(_margin+WIDTH_TRIANGLE,_margin,_margin,_margin);
    }
    this->setLayout(_pHLayout);
}

void BubbleFrame::setMargin(int margin)
{
    _margin = margin;
}

void BubbleFrame::setWidget(QWidget *w)
{
    if(_pHLayout->count()>0) return;
    _pHLayout->addWidget(w);
}

void BubbleFrame::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    if(_role == ChatRole::other)
    {
        //画气泡
        QColor bk_color(Qt::white);
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(WIDTH_TRIANGLE, 0, this->width()-WIDTH_TRIANGLE, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        //画小三角
        QPointF points[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 10+WIDTH_TRIANGLE +2),
            QPointF(bk_rect.x()-WIDTH_TRIANGLE, 10+WIDTH_TRIANGLE-WIDTH_TRIANGLE/2),
        };
        painter.drawPolygon(points, 3);
    }
    else
    {
        QColor bk_color(158,234,106);
        painter.setBrush(QBrush(bk_color));
        //画气泡
        QRect bk_rect = QRect(0, 0, this->width()-WIDTH_TRIANGLE, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        //画三角
        QPointF points[3] = {
            QPointF(bk_rect.x()+bk_rect.width(), 12),
            QPointF(bk_rect.x()+bk_rect.width(), 12+WIDTH_TRIANGLE +2),
            QPointF(bk_rect.x()+bk_rect.width()+WIDTH_TRIANGLE, 10+WIDTH_TRIANGLE-WIDTH_TRIANGLE/2),
        };
        painter.drawPolygon(points, 3);
    }
    return QFrame::paintEvent(e);
}
