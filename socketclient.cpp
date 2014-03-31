#include "socketclient.h"
#include "utility.h"

#include <errno.h>
#include <iostream>
#include <sys/ioctl.h>
//#include "GenericException.h"
#include <memory.h>

//AA: NOTE DI UTILIZZO: tutte le funzioni di socket (eccetto quelle di configurazione) ritornano in caso di errore SOCKET_ERROR o 0 nel caso di timeout
//						volendo analizzare il codice di errore bisogna invocare WSAGetLastError(). Si � scelto nel caso di SOCKET_ERROR
//						di chiudere il socket




SocketClient::SocketClient(int maxlen):MAXLENBUF(maxlen),TIMEOUT(4000)
{
	m_SocketError = SOCKET_ERROR;
}

SocketClient::SocketClient(int maxlen, int timeout,std::string _m_configpath):MAXLENBUF(maxlen),TIMEOUT(timeout),m_configpath(_m_configpath)
{

	m_SocketError = SOCKET_ERROR;
	//Configure_Connect();
 	LoadSocketErrorsMaps();
	//SS std::cout<<"Initializing Socket for "<<m_owner.c_str()<<std::endl;
}

SocketClient::SocketClient(int maxlen, int timeout,std::string _m_configpath,std::string _owner,bool IsUploader):MAXLENBUF(maxlen),TIMEOUT(timeout),m_configpath(_m_configpath),m_owner(_owner)
{

	m_SocketError = SOCKET_ERROR;
	Configure_Connect();
 	LoadSocketErrorsMaps();
	//SS std::cout<<"Initializing Socket for "<<m_owner.c_str()<<std::endl;
}


void SocketClient::Ini_Socket_Interface(void)
{

//	XMLUtility::ReadData("ServerAddress",m_ServerName,m_configpath);
	long app;
//	XMLUtility::ReadData("IPPort",app,m_configpath);
	IPPort = app;
//	XMLUtility::ReadData("Page",m_Page,m_configpath);

	long proxyuse_app = 0;
//	XMLUtility::ReadData("UseProxy",proxyuse_app,m_configpath);

	m_ProxyUse = (ProxyUse)proxyuse_app;




	if(m_ProxyUse == NoProxy)
	{
		//AA: Do nothing

	}
	else if (m_ProxyUse == UseProxyWithPasswrd || m_ProxyUse == UseProxyWithoutPasswrd)
	{

		std::string _Proxy;
//		XMLUtility::ReadData("ProxyAddr",_Proxy,m_configpath);

		int offset = _Proxy.find(":");
		if(offset != std::string::npos && !_Proxy.empty())
		{
			m_ProxyAddr = _Proxy.substr(0,offset);
			m_ProxyPort = _Proxy.substr(offset + 1,4);
		}
		if(m_ProxyUse == UseProxyWithPasswrd)
		{
//			XMLUtility::ReadData("UserName",m_UserName,m_configpath);
//			XMLUtility::ReadData("Password",m_Password,m_configpath);
		}
	}
	else
	{//AA: error!!!!!
		m_ProxyUse = NoProxy;
	}

}

