/********************************************************************************
** Form generated from reading UI file 'PhoneDirectoryQt.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHONEDIRECTORYQT_H
#define UI_PHONEDIRECTORYQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PhoneDirectoryQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PhoneDirectoryQtClass)
    {
        if (PhoneDirectoryQtClass->objectName().isEmpty())
            PhoneDirectoryQtClass->setObjectName(QString::fromUtf8("PhoneDirectoryQtClass"));
        PhoneDirectoryQtClass->resize(600, 400);
        menuBar = new QMenuBar(PhoneDirectoryQtClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        PhoneDirectoryQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PhoneDirectoryQtClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        PhoneDirectoryQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PhoneDirectoryQtClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        PhoneDirectoryQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PhoneDirectoryQtClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PhoneDirectoryQtClass->setStatusBar(statusBar);

        retranslateUi(PhoneDirectoryQtClass);

        QMetaObject::connectSlotsByName(PhoneDirectoryQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *PhoneDirectoryQtClass)
    {
        PhoneDirectoryQtClass->setWindowTitle(QApplication::translate("PhoneDirectoryQtClass", "PhoneDirectoryQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PhoneDirectoryQtClass: public Ui_PhoneDirectoryQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHONEDIRECTORYQT_H
