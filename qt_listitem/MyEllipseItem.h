#pragma once
#include <QGraphicsEllipseItem>
#include <qmessagebox.h>
#include "Tools.h"
//�Զ������̳�QGraphicsEllipseItem ��
//��д���е�mousePressEvent���� �ﵽ�Զ������¼�
class MyEllipseItem : public QGraphicsEllipseItem
{
public:
    MyEllipseItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr)
        : QGraphicsEllipseItem(x, y, width, height, parent) {}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        QGraphicsEllipseItem::mousePressEvent(event);
        //ͨ�����ַ�ʽ��ȡtooltip
        QString id = QGraphicsEllipseItem::toolTip().split(",")[0];
        //��ѯ���id����
        QSqlDatabase db =  SQLiteUtils::getInstance().getConnection();
        QSqlQuery query;
        query.prepare("select * from items where id = ?");
        query.bindValue(0, id);
        if (query.exec()) {
            while (query.next()) {
                QString str = QString("%1,%2,%3��\n\n���ܣ�%4")
                    .arg(query.value("id").toString())
                    .arg(query.value("level").toString())
                    .arg(query.value("price").toString())
                    .arg(query.value("content").toString());
                qDebug() << str;
                QMessageBox msgBox;
                msgBox.setWindowTitle("��Ʒ��Ϣ");
                msgBox.setText(str);
                msgBox.setIconPixmap(Tools::createQRCode(query.value("url").toString()));
                msgBox.exec();
            }
        }
        
    }
};
