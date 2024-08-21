#include "tcpmgr.h"

tcpMgr::tcpMgr(QObject *parent)
    : QObject{parent},_host(""),_port(0),_message_id(0),_message_len(0),_recv_pending(false)
{
    connect(&_socket,&QTcpSocket::connected,[&](){
        qDebug()<<"connect sucess";
        emit sig_con_sucess(true);
    });
    connect(&_socket,&QTcpSocket::readyRead,[&](){
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
            qDebug()<<"receive message is "<< message;
            _buffer = _buffer.mid(_message_len);
        }
    });
    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
              Q_UNUSED(socketError)
              qDebug() << "Error:" << _socket.errorString();
     });
    connect(&_socket,&QTcpSocket::disconnected,[&](){
        qDebug()<<"disconnect to server";
    });
    connect(this,&tcpMgr::sig_send_data,this,&tcpMgr::slot_send_data);
}

void tcpMgr::slot_connect_tcp(ServerInfo si)
{
    qDebug()<<"Receive signal, connect to server!";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toInt());
    _socket.connectToHost(_host,_port);
}

void tcpMgr::slot_send_data(ReqId reqId, QString data)
{
    uint16_t id = reqId;
    QByteArray data_byte = data.toUtf8();
    uint16_t len = data_byte.size();
    QByteArray block;
    QDataStream stream(&block,QIODeviceBase::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream<<id<<len;
    block.append(data_byte);
    _socket.write(block);
}
