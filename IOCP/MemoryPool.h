#pragma once
#include "Memory.h"

class Knight
{
public:
	Knight() :ll(0), hp(100), tl('t'), tem(4214)
	{
		int a = 10;
	};
	Knight(long long _ll, int _hp, char _t, int _tem) :
		ll(_ll), hp(_hp), tl(_t), tem(_tem)
	{
		int a = 10;
	}
	~Knight() {}
public:
	long long ll;
	int hp;
	char tl;
	int tem;
};


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

	void Test()
	{
		BYTE* test = m_vecMemory[0]->m_Buffer;

		// MemoryHeader를 건너뛰어 객체 데이터에 접근
		BYTE* objectStart = test + sizeof(MemoryHeader);

		// 객체 데이터를 수정
		//void* knight = reinterpret_cast<void*>(objectStart);
		
		//knight = new string("GWNGHEfdnbFAKFJDBfu");

		std::string* knight = reinterpret_cast<std::string*>(objectStart);

		// 메모리 풀 내부 데이터를 새로운 문자열로 초기화
		new (knight) std::string("GWNGHEfdnbFAKFJDBfu");


		//knight = new string("GWNGHEfdnbFAKFJDBfu"); 가 메모리 풀 데이터를 수정하지 않는 이유 :
		//
		//이 코드는 새로운 문자열 객체를 동적으로 생성하고, knight 포인터가 이를 가리키게 합니다.
		//	하지만, 이는 메모리 풀 내부 데이터를 덮어쓰는 것이 아니기 때문에, 메모리 풀의 원래 데이터는 그대로 유지됩니다.
		//	올바른 데이터 덮어쓰기 방법 :
		//
		//reinterpret_cast를 사용하여 메모리 풀 내부 데이터를 적절한 타입으로 변환.
		//	기존 데이터를 삭제(필요할 경우 소멸자 호출)한 후, Placement new를 사용해 새로운 데이터를 초기화.
	}

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
