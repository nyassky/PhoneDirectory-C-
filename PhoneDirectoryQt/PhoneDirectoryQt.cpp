#include "PhoneDirectoryQt.h"


PhoneDirectoryQt::PhoneDirectoryQt(QWidget* parent) : QMainWindow(parent) {
    // Настройка основного виджета и макета
    QWidget* centralWidget = new QWidget(this);
    QGridLayout* mainLayout = new QGridLayout(centralWidget);

    // Кнопки для действий
    QGroupBox* buttonGroup = new QGroupBox("Actions", this);
    QVBoxLayout* buttonLayout = new QVBoxLayout(buttonGroup);

    QPushButton* addButton = new QPushButton("Add Contact", this);
    QPushButton* editButton = new QPushButton("Edit Contact", this);
    QPushButton* deleteButton = new QPushButton("Delete Contact", this);
    QPushButton* sortButton = new QPushButton("Sort Contacts", this);
    QPushButton* searchButton = new QPushButton("Search", this);
    QPushButton* saveButton = new QPushButton("Save to File", this);
    QPushButton* loadButton = new QPushButton("Load from File", this);

    QPushButton* addButtonDB = new QPushButton("Add Contact (DB)", this);
    QPushButton* editButtonDB = new QPushButton("Edit Contact (DB)", this);
    QPushButton* searchButtonDB = new QPushButton("Search Contact (DB)", this);
    QPushButton* deleteButtonDB = new QPushButton("Delete Contact (DB)", this);
    QPushButton* sortContactDB = new QPushButton("Sort Contacts (DB)", this);
    QPushButton* updateButton = new QPushButton("Refresh Contacts (DB)", this);
   

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(sortButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);

    buttonLayout->addWidget(addButtonDB);
    buttonLayout->addWidget(editButtonDB);
    buttonLayout->addWidget(searchButtonDB);
    buttonLayout->addWidget(deleteButtonDB);
    buttonLayout->addWidget(sortContactDB);
    buttonLayout->addWidget(updateButton);

    buttonGroup->setLayout(buttonLayout);

    // Таблица для отображения контактов
    contactTable = new QTableWidget(this);
    contactTable->setColumnCount(7);
    contactTable->setHorizontalHeaderLabels({ "First Name", "Last Name", "Patronymic", "Phone", "Address", "Email", "Birth Date" });
    contactTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contactTable->horizontalHeader()->setStretchLastSection(true);
    contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    contactTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Добавляем элементы в макет
    mainLayout->addWidget(buttonGroup, 0, 0);
    mainLayout->addWidget(contactTable, 0, 1);

    // Устанавливаем основной виджет для QMainWindow
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Подключение сигналов кнопок к слотам
    connect(addButton, &QPushButton::clicked, this, &PhoneDirectoryQt::addContact);
    connect(editButton, &QPushButton::clicked, this, &PhoneDirectoryQt::editContact);
    connect(deleteButton, &QPushButton::clicked, this, &PhoneDirectoryQt::deleteContact);
    connect(sortButton, &QPushButton::clicked, this, &PhoneDirectoryQt::sortContacts);
    connect(searchButton, &QPushButton::clicked, this, &PhoneDirectoryQt::searchContact);
    connect(saveButton, &QPushButton::clicked, this, &PhoneDirectoryQt::saveToFile);
    connect(loadButton, &QPushButton::clicked, this, &PhoneDirectoryQt::loadFromFile);

    connect(addButtonDB, &QPushButton::clicked, this, &PhoneDirectoryQt::addContactDB);
    connect(editButtonDB, &QPushButton::clicked, this, &PhoneDirectoryQt::editContactDB);
    connect(searchButtonDB, &QPushButton::clicked, this, &PhoneDirectoryQt::searchContactDB);
    connect(deleteButtonDB, &QPushButton::clicked, this, &PhoneDirectoryQt::deleteContactDB);
    connect(sortContactDB, &QPushButton::clicked, this, &PhoneDirectoryQt::sortContactDB);
    connect(updateButton, &QPushButton::clicked, this, &PhoneDirectoryQt::updateContactTable);
    
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

        // Валидация
        if (firstName.isEmpty() || !Validator::validateName(firstName.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid name!");
            return;
        }
        else if (lastName.isEmpty() || !Validator::validateName(lastName.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid surname!");
            return;

        } 
        else if (patronymic.isEmpty() || !Validator::validateName(patronymic.toStdString())) {
            QMessageBox::warning(this, "Error", "Invalid patronymic!");
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

        // Добавление в таблицу
        int row = contactTable->rowCount();
        contactTable->insertRow(row);
        contactTable->setItem(row, 0, new QTableWidgetItem(firstName));
        contactTable->setItem(row, 1, new QTableWidgetItem(lastName));
        contactTable->setItem(row, 2, new QTableWidgetItem(patronymic));
        contactTable->setItem(row, 3, new QTableWidgetItem(phone));
        contactTable->setItem(row, 4, new QTableWidgetItem(address));
        contactTable->setItem(row, 5, new QTableWidgetItem(email));
        contactTable->setItem(row, 6, new QTableWidgetItem(birthDate));

        // Добавление в объект Phonebook
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
    // Проверяем, выбрана ли строка для редактирования
    QList<QTableWidgetItem*> selectedItems = contactTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Edit Contact", "Please select a contact to edit.");
        return;
    }

    int row = contactTable->currentRow(); // Получаем номер строки

    bool ok;

    // Вводим и проверяем данные для редактирования 
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

    // Обновляем данные в таблице на основе изменений
    contactTable->item(row, 0)->setText(first_name);
    contactTable->item(row, 1)->setText(last_name);
    contactTable->item(row, 3)->setText(phones);
    contactTable->item(row, 5)->setText(email);
    contactTable->item(row, 6)->setText(birth_date);

    // Обновляем данные в объекте Phonebook
    auto& contacts = phonebook.getContacts(); // Получаем ссылку на список контактов
    if (row < contacts.size()) {
        Contact& contact = contacts[row]; // Получаем контакт по строке
        contact.first_name = first_name.toStdString();
        contact.last_name = last_name.toStdString();
        contact.phones = { phones.toStdString() };
        contact.email = email.toStdString();
        contact.birth_date = birth_date.toStdString();
    }

    // Сохраняем обновленные данные в файл
    saveToFile(); 
}


void PhoneDirectoryQt::deleteContact() {
    // Получаем номер выбранной строки
    int row = contactTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Delete Contact", "Please select a contact to delete.");
        return;
    }

    // Получаем список всех контактов
    auto& contacts = phonebook.getContacts();

    // Удаляем контакт из данных
    if (row < contacts.size()) {
        contacts.erase(contacts.begin() + row);
    }

    // Удаляем строку из таблицы
    contactTable->removeRow(row);

    // Перезаписываем файл без удалённого контакта
    phonebook.saveToFile("contacts.txt");

    QMessageBox::information(this, "Delete Contact", "Contact deleted successfully.");
}

void PhoneDirectoryQt::searchContact() {
    // Окно ввода для поиска
    bool ok;
    QString keyword = QInputDialog::getText(this, "Search Contact",
        "Enter Name, Surname, Phones, Address, Email, or Date of Birth:", QLineEdit::Normal, "", &ok);

    if (!ok || keyword.isEmpty()) return;

    bool found = false; // Флаг для проверки, найден ли контакт

    // Проходим по всем строкам таблицы
    for (int i = 0; i < contactTable->rowCount(); ++i) {
        bool matches = contactTable->item(i, 0)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 1)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 2)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 3)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 4)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 5)->text().contains(keyword, Qt::CaseInsensitive) ||
            contactTable->item(i, 6)->text().contains(keyword, Qt::CaseInsensitive);

        if (matches) {
            
            contactTable->selectRow(i); // Выделяем строку
            found = true; // Фиксируем, что контакт найден
        }
        else {
            // Убираем выделение
            contactTable->setRowHidden(i, false); // Не скрываем остальные строки
        }
    }

    if (found) {
        // Показать диалог с найденным пользователем
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

        // Отобразим найденного пользователя
        QMessageBox::information(this, "Contact Found", contactDetails);
    }
    else {
        // Если контакт не найден
        QMessageBox::information(this, "Search Result", "No contacts found matching your search criteria.");
    }
}


