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
            checkLogin(in);          //登录
            break;
        case 2:
            sendFriend();            //获取好友信息
            break;
        case 3:
            sendSoils();             //获取土地信息
            break;
        case 4:
            sendGoods(in);           //获取物品信息
            break;
        case 5:
            sendBusinessResult(in);   //根据客户端发送的交易请求返回交易结果
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
    out << (qint64)outBlock.size() << 2 << temp.size();
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
    out << qint64(0) << 4 << 0 << 0;//长度，类型，business，条数
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
            out << (int)Seed << query.value(0).toInt() << query.value(1).toInt() << query.value(2).toInt();
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
            goodNum++;
        }
    }
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 4 << business << goodNum;
    tcpServerConnection->write(outBlock);
}

void Thread::sendBusinessResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    int business, type, kind, businessNum;
    in >> business >> type >> kind >> businessNum;
    out << qint64(0) << 5 << 0 << business;//长度，类型，结果, business
    if(businessNum <= 0)
    {
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        tcpServerConnection->write(outBlock);
        return;
    }
    if(business == Buy)
    {
        query.prepare("select buyprice from plant where id = ? and kind = ?");
        query.addBindValue(type);
        query.addBindValue(kind);
        query.exec();
        if(!query.next())
        {
            out.device()->seek(0);
            out << (qint64)outBlock.size() << 5 << 1;
            tcpServerConnection->write(outBlock);
            return;
        }
        int buyprice = query.value(0).toInt();
        query.exec(QString("select money from user where id = %1").arg(id));
        int money = query.value(0).toInt();
        if(money < buyprice * businessNum)
        {
            out.device()->seek(0);
            out << (qint64)outBlock.size() << 5 << 2;
            tcpServerConnection->write(outBlock);
            return;
        }
        money -= buyprice * businessNum;
        query.exec(QString("update user set money = %1 where id =%2").arg(money).arg(id));
        query.prepare("update store set num = num + ? where id = ? and type = ? and kind = ?");
    }
    else if(business == Sell || business == Use)
    {
        query.prepare("select num from store where id = ? and type = ? and kind = ?");
        query.addBindValue(id);
        query.addBindValue(type);
        query.addBindValue(kind);
        query.exec();
        if(!query.next())
        {
            out.device()->seek(0);
            out << (qint64)outBlock.size() << 5 << 1;
            tcpServerConnection->write(outBlock);
            return;
        }
        int currentNum = query.value(0).toInt();
        if(currentNum < businessNum)
        {
            out.device()->seek(0);
            out << (qint64)outBlock.size() << 5 << 2;
            tcpServerConnection->write(outBlock);
            return;
        }
        if(business == Sell)
        {
            int businessPrice;
            if(type == Seed)
            {
                query.exec(QString("select buyprice from plant where id = %1").arg(kind));
                businessPrice = query.value(0).toInt() * 0.8;
            }
            else if(type == Fruit)
            {
                query.exec(QString("select sellprice from plant where id = %1").arg(kind));
                businessPrice = query.value(0).toInt();
            }
            query.exec(QString("update user set money = money + %1 where id = %2").arg(businessPrice * businessNum).arg(id));
        }
        query.prepare("update store set num = num - ? where id = ? and type = ? and kind = ?");
    }
    query.addBindValue(businessNum);
    query.addBindValue(id);
    query.addBindValue(type);
    query.addBindValue(kind);
    query.exec();
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 5 << 3;
    tcpServerConnection->write(outBlock);
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

