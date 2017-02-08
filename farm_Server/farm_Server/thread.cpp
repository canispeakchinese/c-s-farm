#include "thread.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QDateTime>
#include <time.h>

const int duration = 3600;
const int interval = 3600 * 5;
const int randTime = 3600;
const int needExp[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

Thread::Thread(QTcpSocket *tcpServerConnection) : tcpServerConnection(tcpServerConnection)
{
    id = -1;
    totalBytes = 0;
    inBlock.resize(0);
    srand(time(NULL));
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
            break;
        case 8:
            sendStatusChangeResult(in);
            break;
        case 9:
            sendReclaResult(in);
            break;
        case 10:
            sendFertilizeResult(in);
            break;
        default:
            break;
        }
        totalBytes = 0;
        inBlock = in.device()->readAll();
        if(inBlock.size() >= (int)(sizeof(qint64) + sizeof(int)))
            readyRead();
    }
}

int Thread::yieldChange(int number, const QDateTime aimTime, QDateTime &water, QDateTime &pyre, QDateTime &weed)
{
    QSqlQuery query;
    query.exec(QString("select yield from soil where id = %1 and number = %2").arg(id).arg(number));
    query.next();
    int yield = query.value(0).toInt();
    while(water != QDateTime(QDate(1900,1,1), QTime(0, 0)) && water.addSecs(duration) <= aimTime)
        water = water.addSecs(duration + interval + rand()%randTime);
    while(pyre != QDateTime(QDate(1900,1,1), QTime(0, 0)) && pyre.addSecs(duration) <= aimTime)
    {
        yield--;
        pyre = pyre.addSecs(duration + interval + rand()%randTime);
    }
    while(weed != QDateTime(QDate(1900,1,1), QTime(0, 0)) && weed.addSecs(duration) <= aimTime)
        weed = weed.addSecs(duration + interval +rand()%randTime);
    if(yield < 0)
        yield = 0;
    QString aim = QString("update soil set yield = %1").arg(yield);
    if(water != QDateTime(QDate(1900,1,1), QTime(0, 0)))
        aim += QString(", water = '%1'").arg(water.toString("yyyy-MM-dd hh:mm:ss"));
    if(pyre != QDateTime(QDate(1900,1,1), QTime(0, 0)))
        aim += QString(", pyre = '%1'").arg(pyre.toString("yyyy-MM-dd hh:mm:ss"));
    if(weed != QDateTime(QDate(1900,1,1), QTime(0, 0)))
        aim += QString(", weed = '%1'").arg(weed.toString("yyyy-MM-dd hh:mm:ss"));
    aim += QString(" where id = %1 and number = %2").arg(id).arg(number);
    query.exec(aim);
    return yield;
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
    out << qint64(0) << 3 << updateResult;
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
    query.prepare("select level, is_recla, kind, cal_time, water, pyre, weed, harvest from soil where id=? order by number");
    query.addBindValue(id);
    query.exec();
    int number = 0;
    int reclaNum = 0;
    out << reclaNum;
    while(query.next())
    {
        number++;
        out << query.value(0).toInt();
        bool is_recla = query.value(1).toBool();
        out << is_recla;
        if(is_recla)
        {
            int kind = query.value(2).toInt();
            out << kind;
            if(kind)
            {
                if(query.value(7).toBool())         //已经收获
                    out << true;
                else
                {
                    out << false;
                    QSqlQuery query2;
                    query2.exec(QString("select growTime, allSta from plant where id=%1").arg(kind));
                    query2.next();
                    int growTime = query2.value(0).toInt();
                    int allSta = query2.value(1).toInt();
                    QDateTime cal_time = query.value(3).toDateTime();
                    QDateTime water = query.value(4).toDateTime();
                    QDateTime pyre = query.value(5).toDateTime();
                    QDateTime weed = query.value(6).toDateTime();
                    if(cal_time.addSecs(growTime * allSta * 3600) <= QDateTime::currentDateTime())      //已经成熟
                        out << true << allSta << yieldChange(number, cal_time.addSecs(growTime * allSta * 3600), water, pyre, weed);
                    else                                                                                //尚未成熟
                    {
                        out << false << growTime << allSta;
                        out << yieldChange(number, QDateTime::currentDateTime(), water, pyre, weed);
                        out << growTime * allSta * 3600 - (int)cal_time.secsTo(QDateTime::currentDateTime());
                        out << (water <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(water)) > 0 ?
                                   (int)QDateTime::currentDateTime().secsTo(water) : (duration + (int)QDateTime::currentDateTime().secsTo(water)));
                        out << (pyre <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(pyre)) > 0 ?
                                   (int)QDateTime::currentDateTime().secsTo(pyre) : (duration + (int)QDateTime::currentDateTime().secsTo(pyre)));
                        out << (water <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(weed)) > 0 ?
                                   (int)QDateTime::currentDateTime().secsTo(weed) : (duration + (int)QDateTime::currentDateTime().secsTo(weed)));
                    }
                }
            }
        }
        else if(reclaNum == 0)
            reclaNum = number;
    }
    out.device()->seek(0);
    out << reclaNum;
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
        query.exec("select id from fertilize");
        while(query.next())
        {
            out << (int)Fertilize << query.value(0).toInt();
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
        query.exec(QString("select type, kind, num from store where id=%1 and type in(%2, %3)").arg(id).arg(Seed).arg(Fertilize));
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
            query.prepare("select buyprice, buylevel from plant where id = ?");
            query.addBindValue(kind);
            query.exec();
        }
        else if(type == Fertilize)
        {
            query.prepare("select buyprice, buylevel from fertilize where id = ?");
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
        int buylevel = query.value(1).toInt();
        query.exec(QString("select money, level from user where id = %1").arg(id));
        query.next();
        int money = query.value(0).toInt();
        int level = query.value(1).toInt();
        if(money < buyprice * businessNum || level < buylevel)
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
            else if(type == Fertilize)
            {
                query.exec(QString("select buyprice from fertilize where id = %1").arg(kind));
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
    int growTime = query.value(1).toInt();
    int allSta = query.value(2).toInt();
    int yield = query.value(0).toInt();
    if(num == 1)
        query.exec(QString("delete from store where id=%1 and kind=%2 and type=0").arg(id).arg(kind));
    else
        query.exec(QString("update store set num=num-1 where id=%1 and kind=%2 and type=0").arg(id).arg(kind));
    QDateTime cal_time = QDateTime::currentDateTime();
    int water = interval + rand()%randTime;
    int pyre = interval + rand()%randTime;
    int weed = interval + rand()%randTime;
    query.exec(QString("update soil set kind=%1,yield=%2,cal_time='%3',water='%4',pyre='%5',weed='%6',harvest=0 where id=%7 and"
        " number=%8").arg(kind).arg(yield).arg(cal_time.toString("yyyy-MM-dd hh:mm:ss")).arg(cal_time.addSecs(water).toString("yyyy-MM-dd hh:mm:ss")).arg(cal_time.addSecs(pyre).toString("yyyy-MM-dd hh:mm:ss")).arg(cal_time.addSecs(weed).toString("yyyy-MM-dd hh:mm:ss")).arg(id).arg(number));
    out << 2 << number << growTime << allSta << yield << water << pyre << weed;
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
    out << 1 << number;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
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
    if(!query.next() || query.value(0).toBool() == false || query.value(1).toInt() == 0 || query.value(2).toBool() == true)
    {
        tcpServerConnection->write(outBlock);      //土地信息错误
        return;
    }
    int kind = query.value(1).toInt();
    //int level = query.value(3).toInt();
    QDateTime cal_time = query.value(4).toDateTime();
    int yield = query.value(5).toInt();
    query.exec(QString("select growTime, allSta, exp from plant where id = %1").arg(kind));
    query.next();
    int growTime = query.value(0).toInt();
    int allSta = query.value(1).toInt();
    int exp = query.value(2).toInt();
    if(cal_time.secsTo(QDateTime::currentDateTime()) < (qint64)growTime * allSta * 3600)
    {
        out << 1;
        tcpServerConnection->write(outBlock);       //当前土地植物未成熟
        return;
    }
    query.exec(QString("update soil set harvest = 1 where id = %1 and number = %2").arg(id).arg(number));
    query.exec(QString("select * from store where id = %1 and type = %2 and kind = %3").arg(id).arg(Fruit).arg(kind));
    if(query.next())
    {
        query.exec(QString("update store set num = num + %1 where id = %2 and type = %3 and kind = %4")
                   .arg(yield).arg(id).arg(Fruit).arg(kind));
    }
    else
        query.exec(QString("insert into store values(%1, %2, %3, %4)").arg(id).arg(kind).arg(yield).arg(Fruit));
    query.exec(QString("select level, exp from user where id = %1").arg(id));
    query.next();
    int currLevel = query.value(0).toInt();
    int currExp = query.value(1).toInt();
    currExp += exp;
    while(currExp >= needExp[currLevel])
    {
        currExp -= needExp[currLevel];
        currLevel++;
    }
    query.exec(QString("update user set level = %1, exp = %2 where id = %3").arg(currLevel).arg(currExp).arg(id));
    out << 2 << number << kind << yield << exp;
    out.device()->seek(0);
    out << (qint64)outBlock.size();
    tcpServerConnection->write(outBlock);
}

void Thread::sendStatusChangeResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)0 << 8 << 0;
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 8;
    int number, _status;
    bool _auto;
    in >> number >> _status >> _auto;
    query.exec(QString("select kind, cal_time, water, pyre, weed from soil where id = %1 and number = %2").arg(id).arg(number));
    query.next();
    int kind = query.value(0).toInt();
    if(!kind)           //未种植
    {
        tcpServerConnection->write(outBlock);
        return;
    }
    else
    {
        QSqlQuery query2;
        query2.exec(QString("select growTime, allSta from plant where id = %1").arg(kind));
        query2.next();
        int growTime = query2.value(0).toInt();
        int allSta = query2.value(1).toInt();
        if(query.value(1).toDateTime().addSecs(growTime * allSta * 3600) <= QDateTime::currentDateTime())//已成熟
        {
            tcpServerConnection->write(outBlock);
            return;
        }
    }
    QDateTime water = QDateTime(QDate(1900,1,1), QTime(0, 0));
    QDateTime pyre = QDateTime(QDate(1900,1,1), QTime(0, 0));
    QDateTime weed = QDateTime(QDate(1900,1,1), QTime(0, 0));
    if(_status == Water)
    {
        water = query.value(2).toDateTime();
        if(_auto)//自动消失
        {
            if(water.addSecs(duration) <= QDateTime::currentDateTime())
            {
                yieldChange(number, QDateTime::currentDateTime(), water, pyre, weed);
                out << 1 << number << _status << _auto << (water <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(water)) > 0 ?
                                                                       (int)QDateTime::currentDateTime().secsTo(water) : (duration + (int)QDateTime::currentDateTime().secsTo(water)));
                out.device()->seek(0);
                out << (qint64)outBlock.size();
                tcpServerConnection->write(outBlock);
                return;
            }
            else
            {
                tcpServerConnection->write(outBlock);
                return;
            }
        }
        else//人工浇水
        {
            if(water > QDateTime::currentDateTime() || water.addSecs(duration) <= QDateTime::currentDateTime())
            {
                tcpServerConnection->write(outBlock);
                return;
            }
            else
            {
                water = QDateTime::currentDateTime().addSecs(interval + rand()%randTime);
                out << 1 << number << _status << _auto << (water <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(water)) > 0 ?
                                                                       (int)QDateTime::currentDateTime().secsTo(water) : (duration + (int)QDateTime::currentDateTime().secsTo(water)));
                query.exec(QString("update soil set yield = yield + 1, water = '%1' where id = %2 and number = %3")
                           .arg(water.toString("yyyy-MM-dd hh:mm:ss")).arg(id).arg(number));
                out.device()->seek(0);
                out << (qint64)outBlock.size();
                tcpServerConnection->write(outBlock);
                return;
            }
        }
    }
    else if(_status == Pyre)
    {
        pyre = query.value(3).toDateTime();
        if(_auto)//自动消失
        {
            if(pyre.addSecs(duration) <= QDateTime::currentDateTime())
            {
                yieldChange(number, QDateTime::currentDateTime(), water, pyre, weed);
                out << 1 << number << _status << _auto << (pyre <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(pyre)) > 0 ?
                                                                       (int)QDateTime::currentDateTime().secsTo(pyre) : (duration + (int)QDateTime::currentDateTime().secsTo(pyre)));
                out.device()->seek(0);
                out << (qint64)outBlock.size();
                tcpServerConnection->write(outBlock);
                return;
            }
            else
            {
                tcpServerConnection->write(outBlock);
                return;
            }
        }
        else//人工除虫
        {
            if(pyre > QDateTime::currentDateTime() || pyre.addSecs(duration) <= QDateTime::currentDateTime())
            {
                tcpServerConnection->write(outBlock);
                return;
            }
            else
            {
                pyre = QDateTime::currentDateTime().addSecs(interval + rand()%randTime);
                out << 1 << number << _status << _auto << (pyre <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(pyre)) > 0 ?
                                                                       (int)QDateTime::currentDateTime().secsTo(pyre) : (duration + (int)QDateTime::currentDateTime().secsTo(pyre)));
                query.exec(QString("update soil set pyre = '%1' where id = %2 and number = %3")
                           .arg(pyre.toString("yyyy-MM-dd hh:mm:ss")).arg(id).arg(number));
                out.device()->seek(0);
                out << (qint64)outBlock.size();
                tcpServerConnection->write(outBlock);
                return;
            }
        }
    }
    else if(_status == Weed)
    {
        weed = query.value(4).toDateTime();
        if(_auto)//自动消失
        {
            if(weed.addSecs(duration) <= QDateTime::currentDateTime())
            {
                yieldChange(number, QDateTime::currentDateTime(), water, pyre, weed);
                out << 1 << number << _status << _auto << (weed <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(weed)) > 0 ?
                                                                       (int)QDateTime::currentDateTime().secsTo(weed) : (duration + (int)QDateTime::currentDateTime().secsTo(weed)));
                out.device()->seek(0);
                out << (qint64)outBlock.size();
                tcpServerConnection->write(outBlock);
                return;
            }
            else
            {
                tcpServerConnection->write(outBlock);
                return;
            }
        }
        else//人工除草
        {
            if(weed > QDateTime::currentDateTime() || weed.addSecs(duration) <= QDateTime::currentDateTime())
            {
                tcpServerConnection->write(outBlock);
                return;
            }
            else
            {
                weed = QDateTime::currentDateTime().addSecs(interval + rand()%randTime);
                out << 1 << number << _status << _auto << (weed <= QDateTime::currentDateTime()) << (((int)QDateTime::currentDateTime().secsTo(weed)) > 0 ?
                                                                       (int)QDateTime::currentDateTime().secsTo(weed) : (duration + (int)QDateTime::currentDateTime().secsTo(weed)));
                query.exec(QString("update soil set weed = '%1' where id = %2 and number = %3")
                           .arg(weed.toString("yyyy-MM-dd hh:mm:ss")).arg(id).arg(number));
                out.device()->seek(0);
                out << (qint64)outBlock.size();
                tcpServerConnection->write(outBlock);
                return;
            }
        }
    }
}

