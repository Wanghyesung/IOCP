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
	//���� ������� �޸� ũ�� Ȯ�ο�
public:
	MemoryHeader(int _iSize) : m_iSize(_iSize) {};
	~MemoryHeader() {};

	//��� ���� �� �������� �޸� ���������� ��ȯ
	static void* AttachHeader(MemoryHeader* _pHeader, int _iSize)
	{
		//��ü�� �̹� Ȯ���� �޸𸮿� �����Ͽ� �޸� �Ҵ��� ������带 ���Դϴ�.
		new(_pHeader)MemoryHeader(_iSize);
		return reinterpret_cast<void*>(_pHeader + 1);
	}

	//������� �޸𸮿� ���ũ�� ��ŭ �̵��� �޸� ��� ��ȯ
	static MemoryHeader* DetachHeader(void* _ptr)
	{
		//MemoryHeader��ŭ �ڷ� ���� ���� �ּ� ��ȯ
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



	//�Ҵ��� ũ�� , �ش� �޸� ��
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
	//����� ũ���� ���۸� ã�� �뵵
	vector<Memory*> m_vecMemory;
	BYTE m_Buffer[MAX_SIZE];
};



