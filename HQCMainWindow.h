#ifndef HQCCMAINWINDOW_H
#define HQCCMAINWINDOW_H

#include <QMainWindow>

#include <QThread>
#include <QHostAddress>
#include "HQCWebSocketServer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HQCCMainWindow; }
QT_END_NAMESPACE

class HQCCMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    HQCCMainWindow(QWidget *parent = nullptr);
    ~HQCCMainWindow();

private:
    Ui::HQCCMainWindow *ui;
signals:
    void signalListen(const QString& ip,quint16 port);
    void signalSendTextMessage(int number,const QString& message);
private slots:
    void slotReceivedMsgFromClient(int number,const QString& message);
    void on_btnListen_clicked();
    void on_btnSend0_clicked();
    void on_btnSend1_clicked();
    void on_btnSend2_clicked();
private:
    HQCCWebSocketServer *p_WebServer = nullptr;
    QThread *p_WebServerThread = nullptr;
};
#endif // HQCCMAINWINDOW_H
