#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H
#include <QWidget>
#include "global.h"
class ListItemBase:public QWidget
{
    Q_OBJECT
public:
    ListItemBase(QWidget* parent);
    void SetItemType(ListItemType type);
    ListItemType GetItemType();
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    ListItemType _type;
};

#endif // LISTITEMBASE_H
