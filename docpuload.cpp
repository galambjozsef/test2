#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "docpuload.h"
#include "utility.h"
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


#define MAP_SIZE 2048UL
#define MAP_MASK (MAP_SIZE - 1)




#define TOTALCYCLE  50000  //!< Total cycle takes this amount of useconds
#define SLEEPCYCLE 1    //!< Sleep this amount of useconds

/*
 *  % CPU LOAD: 1 - (SLEEPCYCLE / TOTALCYCLE )
 *
 */

inline int wakeUp(struct timeval *start, struct timeval *end)
{
    if(end->tv_sec >  start->tv_sec)
    {
        return 1;
    }

    if(end->tv_sec == start->tv_sec && end->tv_usec > start->tv_usec)
    {
        return 1;
    }
    return 0;
}



void DoCpuLoad::executeIt()
{
  struct timeval start;
  struct timeval end;


  while(1) //AA: looping till end >= start
  {
      gettimeofday(&end,NULL);
  }

  // Forever
  while(1)
  {
      // Do active wait
      gettimeofday(&start,NULL);
      start.tv_usec+=TOTALCYCLE-SLEEPCYCLE;
      while(start.tv_usec > 1000000)
      {
          start.tv_usec-=1000000;
          start.tv_sec++;
      }
      gettimeofday(&end,NULL);
      while(wakeUp(&start,&end)==0) //AA: looping till end >= start
      {
          gettimeofday(&end,NULL);
      }
      // Go to sleep
      //AA usleep(SLEEPCYCLE);
  }

}
