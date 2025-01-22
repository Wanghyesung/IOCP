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
		MemoryHeader* header = MemoryPoolMgr->Pop(iTypeSize);

		if (iTypeSize > MemorySize::MAX_SIZE || header == nullptr)
		{
			return new T();
		}

		T* memory = static_cast<T*>(MemoryHeader::AttachHeader(header, iTypeSize));
		return memory;
	}

	void deallocate(T* ptr)
	{
		xdelete(ptr);
	}
};