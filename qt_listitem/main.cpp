#include "qt_listitem.h"
#include <QtWidgets/QApplication>
#include <QWebEngineView>
#include <QPainter>
#include <QVBoxLayout>
#include "qrencode/qrencode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qt_listitem w;
    w.show();
    return a.exec();
}
