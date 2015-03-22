
// SimpleDeviceEmulatorDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
#include "AsyncClientSocket.h"
#include "device_protocol.h"
#include "DevicePacketConvertor.h"

#define WM_ON_CUSTOM_UPDATE_CONTROLS WM_USER + 50

// диалоговое окно CSimpleDeviceEmulatorDlg
class CSimpleDeviceEmulatorDlg : public CDialogEx
{
	// тип пакета от контроллера
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

	// Порт, введённый пользователем
	CString m_PortValue;

	// список типов пакетов
	CComboBox m_PacketTypeCombo;

	// данные пакета
	CString m_PacketData;

	// Лог
	CString m_Log;

	// Элемент для отображения лога
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
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangePacketTypeCombo();
	afx_msg void OnBnClickedSendDataButton();

	virtual void OnOK();
};
