#include "pch.h"
#include "RWLock.h"
#include "GThread.h"

/*
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPINE_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,//상위 16비트
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
*/

RWLock::RWLock():
	m_iWriteCount(0)
{

}

RWLock::~RWLock()
{

}

void RWLock::WriteLock()
{
	//내가 이미 소유중이라면
	if (T_Thread_ID == (m_lockFlag.load() & WRITE_THREAD_MASK) >> 16)
	{
		++m_iWriteCount;
		return;
	}

	//내가 처음으로 들어가는 요구값
	int iDesired = (T_Thread_ID << 16) & WRITE_THREAD_MASK;

	DWORD lBeingTime = GetTickCount();


	while (true)
	{
		for (int i = 0; i < MAX_SPINE_COUNT; ++i)
		{
			int iExpecte = EMPTY_FLAG;
			if (m_lockFlag.compare_exchange_strong(iExpecte, iDesired))
			{
				++m_iWriteCount;
				return;
			}
		}

		DWORD lEndTime = GetTickCount();

		if (lEndTime - lBeingTime >= ACQUIRE_TIMEOUT_TICK)
			assert(nullptr);

		this_thread::yield();
	}

}

void RWLock::UnWriteLock()
{
	if ((m_lockFlag.load() & READ_COUNT_MASK) != 0)
	{
		//누군가 읽고 있음
		assert(nullptr);
	}

	if (--m_iWriteCount == 0)
	{
		m_lockFlag.store(EMPTY_FLAG);
	}
}

void RWLock::ReadLock()
{
	//현재 동일한 스레드가 읽기를 쓴다면 허락
	if (T_Thread_ID == (m_lockFlag.load() & WRITE_THREAD_MASK) >> 16)
	{
		m_lockFlag.fetch_add(1);
		return;
	}

	DWORD lBeingTime = GetTickCount();
	while (true)
	{
		for (int i = 0; i < MAX_SPINE_COUNT; ++i)
		{
			//예상값 ; 나무도 write를 하지 않은 상태
			int iExpected = (m_lockFlag.load() & READ_COUNT_MASK);
			if (m_lockFlag.compare_exchange_strong(iExpected, iExpected + 1))
				return;
		}

		DWORD lEndTime = GetTickCount();

		if (lEndTime - lBeingTime >= ACQUIRE_TIMEOUT_TICK)
			assert(nullptr);

		this_thread::yield();
	}

}

void RWLock::UnReadLock()
{
	//이미 1을 빼기 전 값이 0인 상황
	if (m_lockFlag.fetch_sub(1) == 0)
	{
		assert(nullptr);
	}
}


//lock_guard
WLock::WLock(RWLock& _pLock) :
	m_RWLock(_pLock)
{
	m_RWLock.WriteLock();
}

//WriteLock
WLock::~WLock()
{
	m_RWLock.UnWriteLock();
}


RLock::RLock(RWLock& _pLock) :
	m_RWLock(_pLock)
{
	m_RWLock.ReadLock();
}

RLock::~RLock()
{
	m_RWLock.UnReadLock();
}