bool SocketClient::IsConnected(void)
{
	if(m_Socket == INVALID_SOCKET)
		return false;
	return true;
}
void SocketClient::Configure_Connect(void)
{
	//Configuring socket ...
	Ini_Socket_Interface();  //Load Server name and Page
	//AA:
	// setsockopt(....)
	//TO VERIFY!!!!!!
	m_Socket = INVALID_SOCKET;
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if ( !IsError(m_Socket) )
	{
	/*  From-->http://publib.boulder.ibm.com/infocenter/systems/index.jsp?topic=/com.ibm.aix.progcomm/doc/progcomc/skt_xfr.htm
        Sockets can be set to either blocking or nonblocking I/O mode. The FIONBIO ioctl operation is used to determine this mode. When the FIONBIO ioctl is set,
        the socket is marked nonblocking. If a read is tried and the desired data is not available, the socket does not wait for the data to become available,
        but returns immediately with the EWOULDBLOCK error code.
        Note: The EWOULDBLOCK error code is defined with the _BSD define and is equivalent to the EAGAIN error code.
        When the FIONBIO ioctl is not set, the socket is in blocking mode. In this mode, if a read is tried and the desired data is not available, the calling process waits for the data.
        Similarly, when writing, if FIONBIO is set and the output queue is full, an attempt to write causes the process to return immediately with an error code of EWOULDBLOCK.
    */
    	int BlockingMode = 0;
		int res = ioctl(m_Socket,FIONBIO,&BlockingMode); //Modalit� non bloccante! Altrimenti il timeout impostato non sarebbe valido (?)

        //AA: Setta timeout di send e rec
        struct timeval timeout_rec_send, t;
        socklen_t  size = sizeof(t);
        timeout_rec_send.tv_sec = TIMEOUT;// seconds
        timeout_rec_send.tv_usec = 0;     // microseconds

		if(IsError( getsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(timeval*)&t,&size)))//; //AA: for DEBUG
        {
//        	std::cout<<"???????????????? HERE 31!!!!!!!!!!!!!"<<std::endl;
        	int mydebug_2 = errno;
        	int pippo = 0;
        }
		if(IsError( setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(struct timeval *)&timeout_rec_send, sizeof(struct timeval))))//;
        {
  //      	std::cout<<"???????????????? HERE 32!!!!!!!!!!!!!"<<std::endl;
        	int mydebug_2 = errno;
        	int pippo = 0;
        }
        if(IsError( getsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(timeval*)&t,&size)))//; //AA: for DEBUG
  		{
  //      	std::cout<<"???????????????? HERE 33!!!!!!!!!!!!!"<<std::endl;
        	int mydebug_2 = errno;
        	int pippo = 0;
        }
        if(IsError( getsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(timeval*)&t,&size)))//; //AA: for DEBUG
        {
   //     	std::cout<<"???????????????? HERE 34!!!!!!!!!!!!!"<<std::endl;
        	int mydebug_2 = errno;
        	int pippo = 0;
        }
        if(IsError( setsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(struct timeval *)&timeout_rec_send, sizeof(struct timeval))))//;
        {
 //       	std::cout<<"???????????????? HERE 35!!!!!!!!!!!!!"<<std::endl;
        	int mydebug_2 = errno;
        	int pippo = 0;
        }
        if(IsError( getsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(timeval*)&t,&size)))//; //AA: for DEBUG
        {
 //       	std::cout<<"???????????????? HERE 35!!!!!!!!!!!!!"<<std::endl;
        	int mydebug_2 = errno;
        	int pippo = 0;
        }

		int reuse = 1;
   		if(IsError( setsockopt(m_Socket,SOL_SOCKET,SO_REUSEADDR,&reuse , sizeof ( reuse ))))
        {
        	//SS std::cout<<"???????????????? ERROR SETTING REUSEADDR IN SocketClient!!!!!!!!!!!!!"<<std::endl;
        	int mydebug_2 = errno;
        	int pippo = 0;
        }

        /*
        if( IsError( getsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(timeval*)&t,&size)))//; //AA: for DEBUG
        	int mydebug = 0;
        IsError( setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(struct timeval *)&timeout_rec_send, sizeof(struct timeval)));
        IsError( getsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(timeval*)&t,&size)); //AA: for DEBUG

        IsError( getsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(timeval*)&t,&size)); //AA: for DEBUG
        IsError( setsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(struct timeval *)&timeout_rec_send, sizeof(struct timeval)));
        IsError( getsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(timeval*)&t,&size)); //AA: for DEBUG
*/
		int set = 1;

		Connect2Srv();
	}
//	else
//	{
//		std::cout<<"???????????????? HERE 30!!!!!!!!!!!!!"<<std::endl;
//	}
}


SocketClient::~SocketClient(void)
{
	Disconnect();
}


