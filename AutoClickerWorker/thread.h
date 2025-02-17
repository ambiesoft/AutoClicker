#pragma once

#include "resource.h"

class ThreadData
{
	HANDLE hParentProcess_ = nullptr;
public:
	ThreadData(HANDLE hParentProcess) :
		hParentProcess_(hParentProcess) 
	{

	}
	HANDLE GetProcess() const {
		return hParentProcess_;
	}
};
void threadMain(ThreadData* threadData);
