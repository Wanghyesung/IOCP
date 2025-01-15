#include "pch.h"
#include "Listener.h"
#include "IOCP.h"
#include "IOCPEvent.h"
#include "SockHelper.h"
#include "Service.h"
#include "NetAddress.h"

Listener::Listener()
{

}

Listener::~Listener()
{

}

void Listener::Start(ServerService* _pService)
{
	m_socket = SockHelper::Create_Socket();
	m_pService = _pService;

	//accept
	m_pService->GetIOCP()->RegisterEvent(this);
	
	if (SockHelper::SetTcpNoDelay(m_socket, true) == false)
		assert(nullptr);

	if (SockHelper::SetLinger(m_socket, 0, 0) == false)
		assert(nullptr);
	
	if (SockHelper::Bind(m_socket, _pService->GetAddress()) == false)
		assert(nullptr);

	if (SockHelper::Listen(m_socket, SOMAXCONN) == false)
		assert(nullptr);
	

	UINT AcceptCount = _pService->GetMaxSessionCount();

	for (UINT i = 0; i < AcceptCount; ++i)
	{
		IOCPAcceptEvent* pAcceptEvent = new IOCPAcceptEvent();
		pAcceptEvent->SetOwner(this);

		m_vecEvent.push_back(pAcceptEvent);
		RegisterAccept(pAcceptEvent);
	}
}

void Listener::DisPatch(IOCPEvent* _pEvent, int _iNumOfBytes)
{
	IOCPAcceptEvent* pAccept = static_cast<IOCPAcceptEvent*>(_pEvent);
	pAccept->GetOwner()->ProcessAccept(pAccept);
	
}

void Listener::RegisterAccept(IOCPAcceptEvent* acceptEvent)
{
	
	Session* pSession = m_pService->CreateSession();
	//클라 소켓
	m_pService->GetIOCP()->RegisterEvent(pSession);

	acceptEvent->init();
	acceptEvent->SetOwner(pSession);


	DWORD bytesRecved = 0;
	
	if (false == SockHelper::AcceptEx(m_socket, pSession->GetSocket(), pSession->m_RecvBuffer, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytesRecved, reinterpret_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			//만약 실패하면 다시 accept
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(IOCPAcceptEvent* acceptEvent)
{
	Session* session = acceptEvent->GetOwner();

	//서버로
	if (SockHelper::SetUpdateAcceptSocket(session->GetSocket(), m_socket) == false)
		assert(nullptr);

	SOCKADDR_IN clientAddr;
	int iAddrSize = sizeof(clientAddr);
	if (getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&clientAddr), &iAddrSize) == SOCKET_ERROR)
	{
		RegisterAccept(acceptEvent);
	}

	session->SetAddress(NetAddress(clientAddr));

	//connected
	session->ProcessConnect();
	
	RegisterAccept(acceptEvent);

}
