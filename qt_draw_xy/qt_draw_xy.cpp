#include "qt_draw_xy.h"
#include <QPainter>
#include <qmath.h>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

qt_draw_xy::qt_draw_xy(QWidget *parent)
    : QWidget(parent)
{
   
    ui.setupUi(this);
    this->setWindowTitle("曲线绘图");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFixedSize(600, 500);
}

qt_draw_xy::~qt_draw_xy()
{}

void qt_draw_xy::paintEvent(QPaintEvent * event)
{

    QPainter painter(this);

    // 绘制 Y  轴 (20,height-20) -> (20,0)
    painter.drawLine(20, height()-20, 20, 20);

    int step = (height() - 20) / 10;
    for (int i = 1; i < 10; i++) {
        painter.drawLine(20, 20 + i * step, 25, 20 + i * step);
        painter.drawText(0, 20 + i * step, QString::number(100 - i*10));
    }

    // 绘制 X 轴
    step = (width() - 20) / 10;
    painter.drawLine(20, height()-20, width()-20, height()-20);
    for (int i = 1; i < 10; i++) {
        painter.drawLine(20+i * step, height() - 20, 20 + i * step, height() - 25);
        painter.drawText(20 + i * step, height(), QString::number(i));
    }

    //绘制曲线
    int numPoints = 500; //绘制的点数量
    step = (width() - 40.0f) / numPoints; //x轴的间距
    QVector<QPointF> points;
    for (int i = 0; i < numPoints; i++) {
        float x = 20 + i * step;
        float y = height() - 100 * qSin(x * M_PI /50) -120;
        points.append(QPointF(x, y));
    }
    painter.drawPolyline(points);
}
