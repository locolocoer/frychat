#ifndef USERMGR_H
#define USERMGR_H
#include "singleton.h"
#include "userdata.h"
#include <memory.h>
#include <vector>
class UserMgr:public Singleton<UserMgr>,
    public std::enable_shared_from_this<UserMgr>
{
    friend class Singleton<UserMgr>;
public:
    ~UserMgr()=default;
    void setUid(QString uid);
    void setName(QString name);
    void setToken(QString token);
    QString getName();
    QString getUid();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
private:
    UserMgr();
    QString _uid,_name,_token;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
};

#endif // USERMGR_H
