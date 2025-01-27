#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "PacketSession.h"
#include "SendBufferChunk.h"

void ClientPacketHandler::HandlePacket(BYTE* _buffer, int len)
{
	BufferReader br(_buffer, len);

	PacketHeader header;
	br >> header;


	switch (header.id)
	{
	default:
		break;
	}
}

shared_ptr<SendBuffer> ClientPacketHandler::Make_Client_Test(const wstring& _strMessage, const wstring& _strName)
{
	shared_ptr<SendBuffer> pSendBuffer = SendBufferMgr->Open(4096);
	BufferWriter bw(pSendBuffer->GetData(), pSendBuffer->GetBufferSize());

	PacketHeader* pHeader = bw.Reserve<PacketHeader>(1);
	pHeader->id = 1;
	

	VariableData* pVarData = bw.Reserve<VariableData>(1);
	pVarData->charCount = _strMessage.size();
	pVarData->offsetPos = bw.GetWritePos();
		

	for (WCHAR ch : _strMessage)
		bw << ch;
	
	pHeader->size = bw.GetWritePos();
	pSendBuffer->Close(bw.GetWritePos());

	return pSendBuffer;

}



