
// SimpleDeviceEmulatorDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "SimpleDeviceEmulator.h"
#include "SimpleDeviceEmulatorDlg.h"
#include "afxdialogex.h"

#include "tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CSimpleDeviceEmulatorDlg::show_log()
{
	for (std::wstring str : _log_messages.get_with_cleanup())
	{
		m_Log.Append(str.c_str());
		m_Log.Append(_T("\r\n"));
	}

	UpdateData(FALSE);
}

// ���������� ���� CSimpleDeviceEmulatorDlg

CSimpleDeviceEmulatorDlg::CSimpleDeviceEmulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimpleDeviceEmulatorDlg::IDD, pParent)
	, m_PortValue(_T("26000"))
	, m_PacketData(_T(""))
	, _client_socket(_received_data)
	, m_Log(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	_tr_error = tools::logging::CTraceError::get_instance();

	_tr_error->set_messages_storages(&_log_messages, nullptr);


}

void CSimpleDeviceEmulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT_EDIT, m_PortValue);
	DDX_Control(pDX, IDC_PACKET_TYPE_COMBO, m_PacketTypeCombo);
	DDX_Text(pDX, IDC_PACKET_DATA_EDIT, m_PacketData);
	DDX_Text(pDX, IDC_LOG_EDIT, m_Log);
}

BEGIN_MESSAGE_MAP(CSimpleDeviceEmulatorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CSimpleDeviceEmulatorDlg::OnBnClickedConnectButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// ����������� ��������� CSimpleDeviceEmulatorDlg

BOOL CSimpleDeviceEmulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ ������ ��� ����� ����������� ����.  ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������.  ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CSimpleDeviceEmulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CSimpleDeviceEmulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimpleDeviceEmulatorDlg::OnBnClickedConnectButton()
{
	UpdateData(TRUE);

	tools::networking::tag_connection_params connection_params;
	connection_params.address = "127.0.0.1";
	connection_params.port = tools::wstring_to_string(m_PortValue.GetString());
	connection_params.reconnection_timeout = 10000;

	if (tools::networking::e_socket_result::success == _client_socket.OpenConnection(connection_params))
		_tr_error->trace_message(_T("����������"));
	else
		_tr_error->trace_error(_T("�� ������� ������������"));

	show_log();
}

void CSimpleDeviceEmulatorDlg::OnClose()
{
	_client_socket.CloseConnection();

	delete _tr_error;
	_tr_error = nullptr;

	CDialogEx::OnClose();
}
