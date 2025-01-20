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
        m_Memroy.push(DataPos);
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
        BYTE* memory = m_Memroy.top();
        m_Memroy.pop();

        MemoryHeader* header = reinterpret_cast<MemoryHeader*>(memory);
        return header;
    }

    return nullptr;
}

void Memory::Push(void* _ptr)
{
   //lock
   BYTE* memory = static_cast<BYTE*>(_ptr);
    m_Memroy.push(memory);
}
