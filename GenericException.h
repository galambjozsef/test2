#ifndef _GENERICEXECPT
#define _GENERICEXECPT
#include <exception>
#include <iostream>
#include <signal.h>
#include <execinfo.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "utility.h"

#include <dlfcn.h>

#define BACKTRACE_MAXDEPTH 512


static bool alreadyTraced = false;

class ExceptionTracer
{
public:


ExceptionTracer()
{
	 void * array[128];
	 int nSize = backtrace(array, 128);
	 char ** symbols = backtrace_symbols(array, nSize);
	 std::string logString("STACK: ");
	 for (int i = 0; i < nSize; i++)
	 {
		 logString.append(symbols[i]);
                 //qDebug()<<  logString;
	 }
	 if(!alreadyTraced)
	 {
//		XMLUtility::LogData("LOG",logString,LOG_EXCEPTION_PATH,Technogym::ExceptionsDefs::MaxLogFileEntries );
		std::cout<<logString<<std::endl;
		alreadyTraced = true;
	 }
	 free(symbols);
}

};

class GenericException : public ExceptionTracer, public std::exception
{
public:
	GenericException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	GenericException(std::exception& e) : std::exception(e)
	{
		m_PID =  syscall( __NR_gettid);
		std::string logString("STL: ");
		logString.append(e.what());
//		XMLUtility::LogData("LOG",logString, LOG_EXCEPTION_PATH,Technogym::ExceptionsDefs::MaxLogFileEntries );
		std::cout<<logString<<std::endl;

	}
	int m_PID;

};

class BaseSignalException:public ExceptionTracer//, public std::exception

{
public:
	BaseSignalException(){/*int mydebug = 0;*/}
	virtual std::string GetExceptionName(){ return "BaseSignalException";}
	virtual int GetPID(){return 0;}
};

class SegmentationFault : public BaseSignalException
{
public:
	SegmentationFault()
	{
		m_PID =  syscall( __NR_gettid);
		std::cout<<"SegmentationFault()!!!!!!!!!!!!!!!!!"<<"PID--->"<<m_PID<<std::endl;
	}
	static int GetSignalNumber()
	{
		return SIGSEGV;
	}
	std::string GetExceptionName(){ return "SegmentationFault";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class FloatingPointException : public BaseSignalException
{
public:
	FloatingPointException()
	{
		m_PID =  syscall( __NR_gettid);
		std::cout<<"FloatingPointException()!!!!!!!!!!!!!!!!!"<<std::endl;
	}
	static int GetSignalNumber()
	{
		return SIGFPE;
	}
	std::string GetExceptionName(){ return "FloatingPointException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class IllecitInstructionException : public BaseSignalException
{
public:
	IllecitInstructionException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGILL;
	}
	std::string GetExceptionName(){ return "IllecitInstructionException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class BrokenPipeException : public BaseSignalException
{
public:
	BrokenPipeException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGPIPE;
	}
	std::string GetExceptionName(){ return "BrokenPipeException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class BusErrorException : public BaseSignalException
{
public:
	BusErrorException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGBUS;
	}
	std::string GetExceptionName(){ return "BusErrorException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class WrongArgumentException : public BaseSignalException
{
public:
	WrongArgumentException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGSYS;
	}
	std::string GetExceptionName(){ return "WrongArgumentException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class CpuTimeLimitException : public BaseSignalException
{
public:
	CpuTimeLimitException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGXCPU;
	}
	std::string GetExceptionName(){ return "CpuTimeLimitException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class FileLimitException : public BaseSignalException
{
public:
	FileLimitException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGXFSZ;
	}
	std::string GetExceptionName(){ return "FileLimitException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class IotTrapException : public BaseSignalException
{
public:
	IotTrapException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGIOT;
	}
	std::string GetExceptionName(){ return "IotTrapException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class StackErrorException : public BaseSignalException
{
public:
	StackErrorException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGSTKFLT;
	}
	std::string GetExceptionName(){ return "StackErrorException";}
	int GetPID(){return m_PID;}
	int m_PID;
};

class SignalIOException : public BaseSignalException
{
public:
	SignalIOException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGIO;
	}
	std::string GetExceptionName(){ return "SignalIOException";}
	int GetPID(){return m_PID;}
	int m_PID;
};
class SignalPwrException : public BaseSignalException
{
public:
	SignalPwrException()
	{
		m_PID =  syscall( __NR_gettid);
	}
	static int GetSignalNumber()
	{
		return SIGPWR;
	}
	std::string GetExceptionName(){ return "Power Failure";}
	int GetPID(){return m_PID;}
	int m_PID;
};
#endif
