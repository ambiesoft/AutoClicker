#pragma once

#include "resource.h"

struct ClickerThreadData
{
	long x_ = 0;
	long y_ = 0;
	HWND hParentWnd_ = nullptr;
	ClickerThreadData(LONG x,LONG y, HWND hParentWnd) : x_(x),y_(y),hParentWnd_(hParentWnd)
	{}
};
void clickerThreadMain(ClickerThreadData* data);
