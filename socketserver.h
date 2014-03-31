#ifndef SOCKETSRV_
#define SOCKETSRV_


/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <map>
#include <vector>


#include <cstdlib>
#include <arpa/inet.h>

#include <netdb.h>
#include "criticalsection.h"
#include "clientconnection.h"

#define SOCKET_OK 0
#define SOCKET_ERROR -1
#define	INVALID_SOCKET -1

#define POOL_SIZE 2
#define MAX_CONNECTIONS_NUM 20

class ClientConnection;

class SocketServer
{
public:
	SocketServer(int maxlen);
	virtual ~SocketServer(void);

	const int MAXLENBUF;
	const int TIMEOUT;

	void Disconnect(void);

	std::string m_ServerName;
	unsigned short	IPPort;

	enum{
			Err_Codes = 45
	};
	// Error Code || Description
    std::map<int, std::string> SocketErrorsMap;
	bool Accept(std::string& err);



	void LoadSocketErrorsMaps(void);
	bool IsError(int err,std::string& errcode);
    bool Ini_Socket_Interface(std::string& err);

   // bool SrvComm(std::string err,std::string& FromClient);
   // bool ReadClnt(std::string err,std::string& FromClient);
   // bool WriteClnt(std::string err,std::string& ToClient);
    struct sockaddr_in m_SockAddrIn;
    struct sockaddr_in m_serv_addr;
	int m_SocketClnt;

private:
	std::string  get_ip(const char *host);
	CriticalSection guard ;

public:
	int m_SocketSrv;

	unsigned int m_timeout;
	int m_clilen;
	struct sockaddr m_cli_addr;
//AA	ClientConnection* m_PermanentSocket;
	typedef std::vector<ClientConnection*> ClientsPool;
	ClientsPool TheClientPool;
};
#endif
