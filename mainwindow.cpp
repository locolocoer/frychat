#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    //_login_dlg->show();
    //创建注册消息连接
    connect(_login_dlg,&LoginDialog::switchRegister,this,&MainWindow::SlotSwitchReg);
}

MainWindow::~MainWindow()
{
    delete ui;
    // if(_login_dlg){
    //     delete _login_dlg;
    //     _login_dlg = nullptr;
    // }
    // if(_register_dlg){
    //     delete _register_dlg;
    //     _register_dlg = nullptr;
    // }
}

void MainWindow::SlotSwitchReg()
{
    _register_dlg = new RegisterDialog(this);
    _register_dlg->hide();
    _register_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_register_dlg);
    _login_dlg->hide();
    _register_dlg->show();
    connect(_register_dlg,&RegisterDialog::sig_return_login,this,&MainWindow::SlotReturnLogin);
}

void MainWindow::SlotReturnLogin()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dlg);
    _register_dlg->hide();
    _login_dlg->show();
    //连接登录界面注册信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}
