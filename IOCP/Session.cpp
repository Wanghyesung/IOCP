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
		ProcessDisConnect();
		break;

	case eIocpEvent::Send:
		ProcessSend(_iNumOfBytes);
		break;

	case eIocpEvent::Recv:
		ProcessRecv(_iNumOfBytes);
		break;
	default:
		break;
	}
}


void Session::DisConnect(const WCHAR* _strCause)
{
	if (m_bConnected.exchange(false) == false)
		return;

	wcout << _strCause;

	GetService()->EraseSession(shared_from_this());

}

void Session::Connect()
{
	RegisterConnect();
}

void Session::Send(BYTE* _pBuffer, int _iLen)
{
	memcpy(m_sendEvent.m_sendBuffer, _pBuffer, _iLen);

	RegisterSend();
}

void Session::RegisterConnect()
{	
	if (GetService()->GetServiceType() != eServiceType::Client)
		return;


	if (SockHelper::SetReuseAddress(m_socket, true) == false)
		assert(nullptr);

	//�����ϸ� ����
	if (SockHelper::BindAny(m_socket, 0/*���� ��Ʈ, �ּҹ�ȣ��*/) == false)
		assert(nullptr);

	m_ConnectEvent.init();
	m_ConnectEvent.SetOwner(shared_from_this());

	DWORD iNumOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetAddress().GetAddr();
	if (false == SockHelper::ConnectEx(m_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &iNumOfBytes, &m_ConnectEvent))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			//����
			assert(nullptr);
		}
	}
}

void Session::ProcessConnect()
{
	m_ConnectEvent.SetOwner(nullptr);
	//�� ������ �ֱ�
	m_bConnected.store(true);

	GetService()->AddSession(shared_from_this());

	//�������̵�
	OnConnected();

	//���� ��� 
	RegisterRecv();
}

void Session::RegisterDisConnect()
{
	m_DisConnectEvent.SetOwner(shared_from_this());
	m_DisConnectEvent.init();


	if (SockHelper::DisConnectEx(m_socket, &m_DisConnectEvent, TF_REUSE_SOCKET, 0) != false)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			assert(nullptr);
		}
	}
}

void Session::ProcessDisConnect()
{
	m_DisConnectEvent.SetOwner(nullptr); //���� ����

	GetService()->EraseSession(shared_from_this()); //lock

	OnDisConnected();

}

void Session::RegisterSend()
{
	m_sendEvent.SetOwner(shared_from_this()); //���� ����ְ� �ؼ� �ٷ� �������� ���ϰ� (���� Event����)
	m_sendEvent.init();

	WSABUF wsaBuf = {};
	wsaBuf.buf = reinterpret_cast<char*>(m_sendEvent.m_sendBuffer);
	wsaBuf.len = 1024;

	DWORD sendBytes = 0;
	if (WSASend(m_socket, &wsaBuf, 1, &sendBytes, 0, &m_sendEvent, nullptr) != false)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_sendEvent.SetOwner(nullptr);
			//assert(nullptr);
		}
	}
}

void Session::ProcessSend(int _iNumOfBytes)
{
	if (_iNumOfBytes == 0)
	{
		//������ ����
		DisConnect(L"Recv 0");
		return;
	}

	m_sendEvent.SetOwner(nullptr);
	
	OnSend(_iNumOfBytes);	
}

void Session::RegisterRecv()
{
	m_recvEvent.init();
	m_recvEvent.SetOwner(shared_from_this());

	//���� ����
	WSABUF wsaBuf = {};
	wsaBuf.buf = reinterpret_cast<char*>(m_RecvBuffer);//writePos ��ġ�� ���ۺ���
	wsaBuf.len = 1024;

	DWORD numOfBytes = 0;
	DWORD flag = 0;

	if(WSARecv(m_socket, &wsaBuf, 1, &numOfBytes, &flag, &m_recvEvent, nullptr) != false)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			assert(nullptr);
			//m_recvEvent.SetOwner(nullptr);
		}
	}
}

void Session::ProcessRecv(int _iNumOfBytes)
{
	m_recvEvent.SetOwner(nullptr);

	if (_iNumOfBytes == 0)
	{
		//������ ����
		DisConnect(L"Recv 0");
		return;
	}

	OnRecv(m_RecvBuffer, _iNumOfBytes);

	RegisterRecv();
}
