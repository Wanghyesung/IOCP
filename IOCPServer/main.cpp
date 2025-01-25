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
        shared_ptr<SendBuffer> SendBuffer = SendBufferMgr->Open(4096);
        BYTE* Data = SendBuffer->GetData();


        BufferWriter bw(Data, sizeof(PacketHeader) + sizeof(sendData));
        PacketHeader* header = bw.Reserve<PacketHeader>(1); //패킷 헤더만큼 미리 할당받기
        
        bw << 1001;
        header->id = 1;
        header->size = bw.GetWritePos();
       

        SendBuffer->Close(bw.GetWritePos());
        pService->BroadCast(SendBuffer);

        //(*reinterpret_cast<PacketHeader*>(Data)).id = 1;
        //(*reinterpret_cast<PacketHeader*>(Data)).size = sizeof(PacketHeader) + sizeof(sendData);
        //pService->BroadCast(SendBuffer);
        //SendBuffer->Close(sizeof(PacketHeader) + sizeof(sendData));

        this_thread::sleep_for(1s);
    }

    ThreadMgr->Join();
}

