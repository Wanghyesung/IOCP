#pragma once

class Session;
enum eIocpEvent
{
	Send,
	Recv,
	Connect,
	DisConnect,
	Accept
};

class IOCPEvent : public OVERLAPPED
{
public:
	IOCPEvent(eIocpEvent _eType);
	~IOCPEvent();

	void init();

	eIocpEvent GetEvent() { return m_eEventType; }

	void SetOwner(shared_ptr<Session> _pOwner) { m_pOwner = _pOwner; }
	shared_ptr<Session> GetOwner() { return m_pOwner; }
private:
	eIocpEvent m_eEventType;

	shared_ptr<Session> m_pOwner;

};


class IOCPSendEvent : public IOCPEvent
{
public:
	IOCPSendEvent();
	~IOCPSendEvent();

public:
	BYTE m_sendBuffer[1024];
};


class IOCPRecvEvent : public IOCPEvent
{
public:
	IOCPRecvEvent();
	~IOCPRecvEvent();
};

class IOCPConnectEvent : public IOCPEvent
{
public:
	IOCPConnectEvent();
	~IOCPConnectEvent();
};

class ICOPDisConnectEvent : public IOCPEvent
{
public:
	ICOPDisConnectEvent();
	~ICOPDisConnectEvent();
};

class IOCPAcceptEvent : public IOCPEvent
{
public:
	IOCPAcceptEvent();
	~IOCPAcceptEvent();
};
