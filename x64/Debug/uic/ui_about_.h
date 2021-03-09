/********************************************************************************
** Form generated from reading UI file 'about_.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT__H
#define UI_ABOUT__H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_about_
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *about_)
    {
        if (about_->objectName().isEmpty())
            about_->setObjectName(QStringLiteral("about_"));
        about_->resize(400, 300);
        verticalLayout = new QVBoxLayout(about_);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textBrowser = new QTextBrowser(about_);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);


        retranslateUi(about_);

        QMetaObject::connectSlotsByName(about_);
    } // setupUi

    void retranslateUi(QWidget *about_)
    {
        about_->setWindowTitle(QApplication::translate("about_", "about_", nullptr));
        textBrowser->setHtml(QApplication::translate("about_", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:26pt;\">\350\277\231\346\230\257\350\267\257\347\272\277\350\247\204\345\210\222\357\274\201</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class about_: public Ui_about_ {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT__H
