#include "PhonebookDatabase.h"
#include <iostream>
using namespace std;
PhonebookDatabase::PhonebookDatabase() : db(nullptr) {}

PhonebookDatabase::~PhonebookDatabase() {
    closeDatabase();
}

bool PhonebookDatabase::openDatabase(const string& db_name) {
    int rc = sqlite3_open(db_name.c_str(), &db);
    return rc == SQLITE_OK;
}

void PhonebookDatabase::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool PhonebookDatabase::createTable() {
    const char* create_table_sql =
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "first_name TEXT, "
        "last_name TEXT, "
        "patronymic TEXT, "
        "address TEXT, "
        "birth_date TEXT, "
        "email TEXT, "
        "phones TEXT);";

    char* err_msg = nullptr;
    int rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << err_msg << endl;
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}


