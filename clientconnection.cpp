#include "clientconnection.h"
#include "utility.h"
//#include "FileTar.h"
#include "timestamp.h"
#include <vector>
#include <string.h>
#include <stdio.h>


#include <fcntl.h>
#include <unistd.h>
#include <iostream>


#define KILL 9
#define NBUFFSIZE 1024*80

#include <signal.h>
#include "criticalsectionlock.h"
//#include "GetHttpResource.h"

static int ConnectionCnt;

ClientConnection::ClientConnection(SocketServer* TheServer,IProxy* _IProxy,int _Socket,std::string _BasePath):m_Socket(_Socket),m_Proxy(_IProxy),m_BasePath(_BasePath)
{
	m_TheServer = TheServer;
	IsActive = false;
	m_isPersistent = false;
}

ClientConnection::ClientConnection(SocketServer* TheServer,IProxy* _IProxy,std::string _BasePath):m_Proxy(_IProxy),m_BasePath(_BasePath)
{
	m_busy = false;
	m_TheServer = TheServer;
	m_isPersistent = false;
	IsActive = false;
	std::string threadname = "ClientConnection";	
	ConnectionCnt++;
}
ClientConnection::~ClientConnection()
{
	int mydebug =0;
}

static int pippo = 0;

void ClientConnection::Main()
{
	std::string err;
	std::string in;

//	ThreadMapMan::InsertThreadInfoOnMap(GetThreadInfo());


	this->SetScheduling(50);
	for(;;)
	{
		err.clear();
		in.clear();

		if(m_isPersistent)
		{
			if(ReadClnt(err,in))
			{
				std::cout<<"PERMANENT SOCKET-----> "<<in.c_str()<<std::endl;
//				m_FlashProxy->ManageFlashCmd(in);
				m_Proxy->ProcessCommIn(in); //AA: quà dovrebbe avvenire la gestione dei messaggi verso l'esterno !!!!
			}
			else
			{	//Errore nella connessione della Persistent Socket ...
				std::cout<<"Error PERMANENT SOCKET"<<std::endl;
				m_isPersistent = false;
				m_busy = false;
				close(m_Socket);
			}
		}
		else
		{

			if(((!m_busy) && IsActive)  )
			{
				m_busy = true;
				IsActive = false;

				if(ReadClnt(err,in))
				{
					if( ProcessMessage(in))
					{
						close(m_Socket);
//						std::cout<<"Closed Connection n° "<<int2string(m_Socket).c_str()<<std::endl;
						m_busy = false;
					}
					else
					{
						if(InsensitiveSearch(in,"*PERSISTENT_SOCKET*"))
						{
							//AA: a questo punto la connessione è persistente e quì non si torna più!
							m_isPersistent = true;
							WriteClnt( err,"*PERSISTENT_SOCKET*");//AA: sicronizzazione con l'applicazione Flash ...							
						}
					}

				}
				else
				{
//					std::cout<<"Error reading Client"<<std::endl;
					close(m_Socket);
//					std::cout<<"Closed Connection n° "<<int2string(m_Socket).c_str()<<std::endl;
					m_busy = false;
				}
				Sleep(10);
			}


		}
		Sleep(10);
		//AA m_MutexClientConnection.ReleasepMutex();
	}



}
//AA: Evade eventuale richiesta da FLASH del policy file
//    Evade richiest di GET
bool ClientConnection::ProcessMessage(std::string _msg)
{
	if(_msg == "<policy-file-request/>")
	{
		//AA: richiesta policy file da FLASH
		//AA std::cout<<"Sending policy file"<<std::endl;

		if(!writeFile("crossdomain.xml",false))
			std::cout<<"Error Sending policy file!!!!!!!!!!!!!!!1"<<std::endl;
		return true; //disconnette il client


	}
	else if(InsensitiveSearch(_msg,"GET ") && InsensitiveSearch(_msg," HTTP/1.1") )
	{
		TimeStamp elapsed;
		elapsed.SnapShot();
		int offset = _msg.find("/");
		int offset2 = _msg.find("HTTP");
		std::string FilePath = _msg.substr( offset,offset2 -5 );

		if(InsensitiveSearch(_msg,"service?url="))
		{
		    //AA: Never Happens
		    std::string encoded_addr = _msg.substr(_msg.find("service?url=")+12,offset2-18);
		    //std::cout<<_msg << std::endl << "ENCODED ADDRESS!!!!!"<<encoded_addr<<std::endl;
		  //DoDownloadResource(encoded_addr);

		}
		else if( writeFileEx(FilePath,true))
		{
			//SS long2string( elapsed.ElapsedTime());
			//std::cout<<"FILE SENT ! path  = "<< FilePath.c_str()<<std::endl;
			//SS std::string log = "From Socket n°-->" + int2string(m_Socket);
			//SS std::cout<<log.c_str()<<std::endl;
		}
		else
		{
			std::cout<<"ERROR -->FILE NOT SENT !"<< std::endl;
		}
		return true;//disconnette il client
	}
	else if(InsensitiveSearch(_msg,"PUT ") && InsensitiveSearch(_msg," HTTP/1.1") )
	{

	}

	else
	{
		return  false; // non disconnette il client
	}


	return true;
}

