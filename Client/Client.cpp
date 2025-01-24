// Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "Service.h"
#include "IOCP.h"
#include "SockHelper.h"
#include "Session.h"
#include "ThreadManager.h"
#include "Global.h"
#include "SendBufferChunk.h"

BYTE sendData[] = "hellow";

class ServerSession : public Session
{
public:
    ServerSession() {};
    ~ServerSession() {};

    virtual void OnConnected()
    {
        shared_ptr<SendBuffer> sendBuffer = SendBufferMgr->Open(4096);
        memcpy(sendBuffer->GetData(), sendData, sizeof(sendData));
        sendBuffer->Close(sizeof(sendData));
        Send(sendBuffer);
    }
    virtual int OnRecv(BYTE* buffer, int len) 
    {
        cout << len <<endl;

        shared_ptr<SendBuffer> sendBuffer = SendBufferMgr->Open(4096);
        memcpy(sendBuffer->GetData(), buffer, sizeof(buffer));
        sendBuffer->Close(sizeof(buffer));
        Send(sendBuffer);

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


shared_ptr<ServerSession> MakeSharedSesion()
{
    return make_shared<ServerSession>();
}
int main()
{
    SockHelper::init();
    
    this_thread::sleep_for(1s);
    
    shared_ptr<ClientService> pClientService = make_shared<ClientService>(NetAddress(L"127.0.0.1", 7777),
        make_shared<IOCP>(), MakeSharedSesion, 10);

    pClientService->Start();

    for (int i = 0; i < 5; ++i)
    {
        ThreadMgr->Excute([=]()
            {
                while (true)
                {
                    this_thread::sleep_for(1s);

                    pClientService->GetIOCP()->Excute();
                }
            }
        );
    }
   
    ThreadMgr->Join();

}
