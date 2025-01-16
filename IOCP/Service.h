#pragma once

#include "NetAddress.h"

class IOCP;
class Session;
class Listener;

enum eServiceType
{
	Client,
	Server,

};
class Service : enable_shared_from_this<Service>
{
public:
	//Service(const wstring& _strAddr , UINT _iProt);
	Service(eServiceType _eType, NetAddress _addr, shared_ptr<IOCP> _pIOCP, UINT _iMaxSessionCount);
	~Service();

	virtual void Start() =0;

	shared_ptr<Session> CreateSession();

	NetAddress GetAddress() { return m_Address; }
	shared_ptr<IOCP> GetIOCP() { return m_pIOCP; }
	UINT GetMaxSessionCount() { return m_iMaxSessionCount; }
	eServiceType GetServiceType() { return m_etype; }

	void AddSession(shared_ptr<Session> _pSession) { m_setSession.insert(_pSession); }
	void EraseSession(shared_ptr<Session> _pSession) { m_setSession.erase(_pSession); }


	



private:
	NetAddress m_Address;
	shared_ptr<IOCP> m_pIOCP;
	set<shared_ptr<Session>> m_setSession;
	eServiceType m_etype;
	
	UINT m_iMaxSessionCount;
};




class ServerService : public Service
{
public:
	ServerService(NetAddress _addr, shared_ptr<IOCP> _pIOCP, UINT _iMaxSessionCount);
	~ServerService();


public:
	virtual void Start() override;


private:
	Listener* m_pLisener;
};


class ClientService : public Service
{
public:
	ClientService(NetAddress _addr, shared_ptr<IOCP> _pIOCP, UINT _iMaxSessionCount);
	~ClientService();


public:
	virtual void Start() override;
};