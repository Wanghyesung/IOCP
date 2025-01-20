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

public:
	MemoryPool();
	~MemoryPool();

public:
	MemoryHeader* Pop(size_t _size);
	void Push(void* _ptr);

private:
	//비슷한 크기의 버퍼를 찾는 용도
	vector<Memory*> m_vecMemory;
	BYTE m_Buffer[MAX_SIZE];
};



