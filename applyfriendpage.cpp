#include "applyfriendpage.h"
#include "ui_applyfriendpage.h"

ApplyFriendPage::ApplyFriendPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}
