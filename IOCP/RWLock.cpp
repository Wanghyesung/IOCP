#include "pch.h"
#include "RWLock.h"
#include "GThread.h"

/*
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPINE_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,//���� 16��Ʈ
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
	//���� �̹� �������̶��
	if (T_Thread_ID == (m_lockFlag.load() & WRITE_THREAD_MASK) >> 16)
	{
		++m_iWriteCount;
		return;
	}

	//���� ó������ ���� �䱸��
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
		//������ �а� ����
		assert(nullptr);
	}

	if (--m_iWriteCount == 0)
	{
		m_lockFlag.store(EMPTY_FLAG);
	}
}

void RWLock::ReadLock()
{
	//���� ������ �����尡 �б⸦ ���ٸ� ���
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
			//���� ; ������ write�� ���� ���� ����
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
	//�̹� 1�� ���� �� ���� 0�� ��Ȳ
	if (m_lockFlag.fetch_sub(1) == 0)
	{
		assert(nullptr);
	}
}
