#ifndef I_FLASH_PROXY_
#define I_FLASH_PROXY_

#include <string>




class IProxy
{

public:

	IProxy() {}
	virtual ~IProxy() {}
	virtual bool IsBusy() { return true; }

	virtual void ProcessCommOut(std::string _cmd){}
	virtual void ProcessCommIn(std::string _cmd){}
};


#if defined(__TRACE_INCLUDES__)
	#pragma message("End of:    " __FILE__)
#endif

#endif
