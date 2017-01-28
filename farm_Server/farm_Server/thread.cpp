#include "thread.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QDateTime>

Thread::Thread(QTcpSocket *tcpServerConnection) : tcpServerConnection(tcpServerConnection)
{
    id = -1;
    totalBytes = 0;
    inBlock.resize(0);
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
    inBlock.append(tcpServerConnection->readAll());
    if(totalBytes == 0 && inBlock.size() >= (int)sizeof(qint64))
    {
        QDataStream in(&inBlock, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_5_5);
        in >> totalBytes;
        in >> messageType;
        inBlock = in.device()->readAll();
    }
    if(inBlock.size() >= totalBytes - 12)
    {
        QDataStream in(&inBlock, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_5_5);
        if(messageType > 1)
        {
            int _id;
            QString _password;
            in >> _id >> _password;
            if(id != _id || _password != password)
            {
                //error;
                exit(0);
            }
        }
        switch (messageType) {
        case 1:
            checkLogin(in);
            break;
        case 2:
            sendFriend();
            break;
        case 3:
            sendSoils();
            break;
        case 4:
            sendGoods(in);
            break;
        default:
            break;
        }
        totalBytes = 0;
        inBlock = in.device()->readAll();
        if(inBlock.size() > (int)sizeof(qint64))
            readyRead();
    }
}

void Thread::checkLogin(QDataStream &in)
{
    QString username;
    in >> username >> password;
    query.prepare("select * from user where username=? and password=?");
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 1;
    if(!query.next())
    {
        out << false << 0;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
    }
    else if(query.value(7).toBool() == true)
    {
        out << false << 1;
        out.device()->seek(0);
        out << (qint64)outBlock.size();
    }
    else
    {
        id = query.value(0).toInt();
        out << true << id << query.value(3).toString() << query.value(4).toInt()
            << query.value(5).toInt() << query.value(6).toInt();
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        query.prepare("update user set isonline=1 where username=?");
        query.addBindValue(username);
        query.exec();
    }
    tcpServerConnection->write(outBlock);
}

void Thread::sendFriend()
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 2 << 0;
    query.prepare("select id1 from goodfriend where id2=? union select id2 from goodfriend where id1=?");
    query.addBindValue(id);
    query.addBindValue(id);
    query.exec();
    QVector<int>temp;
    while(query.next())
        temp.append(query.value(0).toInt());
    for(int i=0; i<(int)temp.size(); i++)
    {
        int _id = temp[i];
        query.exec(QString("select username,faceaddress,level,exp,money from user where id=%1").arg(_id));
        query.next();
        out << query.value(0).toString() << query.value(1).toString() << query.value(2).toInt()
            << query.value(3).toInt() << query.value(4).toInt();
    }
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    out << 2 << temp.size();
    tcpServerConnection->write(outBlock);
}

void Thread::sendSoils()
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 3;
    query.prepare("select level, is_recla, kind, plant_time, cal_time, yield, water, pyre, weed, harvest from soil where id=? order by number");
    query.addBindValue(id);
    query.exec();
    while(query.next())
    {
        out << query.value(0).toInt();
        bool is_recla = query.value(1).toBool();
        out << is_recla;
        if(is_recla)
        {
            int kind = query.value(2).toInt();
            out << kind;
            if(kind)
            {
                for(int i=3; i<5; i++)
                    out << query.value(i).toDateTime();
                out << query.value(5).toInt();
                for(int i=6; i<9; i++)
                    out << query.value(i).toDateTime();
                out << query.value(9).toBool();
                QSqlQuery query2;
                query2.exec(QString("select growTime, allSta from plant where id=%1").arg(kind));
                query2.next();
                out << query2.value(0).toInt() << query2.value(1).toInt();
            }
        }
    }
    out.device()->seek(0);
    out << (qint64)sizeof(outBlock);
    tcpServerConnection->write(outBlock);
}

void Thread::sendGoods(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 4 << 0;
    int business, goodNum = 0;
    in >> business;
    if(business == Buy)
    {
        query.exec(QString("select money from user where id = %1").arg(id));
        query.next();
        out << query.value(0).toInt();
        query.exec("select id, buyprice, sellprice from plant");
        while(query.next())
        {
            out << Seed << query.value(0).toInt() << query.value(1).toInt() << query.value(2).toInt();
            goodNum++;
        }
    }
    else if(business == Sell)
    {
        query.exec(QString("select type, kind, buyprice, sellprice, num from store, plant where store.id=%1 ").arg(id)
            +QString("and (type=%1 or type=%2) and store.kind=plant.id").arg(Seed).arg(Fruit));
        while(query.next())
        {
            out << query.value(0).toInt() << query.value(1).toInt() << query.value(2).toInt() << query.value(3).toInt()
                << query.value(4).toInt();
            goodNum++;
        }
    }
    else if(business == Use)
    {
        query.exec(QString("select kind, num from store where id=%1 and type=%2").arg(id).arg(Seed));
        while(query.next())
        {
            out << query.value(0).toInt() << query.value(1).toInt();
        }
    }
}

void Thread::displayError()
{
    qDebug() << tcpServerConnection->errorString();
}

void Thread::disconnected()
{
    if(id != -1)
    {
        query.prepare("update user set isonline=0 where id=?");
        query.addBindValue(id);
        query.exec();
    }
    delete this;
}

