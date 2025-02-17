#pragma once
#include <Windows.h>

enum {
	WM_APP_WORKERREADY = WM_APP+1,
	WM_APP_WORKERFINISHED,
};

#define WORKER_EXE_NAME L"AutoClickerWorker.exe"
