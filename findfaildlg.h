#ifndef FINDFAILDLG_H
#define FINDFAILDLG_H

#include <QDialog>

namespace Ui {
class FindFailDlg;
}

class FindFailDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FindFailDlg(QWidget *parent = nullptr);
    ~FindFailDlg();

private slots:
    void on_btnFailSure_clicked();

private:
    Ui::FindFailDlg *ui;
};

#endif // FINDFAILDLG_H
