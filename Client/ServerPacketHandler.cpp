#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "PacketSession.h"
#include "Protocol.pb.h"

void ServerPacketHandler::HandlePacket(BYTE* _buffer, int _iLen)
{
	BufferReader br(_buffer,_iLen);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case 1:
		Handle_S_TEST(_buffer, _iLen);
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

void ServerPacketHandler::Handle_S_TEST(BYTE* _buffer, int _iLen)
{
	Protocol::S_TEST pkt;
	//해당 데이터 파싱
	pkt.ParseFromArray(_buffer + sizeof(PacketHeader), _iLen - sizeof(PacketHeader));

	cout << "ID : " << pkt.id() << " TEXT : " << pkt.text()<<endl;

	int a = 10;
}
