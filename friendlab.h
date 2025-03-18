#ifndef FRIENDLAB_H
#define FRIENDLAB_H

#include <QFrame>

namespace Ui {
class FriendLab;
}

class FriendLab : public QFrame
{
    Q_OBJECT

public:
    explicit FriendLab(QWidget *parent = nullptr);
    ~FriendLab();
    void SetText(QString text);
    int Width();
    int Height();
    QString Text();

private:
    Ui::FriendLab *ui;
    QString _text;
    int _width;
    int _height;

public slots:
    void slot_close();
signals:
    void sig_close(QString);
};

#endif // FRIENDLAB_H
