#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"
RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog),_count(5)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reg_mod_finish,this,&RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
    ui->err_tip->clear();
    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_edit,&QLineEdit::editingFinished,this,[this](){
        checkEmailValid();
    });
    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->varify_edit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });
    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);
    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");
    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");
    connect(ui->pass_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->pass_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });
    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });
    _count_down=new QTimer(this);
    connect(_count_down,&QTimer::timeout,this,[this](){
        if(_count ==0){
            _count_down->stop();
            emit sig_return_login();
            return;
        }
        _count--;
        qDebug()<<_count;
        auto str = QString("注册成功，%1秒后放回登陆界面").arg(_count);
        ui->cd_lab->setText(str);
    });

}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_code_clicked()
{
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        //发送验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),json_obj,ReqId::ID_GET_VERFIY_CODE,Modules::REGISTERMOD);
    }else{
        showTip(tr("邮箱地址不正确"),false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id,QString res,ErrorCodes err)
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

void RegisterDialog::showTip(QString str,bool b_ok)
{
    if(b_ok){
      ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

void RegisterDialog::initHttpHandlers()
{
    //注册
    _handlers.insert(ReqId::ID_GET_VERFIY_CODE,[this](const QJsonObject & jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        qDebug()<<"email is "<<email;
        showTip(tr("验证码已发送到邮箱，注意查收"),true);
        return;
    });
    _handlers.insert(ReqId::ID_REG_USER,[this](const QJsonObject & jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCESS){
            showTip(tr("注册失败"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("注册成功"),true);
        qDebug()<<jsonObj;
        qDebug()<<"user uid is"<<jsonObj["uid"].toString();
        qDebug()<<"email is"<<email;
        changeTipPage();
    });
}

void RegisterDialog::AddTipErr(TipErr err, QString tip)
{
    _tip_errs[err]=tip;
    showTip(tip,false);
}

void RegisterDialog::DelTipErr(TipErr err)
{
    _tip_errs.remove(err);
    if(_tip_errs.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(),false);
}

bool RegisterDialog::checkUserValid()
{
    if(ui->user_edit->text()==""){
        AddTipErr(TipErr::TIP_USER_ERR,tr("用户名不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_EMAIL_ERR,tr("邮箱不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPassValid()
{

    auto pass = ui->pass_edit->text();
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

bool RegisterDialog::checkConfirmValid()
{
    auto pass = ui->pass_edit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("不能包含非法字符"));
        return false;
    }
    auto comfirm = ui->confirm_edit->text();
    if(pass!=comfirm){
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("确认密码不一致"));
        return false;
    }
    DelTipErr(TipErr::TIP_CONFIRM_ERR);
    return true;
}

bool RegisterDialog::checkVarifyValid()
{
    auto pass = ui->varify_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::changeTipPage()
{
    _count_down->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    _count = 5;
    _count_down->start(1000);
}


void RegisterDialog::on_sure_btn_clicked()
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
    //day11 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorstring(ui->pass_edit->text());
    json_obj["confirm"] = xorstring(ui->confirm_edit->text());
    json_obj["varifycode"] = ui->varify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}


void RegisterDialog::on_pushButton_clicked()
{
    _count_down->stop();
    emit sig_return_login();
}


void RegisterDialog::on_cancel_btn_clicked()
{
    emit sig_return_login();
}

