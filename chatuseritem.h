#ifndef CHATUSERITEM_H
#define CHATUSERITEM_H

#include <QWidget>
#include "listitembase.h"

namespace Ui {
class ChatUserItem;
}

class ChatUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserItem(QWidget *parent = nullptr);
    ~ChatUserItem();
    void SetInfo(QString name,QString head,QString msg);

private:
    Ui::ChatUserItem *ui;
    QString _name;
    QString _head;
    QString _msg;
};

#endif // CHATUSERITEM_H
