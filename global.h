#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include "QStyle"
#include <QRegularExpression>
#include <memory>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QSettings>
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> xorstring;
enum ReqId{
    ID_GET_VERFIY_CODE = 1001,
    ID_REG_USER = 1002,
    ID_RESET_PWD = 1003,
    ID_LOGIN_USER = 1004,
};

enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};

enum ErrorCodes {
    SUCESS = 0,
    ERR_JSON = 1,
    ERR_NETWORK = 2,
    RPCFailed = 3,
    VarifyExpired = 4,
    VarifyError = 5,
    UserExist = 6,
    EmailNotMatch = 7,
    PasswdUpateFailed = 8,
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ClickLbState{
    Normal = 0,
    Selected = 1,
};

extern QString gate_url_prefix;
extern QString head_pic;

struct ServerInfo{
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

#endif // GLOBAL_H