/*
bool ClientConnection::DoDownloadResource(std::string msg)
{
    std::string dec_addr;
	std::string in = "aaaa", //lets us work with the first 4 bytes
				out = "aaa"; // -"- 3 bytes


	unsigned int    msgsize        = msg.size(),
					processbytes   = 4,
					fillbytes      = 0;

	dec_addr.clear();
	dec_addr.reserve((msgsize)/4/3+1);

	if(msgsize%4 != 0)
	{
		return false;
	}

	for(unsigned int i = msgsize-1; i > msgsize-3; i--)
	{
		if(msg[i] == '=')
			fillbytes++;
		else
			break;
	}

	for(unsigned int i = 0; processbytes == 4; i += processbytes)
	{
		if(msgsize - i - fillbytes < 4)
		{
			processbytes = msgsize - i - fillbytes;
			if(!processbytes)
				break;
		}

		in = msg.substr(i, processbytes);

		for(unsigned int j = 0; j < processbytes; j++)
		{
			if(in[j] >= 'A' and in[j] <= 'Z')
				in[j] -= 'A';
			else if(in[j] >= 'a' and in[j] <= 'z')
				in[j] -= 'a'-26;
			else if(in[j] >= '0' and in[j] <= '9')
				in[j] -= '0'-52;
			else if(in[j] == '+')
				in[j] = 62;
			else if(in[j] == '/')
				in[j] = 63;
			else if(in[j] == '=')
				in[j] = 0;
			else
				return false;
		}

		out[0] = ((in[0]<<2)&252) + ((in[1]>>4)&3);
		out[1] = ((in[1]<<4)&240) + ((in[2]>>2)&15);
		out[2] = ((in[2]<<6)&192) + ((in[3])&63);

		dec_addr.append(out.substr(0, processbytes-1));
	}

	//std::cout << "ClientConnection::DoDownloadResource " << dec_addr << std::endl;
    const int buf_max_len = 1024*100;
    GetHTTPResource* get_resource = new GetHTTPResource(dec_addr,buf_max_len);

    char *szBuffer = new char[buf_max_len];

	int read = 0;

	do
	{
	    get_resource->GetTheResourceContent(szBuffer,&read);
	    std::string err;
	    WriteSlotToClnt(err,szBuffer,read);
	    //std::cout << "ciclo: " << read << std::endl;
	}
	while(read!=0);

	delete[] szBuffer;
	delete get_resource;
}
*/
bool ClientConnection::WriteClnt(std::string err,const std::string& ToClient)
{
	std::string SrvOutPut = ToClient;
	SrvOutPut = SrvOutPut + "\0";

	int n = send (m_Socket, SrvOutPut.c_str(),SrvOutPut.size() + 1, MSG_NOSIGNAL);

//  	std::cout<<"Written to socket n° --->";
//	std::cout<<int2string(m_Socket).c_str()<<std::endl;

	if( m_TheServer->IsError(n,err))
	{
		std::cout<<"5 Written to socket n° --->ERROR"<<std::endl;
		return false;
	}
	return true;
}

