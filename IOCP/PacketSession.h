#pragma once
#include "Session.h"

struct PacketHeader
{
	UINT16 size;
	UINT16 id;
};

class PacketSession : public Session
{
public:
	PacketSession();
	~PacketSession();

	shared_ptr<PacketSession> GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int OnRecv(BYTE* _buffer, int _iLen) sealed;
	virtual int OnRecvPacket(BYTE* _buffer, int _iLen) = 0;


};

