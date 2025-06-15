#include "tcpmgr.h"
#include "global.h"
#include "usermgr.h"
tcpMgr::tcpMgr(QObject *parent)
    : QObject{parent},_host(""),_port(0),_message_len(0),_recv_pending(false)
{
    connect(&_socket,&QTcpSocket::connected,this,[&](){
        qDebug()<<"connect sucess";
        emit sig_con_sucess(true);
    });
    connect(&_socket,&QTcpSocket::readyRead,&_socket,[&](){
        _buffer.append(_socket.readAll());
        QDataStream stream(&_buffer,QIODeviceBase::ReadOnly);
        stream.setVersion(QDataStream::Qt_6_0);
        forever{
            if(!_recv_pending){
                if(_buffer.size()<static_cast<int>(sizeof(uint16_t))){
                    return;//数据不够，继续接收。
                }
                stream>>_message_id>>_message_len;
                _buffer = _buffer.mid(sizeof(uint16_t)*2);

                qDebug()<<"message id:"<<_message_id<<" message len: "<<_message_len;
            }
            if(_buffer.size()<_message_len){
                _recv_pending = true;
                return;
            }
            _recv_pending = false;
            QByteArray message = _buffer.mid(0,_message_len);
            QString msgStr = QString::fromUtf8(message);
            _handlers[_message_id](_message_id,message);
            qDebug()<<"receive message is "<< message;
            _buffer = _buffer.mid(_message_len);
        }
    });
    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),&_socket, [&](QAbstractSocket::SocketError socketError) {
              Q_UNUSED(socketError)
              qDebug() << "Error:" << _socket.errorString();
     });
    connect(&_socket,&QTcpSocket::disconnected,[&](){
        qDebug()<<"disconnect to server";
    });
    connect(this,&tcpMgr::sig_send_data,this,&tcpMgr::slot_send_data);
}

void tcpMgr::initHandlers()
{
    _handlers.insert(ReqId::ID_CHAT_LOGIN,[this](ReqId id,QByteArray msg){
        QJsonDocument doc = QJsonDocument::fromJson(msg);
        if(doc.isNull()){
            qDebug()<<"conver to jsondoc failed";
            return;
        }
        QJsonObject json = doc.object();
        if(!json.contains("error")){
            qDebug()<<"error ";
            emit sig_login_failed(ErrorCodes::ERR_JSON);
            return;
        }
        ErrorCodes err = (ErrorCodes)json["error"].toInt();
        if(err!=ErrorCodes::SUCESS){
            qDebug()<<"error code:"<<err;
            emit sig_login_failed(err);
            return;
        }
        UserMgr::GetInstance()->setUid(json["uid"].toString());
        UserMgr::GetInstance()->setName(json["name"].toString());
        UserMgr::GetInstance()->setToken(json["token"].toString());
        emit sig_login_sucess();
    });
    _handlers.insert(ReqId::ID_SEARCH_USR_RSP,[this](ReqId id,QByteArray msg){
        QJsonDocument doc = QJsonDocument::fromJson(msg);
        if(doc.isNull()){
            qDebug()<<"conver to jsondoc failed";
            return;
        }
        QJsonObject json = doc.object();
        if(!json.contains("error")){
            qDebug()<<"error ";
            return;
        }
        int err = json["error"].toInt();
        if(err!=ErrorCodes::SUCESS){
            qDebug()<<"error code:"<<err;
            return;
        }
        int uid = json["uid"].toInt();
        QString name = json["name"].toString();
        QString nick = json["nick"].toString();
        QString desc = json["desc"].toString();
        int sex = json["sex"].toInt();
        emit sig_user_search(std::make_shared<SearchInfo>(uid,name,nick,desc,sex));
    });
}

void tcpMgr::slot_connect_tcp(ServerInfo si)
{
    qDebug()<<"Receive signal, connect to server!";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toInt());
    _socket.connectToHost(_host,_port);
}

void tcpMgr::slot_send_data(ReqId reqId, QByteArray data)
{
    uint16_t id = reqId;
    uint16_t len = data.size();
    QByteArray block;
    QDataStream stream(&block,QIODeviceBase::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream<<id<<len;
    block.append(data);
    _socket.write(block);
}
