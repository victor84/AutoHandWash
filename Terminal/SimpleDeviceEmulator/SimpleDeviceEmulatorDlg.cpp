
// SimpleDeviceEmulatorDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "SimpleDeviceEmulator.h"
#include "SimpleDeviceEmulatorDlg.h"
#include "afxdialogex.h"

#include "tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace device_exchange;

// вывод сообщения в интерфейс
#define LOG(str) { _tr_error->trace_message(str); show_log(); }

// обновление элементов UI
#define UPDATE_UI(save_and_validate) { this->SendMessage(WM_ON_CUSTOM_UPDATE_CONTROLS, save_and_validate, 0); }

void CSimpleDeviceEmulatorDlg::fill_packets_types()
{
	_packets_types.clear();

	_packets_types.push_back(_tag_packet_type(e_command_from_device::button_press,			_T("Нажата кнопка")));
	_packets_types.push_back(_tag_packet_type(e_command_from_device::bill_acceptor,			_T("Данные от купюроприемника или монетоприемника")));
	_packets_types.push_back(_tag_packet_type(e_command_from_device::hopper_issue_coin,		_T("Выдана монета хоппером")));
	_packets_types.push_back(_tag_packet_type(e_command_from_device::discount_card_issued,	_T("Выдана дисконтная карта")));
	_packets_types.push_back(_tag_packet_type(e_command_from_device::data_from_eeprom,		_T("Передать данные из EEPROM")));
	_packets_types.push_back(_tag_packet_type(e_command_from_device::buttons_state,			_T("Выдача статуса всех кнопок")));
	_packets_types.push_back(_tag_packet_type(e_command_from_device::command_confirmation,	_T("Подтверждение выполнения команды")));
	_packets_types.push_back(_tag_packet_type(e_command_from_device::error,					_T("Ошибка")));
}

void CSimpleDeviceEmulatorDlg::show_log()
{
	for (std::wstring str : _log_messages.get_with_cleanup())
	{
		m_Log.Append(str.c_str());
		m_Log.Append(_T("\r\n"));
	}

	UPDATE_UI(FALSE);
}

void CSimpleDeviceEmulatorDlg::show_packets_types()
{
	m_PacketTypeCombo.ResetContent();

	for (_tag_packet_type packet_type : _packets_types)
	{
		m_PacketTypeCombo.AddString(packet_type.name);
	}
}

LRESULT CSimpleDeviceEmulatorDlg::OnCustomUpdate(WPARAM update, LPARAM)
{
	UpdateData(update);

	SCROLLINFO si;
	ZeroMemory(&si, sizeof(si));
	m_LogControll.GetScrollInfo(SB_VERT, &si, SIF_RANGE);
	m_LogControll.LineScroll(si.nMax);

	return 1;
}

LRESULT CSimpleDeviceEmulatorDlg::OnAutoSend(WPARAM, LPARAM)
{
	OnBnClickedSendDataButton();

	return 1;
}

