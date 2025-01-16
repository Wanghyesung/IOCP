#include "pch.h"
#include "SockHelper.h"
#include "NetAddress.h"


LPFN_CONNECTEX SockHelper::ConnectEx = nullptr;
LPFN_DISCONNECTEX SockHelper::DisConnectEx = nullptr;
LPFN_ACCEPTEX SockHelper::AcceptEx = nullptr;

void SockHelper::init()
{
	WSAData wsaData;
	assert(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);

	/* 런타임에 주소 얻어오는 API */
	SOCKET dummySocket = Create_Socket();
	assert(BindWindowFunc(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	assert(BindWindowFunc(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisConnectEx)));
	assert(BindWindowFunc(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));

	Close(dummySocket);
}

bool SockHelper::BindWindowFunc(SOCKET _socket, GUID WSAID, LPVOID _pFunc)
{
	DWORD bytes = 0;

	bool iResult = WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&WSAID, sizeof(WSAID),
		&_pFunc, sizeof(_pFunc),
		&bytes, NULL, NULL);

	if (iResult == SOCKET_ERROR)
		return false;

	return true;
}

SOCKET SockHelper::Create_Socket()
{
	//WSA_FLAG_OVERLAPPED 비동기 함수를 사용하기 위해서
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SockHelper::SetLinger(SOCKET _socket, int _iOnoff, int _iLinger)
{
	LINGER option;
	option.l_linger = _iOnoff;
	option.l_linger = _iLinger;

	//연결이 닫힐 때 데이터를 버퍼에서 전송할지, 또는 즉시 소켓을 닫을지를 결정
	return setsockopt(_socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&option), sizeof(LINGER));
}

bool SockHelper::SetReuseAddress(SOCKET _socket, bool _bFlag)
{
	return setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&_bFlag) ,sizeof(bool));
}

bool SockHelper::SetRecvBufferSize(SOCKET _socket, int _iSize)
{
	return setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&_iSize), sizeof(int));
}

bool SockHelper::SetSendBufferSize(SOCKET _socket, int _iSize)
{
	return setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&_iSize), sizeof(int));
}

bool SockHelper::SetTcpNoDelay(SOCKET _socket, bool _bFlag)
{
	return setsockopt(_socket, SOL_SOCKET, TCP_NODELAY, reinterpret_cast<const char*>(&_bFlag), sizeof(bool));
}

bool SockHelper::SetUpdateAcceptSocket(SOCKET _socket, SOCKET _listenSocket)
{
	return setsockopt(_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<const char*>(&_listenSocket), sizeof(SOCKET));
}

bool SockHelper::Start()
{
	WSADATA wsaData = {};
	return WSAStartup(MAKEWORD(2, 2), &wsaData) != false;
}

bool SockHelper::Close(SOCKET& _socket )
{
	return (closesocket(_socket) != SOCKET_ERROR);
}

bool SockHelper::Clear()
{
	WSACleanup();


	return true;
}

bool SockHelper::Bind(SOCKET _socket, NetAddress addr)
{
	return  bind(_socket, reinterpret_cast<const SOCKADDR*>(&addr.GetAddr()), sizeof(SOCKADDR_IN)) != SOCKET_ERROR;
}

bool SockHelper::BindAny(SOCKET _socket, UINT _iPort)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(_iPort);
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	return bind(_socket, reinterpret_cast<const SOCKADDR*>(&sockAddr), sizeof(SOCKADDR_IN)) != SOCKET_ERROR;
}

bool SockHelper::Listen(SOCKET _socket, int backlog)
{
	return listen(_socket, backlog) != SOCKET_ERROR;
}


