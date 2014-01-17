#ifndef PSEMAPHORE_H
#define PSEMAPHORE_H

#include <iostream>
#include <semaphore.h>


class pSemaphore
{
public:
	pSemaphore(const char*,int=0);
	virtual ~pSemaphore();

	bool Wait(int timeout);
	bool WaitEx(int msec_timeout);
	bool WaitInfinite();
	bool Unlock();
	void UnLink();

private:
	sem_t *semaphore;
	std::string m_name;
};


class pThreadSemaphore
{
public:
	pThreadSemaphore(const char*,int=0);
	virtual ~pThreadSemaphore();

	bool Wait(int timeout);

	bool Unlock();
	void Delete();

private:
	sem_t *semaphore;
	std::string m_name;
};

#endif // PSEMAPHORE_H
