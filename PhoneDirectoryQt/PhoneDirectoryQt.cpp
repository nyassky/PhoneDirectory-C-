#include "PhoneDirectoryQt.h"


PhoneDirectoryQt::PhoneDirectoryQt(QWidget* parent) : QMainWindow(parent) {

    // ��������� ��������� ������� � ������
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);


    // ������� ��� ����������� ���������
    contactTable = new QTableWidget(0, 7, this); 
    contactTable->setHorizontalHeaderLabels({ "Name", "Surname", "Patronymic", "Phone Number", "Address", "Email", "Date of Birth" });
    contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    contactTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(contactTable);

    // ������ ��� ��������
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* addButton = new QPushButton("Add Contact", this);
    QPushButton* editButton = new QPushButton("Edit Contact", this);
    QPushButton* deleteButton = new QPushButton("Delete Contact", this);
    QPushButton* sortButton = new QPushButton("Sort Contacts", this);
    QPushButton* searchButton = new QPushButton("Search", this);
    QPushButton* saveButton = new QPushButton("Save to File", this);
    QPushButton* loadButton = new QPushButton("Load from File", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(sortButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);

    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    // ����������� �������� ������ � ������
    connect(addButton, &QPushButton::clicked, this, &PhoneDirectoryQt::addContact);
    connect(editButton, &QPushButton::clicked, this, &PhoneDirectoryQt::editContact);
    connect(deleteButton, &QPushButton::clicked, this, &PhoneDirectoryQt::deleteContact);
    connect(sortButton, &QPushButton::clicked, this, &PhoneDirectoryQt::sortContacts);
    connect(searchButton, &QPushButton::clicked, this, &PhoneDirectoryQt::searchContact);
    connect(saveButton, &QPushButton::clicked, this, &PhoneDirectoryQt::saveToFile);
    connect(loadButton, &QPushButton::clicked, this, &PhoneDirectoryQt::loadFromFile);
}

PhoneDirectoryQt::~PhoneDirectoryQt() {
    
}

