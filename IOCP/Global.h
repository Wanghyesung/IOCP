#pragma once
#include "SockHelper.h"

class Global
{
public:
	Global()
	{
		SockHelper::init();
	}
	~Global()
	{

	}

};

