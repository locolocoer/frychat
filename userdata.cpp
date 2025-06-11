#include "userdata.h"

UserData::UserData() {}

SearchInfo::SearchInfo(int uid, QString name, QString nic, QString desc, int sex):_uid(uid),_name(name),_nick(nic),_desc(desc),_sex(sex)
{

}

ApplyInfo::ApplyInfo(QString icon,QString name,QString desc,
                     QString nick,int uid,int sex,int status):
    _icon(icon),_name(name),_desc(desc),_nick(nick),_uid(uid),_sex(sex),_status(status)
{

}

void ApplyInfo::setIcon(QString icon)
{
    _icon = icon;
}

AddFriendApply::AddFriendApply(int from_uid, QString name, QString desc):_from_uid(from_uid),_name(name),_desc(desc)
{

}



AuthRsp::AuthRsp(int peer_uid, QString peer_name, QString peer_nick, QString peer_icon, int sex):
    _uid(peer_uid),_name(peer_name),_nick(peer_nick),_icon(peer_icon),_sex(sex)
{

}

UserInfo::UserInfo(int uid, QString name, QString nick, QString icon, int sex):_uid(uid),_name(name),_nick(nick),_icon(icon),_sex(sex)
{

}

UserInfo::UserInfo(std::shared_ptr<AuthRsp> auth)
{
    _uid = auth->_uid;
    _name = auth->_name;
    _nick = auth->_nick;
    _icon = auth->_icon;
    _sex = auth->_sex;
}

UserInfo::UserInfo(std::shared_ptr<AuthInfo> info)
{
    _uid = info->_uid;
    _name = info->_name;
    _nick = info->_nick;
    _icon = info->_icon;
    _sex = info->_sex;
}

UserInfo::UserInfo(int uid, QString name, QString icon):_uid(uid),_name(name),_nick(""),_icon(icon),_sex(0)
{

}

AuthInfo::AuthInfo(int uid, QString name, QString nick, QString icon, int sex):_uid(uid),_name(name),_nick(nick),_icon(icon),_sex(sex)
{

}
