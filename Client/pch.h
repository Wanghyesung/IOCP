#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "IOCP\\Debug\\IOCP.lib")
#pragma comment(lib, "ProtoBuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "IOCP\\Release\\IOCP.lib")
#pragma comment(lib, "ProtoBuf\\Release\\libprotobuf.lib")

#endif

#include "Corepch.h";

#include "SendBuffer.h"