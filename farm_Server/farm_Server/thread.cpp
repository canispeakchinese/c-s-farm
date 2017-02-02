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
    if(totalBytes == 0 && inBlock.size() >= (int)(sizeof(qint64) + sizeof(int)))
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
        if(messageType > 2)
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
            checkSign(in);           //注册
            break;
        case 3:
            sendUpdateResult(in);    //返回要求的信息
            break;
        case 4:
            sendPlantResult(in);     //返回种植结果
            break;
        case 5:
            sendBusinessResult(in);  //根据客户端发送的交易请求返回交易结果
            break;
        case 6:
            sendSpadResult(in);
            break;
        case 7:
            sendHarvestResult(in);
        default:
            break;
        }
        totalBytes = 0;
        inBlock = in.device()->readAll();
        if(inBlock.size() >= (int)(sizeof(qint64) + sizeof(int)))
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

void Thread::checkSign(QDataStream &in)
{
    QString username;
    in >> username >> password;
    query.prepare("select * from user where username=?");
    query.addBindValue(username);
    query.exec();
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 2;
    if(!query.next())
    {
        query.prepare("insert into user(username, password, faceaddress, level, exp, money, isonline) values(?, ?, 1, 1, 0, 30, 1)");
        query.addBindValue(username);
        query.addBindValue(password);
        query.exec();
        query.prepare("select id from user where username = ? and password = ?");
        query.addBindValue(username);
        query.addBindValue(password);
        query.exec();
        query.next();
        id = query.value(0).toInt();
        for(int i=1; i<=18; i++)
        {
            if(i <= 3)
                query.exec(QString("insert into soil(id, number, level, is_recla) values(%1, %2, 0, 1)").arg(id).arg(i));
            else
                query.exec(QString("insert into soil(id, number, level, is_recla) values(%1, %2, 0, 0)").arg(id).arg(i));
        }
        out << 0 << id;
    }
    else
        out << 1;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpServerConnection->write(outBlock);
}

void Thread::sendUpdateResult(QDataStream &in)
{
    int updateResult;
    in >> updateResult;
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << 3;
    if(updateResult&1)
        outBlock.append(getSoilResult());
    if(updateResult&2)
        outBlock.append(getFriendResult());
    if(updateResult&4)
        outBlock.append(getGoodResult(Buy));
    if(updateResult&8)
        outBlock.append(getGoodResult(Sell));
    if(updateResult&16)
        outBlock.append(getGoodResult(Use));
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpServerConnection->write(outBlock);
}

QByteArray Thread::getSoilResult()
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
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
    return outBlock;
}

QByteArray Thread::getFriendResult()
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << 0;
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
    out << temp.size();
    return outBlock;
}

QByteArray Thread::getGoodResult(Business business)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << 0;//条数
    int goodNum = 0;
    if(business == Buy)
    {
        query.exec(QString("select money from user where id = %1").arg(id));
        query.next();
        out << query.value(0).toInt();
        query.exec("select id from plant");
        while(query.next())
        {
            out << (int)Seed << query.value(0).toInt();
            goodNum++;
        }
    }
    else if(business == Sell)
    {
        query.exec(QString("select type, kind, num from store where id=%1").arg(id));
        while(query.next())
        {
            out << query.value(0).toInt() << query.value(1).toInt() << query.value(2).toInt();
            goodNum++;
        }
    }
    else if(business == Use)
    {
        query.exec(QString("select type, kind, num from store where id=%1 and type=%2").arg(id).arg(Seed));
        while(query.next())
        {
            out << query.value(0).toInt() << query.value(1).toInt() << query.value(2).toInt();
            goodNum++;
        }
    }
    out.device()->seek(0);
    out << goodNum;
    return outBlock;
}

void Thread::sendBusinessResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    int business, type, kind, businessNum;
    in >> business >> type >> kind >> businessNum;
    out << qint64(0) << 5 << 0;//长度，类型，结果, business
    if(businessNum <= 0)
    {
        out.device()->seek(0);
        out << (qint64)outBlock.size();
        tcpServerConnection->write(outBlock);
        return;
    }
    if(business == Buy)
    {
        if(type == Seed)
        {
            query.prepare("select buyprice from plant where id = ?");
            query.addBindValue(kind);
            query.exec();
        }
        if(!query.next())
        {
            out.device()->seek(0);
            out << (qint64)outBlock.size() << 5 << 1;
            tcpServerConnection->write(outBlock);
            return;
        }
        int buyprice = query.value(0).toInt();
        query.exec(QString("select money from user where id = %1").arg(id));
        query.next();
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
        query.prepare("select * from store where id = ? and type = ? and kind =?");
        query.addBindValue(id);
        query.addBindValue(type);
        query.addBindValue(kind);
        query.exec();
        if(query.next())
            query.prepare("update store set num = num + ? where id = ? and type = ? and kind = ?");
        else
            query.prepare("insert into store(num, id, type, kind) values(?, ?, ?, ?)");
        query.addBindValue(businessNum);
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
            int businessPrice = 0;
            if(type == Seed)
            {
                query.exec(QString("select buyprice from plant where id = %1").arg(kind));
                query.next();
                businessPrice = query.value(0).toInt() * 0.8;
            }
            else if(type == Fruit)
            {
                query.exec(QString("select sellprice from plant where id = %1").arg(kind));
                query.next();
                businessPrice = query.value(0).toInt();
            }
            query.exec(QString("update user set money = money + %1 where id = %2").arg(businessPrice * businessNum).arg(id));
        }
        if(currentNum == businessNum)
            query.prepare("delete from store where id = ? and type = ? and kind = ?");
        else
        {
            query.prepare("update store set num = num - ? where id = ? and type = ? and kind = ?");
            query.addBindValue(businessNum);
        }
    }
    query.addBindValue(id);
    query.addBindValue(type);
    query.addBindValue(kind);
    query.exec();
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 5 << 3;
    tcpServerConnection->write(outBlock);
}

