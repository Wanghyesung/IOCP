#include "pch.h"
#include "Global.h"
#include "MemoryPool.h"
#include "SockHelper.h"

MemoryPool* MemoryPoolMgr= nullptr;

class Global
{
public:
	Global()
	{
		SockHelper::init();
		MemoryPoolMgr = new MemoryPool();
	}
	~Global()
	{
		delete MemoryPoolMgr;
	}

}GGlobal;