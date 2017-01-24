#include "farm_server.h"
#include "thread.h"
#include <QMessageBox>
#include <QDataStream>
#include <iostream>
using namespace std;

farm_Server::farm_Server()
    : tcpServer(new QTcpServer())
{
    if(!tcpServer->listen(QHostAddress::LocalHost, 6666))
    {
        QMessageBox::warning(0, "启动服务器失败", tcpServer->errorString());
        exit(0);
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void farm_Server::acceptConnection()
{
    tcpServerConnection = tcpServer->nextPendingConnection();
    Thread * newThread = new Thread(tcpServerConnection);
    newThread->start();
}
