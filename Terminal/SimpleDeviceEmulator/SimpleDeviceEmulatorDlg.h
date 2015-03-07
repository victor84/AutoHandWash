
// SimpleDeviceEmulatorDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
#include "AsyncClientSocket.h"


// диалоговое окно CSimpleDeviceEmulatorDlg
class CSimpleDeviceEmulatorDlg : public CDialogEx
{
	// Порт, введённый пользователем
	CString m_PortValue;

	// список типов пакетов
	CComboBox m_PacketTypeCombo;

	// данные пакета
	CString m_PacketData;

	// Лог
	CString m_Log;

	tools::networking::CAsyncClientSocket _client_socket;

	tools::lock_vector<tools::data_wrappers::_tag_data_const> _received_data;

	tools::logging::CTraceError* _tr_error;

	tools::lock_vector<std::wstring> _log_messages;

	void show_log();

// Создание
public:
	CSimpleDeviceEmulatorDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_SIMPLEDEVICEEMULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedConnectButton();

public:
	afx_msg void OnClose();
};
