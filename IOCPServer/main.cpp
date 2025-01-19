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

class Knight
{
public:
    Knight():ll(0), hp(100),tl('t'),tem(4214) 
    {
        int a = 10;
    };
    Knight(long long _ll, int _hp, char _t, int _tem):
        ll(_ll), hp(_hp), tl(_t), tem(_tem)
    {
        int a = 10;
    }
    ~Knight();
public:
    long long ll;
    int hp;
    char tl;
    int tem;
};

int main()
{
   // SockHelper::init();
    //SockHelper::init();
    
    LARGE_INTEGER frequency, start, end;

    // 타이머 주파수 가져오기
    QueryPerformanceFrequency(&frequency);

    // 시작 시간
    QueryPerformanceCounter(&start);

    Knight* tem;
    for (int i = 0; i < 2048; ++i)
    {
        tem = new Knight();
    }

    // 끝 시간
    QueryPerformanceCounter(&end);
    cout << (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart <<endl;


    QueryPerformanceCounter(&start);

    tem;
    for (int i = 0; i < 2048; ++i)
    {
        tem = MemoryPoolMgr->xnew<Knight>();
    }

    // 끝 시간
    QueryPerformanceCounter(&end);
    cout << (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart<<endl;


    int a = 10;

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

