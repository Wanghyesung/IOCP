#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "IOCP\\Debug\\ServerCore.lib")

#else
#pragma comment(lib, "IOCP\\Release\\ServerCore.lib")

#endif

#include "Corepch.h";