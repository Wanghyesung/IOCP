// IOCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <Service.h>
#include <IOCP.h>
#include "SockHelper.h"
#include "Listener.h"
#include "PacketSession.h"
#include "Allocator.h"
#include "RWLock.h"
#include "ThreadManager.h"
#include "Global.h"
#include "SendBufferChunk.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"

class ClientSession: public PacketSession
{
public:
    ClientSession() {};
    ~ClientSession() {};

    virtual void OnConnected()
    {

    }
    virtual int OnRecvPacket(BYTE* buffer, int len)
    {
        //cout << buffer << endl;
        //
        //shared_ptr<SendBuffer> sendBuffer = SendBufferMgr->Open(4096);
        //memcpy(sendBuffer->GetData(), buffer, sizeof(buffer));
        //sendBuffer->Close(sizeof(buffer));
        //
        //GetService()->BroadCast(sendBuffer);

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


wstring sendData = L"Hellow Word";
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


    while (true)
    {
       
        shared_ptr<SendBuffer> pSendBuffer = ClientPacketHandler::Make_Client_Test(L"hellow World", L"임시");
        pService->BroadCast(pSendBuffer);
        this_thread::sleep_for(1s);

        //(*reinterpret_cast<PacketHeader*>(Data)).id = 1;
        //(*reinterpret_cast<PacketHeader*>(Data)).size = sizeof(PacketHeader) + sizeof(sendData);
        //pService->BroadCast(SendBuffer);
        //SendBuffer->Close(sizeof(PacketHeader) + sizeof(sendData));
    }

    ThreadMgr->Join();
}

