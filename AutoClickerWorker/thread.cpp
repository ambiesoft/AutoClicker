// AutoClickerWorker.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "AutoClickerWorker.h"
#include "thread.h"

// using namespace Ambiesoft;

void threadMain(ThreadData* threadData)
{
	WaitForSingleObject(threadData->GetProcess(), INFINITE);
	TerminateProcess(GetCurrentProcess(), -1);
}