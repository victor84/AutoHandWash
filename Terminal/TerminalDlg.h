
// TerminalDlg.h : ���� ���������
//

#include "Tools/Logging/TraceError.h"

#pragma once


// ���������� ���� CTerminalDlg
class CTerminalDlg : public CDialog
{
	tools::logging::CTraceError* _tr_error;

// ��������
public:
	CTerminalDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_TERMINAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
