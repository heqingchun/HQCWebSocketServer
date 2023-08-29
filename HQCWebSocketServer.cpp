#include "HQCWebSocketServer.h"

HQCCWebSocketServer::HQCCWebSocketServer(const QString &serverName, QWebSocketServer::SslMode secureMode,const QStringList &clientList,QObject *parent) : QObject(parent){
    m_ClientList = clientList;
    p_WebServer = new QWebSocketServer(serverName,secureMode,this);
    connect(p_WebServer,&QWebSocketServer::newConnection,[&](){
        qDebug()<<"WebSocket客户端接入";
        p_WebSocket = p_WebServer->nextPendingConnection();
        connect(p_WebSocket,&QWebSocket::textMessageReceived,this,&HQCCWebSocketServer::slotTextMessageReceived);
    });
}
HQCCWebSocketServer::~HQCCWebSocketServer(){
    if(p_WebServer){
        p_WebServer->deleteLater();
    }
    if(p_WebSocket){
        p_WebSocket->deleteLater();
    }
}
void HQCCWebSocketServer::slotListen(const QString& ip,quint16 port){
    QHostAddress address(ip);
    if(!p_WebServer->isListening()){
        qDebug()<<"WebServer开始监听";
        p_WebServer->listen(address,port);
    }
}
void HQCCWebSocketServer::slotTextMessageReceived(const QString& message){
    int num = m_ClientList.indexOf(message);
    if(num != -1){
        m_WebSocketMap.insert(p_WebSocket,num);
        disconnect(p_WebSocket,&QWebSocket::textMessageReceived,this,&HQCCWebSocketServer::slotTextMessageReceived);
        connect(p_WebSocket,&QWebSocket::textMessageReceived,this,&HQCCWebSocketServer::slotTextMessageReceivedFromClient);
        connect(p_WebSocket,&QWebSocket::disconnected,this,&HQCCWebSocketServer::slotDisconnected);
    }
}
void HQCCWebSocketServer::slotTextMessageReceivedFromClient(const QString& message){
    emit signalReceivedMsgFromClient(m_WebSocketMap.value(qobject_cast<QWebSocket*>(sender())),message);
}
void HQCCWebSocketServer::slotSendTextMessage(int number,const QString& message){
    if(m_WebSocketMap.key(number)){
        m_WebSocketMap.key(number)->sendTextMessage(message);
    }
}
void HQCCWebSocketServer::slotDisconnected(){
    m_WebSocketMap.remove(qobject_cast<QWebSocket*>(sender()));
}