void SocketClient::Disconnect(void)
{
	if ( m_Socket != INVALID_SOCKET)
	{
	    shutdown(m_Socket,SHUT_RDWR);
	    if( true /*close(m_Socket) == -1*/) //AA: ??????????
	    {
	    	int mydebug = 0;
			//SS std::cout<<"ERROR CLOSING SOCKET CLIENT";
		}
		m_Socket = INVALID_SOCKET;
	}
	else
	{

		int mydebug = 0;
		//SS std::cout<<"ERROR CLOSING SOCKET CLIENT";
	}
}
void SocketClient::Connect2Srv(void)
{

	std::string srv_ip;
	if((m_ProxyUse == UseProxyWithPasswrd) || (m_ProxyUse == UseProxyWithoutPasswrd))
	{
		m_SockAddrIn.sin_port = htons( atoi( m_ProxyPort.c_str()));
		m_SockAddrIn.sin_family = AF_INET;
		srv_ip = get_ip(m_ProxyAddr.c_str());
	}
	else
	{
		m_SockAddrIn.sin_port = htons( IPPort );
		m_SockAddrIn.sin_family = AF_INET;
		srv_ip = get_ip(m_ServerName.c_str());
	}
//	m_SockAddrIn.sin_port = htons( IPPort );
//	m_SockAddrIn.sin_family = AF_INET;
//	std::string srv_ip = get_ip(m_ServerName.c_str());

	if (srv_ip.size() == 0)
	{
//		std::cout<<"???????????????? HERE 41!!!!!!!!!!!!!"<<std::endl;
		Disconnect();
		return;	// verificare in caso debug !!!
	}

	if( IsError(inet_pton(AF_INET, srv_ip.c_str(), &m_SockAddrIn.sin_addr) ))
	{
		Disconnect();
		//m_Socket = SOCKET_ERROR;
//		std::cout<<"???????????????? HERE 40!!!!!!!!!!!!!"<<std::endl;
	}
    //AA: inet_pton() returns a negative value and sets errno to EAFNOSUPPORT if af does not contain a valid address family.
    //AA  0 is returned if src does not contain a character string representing a valid network address in the specified address family.


	if( IsError( connect(m_Socket, (struct sockaddr *)&m_SockAddrIn, sizeof(m_SockAddrIn))))
	{
		Disconnect();
//		std::cout<<"???????????????? HERE 7!!!!!!!!!!!!!"<<std::endl;
	}
	int mydebug = 0;

}





std::string SocketClient::BuildPost(const std::string& Message2Post)
{
	std::string msg = "POST http://ssantilli:pama6pefi@proxy:3128/" + m_ServerName +":8080" +m_Page + " HTTP/1.0\r\n";
	//AA std::string msg = "POST http://" + m_ServerName + m_Page + " HTTP/1.0\r\n";
	msg += "Content-Type: text/html\r\n";


	char buffer [20];
	sprintf (buffer, "%d", Message2Post.size());

	msg +="Content-Length: ";
 	msg += buffer;
	msg +="\r\n\r\n";
	msg += Message2Post;
	msg +="\r\n";
	return msg;
}

std::string SocketClient::BuildGet(const std::string& Resource2Upload)
{
	std::string msg = "GET " + Resource2Upload + " HTTP/1.0\r\n\r\n";
	return msg;
}