bool CSimpleDeviceEmulatorDlg::send_answer(std::wstring packet_hex)
{
	if (_T("0aa0000000000000a0a0") == packet_hex)
	{
		m_PacketData = _T("0a0a0000000000000aa0");
		return true;
	}

	if (_T("0aa0010000000000a1a0") == packet_hex)
	{
		m_PacketData = _T("0a0a0100000000000ba0");
		return true;
	}

	if (_T("0aa0020000000000a2a0") == packet_hex)
	{
		m_PacketData = _T("0a0a02000000000008a0");
		return true;
	}

	if (_T("0aa0030000000000a3a0") == packet_hex)
	{
		m_PacketData = _T("0a0a03000000000009a0");
		return true;
	}

	if (_T("0aa0040000000000a4a0") == packet_hex)
	{
		m_PacketData = _T("0a0a0401000000000fa0");
		return true;
	}

	if (_T("0aa0050000000000a5a0") == packet_hex)
	{
		m_PacketData = _T("0a0a050a0000000005a0");
		return true;
	}

	if (_T("0aa0060000000000a6a0") == packet_hex)
	{
		m_PacketData = _T("0a0a0601000000000da0");
		return true;
	}

	if (_T("0aa0070000000000a7a0") == packet_hex)
	{
		m_PacketData = _T("0a0a0701000000000ca0");
		return true;
	}

	if (_T("0aa0110000000000b1a0") == packet_hex)
	{
		m_PacketData = _T("0a0a11320000000029a0");
		return true;
	}

	if (_T("0aa0120000000000b2a0") == packet_hex)
	{
		m_PacketData = _T("0a0a1232000000002aa0");
		return true;
	}

	if (_T("0aa0130000000000b3a0") == packet_hex)
	{
		m_PacketData = _T("0a0a1332000000002ba0");
		return true;
	}

	if (_T("0aa0140000000000b4a0") == packet_hex)
	{
		m_PacketData = _T("0a0a1432000000002da0");
		return true;
	}

	if (_T("0aa0150000000000b5a0") == packet_hex)
	{
		m_PacketData = _T("0a0a1532000000002da0");
		return true;
	}

	if (_T("0aa0160000000000b6a0") == packet_hex)
	{
		m_PacketData = _T("0a0a1632000000002da0");
		return true;
	}

	if (_T("0aa0170000000000b7a0") == packet_hex)
	{
		m_PacketData = _T("0a0a1732000000002da0");
		return true;
	}

	if (_T("0aa0180000000000b8a0") == packet_hex)
	{
		m_PacketData = _T("0a0a1832000000002da0");
		return true;
	}

	if (_T("0aa021000000000081a0") == packet_hex)
	{
		m_PacketData = _T("0a0a21320000000019a0");
		return true;
	}

	if (_T("0aa022000000000082a0") == packet_hex)
	{
		m_PacketData = _T("0a0a22320000000019a0");
		return true;
	}

	if (_T("0aa023000000000083a0") == packet_hex)
	{
		m_PacketData = _T("0a0a23320000000019a0");
		return true;
	}

	if (_T("0aa024000000000084a0") == packet_hex)
	{
		m_PacketData = _T("0a0a24320000000019a0");
		return true;
	}

	if (_T("0aa025000000000085a0") == packet_hex)
	{
		m_PacketData = _T("0a0a25320000000019a0");
		return true;
	}

	if (_T("0aa026000000000086a0") == packet_hex)
	{
		m_PacketData = _T("0a0a26320000000019a0");
		return true;
	}

	if (_T("0aa027000000000087a0") == packet_hex)
	{
		m_PacketData = _T("0a0a27320000000019a0");
		return true;
	}

	if (_T("0aa028000000000088a0") == packet_hex)
	{
		m_PacketData = _T("0a0a28320000000019a0");
		return true;
	}

	return false;
}

CSimpleDeviceEmulatorDlg::_tag_packet_type CSimpleDeviceEmulatorDlg::get_selected_packet_type(INT sel_num)
{
	_tag_packet_type result = _packets_types[sel_num];
	return result;
}

// диалоговое окно CSimpleDeviceEmulatorDlg

CSimpleDeviceEmulatorDlg::CSimpleDeviceEmulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimpleDeviceEmulatorDlg::IDD, pParent)
	, m_PortValue(_T("26000"))
	, m_PacketData(_T(""))
	, m_Log(_T(""))
	, _client_socket(_received_data, 
					std::bind(std::mem_fn(&CSimpleDeviceEmulatorDlg::on_data_received_fn), this, std::placeholders::_1),
					nullptr,
					nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	_tr_error = tools::logging::CTraceError::get_instance();
	_tr_error->set_messages_storages(&_log_messages, nullptr);
}

void CSimpleDeviceEmulatorDlg::on_data_received_fn(tools::data_wrappers::_tag_data_managed data)
{
	std::wstring hex_string = tools::binary_to_hex(data);

	LOG(_T("Приняты данные: ") + hex_string);

	if (true == send_answer(hex_string))
	{
		UPDATE_UI(FALSE);
		this->PostMessage(WM_ON_AUTO_SEND_DATA, 0, 0);
	}
}

void CSimpleDeviceEmulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT_EDIT, m_PortValue);
	DDX_Control(pDX, IDC_PACKET_TYPE_COMBO, m_PacketTypeCombo);
	DDX_Text(pDX, IDC_PACKET_DATA_EDIT, m_PacketData);
	DDX_Text(pDX, IDC_LOG_EDIT, m_Log);
	DDX_Control(pDX, IDC_LOG_EDIT, m_LogControll);
}

BEGIN_MESSAGE_MAP(CSimpleDeviceEmulatorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CSimpleDeviceEmulatorDlg::OnBnClickedConnectButton)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_PACKET_TYPE_COMBO, &CSimpleDeviceEmulatorDlg::OnCbnSelchangePacketTypeCombo)
	ON_BN_CLICKED(IDC_SEND_DATA_BUTTON, &CSimpleDeviceEmulatorDlg::OnBnClickedSendDataButton)
	ON_MESSAGE(WM_ON_CUSTOM_UPDATE_CONTROLS, &CSimpleDeviceEmulatorDlg::OnCustomUpdate)
	ON_MESSAGE(WM_ON_AUTO_SEND_DATA, &CSimpleDeviceEmulatorDlg::OnAutoSend)
END_MESSAGE_MAP()


// обработчики сообщений CSimpleDeviceEmulatorDlg

BOOL CSimpleDeviceEmulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	fill_packets_types();
	show_packets_types();

	m_PacketTypeCombo.SetCurSel(0);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSimpleDeviceEmulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSimpleDeviceEmulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimpleDeviceEmulatorDlg::OnBnClickedConnectButton()
{
	UPDATE_UI(TRUE);

	tools::networking::tag_connection_params connection_params;
	connection_params.address = "127.0.0.1";
	connection_params.port = tools::wstring_to_string(m_PortValue.GetString());
	connection_params.reconnection_timeout = 10000;

	if (tools::networking::e_socket_result::success == _client_socket.OpenConnection(connection_params))
		_tr_error->trace_message(_T("Подключено"));
	else
		_tr_error->trace_error(_T("Не удалось подключиться"));

	show_log();
}

void CSimpleDeviceEmulatorDlg::OnClose()
{
	_client_socket.CloseConnection();

	delete _tr_error;
	_tr_error = nullptr;

	CDialogEx::OnClose();
}

void CSimpleDeviceEmulatorDlg::OnCbnSelchangePacketTypeCombo()
{
	INT selected_item = m_PacketTypeCombo.GetCurSel();

	if (CB_ERR == selected_item)
		return;

	_current_packet_type = get_selected_packet_type(selected_item);
	_packet_from_device.clear();
	_packet_from_device.command = _current_packet_type.command;
	tools::data_wrappers::_tag_data_managed raw_data;
	_packet_convertor.CreateRawData(_packet_from_device, raw_data);
	m_PacketData = tools::binary_to_hex(raw_data).c_str();

	UPDATE_UI(FALSE);
}

void CSimpleDeviceEmulatorDlg::OnBnClickedSendDataButton()
{
	UPDATE_UI(TRUE);

	std::wstring packet_hex = m_PacketData.GetString();
	tools::data_wrappers::_tag_data_managed raw_data;
	raw_data = tools::hex_to_binary(packet_hex);

	if (nullptr == raw_data.p_data)
	{
		LOG(_T("Неверный формат данных"));
		return;
	}

	if (e_convert_result::success != _packet_convertor.ParseWithUpdateCheckSum(raw_data, _packet_from_device))
	{
		LOG(_T("Неверный формат пакета"));
		return;
	}

	_packet_convertor.CreateRawData(_packet_from_device, raw_data);
	packet_hex = tools::binary_to_hex(raw_data);
	_client_socket.PushBackToSend(raw_data);
	LOG(_T("Отправлен пакет данных: ") + packet_hex);

	m_PacketData = packet_hex.c_str();
	UPDATE_UI(FALSE);
}

void CSimpleDeviceEmulatorDlg::OnOK()
{
	// TODO: добавьте специализированный код или вызов базового класса

	// CDialogEx::OnOK();
}
