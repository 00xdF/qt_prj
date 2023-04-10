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

ItemsStastic::ItemsStastic(QWidget* parent) : QWidget(parent) // 构造函数实现
{
	ui.setupUi(this);
	initWindows();
    this->setWindowTitle(tr("商品价格智能分析"));
    getData(ui.listWidget_items);
    connect(ui.listWidget_items,
        SIGNAL(itemClicked(QListWidgetItem*)),
        this, SLOT(itemListCliked(QListWidgetItem*)));
   
}



ItemsStastic::~ItemsStastic()
{}

//初始化界面
void ItemsStastic::initWindows()
{
    // 创建一个 QGraphicsView
    QGraphicsView* view = new QGraphicsView(ui.qwidget);
    // 创建一个 QGraphicsScene
    global::scene = new QGraphicsScene();
    view->setScene(global::scene);
    // 设置场景的大小
    global::scene->setSceneRect(0, 0, ui.qwidget->width(), ui.qwidget->height());
}

//获取数据
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

//列表项单击事件
void ItemsStastic::itemListCliked(QListWidgetItem* item) {
    //清空数据
    global::priceList.clear();
    global::scene->clear();
    QString name = item->text().split("-")[0]; //获取名字
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    //价格sql语句
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
    //绘制
    drawXy();
}

//绘制坐标
void ItemsStastic::drawXy() {
 
    // 创建 x 轴和 y 轴
    QGraphicsLineItem* xAxis = new QGraphicsLineItem(0, 0, ui.qwidget->width(), 0);
    QGraphicsLineItem* yAxis = new QGraphicsLineItem(0, 0, 0, ui.qwidget->height());

    // 将 x 轴和 y 轴添加到场景中
    global::scene->addItem(xAxis);
    global::scene->addItem(yAxis);
    // 添加 x 轴刻度
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

    //计算出y轴的高度基本单位
    double heightUnit = 1.0*double(ui.qwidget->height())/(highPrice-lowPrice);
    qDebug() << "y轴基本高度单位"<<heightUnit;

    //计算出x轴的宽度基本单位
    double widthUnit = double(ui.qwidget->width())/ double(global::priceList.length())*1.0;

    //绘制平均价格分割线
    QGraphicsLineItem* avgAxis = new QGraphicsLineItem(0, (avg - lowPrice) * heightUnit, ui.qwidget->width(), (avg- lowPrice )* heightUnit);
    avgAxis->setPen(QPen(Qt::blue, 4)); 

    global::scene->addItem(avgAxis);
    //遍历价格表 绘制点
    double i = 0;
    for each (QString price in global::priceList)
    {
        // 创建坐标系上的点
        MyEllipseItem* point = new MyEllipseItem(i, (price.split(",")[1].toDouble() - lowPrice) * heightUnit, 8, 8);
        point->setToolTip(price);
        //qDebug() << (price.toDouble() - lowPrice) * heightUnit;
        point->setBrush(QBrush(Qt::red));
        // 将点添加到场景中
        global::scene->addItem(point);
        qDebug() << "(" << i << "," << (price.split(",")[1].toDouble() - lowPrice) * heightUnit << ")";
        i += widthUnit;
       
    }
 


    double unit = double(highPrice - lowPrice)/double(ui.qwidget->height()) * 20;
    // 添加 y 轴刻度
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

//分析SQL
void ItemsStastic::analyseSQL() {
    //清空数据
    global::priceList.clear();
    global::scene->clear();
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    //价格sql语句
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
    //绘制
    drawXy();
}

//接收到sql语句槽信号处理 
void ItemsStastic::receviedSQL(const QString sql)
{
    qDebug() << "your SQL:" << sql;
    this->sql = sql;
    analyseSQL();
}
