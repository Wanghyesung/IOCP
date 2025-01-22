#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(size_t _BufferSize):
	m_iBufferSize(_BufferSize),
	m_iWritePos(0)
{
	m_vecSendBuffer.resize(m_iBufferSize);
}

SendBuffer::~SendBuffer()
{

}

void SendBuffer::CopyData(BYTE* _data, int _iLen)
{
	memcpy(&m_vecSendBuffer[m_iWritePos], _data, _iLen);

	m_iWritePos += _iLen;
}
