#include "pch.h"
#include "Allocator.h"

MemoryHeader* Allocator::_xnew(size_t _size)
{
    return  MemoryPoolMgr->Pop(_size);
}

void Allocator::_xdelete(void* _ptr)
{
    MemoryPoolMgr->Push(_ptr);
}
