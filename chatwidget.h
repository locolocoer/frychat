#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include "global.h"
#include "statewidget.h"

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
    void addLbGroup(StateWidget* lb);
    QList<StateWidget*> _lb_group;
    void clearOtherSelectState(StateWidget* lb);
    void handleGlobalMouseEvent(QMouseEvent* ev);
protected:
    bool eventFilter(QObject* watched,QEvent* event) override;
    void paintEvent(QPaintEvent *e) override;
public slots:
    void slot_loading_user();
    void slot_chatLab_click();
    void slot_contactLab_click();
    void slot_text_changed(const QString &text);
};

#endif // CHATWIDGET_H
