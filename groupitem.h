#ifndef GROUPITEM_H
#define GROUPITEM_H

#include "listitembase.h"

#include <QWidget>

namespace Ui {
class GroupItem;
}

class GroupItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit GroupItem(QWidget *parent = nullptr);
    ~GroupItem();
    void SetGroupTip(QString tip);
private:
    Ui::GroupItem *ui;
};

#endif // GROUPITEM_H
