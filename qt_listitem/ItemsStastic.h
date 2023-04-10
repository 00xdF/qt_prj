#pragma once

#include <QWidget>
#include "ui_ItemsStastic.h"

class ItemsStastic : public QWidget
{
	Q_OBJECT

public:
	QString sql = "";
	ItemsStastic(QWidget *parent = nullptr);
	~ItemsStastic();
	void initWindows();
	
protected:
	void drawXy();
	void analyseSQL();
	

private:
	Ui::ItemsStasticClass ui;

private slots:
	void itemListCliked(QListWidgetItem* item);
	void receviedSQL(const QString sql);
	
};
