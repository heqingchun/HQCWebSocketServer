#ifndef HQCCWEBSOCKETSERVER_H
#define HQCCWEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QDebug>

class HQCCWebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit HQCCWebSocketServer(const QString &serverName,QWebSocketServer::SslMode secureMode,const QStringList &clientList,QObject *parent = nullptr);
    ~HQCCWebSocketServer();
signals:
    void signalReceivedMsgFromClient(int number,const QString& message);
public:
    void slotListen(const QString& ip,quint16 port);
    void slotSendTextMessage(int number,const QString& message);
private slots:
    void slotTextMessageReceived(const QString& message);
    void slotTextMessageReceivedFromClient(const QString& message);
    void slotDisconnected();
private:
    QWebSocketServer* p_WebServer = nullptr;
    QWebSocket* p_WebSocket = nullptr;
    QStringList m_ClientList;
    QMap<QWebSocket*,int> m_WebSocketMap;
};

#endif // HQCCWEBSOCKETSERVER_H
