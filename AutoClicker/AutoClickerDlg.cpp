
// AutoClickerDlg.cpp : implementation file
//

#include "pch.h"
#include "resource.h"
#include "framework.h"
#include "../common/common.h"

#include "AutoClicker.h"
#include "AutoClickerDlg.h"
#include "afxdialogex.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

// CAutoClickerDlg dialog

CAutoClickerDlg::CAutoClickerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AUTOCLICKER_DIALOG, pParent)
	, m_strEditX(_T(""))
	, m_strEditY(_T(""))
	, m_strGetPositionMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	try
	{
		initialIni_ = Profile::ReadAll(std::wstring((LPCWSTR)theApp.GetIniFile()), true);
		int x, y;
		bool ok = true;
		ok &= Profile::GetInt(SECTION_OPTION, KEY_MOUSE_X, 0, x, initialIni_);
		ok &= Profile::GetInt(SECTION_OPTION, KEY_MOUSE_Y, 0, y, initialIni_);
		if (!ok)
		{
			AfxMessageBox(I18N(L"Failed to read ini file"));
			ExitProcess(-1);
		}
		m_strEditX = stdToString(x).c_str();
		m_strEditY = stdToString(y).c_str();
	}
	catch (file_not_found_error)
	{

	}
	m_brBkgnd.CreateSolidBrush(RGB(145, 70, 42));
}

void CAutoClickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_strEditX);
	DDX_Text(pDX, IDC_EDIT_Y, m_strEditY);
	DDX_Control(pDX, IDC_BUTTON_START_CLICKING, m_btnStart);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Text(pDX, IDC_STATIC_GETPOSITION_MESSAGE, m_strGetPositionMessage);
}

BEGIN_MESSAGE_MAP(CAutoClickerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GET_POSITION, &CAutoClickerDlg::OnBnClickedButtonGetPosition)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_START_CLICKING, &CAutoClickerDlg::OnBnClickedButtonStartClicking)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_APP_WORKERREADY, &CAutoClickerDlg::OnWokerReady)
	ON_MESSAGE(WM_APP_WORKERFINISHED, &CAutoClickerDlg::OnWokerFinished)
	ON_MESSAGE(WM_APP_WORKERERROR, &CAutoClickerDlg::OnWokerError)
	ON_MESSAGE(WM_APP_WORKERCLICKED, &CAutoClickerDlg::OnWokerClicked)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAutoClickerDlg message handlers

BOOL CAutoClickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	LoadWindowLocation(*this, initialIni_);
	SetWindowText(AfxGetAppName());
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutoClickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoClickerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoClickerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAutoClickerDlg::OnBnClickedButtonGetPosition()
{
	m_bGettingPosition = true;
	
	m_strGetPositionMessage = I18N(L"Place mouse pointer on the target posion and press ESC");
	UpdateData(FALSE);
}


void CAutoClickerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
	
	if (!m_bGettingPosition)
		return;

	
}


