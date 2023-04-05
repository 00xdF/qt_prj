#pragma once

#include <QtWidgets/QWidget>
#include "ui_qt_login_widget.h"

class qt_login_widget : public QWidget
{
    Q_OBJECT

public:
    qt_login_widget(QWidget *parent = nullptr);
    ~qt_login_widget();


private:
    Ui::qt_login_widgetClass ui;
};
