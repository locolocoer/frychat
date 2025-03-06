#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

class ChatView:public QWidget
{
    Q_OBJECT
public:
    ChatView(QWidget* parent=nullptr);
    void appendChatItem(QWidget* item);
    void prependChatItem(QWidget* item);
    void insertChatItem(QWidget* before,QWidget* item);
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
private slots:
    void onVScrollBarMoved(int min,int max);
private:
    QVBoxLayout* _vblay;
    QScrollArea* m_pScrollArea;
    bool isAppended;
};

#endif // CHATVIEW_H
