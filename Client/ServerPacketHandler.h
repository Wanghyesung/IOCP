#pragma once

enum
{
	S_TEST =1,
};

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* _buffer, int _iLen);
	static void HandleServerTest(BYTE* _buffer, int _iLen);
	static void Handle_S_TEST(BYTE* _buffer, int _iLen);

};

