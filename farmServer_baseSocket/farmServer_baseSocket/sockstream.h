#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <bits/stdc++.h>
using namespace std;
class SocketStream
{
public:
	SocketStream();
	void createMysqlConnection();
	void createSocket();
    void listenSocket();
	~SocketStream();

private:
	int server_sockfd;
    sockaddr_in server_addr;
};
