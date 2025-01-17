// IOCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <Service.h>
#include <IOCP.h>
#include "SockHelper.h"
#include "Listener.h"

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


shared_ptr<ClientSession> MakeSharedListener()
{
    return make_shared<ClientSession>();
}

int main()
{
    SockHelper::init();
    //SockHelper::init();

    shared_ptr<ServerService> pService = make_shared<ServerService>(NetAddress(L"127.0.0.1",7777), 
        make_shared<IOCP>(), MakeSharedListener , 1);
    
    pService->Start();
    
    while (true)
    {
        pService->GetIOCP()->Excute();

        this_thread::sleep_for(1s);
    }
}

