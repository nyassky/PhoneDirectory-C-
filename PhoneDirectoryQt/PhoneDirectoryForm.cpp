#include "PhoneDirectoryForm.h"

PhoneDirectoryForm::PhoneDirectoryForm(QWidget* parent) : QDialog(parent) {
    // Используем QFormLayout для улучшенной структуры
    QFormLayout* formLayout = new QFormLayout(this);

    // Поля ввода с метками
    firstNameEdit = new QLineEdit(this);
    lastNameEdit = new QLineEdit(this);
    patronymicEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    birthDateEdit = new QLineEdit(this);

    firstNameEdit->setPlaceholderText("Enter Name");
    lastNameEdit->setPlaceholderText("Enter Surname");
    patronymicEdit->setPlaceholderText("Enter Patronymic (Optional)");
    addressEdit->setPlaceholderText("Enter Address (Optional)");
    emailEdit->setPlaceholderText("Enter Email (Optional)");
    birthDateEdit->setPlaceholderText("Enter Date of Birth (DD-MM-YYYY)");
    phoneEdit->setPlaceholderText("Enter Phone Numbers");

    // Устанавливаем минимальную ширину полей ввода
    firstNameEdit->setMinimumWidth(300);
    lastNameEdit->setMinimumWidth(300);
    patronymicEdit->setMinimumWidth(300);
    phoneEdit->setMinimumWidth(300);
    addressEdit->setMinimumWidth(300);
    emailEdit->setMinimumWidth(300);
    birthDateEdit->setMinimumWidth(300);

    // Добавляем элементы в форму с метками
    formLayout->addRow("First Name:", firstNameEdit);
    formLayout->addRow("Last Name:", lastNameEdit);
    formLayout->addRow("Patronymic:", patronymicEdit);
    formLayout->addRow("Phone Number:", phoneEdit);
    formLayout->addRow("Address:", addressEdit);
    formLayout->addRow("Email:", emailEdit);
    formLayout->addRow("Date of Birth:", birthDateEdit);

    // Кнопки для сохранения и отмены
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);

    
    saveButton->setMinimumWidth(100);
    cancelButton->setMinimumWidth(100);

    // Добавляем кнопки в горизонтальный layout
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    // Добавляем кнопки в главный layout
    formLayout->addRow(buttonLayout);

    // Устанавливаем основной layout
    setLayout(formLayout);

    // Сигналы
    connect(saveButton, &QPushButton::clicked, this, &PhoneDirectoryForm::accept);
    connect(cancelButton, &QPushButton::clicked, this, &PhoneDirectoryForm::reject);
}

void PhoneDirectoryForm::setContactData(const QString& firstName, const QString& lastName, const QString& patronymic,
    const QString& phone, const QString& address, const QString& email, const QString& birthDate) {
    firstNameEdit->setText(firstName);
    lastNameEdit->setText(lastName);
    patronymicEdit->setText(patronymic);
    phoneEdit->setText(phone);
    addressEdit->setText(address);
    emailEdit->setText(email);
    birthDateEdit->setText(birthDate);
}

QString PhoneDirectoryForm::getFirstName() const { return firstNameEdit->text(); }
QString PhoneDirectoryForm::getLastName() const { return lastNameEdit->text(); }
QString PhoneDirectoryForm::getPatronymic() const { return patronymicEdit->text(); }
QString PhoneDirectoryForm::getPhone() const { return phoneEdit->text(); }
QString PhoneDirectoryForm::getAddress() const { return addressEdit->text(); }
QString PhoneDirectoryForm::getEmail() const { return emailEdit->text(); }
QString PhoneDirectoryForm::getBirthDate() const { return birthDateEdit->text(); }
