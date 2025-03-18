#include "usermgr.h"

void UserMgr::SetUid(QString uid)
{
    _uid = uid;
}

void UserMgr::setName(QString name)
{
    _name = name;
}

void UserMgr::setToken(QString token)
{
    _token = token;
}

QString UserMgr::getName()
{
    return _name;
}

QString UserMgr::getUid()
{
    return _uid;
}

UserMgr::UserMgr() {}
