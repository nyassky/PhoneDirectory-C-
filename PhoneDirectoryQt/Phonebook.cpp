#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <ctime>
#include "Contact.cpp"

using namespace std;

// Проверка корректности данных
class Validator {
public:
    static bool validateName(const string& name) {
        // Убираем пробелы в начале и конце
        string trimmed_name = name;
        trimmed_name.erase(0, trimmed_name.find_first_not_of(" \t\n\r"));
        trimmed_name.erase(trimmed_name.find_last_not_of(" \t\n\r") + 1);

        // Регулярное выражение для проверки имени
        regex name_regex(R"(^[A-Za-zА-Яа-яЁё]+([ '-][A-Za-zА-Яа-яЁё]+)*$)");
        return regex_match(trimmed_name, name_regex);
    }


    static bool validatePhone(const string& phone) {
        regex phone_regex(R"(^\+?7\d{10}$|^8\d{10}$)");
        return regex_match(phone, phone_regex);
    }

    static bool validateEmail(const string& email) {
        regex email_regex(R"(^[A-Za-z0-9](\.?[A-Za-z0-9_-])*@[A-Za-z0-9-]+(\.[A-Za-z]{2,})+$)");
        return regex_match(email, email_regex);
    }

    static bool validateDate(const string& date) {
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

};

// Функции для работы с контактами
class Phonebook {
    vector<Contact> contacts;
    int next_id = 1;

public:
    void addContact(const Contact& contact) {
        Contact new_contact = contact;
        new_contact.id = next_id++;
        contacts.push_back(new_contact);
    }

    void removeContact(int id) {
        contacts.erase(remove_if(contacts.begin(), contacts.end(),
            [id](const Contact& c) { return c.id == id; }),
            contacts.end());
    }

    void listContacts() const {
        for (const auto& contact : contacts) {
            cout << "ID: " << contact.id << "\n"
                << "Name: " << contact.first_name << "\n"
                << "Surname: " << contact.last_name << "\n"
                << "Patronymic: " << contact.patronymic << "\n"
                << "Address: " << contact.address << "\n"
                << "Date of birth: " << contact.birth_date << "\n"
                << "Email: " << contact.email << "\n"
                << "Phone numbers: ";
            for (const auto& phone : contact.phones) {
                cout << phone << " ";
            }
            cout << "\n\n";
        }
    }

    Contact* findContact(int id) {
        for (auto& contact : contacts) {
            if (contact.id == id) {
                return &contact;
            }
        }
        return nullptr;
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "The file could not be opened for writing." << endl;
            return;
        }

        for (const auto& contact : contacts) {
            file << contact.id << "," << contact.first_name << "," << contact.last_name << ","
                << contact.patronymic << "," << contact.address << "," << contact.birth_date << ","
                << contact.email << ",";
            for (size_t i = 0; i < contact.phones.size(); ++i) {
                file << contact.phones[i];
                if (i < contact.phones.size() - 1) {
                    file << ";";
                }
            }
            file << "\n";
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "The file could not be opened for reading." << endl;
            return;
        }

        contacts.clear(); // Очистить текущие контакты
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue; // Пропуск пустых строк

            istringstream ss(line);
            Contact contact;
            string phones;

            // Чтение данных из строки
            string id_str;
            getline(ss, id_str, ',');
            contact.id = stoi(id_str);

            getline(ss, contact.first_name, ',');
            getline(ss, contact.last_name, ',');
            getline(ss, contact.patronymic, ',');
            getline(ss, contact.address, ',');
            getline(ss, contact.birth_date, ',');
            getline(ss, contact.email, ',');

            // Чтение телефонов
            getline(ss, phones);
            istringstream phone_stream(phones);
            string phone;
            while (getline(phone_stream, phone, ';')) {
                contact.phones.push_back(phone);
            }

            // Добавление контакта в список
            contacts.push_back(contact);

            // Обновление следующего ID
            next_id = max(next_id, contact.id + 1);
        }

        cout << "Contacts loaded successfully!" << endl;

