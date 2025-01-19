#pragma once
#include "Memory.h"

class MemoryHeader
{
	//내가 사용중인 메모리 크기 확인용
public:
	MemoryHeader(int _iSize) : m_iSize(_iSize) {};
	~MemoryHeader() {};

	//헤더 부착 후 실질적인 메모리 사용시점부터 반환
	static void* AttachHeader(MemoryHeader* _pHeader, int _iSize)
	{
		//객체를 이미 확보된 메모리에 생성하여 메모리 할당의 오버헤드를 줄입니다.
		new(_pHeader)MemoryHeader(_iSize);
		return reinterpret_cast<void*>(_pHeader + 1);
	}

	//사용중인 메모리에 헤더크기 만큼 이동후 메모리 헤더 반환
	static MemoryHeader* DetachHeader(void* _ptr)
	{
		//MemoryHeader만큼 뒤로 가서 시작 주소 반환
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(_ptr) - 1;
		return header;
	}

private:
	int m_iSize;


	friend class MemoryPool;
};

enum MemorySize
{
	DIV_SIZE_32 = 2048,
	DIV_SIZE_64 = 1024,
	DIV_SIZE_128 = 512,
	DIV_SIZE_256 = 256,
	DIV_SIZE_512 = 128,
	DIV_SIZE_1024 = 64,
	DIV_SIZE_2048 = 32,



	//할당할 크기 , 해당 메모리 수
	MAX_SIZE = 32   *   DIV_SIZE_32 + 
			   64   *   DIV_SIZE_64 + 
			   128  *   DIV_SIZE_128 + 
			   256  *   DIV_SIZE_256 + 
			   512  *   DIV_SIZE_512 + 
			   1024 *   DIV_SIZE_1024 + 
	           2048 *   DIV_SIZE_2048,
};



class MemoryPool
{
	//MAX_SIZE[Memory(32)[32*2048] , Memory(64)[64 * 1024]     ....      ]
	
public:
	MemoryPool();
	~MemoryPool();

private:
	MemoryHeader* _xnew(size_t _size);
	void _xdelete(void* _ptr);


public:
	template<typename T, typename... Args>
	T* xnew(Args&&... args);

	template<typename T>
	void xdelete(T* _ptr);

private:
	//비슷한 크기의 버퍼를 찾는 용도
	vector<Memory*> m_vecMemory;
	BYTE m_Buffer[MAX_SIZE];
};

template<typename T, typename... Args>
inline T* MemoryPool::xnew(Args&&... args)
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
inline void MemoryPool::xdelete(T* _ptr)
{
	_ptr->~T();
	_xdelete(_ptr);
}
