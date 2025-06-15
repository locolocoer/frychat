#include "findfaildlg.h"
#include "ui_findfaildlg.h"

FindFailDlg::FindFailDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindFailDlg)
{
    ui->setupUi(this);
}

FindFailDlg::~FindFailDlg()
{
    delete ui;
}

void FindFailDlg::on_btnFailSure_clicked()
{

}

