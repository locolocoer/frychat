#include "logindialog.h"
#include "ui_logindialog.h"
#include <QPainter>
#include <QPainterPath>
#include "HttpMgr.h"
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn,&QPushButton::clicked,this,&LoginDialog::switchRegister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");

    connect(ui->forget_label,&ClickedLabel::clicked,this,&LoginDialog::slot_forget_pwd);
    initHead();
    connect(ui->emailEdit,&QLineEdit::editingFinished,this,[this](){
        checkEmailValid();
    });
    connect(ui->passEdit,&QLineEdit::editingFinished,this,[this](){
        checkPassValid();
    });
    initHandlers();
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_login_mod_finish,this,&LoginDialog::slot_login_mod_finish);
    ui->loginButton->setEnabled(true);
    connect(ui->loginButton,&QPushButton::clicked,this,&LoginDialog::on_loginButton_clicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::initHead()
{
    QPixmap originPixmap(":/images/frychat.ico");
    qDebug()<<originPixmap.size()<<ui->head_label->size();
    originPixmap = originPixmap.scaled(ui->head_label->size(),Qt::KeepAspectRatio,Qt::FastTransformation);

    QPixmap roundPixmap(originPixmap.size());
    roundPixmap.fill(Qt::transparent);
    QPainter painter(&roundPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addRoundedRect(0,0,originPixmap.width(),originPixmap.height(),originPixmap.width()/2,originPixmap.height()/2 );
    painter.setClipPath(path);
    painter.drawPixmap(0,0,originPixmap);
    ui->head_label->setPixmap(roundPixmap);
}

void LoginDialog::slot_forget_pwd()
{
    qDebug()<<"slot forget pwd";
    emit switchReset();
}

void LoginDialog::showTip(QString str,bool b_ok)
{
    if(b_ok){
        ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

void LoginDialog::AddTipErr(TipErr err, QString tip)
{
    _tip_errs[err]=tip;
    showTip(tip,false);
}

void LoginDialog::DelTipErr(TipErr err)
{
    _tip_errs.remove(err);
    if(_tip_errs.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(),false);
}

void LoginDialog::enableBtn(bool flag)
{
    ui->loginButton->setEnabled(flag);
    ui->reg_btn->setEnabled(flag);
    return;
}

void LoginDialog::initHandlers()
{
    _handlers.insert(ReqId::ID_LOGIN_USER,[this](const QJsonObject & jsonobj){
        int error = jsonobj["error"].toInt();
        if(error!=ErrorCodes::SUCESS){
            showTip("参数错误",false);
           enableBtn(true);
            return;
        }
        auto email  = jsonobj["email"].toString();
        ServerInfo si;
        si.Uid = jsonobj["uid"].toInt();
        si.Host = jsonobj["host"].toString();
        si.Port = jsonobj["port"].toString();
        si.Token = jsonobj["token"].toString();
        _uid = si.Uid;
        _token = si.Token;
        showTip("登陆成功",true);
        qDebug()<<"email is "<<email<<" uid is "<<si.Uid<<" host is "<<si.Host<<" port is "<< si.Port<<" Token is "<<si.Token;
        emit sig_connect_tcp(si);
    });
}

bool LoginDialog::checkEmailValid()
{
    auto email = ui->emailEdit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_EMAIL_ERR,tr("邮箱不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPassValid()
{

    auto pass = ui->passEdit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}


void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCESS){
        showTip(tr("网络请求错误"),false);
        return;
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        showTip(tr("json解析失败"),false);
        return;
    }
    if(!jsonDoc.isObject()){
        showTip(tr("json解析失败"),false);
        return;
    }
    _handlers[id](jsonDoc.object());
    return;
}


void LoginDialog::on_loginButton_clicked()
{
    qDebug()<<"login btn clicked";
    if(checkEmailValid()==false){
        return;
    }
    if(checkPassValid()==false){
        return;
    }
    enableBtn(false);
    auto email = ui->emailEdit->text();
    auto pwd = ui->passEdit->text();

    QJsonObject  jsonobj;
    jsonobj["email"]=email;
    jsonobj["passwd"]=xorstring(pwd);
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                        jsonobj,ReqId::ID_LOGIN_USER,Modules::LOGINMOD);
}

