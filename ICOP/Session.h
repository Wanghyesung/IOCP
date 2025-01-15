#pragma once
class IOCPEvent;
class Service;
class NetAddress;
class IOCPConnectEvent;

class Session
{
public:
	Session();
	~Session();

	virtual void DisPatch(IOCPEvent* _pEvent, int _iNumOfBytes);

	void SetService(Service* _pService) { m_pService = _pService; }
	void SetAddress(NetAddress _Address) { m_Address = _Address; }

	HANDLE GetHandle() { return reinterpret_cast<HANDLE>(m_socket);}
	SOCKET GetSocket() { return m_socket; }
	Service* GetService() { return m_pService; }


public:
	virtual void RegisterAccept(IOCPAcceptEvent* _pIocpEvent) {};
	virtual void ProcessAccept(IOCPAcceptEvent* _pIocpEvent) {};

	virtual void RegisterConnect();
	virtual void ProcessConnect();

	virtual void RegisterDisConnect();
	virtual void ProcessDisConnect();

	virtual void RegisterSend();
	virtual void ProcessSend();

	virtual void RegisterRecv();
	virtual void ProcessRecv();

public:
	void Connect();

protected:
	//만약 연결이 되었을 때 실행할 함수 
	//(특정 동작이 되었을 때 실행할 함수들)
	virtual void OnConnected() {};
	virtual int OnRecv(BYTE* buffer, int len) { return len; }
	virtual void OnSend(int len) {}
	virtual void OnDisConnected() {}

protected:
	SOCKET m_socket;
	Service* m_pService;
	NetAddress m_Address;
	IOCPConnectEvent* m_pConnectEvent;
	
	bool m_bConnected;
public:
	unsigned char m_RecvBuffer[1024];
	unsigned char m_SendBuffer[1024];
};

