#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"
#include <QTimer>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:

    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_get_code_clicked();
    void slot_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
    void on_sure_btn_clicked();

    void on_pushButton_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::RegisterDialog *ui;
    void showTip(QString str,bool b_ok);
    void initHttpHandlers();
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr,QString> _tip_errs;
    void AddTipErr(TipErr err,QString tip);
    void DelTipErr(TipErr err);
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();
    QTimer* _count_down;
    int _count;
    void changeTipPage();
signals:
    void sig_return_login();
};

#endif // REGISTERDIALOG_H
