#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "PacketSession.h"

void ServerPacketHandler::HandlePacket(BYTE* _buffer, int _iLen)
{
	BufferReader br(_buffer,_iLen);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case 1:
		HandleServerTest(_buffer, _iLen);
		break;
	default:
		break;
	}
}

void ServerPacketHandler::HandleServerTest(BYTE* _buffer, int _iLen)
{
	BufferReader br(_buffer, _iLen);

	if (_iLen < sizeof(PacketTest))
		return;


	PacketTest test;
	br >> test;

	if (test.IsVaild() == false)
		return;


	wstring tem = L"";
	for (int i = 0; i < test.variableData.charCount; ++i)
	{
		WCHAR ch;
		br >> ch;
		tem += ch;

	}

	wcout << tem<< endl;



}
