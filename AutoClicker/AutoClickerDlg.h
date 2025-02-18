
// AutoClickerDlg.h : header file
//

#pragma once


// CAutoClickerDlg dialog
class CAutoClickerDlg : public CDialogEx
{
	bool m_bGettingPosition = false;
	Ambiesoft::HashIniHandle initialIni_;
	HANDLE hClicker_ = nullptr;

	void ToggleWorker(bool bStart);
	void ToggleWorker();
	void AppendLog(const wchar_t* pMessage);
// Construction
public:
	CAutoClickerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTOCLICKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CBrush m_brBkgnd;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGetPosition();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strEditX;
	CString m_strEditY;
	afx_msg void OnBnClickedButtonStartClicking();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnWokerReady(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWokerFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWokerError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWokerClicked(WPARAM wParam, LPARAM lParam);
	CButton m_btnStart;
	CEdit m_edtLog;
	CString m_strGetPositionMessage;
};
