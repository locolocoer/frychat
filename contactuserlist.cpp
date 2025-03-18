#include "contactuserlist.h"


ContactUserList::ContactUserList(QWidget *parent):QListWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->viewport()->installEventFilter(this);
    AddContactUserList();
    connect(this,&QListWidget::itemClicked,this,&ContactUserList::slot_item_clicked);
}

void ContactUserList::ShowRedpoint(bool bshow)
{

}