BOOL CAutoClickerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			if (m_bGettingPosition)
			{
				m_bGettingPosition = false;
				m_strGetPositionMessage = L"";
				UpdateData(FALSE);
				POINT pos;
				if (!GetCursorPos(&pos))
				{
					AfxMessageBox(I18N(L"Failed to get mouse position"));
					return TRUE;
				}
				m_strEditX = stdToString(pos.x).c_str();
				m_strEditY = stdToString(pos.y).c_str();
				UpdateData(FALSE);
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_RETURN)
		{
			// NOT CLOSE
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAutoClickerDlg::OnBnClickedButtonStartClicking()
{
	ToggleWorker();
}


void CAutoClickerDlg::OnDestroy()
{
	UpdateData();
	int x = StrToInt(m_strEditX);
	int y = StrToInt(m_strEditY);
	
	Profile::CHashIni ini(Profile::ReadAll(std::wstring((LPCWSTR)theApp.GetIniFile())));
	bool ok = true;
	ok &= Profile::WriteInt(SECTION_OPTION, KEY_MOUSE_X, x, ini);
	ok &= Profile::WriteInt(SECTION_OPTION, KEY_MOUSE_Y, y, ini);
	ok &= SaveWindowLocation(*this, ini);
	if (!ok || !Profile::WriteAll(ini,std::wstring( (LPCWSTR)theApp.GetIniFile())))
	{
		AfxMessageBox(I18N(L"Failed to write to ini file"));
	}

	CDialogEx::OnDestroy();
}

LRESULT CAutoClickerDlg::OnWokerReady(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CAutoClickerDlg::ToggleWorker(bool bStart)
{
	struct stInvalidator {
		CAutoClickerDlg* pThis_;
		stInvalidator(CAutoClickerDlg* pThis) :pThis_(pThis) {}
		~stInvalidator() {
			// Makes backgroud refresh
			pThis_->Invalidate();
		}
	} invalidator(this);

	if(bStart)
	{
		UpdateData();

		POINT targetPos;
		targetPos.x = StrToInt(m_strEditX);
		targetPos.y = StrToInt(m_strEditY);

		std::wstring app = stdCombinePath(
			stdGetParentDirectory(stdGetModuleFileName()),
			WORKER_EXE_NAME);
		std::wstringstream arg;
		arg << L"-p " << GetCurrentProcessId() <<
			L" -w " << (ULONGLONG)m_hWnd <<
			L" -x " << targetPos.x <<
			L" -y " << targetPos.y;
		if (!OpenCommon(*this,
			app.c_str(),
			arg.str().c_str(),
			nullptr, // dir
			&hClicker_))
		{
			AfxMessageBox(I18N(L"Failed to create worker process"));
			return;
		}

		m_btnStart.SetWindowText(L"Stop");
		SetWindowText(stdFormat(L"%s | %s", I18N(L"CLICKING NOW"), AfxGetAppName()).c_str());
		m_edtLog.SetWindowText(L"");
		AppendLog(I18N(L"Started"));
	}
	else
	{
		if (hClicker_)
		{
			TerminateProcess(hClicker_, -1);
			CloseHandle(hClicker_);
			hClicker_ = nullptr;
		}
		m_btnStart.SetWindowText(L"&Start");
		SetWindowText(AfxGetAppName());
		AppendLog(I18N(L"Ended"));
	}
}
void CAutoClickerDlg::ToggleWorker()
{
	ToggleWorker(hClicker_ == nullptr);
	return;
}


LRESULT CAutoClickerDlg::OnWokerFinished(WPARAM wParam, LPARAM lParam)
{
	if (WAIT_OBJECT_0 == WaitForSingleObject(hClicker_, 10000))
	{
		CloseHandle(hClicker_);
		hClicker_ = nullptr;
	}
	ToggleWorker(false);
	AppendLog(I18N(L"Finished"));
	return 0;
}

void CAutoClickerDlg::AppendLog(const wchar_t* pMessage)
{

	if (!pMessage || pMessage[0] == 0)
		return;

	CString strLogLine;
	CTime time = CTime::GetCurrentTime();

	int nLength = m_edtLog.GetWindowTextLength();
	strLogLine.Format(_T("%s%s %s"),
		nLength==0 ? L"": L"\r\n",
		time.Format(_T("%H:%M:%S")), pMessage);
	

	// put the selection at the end of text
	m_edtLog.SetSel(nLength, nLength);

	// replace the selection
	m_edtLog.ReplaceSel(strLogLine);
}

LRESULT CAutoClickerDlg::OnWokerClicked(WPARAM wParam, LPARAM lParam)
{
	AppendLog( I18N(L"Clicked"));
	return 0;
}
LRESULT CAutoClickerDlg::OnWokerError(WPARAM wParam, LPARAM lParam)
{
	CString strLogLine;
	switch(wParam)
	{
	case WORKERERROR_LBUTTONPRESSED:
		strLogLine = I18N(L"Cancel clicking because LButton is pressed");
		break;
	case WORKERERROR_GETCURSORPOS:
		strLogLine = I18N(L"Failed to get mouse position");
		break;
	case WORKERERROR_SETCURSORPOS:
		strLogLine = I18N(L"failed to set mouse position");
		break;
	default:
		strLogLine = I18N(L"Unknown Error");
		break;
	}

	AppendLog(strLogLine);
	return 0;
}


HBRUSH CAutoClickerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		if (hClicker_ != nullptr)
			return (HBRUSH)m_brBkgnd;
		else
			return hbr;
	}

	return hbr;
}