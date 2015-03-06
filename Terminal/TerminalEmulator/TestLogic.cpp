#include "stdafx.h"
#include "TestLogic.h"


CTestLogic::CTestLogic(tools::lock_vector<tools::data_wrappers::_tag_data_const>& received_data,
					   tools::networking::CAsyncClientSocket& client_socket)
					   : _received_data(received_data)
					   , _device_socket(client_socket)
{
}

CTestLogic::~CTestLogic()
{
}
