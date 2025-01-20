#ifndef CONTACTFORM_H
#define CONTACTFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
class PhoneDirectoryForm : public QDialog {
    Q_OBJECT

public:
    explicit PhoneDirectoryForm(QWidget* parent = nullptr);
    void setContactData(const QString& firstName, const QString& lastName, const QString& patronymic,
        const QString& phone, const QString& address, const QString& email, const QString& birthDate);
    QString getFirstName() const;
    QString getLastName() const;
    QString getPatronymic() const;
    QString getPhone() const;
    QString getAddress() const;
    QString getEmail() const;
    QString getBirthDate() const;

private:
    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* patronymicEdit;
    QLineEdit* phoneEdit;
    QLineEdit* addressEdit;
    QLineEdit* emailEdit;
    QLineEdit* birthDateEdit;

    QPushButton* saveButton;
    QPushButton* cancelButton;
};

#endif // CONTACTFORM_H