void PhoneDirectoryQt::saveToFile() {
    // Открываем файл для записи
    QFile file("contacts.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save Contacts", "Could not open the file for saving.");
        return;
    }

    QTextStream out(&file);

    // Проходим по всем контактам и записываем их в файл
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
                out << ";"; // Разделяем несколько номеров телефона
            }
        }
        out << "\n"; // Каждому контакту новая строка
    }

    file.close(); // Закрываем файл

    QMessageBox::information(this, "Save Contacts", "Contacts saved successfully!");
}


void PhoneDirectoryQt::loadFromFile() {
    QFile file("contacts.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Load Contacts", "Could not open the file for reading.");
        return;
    }

    QTextStream in(&file);
    contactTable->setRowCount(0); // Очистим таблицу перед загрузкой данных

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        // Проверка, что строка имеет все необходимые поля
        if (fields.size() < 7) {
            continue; // Пропустить строку, если она неполная
        }

        Contact contact;
        contact.first_name = fields[0].toStdString();
        contact.last_name = fields[1].toStdString();
        contact.patronymic = fields[2].toStdString();
        contact.address = fields[3].toStdString();
        contact.birth_date = fields[4].toStdString();
        contact.email = fields[5].toStdString();

        // Чтение телефонов (могут быть несколько, разделенных ";")
        QString phone_str = fields[6];
        QStringList phones = phone_str.split(";");
        for (const QString& phone : phones) {
            contact.phones.push_back(phone.toStdString());
        }

        // Добавляем контакт в объект Phonebook
        phonebook.addContact(contact);

        // Отображаем контакт в таблице
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
    QStringList fields = { "Name", "Surname", "Email" }; // Поля сортировки
    QString field = QInputDialog::getItem(this, "Sort Contacts", "Select field to sort by:", fields, 0, false, &ok);

    if (!ok || field.isEmpty()) return;

    phonebook.sortContacts(field.toStdString()); 
    loadFromFile(); // Обновить отображение
}


