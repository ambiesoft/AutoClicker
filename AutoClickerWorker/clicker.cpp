// AutoClickerWorker.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "../../lsMisc/BringWinTop.h"
#include "../../lsMisc/DebugMacro.h"
#include "../common/common.h"


#include "AutoClickerWorker.h"
#include "clicker.h"

using namespace Ambiesoft;

static void ErrorTerminate(const wchar_t* pMessage)
{
	MessageBox(nullptr,
		pMessage,
		APPNAME,
		MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), -1);
}
void clickerMain(ClickerData* data)
{
	POINT origPos;
	POINT targetPos;
	targetPos.x = data->x_;
	targetPos.y = data->y_;

	//HDESK hDesk = OpenInputDesktop(0, FALSE, 0);
	//SetThreadDesktop(hDesk);

	int count = 50;
#ifdef _DEBUG
	wchar_t szT[500];
#endif
	for (int i = 0; i < count; ++i)
	{
#ifdef _DEBUG
		HWND hActive = ::GetForegroundWindow();
		GetWindowText(hActive, szT, _countof(szT));
		DTRACE(szT);
#endif
		if (GetAsyncKeyState(VK_LBUTTON) < 0)
		{
			DTRACE(L"LBUTTON is pressed");
			--i;
			continue;
		}

		if (!GetCursorPos(&origPos))
		{
			ErrorTerminate(I18N(L"Failed to get mouse position"));
		}

		if (!SetCursorPos(targetPos.x, targetPos.y))
		{
			ErrorTerminate(I18N(L"Failed to set cursor pos"));
		}

		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		// Sleep(100);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);


		if (!SetCursorPos(origPos.x, origPos.y))
		{
			ErrorTerminate(I18N(L"Failed to set cursor pos"));
		}
#ifdef _DEBUG
		BringWinTop(hActive);
#endif
		Sleep(3000);
	}
	PostMessage(data->hParentWnd_, WM_APP_WORKERFINISHED, 0, 0);
	TerminateProcess(GetCurrentProcess(), 0);
}
