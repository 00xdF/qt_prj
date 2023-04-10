#pragma once
#include<QPixmap>
#include "qrencode/qrencode.h"
#include <QPainter>
class Tools {
public:
    //生成二维码
    static QPixmap createQRCode(QString strUrl) {
        QRcode* qrcode;
        qrcode = QRcode_encodeString(strUrl.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
        //定义二维码的长宽
        qint32 temp_width = 100;
        qint32 temp_height = 100;

        qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
        double scale_x = (double)temp_width / (double)qrcode_width;
        double scale_y = (double)temp_height / (double)qrcode_width;

        int offset = 14;
        QImage mainimg = QImage(temp_width + offset * 2, temp_height + offset * 2, QImage::Format_ARGB32);
        //创建画板
        QPainter painter(&mainimg);
        QColor background(Qt::white);
        //填充背景
        painter.setBrush(background);
        //设置画笔
        painter.setPen(Qt::NoPen);
        painter.drawRect(offset, offset, temp_width, temp_height);
        //设置画笔颜色
        QColor foreground(Qt::blue);
        painter.setBrush(foreground);
        //开始绘制二维码 为1 绘制矩形框
        for (qint32 y = 0; y < qrcode_width; y++)
        {
            for (qint32 x = 0; x < qrcode_width; x++)
            {
                unsigned char b = qrcode->data[y * qrcode_width + x];
                if (b & 0x01)
                {
                    QRectF r(offset + x * scale_x, offset + y * scale_y, scale_x, scale_y);
                    painter.drawRects(&r, 1);
                }
            }
        }

        QPixmap mainmap = QPixmap::fromImage(mainimg);
        return mainmap;
    }
};
