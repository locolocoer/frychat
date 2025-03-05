#include "customizeedit.h"



CustomizeEdit::CustomizeEdit():_maxlen(0)
{
    connect(this,&CustomizeEdit::textChanged,this,&CustomizeEdit::limitMaxLength);
    setUp();
}

CustomizeEdit::CustomizeEdit(QWidget *parent):_maxlen(0),QLineEdit(parent)
{
    connect(this,&CustomizeEdit::textChanged,this,&CustomizeEdit::limitMaxLength);
    setUp();
}

void CustomizeEdit::setMaxLength(int maxlen)
{
    _maxlen = maxlen;
}

void CustomizeEdit::limitMaxLength(QString text)
{
    if(_maxlen<=0) return;
    QByteArray byteArray = text.toUtf8();
    if(byteArray.size()>=_maxlen){
        byteArray = byteArray.left(_maxlen);
        setText(QString::fromUtf8(byteArray));
    }
}

void CustomizeEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    emit sig_focus_out();
}

void CustomizeEdit::setUp()
{
    QAction* searchAction = new QAction(this);
    searchAction->setIcon(QIcon(":/images/search.png"));
    this->addAction(searchAction,QLineEdit::LeadingPosition);
    this->setPlaceholderText("搜索");
    QAction* clearAction = new QAction(this);
    clearAction->setIcon(QIcon(":/images/close_transparent.png"));
    this->addAction(clearAction,QLineEdit::TrailingPosition);
    connect(this,&CustomizeEdit::textChanged,[clearAction](const QString& text){
        if(!text.isEmpty()){
            clearAction->setIcon(QIcon(":/images/close_search.png"));
        }else{
            clearAction->setIcon(QIcon(":/images/close_transparent.png"));
        }
    });
}
