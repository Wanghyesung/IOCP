#include "pch.h"
#include "Service.h"
#include "NetAddress.h"
#include "SockHelper.h"
#include "IOCP.h"
#include "Session.h"
#include "Listener.h"


Service::Service(eServiceType _eType, NetAddress _addr, IOCP* _pIOCP, UINT _iMaxSessionCount) :
	m_etype(_eType),
	m_Address(_addr),
	m_pIOCP(_pIOCP),
	m_iMaxSessionCount(_iMaxSessionCount)
{
	
}

Service::~Service()
{

}

Session* Service::CreateSession()
{
	Session* pSession = new Session();
	pSession->SetService(this);

	//iocpµî·Ï
	m_pIOCP->RegisterEvent(pSession);

	return pSession;
}



ServerService::ServerService(NetAddress _addr, IOCP* _pIOCP, UINT _iMaxSessionCount):
	Service(eServiceType::Server,_addr, _pIOCP,_iMaxSessionCount)
{

}

ServerService::~ServerService()
{

}

void ServerService::Start()
{
	if (SockHelper::Start())
		assert(nullptr);

		
	m_pLisener = new Listener();
	m_pLisener->Start(this);

}




ClientService::ClientService(NetAddress _addr, IOCP* _pIOCP, UINT _iMaxSessionCount):
	Service(eServiceType::Server, _addr, _pIOCP, _iMaxSessionCount)
{

}

ClientService::~ClientService()
{

}

void ClientService::Start()
{
	if (GetServiceType() != eServiceType::Client)
	{
		return;
	}

	Session* pSession = CreateSession();
	//pSession->Conn
}
