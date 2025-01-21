#pragma once
//[0~ 15 write, 16~31 read]

class RWLock
{
	enum : int
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPINE_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,//상위 16비트
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};


public:
	RWLock();
	~RWLock();

	void WriteLock();
	void UnWriteLock();

	void ReadLock();
	void UnReadLock();


private:
	atomic<int> m_lockFlag;
	int m_iWriteCount; //재귀 카운트

};

