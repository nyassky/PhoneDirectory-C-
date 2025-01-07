#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <ctime>
#include "Phonebook.cpp"
#include "PhoneDirectoryQt.h"
#include <QtWidgets/QApplication>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PhoneDirectoryQt w;
    w.show();
    return a.exec();

    Phonebook phonebook;
    int choice;

    do {
        cout << "\nPhone directory:\n"
            << "1. Add contact\n"
            << "2. Delete contact\n"
            << "3. Contact list\n"
            << "4. Sort contacts\n"
            << "5. Search contacts\n"
            << "6. Save to a file\n"
            << "7. Download from a file\n"
            << "8. Find contact\n"
            << "9. Edit contact\n"
            << "0. Exit\n"
            << "Your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Contact contact;
            cout << "Enter a name: ";
            getline(cin, contact.first_name);
            if (!Validator::validateName(contact.first_name)) {
                cout << "Invalid name. Please try again.\n";
                continue;
            }
            cout << "Enter a surname: ";
            getline(cin, contact.last_name);
            if (!Validator::validateName(contact.last_name)) {
                cout << "Invalid surname. Please try again.\n";
                continue;
            }
            cout << "Enter a patronymic: ";
            getline(cin, contact.patronymic);
            if (!Validator::validateName(contact.patronymic)) {
                cout << "Invalid patronymic. Please try again.\n";
                continue;
            }
            cout << "Enter an address: ";
            getline(cin, contact.address);
            cout << "Enter a date of birth (DD-MM-YYYY): ";
            getline(cin, contact.birth_date);
            if (!Validator::validateDate(contact.birth_date)) {
                cout << "Invalid date of birth. Please try again.\n";
                continue;
            }
            cout << "Enter an email: ";
            getline(cin, contact.email);
            if (!Validator::validateEmail(contact.email)) {
                cout << "Invalid email. Please try again.\n";
                continue;
            }
            cout << "Enter phone numbers (separated by spaces): ";
            string phones;
            getline(cin, phones);
            istringstream ss(phones);
            string phone;
            while (ss >> phone) {
                if (!Validator::validatePhone(phone)) {
                    cout << "Invalid phone number: " << phone << ". Please try again.\n";
                    continue;
                }
                contact.phones.push_back(phone);
            }
            phonebook.addContact(contact);
            cout << "Contact added successfully!\n";
        }
        else if (choice == 2) {
            int id;
            cout << "Enter the contact ID to delete: ";
            cin >> id;
            phonebook.removeContact(id);
            cout << "Contact with ID: " << id << " was deleted!";
        }
        else if (choice == 3) {
            phonebook.listContacts();
        }
        else if (choice == 4) {
            cout << "Enter field to sort by (name/surname/email): ";
            string field;
            cin >> field;
            phonebook.sortContacts(field);
            cout << "Contacts sorted by " << field << ".\n";
        }
        else if (choice == 5) {
            cout << "Enter search query: ";
            string query;
            cin >> query;
            vector<Contact> results = phonebook.searchContacts(query);
            if (!results.empty()) {
                cout << "Search results:\n";
                for (const auto& contact : results) {
                    cout << "ID: " << contact.id << "\n"
                        << "Name: " << contact.first_name << "\n"
                        << "Surname: " << contact.last_name << "\n"
                        << "Email: " << contact.email << "\n"
                        << "\n";
                }
            }
            else {
                cout << "No contacts found for query '" << query << "'.\n";
            }
        }
        else if (choice == 6) {
            phonebook.saveToFile("contacts.txt");
        }
        else if (choice == 7) {
            phonebook.loadFromFile("contacts.txt");
        }
        else if (choice == 8) {
            int idF;
            cout << "Enter the contact ID to find: ";
            cin >> idF;
            Contact* contact = phonebook.findContact(idF);
            if (contact) {
                cout << "Contact found:\n";
                cout << "ID: " << contact->id << "\n"
                    << "Name: " << contact->first_name << "\n"
                    << "Surname: " << contact->last_name << "\n"
                    << "Patronymic: " << contact->patronymic << "\n"
                    << "Address: " << contact->address << "\n"
                    << "Date of birth: " << contact->birth_date << "\n"
                    << "Email: " << contact->email << "\n"
                    << "Phone numbers: ";
                for (const auto& phone : contact->phones) {
                    cout << phone << " ";
                }
                cout << "\n";
            }
            else {
                cout << "Contact with ID " << idF << " not found.\n";
            }
        }
        else if (choice == 9) {
            int idE;
            cout << "Enter the contact ID to edit: ";
            cin >> idE;
            phonebook.editContact(idE);
        }
    } while (choice != 0);

    return 0;
}

