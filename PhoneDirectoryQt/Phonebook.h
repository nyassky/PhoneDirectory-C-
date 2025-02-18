#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <vector>
#include <string>
#include "Contact.h"
#include "PhonebookDatabase.h"
using namespace std;
class Phonebook {
    vector<Contact> contacts;
    int next_id = 1;
    PhonebookDatabase db;

public:
    Phonebook();

    void addContact(const Contact& contact);
    void removeContact(int id);
    void listContacts() const;
    Contact* findContact(int id);
    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
    void editContact(int id);
    void sortContacts(const string& field);
    vector<Contact> searchContacts(const string& query);
    const vector<Contact>& getContacts() const;
    vector<Contact>& getContacts();

   
};

#endif // PHONEBOOK_H
