// Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "Service.h"
#include "IOCP.h"
#include "SockHelper.h"
#include "Session.h"

class ServerSession : public Session
{
public:
    ServerSession() {};
    ~ServerSession() {};

    virtual void OnConnected()
    {

    }
    virtual int OnRecv(BYTE* buffer, int len) 
    {
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
        make_shared<IOCP>(), MakeSharedSesion, 1);

    pClientService->Start();

    while (true)
    {
        this_thread::sleep_for(1s);

        pClientService->GetIOCP()->Excute();

        BYTE SendBuffer[1024] = "Hellow";
        pClientService->BroadCast(SendBuffer);

    }


}
