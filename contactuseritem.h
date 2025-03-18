#ifndef CONTACTUSERITEM_H
#define CONTACTUSERITEM_H

#include <QWidget>

namespace Ui {
class ContactUserItem;
}

class ContactUserItem : public QWidget
{
    Q_OBJECT

public:
    explicit ContactUserItem(QWidget *parent = nullptr);
    ~ContactUserItem();

private:
    Ui::ContactUserItem *ui;
};

#endif // CONTACTUSERITEM_H
