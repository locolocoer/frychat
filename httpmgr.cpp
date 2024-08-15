#include "httpmgr.h"



HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    connect(this,&HttpMgr::sig_http_finish,this,&HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length()));
    auto self = shared_from_this();
    QNetworkReply* reply = _manager.post(request,data);
    connect(reply,&QNetworkReply::finished,[self,reply,req_id,mod](){
        //处理错误情况
        if(reply->error()!= QNetworkReply::NoError){
            qDebug()<<reply->errorString();
            emit self->sig_http_finish(req_id,"",ErrorCodes::ERR_NETWORK,mod);
            reply->deleteLater();
            return;
        }
        QString res = reply->readAll();
        emit self->sig_http_finish(req_id,res,ErrorCodes::SUCESS,mod);
        reply->deleteLater();
        return;
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD){
        //发送信号通知指定模块http的响应结束
        emit sig_reg_mod_finish(id,res,err);
    }
    if(mod==Modules::RESETMOD){
        emit sig_reset_mod_finish(id,res,err);
    }
    if(mod==Modules::LOGINMOD){
        emit sig_login_mod_finish(id,res,err);
    }
}
