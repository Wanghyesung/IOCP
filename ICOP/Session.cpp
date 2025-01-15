#include "pch.h"
#include "Session.h"
#include "IOCPEvent.h"
#include "SockHelper.h"
#include "Service.h"
#include "IOCP.h"
#include "NetAddress.h"
Session::Session() :
	m_bConnected(false)
{
	m_socket = SockHelper::Create_Socket();
}

Session::~Session()
{

}

void Session::DisPatch(IOCPEvent* _pEvent, int _iNumOfBytes)
{
	eIocpEvent eEvent = _pEvent->GetEvent();
	switch (eEvent)
	{
	case eIocpEvent::Connect:
		ProcessConnect();
		break;

	case eIocpEvent::DisConnect:

		break;

	case eIocpEvent::Send:

		break;

	case eIocpEvent::Recv:

		break;
	default:
		break;
	}
}


void Session::Connect()
{
	RegisterConnect();

	
}

void Session::RegisterConnect()
{	
	if (GetService()->GetServiceType() != eServiceType::Client)
		return;


	if (SockHelper::SetReuseAddress(m_socket, true) == false)
		assert(nullptr);

	//누락하면 에러
	if (SockHelper::BindAny(m_socket, 0/*남는 포트, 주소번호로*/) == false)
		assert(nullptr);

	m_pConnectEvent->init();
	m_pConnectEvent->SetOwner(this);

	DWORD iNumOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetAddress().GetAddr();
	if (false == SockHelper::ConnectEx(m_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &iNumOfBytes, m_pConnectEvent))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			//참조
			assert(nullptr);
		}
	}
}

void Session::ProcessConnect()
{
	m_pConnectEvent->SetOwner(nullptr);
	//내 서버에 넣기
	m_bConnected.store(true);

	GetService()->AddSession(this);

	//오버라이딩
	OnConnected();

	//수신 등록 
	RegisterRecv();
}

void Session::RegisterDisConnect()
{

}

void Session::ProcessDisConnect()
{

}

void Session::RegisterSend()
{

}

void Session::ProcessSend()
{

}

void Session::RegisterRecv()
{

}

void Session::ProcessRecv()
{

}