void PhoneDirectoryQt::addContactDB() {
    PhoneDirectoryForm form(this);
    if (form.exec() == QDialog::Accepted) {
        QString firstName = form.getFirstName().trimmed();
        QString lastName = form.getLastName().trimmed();
        QString patronymic = form.getPatronymic().trimmed();
        QString phone = form.getPhone().trimmed();
        QString address = form.getAddress().trimmed();
        QString email = form.getEmail().trimmed();
        QString birthDate = form.getBirthDate().trimmed();

        // Список ошибок
        QStringList errors;

        if (!Validator::validateName(firstName.toStdString())) {
            errors << "Invalid first name.";
        }
        if (!Validator::validateName(lastName.toStdString())) {
            errors << "Invalid last name.";
        }
        if (!Validator::validateName(patronymic.toStdString())) {
            errors << "Invalid patronymic.";
        }
        if (!Validator::validatePhone(phone.toStdString())) {
            errors << "Invalid phone number.";
        }
        if (!Validator::validateEmail(email.toStdString())) {
            errors << "Invalid email address.";
        }
        if (!Validator::validateDate(birthDate.toStdString())) {
            errors << "Invalid birth date format (use DD-MM-YYYY).";
        }

        // Если есть ошибки – показываем их и выходим
        if (!errors.isEmpty()) {
            QMessageBox::critical(this, "Validation Error", errors.join("\n"));
            return;
        }

        // Открываем базу
        sqlite3* db;
        if (sqlite3_open("contacts.db", &db) != SQLITE_OK) {
            QMessageBox::critical(this, "Database Error", "Failed to open database");
            return;
        }

        const char* sql = "INSERT INTO contacts (first_name, last_name, patronymic, phones, address, email, birth_date) "
            "VALUES (?, ?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            QMessageBox::critical(this, "Database Error", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        // Привязываем данные
        sqlite3_bind_text(stmt, 1, firstName.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, lastName.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, patronymic.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, phone.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, address.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, email.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, birthDate.toUtf8().constData(), -1, SQLITE_TRANSIENT);

        // Выполняем запрос
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            QMessageBox::critical(this, "Database Error", sqlite3_errmsg(db));
        }

        // Завершаем работу с базой
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        updateContactTable(); // Обновляем таблицу после добавления
    }
}



