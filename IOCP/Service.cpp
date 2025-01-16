#include "pch.h"
#include "Service.h"
#include "NetAddress.h"
#include "SockHelper.h"
#include "IOCP.h"
#include "Session.h"
#include "Listener.h"


Service::Service(eServiceType _eType, NetAddress _addr, shared_ptr<IOCP> _pIOCP, UINT _iMaxSessionCount) :
	m_etype(_eType),
	m_Address(_addr),
	m_pIOCP(_pIOCP),
	m_iMaxSessionCount(_iMaxSessionCount)
{
	
}

Service::~Service()
{

}

shared_ptr<Session> Service::CreateSession()
{
	shared_ptr<Session> pSession = make_shared<Session>();
	pSession->SetService(shared_from_this());

	//iocpµî·Ï
	m_pIOCP->RegisterEvent(pSession);

	return pSession;
}



ServerService::ServerService(NetAddress _addr, shared_ptr<IOCP> _pIOCP, UINT _iMaxSessionCount):
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




ClientService::ClientService(NetAddress _addr, shared_ptr<IOCP> _pIOCP, UINT _iMaxSessionCount):
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

	int iCount = GetMaxSessionCount();

	for (int i = 0; i < iCount; ++i)
	{
		shared_ptr<Session> pSession = CreateSession();
		pSession->Connect();
	}
}
