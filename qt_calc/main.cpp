#include "qt_calc.h"
#include <QtWidgets/QApplication>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")// ��ָ���֧��VS����
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qt_calc w;
    w.show();
    return a.exec();
}