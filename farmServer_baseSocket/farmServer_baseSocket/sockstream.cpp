#include "sockstream.h"
#include "deelinforprocess.h"
#include "connect_mysql.h"

SocketStream::SocketStream()
{
	createMysqlConnection();
	createSocket();
}

void SocketStream::createMysqlConnection()
{
    if(!connect_mysql())
        exit(1);//errexit("connect mysql database: %s\n",strerror(errno));
}

void SocketStream::createSocket()
{
	//创建流套接字
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//设置服务器接收的连接地址和监听的端口
	server_addr.sin_family = AF_INET;//指定网络套接字
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//接受所有IP地址的连接
    server_addr.sin_port = htons(7777);//绑定到7777端口
	//绑定（命名）套接字
	bind(server_sockfd, (sockaddr*)&server_addr, sizeof(server_addr));
}

void SocketStream::listenSocket()
{
	//创建套接字队列，监听套接字
	listen(server_sockfd, 5);
	//忽略子进程停止或退出信号
	signal(SIGCHLD, SIG_IGN);
	
    DeelInforProcess deelInforProcess(server_sockfd);
    deelInforProcess.deelInfor();
}

SocketStream::~SocketStream()
{

}
