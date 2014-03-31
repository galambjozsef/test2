#include "socketserver.h"
#include "utility.h"

#include <errno.h>
#include <iostream>
#include <sys/ioctl.h>
#include <memory.h>


#include "criticalsectionlock.h"


//AA: NOTE DI UTILIZZO: tutte le funzioni di socket (eccetto quelle di configurazione) ritornano in caso di errore SOCKET_ERROR o 0 nel caso di timeout
//						volendo analizzare il codice di errore bisogna invocare WSAGetLastError(). Si � scelto nel caso di SOCKET_ERROR
//						di chiudere il socket




SocketServer::SocketServer(int maxlen):MAXLENBUF(maxlen),TIMEOUT(4000)
{
	m_SocketSrv = SOCKET_ERROR;
	int mydebug = 0;
	LoadSocketErrorsMaps();
}

bool SocketServer::Ini_Socket_Interface(std::string& err)
{
	char buffer[256];

	int n;
	if(m_SocketSrv != SOCKET_ERROR)
		return true; //La socket è già definita!!!!
	m_SocketSrv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (IsError(m_SocketSrv,err))
	{
		m_SocketSrv = SOCKET_ERROR;
		return false;
	}

        int list_fd , conn_fd ;
        int waiting = 0;
        int keepalive = 1;
        int e =  setsockopt ( m_SocketSrv , SOL_SOCKET , SO_KEEPALIVE ,&keepalive , sizeof ( keepalive ));
   	int reuse = 1;
	e = setsockopt ( m_SocketSrv , SOL_SOCKET , SO_REUSEADDR , &reuse , sizeof ( reuse ));

	int set = 1;
	setsockopt(m_SocketSrv, SOL_SOCKET, MSG_NOSIGNAL, (void *)&set, sizeof(int)); //Evita il SIGPIPE nel caso di una chiusura inattesa di connessione ...


	IPPort = 8081;
	m_serv_addr.sin_family = AF_INET;
	m_serv_addr.sin_addr.s_addr = INADDR_ANY;
	m_serv_addr.sin_port = htons(IPPort);
	int r = bind(m_SocketSrv, (struct sockaddr *) &m_serv_addr, sizeof(m_serv_addr));
	//AA: TO UNDERSTAND!!!!!!!!!!!!!!!!!!!!!
	if (IsError(r,err))
	{
		//m_SocketSrv = SOCKET_ERROR;
		Disconnect();
		return false;
	}


	if( listen(m_SocketSrv,MAX_CONNECTIONS_NUM) != 0)
	{
		//m_SocketSrv = SOCKET_ERROR;
		Disconnect();
		return false;
	}

	m_clilen = sizeof(m_cli_addr);

	return true;
}


bool SocketServer::Accept(std::string& err)
{
	m_SocketClnt = accept(m_SocketSrv, (struct sockaddr *) &m_cli_addr,(socklen_t*) &m_clilen);
	if (IsError(m_SocketClnt,err))
	{
		//m_SocketSrv = SOCKET_ERROR;
		Disconnect();
		return false;
	}
	return true;
}
/*
void SocketServer::AddClient(ClientConnection* TheClient)
{
	CriticalSectionLock l( guard ) ;
	TheClientPool.push_back(TheClient);
}

bool SocketServer::ReadClnt(std::string err,std::string& FromClient)
{
	FromClient = "";
	char *szBuffer = NULL;
	szBuffer = new char[MAXLENBUF];
	bzero(szBuffer,MAXLENBUF);
	int n = read(m_SocketClnt,szBuffer,MAXLENBUF);
	if(n == 0)
	{
		err = "Unknown error !!!";
		return false;
	}
	if( IsError(n,err))
	{
		delete[] szBuffer;
		return false;
	}

	FromClient = szBuffer;
	delete[] szBuffer;
	return true;
}
*/
/*
bool SocketServer::WriteClnt(std::string err,std::string& ToClient)
{
	std::string SrvOutPut = ToClient;
	SrvOutPut = SrvOutPut + "\0";
    int n = write(m_SocketClnt,SrvOutPut.c_str(),SrvOutPut.size());

	if( IsError(n,err))
		return false;

	return true;
}
*/
/*
bool SocketServer::SrvComm(std::string err,std::string& FromClient)
{
	CriticalSectionLock l( guard ) ;

	FromClient = "";
	char *szBuffer = NULL;
	szBuffer = new char[MAXLENBUF];
	bzero(szBuffer,MAXLENBUF);
	int n = read(m_SocketClnt,szBuffer,MAXLENBUF);
	if(n == 0)
	{
		err = "Unknown error !!!";
		return false;
	}
	if( IsError(n,err))
	{
		delete[] szBuffer;
		return false;
	}

	FromClient = szBuffer;
	std::string SrvOutPut = szBuffer;
	SrvOutPut = SrvOutPut + "\0";
	n = write(m_SocketClnt,SrvOutPut.c_str(),SrvOutPut.size());

	delete[] szBuffer;

	if( IsError(n,err))
		return false;

	return true;
}
*/



SocketServer::~SocketServer(void)
{
	Disconnect();
}


void SocketServer::Disconnect(void)
{
	if ( m_SocketSrv != INVALID_SOCKET)
	{
	    shutdown(m_SocketSrv,SHUT_RDWR);
//	    close(m_SocketSrv);
	    m_SocketSrv = INVALID_SOCKET;
	}
}




