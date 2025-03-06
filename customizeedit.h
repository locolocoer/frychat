#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H
#include <QString>
#include <QLineEdit>
class CustomizeEdit:public QLineEdit
{
    Q_OBJECT
public:
    CustomizeEdit();
    CustomizeEdit(QWidget *parent);
    void setMaxLength(int maxlen);
private:
    void limitMaxLength(QString text);
    int _maxlen;
    void focusOutEvent(QFocusEvent *) override;
    void setUp();
signals:
    void sig_focus_out();
    void sig_clear_text();
};

#endif // CUSTOMIZEEDIT_H
