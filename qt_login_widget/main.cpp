#include "qt_login_widget.h"
#include <QtWidgets/QApplication>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")// ��ָ���֧��VS����
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qt_login_widget w;
    w.show();
    return a.exec();
}
