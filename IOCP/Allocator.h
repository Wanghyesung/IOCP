#pragma once
#include "MemoryPool.h"


template<typename T, typename ...Args>
T* xnew(Args && ...args)
{
	int iTypeSize = sizeof(T) + sizeof(MemoryHeader);
	MemoryHeader* header = MemoryPoolMgr->Pop(iTypeSize);

	//maxsize보다 크거나 메모리풀에 데이터가 없다면
	if (iTypeSize > 2048 || header == nullptr)
	{
		return new T(std::forward<Args>(args)...);
	}

	T* memory = static_cast<T*>(MemoryHeader::AttachHeader(header, iTypeSize));
	new(memory)T(std::forward<Args>(args)...);
	return memory;
}

template<typename T>
void xdelete(T* _ptr)
{
	_ptr->~T();
	MemoryPoolMgr->Push(_ptr);
}

template<typename T, typename ...Args>
shared_ptr<T> MakeShared(Args && ...args)
{
	return shared_ptr<T>(xnew<T>(std::forward<Args>(args)...), xdelete<T>);
}


