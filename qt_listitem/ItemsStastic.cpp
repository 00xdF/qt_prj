#include "ItemsStastic.h"
#include <QPainter>
#include<QGraphicsLineItem>
#include<QGraphicsView>
#include "SQLiteUtils.cpp"
#include <QMainWindow>
#include "qt_listitem.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "MyGraphicsEllipseItem.h"
#include "MyEllipseItem.h"


namespace global{
    QList<QString> priceList;
    QGraphicsScene* scene;
}

void getData(QListWidget* widget);

ItemsStastic::ItemsStastic(QWidget* parent) : QWidget(parent) // ���캯��ʵ��
{
	ui.setupUi(this);
	initWindows();
    this->setWindowTitle(tr("��Ʒ�۸����ܷ���"));
    getData(ui.listWidget_items);
    connect(ui.listWidget_items,
        SIGNAL(itemClicked(QListWidgetItem*)),
        this, SLOT(itemListCliked(QListWidgetItem*)));
   
}



ItemsStastic::~ItemsStastic()
{}

//��ʼ������
void ItemsStastic::initWindows()
{
    // ����һ�� QGraphicsView
    QGraphicsView* view = new QGraphicsView(ui.qwidget);
    // ����һ�� QGraphicsScene
    global::scene = new QGraphicsScene();
    view->setScene(global::scene);
    // ���ó����Ĵ�С
    global::scene->setSceneRect(0, 0, ui.qwidget->width(), ui.qwidget->height());
}

//��ȡ����
void getData(QListWidget* widget) {
    QString sql = R"(
        select * from 
        (SELECT substr(content, 1, 
        instr(content || ' ', ' ') - 1) AS keyword,
        count(*) AS count FROM items
        GROUP BY keyword
        ORDER BY count DESC)
		where count!=1)";
    sql = sql.replace("\n", "");
    QSqlDatabase db =  SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    if (query.exec(sql)) {
        while (query.next()) {
           /* qDebug() << "keyword:" << query.value("keyword").toString()
                << "count:" << query.value("count").toString();*/
            widget->addItem(query.value("keyword").toString()+"-"+ query.value("count").toString());
        }
    }
    else {
        qDebug() << query.lastError().text();
    }
}

//�б�����¼�
void ItemsStastic::itemListCliked(QListWidgetItem* item) {
    //�������
    global::priceList.clear();
    global::scene->clear();
    QString name = item->text().split("-")[0]; //��ȡ����
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    //�۸�sql���
    QString priceSql = "select  id,price from items where content like '%" + name + "%'";
    qDebug() << priceSql;
    if (query.exec(priceSql)) {
        while (query.next()) {
            global::priceList.append(query.value("id").toString()+"," + query.value("price").toString());
        }
    }
    else {
        qDebug() << query.lastError().text();
    }
    qDebug() << global::priceList.length();
    //����
    drawXy();
}

//��������
void ItemsStastic::drawXy() {
 
    // ���� x ��� y ��
    QGraphicsLineItem* xAxis = new QGraphicsLineItem(0, 0, ui.qwidget->width(), 0);
    QGraphicsLineItem* yAxis = new QGraphicsLineItem(0, 0, 0, ui.qwidget->height());

    // �� x ��� y ����ӵ�������
    global::scene->addItem(xAxis);
    global::scene->addItem(yAxis);
    // ��� x ��̶�
    for (int i = 100; i <= ui.qwidget->width(); i += 50) {
        QGraphicsLineItem* tick = new QGraphicsLineItem(i, -5, i, 5);
        global::scene->addItem(tick);
    }


    double avg = 0.0; 
    int lowPrice = 0;
    int highPrice = 9999;
    for each (QString price in global::priceList) {
        avg += price.split(",")[1].toDouble();
    }
    avg = avg / global::priceList.length();
    lowPrice = (avg - 2000) > 0 ? avg-2000 : 0;
    highPrice = (avg + 2000);
    qDebug() << "low:"  << lowPrice << ",high=" << highPrice;

    //�����y��ĸ߶Ȼ�����λ
    double heightUnit = 1.0*double(ui.qwidget->height())/(highPrice-lowPrice);
    qDebug() << "y������߶ȵ�λ"<<heightUnit;

    //�����x��Ŀ�Ȼ�����λ
    double widthUnit = double(ui.qwidget->width())/ double(global::priceList.length())*1.0;

    //����ƽ���۸�ָ���
    QGraphicsLineItem* avgAxis = new QGraphicsLineItem(0, (avg - lowPrice) * heightUnit, ui.qwidget->width(), (avg- lowPrice )* heightUnit);
    avgAxis->setPen(QPen(Qt::blue, 4)); 

    global::scene->addItem(avgAxis);
    //�����۸�� ���Ƶ�
    double i = 0;
    for each (QString price in global::priceList)
    {
        // ��������ϵ�ϵĵ�
        MyEllipseItem* point = new MyEllipseItem(i, (price.split(",")[1].toDouble() - lowPrice) * heightUnit, 8, 8);
        point->setToolTip(price);
        //qDebug() << (price.toDouble() - lowPrice) * heightUnit;
        point->setBrush(QBrush(Qt::red));
        // ������ӵ�������
        global::scene->addItem(point);
        qDebug() << "(" << i << "," << (price.split(",")[1].toDouble() - lowPrice) * heightUnit << ")";
        i += widthUnit;
       
    }
 


    double unit = double(highPrice - lowPrice)/double(ui.qwidget->height()) * 20;
    // ��� y ��̶�
    for (int i = lowPrice,j=0; i <= highPrice;) {
        QGraphicsLineItem* tick = new QGraphicsLineItem(0, j, 5,j);
        global::scene->addItem(tick);
        QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(i));
        label->setPos(label->boundingRect().width()-10, j - label->boundingRect().height() / 2);
        global::scene->addItem(label);
        i += unit;
        j += 20;
    }


}

//����SQL
void ItemsStastic::analyseSQL() {
    //�������
    global::priceList.clear();
    global::scene->clear();
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    //�۸�sql���
    qDebug() << sql;
    if (query.exec(sql)) {
        while (query.next()) {
            global::priceList.append(query.value("id").toString() + "," + query.value("price").toString());
        }
    }
    else {
        qDebug() << query.lastError().text();
    }
    qDebug() << global::priceList.length();
    //����
    drawXy();
}

//���յ�sql�����źŴ��� 
void ItemsStastic::receviedSQL(const QString sql)
{
    qDebug() << "your SQL:" << sql;
    this->sql = sql;
    analyseSQL();
}
