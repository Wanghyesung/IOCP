#pragma once
#include "MemoryPool.h"
template <typename T>
class StlAllocator
{
	using value_type = T;

public:
	StlAllocator() {};

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {};

	//컨테이너 사이즈
	T* allocate(size_t count)
	{
		const int iTypeSize = static_cast<int>(count * (sizeof(T)) + sizeof(MemoryHeader));
		MemoryHeader* header = nullptr;
		
		if (iTypeSize > MAX_POOLING_SIZE)
		{
			header = reinterpret_cast<MemoryHeader*>(malloc(iTypeSize));	 
		}
		else
		{
			header = MemoryPoolMgr->Pop(iTypeSize);
		}

		T* memory = static_cast<T*>(MemoryHeader::AttachHeader(header, iTypeSize));
		return memory;
	}

	void deallocate(T* ptr)
	{
		xdelete(ptr);
	}
};