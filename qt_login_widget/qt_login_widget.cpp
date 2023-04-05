#include "qt_login_widget.h"
#include <qstring.h>
#include <qmessagebox.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

qt_login_widget::qt_login_widget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("登录");
    connect(ui.pushButton_login, &QPushButton::clicked, [this] {
        QString username = ui.lineEdit_username->text();
        QString password = ui.lineEdit_password->text();
        if (username == "ct" && password == "123456") {
            QMessageBox::information(this,"警告", "登录成功！");
        }
        else {
            QMessageBox::warning(this, "警告", "登录失败！");
        }
    });
    
}

qt_login_widget::~qt_login_widget()
{}
