#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global.h"
#include <QMap>

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();

private:
    Ui::ResetDialog *ui;
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkVarifyValid();
    QMap<TipErr,QString> _tip_msg;
    void AddTipErr(TipErr err,QString msg);
    void DelTipErr(TipErr err);
    void ShowTip(QString msg,bool normal);
    void initHandlers();
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
signals:
    void switchLogin();
private slots:
    void on_returnBtn_clicked();
    void slot_reset_mod_finish(ReqId id,QString res,ErrorCodes err);
    void on_get_code_clicked();
    void on_confirmBtn_clicked();
};

#endif // RESETDIALOG_H
