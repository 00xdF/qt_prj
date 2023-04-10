#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
class MyGraphicsEllipseItem : public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    MyGraphicsEllipseItem(QGraphicsItem* parent = nullptr) : QGraphicsEllipseItem(parent) {
        
    }
    MyGraphicsEllipseItem(int x,int y,int w,int h) {
        QGraphicsEllipseItem(x,y,w,h);
    }

signals:
    void itemClicked();

public slots:
    void onItemClicked()
    {
        // 处理点击事件
        qDebug() << "Item clicked!";
    }


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        emit itemClicked();
        QGraphicsEllipseItem::mousePressEvent(event);
    }
};