std::string SocketClient::BuildPostHttp1_1(const std::string& Message2Post)
{

	std::string app_IPPORT = ":" + long2string(IPPort);

	std::string msg;
	if(m_ProxyUse == UseProxyWithPasswrd || m_ProxyUse == UseProxyWithoutPasswrd)
	{//std::string msg = "POST http://alan84.dyndns.org:8080" + m_Page + " HTTP/1.1\r\n";
		msg = "POST http://" + m_ServerName + ":" + long2string(IPPort)  + m_Page + " HTTP/1.1\r\n";

	}
	else
		msg = "POST " + m_Page + " HTTP/1.1\r\n";


	msg += "Content-Type: text/html\r\n";
	msg += "Connection: Close\r\n";


	char buffer [20];
	sprintf (buffer, "%d", Message2Post.size());

	msg +="Content-Length: ";
 	msg += buffer;
 	msg += "\r\n";
 	std::string host = "Host: " + m_ServerName + app_IPPORT;
 	msg += host;


	if(m_ProxyUse == UseProxyWithPasswrd)
	{
		msg += "\r\n";
		std::string ProxyAuthorization = "Proxy-Authorization: "+ basic_authentication_encode(m_UserName.c_str(),m_Password.c_str());
		msg += ProxyAuthorization;
	}


	msg += "\r\n";
	std::string UserAgent = "User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; WOW64; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; OfficeLiveConnector.1.5; OfficeLivePatch.1.3)";
	msg += UserAgent;

	msg += "\r\n";
	std::string Expect = "Expect: 100-continue";
	msg += Expect;

	msg += "\r\n";
	std::string Proxy_Connection = "Proxy-Connection: Keep-Alive";
	msg += Proxy_Connection;

	msg +="\r\n\r\n";
	msg += Message2Post;
	msg +="\r\n";
	return msg;


}


std::string SocketClient::BuildGetHttp1_1(const std::string& Resource2Upload)
{

	std::string _Resource2Upload = Resource2Upload;

	if(m_ProxyUse == UseProxyWithPasswrd || m_ProxyUse == UseProxyWithoutPasswrd)
	{//std::string msg = "POST http://alan84.dyndns.org:8080" + m_Page + " HTTP/1.1\r\n";
		//msg = "POST http://" + m_ServerName + ":" + StrUtility::long2string(IPPort)  + m_Page + " HTTP/1.1\r\n";
		_Resource2Upload = "http://" + m_ServerName + ":" + long2string(IPPort) + Resource2Upload;
	}


	std::string msg = "GET " + _Resource2Upload  + " HTTP/1.1\r\n";

	std::string app_IPPORT = ":" + long2string(IPPort);
	msg += "Connection: Close\r\n";
//	std::string host = "Host: " + m_ServerName + app_IPPORT + "\r\n\r\n";
//	msg += host;

	if(m_ProxyUse == UseProxyWithPasswrd)
	{
		std::string ProxyAuthorization = "Proxy-Authorization: "+ basic_authentication_encode(m_UserName.c_str(),m_Password.c_str());
		msg += ProxyAuthorization;
		msg += "\r\n";
	}

	std::string host = "Host: " + m_ServerName + app_IPPORT + "\r\n\r\n";
	msg += host;


	return msg;
}



bool SocketClient::SocketPost(const std::string& Message2Post, std::string& ReadMsg)
{
    if(m_Socket == SOCKET_ERROR)
        return false;

	fd_set fdsockets;
	FD_ZERO (&fdsockets);
	FD_SET (m_Socket, &fdsockets);

	std::string Message2Write = BuildPostHttp1_1(Message2Post);
	//std::string Message2Write = BuildPost(Message2Post);

	////////////////////////////////////////////////////
	//std::cout<<"POST--->"<< Message2Write.c_str()<<std::endl;
	///////////////////////////////////////////////////

	if(IsError( send (m_Socket, Message2Write.c_str(), Message2Write.length()+1, MSG_NOSIGNAL)))
	{
		Disconnect();
	    //m_Socket = SOCKET_ERROR;
        return false;
	}

	std::string app;
	int bRead = 0;
	char *szBuffer = NULL;
	szBuffer = new char[MAXLENBUF];

	do
	{
		bRead = recv(m_Socket, szBuffer, MAXLENBUF, 0);
		if(IsError(bRead))
		{
			//AA EINTR Interrupted function call. Una funzione di libreria  e` stata interrotta. In genere questo
    		//AA avviene causa di un segnale asincrono al processo che impedisce la conclusione della chia-
    		//AA mata, la funzione ritorna con questo errore una volta che si sia correttamente eseguito il
    		//AA gestore del segnale. In questo caso e` necessario ripetere la chiamata alla funzione.
			if( errno != EINTR	)
			{
	            delete[] szBuffer;
	            ReadMsg = "";
	            return false;
			}
		}
		else
		{
			app.assign(szBuffer,bRead);
			ReadMsg += app;
		}
		Sleep(10);
	} while(bRead != 0);

	delete[] szBuffer;

	int offset = ReadMsg.find("<");
	if (offset != std::string::npos)
		ReadMsg = ReadMsg.substr(offset,ReadMsg.size());
	else
		ReadMsg = "";

	return true;
}