void Thread::sendReclaResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)0 << 9 << 0;
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 9;
    int number;
    in >> number;
    query.exec(QString("select is_recla from soil where id = %1 and number in(%2, %3) order by number").arg(id).arg(number).arg(number+1));
    query.next();
    if(query.value(0).toBool() == false || !query.next() || query.value(0).toBool() == true)
        tcpServerConnection->write(outBlock);//当前土地不可开垦;
    else
    {
        query.exec(QString("select money, level from user where id = %1").arg(id));
        query.next();
        if(query.value(0).toInt() < number || query.value(1).toInt() < number/3+1)
        {
            out << 1;
            tcpServerConnection->write(outBlock);//当前条件不满足
        }
        else
        {
            int money = query.value(0).toInt() - number;
            query.exec(QString("update soil set money = %1 where id = %2").arg(money).arg(id));
            query.exec(QString("update soil set is_recla = 1 where id = %1 and number = %2").arg(id).arg(number+1));
            out << 2 << money;
            out.device()->seek(0);
            out << (qint64)outBlock.size();
            tcpServerConnection->write(outBlock);
        }
    }
}

void Thread::sendFertilizeResult(QDataStream &in)
{
    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << (qint64)0 << 10 << 0;
    out.device()->seek(0);
    out << (qint64)outBlock.size() << 10;
    int number, kind;
    in >> number >> kind;
    query.exec(QString("select num from store where id=%1 and kind=%2 and type=2").arg(id).arg(kind));
    if(!query.next())
    {
        tcpServerConnection->write(outBlock);        //化肥不足
        return;
    }
    int num = query.value(0).toInt();
    query.exec(QString("select is_recla, kind, cal_time from soil where id = %1 and number = %2").arg(id).arg(number));
    if(!query.next() || query.value(0).toBool() == false || query.value(1).toInt() == 0)
    {
        out << 1;
        tcpServerConnection->write(outBlock);        //土地信息错误
        return;
    }
    int seedkind = query.value(1).toInt();
    QDateTime cal_time = query.value(2).toDateTime();
    query.exec(QString("select growTime,allSta from plant where id=%1").arg(seedkind));
    query.next();
    int growTime = query.value(0).toInt();
    int allSta = query.value(1).toInt();
    if(cal_time.addSecs(growTime * allSta * 3600) < QDateTime::currentDateTime())
    {
        out << 1;
        tcpServerConnection->write(outBlock);        //土地信息错误
        return;
    }
    query.exec(QString("select reduTime from fertilize where id = %1").arg(kind));
    query.next();
    int reduTime = query.value(0).toInt();
    if(num == 1)
        query.exec(QString("delete from store where id=%1 and kind=%2 and type=2").arg(id).arg(kind));
    else
        query.exec(QString("update store set num=num-1 where id=%1 and kind=%2 and type=2").arg(id).arg(kind));
    cal_time = cal_time.addSecs(-reduTime);
    query.exec(QString("update soil set cal_time='%1' where id=%2 and number=%3")
               .arg(cal_time.toString("yyyy-MM-dd hh:mm:ss")).arg(id).arg(number));
    out << 2 << number << reduTime;
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