void Thread::sendPlantResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)0 << 4 << 0;
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 4;
    int number, kind;
    in >> number >> kind;
    query.exec(QString("select num from store where id=%1 and kind=%2 and type=0").arg(id).arg(kind));
    if(!query.next())
    {
        tcpServerConnection->write(outBlock);        //种子不足
        return;
    }
    int num = query.value(0).toInt();
    query.exec(QString("select is_recla, kind from soil where id = %1 and number = %2").arg(id).arg(number));
    if(!query.next() || query.value(0).toBool() == false || query.value(1).toInt())
    {
        out << 1;
        tcpServerConnection->write(outBlock);        //土地信息错误
        return;
    }
    query.exec(QString("select yield,growTime,allSta from plant where id=%1").arg(kind));
    query.next();
    int group_time = query.value(1).toInt();
    int allSta = query.value(2).toInt();
    int yield = query.value(0).toInt();
    if(num == 1)
        query.exec(QString("delete from store where id=%1 and kind=%2 and type=0").arg(id).arg(kind));
    else
        query.exec(QString("update store set num=num-1 where id=%1 and kind=%2 and type=0").arg(id).arg(kind));
    QDateTime plant_time = QDateTime::currentDateTime();
    query.exec(QString("update soil set kind=%1,yield=%2,plant_time='%3',water='%3',pyre='%3',weed='%3',cal_time='%3',harvest=0 where id=%4 and"
        " number=%5").arg(kind).arg(yield).arg(plant_time.toString("yyyy-MM-dd hh:mm:ss")).arg(id).arg(number));
    out << 2 << group_time << allSta << yield << plant_time;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpServerConnection->write(outBlock);
}

void Thread::sendSpadResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)0 << 6 << 0;
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 6;
    int number;
    in >> number;
    query.exec(QString("select is_recla, kind from soil where id = %1 and number = %2").arg(id).arg(number));
    if(!query.next() || query.value(0).toBool() == false || query.value(1).toInt() == 0)
    {
        tcpServerConnection->write(outBlock);      //土地信息错误
        return;
    }
    query.exec(QString("update soil set kind = 0, harvest = 0 where id = %1 and number = %2").arg(id).arg(number));
    out << 1;
    tcpServerConnection->write(outBlock);
}

void Thread::sendHarvestResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)0 << 7 << 0;
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 7;
    int number;
    in >> number;
    query.exec(QString("select is_recla, kind, harvest, level, cal_time, yield from soil where id = %1 and number = %2").arg(id).arg(number));
    if(!query.next() || query.value(0).toBool() == false || query.value(1).toInt() == 0 || query.value(2).toInt() == 1)
    {
        tcpServerConnection->write(outBlock);      //土地信息错误
        return;
    }
    int kind = query.value(1).toInt();
    //int level = query.value(3).toInt();
    QDateTime cal_time = query.value(4).toDateTime();
    int yield = query.value(5).toInt();
    query.exec(QString("select growTime, allSta from plant where id = %1").arg(kind));
    query.next();
    int growTime = query.value(2).toInt();
    int allSta = query.value(3).toInt();
    if(cal_time.secsTo(QDateTime::currentDateTime()) < (qint64)growTime * allSta * 3600)
    {
        out << 1;
        tcpServerConnection->write(outBlock);       //当前土地植物未成熟
        return;
    }
    query.exec(QString("update soil set harvest = 1 where id = %1 and number = %2").arg(id).arg(number));
    query.exec(QString("select * from store where id = %1 and type = %2 and kind = %2").arg(id).arg(Fruit).arg(kind));
    if(query.next())
    {
        query.exec(QString("update store set num = num + %1 where id = %2 and type = %3 and kind = %4")
                   .arg(yield).arg(id).arg(Fruit).arg(kind));
    }
    else
        query.exec(QString("insert into store values(%1, %2, %3, %4)").arg(id).arg(kind).arg(yield).arg(Fruit));
    out << 2 << kind << yield;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
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

