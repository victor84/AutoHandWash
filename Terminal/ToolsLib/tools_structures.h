#pragma once

/************************************************************************
��������� � ������������ ������ ����������
************************************************************************/

namespace tools
{
// ������ ���������� �������� �����
enum class e_work_loop_status
{
	ok,		// ���� ������������
	error,	// �������� ������
	stop	// ������� ������������ �������
};

// ��������� �������������
enum class e_init_state
{
	not_init,	// ������������� �� ���� �����������
	was_init	// ������������� �������
};

}