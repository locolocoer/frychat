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
    ApplyInfo();
    QString _icon,_name,_desc;
    int _uid;
};

#endif // USERDATA_H
