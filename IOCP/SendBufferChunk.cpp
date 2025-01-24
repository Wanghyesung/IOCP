#include "pch.h"
#include "GThread.h"
#include "Allocator.h"
#include "SendBuffer.h"


SendBufferManager::SendBufferManager():
    m_Lock{}
{
}

SendBufferManager::~SendBufferManager()
{
}

shared_ptr<SendBuffer> SendBufferManager::Open(size_t _size)
{
    if (T_SendBufferChunk == nullptr)
    {
        T_SendBufferChunk = Pop();
        T_SendBufferChunk->Reset();
    }
    
    assert(T_SendBufferChunk->IsOpen() == false);

    if (T_SendBufferChunk->FreeSize() < _size)
    {
        T_SendBufferChunk = Pop();
        T_SendBufferChunk->Reset();
    }

   return T_SendBufferChunk->Open(_size);

}

shared_ptr<SendBufferChunk> SendBufferManager::Pop()
{
    WLock lock(m_Lock);

    
    if (m_vecSendBufferChunk.empty() == false)
    {
        shared_ptr<SendBufferChunk> sendBuffer = m_vecSendBufferChunk.back();
        m_vecSendBufferChunk.pop_back();
      
        return sendBuffer;
    } 


    //전역으로
    return shared_ptr<SendBufferChunk>(xnew<SendBufferChunk>(), PushGlobal);
    
}

void SendBufferManager::Push(shared_ptr<SendBufferChunk> _pSendBufferChunk)
{
    WLock lock(m_Lock);
    m_vecSendBufferChunk.push_back(_pSendBufferChunk);
}

void SendBufferManager::PushGlobal(SendBufferChunk* _pSendBufferChunk)
{
    //처음에먄 new 그 다음부터는 재사용
    SendBufferMgr->Push(shared_ptr<SendBufferChunk>(_pSendBufferChunk, PushGlobal));
}


SendBufferChunk::SendBufferChunk() :
    m_bOpen(false),
    m_iUsedPos(0)
{
    m_vecSendBuffer.resize(MAX_SENDBUFFER_SIZE);
}
SendBufferChunk::~SendBufferChunk()
{

}

shared_ptr<SendBuffer> SendBufferChunk::Open(size_t _size)
{
    if (m_bOpen)
        assert(nullptr);

    if (FreeSize() < _size)
        assert(nullptr);

    m_bOpen = true;
  
    return make_shared<SendBuffer>(shared_from_this(), GetBuffer(), _size);

    //내 SendBuffer에서 sendBufferchunk를 shared로 참조하기 때문에 내 메모리풀을 사용하면(재사용함) 해당 참조를 놔주지 않음 
    //에러발생
    //return MakeShared<SendBuffer>(shared_from_this(), GetBuffer(), _size);

}

void SendBufferChunk::Close(int _iUsedSize)
{
    assert(m_bOpen == true);

    m_bOpen = false;
    m_iUsedPos += _iUsedSize;
}

void SendBufferChunk::Reset()
{
    m_iUsedPos = 0;
    m_bOpen = false;
}
