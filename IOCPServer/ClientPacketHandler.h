#pragma once
class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* _buffer, int len);

	static shared_ptr<SendBuffer> Make_Client_Test(const wstring& _strMessage, const wstring& _strName);

};

