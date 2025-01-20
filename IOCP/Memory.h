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
	size_t m_size;//�� ������
	size_t m_memoryCount; //��� ��������
	BYTE* m_Buffer;//�޸� Ǯ���� �Ҵ����� �� ����

/*
Memory Ŭ�������� �޸� ����� std::queue�� �����ϰ� �ֽ��ϴ�. �̴� LIFO(����) ����� �ƴ� FIFO(ť) ������� �޸𸮸� ��ȯ�մϴ�.

�޸𸮸� �ٽ� ����� �� ���ʿ��ϰ� ���� �޸𸮸� ��ȯ�ϹǷ� ĳ�� ��ȿ���� ������ ���ɼ��� �ֽ��ϴ�.
���� ����
�޸� ������ std::stack�� ����ϴ� ���� �� �����մϴ�.
*/

	queue<BYTE*> m_Memroy;//���� ����Ʈ�� �� �����ŭ ������ ����
	MemoryPool* m_pOwner;


	friend class MemoryPool;
};

