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
		ThreadMgr = new ThreadManager();
		MemoryPoolMgr = new MemoryPool();
		SockHelper::init();
	}
	~Global()
	{
		delete ThreadMgr;
		delete MemoryPoolMgr;
		SockHelper::Clear();
	}

}GGlobal;