#pragma once
#include "MemoryPool.h"

class MemoryHeader;
template<typename T, typename ...Args>
T* xnew(Args && ...args)
{
	int iTypeSize = sizeof(T) + sizeof(MemoryHeader);
	MemoryHeader* pHeader = nullptr;
	
	if (iTypeSize > MAX_POOLING_SIZE)
	{
		pHeader = reinterpret_cast<MemoryHeader*>(malloc(iTypeSize));
	}
	else
	{
		pHeader = MemoryPoolMgr->Pop(iTypeSize);
	}

	T* memory = static_cast<T*>(MemoryHeader::AttachHeader(pHeader, iTypeSize));
	new(memory)T(std::forward<Args>(args)...);
	return memory;
}

template<typename T>
void xdelete(T*& _ptr)
{
	_ptr->~T();

	MemoryHeader* pHeader = MemoryHeader::DetachHeader(_ptr);
	assert(pHeader->m_iSize > 0);

	if (pHeader->m_iSize > MAX_POOLING_SIZE)
	{
		free(_ptr);
	}
	else
	{
		MemoryPoolMgr->Push(pHeader);
		pHeader->m_iSize = 0;
	}

	_ptr = nullptr;

	/*
	_ptr = nullptr;
	xdelete ȣ�� �� �����Ͱ� nullptr�� �������� �ʴ� ������ �Լ��� �����͸� ������ �����ϱ� �����Դϴ�. �̸� �ذ��Ϸ���:
	�����͸� ������ �����մϴ�. (T*& _ptr)
	*/
	
}

template<typename T, typename ...Args>
shared_ptr<T> MakeShared(Args && ...args)
{
	return shared_ptr<T>(xnew<T>(std::forward<Args>(args)...), xdelete<T>);
}


