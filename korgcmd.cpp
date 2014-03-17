#include "korgcmd.h"
//#include "EventSource.h"

KorgCmd::KorgCmd(std::string cmdtype,std::vector<std::string> cmdargs):cmdType(cmdtype),cmdArgs(cmdargs)
{
}

KorgCmd::~KorgCmd()
{
}







//void Event :: Fire( EventSource& s ) const
//{
//	s.Dispatch( *this ) ;
//}

KorgCmd* KorgCmd :: Clone() const
{
        return( new KorgCmd( *this ) ) ;
}

//void Event::SerializeIn(std::string)
//{
//}

//std::string Event::SerializeOut(void) const
//{
//	std::string result ;
//	result="";
//	return result ;
//}

