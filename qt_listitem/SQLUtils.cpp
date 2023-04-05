#include <QSqlDatabase>
#include <qsqlquery.h>
#include <qdebug.h>
#include <qsqlerror.h>



//MySQL数据操作类
class SQLUtils{

public:
    


    ~SQLUtils() {
        db.close();
    }

    QSqlDatabase getConnection() {
        if (connectDB()) {
            return db;
        }
    }

    static SQLUtils& getInstance() {
        static SQLUtils instance; // 静态局部变量，保证全局只有一个实例
        return instance;
    }


private:
    //放私有的 防止被创建
    SQLUtils() { }

    SQLUtils(const SQLUtils&) = delete; // 禁用拷贝构造函数

    SQLUtils& operator= (const SQLUtils&) = delete; // 禁用赋值运算符

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    const QString url = "localhost" 
        , dbName="MyDB"
        , username="root"
        , password="123456";


    //连接到数据库
    bool connectDB() {
        db.setHostName(url); // 数据库服务器地址
        db.setDatabaseName(dbName); // 数据库名称
        db.setUserName(username); // 数据库用户名
        db.setPassword(password); // 数据库密码
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