std::string  SocketClient::get_ip(const char *host)
{

	struct hostent *hent;
	int iplen = INET_ADDRSTRLEN; //XXX.XXX.XXX.XXX
	char *ip = (char *)malloc(iplen+1);
	memset(ip, 0, iplen+1);

	if(host == NULL)
		return "";
	if((hent = gethostbyname(host)) == NULL)

	{
		herror("Can't get IP");
		//AA exit(1);
		//AA std::string _ip(_ip);
		return "";
	}

	if(!(hent->h_addr_list[0]))		//SS 5-10-2010
		return "";					//SS 5-10-2010

	if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen ) == NULL)

	{
		perror("Can't resolve host");
		//AA exit(1);
		//AA std::string _ip(_ip);
		return "";
	}

	std::string _ip(ip);
	return _ip;
}



DownloadResult SocketClient::UploadFile(std::string ResourceToDownload,std::string FilePath)
{
	unsigned int WrittenBytes = 0;
	int BlockSize  = 0;
    DownloadResult Err = unknownerror;
	int RecBytes = 0;
	int nUpoadedBytes = 0;


	FILE *streamin;
    streamin = fopen( FilePath.c_str(), ("w+b") );

	if( streamin  != NULL )
	{
		Err = GetResource(streamin,ResourceToDownload.c_str());
        if(Err != noerror)
        {
            fclose ( streamin );
            return Err;
        }

    }
    else
        return badfileopen;

    fclose ( streamin );
	return Err;
}

DownloadResult SocketClient::GetResource(FILE *streamin,std::string Resource2Upload)
{
   if(m_Socket == SOCKET_ERROR)
        return badsocket;

	fd_set fdsockets;
	FD_ZERO (&fdsockets);
	FD_SET (m_Socket, &fdsockets);
	const struct timeval t = {TIMEOUT,0};
	std::string Message2Write = BuildGetHttp1_1(Resource2Upload);
	////////////////////////////////////////////////////
	//std::cout<<"GET--->"<< Message2Write.c_str()<<std::endl;
	///////////////////////////////////////////////////



	if(IsError(send (m_Socket, Message2Write.c_str(), Message2Write.length()+1, MSG_NOSIGNAL)))
	{
        return badsocketsend;
	}

	int bRead = 0;
	char *szBuffer = NULL;
	szBuffer = new char[MAXLENBUF];
    DownloadResult UploadResult = noerror;

	do
	{
		bRead = recv(m_Socket, szBuffer, MAXLENBUF, 0);
		Sleep(20);
		//AA: Test
		std::string toFile;
		if(bRead == -1)
		{
			//AA EINTR Interrupted function call. Una funzione di libreria  e` stata interrotta. In genere questo
    		//AA avviene causa di un segnale asincrono al processo che impedisce la conclusione della chia-
    		//AA mata, la funzione ritorna con questo errore una volta che si sia correttamente eseguito il
    		//AA gestore del segnale. In questo caso e` necessario ripetere la chiamata alla funzione.

            if( errno != EINTR	)
			{
				  delete[] szBuffer;
				  return badsocketrec;
			}
		}
		else
		{
			toFile.assign(szBuffer,bRead);
			//AA: estrazione header dal messaggio
			if (toFile.substr(0,4) == "HTTP")
			{
				int index_1 = toFile.find(" ");
				int index_2 = toFile.find(" ",index_1+1);
				std::string statushttp = toFile.substr(index_1+1,index_2-index_1);
				int status_int = atoi(statushttp.c_str());
				if(status_int != 200)
				{
					std::cout << toFile << "Status " << status_int << std::endl;
					UploadResult=unknownerror;
					break;
				}
				int offset = toFile.find("Content-Length:");
//				if(offset != std::string::npos)
//				{
					int HeaderEnd = toFile.find("\r\n\r\n",0);
					toFile = toFile.substr(HeaderEnd + 4,toFile.size()  - (HeaderEnd + 4));
					unsigned int WrittenBytes;
					WrittenBytes = fwrite( toFile.c_str(), sizeof( char ), toFile.size(), streamin );
					Sleep(20);
					if(toFile.size() != WrittenBytes)
					{
						UploadResult = badfilewrite;
						break;
					}
//				}
			}
			else
			{
				unsigned int WrittenBytes;
				WrittenBytes = fwrite( (void*) szBuffer, sizeof( char ), bRead, streamin );
				Sleep(20);
				if(toFile.size() != WrittenBytes)
				{
					UploadResult = badfilewrite;
					break;
				}
			}
		}
		//Sleep(20);

	} while(bRead != 0);

	delete[] szBuffer;
	return UploadResult;
}

