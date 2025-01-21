#include "pch.h"
#include "Global.h"
#include "MemoryPool.h"
#include "SockHelper.h"
#include "ThreadManager.h"

MemoryPool* MemoryPoolMgr= nullptr;
ThreadManager* ThreadMgr = nullptr;

class Global
{
public:
	Global()
	{
		SockHelper::init();
		MemoryPoolMgr = new MemoryPool();
		ThreadMgr = new ThreadManager();
	}
	~Global()
	{
		delete MemoryPoolMgr;
		delete ThreadMgr;
	}

}GGlobal;