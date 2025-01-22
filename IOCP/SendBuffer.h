#pragma once

class SendBuffer : public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(size_t _BufferSize);
	~SendBuffer();


	int GetWritePos() { return m_iWritePos; }
	BYTE* GetData() { return m_vecSendBuffer.data(); }

	void CopyData(BYTE* _data, int _iLen);
private:
	int m_iBufferSize;

	int m_iWritePos;
	vector<BYTE> m_vecSendBuffer;
};