bool ClientConnection::WriteSlotToClnt(std::string& err,const char* ToClient,int size)
{
	CriticalSectionLock l( guard ) ;

//	std::cout<<"WriteSlotToClnt ToClient --->"<<ToClient<<" size="<<size<<std::endl;

	//SS modificato in data 20-9-2010 int n = send (m_Socket,ToClient,size, MSG_NOSIGNAL);
	int n = 0;
	int res;
	while(n < size)
	{
		res = send (m_Socket,ToClient+n,size-n, MSG_NOSIGNAL);
		if(res < 0)
		{
		    m_TheServer->IsError(res,err);
		    std::cout<<"1 Written to socket n° --->ERROR " << res <<" n = "<<n<<std::endl;
		    return false;
		}
		n += res;
		//SS 18-11-2010 Sleep(10);
//		std::cout<<"Written to socket n° --->"<<n<<std::endl;
		Sleep(2);
	}

	std::cout<<"WriteSlotToClnt res --->"<<res<<std::endl;
	std::cout<<int2string(m_Socket).c_str()<<std::endl;

//	if( m_TheServer->IsError(res,err))
//	{
//		 std::cout<<"Written to socket n° --->ERROR"<<std::endl;
//		return false;
//	}
	return true;
}


bool ClientConnection::RestartClient(int _Socket)
{
	if(IsActive )
	{
		return false;
	}

	m_Socket = _Socket;
	m_isPersistent = false;
	IsActive = true;

	return true;
}

bool ClientConnection::ReadClnt(std::string err,std::string& FromClient)
{
	FromClient = "";
	char *szBuffer = NULL;
	szBuffer = new char[m_TheServer->MAXLENBUF];
	bzero(szBuffer,m_TheServer->MAXLENBUF);
	int n = read(m_Socket,szBuffer,m_TheServer->MAXLENBUF);
	if(n == 0)
	{
		err = "Unknown error !!!";
		return false;
	}
	if( m_TheServer->IsError(n,err))
	{
		delete[] szBuffer;
		return false;
	}

	FromClient = szBuffer;
	delete[] szBuffer;
	return true;
}

bool ClientConnection:: writeFile(std::string file, bool headerPresent)
{
	CriticalSectionLock l( guard ) ;
	if(file.size() == 0)
	{
		std::cout<<"Error File dimension is 0!!!!"<<std::endl;
		return false;
	}

	long filedim;
//	std::string BasePath = "/technogym/usb_disk/";
//m_BasePath
	if(file.find("/") == 0)
		file = file.substr(1,file.size() - 1);

	file = m_BasePath + file;
	GetFileDim(file,filedim);
	int start = file.find(".");
	std::string fileExt = file.substr(start+1,file.size()-(start+1));

//	std::cout<<"Opening file: "<< file.c_str();

	std::string	header = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: "+getContentType(fileExt)+"\r\nContent-Length: "
						 +long2string(filedim)+"\r\n\r\n";

	if(!headerPresent)header = "";

	std::string bb;
	for(int i = 0;i<header.size();i++)
	{
		bb.push_back(header[i]);
	}
	if(!LoadBufferFromFile(file,bb))
		return false;

	std::string err;
	return WriteClnt( err,bb);
}

bool ClientConnection::writeFileEx(std::string file, bool headerPresent)
{
	CriticalSectionLock l( guard ) ;
	if(file.size() == 0)
	{
		std::cout<<"Error File dimension is 0!!!!"<<std::endl;
		return false;
	}

	long filedim;


	if(file.find("/") == 0)
		file = file.substr(1,file.size() - 1);

	file = m_BasePath + file;
	GetFileDim(file,filedim);
	filedim -= 2;

	int start = file.find(".");
	std::string fileExt = file.substr(start+1,file.size()-(start+1));

	std::cout<<"Opening file: "<< file.c_str()<< std::endl;

	std::string header;

	if(filedim)
	  header = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: "+getContentType(fileExt)+"\r\nContent-Length: "
						   +long2string(filedim)+"\r\n\r\n";
	else
	  header = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: "+getContentType(fileExt)+"\r\nContent-Length: "
						   +"\r\n\r\n";

	if(!headerPresent)header = "";

	std::string err;
	if( !WriteSlotToClnt(err,header.c_str(),header.size()))
		return false;

	//return WriteBufferFromFile(file);
	readAndSendFileKorg(file);
}


