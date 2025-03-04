#include "chatwidget.h"
#include "ui_chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    ui->addBtn->setState("normal","hover","press");
}

ChatWidget::~ChatWidget()
{
    delete ui;
}
