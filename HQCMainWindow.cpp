#include "HQCMainWindow.h"
#include "ui_HQCMainWindow.h"

HQCCMainWindow::HQCCMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HQCCMainWindow)
{
    ui->setupUi(this);
    QStringList tmpWebClientList;
    tmpWebClientList<<"WebClient0"<<"WebClient1"<<"WebClient2";
    p_WebServer = new HQCCWebSocketServer("WebServer",QWebSocketServer::SslMode::NonSecureMode,tmpWebClientList);
    p_WebServerThread = new QThread();
    p_WebServer->moveToThread(p_WebServerThread);
    connect(this,&HQCCMainWindow::signalListen,p_WebServer,&HQCCWebSocketServer::slotListen);
    connect(p_WebServer,&HQCCWebSocketServer::signalReceivedMsgFromClient,this,&HQCCMainWindow::slotReceivedMsgFromClient);
    connect(this,&HQCCMainWindow::signalSendTextMessage,p_WebServer,&HQCCWebSocketServer::slotSendTextMessage);
    connect(p_WebServerThread,&QThread::finished,p_WebServerThread,&QThread::deleteLater);
    p_WebServerThread->start();
}

HQCCMainWindow::~HQCCMainWindow(){
    if(p_WebServerThread){
        p_WebServerThread->quit();
        p_WebServerThread->wait();
    }
    if(p_WebServer){
        p_WebServer->deleteLater();
    }
    delete ui;
}

void HQCCMainWindow::slotReceivedMsgFromClient(int number,const QString& message){
    switch (number) {
    case 0:
        ui->textEdit0->append(message);
        break;
    case 1:
        ui->textEdit1->append(message);
        break;
    case 2:
        ui->textEdit2->append(message);
        break;
    default:
        break;
    }
}

void HQCCMainWindow::on_btnListen_clicked(){
    emit signalListen(ui->lineEditIp->text(),ui->lineEditPort->text().toInt());
}

void HQCCMainWindow::on_btnSend0_clicked(){
    emit signalSendTextMessage(0,ui->lineEdit0->text());
}

void HQCCMainWindow::on_btnSend1_clicked(){
    emit signalSendTextMessage(1,ui->lineEdit1->text());
}

void HQCCMainWindow::on_btnSend2_clicked(){
    emit signalSendTextMessage(2,ui->lineEdit2->text());
}
