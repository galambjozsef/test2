#ifndef _SOCKETCLIENT
#define _SOCKETCLIENT

#include <string>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//#include "Global.h"


#include <map>
#include <vector>



#define SOCKET_OK 0
#define SOCKET_ERROR -1

typedef enum {
    badfileopen = -6,
    badfilewrite = -5,
    badfilesize = -4,
    badsocketsend = -3,
    badsocketrec = -2,
    badsocket = -1,
    unknownerror = 0,
    noerror = 1,
    allfilesdownloaded = 2
}DownloadResult;
#define	INVALID_SOCKET -1

/* How many bytes it will take to store LEN bytes in base64.  */
#define BASE64_LENGTH(len) (4 * (((len) + 2) / 3))
# define VA_START(args, arg1) va_start (args, arg1)
#define countof(array) (sizeof (array) / sizeof ((array)[0]))

//typedef enum {
//	badfileopen = -6,
//	badfilewrite = -5,
//	badfilesize = -4,
//	badsocketsend = -3,
//	badsocketrec = -2,
//	badsocket = -1,
//	unknownerror = 0,
//	noerror = 1,
//} ErrUpLoad ;


typedef enum {
	NoProxy = 0,
	UseProxyWithPasswrd = 1,
	UseProxyWithoutPasswrd = 2,
} ProxyUse ;



class SocketClient
{
public:
	SocketClient(int maxlen);
	SocketClient(int maxlen, int timeout,std::string _m_configpath );
	SocketClient(int maxlen, int timeout,std::string _m_configpath ,std::string _owner,bool IsUploader = false );
	virtual ~SocketClient(void);

	//const
	int MAXLENBUF;
	const int TIMEOUT;
	int m_SocketError;
	virtual void Configure_Connect();
	void Disconnect(void);
	bool SocketPost(const std::string& Message2Post, std::string& ReadMsg);
    DownloadResult UploadFile(std::string ResourceToDownload,std::string FilePath);
	std::string m_ServerName;
	unsigned short	IPPort;

	enum{
			Err_Codes = 45
	};

	// Error Code || Description
    std::map<int, std::string> SocketErrorsMap;

	void Connect2Srv(void);
    std::string BuildGet(const std::string& Resource2Upload);
    std::string BuildPost(const std::string& Message2Post);
	std::string BuildGetHttp1_1(const std::string& Resource2Upload);
	std::string BuildPostHttp1_1(const std::string& Message2Post);

    DownloadResult GetResource(FILE *streamin, std::string file);

	std::string m_configpath;

	std::string m_Page;
	//bool m_UseProxy;
	std::string m_ProxyAddr;
	std::string m_ProxyPort;

	void LoadSocketErrorsMaps(void);
	bool IsError(int err/*,std::string& errcode*/);
    virtual void Ini_Socket_Interface(void);
    struct sockaddr_in m_SockAddrIn;
    bool IsConnected(void);
    ProxyUse m_ProxyUse;
    std::string m_UserName;
    std::string m_Password;

    void SetBufferDim(long _bufdim)
    {
    	MAXLENBUF = _bufdim;
	}

private:
	std::string get_ip(const char *host);
	std::string m_owner;

	int base64_encode (const char *str, int length, char *b64store);
	std::string basic_authentication_encode (const char *user, const char *passwd);


public:
	int m_Socket;
	unsigned int m_timeout;



};

#endif
