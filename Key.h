#pragma once
#include "Date.h"
#include "Direction_code.h"
#include <string>

struct Key {
	Direction_code direction_code; // ��� �����������
	std::string discipline_name; // �������� ����������
	std::string teacher_name; // ��� �������������
	Date date; // ����
};
