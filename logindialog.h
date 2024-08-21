#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <global.h>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    void initHead();
    bool checkEmailValid();
    bool checkPassValid();
    void showTip(QString str,bool b_ok);
    void AddTipErr(TipErr err, QString tip);
    void DelTipErr(TipErr err);
    QMap<TipErr,QString> _tip_errs;
    void enableBtn(bool flag);
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
    void initHandlers();
    int _uid;
    QString _token;
    void slot_forget_pwd();
    void on_loginButton_clicked();

signals:
    void sig_connect_tcp(ServerInfo);
    void switchRegister();
    void switchReset();
private slots:
    void slot_login_mod_finish(ReqId id,QString res,ErrorCodes err);
    void slot_tcp_con_finish(bool flag);
};

#endif // LOGINDIALOG_H
