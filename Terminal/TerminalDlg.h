
// TerminalDlg.h : файл заголовка
//

#include "Tools/Logging/TraceError.h"

#pragma once


// диалоговое окно CTerminalDlg
class CTerminalDlg : public CDialog
{
	tools::logging::CTraceError* _tr_error;

// Создание
public:
	CTerminalDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_TERMINAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
