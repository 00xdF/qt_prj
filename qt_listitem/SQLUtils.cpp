#include <QSqlDatabase>
#include <qsqlquery.h>
#include <qdebug.h>
#include <qsqlerror.h>



//MySQL���ݲ�����
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
        static SQLUtils instance; // ��̬�ֲ���������֤ȫ��ֻ��һ��ʵ��
        return instance;
    }


private:
    //��˽�е� ��ֹ������
    SQLUtils() { }

    SQLUtils(const SQLUtils&) = delete; // ���ÿ������캯��

    SQLUtils& operator= (const SQLUtils&) = delete; // ���ø�ֵ�����

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    const QString url = "localhost" 
        , dbName="MyDB"
        , username="root"
        , password="123456";


    //���ӵ����ݿ�
    bool connectDB() {
        db.setHostName(url); // ���ݿ��������ַ
        db.setDatabaseName(dbName); // ���ݿ�����
        db.setUserName(username); // ���ݿ��û���
        db.setPassword(password); // ���ݿ�����
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




