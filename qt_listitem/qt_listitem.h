#pragma once

#include <QtWidgets/QWidget>
#include "ui_qt_listitem.h"
#include <qset.h>

class qt_listitem : public QWidget
{
    Q_OBJECT

public:
    qt_listitem(QWidget *parent = nullptr);
    ~qt_listitem();

private:
    Ui::qt_listitemClass ui;

private slots:
    void timeOutEvent();
    void onRowClicked(const QModelIndex&);
    void on_pushButton_save_clicked();
    void on_pushButton_timeDesc_clicked();
    void on_pushButton_filterUncheck_clicked();
    void on_pushButton_init_clicked();
    void on_pushButton_checkSQL_clicked();
    void on_pushButton_filterPrice_clicked();
    void initWindows();
};
