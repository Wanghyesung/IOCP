#pragma once
#include "MemoryPool.h"


class Allocator
{
private:
	static MemoryHeader* _xnew(size_t _size);
	static void _xdelete(void* _ptr);


public:
	template<typename T, typename... Args>
	static T* xnew(Args&&... args);

	template<typename T, typename... Args>
	static shared_ptr<T> MakeShared(Args&&... args);

	template<typename T>
	static void xdelete(T* _ptr);

};

template<typename T, typename ...Args>
inline T* Allocator::xnew(Args && ...args)
{
	int iTypeSize = sizeof(T) + sizeof(MemoryHeader);
	MemoryHeader* header = _xnew(iTypeSize);

	//maxsize보다 크거나 메모리풀에 데이터가 없다면
	if (iTypeSize > MemorySize::MAX_SIZE || header == nullptr)
	{
		return new T(std::forward<Args>(args)...);
	}

	T* memory = static_cast<T*>(MemoryHeader::AttachHeader(header, iTypeSize));
	new(memory)T(std::forward<Args>(args)...);
	return memory;	
}

template<typename T>
inline void Allocator::xdelete(T* _ptr)
{
	_ptr->~T();
	_xdelete(_ptr);
}

template<typename T, typename ...Args>
inline shared_ptr<T> Allocator::MakeShared(Args && ...args)
{
	return shared_ptr<T>(xnew<T>(std::forward<Args>(args)...), xdelete<T>);
}

