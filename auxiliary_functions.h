#ifndef AUXILIARY_FUNCTIONS_H
#include <iostream>
#include "Knot.h"
#include <vector>
#include <fstream>
#include <set>
#include <cctype>
#include <QMessageBox>
#include <regex>
#include <regex>
#include <string>
#include "iomanip"
#define AUXILIARY_FUNCTIONS_H


inline bool isValidDirectionCode(const std::string &str) {
    std::regex pattern(R"(^\d{1}[1-9]\.\d{1}[1-9]\.\d{1}[1-9]$)");
    return std::regex_match(str, pattern);
}

bool isValidDisciplineName(const std::string &input) {
    std::regex pattern(R"(^[A-Za-z]+(?:_[A-Za-z]+)*$)");
    return std::regex_match(input, pattern);
}

bool isValidFullName(const std::string &input) {
    std::regex pattern(R"(^[A-Z][a-z]*_[A-Z][a-z]*_[A-Z][a-z]*$)");
    return std::regex_match(input, pattern);
}

inline int CheckCorrectField(std::string DirCode, std::string DispName, std::string FullName){
    if (!isValidDirectionCode(DirCode)){
        return -1;
    }else if (!isValidDisciplineName(DispName)){
        return -2;
    }else if (!isValidFullName(FullName)){
        return -3;
    }else return 1;
}

inline void ErrorHandler(int status){
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Critical);
switch (status){
    case -1:Warn.setText("Неправильный формат кода направления. (AA.BB.CC)");break;
    case -2:Warn.setText("Неправильный формат названия направления.(Software_Engineering)");break;
    case -3:Warn.setText("Неправильный формат ФИО. (Ivanov_Ivan_Ivanovich)");break;
}
    Warn.exec();
}


#endif // AUXILIARY_FUNCTIONS_H
