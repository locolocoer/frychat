#include "resetdialog.h"
#include "ui_resetdialog.h"
#include "httpmgr.h"
ResetDialog::ResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResetDialog)
{
    ui->setupUi(this);
    ui->newpwdEdit->setEchoMode(QLineEdit::Password);
    connect(ui->userEdit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->emailEdit,&QLineEdit::editingFinished,this,[this](){
        checkEmailValid();
    });
    connect(ui->varifyEdit,&QLineEdit::editingFinished,this,[this](){
        checkVarifyValid();
    });
    connect(ui->newpwdEdit,&QLineEdit::editingFinished,this,[this](){
        checkPassValid();
    });
    initHandlers();
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reset_mod_finish,this,&ResetDialog::slot_reset_mod_finish);
}


bool ResetDialog::checkUserValid()
{
    if(ui->userEdit->text()==""){
        AddTipErr(TipErr::TIP_USER_ERR,tr("用户名不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::checkEmailValid()
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

bool ResetDialog::checkPassValid()
{

    auto pass = ui->newpwdEdit->text();
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


bool ResetDialog::checkVarifyValid()
{
    auto pass = ui->varifyEdit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void ResetDialog::AddTipErr(TipErr err, QString msg)
{
    _tip_msg[err]=msg;
    ShowTip(msg,false);
}

void ResetDialog::DelTipErr(TipErr err)
{
    _tip_msg.remove(err);
    if(!_tip_msg.empty()){
        ShowTip(_tip_msg.first(),false);
        return;
    }
    ui->err_tip->clear();
}

void ResetDialog::ShowTip(QString msg, bool normal)
{
    if(!normal){
        ui->err_tip->setProperty("state","err");
    }else{
        ui->err_tip->setProperty("state","normal");
    }
    ui->err_tip->setText(msg);
    repolish(ui->err_tip);
}

void ResetDialog::initHandlers()
{
    //注册
    _handlers.insert(ReqId::ID_GET_VERFIY_CODE,[this](const QJsonObject & jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCESS){
            ShowTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        qDebug()<<"email is "<<email;
        ShowTip(tr("验证码已发送到邮箱，注意查收"),true);
        return;
    });
    _handlers.insert(ReqId::ID_RESET_PWD,[this](const QJsonObject & jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCESS){
            ShowTip(tr("重置失败"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        ShowTip(tr("重置成功，点击返回登陆"),true);
        qDebug()<<jsonObj;
        qDebug()<<"user uid is"<<jsonObj["uid"].toString();
        qDebug()<<"email is"<<email;

    });
}

ResetDialog::~ResetDialog()
{
    delete ui;
}

void ResetDialog::on_returnBtn_clicked()
{
    emit switchLogin();
}

void ResetDialog::slot_reset_mod_finish(ReqId id,QString res,ErrorCodes err)
{
    if(err != ErrorCodes::SUCESS){
        ShowTip(tr("网络请求错误"),false);
        return;
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        ShowTip(tr("json解析失败"),false);
        return;
    }
    if(!jsonDoc.isObject()){
        ShowTip(tr("json解析失败"),false);
        return;
    }
    _handlers[id](jsonDoc.object());
    return;
}


void ResetDialog::on_get_code_clicked()
{
    auto email = ui->emailEdit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        //发送验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),json_obj,ReqId::ID_GET_VERFIY_CODE,Modules::RESETMOD);
    }else{
        ShowTip(tr("邮箱地址不正确"),false);
    }
}


void ResetDialog::on_confirmBtn_clicked()
{
    bool valid = checkUserValid();
    if(!valid){
        return;
    }
    valid = checkEmailValid();
    if(!valid){
        return;
    }
    valid = checkPassValid();
    if(!valid){
        return;
    }
    valid = checkVarifyValid();
    if(!valid){
        return;
    }

    QJsonObject json_obj;
    json_obj["user"] = ui->userEdit->text();
    json_obj["email"] = ui->emailEdit->text();
    json_obj["passwd"] = xorstring(ui->newpwdEdit->text());
    json_obj["varifycode"] = ui->varifyEdit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/reset_pwd"),
                                        json_obj, ReqId::ID_RESET_PWD,Modules::RESETMOD);
}

