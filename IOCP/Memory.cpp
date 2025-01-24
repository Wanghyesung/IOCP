#include "pch.h"
#include "Memory.h"

Memory::Memory(size_t _size, BYTE* _buffer, size_t _count):
    m_size(_size),
    m_Buffer(_buffer),
    m_memoryCount(_count)
{
    for (int i = 0; i < m_memoryCount; ++i)
    {

        BYTE* DataPos = m_Buffer + (m_size * i);
        //uintptr_t* p = reinterpret_cast<uintptr_t*>(DataPos);
        m_Memroy.push_back(DataPos);
    }
}

Memory::~Memory()
{

}

//���߿� ��Ƽ������ ȯ���� ���� lock�� �ɱ�
MemoryHeader* Memory::Pop()
{
    if (!m_Memroy.empty())
    {
        BYTE* memory = m_Memroy.back();
        m_Memroy.pop_back();

        MemoryHeader* header = reinterpret_cast<MemoryHeader*>(memory);
        return header;
    }

    //���ٸ� ���� �Ҵ�
    return reinterpret_cast<MemoryHeader*>(malloc(m_size));
}

void Memory::Push(MemoryHeader* _ptr)
{
   //lock
   BYTE* memory = reinterpret_cast<BYTE*>(_ptr);
    m_Memroy.push_back(memory);
}
