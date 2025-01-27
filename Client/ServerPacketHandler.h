#pragma once
class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* _buffer, int _iLen);
	static void HandleServerTest(BYTE* _buffer, int _iLen);
};

