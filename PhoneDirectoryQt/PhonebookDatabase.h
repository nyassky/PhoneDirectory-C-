#ifndef PHONEBOOKDATABASE_H
#define PHONEBOOKDATABASE_H

#include <sqlite3.h>
#include <vector>
#include "Contact.h"
#include "Validator.h"

class PhonebookDatabase {
private:
    sqlite3* db;
public:
    PhonebookDatabase();
    ~PhonebookDatabase();

    bool openDatabase(const std::string& db_name);
    void closeDatabase();
    bool createTable();
   
    
};

#endif // PHONEBOOKDATABASE_H
