#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H
#include "contactuseritem.h"

#include <QListWidget>
class ContactUserItem;
class ContactUserList:public QListWidget
{
    Q_OBJECT
public:
    ContactUserList(QWidget *parent=nullptr);
    void ShowRedpoint(bool bshow);
protected:
    bool eventFilter(QObject* watched,QEvent* event)override;
private:
    void AddContactUserList();
    ContactUserItem* _add_friend_item;
    QListWidgetItem* _group_item;
public slots:
    void slot_item_clicked(QListWidgetItem *item);
signals:
    void sig_loding_contact_user();
    void sig_switch_appyfriend_page();
    void sig_switch_friendinfo_page();
};

#endif // CONTACTUSERLIST_H
