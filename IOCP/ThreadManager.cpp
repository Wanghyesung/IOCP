#include "pch.h"
#include "ThreadManager.h"
#include "GThread.h"

void ThreadExcute(function<void(void)> _pFunc)
{
	ThreadManager::init();
	_pFunc();
	ThreadManager::clear();
}

ThreadManager::ThreadManager()
{

}

ThreadManager::~ThreadManager()
{

}


void ThreadManager::Excute(function<void(void)> _pFunc)
{
	lock_guard<mutex> lock(m_mutex);

	//std::move
	m_vecThread.emplace_back(thread(ThreadExcute, _pFunc));
}

void ThreadManager::Join()
{
	for (int i = 0; i < m_vecThread.size(); ++i)
	{
		if (m_vecThread[i].joinable())
			m_vecThread[i].join();
	}

	m_vecThread.clear();
}

void ThreadManager::init()
{
	static atomic<int> atID = 1;

	T_Thread_ID = atID.fetch_add(1);
}

void ThreadManager::clear()
{

}

