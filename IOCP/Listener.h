#pragma once
#include "Session.h"
class IOCPAcceptEvent;

class ServerService;
class Listener : public Session 
{
public:
	Listener();
	~Listener();

public:
	void Start(ServerService* _pService);

	virtual void DisPatch(IOCPEvent* _pEvent, int _iNumOfBytes) override;
public:
	void RegisterAccept(IOCPAcceptEvent* acceptEvent)override ;
	void ProcessAccept(IOCPAcceptEvent* acceptEvent) override ;

private:
	vector<IOCPAcceptEvent*> m_vecEvent;
};

