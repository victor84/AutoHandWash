#pragma once
#include "stdafx.h"
#include "lock_vector.h"
#include "raw_data_warappers.h"
#include "AsyncClientSocket.h"

// �������� ������ ���������
class CTestLogic
{
	// �������� ����� ������
	tools::lock_vector<tools::data_wrappers::_tag_data_const>& _received_data;

	// ��� �������� ������
	tools::networking::CAsyncClientSocket& _device_socket;



public:
	CTestLogic(tools::lock_vector<tools::data_wrappers::_tag_data_const>& received_data,
			   tools::networking::CAsyncClientSocket& client_socket);
	~CTestLogic();





};

