#include "deelinforprocess.h"
#include "useful.h"
#include <QDataStream>
#include <QDateTime>
#include <QDebug>

#define MAXINFORLEN 50
#define MAXMESSLEN 150
#define HEADLEN (sizeof(long long) + sizeof(int))

DeelInforProcess::DeelInforProcess(int server_sockfd) :
    server_sockfd(server_sockfd)
{
    inBlock.clear();
    nfds = getdtablesize();
    FD_ZERO(&afds);
    FD_SET(server_sockfd, &afds);
    list.clear();
    mp.clear();
}

void DeelInforProcess::deelInfor()
{
    while(1)
    {
        memcpy(&rfds, &afds, sizeof(rfds));
        if(select(nfds, &rfds, (fd_set *)NULL, (fd_set *)NULL, (timeval *)0) < 0)
            errexit("select: %s\n", strerror(errno));
        if(FD_ISSET(server_sockfd, &rfds))
        {
            alen = sizeof(addr);
            if((fd = accept(server_sockfd, (sockaddr *)&addr, (socklen_t *)&alen)) < 0)
            {
                errcont("accept: %s\n", strerror(errno));
                continue;
            }
            FD_SET(fd, &afds);
        }
        for(int client_sockfd=0; client_sockfd<nfds; client_sockfd++)
        {
            if(client_sockfd != server_sockfd && FD_ISSET(client_sockfd, &rfds))
            {
                inBlock.resize(HEADLEN);
                if(readn(client_sockfd, inBlock.begin(), HEADLEN) == -1)
                {
                    connectAbort(client_sockfd);
                    continue;
                }
                QDataStream in(&inBlock, QIODevice::ReadWrite);
                in.setVersion(QDataStream::Qt_5_5);
                in >> length >> messageType;
                switch (messageType)
                {
                case newMessage:
                    newMessComing(client_sockfd, length);
                    break;
                case newParti:
                    newParticipator(client_sockfd, length);
                    break;
                case newChat:
                    break;
                default:
                    break;
                }
            }
        }
    }
}

int DeelInforProcess::newParticipator(int client_sockfd, long long &length)
{
    if(length > MAXINFORLEN || mp.find(client_sockfd) != mp.end())
        return connectAbort(client_sockfd);

    inBlock.resize(length - HEADLEN);
    if(readn(client_sockfd, inBlock.begin(), length - HEADLEN) == -1)
        return connectAbort(client_sockfd);

    QDataStream in(&inBlock, QIODevice::ReadWrite);

    QString username, password;
    in >> username >> password;

    query.prepare("select isonline from user where username = ? and password = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();

    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_5);
    out << qint64(0) << newParti;

    if(query.next())
    {
        mp[client_sockfd] = username;           //用户登录成功
        out << true;
        out << list.size();
        foreach (QByteArray message, list) {
            outBlock.append(message);
        }
    }
    else
        out << false;
    out.device()->seek(0);
    out << (qint64)outBlock.size();

    if(writen(client_sockfd, outBlock.begin(), outBlock.size()) == -1)
        return connectAbort(client_sockfd);
    return 0;
}

int DeelInforProcess::newMessComing(int client_sockfd, long long &length)
{
    if(length > MAXMESSLEN || mp.find(client_sockfd) == mp.end())
        return connectAbort(client_sockfd);

    inBlock.resize(length - HEADLEN);
    if(readn(client_sockfd, inBlock.begin(), length - HEADLEN) == -1)
        return connectAbort(client_sockfd);

    QByteArray outBlock;
    QDataStream out(&outBlock, QIODevice::ReadWrite);
    QString username = mp[client_sockfd];
    out << (qint64)(0) << newMessage << username << QDateTime::currentDateTime();
    out.device()->seek(0);
    out << (qint64)(outBlock.size()+inBlock.size());
    outBlock.append(inBlock);

    for(int i=0; i<nfds; i++)
        if(i != server_sockfd && mp.find(i) != mp.end())
            if(writen(i, outBlock.begin(), outBlock.size()) == -1)
                connectAbort(i);

    out.device()->seek(0);
    out >> length >> messageType;
    list.append(out.device()->readAll());
    if(list.size()>5)
        list.pop_front();
    return 0;
}

int DeelInforProcess::connectAbort(int client_sockfd)
{
    close(client_sockfd);
    FD_CLR(client_sockfd, &afds);
    mp.erase(mp.find(client_sockfd));
    return errcont("read: %s\n", strerror(errno));
}

DeelInforProcess::~DeelInforProcess()
{

}
