#ifndef TCPMGR_H
#define TCPMGR_H

#include <QObject>
#include "singleton.h"
#include "global.h"
#include <QTcpSocket>
#include <functional>
#include "userdata.h"

class tcpMgr : public QObject,public Singleton<tcpMgr>
{
    Q_OBJECT
public:
    explicit tcpMgr(QObject *parent = nullptr);
private:
    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    uint16_t _message_id;
    uint16_t _message_len;
    bool _recv_pending;
    QByteArray _buffer;
    QMap<ReqId,std::function<void(ReqId,QString)>> _handlers;
    void initHandlers();
public slots:
    void slot_connect_tcp(ServerInfo si);
    void slot_send_data(ReqId id, QByteArray data);
signals:
    void sig_con_sucess(bool flag);
    void sig_send_data(ReqId id,QByteArray data);
    void sig_switch_chat();
    void sig_user_search(std::shared_ptr<SearchInfo> si);
    void sig_auth_rsp(std::shared_ptr<AuthRsp> rsp);
    void sig_login_failed(ErrorCodes ec);
    void sig_login_sucess();
};

#endif // TCPMGR_H