        // Вывод загруженных контактов
        listContacts();
    }

    void editContact(int id) {
        Contact* contact = findContact(id);
        if (!contact) {
            cout << "Contact with ID " << id << " not found.\n";
            return;
        }

        cout << "Editing contact with ID: " << id << "\n";
        cout << "1. Name\n2. Surname\n3. Patronymic\n4. Address\n5. Date of Birth\n6. Email\n7. Phones\n";
        cout << "Enter the number of the field to edit: ";
        int field;
        cin >> field;
        cin.ignore();

        switch (field) {
        case 1: {
            cout << "Enter new name: ";
            string new_name;
            getline(cin, new_name);
            if (Validator::validateName(new_name)) {
                contact->first_name = new_name;
                cout << "Name updated successfully!\n";
            }
            else {
                cout << "Invalid name. Update failed.\n";
            }
            break;
        }
        case 2: {
            cout << "Enter new surname: ";
            string new_surname;
            getline(cin, new_surname);
            if (Validator::validateName(new_surname)) {
                contact->last_name = new_surname;
                cout << "Surname updated successfully!\n";
            }
            else {
                cout << "Invalid surname. Update failed.\n";
            }
            break;
        }
        case 3: {
            cout << "Enter new patronymic: ";
            string new_patronymic;
            getline(cin, new_patronymic);
            if (Validator::validateName(new_patronymic)) {
                contact->patronymic = new_patronymic;
                cout << "Patronymic updated successfully!\n";
            }
            else {
                cout << "Invalid patronymic. Update failed.\n";
            }
            break;
        }
        case 4: {
            cout << "Enter new address: ";
            string new_address;
            getline(cin, new_address);
            contact->address = new_address;
            cout << "Address updated successfully!\n";
            break;
        }
        case 5: {
            cout << "Enter new date of birth (DD-MM-YYYY): ";
            string new_date;
            getline(cin, new_date);
            if (Validator::validateDate(new_date)) {
                contact->birth_date = new_date;
                cout << "Date of birth updated successfully!\n";
            }
            else {
                cout << "Invalid date. Update failed.\n";
            }
            break;
        }
        case 6: {
            cout << "Enter new email: ";
            string new_email;
            getline(cin, new_email);
            if (Validator::validateEmail(new_email)) {
                contact->email = new_email;
                cout << "Email updated successfully!\n";
            }
            else {
                cout << "Invalid email. Update failed.\n";
            }
            break;
        }
        case 7: {
            cout << "Enter new phone numbers (separated by spaces): ";
            string phones;
            getline(cin, phones);
            istringstream ss(phones);
            vector<string> new_phones;
            string phone;
            bool valid = true;
            while (ss >> phone) {
                if (Validator::validatePhone(phone)) {
                    new_phones.push_back(phone);
                }
                else {
                    cout << "Invalid phone number: " << phone << ". Update failed.\n";
                    valid = false;
                    break;
                }
            }
            if (valid) {
                contact->phones = new_phones;
                cout << "Phone numbers updated successfully!\n";
            }
            break;
        }
        default:
            cout << "Invalid choice.\n";
        }
    }
    void sortContacts(const string& field) {
        if (field == "name") {
            sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
                return a.first_name < b.first_name;
                });
        }
        else if (field == "surname") {
            sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
                return a.last_name < b.last_name;
                });
        }
        else if (field == "email") {
            sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
                return a.email < b.email;
                });
        }
        else {
            cout << "Invalid field for sorting.\n";
        }
    }

    vector<Contact> searchContacts(const string& query) {
        vector<Contact> results;
        for (const auto& contact : contacts) {
            if (contact.first_name.find(query) != string::npos ||
                contact.last_name.find(query) != string::npos ||
                contact.email.find(query) != string::npos ||
                any_of(contact.phones.begin(), contact.phones.end(), [&query](const string& phone) {
                    return phone.find(query) != string::npos;
                    })) {
                results.push_back(contact);
            }
        }
        return results;
    }
};