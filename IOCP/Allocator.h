#pragma once
#include "PoolAllocator.h"

class MemoryHeader;
template<typename T, typename ...Args>
T* xnew(Args && ...args)
{
	void* pHeader = PoolAllocator::Alloc(sizeof(T));
	
	T* memory = static_cast<T*>(pHeader);
	new(memory)T(std::forward<Args>(args)...);
	return memory;
}

template<typename T>
void xdelete(T*& _ptr)
{
	_ptr->~T();

	PoolAllocator::Release(_ptr);

	_ptr = nullptr;
	
	/*
	_ptr = nullptr;
	xdelete 호출 후 포인터가 nullptr로 설정되지 않는 이유는 함수가 포인터를 값으로 전달하기 때문입니다. 이를 해결하려면:
	포인터를 참조로 전달합니다. (T*& _ptr)
	*/
}

template<typename T, typename ...Args>
std::shared_ptr<T> MakeShared(Args && ...args)
{
	return std::shared_ptr<T>(xnew<T>(std::forward<Args>(args)...), xdelete<T>);
}


template<typename T>
class StlAllocator
{
public:
	//stl value_type를 내 T타입으로
	using value_type = T;


	StlAllocator() {};

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {};

	//컨테이너 사이즈
	T* allocate(size_t count)
	{
		int iSize = static_cast<int>(count * (sizeof(T)));
		return static_cast<T*>(PoolAllocator::Alloc(iSize));
	}

	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::Release(ptr);
	}
};

