#include "timestamp.h"



TimeStamp::TimeStamp()
{
    //per non oltrepassare i limiti del int64 tolgo il numero di secondi dal gennaio 1970
    gettimeofday(&tv,0);
    m_seconds_since_01011970=tv.tv_sec;

    InternalSnapShot();
}

TimeStamp::~TimeStamp()
{
}

void TimeStamp::SnapShot()
{
    InternalSnapShot();
}



// ritorna il tempo trascorso dall'ultimo snapshot  in ms
int64_t TimeStamp::ElapsedTime()
{
    int64_t sysTime;
    int64_t dwlDiff;

    gettimeofday(&tv,0);
    sysTime=(tv.tv_sec-m_seconds_since_01011970)*1000 + tv.tv_usec/1000;
    dwlDiff=sysTime-m_ulTimeStamp-m_ulTimeStampDeltaDisable;

    return (dwlDiff);  // in msec
}


/* RET.VALUE: true se � gi� trascorso il periodo specificato
              false altrimenti.
*/
bool TimeStamp::IsElapsed(int64_t ulPeriodMSecs)
{
    int64_t sysTime;
    int64_t dwlDiff;

//	sysTime=SDL_GetTicks();
//	dwlDiff=sysTime-m_ulTimeStamp;
//	return (dwlDiff >= ulPeriodMSecs );
    gettimeofday(&tv,0);
    sysTime=(tv.tv_sec-m_seconds_since_01011970)*1000 + tv.tv_usec/1000;
    dwlDiff=sysTime-m_ulTimeStamp-m_ulTimeStampDeltaDisable;
    if(dwlDiff < 0)
    {
            InternalSnapShot();
            return true;
    }
    return (dwlDiff >= ulPeriodMSecs );
}


// Fa ripartire il timeout sulla base della durata impostata.
void TimeStamp::InternalSnapShot()
{
    m_enabled=true;
    m_ulTimeStampDisable=0;
    m_ulTimeStampDeltaDisable=0;

    gettimeofday(&tv,0);
    m_ulTimeStamp=(tv.tv_sec-m_seconds_since_01011970)*1000 + tv.tv_usec/1000;
}


void TimeStamp::Restart()
{
    if(m_enabled==false)
    {
        gettimeofday(&tv,0);
        int64_t tmp=(tv.tv_sec-m_seconds_since_01011970)*1000 + tv.tv_usec/1000;
        m_ulTimeStampDeltaDisable += (tmp-m_ulTimeStampDisable);
        m_enabled=true;
    }
}
void TimeStamp::Pause()
{
    if(m_enabled==true)
    {
        gettimeofday(&tv,0);
        int64_t tmp=(tv.tv_sec-m_seconds_since_01011970)*1000 + tv.tv_usec/1000;
        m_ulTimeStampDisable=tmp;
        m_enabled=false;
    }
}
