#pragma once
#include <QGraphicsEllipseItem>
#include <qmessagebox.h>
#include "Tools.h"
//自定义的类继承QGraphicsEllipseItem ，
//重写其中的mousePressEvent方法 达到自定义点击事件
class MyEllipseItem : public QGraphicsEllipseItem
{
public:
    MyEllipseItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr)
        : QGraphicsEllipseItem(x, y, width, height, parent) {}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        QGraphicsEllipseItem::mousePressEvent(event);
        //通过这种方式获取tooltip
        QString id = QGraphicsEllipseItem::toolTip().split(",")[0];
        //查询相关id数据
        QSqlDatabase db =  SQLiteUtils::getInstance().getConnection();
        QSqlQuery query;
        query.prepare("select * from items where id = ?");
        query.bindValue(0, id);
        if (query.exec()) {
            while (query.next()) {
                QString str = QString("%1,%2,%3￥\n\n介绍：%4")
                    .arg(query.value("id").toString())
                    .arg(query.value("level").toString())
                    .arg(query.value("price").toString())
                    .arg(query.value("content").toString());
                qDebug() << str;
                QMessageBox msgBox;
                msgBox.setWindowTitle("商品信息");
                msgBox.setText(str);
                msgBox.setIconPixmap(Tools::createQRCode(query.value("url").toString()));
                msgBox.exec();
            }
        }
        
    }
};
