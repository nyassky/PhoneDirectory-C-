#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PhoneDirectoryQt.h"

class PhoneDirectoryQt : public QMainWindow
{
    Q_OBJECT

public:
    PhoneDirectoryQt(QWidget *parent = nullptr);
    ~PhoneDirectoryQt();

private:
    Ui::PhoneDirectoryQtClass ui;
};
