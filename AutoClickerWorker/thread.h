#pragma once

#include "resource.h"

class DieMonitorThreadData
{
	HANDLE hParentProcess_ = nullptr;
public:
	DieMonitorThreadData(HANDLE hParentProcess) :
		hParentProcess_(hParentProcess) 
	{

	}
	HANDLE GetProcess() const {
		return hParentProcess_;
	}
};
void dieMonitorThreadMain(DieMonitorThreadData* threadData);
