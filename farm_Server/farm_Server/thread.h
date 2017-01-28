#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QSqlQuery>

class QTcpSocket;

enum GoodType {
    Seed, Fruit
};
enum Business {
    Buy, Sell, Use//买,卖,使用
};

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QTcpSocket * tcpServerConnection);
    void checkLogin(QDataStream &in);
    void sendFriend();
    void sendSoils();
    void sendGoods(QDataStream &in);
    void sendBusinessResult(QDataStream &in);
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
