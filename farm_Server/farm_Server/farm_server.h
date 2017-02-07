#ifndef FARM_SERVER_H
#define FARM_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QWidget>

class farm_Server : public QObject
{
    Q_OBJECT
public:
    farm_Server();

protected slots:
    void acceptConnection();

private:
    QTcpServer * tcpServer;
    QTcpSocket * tcpServerConnection;
    QSqlQuery query;
};

#endif // FARM_SERVER_H
