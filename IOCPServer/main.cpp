// IOCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <Service.h>
#include <IOCP.h>
#include "SockHelper.h"
#include "Listener.h"
#include "Allocator.h"
#include "RWLock.h"
#include "ThreadManager.h"
#include "Global.h"
#include "SendBufferChunk.h"

class ClientSession: public Session
{
public:
    ClientSession() {};
    ~ClientSession() {};

    virtual void OnConnected()
    {

    }
    virtual int OnRecv(BYTE* buffer, int len)
    {
        cout << buffer << endl;

        shared_ptr<SendBuffer> sendBuffer = SendBufferMgr->Open(4096);
        memcpy(sendBuffer->GetData(), buffer, len);
        sendBuffer->Close(len);

        GetService()->BroadCast(sendBuffer);

        return len;
    }
    virtual void OnSend(int len)
    {

    }
    virtual void OnDisConnected()
    {

    }

private:

};


//tem
shared_ptr<ClientSession> MakeSharedListener()
{
    return make_shared<ClientSession>();
}


int main()
{
    SockHelper::init();
 
    shared_ptr<ServerService> pService = make_shared<ServerService>(NetAddress(L"127.0.0.1",7777), 
        make_shared<IOCP>(), MakeSharedListener , 10);
 
    pService->Start();
 
    for (int i = 0; i < 5; ++i)
    {
        ThreadMgr->Excute([=]()
            {
                while (true)
                {
                    this_thread::sleep_for(1s);

                    pService->GetIOCP()->Excute();
                }
            }
        );
    }

    ThreadMgr->Join();
}

