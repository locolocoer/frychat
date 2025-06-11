#ifndef APPLYFRIENDLIST_H
#define APPLYFRIENDLIST_H
#include <QListWidget>
class ApplyFriendList:public QListWidget
{
    Q_OBJECT
public:
    ApplyFriendList(QWidget* parent=nullptr);
    ~ApplyFriendList();
protected:
    bool eventFilter(QObject* watched,QEvent* event) override;
private slots:

signals:
    void sig_show_search(bool show);
};

#endif // APPLYFRIENDLIST_H
