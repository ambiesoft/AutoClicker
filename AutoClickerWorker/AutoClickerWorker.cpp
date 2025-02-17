// AutoClickerWorker.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <sstream>
#include <thread>

#include "../../lsMisc/CreateSimpleWindow.h"
#include "../../lsMisc/CommandLineParser.h"
#include "../../lsMisc/CHandle.h"
#include "../../lsMisc/GetLastErrorString.h"
#include "../../lsMisc/DebugMacro.h"
#include "../../lsMisc/HighDPI.h"

#include "AutoClickerWorker.h"
#include "thread.h"
#include "clicker.h"

using namespace Ambiesoft;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}

template<typename T>
void ErrorExit(const wchar_t* pMessage, const T& value)
{
    std::wstringstream wss;
    wss << value << L":" << pMessage;

    MessageBox(nullptr,
        wss.str().c_str(),
        APPNAME,
        MB_ICONERROR);
    exit(1);
}
template<typename T>
void ErrorLastErrorExit(DWORD dwLastError, const T& value)
{
    ErrorExit(GetLastErrorString(dwLastError).c_str(), value);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    InitHighDPISupport();

    CCommandLineParser parser(APPNAME, I18N(L"This process makes auto click"));

    DWORD idProcess = 0;
    parser.AddOption({ L"-p", L"--process" },
        ArgCount::ArgCount_One,
        & idProcess,
        ArgEncodingFlags_Default,
        I18N(L"Parent process"));

    ULONGLONG uParentWnd = 0;
    parser.AddOption({ L"-w", L"--window" },
        ArgCount::ArgCount_One,
        & uParentWnd,
        ArgEncodingFlags_Default,
        I18N(L"Parent window"));

    LONG x = 0;
    LONG y = 0;
    parser.AddOption({ L"-x", L"--xpos" },
        ArgCount::ArgCount_One,
        &x,
        ArgEncodingFlags_Default,
        I18N(L"X Position"));
    parser.AddOption({ L"-y", L"--ypos" },
        ArgCount::ArgCount_One,
        &y,
        ArgEncodingFlags_Default,
        I18N(L"X Position"));

    parser.Parse();

    CKernelHandle hParentProcess(OpenProcess(SYNCHRONIZE, FALSE, idProcess));
    if (!hParentProcess)
    {
        ErrorLastErrorExit(GetLastError(), idProcess);
    }

    DWORD dwWait = WaitForSingleObject(hParentProcess, 0);
    if (dwWait != WAIT_TIMEOUT)
    {
        ErrorLastErrorExit(GetLastError(), idProcess);
    }

    HWND hParentWnd = (HWND)uParentWnd;
    if (!IsWindow(hParentWnd))
    {
        ErrorExit(I18N(L"Not a window"), (unsigned long long)hParentProcess);
    }

    HWND hWnd = CreateSimpleWindow(WndProc);
    if (!IsWindow(hWnd))
    {
        ErrorExit(I18N(L"Failed to create a window"), (unsigned long long)hWnd);
    }

    DVERIFY(PostMessage(hParentWnd, WM_APP_WORKERREADY, 0, 0));

    ThreadData threadData(hParentProcess);
    std::thread thKillme(threadMain, &threadData);

    ClickerData clickerData(x, y,hParentWnd);
    std::thread thClicker(clickerMain, &clickerData);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

