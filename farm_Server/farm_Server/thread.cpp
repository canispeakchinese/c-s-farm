#include "thread.h"
#include <QTcpSocket>
#include <QDataStream>

Thread::Thread(QTcpSocket *tcpServerConnection) : tcpServerConnection(tcpServerConnection)
{
    totalBytes = 0;
    inBlock.resize(0);
    outBlock.resize(0);
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
    if(!tcpServerConnection->bytesAvailable())
        return;
    if(totalBytes == 0)
    {
        QDataStream in(tcpServerConnection);
        in.setVersion(QDataStream::Qt_5_5);
        in >> totalBytes;
    }
    inBlock.append(tcpServerConnection->readAll());
    if(inBlock.size() == totalBytes)
    {
        QDataStream in(&inBlock, QIODevice::ReadWrite);
        totalBytes = 0;
        int type;
        in >> type;
        switch (type) {
        case 1:
            checkLogin(in);
            break;
        default:
            break;
        }
        inBlock.resize(0);
    }
}

void Thread::checkLogin(QDataStream &in)
{
    in >> username >> password;
    query.prepare("select * from user where username=? and password=?");
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    if(!query.next())
    {
        username.clear();
        out << qint64(0) << false << 0;
        out.device()->seek(0);
        out << (qint64)(outBlock.size() - sizeof(qint64));
    }
    else if(query.value(7).toBool() == true)
    {
        username.clear();
        out << qint64(0) << false << 1;
        out.device()->seek(0);
        out << (qint64)(outBlock.size() - sizeof(qint64));
    }
    else
    {
        out << qint64(0) << true << query.value(0).toInt() << query.value(3).toString() << query.value(4).toInt()
            << query.value(5).toInt() << query.value(6).toInt();
        out.device()->seek(0);
        out << (qint64)(outBlock.size() - sizeof(qint64));
        query.prepare("update user set isonline=1 where username=?");
        query.addBindValue(username);
        query.exec();
    }
    tcpServerConnection->write(outBlock);
    outBlock.resize(0);
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