bool SocketClient::IsError(int err/*,std::string& errcode*/)
{
    std::string errcode = "";
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

	//std::cout<<"SOCKET CLIENT ERROR: ";
	//std::cout<<errcode.c_str();//<<std::endl;
	//std::cout<<" ERRNO:";
	//std::cout<<StrUtility::int2string(errno)<<std::endl;
	//std::cout<<"OWNER: ";
	//std::cout<<m_owner.c_str()<<std::endl;
    return true;
}

void SocketClient::LoadSocketErrorsMaps(void)
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


int
SocketClient::base64_encode (const char *str, int length, char *b64store)
{
  /* Conversion table.  */
  static char tbl[64] = {
    'A','B','C','D','E','F','G','H',
    'I','J','K','L','M','N','O','P',
    'Q','R','S','T','U','V','W','X',
    'Y','Z','a','b','c','d','e','f',
    'g','h','i','j','k','l','m','n',
    'o','p','q','r','s','t','u','v',
    'w','x','y','z','0','1','2','3',
    '4','5','6','7','8','9','+','/'
  };
  int i;
  const unsigned char *s = (const unsigned char *) str;
  char *p = b64store;

  /* Transform the 3x8 bits to 4x6 bits, as required by base64.  */
  for (i = 0; i < length; i += 3)
    {
      *p++ = tbl[s[0] >> 2];
      *p++ = tbl[((s[0] & 3) << 4) + (s[1] >> 4)];
      *p++ = tbl[((s[1] & 0xf) << 2) + (s[2] >> 6)];
      *p++ = tbl[s[2] & 0x3f];
      s += 3;
    }

  /* Pad the result if necessary...  */
  if (i == length + 1)
    *(p - 1) = '=';
  else if (i == length + 2)
    *(p - 1) = *(p - 2) = '=';

  /* ...and zero-terminate it.  */
  *p = '\0';

  return p - b64store;
}

std::string SocketClient::basic_authentication_encode (const char *user, const char *passwd)
{
  char *t1, *t2;
  int len1 = strlen (user) + 1 + strlen (passwd);

  t1 = (char *)alloca (len1 + 1);
  sprintf (t1, "%s:%s", user, passwd);

  t2 = (char *)alloca (BASE64_LENGTH (len1) + 1);
  base64_encode (t1, len1, t2);

  std::string ret = "Basic ";
  ret += t2;
  //std::string app = (char *) 0;
  ret += + "\0";

  return ret;// concat_strings ("Basic ", t2, (char *) 0);
}




