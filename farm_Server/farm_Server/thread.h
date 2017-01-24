#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QSqlQuery>

class QTcpSocket;

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QTcpSocket * tcpServerConnection);
    void checkLogin(QDataStream &in);
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
    QByteArray block;
    QString username, password;
};

#endif // THREAD_H
