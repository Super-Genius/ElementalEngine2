#pragma once

// A non-library specific implementation of an automatically locking/unlocking critical section handler
// Suggested Usage:  std:auto_ptr<CAutoLock> lock(&critsec);
// Once that statement exists, the lock will automatically clear if an exception occurs or any other thing
// causes the function to exit.
// call lock.reset(); to unlock.
class CAutoLock
{
protected:
	CAutoLock(void) { m_critSec = NULL ; }
	CRITICAL_SECTION *m_critSec;
public:

	CAutoLock(CRITICAL_SECTION *sec)
	{
		EnterCriticalSection(sec);
		m_critSec = sec;
	}

	~CAutoLock(void)
	{
		if (m_critSec) LeaveCriticalSection(m_critSec);
		m_critSec = NULL;
	}
};

typedef std::auto_ptr<CAutoLock> AutoLockedHandle;
#define AUTOLOCK(X,Y) AutoLockedHandle X(new CAutoLock(Y))