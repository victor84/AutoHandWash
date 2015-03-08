
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
					std::bind(std::mem_fn(&CSimpleDeviceEmulatorDlg::on_data_received_fn), this, std::placeholders::_1))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	_tr_error = tools::logging::CTraceError::get_instance();
	_tr_error->set_messages_storages(&_log_messages, nullptr);
}

void CSimpleDeviceEmulatorDlg::on_data_received_fn(tools::data_wrappers::_tag_data_managed data)
{
	std::wstring hex_string = tools::binary_to_hex(data);

	LOG(_T("Приняты данные: ") + hex_string);
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

	_client_socket.Send(raw_data);
	LOG(_T("Отправлен пакет данных: ") + packet_hex);

	m_PacketData = packet_hex.c_str();
	UPDATE_UI(FALSE);
}

void CSimpleDeviceEmulatorDlg::OnOK()
{
	// TODO: добавьте специализированный код или вызов базового класса

	// CDialogEx::OnOK();
}
