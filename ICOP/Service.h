#pragma once

class NetAddress;
class IOCP;
class Session;
class Listener;

enum eServiceType
{
	Client,
	Server,

};
class Service
{
public:
	//Service(const wstring& _strAddr , UINT _iProt);
	Service(eServiceType _eType, NetAddress _addr, IOCP* _pIOCP, UINT _iMaxSessionCount);
	~Service();

	virtual void Start() =0;

	Session* CreateSession();

	NetAddress GetAddress() { return m_Address; }
	IOCP* GetIOCP() { return m_pIOCP; }
	UINT GetMaxSessionCount() { return m_iMaxSessionCount; }
	eServiceType GetServiceType() { return m_etype; }

	void AddSession(Session* _pSession) { m_setSession.insert(_pSession); }
	void EraseSession(Session* _pSession) { m_setSession.erase(_pSession); }


	



private:
	NetAddress m_Address;
	IOCP* m_pIOCP;
	set<Session*> m_setSession;
	eServiceType m_etype;
	
	UINT m_iMaxSessionCount;
};




class ServerService : public Service
{
public:
	ServerService(NetAddress _addr, IOCP* _pIOCP, UINT _iMaxSessionCount);
	~ServerService();


public:
	virtual void Start() override;


private:
	Listener* m_pLisener;
};


class ClientService : public Service
{
public:
	ClientService(NetAddress _addr, IOCP* _pIOCP, UINT _iMaxSessionCount);
	~ClientService();


public:
	virtual void Start() override;
};