#ifndef PHONEDIRECTORYQT_H
#define PHONEDIRECTORYQT_H
#include <QFile>
#include <QTextStream>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
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
    void saveToFile();     // Новый слот
    void loadFromFile();   // Новый слот

private:
    QTableWidget* contactTable;
    Phonebook phonebook;   // Хранение контактов
};


#endif // PHONEDIRECTORYQT_H