std::string  SocketServer::get_ip(const char *host)
{

	struct hostent *hent;
	int iplen = INET_ADDRSTRLEN; //XXX.XXX.XXX.XXX
	char *ip = (char *)malloc(iplen+1);
	memset(ip, 0, iplen+1);

	if((hent = gethostbyname(host)) == NULL)

	{
		herror("SocketServer Can't get IP");
		//AA exit(1);
		//AA std::string _ip(_ip);
		return "";
	}

	if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen ) == NULL)

	{
		perror("SocketServer Can't resolve host");
		//AA exit(1);
		//AA std::string _ip(_ip);
		return "";
	}

	std::string _ip(ip);
	return _ip;
}



bool SocketServer::IsError(int err,std::string& errcode)
{
	CriticalSectionLock l( guard ) ;
    errcode = "";

    if(err != SOCKET_ERROR)
    {
        errcode = "";
        //AA std::cout<<"SOCKET CONNECTING SUCCESS "<<std::endl;
        return false;
    }
    std::map<int, std::string>::iterator it;

    it = SocketErrorsMap.find(errno);
    if (it == SocketErrorsMap.end())
        errcode = "Generic Error";
    else
        errcode = (*it).second;

//    std::cout<<"SOCKET SRV ERROR: ";
//    std::cout<<errcode.c_str();//<<std::endl;
//    std::cout<<" ERRNO:";
//    std::cout<<int2string(errno)<<std::endl;
    return true;
}

void SocketServer::LoadSocketErrorsMaps(void)
{
    std::pair<int, std::string> temp_item;

    temp_item.first = ECONNREFUSED;
    temp_item.second = "The attempt to connect was rejected.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ETIMEDOUT;
    temp_item.second = "Si e` avuto timeout durante il tentativo di connessione";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENETUNREACH;
    temp_item.second = "La rete non e` raggiungibile.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EINPROGRESS;
    temp_item.second = "Il socket non e' bloccante  e la connessione non puo` essere conclusa.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EALREADY;
    temp_item.second = "Il socket non e' bloccante  e la connessione non puo` essere conclusa.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EAGAIN;
    temp_item.second = "Non ci sono piu` porte locali libere.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EAFNOSUPPORT;
    temp_item.second = "The addresses in the specified address family cannot be used with this socket.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EACCES;
    temp_item.second = "Si ` tentato di eseguire una connessione ad un indirizzo broadcast senza che il \nsocket fosse stato abilitato per il broadcast.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EPERM;
    temp_item.second = "Si ` tentato di eseguire una connessione ad un indirizzo broadcast senza che il \nsocket fosse stato abilitato per il broadcast.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EBADF;
    temp_item.second = "L’argomento sockfd non e` un file descriptor valido.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENOTSOCK;
    temp_item.second = "L’argomento sockfd non e` un socket.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EOPNOTSUPP;
    temp_item.second = "Il socket ` di un tipo che non supporta questa operazione.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EWOULDBLOCK;
    temp_item.second = "The socket is marked nonblocking, and no connections are present to be accepted";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENOBUFS;
    temp_item.second = "Spesso significa che l’allocazione della memoria e` limitata dai limiti sui buffer dei socket, non dalla memoria di sistema.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENOMEM;
    temp_item.second = "Spesso significa che l’allocazione della memoria e` limitata dai limiti sui buffer dei socket, non dalla memoria di sistema.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EINTR;
    temp_item.second = "A signal interrupted send before any data was transmitted.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EFAULT;
    temp_item.second = "L’indirizzo name non e` valido.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENOTCONN;
    temp_item.second = "Il socket non ` connesso.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EADDRNOTAVAIL;
    temp_item.second = "The specified address is not a valid address.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ECONNRESET;
    temp_item.second = "A connection was forcibly closed by a peer.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EDESTADDRREQ;
    temp_item.second = "The socket is not in connection-mode and no peer address is set.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EDESTADDRREQ;
    temp_item.second = "The socket is not in connection-mode and no peer address is set.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EHOSTUNREACH;
    temp_item.second = "The destination host cannot be reached.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EINVAL;
    temp_item.second = "The Length parameter is invalid \n or \n The specified path name contains a character with the high-order bit set";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EISCONN;
    temp_item.second = "A SOCK_DGRAM socket is already connected.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EMSGSIZE;
    temp_item.second = "The message is too large to be sent all at once, as the socket requires.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EPIPE;
    temp_item.second = "An attempt was made to send on a socket that was connected, but the connection has been shut down either by the remote peer or by this side of the connection.\n If the socket is of type SOCK_STREAM, the SIGPIPE signal is generated to the calling process.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EADDRINUSE;
    temp_item.second = "The specified address is already in use. This error will also occur if the SO_REUSEADDR socket option was set and the local address (whether specified or selected by the system) is already in use.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENOSPC;
    temp_item.second = "There is no space left on a device or system table.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENETDOWN;
    temp_item.second = "The specified physical network is down.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = EPROTOTYPE;
    temp_item.second = "The specified address has a different type from the socket that is bound to the specified peer address.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ELOOP;
    temp_item.second = "Too many symbolic links were encountered in translating the path name in address.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENOENT;
    temp_item.second = "A component of the path name does not name an existing file or the path name is an empty string.";
    SocketErrorsMap.insert(temp_item);
    temp_item.first = ENOTDIR;
    temp_item.second = "A component of the path prefix of the path name in address is not a directory.";
    SocketErrorsMap.insert(temp_item);
}


