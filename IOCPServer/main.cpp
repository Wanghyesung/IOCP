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


class Test1
{
public:
    Test1()
    {

    }
    ~Test1()
    {

    }

    void Test() 
    {
        m_lock.WriteLock();

        ++i;

        m_lock.UnWriteLock();
    }

    void Get()
    {
        m_lock.ReadLock();

        --i;

        m_lock.UnReadLock();
    }

private:
    RWLock m_lock;
    int i = 0;
};

Test1* tt = new Test1();

void Run()
{
    for (int i = 0; i < 20000; ++i)
        tt->Test();
}

void Run1()
{
    for (int i = 0; i < 20000; ++i)
        tt->Get();
}

int main()
{
    for(int i = 0; i<2; ++i)
        ThreadMgr->Excute(Run);

    for(int i = 0; i<1; ++i)
        ThreadMgr->Excute(Run1);

    ThreadMgr->Join();

    SockHelper::init();
 
    shared_ptr<ServerService> pService = make_shared<ServerService>(NetAddress(L"127.0.0.1",7777), 
        make_shared<IOCP>(), MakeSharedListener , 1);
 
    pService->Start();
    
    while (true)
    {
        pService->GetIOCP()->Excute();

        this_thread::sleep_for(1s);
    }
}

