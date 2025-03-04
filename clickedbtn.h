#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H
#include <QPushButton>

class ClickedBtn:public QPushButton
{
    Q_OBJECT
public:
    void setState(QString normal,QString hover,QString press);
    ClickedBtn()=default;
    ClickedBtn(QWidget* parent);
    ~ClickedBtn();

private:
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    QString _normal;
    QString _hover;
    QString _press;
};

#endif // CLICKEDBTN_H
