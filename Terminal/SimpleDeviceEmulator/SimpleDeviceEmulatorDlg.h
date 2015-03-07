
// SimpleDeviceEmulatorDlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"
#include "AsyncClientSocket.h"


// ���������� ���� CSimpleDeviceEmulatorDlg
class CSimpleDeviceEmulatorDlg : public CDialogEx
{
	// ����, �������� �������������
	CString m_PortValue;

	// ������ ����� �������
	CComboBox m_PacketTypeCombo;

	// ������ ������
	CString m_PacketData;

	// ���
	CString m_Log;

	tools::networking::CAsyncClientSocket _client_socket;

	tools::lock_vector<tools::data_wrappers::_tag_data_const> _received_data;

	tools::logging::CTraceError* _tr_error;

	tools::lock_vector<std::wstring> _log_messages;

	void show_log();

// ��������
public:
	CSimpleDeviceEmulatorDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_SIMPLEDEVICEEMULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedConnectButton();

public:
	afx_msg void OnClose();
};
