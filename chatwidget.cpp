#include "chatwidget.h"
#include "ui_chatwidget.h"
#include <QAction>
ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    ui->addBtn->setState("normal","hover","press");
    ui->searchEdit->setMaxLength(15);


}

ChatWidget::~ChatWidget()
{
    delete ui;
}
