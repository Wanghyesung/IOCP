#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "IOCP\\Debug\\IOCP.lib")

#else
#pragma comment(lib, "IOCP\\Release\\IOCP.lib")

#endif

#include "Corepch.h";

#include "SendBuffer.h"