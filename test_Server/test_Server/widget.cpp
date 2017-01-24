#include "widget.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(400,300);
    count = 0;
    tcpServer = new QTcpServer;
    retran = new QPushButton(this);
    retran->setGeometry(200, 200, 100, 30);
    retran->setText("重传");
    tcpServer->listen(QHostAddress("127.0.0.1"), 6666);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(retran, SIGNAL(clicked(bool)), this, SLOT(reTran()));
}

void Widget::newConnection()
{
    tcpServerConnection = tcpServer->nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug() << tcpServerConnection;
    QDataStream outStream(&outBlock, QIODevice::ReadWrite);
    outStream.setVersion(QDataStream::Qt_5_5);
    for(int i=0; i<100; i++)
        outStream << count++;
    qDebug() << outBlock.size();
    tcpServerConnection->write(outBlock);
    outBlock.resize(0);
}

void Widget::timeOut()
{
    qDebug() << tcpServerConnection;
}

void Widget::reTran()
{
    QDataStream outStream(&outBlock, QIODevice::ReadWrite);
    outStream.setVersion(QDataStream::Qt_5_5);
    for(int i=0; i<100; i++)
        outStream << count++;
    qDebug() << outBlock.size();
    tcpServerConnection->write(outBlock);
    outBlock.resize(0);
}

void Widget::readyRead()
{
    QDataStream inStream(tcpServerConnection);
    char * aim;
    inStream >> aim;
    qDebug() << aim;
}

Widget::~Widget()
{

}
