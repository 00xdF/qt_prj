#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <qsqlquery.h>
#include <qdebug.h>
#include <qsqlerror.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("MyDB");
    if (db.open()) {
        qDebug() << "Connected to database!";
        QSqlQuery query;
        query.prepare("select * from users where id = ?");
        query.bindValue(0, 10001);
        query.exec();
        while (query.next()) {
            qDebug() << query.value("id").toInt() << query.value("name").toString();
        }
    }
    else {
        qDebug() << "can't connect this database!";
    }
   
    return a.exec();
}
