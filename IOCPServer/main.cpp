// IOCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <Service.h>
#include <IOCP.h>
#include "SockHelper.h"
#include "Listener.h"
#include "MemoryPool.h"

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


//tem
shared_ptr<ClientSession> MakeSharedListener()
{
    return make_shared<ClientSession>();
}


int main()
{
    SockHelper::init();

    Knight* Test = MemoryPoolMgr->xnew<Knight>();
    
    MemoryPoolMgr->Test();

    /*Knight* tem = MemoryPoolMgr->xnew<Knight>();
    Knight* tem1 = MemoryPoolMgr->xnew<Knight>(12421,24,'s',102);
    tem->ll = 124124;
    tem->hp = 4;
    tem->tl = 't';
    tem->tem = 124;*/
    shared_ptr<ServerService> pService = make_shared<ServerService>(NetAddress(L"127.0.0.1",7777), 
        make_shared<IOCP>(), MakeSharedListener , 1);
    
    pService->Start();
    
    while (true)
    {
        pService->GetIOCP()->Excute();

        this_thread::sleep_for(1s);
    }
}

