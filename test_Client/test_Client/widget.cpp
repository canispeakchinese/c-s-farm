#include "widget.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QPushButton>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(400, 300);
    count = 0;
    tcpClient = new QTcpSocket;
    start = new QPushButton(this);
    start->setGeometry(100, 130, 80, 40);
    start->setText("开始连接");
    over = new QPushButton(this);
    over->setGeometry(220, 130, 80, 40);
    over->setText("断开连接");
    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpConnected()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError()));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(start, SIGNAL(clicked(bool)), this, SLOT(tryConnection()));
    connect(over, SIGNAL(clicked(bool)), this, SLOT(tryDisconnection()));
}

void Widget::tryConnection()
{
    tcpClient->connectToHost(QHostAddress("127.0.0.1"), 6666);
}

void Widget::tryDisconnection()
{
    QDataStream outStream(&outBlock, QIODevice::ReadWrite);
    outStream << "hello world";
    tcpClient->write(outBlock);
    outBlock.resize(0);
    tcpClient->disconnectFromHost();
    //tcpClient->abort();
}

void Widget::tcpConnected()
{
    qDebug() << tcpClient;
    qDebug() << QTcpSocket::ConnectedState;
}

void Widget::tcpError()
{
    qDebug() << tcpClient->errorString();
}

void Widget::readyRead()
{
    //if(!tcpClient->bytesAvailable())
      //  return;
    int count2;
    QDataStream inStream(tcpClient);
    inStream.setVersion(QDataStream::Qt_5_5);
    while(!inStream.atEnd())
    {
        inStream >> count2;
        qDebug() << count << count2;
    }
    count++;
}

Widget::~Widget()
{

}
