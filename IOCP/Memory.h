#pragma once


class Memory
{
public:
	Memory(size_t _size, BYTE* _buffer, size_t _count);
	~Memory();

public:
	class MemoryHeader* Pop();
	void Push(MemoryHeader* _ptr);


private:
	size_t m_size;//�� ������
	size_t m_memoryCount; //��� ��������
	BYTE* m_Buffer;//�޸� Ǯ���� �Ҵ����� �� ����

/*
���� ����Ʈ ��� ����/ť�� �޸� ������ �л�Ǿ� ĳ�� ģȭ������ ���� �� ����.
��尡 �������� �޸� ������ ��ġ���� �ʱ� ����. �迭 ��� ����/ť�� ��ȯ
ĳ�� ��Ʈ�� ����.
�޸� ���� �ӵ� ���.
*/

	vector<BYTE*> m_Memroy;//���� ����Ʈ�� �� �����ŭ ������ ����
	MemoryPool* m_pOwner;


	friend class MemoryPool;
};