bool ClientConnection::readAndSendFileKorg(std::string path)
{

  ssize_t res = NBUFFSIZE;
  ssize_t bRead = 0;
  std::string err;

  char buffer[NBUFFSIZE];
  int device_fd = open((const char*)path.c_str(), O_RDWR);

  do
  {
      memset(buffer,0,NBUFFSIZE);

      ssize_t bytesToRead = NBUFFSIZE;
      res = pread(device_fd, (char*)buffer, bytesToRead, bRead);
      bRead += res;
      std::cout<<"res = "<<res<<" bRead = "<<bRead<<std::endl;

      if(res == -1)
      {
          printf("error reading !\n");
          return false;
      }


      if(res > 0)
      {

          std::string app(buffer,res);

          std::cout<<"Read String  =  "<<app<<std::endl;
          if (!WriteSlotToClnt(err,buffer,res))
          {
              std::cout<<"2 Written to socket n° --->ERROR " << err <<std::endl;
              close(device_fd);
              return false;
          }
      }

  }while (res);


  if( !WriteClnt( err,""))	//AA: aggiunge '\0' alla fine del file trasmesso !!!!!!!!!!!!!
  {
       std::cout<<"3 Written to socket n° --->ERROR " << err <<std::endl;
       return false;
  }

  std::cout<<std::endl;
//  std::cout<<"Read String  =  "<<readString<<std::endl;
  std::cout<<std::endl;

  std::cout<<std::endl;
  std::cout<<"device_fd =  "<<device_fd<<std::endl;

  close(device_fd);
  return true;
}


bool ClientConnection::WriteBufferFromFile(std::string _file)
{
	CriticalSectionLock l( guard ) ;

	FILE * streamin = fopen(_file.c_str(), ("rb") );
	if( streamin  == NULL )
	{
		std::cout<<"Error reading file: "<<_file.c_str()<<std::endl;
		return false;
	}
	fseek(streamin,0,SEEK_END);

	long filedim = ftell(streamin);

	std::cout << _file << " FILE DIMENSION: " << filedim << std::endl;
	fseek(streamin,0,SEEK_SET);

	char* buff = new char[_NBUFFSIZE];
	memset(buff,0,_NBUFFSIZE);
	std::string err;
	long n = 0;
	while(n < filedim)
	{
        long read = fread( buff, sizeof( char ), _NBUFFSIZE, streamin );

        //std::cout << "READ: " << read;
        if (!WriteSlotToClnt(err,buff,read))
        {
            std::cout<<"4 Written to socket n° --->ERROR " << err <<std::endl;
            delete[] buff;
            fclose( streamin );
            return false;
        }

        //std::cout << std::endl;
        n += read;
        Sleep(2);
	}
    delete[] buff;
    fclose( streamin );


    if( !WriteClnt( err,""))	//AA: aggiunge '\0' alla fine del file trasmesso !!!!!!!!!!!!!
            return false;

    return true;

}


bool ClientConnection::LoadBufferFromFile(std::string _file,std::string& _bb)
{
	FILE * streamin = fopen(_file.c_str(), ("r+b") );
	if( streamin  == NULL )
	{
		std::cout<<"Error reading file: "<<_file.c_str()<<std::endl;
		return false;
	}
	fseek(streamin,0,SEEK_SET);

	long rem,filedim;
	char* buff;
	buff = new char[NBUFFSIZE];
	memset(buff,0,NBUFFSIZE);

	GetFileDim(_file,filedim);
	filedim -= 2; //??
	rem=filedim;

	while(rem > NBUFFSIZE)
	{
		fread( buff, sizeof( char ), NBUFFSIZE, streamin );

		for(int i = 0;i<NBUFFSIZE;i++)
		{
			_bb.push_back(buff[i]);
		}
		rem -= NBUFFSIZE;
	}
	if(rem>0)
	{
		fread( buff, sizeof( char ), rem, streamin );
		for(int i = 0;i<rem;i++)
		{
			_bb.push_back(buff[i]);
		}

	}
	delete[] buff;
	if(streamin)
	fclose( streamin );
	return true;
}


std::string ClientConnection::getContentType(std::string ext)
{

//	ext = ext.trim();
	if(InsensitiveSearch(ext,"xml"))return "text/xml; charset=UTF-8";
	if(InsensitiveSearch(ext,"swf"))return "application/x-shockwave-flash";
	if(InsensitiveSearch(ext,"png"))return "image/jpeg";
	if(InsensitiveSearch(ext,"jpg"))return "image/jpeg";
	if(InsensitiveSearch(ext,"gif"))return "image/jpeg";
	if(InsensitiveSearch(ext,"bmp"))return "image/jpeg";
	if(InsensitiveSearch(ext,"flv"))return "video/x-flv";
	if(InsensitiveSearch(ext,"pdf"))return "application/pdf";


	return "text/html";
}




