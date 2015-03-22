
// SimpleDeviceEmulatorDlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"
#include "AsyncClientSocket.h"
#include "device_protocol.h"
#include "DevicePacketConvertor.h"

#define WM_ON_CUSTOM_UPDATE_CONTROLS WM_USER + 50

// ���������� ���� CSimpleDeviceEmulatorDlg
class CSimpleDeviceEmulatorDlg : public CDialogEx
{
	// ��� ������ �� �����������
	struct _tag_packet_type 
	{
		device_exchange::e_command_from_device	command;
		CString									name;

		_tag_packet_type(device_exchange::e_command_from_device	command, CString name)
		{
			this->command	= command;
			this->name		= name;
		}

		_tag_packet_type()
			: command(device_exchange::e_command_from_device::empty)
			, name(_T(""))
		{
		}
	};

	// ����, �������� �������������
	CString m_PortValue;

	// ������ ����� �������
	CComboBox m_PacketTypeCombo;

	// ������ ������
	CString m_PacketData;

	// ���
	CString m_Log;

	// ������� ��� ����������� ����
	CEdit m_LogControll;

	tools::networking::CAsyncClientSocket _client_socket;

	tools::lock_vector<tools::data_wrappers::_tag_data_managed> _received_data;

	tools::logging::CTraceError* _tr_error;

	tools::lock_vector<std::wstring> _log_messages;

	std::vector<_tag_packet_type> _packets_types;

	_tag_packet_type _current_packet_type;

	_tag_packet_type get_selected_packet_type(INT sel_num);

	device_exchange::tag_packet_from_device _packet_from_device;

	device_exchange::CDevicePacketConvertor<device_exchange::tag_packet_from_device> _packet_convertor;

	void on_data_received_fn(tools::data_wrappers::_tag_data_managed data);

	void fill_packets_types();

	void show_log();

	void show_packets_types();

	LRESULT OnCustomUpdate(WPARAM update, LPARAM);

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
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangePacketTypeCombo();
	afx_msg void OnBnClickedSendDataButton();

	virtual void OnOK();
};
