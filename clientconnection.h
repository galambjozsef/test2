#ifndef CLNTCONNTHREAD_
#define CLNTCONNTHREAD_

#include "thread.h"
#include "socketserver.h"
#include "iproxy.h"
#define _NBUFFSIZE	1024*80

#include "criticalsection.h"


class SocketServer;

class ClientConnection: public Thread
{
public:
	ClientConnection(SocketServer* TheServer,IProxy* _IProxy,std::string _BasePath);
	ClientConnection(SocketServer* TheServer,IProxy* _IProxy,int _Socket,std::string _BasePath);
	virtual ~ClientConnection();
	virtual void Main();

	SocketServer* m_TheServer;
	bool ProcessMessage(std::string _msg);
	bool RestartClient(int _Socket);




	bool writeFile(std::string file, bool headerPresent);
	bool LoadBufferFromFile(std::string _file,std::string& _bb);
	bool WriteBufferFromFile(std::string _file);
	bool IsActive;


	bool writeFileEx(std::string file, bool headerPresent);
	bool WriteClnt(std::string err,const std::string& ToClient);
	bool WriteSlotToClnt(std::string& err,const char* ToClient,int size);
	bool ReadClnt(std::string err,std::string& FromClient);
	int m_Socket;
	bool m_busy;
	bool isPersistent()
	{
	  return m_isPersistent;
	}
private:
	CriticalSection guard ;
	//bool DoDownloadResource(std::string enc_addr);

	std::string getContentType(std::string ext);
	//int m_Socket;
	const std::string m_BasePath;
	IProxy* m_Proxy;
	bool m_isPersistent;

	bool readAndSendFileKorg(std::string path);



};

#endif
