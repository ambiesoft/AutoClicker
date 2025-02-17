#pragma once

#include "resource.h"

struct ClickerData
{
	long x_ = 0;
	long y_ = 0;
	HWND hParentWnd_ = nullptr;
	ClickerData(LONG x,LONG y, HWND hParentWnd) : x_(x),y_(y),hParentWnd_(hParentWnd)
	{}
};
void clickerMain(ClickerData* data);
