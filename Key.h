#pragma once
#include "Date.h"
#include "Direction_code.h"
#include <string>

struct Key {
	Direction_code direction_code; // Код направления
	std::string discipline_name; // название дисциплины
	std::string teacher_name; // ФИО преподавателя
	Date date; // дата
};
