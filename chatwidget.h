#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();
    void showSearch(bool bsearch);
    void addChatUserList();
private:
    Ui::ChatWidget *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
public slots:
    void slot_loading_user();
};

#endif // CHATWIDGET_H
