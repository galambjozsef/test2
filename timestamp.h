#ifndef TIMESTAMP_H
#define TIMESTAMP_H


#include <sys/types.h>
#include <sys/time.h>

class TimeStamp
{
public :
  TimeStamp();
  ~TimeStamp();

  void	SnapShot(void);
  int64_t ElapsedTime(void);
  bool	IsElapsed(int64_t ulPeriodMSecs);

  bool	IsEnable(void) {return m_enabled;}
  void	Disabled(void) {m_enabled=false;}
  void	Pause();
  void	Restart();

protected :
  void	InternalSnapShot(void);
  int64_t m_ulTimeStamp;
  int64_t m_ulTimeStampDisable;
  int64_t m_ulTimeStampDeltaDisable;
private:
  bool m_enabled;
  struct timeval tv;
  int64_t m_seconds_since_01011970;
};
#endif // TIMESTAMP_H