void PhoneDirectoryQt::updateContactTable() {
    sqlite3* db;
    if (sqlite3_open("contacts.db", &db) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", "Failed to open database");
        return;
    }

    contactTable->setRowCount(0); // Очищаем таблицу перед обновлением

    const char* sql = "SELECT first_name, last_name, patronymic, phones, address, email, birth_date FROM contacts;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int row = contactTable->rowCount();
            contactTable->insertRow(row);

            for (int col = 0; col < 7; ++col) { // 7 колонок без ID
                contactTable->setItem(row, col, new QTableWidgetItem(
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, col))
                ));
            }
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void PhoneDirectoryQt::searchContactDB() {
    QString searchQuery = QInputDialog::getText(this, "Search Contact", "Enter name, phone, or email:");
    if (searchQuery.isEmpty()) return;

    sqlite3* db;
    if (sqlite3_open("contacts.db", &db) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", "Failed to open database");
        return;
    }

    const char* sql = "SELECT first_name, last_name, patronymic, phones, address, email, birth_date FROM contacts "
        "WHERE first_name LIKE ? OR last_name LIKE ? OR phones LIKE ? OR email LIKE ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    QString searchPattern = "%" + searchQuery + "%";
    sqlite3_bind_text(stmt, 1, searchPattern.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, searchPattern.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, searchPattern.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, searchPattern.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        QString firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        QString lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        QString patronymic = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        QString phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        QString address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        QString email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        QString birthDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        QString result = QString("Name: %1 %2 %3\nPhone: %4\nAddress: %5\nEmail: %6\nBirth Date: %7")
            .arg(firstName, lastName, patronymic, phone, address, email, birthDate);

        QMessageBox::information(this, "Contact Found", result);
    }
    else {
        QMessageBox::warning(this, "Search", "Contact not found.");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void PhoneDirectoryQt::deleteContactDB() {
    int row = contactTable->currentRow(); // Получаем выбранную строку
    if (row < 0) {
        QMessageBox::warning(this, "Delete Contact", "Please select a contact to delete.");
        return;
    }
    // Подтверждение удаления
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Contact", "Are you sure you want to delete this contact?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }
    sqlite3* db;
    if (sqlite3_open("contacts.db", &db) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", "Failed to open database");
        return;
    }

    // Получаем номер телефона (или email) из выделенной строки - это наш уникальный идентификатор
    QString phone = contactTable->item(row, 3)->text(); // 3 — это индекс колонки "Phone"

    const char* sql = "DELETE FROM contacts WHERE phones = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, phone.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        QMessageBox::critical(this, "Database Error", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    contactTable->removeRow(row); // Удаляем строку из таблицы
}

void PhoneDirectoryQt::sortContactDB() {
    bool ok;
    QStringList fields = { "First Name", "Last Name", "Email", "Phone" }; // Поля для сортировки
    QString field = QInputDialog::getItem(this, "Sort Contacts", "Select field to sort by:", fields, 0, false, &ok);

    if (!ok || field.isEmpty()) return;

    int column = -1;
    if (field == "First Name") column = 0;
    else if (field == "Last Name") column = 1;
    else if (field == "Email") column = 5;
    else if (field == "Phone") column = 3;

    if (column == -1) return;

    contactTable->sortItems(column, Qt::AscendingOrder);
    
}
void PhoneDirectoryQt::editContactDB() {
    // Проверяем, выбрана ли строка
    int row = contactTable->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Edit Contact", "Please select a contact to edit.");
        return;
    }

    // Получаем ID контакта 
    QString phone = contactTable->item(row, 3)->text();  // Уникальный идентификатор — номер телефона

    bool ok;
    QString firstName = QInputDialog::getText(this, "Edit Contact", "First Name:", QLineEdit::Normal, contactTable->item(row, 0)->text(), &ok);
    if (ok && (firstName.isEmpty() || !Validator::validateName(firstName.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid name!");
        return;
    }
    QString lastName = QInputDialog::getText(this, "Edit Contact", "Last Name:", QLineEdit::Normal, contactTable->item(row, 1)->text(), &ok);
    if (ok && (lastName.isEmpty() || !Validator::validateName(lastName.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid lastname!");
        return;
    }
    QString patronymic = QInputDialog::getText(this, "Edit Contact", "Patronymic:", QLineEdit::Normal, contactTable->item(row, 2)->text(), &ok);
    if (ok && (patronymic.isEmpty() || !Validator::validateName(patronymic.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid patronymic!");
        return;
    }
    QString newPhone = QInputDialog::getText(this, "Edit Contact", "Phone:", QLineEdit::Normal, phone, &ok);
    if (ok && (newPhone.isEmpty() || !Validator::validatePhone(newPhone.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid phone number!");
        return;
    }
    QString address = QInputDialog::getText(this, "Edit Contact", "Address:", QLineEdit::Normal, contactTable->item(row, 4)->text(), &ok);
    if (!ok) return;

    QString email = QInputDialog::getText(this, "Edit Contact", "Email:", QLineEdit::Normal, contactTable->item(row, 5)->text(), &ok);
    if (ok && (!email.isEmpty() && !Validator::validateEmail(email.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid Email Address!");
        return;
    }

    QString birthDate = QInputDialog::getText(this, "Edit Contact", "Birth Date:", QLineEdit::Normal, contactTable->item(row, 6)->text(), &ok);
    if (ok && (!birthDate.isEmpty() && !Validator::validateDate(birthDate.toStdString()))) {
        QMessageBox::warning(this, "Error", "Invalid Date of Birth! Use DD-MM-YYYY format.");
        return;
    }
    // Открываем базу данных
    sqlite3* db;
    if (sqlite3_open("contacts.db", &db) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", "Failed to open database");
        return;
    }

    // SQL-запрос для обновления данных
    const char* sql = "UPDATE contacts SET first_name = ?, last_name = ?, patronymic = ?, phones = ?, address = ?, email = ?, birth_date = ? WHERE phones = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        QMessageBox::critical(this, "Database Error", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Привязываем данные к запросу
    sqlite3_bind_text(stmt, 1, firstName.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, lastName.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, patronymic.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, newPhone.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, address.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, email.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, birthDate.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, phone.toUtf8().constData(), -1, SQLITE_TRANSIENT);  // Ищем старый телефон

    // Выполняем SQL-запрос
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        QMessageBox::critical(this, "Database Error", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Обновляем интерфейс
    updateContactTable();

    QMessageBox::information(this, "Edit Contact", "Contact updated successfully!");
}

