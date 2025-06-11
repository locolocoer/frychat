#ifndef USERDATA_H
#define USERDATA_H

#include <QString>

class SearchInfo{
public:
    SearchInfo(int uid,QString name,QString nick,QString desc,int sex);
    int _uid;
    QString _name;
    QString _nick;
    QString _desc;
    int _sex;
};

class UserData
{
public:
    UserData();
};

class ApplyInfo{
public:
    ApplyInfo(QString icon,QString name,QString desc,QString nick,int uid,int sex,int status);
    void setIcon(QString iocn);
    QString _icon,_name,_desc,_nick;
    int _uid;
    int _sex;
    int _status;
};

class AddFriendApply{
public:
    AddFriendApply(int from_uid,QString name,QString desc);
    QString _name,_desc;
    int _from_uid;
};
class AuthInfo{
public:
    AuthInfo(int uid,QString name,QString nick,QString icon,int sex);
    int _uid;
    QString _name,_nick,_icon;
    int _sex;
};

class AuthRsp{
public:
    AuthRsp(int peer_uid,QString peer_name,QString peer_nick,QString peer_icon,int sex);
    int _uid;
    QString _name,_nick,_icon;
    int _sex;
};

class UserInfo{
public:
    UserInfo(int uid,QString name,QString nick,QString icon,int sex);
    UserInfo(std::shared_ptr<AuthRsp> auth);
    UserInfo(std::shared_ptr<AuthInfo> info);
    UserInfo(int uid,QString name,QString icon);
    int _uid;
    QString _name,_nick,_icon;
    int _sex;
};

#endif // USERDATA_H
