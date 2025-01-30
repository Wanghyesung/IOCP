#include "pch.h"
#include "MemoryBlock.h"

MemoryBlock::MemoryBlock(size_t _size, BYTE* _buffer, size_t _count):
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

MemoryBlock::~MemoryBlock()
{

}

//나중엔 멀티스레드 환경을 위해 lock을 걸기
MemoryHeader* MemoryBlock::Pop()
{
    if (!m_Memroy.empty())
    {
        BYTE* memory = m_Memroy.back();
        m_Memroy.pop_back();

        MemoryHeader* header = reinterpret_cast<MemoryHeader*>(memory);
        return header;
    }

    //없다면 새로 할당
    return reinterpret_cast<MemoryHeader*>(malloc(m_size));
}

void MemoryBlock::Push(MemoryHeader* _ptr)
{
   //lock
   BYTE* memory = reinterpret_cast<BYTE*>(_ptr);
    m_Memroy.push_back(memory);
}
