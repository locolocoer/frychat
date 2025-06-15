#include "usermgr.h"

void UserMgr::setUid(QString uid)
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

std::vector<std::shared_ptr<ApplyInfo> > UserMgr::GetApplyList()
{
    return _apply_list;
}

UserMgr::UserMgr() {}
