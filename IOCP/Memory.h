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

/*
Memory 클래스에서 메모리 블록을 std::queue로 관리하고 있습니다. 이는 LIFO(스택) 방식이 아닌 FIFO(큐) 방식으로 메모리를 반환합니다.

메모리를 다시 사용할 때 불필요하게 앞쪽 메모리를 반환하므로 캐시 비효율을 유발할 가능성이 있습니다.
수정 방향
메모리 관리에 std::stack을 사용하는 것이 더 적합합니다.
*/

	stack<BYTE*> m_Memroy;//받은 바이트를 내 사이즈만큼 나눠서 보관
	MemoryPool* m_pOwner;


	friend class MemoryPool;
};

