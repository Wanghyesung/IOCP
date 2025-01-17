#pragma once

#include "NetAddress.h"
#include "IOCPEvent.h"

class IOCPEvent;
class Service;

class Session : public enable_shared_from_this<Session>
{
public:
	Session();
	~Session();

	virtual void DisPatch(IOCPEvent* _pEvent, int _iNumOfBytes);

	void SetService(shared_ptr<Service> _pService) { m_weakService = _pService; }
	void SetAddress(NetAddress _Address) { m_Address = _Address; }

	HANDLE GetHandle() { return reinterpret_cast<HANDLE>(m_socket);}
	SOCKET GetSocket() { return m_socket; }
	shared_ptr<Service> GetService() { return m_weakService.lock(); }


public:
	virtual void RegisterAccept(IOCPAcceptEvent* _pIocpEvent) {};
	virtual void ProcessAccept(IOCPAcceptEvent* _pIocpEvent) {};

	virtual void RegisterConnect();
	virtual void ProcessConnect();

	virtual void RegisterDisConnect();
	virtual void ProcessDisConnect();

	virtual void RegisterSend();
	virtual void ProcessSend(int _iNumOfBytes);

	virtual void RegisterRecv();
	virtual void ProcessRecv(int _iNumOfBytes);

public:
	void DisConnect(const WCHAR* _strCause);
	void Connect();
	void Send(BYTE* _pBuffer, int _iLen);
	//void Recv();
protected:
	//만약 연결이 되었을 때 실행할 함수 
	//(특정 동작이 되었을 때 실행할 함수들)
	virtual void OnConnected() {};
	virtual int OnRecv(BYTE* buffer, int len) { return len; }
	virtual void OnSend(int len) {}
	virtual void OnDisConnected() {}

protected:
	SOCKET m_socket;
	weak_ptr<Service> m_weakService;//순환 참조
	NetAddress m_Address;

	IOCPConnectEvent m_ConnectEvent;
	ICOPDisConnectEvent m_DisConnectEvent;
	IOCPSendEvent m_sendEvent;
	IOCPRecvEvent m_recvEvent;

	atomic<bool> m_bConnected;

public:
	BYTE m_RecvBuffer[1024] = {};
};

