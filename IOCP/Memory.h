#pragma once



class Memory
{
public:
	Memory(size_t _size, BYTE* _buffer, size_t _count);
	~Memory();

public:
	class MemoryHeader* Pop();
	void Push(void* _ptr);


private:
	size_t m_size;//내 사이즈
	size_t m_memoryCount; //몇개로 관리할지
	BYTE* m_Buffer;//메모리 풀에서 할당해준 내 버퍼

	queue<BYTE*> m_Memroy;//받은 바이트를 내 사이즈만큼 나눠서 보관
	MemoryPool* m_pOwner;


	friend class MemoryPool;
};

