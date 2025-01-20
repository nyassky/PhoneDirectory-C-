#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <ctime>
#include "Validator.h"

// Проверка корректности данных

bool Validator::validateName(const string& name) {
    // Убираем пробелы в начале и конце
    string trimmed_name = name;
    trimmed_name.erase(0, trimmed_name.find_first_not_of(" \t\n\r"));
    trimmed_name.erase(trimmed_name.find_last_not_of(" \t\n\r") + 1);

    // Регулярное выражение для проверки имени
    regex name_regex(R"(^[A-Za-zА-Яа-яЁё]+([ '-][A-Za-zА-Яа-яЁё]+)*$)");
    return regex_match(trimmed_name, name_regex);
}


bool Validator::validatePhone(const string& phone) {
    regex phone_regex(R"(^\+?7\d{10}$|^8\d{10}$)");
    return regex_match(phone, phone_regex);
}

bool Validator::validateEmail(const string& email) {
    regex email_regex(R"(^[A-Za-z0-9](\.?[A-Za-z0-9_-])*@[A-Za-z0-9-]+(\.[A-Za-z]{2,})+$)");
    return regex_match(email, email_regex);
}

bool Validator::validateDate(const string& date) {
    // Формат даты: день-месяц-год
    regex date_regex(R"(^\d{2}-(0[1-9]|1[0-2])-(\d{4})$)");
    if (!regex_match(date, date_regex)) {
       return false;
    }

    // Проверка корректности даты
    int day, month, year;
    char delim1, delim2;
    istringstream date_stream(date);
    date_stream >> day >> delim1 >> month >> delim2 >> year;

    if (delim1 != '-' || delim2 != '-') {
        return false;
    }

    // Проверка существования даты
    struct tm timeinfo = { 0 };
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;

    if (mktime(&timeinfo) == -1) {
        return false;
    }

    return (timeinfo.tm_year + 1900 == year &&
        timeinfo.tm_mon + 1 == month &&
        timeinfo.tm_mday == day);
}

