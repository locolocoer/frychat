#ifndef USERMGR_H
#define USERMGR_H
#include "singleton.h"
#include <memory.h>
class UserMgr:public Singleton<UserMgr>,
    public std::enable_shared_from_this<UserMgr>
{
    friend class Singleton<UserMgr>;
public:
    ~UserMgr()=default;
    void SetUid(QString uid);
    void setName(QString name);
    void setToken(QString token);
    QString getName();
    QString getUid();
private:
    UserMgr();
    QString _uid,_name,_token;
};

#endif // USERMGR_H
