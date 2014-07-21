#ifndef _SIGEXECPT
#define _SIGEXECPT
#include <execinfo.h>
#include <signal.h>


#include <exception>
#include <iostream>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "GenericException.h"

using namespace std;

bool alreadyLogged = false;




template <class SignalExceptionClass> class SignalTranslator
{
private:
	class SingleTonTranslator
	{
	public:
		SingleTonTranslator()
		{
			signal(SignalExceptionClass::GetSignalNumber(), SignalHandler);
		}

		static void SignalHandler(int Signum)
		{			
			throw SignalExceptionClass();
		}
	};

public:
	SignalTranslator()
	{
		static SingleTonTranslator s_objTranslator;
	}
 };

//Build Translators
SignalTranslator<SegmentationFault> SegmentationFaultTranslator;
SignalTranslator<FloatingPointException> FloatingPointExceptionTranslator;
SignalTranslator<IllecitInstructionException>IllecitInstructionExceptionTranslator;
SignalTranslator<BrokenPipeException>BrokenPipeExceptionExceptionTranslator;
SignalTranslator<BusErrorException>BusErrorExceptionTranslator;
SignalTranslator<WrongArgumentException>Translator;
SignalTranslator<CpuTimeLimitException>CpuTimeLimitExceptionExceptionTranslator;
SignalTranslator<FileLimitException>FileLimitExceptionTranslator;
SignalTranslator<IotTrapException>IotTrapExceptionTranslator;
SignalTranslator<StackErrorException>StackErrorExceptionTranslator;
SignalTranslator<SignalIOException>SignalIOExceptionTranslator;

SignalTranslator<SignalPwrException>SignalPwrExceptionTranslator;





class ExceptionHandler
{
public:

	ExceptionHandler()
	{
		static SingleTonHandler s_objHandler;
	}

private:
	class SingleTonHandler
	{
	public:		
		SingleTonHandler()
		{			
			set_terminate(Handler);
		}

		static void Handler()
		{
			// Exception from construction/destruction of global variables
			try
			{
				// re-throw
				throw;
			}


			catch(BaseSignalException& bse)
			{
				std::string logString("Exception Type: ");
				logString.append(bse.GetExceptionName());
				logString.append(" PID of the assertion: ").append(int2string(bse.GetPID()));
				if(!alreadyLogged)
				{
//					XMLUtility::LogData("LOG",logString, LOG_EXCEPTION_PATH, Technogym::ExceptionsDefs::MaxLogFileEntries );
					std::cout<<logString<<std::endl;
					alreadyLogged = true;
				}
			}
			catch (std::exception& exc)
			{
				std::string  logString("Standard Exception: The Thread That caused the assertion: ");
				if(!alreadyLogged)
				{
//					XMLUtility::LogData("LOG",logString, LOG_EXCEPTION_PATH, Technogym::ExceptionsDefs::MaxLogFileEntries );
					std::cout<<logString<<std::endl;
					alreadyLogged = true;
				}

			}
			catch (...)
			{
				GenericException ge;
				cout << "Unknown Exception" << endl;
				std::string logString("Unknown Exception: The Thread That caused the assertion: ");
				logString.append(" PID of the assertion: ").append(int2string(ge.m_PID));
				if(!alreadyLogged)
				{
					//std::string LogUserName = Register::GetProperty("LogUserName");
//					XMLUtility::LogData("LOG",logString, LOG_EXCEPTION_PATH, Technogym::ExceptionsDefs::MaxLogFileEntries );
					std::cout<<logString<<std::endl;
					alreadyLogged = true;
				}
			}


			abort();
		}
	};

};



#endif
