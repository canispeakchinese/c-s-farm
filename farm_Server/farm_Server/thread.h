#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QSqlQuery>

class QTcpSocket;

enum GoodType {
    Seed, Fruit, Fertilize
};
enum Business {
    Buy, Sell, Use//买,卖,使用
};

enum ToolType {
    Spad, Pack, Water, Pyre, Weed, Harv, Alhar, Kit, Plant, Ferti, Empty
};

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QTcpSocket * tcpServerConnection);
    int yieldChange(int number, const QDateTime aimTime, QDateTime &water, QDateTime &pyre, QDateTime &weed);
    void checkLogin(QDataStream &in);
    void checkSign(QDataStream &in);
    void sendUpdateResult(QDataStream &in);
    QByteArray getSoilResult();
    QByteArray getFriendResult();
    QByteArray getGoodResult(Business business);
    void sendBusinessResult(QDataStream &in);
    void sendPlantResult(QDataStream &in);
    void sendSpadResult(QDataStream &in);
    void sendHarvestResult(QDataStream &in);
    void sendStatusChangeResult(QDataStream &in);
    void sendReclaResult(QDataStream &in);
    void sendFertilizeResult(QDataStream &in);
    ~Thread();

protected:
    void run();

protected slots:
    void readyRead();
    void displayError();
    void disconnected();

private:
    QTcpSocket * tcpServerConnection;
    QSqlQuery query;
    QByteArray inBlock;
    int id;
    QString password;
    qint64 totalBytes;
    int messageType;
};

#endif // THREAD_H
