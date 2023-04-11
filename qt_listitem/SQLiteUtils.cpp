#include <QSqlDatabase>
#include <qsqlquery.h>
#include <qdebug.h>
#include <qsqlerror.h>
#include<qdir.h>


//SQLite���ݲ�����
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
        static SQLiteUtils instance; // ��̬�ֲ���������֤ȫ��ֻ��һ��ʵ��
        return instance;
    }


private:
    //��˽�е� ��ֹ������
    SQLiteUtils() { }

    SQLiteUtils(const SQLiteUtils&) = delete; // ���ÿ������캯��

    SQLiteUtils& operator= (const SQLiteUtils&) = delete; // ���ø�ֵ�����

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // QString currentDir = QDir::currentPath();
    const QString  dbName = "C:/Users/Administrator/source/repos/qt_calc/qt_listitem/Items.db";


    //���ӵ����ݿ�
    bool connectDB() {
        db.setDatabaseName(dbName); // ���ݿ�����
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




