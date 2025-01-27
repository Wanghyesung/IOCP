#pragma once
#include "Protocol.pb.h"
#include "PacketSession.h"
#include "SendBufferChunk.h"

template<typename T> shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT16 _ID);

enum
{
	S_TEST = 1,
};

class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* _buffer, int len);

	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_TEST& _pkt) { return _MakeSendBuffer(_pkt, S_TEST); }
	static shared_ptr<SendBuffer> Make_Client_Test(const wstring& _strMessage, const wstring& _strName);

};

template<typename T>
shared_ptr<SendBuffer> _MakeSendBuffer(T& _pkt, UINT16 _ID)
{
	UINT16 iDataSize = _pkt.ByteSizeLong(); //해당 패킷 사이즈
	UINT16 iPacketSize = iDataSize + sizeof(PacketHeader);

	shared_ptr<SendBuffer> sendBuffer = SendBufferMgr->Open(iPacketSize);

	PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(sendBuffer->GetData());
	pHeader->size = iPacketSize;
	pHeader->id = _ID;

	_pkt.SerializeToArray(&pHeader[1], iDataSize);

	sendBuffer->Close(iPacketSize);

	return sendBuffer;
}
