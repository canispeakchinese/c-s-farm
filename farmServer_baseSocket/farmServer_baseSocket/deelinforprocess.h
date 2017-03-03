#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <signal.h>
#include <bits/stdc++.h>
#include <QByteArray>
#include <QMap>
#include <QList>
#include <QtSql/QSqlQuery>
#define newMessage 0
#define newParti 1
#define newChat 2

using namespace std;
class DeelInforProcess
{
public:
    DeelInforProcess(int server_sockfd);
    void deelInfor();
    int newParticipator(int client_sockfd, long long &length);
    int newMessComing(int client_sockfd, long long &length);
    int connectAbort(int client_sockfd);
	~DeelInforProcess();
private:
    int server_sockfd;

    int nfds, fd;
    fd_set rfds;            /* read file descriptor set   */
    fd_set afds;            /* active file descriptor set */

    sockaddr_in addr;
    int alen;

    QByteArray inBlock;
    qint64 length;
    int messageType;

    QSqlQuery query;

    QList<QByteArray>list;
    QMap<int, QString>mp;
};
