#pragma once

#include <QtWidgets/QWidget>
#include "ui_qt_draw_xy.h"

class qt_draw_xy : public QWidget
{
    Q_OBJECT

public:
    qt_draw_xy(QWidget *parent = nullptr);
    ~qt_draw_xy();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::qt_draw_xyClass ui;
};
