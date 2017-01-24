#include "thread.h"
#include <QTcpSocket>
#include <QDataStream>

Thread::Thread(QTcpSocket *tcpServerConnection) : tcpServerConnection(tcpServerConnection)
{

}

Thread::~Thread()
{

}

void Thread::run()
{
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError()));
    connect(tcpServerConnection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    exec();
}

void Thread::readyRead()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_5);
    int type;
    in >> type;
    switch (type) {
    case 1:
       checkLogin(in);
        break;
    default:
        break;
    }
}

void Thread::checkLogin(QDataStream &in)
{
    in >> username >> password;
    query.prepare("select * from user where username=? and password=?");
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    if(!query.next())
    {
        username.clear();
        out << false << 0;
    }
    else if(query.value(7).toBool() == true)
    {
        username.clear();
        out << false << 1;
    }
    else
    {
        out << true << query.value(0).toInt() << query.value(3).toString() << query.value(4).toInt()
            << query.value(5).toInt() << query.value(6).toInt();
        query.prepare("update user set isonline=1 where username=?");
        query.addBindValue(username);
        query.exec();
    }
    tcpServerConnection->write(block);
    block.resize(0);
}

void Thread::displayError()
{
    qDebug() << tcpServerConnection->errorString();
}

void Thread::disconnected()
{
    if(!username.isNull())
    {
        query.prepare("update user set isonline=0 where username=?");
        query.addBindValue(username);
        query.exec();
    }
    delete this;
}

