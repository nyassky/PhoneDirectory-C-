#ifndef CONTACT_H
#define CONTACT_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <ctime>

using namespace std;
// Структура для хранения контакта
struct Contact {
    int id;
    string first_name;
    string last_name;
    string patronymic;
    string address;
    string birth_date; // Формат: Day-Month-Year
    string email;
    vector<string> phones;
};

#endif // CONTACT_H