#pragma once
#include "Memory.h"

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
	//����� ũ���� ���۸� ã�� �뵵
	vector<Memory*> m_vecMemory;
	BYTE m_Buffer[MAX_SIZE];
};

template<typename T, typename... Args>
inline T* MemoryPool::xnew(Args&&... args)
{
	int iTypeSize = sizeof(T) + sizeof(MemoryHeader);
	MemoryHeader* header = _xnew(iTypeSize);

	//maxsize���� ũ�ų� �޸�Ǯ�� �����Ͱ� ���ٸ�
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
