#pragma once

#include <QtWidgets/QWidget>
#include "ui_qt_calc.h"

class qt_calc : public QWidget
{
    Q_OBJECT

public:
    qt_calc(QWidget *parent = nullptr);
    ~qt_calc();

    
    
private slots:
    void digistPressed();
    void equals_Pressed();
    void back_Pressed();
    void clear_Pressed();
    void qiuDao();
    void sqrtNum();
    

private:
    Ui::qt_calcClass ui;
    QString expression;

};
