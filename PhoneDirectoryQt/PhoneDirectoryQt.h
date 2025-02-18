#ifndef PHONEDIRECTORYQT_H
#define PHONEDIRECTORYQT_H
#include <QFile>
#include <QTextStream>
#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QString> 
#include <QtWidgets/QMainWindow>
#include "ui_PhoneDirectoryQt.h"
#include "Phonebook.h" 
#include "Validator.h"
#include "PhoneDirectoryForm.h"

class PhoneDirectoryQt : public QMainWindow {
    Q_OBJECT

public:
    PhoneDirectoryQt(QWidget* parent = nullptr);
    ~PhoneDirectoryQt();

private slots:
    void addContact();
    void editContact();
    void deleteContact();
    void sortContacts();
    void searchContact();
    void saveToFile();     // ����� ����
    void loadFromFile();   // ����� ����
    void addContactDB();  // DB
    void updateContactTable();  // DB
    void searchContactDB();  // DB
    void deleteContactDB(); //DB
    void sortContactDB(); //DB
    void editContactDB(); //DB
private:
    QTableWidget* contactTable;
    Phonebook phonebook;   // �������� ���������
    PhonebookDatabase phonebookDatabase;
};


#endif // PHONEDIRECTORYQT_H