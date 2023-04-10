#include <QSqlDatabase>
#include <qsqlquery.h>
#include <qdebug.h>
#include <qsqlerror.h>
#include<qdir.h>


//SQLite数据操作类
class SQLiteUtils {

public:


    ~SQLiteUtils() {
        db.close();
    }

    QSqlDatabase getConnection() {
        if (connectDB()) {
            return db;
        }
    }

    static SQLiteUtils& getInstance() {
        static SQLiteUtils instance; // 静态局部变量，保证全局只有一个实例
        return instance;
    }


private:
    //放私有的 防止被创建
    SQLiteUtils() { }

    SQLiteUtils(const SQLiteUtils&) = delete; // 禁用拷贝构造函数

    SQLiteUtils& operator= (const SQLiteUtils&) = delete; // 禁用赋值运算符

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // QString currentDir = QDir::currentPath();
    const QString  dbName = "C:/Users/Administrator/source/repos/qt_calc/qt_listitem/Items.db";


    //连接到数据库
    bool connectDB() {
        db.setDatabaseName(dbName); // 数据库名称
        if (db.open()) {
            qDebug() << "Connected to database!";
            return true;
        }
        else {
            qDebug() << "Error connecting to database:" << db.lastError().text();
            return false;
        }

    }



};




