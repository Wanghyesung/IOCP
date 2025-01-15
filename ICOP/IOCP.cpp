#include "pch.h"
#include "IOCP.h"
#include "Session.h"
#include "IOCPEvent.h"

IOCP::IOCP()
{
	//icop ����
	m_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}

IOCP::~IOCP()
{

}

void IOCP::RegisterEvent(Session* _pSession)
{
	HANDLE handle = _pSession->GetHandle();
	CreateIoCompletionPort(handle, m_IOCPHandle, 0, 0 );

}

void IOCP::Excute()
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IOCPEvent* iocpEvnet = nullptr;
	
	DWORD milliseconds = 1;
	
	//������ I/O �Ϸ� ��Ʈ���� I/O �Ϸ� ��Ŷ�� ť���� �����Ϸ��� �õ��մϴ�. ���ٸ� ���涧���� ��� 
	if (GetQueuedCompletionStatus(m_IOCPHandle, &numOfBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvnet), milliseconds) != false)
	{
		Session* pSession = iocpEvnet->GetOwner();
		pSession->DisPatch(iocpEvnet, numOfBytes);
	}
	else
	{
		int error = GetLastError();
		if(error == WAIT_TIMEOUT)
			return;
		else
		{

		}
	}
}
