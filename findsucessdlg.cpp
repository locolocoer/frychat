#include "findsucessdlg.h"
#include "ui_findsucessdlg.h"

#include <QDir>
#include "applyfriend.h"

FindSucessDlg::FindSucessDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindSucessDlg),
    _parent(parent)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path+QDir::separator()+"static"+QDir::separator()+"head_1.jpg");

    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->headLab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->headLab->setPixmap(head_pix);
    ui->addFriendBtn->setState("normal","hover","press");
    this->setModal(true);
    show();
    connect(ui->addFriendBtn,&QPushButton::clicked,this,&FindSucessDlg::slot_addFrinedBtn_click);
    setStyleSheet("background-color:white");
}

FindSucessDlg::~FindSucessDlg()
{
    delete ui;
}

void FindSucessDlg::setSearchInfo(std::shared_ptr<SearchInfo> si)
{

    _si = si;
    ui->nameLab->setText(si->_name);
}

void FindSucessDlg::slot_addFrinedBtn_click()
{
    //todo 待完善
    hide();
    auto applyfriendPage = new ApplyFriend(this);
    applyfriendPage->show();
}
