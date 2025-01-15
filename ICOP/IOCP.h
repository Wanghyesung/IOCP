#pragma once
class Session;

class IOCP
{
public:
	IOCP();
	~IOCP();

public:
	void RegisterEvent(Session* _pSession);
	void Excute();

private:
	HANDLE m_IOCPHandle;
};