void PhoneDirectoryQt::addContact() {
    PhoneDirectoryForm form(this);

    if (form.exec() == QDialog::Accepted) {
        QString firstName = form.getFirstName();
        QString lastName = form.getLastName();
        QString patronymic = form.getPatronymic();
        QString phone = form.getPhone();
        QString address = form.getAddress();
        QString email = form.getEmail();
        QString birthDate = form.getBirthDate();

        // ���������
        if (firstName.isEmpty() || !Validator::validateName(firstName.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid name!");
            return;
        }
        else if (lastName.isEmpty() || !Validator::validateName(lastName.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid surname!");
            return;

        } 
        else if (!Validator::validatePhone(phone.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid phones!");
            return;
        } 
        else if (!email.isEmpty() && !Validator::validateEmail(email.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid email!");
            return;
        }
        else if (!birthDate.isEmpty() && !Validator::validateDate(birthDate.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid birth date!");
            return;
        }

        // ���������� � �������
        int row = contactTable->rowCount();
        contactTable->insertRow(row);
        contactTable->setItem(row, 0, new QTableWidgetItem(firstName));
        contactTable->setItem(row, 1, new QTableWidgetItem(lastName));
        contactTable->setItem(row, 2, new QTableWidgetItem(patronymic));
        contactTable->setItem(row, 3, new QTableWidgetItem(phone));
        contactTable->setItem(row, 4, new QTableWidgetItem(address));
        contactTable->setItem(row, 5, new QTableWidgetItem(email));
        contactTable->setItem(row, 6, new QTableWidgetItem(birthDate));

        // ���������� � ������ Phonebook
        Contact newContact;
        newContact.first_name = firstName.toStdString();
        newContact.last_name = lastName.toStdString();
        newContact.patronymic = patronymic.toStdString();
        newContact.phones.push_back(phone.toStdString());
        newContact.address = address.toStdString();
        newContact.email = email.toStdString();
        newContact.birth_date = birthDate.toStdString();
        phonebook.addContact(newContact);
    }
}


void PhoneDirectoryQt::editContact() {
    // ���������, ������� �� ������ ��� ��������������
    QList<QTableWidgetItem*> selectedItems = contactTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Edit Contact", "Please select a contact to edit.");
        return;
    }

    int row = contactTable->currentRow(); // �������� ����� ������

    bool ok;

    // ������ � ��������� ������ ��� �������������� 
    QString first_name = QInputDialog::getText(this, "Edit Contact", "Edit name:", QLineEdit::Normal, contactTable->item(row, 0)->text(), &ok);
    if (ok && (first_name.isEmpty() || !Validator::validateName(first_name.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid name!");
        return;
    }

    QString last_name = QInputDialog::getText(this, "Edit Contact", "Edit Surname:", QLineEdit::Normal, contactTable->item(row, 1)->text(), &ok);
    if (ok && (last_name.isEmpty() || !Validator::validateName(last_name.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid Last Name!");
        return;
    }

    QString phones = QInputDialog::getText(this, "Edit Contact", "Edit Phone Numbers:", QLineEdit::Normal, contactTable->item(row, 3)->text(), &ok);
    if (ok && !Validator::validatePhone(phones.toStdString())) {
        QMessageBox::warning(this, "Error", "Invalid Phone Number! Must start with +7 or 8 and contain 10 digits.");
        return;
    }

    QString email = QInputDialog::getText(this, "Edit Contact", "Edit Email:", QLineEdit::Normal, contactTable->item(row, 5)->text(), &ok);
    if (ok && (!email.isEmpty() && !Validator::validateEmail(email.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid Email Address!");
        return;
    }

    QString birth_date = QInputDialog::getText(this, "Edit Contact", "Edit Date of Birth:", QLineEdit::Normal, contactTable->item(row, 6)->text(), &ok);
    if (ok && (!birth_date.isEmpty() && !Validator::validateDate(birth_date.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid Date of Birth! Use DD-MM-YYYY format.");
        return;
    }

    // ��������� ������ � ������� �� ������ ���������
    contactTable->item(row, 0)->setText(first_name);
    contactTable->item(row, 1)->setText(last_name);
    contactTable->item(row, 3)->setText(phones);
    contactTable->item(row, 5)->setText(email);
    contactTable->item(row, 6)->setText(birth_date);

    // ��������� ������ � ������� Phonebook
    auto& contacts = phonebook.getContacts(); // �������� ������ �� ������ ���������
    if (row < contacts.size()) {
        Contact& contact = contacts[row]; // �������� ������� �� ������
        contact.first_name = first_name.toStdString();
        contact.last_name = last_name.toStdString();
        contact.phones = { phones.toStdString() };
        contact.email = email.toStdString();
        contact.birth_date = birth_date.toStdString();
    }

    // ��������� ����������� ������ � ����
    saveToFile(); 
}


void PhoneDirectoryQt::deleteContact() {
    // �������� ����� ��������� ������
    int row = contactTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Delete Contact", "Please select a contact to delete.");
        return;
    }

    // �������� ������ ���� ���������
    auto& contacts = phonebook.getContacts();

    // ������� ������� �� ������
    if (row < contacts.size()) {
        contacts.erase(contacts.begin() + row);
    }

    // ������� ������ �� �������
    contactTable->removeRow(row);

    // �������������� ���� ��� ��������� ��������
    phonebook.saveToFile("contacts.txt");

    QMessageBox::information(this, "Delete Contact", "Contact deleted successfully.");
}

void PhoneDirectoryQt::searchContact() {
    // ���� ����� ��� ������
    bool ok;
    QString keyword = QInputDialog::getText(this, "Search Contact",
        "Enter Name, Surname, Phones, Address, Email, or Date of Birth:", QLineEdit::Normal, "", &ok);

    if (!ok || keyword.isEmpty()) return;

    bool found = false; // ���� ��� ��������, ������ �� �������

    // �������� �� ���� ������� �������
    for (int i = 0; i < contactTable->rowCount(); ++i) {
        bool matches = contactTable->item(i, 0)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 1)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 2)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 3)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 4)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 5)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 6)->text().contains(keyword, Qt::CaseInsensitive);

        if (matches) {
            
            contactTable->selectRow(i); // �������� ������
            found = true; // ���������, ��� ������� ������
        }
        else {
            // ������� ���������
            contactTable->setRowHidden(i, false); // �� �������� ��������� ������
        }
    }

    if (found) {
        // �������� ������ � ��������� �������������
        int row = contactTable->currentRow();
        QString firstName = contactTable->item(row, 0)->text();
        QString lastName = contactTable->item(row, 1)->text();
        QString patronymic = contactTable->item(row, 2)->text();
        QString phone = contactTable->item(row, 3)->text();
        QString address = contactTable->item(row, 4)->text();
        QString email = contactTable->item(row, 5)->text();
        QString birthDate = contactTable->item(row, 6)->text();

        QString contactDetails = QString("Name: %1\nSurname: %2\nPatronymic: %3\nPhone: %4\nAddress: %5\nEmail: %6\nBirth Date: %7")
            .arg(firstName).arg(lastName).arg(patronymic).arg(phone).arg(address).arg(email).arg(birthDate);

        // ��������� ���������� ������������
        QMessageBox::information(this, "Contact Found", contactDetails);
    }
    else {
        // ���� ������� �� ������
        QMessageBox::information(this, "Search Result", "No contacts found matching your search criteria.");
    }
}


void PhoneDirectoryQt::saveToFile() {
    // ��������� ���� ��� ������
    QFile file("contacts.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save Contacts", "Could not open the file for saving.");
        return;
    }

    QTextStream out(&file);

    // �������� �� ���� ��������� � ���������� �� � ����
    for (const auto& contact : phonebook.getContacts()) {
        out << QString::fromStdString(contact.first_name) << ","
            << QString::fromStdString(contact.last_name) << ","
            << QString::fromStdString(contact.patronymic) << ","
            << QString::fromStdString(contact.address) << ","
            << QString::fromStdString(contact.birth_date) << ","
            << QString::fromStdString(contact.email) << ",";

        for (size_t i = 0; i < contact.phones.size(); ++i) {
            out << QString::fromStdString(contact.phones[i]);
            if (i < contact.phones.size() - 1) {
                out << ";"; // ��������� ��������� ������� ��������
            }
        }
        out << "\n"; // ������� �������� ����� ������
    }

    file.close(); // ��������� ����

    QMessageBox::information(this, "Save Contacts", "Contacts saved successfully!");
}


void PhoneDirectoryQt::loadFromFile() {
    QFile file("contacts.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Load Contacts", "Could not open the file for reading.");
        return;
    }

    QTextStream in(&file);
    contactTable->setRowCount(0); // ������� ������� ����� ��������� ������

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        // ��������, ��� ������ ����� ��� ����������� ����
        if (fields.size() < 7) {
            continue; // ���������� ������, ���� ��� ��������
        }

        Contact contact;
        contact.first_name = fields[0].toStdString();
        contact.last_name = fields[1].toStdString();
        contact.patronymic = fields[2].toStdString();
        contact.address = fields[3].toStdString();
        contact.birth_date = fields[4].toStdString();
        contact.email = fields[5].toStdString();

        // ������ ��������� (����� ���� ���������, ����������� ";")
        QString phone_str = fields[6];
        QStringList phones = phone_str.split(";");
        for (const QString& phone : phones) {
            contact.phones.push_back(phone.toStdString());
        }

        // ��������� ������� � ������ Phonebook
        phonebook.addContact(contact);

        // ���������� ������� � �������
        int row = contactTable->rowCount();
        contactTable->insertRow(row);
        contactTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(contact.first_name)));
        contactTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(contact.last_name)));
        contactTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(contact.patronymic)));
        contactTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(contact.phones.empty() ? "" : contact.phones[0])));
        contactTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(contact.address)));
        contactTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(contact.email)));
        contactTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(contact.birth_date)));
    }

    file.close();
    QMessageBox::information(this, "Load Contacts", "Contacts loaded successfully!");
}


void PhoneDirectoryQt::sortContacts() {
    bool ok;
    QStringList fields = { "Name", "Surname", "Email" }; // ���� ����������
    QString field = QInputDialog::getItem(this, "Sort Contacts", "Select field to sort by:", fields, 0, false, &ok);

    if (!ok || field.isEmpty()) return;

    phonebook.sortContacts(field.toStdString()); 
    loadFromFile(); // �������� �����������
}

