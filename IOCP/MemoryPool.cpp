#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool()
{
	int i = 0;

	BYTE* nextMemory = m_Buffer;
	m_vecMemory.resize(MAX_SIZE);

	Memory* memory32 = new Memory(32, nextMemory, DIV_SIZE_32);
	memory32->m_pOwner = this;
	for (i ; i < 32; ++i)
	{
		m_vecMemory[i] = memory32;
	}
	nextMemory += DIV_SIZE_32 * 32;


	Memory* memory64 = new Memory(64, nextMemory, DIV_SIZE_64);
	memory64->m_pOwner = this;
	for (i; i < 64; ++i)
	{
		m_vecMemory[i] = memory64;
	}
	nextMemory += (DIV_SIZE_64 * 64);
	
	
	Memory* memory128 = new Memory(128, nextMemory, DIV_SIZE_128);
	memory128->m_pOwner = this;
	for (i; i < 128; ++i)
	{
		m_vecMemory[i] = memory128;
	}
	nextMemory += (DIV_SIZE_128 * 128);
	
	
	Memory* memory256 = new Memory(256, nextMemory, DIV_SIZE_256);
	memory256->m_pOwner = this;
	for (i; i < 256; ++i)
	{
		m_vecMemory[i] = memory256;
	}
	nextMemory += (DIV_SIZE_256 * 256);
	
	
	Memory* memory512 = new Memory(512, nextMemory, DIV_SIZE_512);
	memory512->m_pOwner = this;
	for (i; i < 512; ++i)
	{
		m_vecMemory[i] = memory32;
	}
	nextMemory += (DIV_SIZE_512 * 512);
	
	
	Memory* memory1024 = new Memory(1024, nextMemory, DIV_SIZE_1024);
	memory1024->m_pOwner = this;
	for (i; i < 1024; ++i)
	{
		m_vecMemory[i] = memory1024;
	}
	nextMemory += (DIV_SIZE_1024 * 1024);
	
	
	Memory* memory2048 = new Memory(2048, nextMemory, DIV_SIZE_2048);
	memory2048->m_pOwner = this;
	for (i; i < 2048; ++i)
	{
		m_vecMemory[i] = memory2048;
	}
	nextMemory += (DIV_SIZE_2048 * 2048);
}

MemoryPool::~MemoryPool()
{

}

MemoryHeader* MemoryPool::_xnew(size_t _size)
{
	//if (_size > MAX_SIZE)
	//{
	//	return malloc(_size);
	//}

	int iAllocSize = _size;

	return  m_vecMemory[iAllocSize]->Pop();
}

void MemoryPool::_xdelete(void* _ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(_ptr);
	
	m_vecMemory[header->m_iSize]->Push(_ptr);
}
