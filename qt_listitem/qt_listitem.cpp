#include "qt_listitem.h"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QString>
#include <qdebug.h>
#include <qmessagebox.h>
#include "SQLUtils.cpp"
#include "SQLiteUtils.cpp"
#include "User.cpp"
#include <qtimer.h>
#include <qurl.h>
#include <QWebEngineView>
#include "qrencode/qrencode.h"
#include "treeViewDelegate.cpp"
#include "ItemsStastic.h"
#include "Tools.h"

extern QString sql;
//定义全局变量
namespace Globals {
    QStandardItemModel* model = new QStandardItemModel();
    QSet<QString> set;
    QString sql = "select * from items where 1 = 1 ";
    QString prevPriceFilterSQL, prevAllowedFilterSQL,prevBannedFilterSQL;
}

const int ALLOWED = 0; 
const int BANNED = 1;

void updateTreeItemData(qt_listitem* parent, QTreeView* widget); //更新列表
QString CreateSQLStatement(QString pattern, int type); //创建SQL语句
QString createAllowSQL(QString pattern);
QString createBanSQL(QString pattern);
QPixmap createQRCode(QString strUrl);

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


qt_listitem::qt_listitem(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setFixedSize(1562, 796);
    this->setWindowTitle(tr("闲鱼商品统计"));
    initWindows();

    //开启定时器事件
    QTimer* time = new QTimer(this);
    time->start(1000);
    //初始化treeview 设置列名
    QStringList headers;
    headers << "ID " << "发布时间" << "商品介绍" << "价格" << "用户昵称" << "信用等级" << "链接";
    Globals::model->setHorizontalHeaderLabels(headers);
    /*
    //获取数据库连接
    QSqlDatabase* db = &SQLUtils::getInstance().getConnection();
    //创建查询
    if (db != nullptr) {
        QSqlQuery query;
        if (query.exec("select * from users")) {
            while (query.next()) {
                User user = User(query.value("id").toInt(), query.value("name").toString());
                qDebug() << user.toString();
                QTreeWidgetItem* item_sub = new QTreeWidgetItem(group);
                item_sub->setText(0, QString::number(user.getId()));
                item_sub->setCheckState(0, Qt::Unchecked);
            }
        }
        else {
            qDebug() << "exec failed!";
        }

    }
    else {
        qDebug() << "not open!";
    }
  */
    //连接timer计时器
    connect(time, SIGNAL(timeout()), this, SLOT(timeOutEvent()));
    //连接列表项目点击信号
    connect(ui.treeView, SIGNAL(clicked(const QModelIndex&)),
        this, SLOT(onRowClicked(const QModelIndex&)));


}

qt_listitem::~qt_listitem()
{}

void qt_listitem::onRowClicked(const QModelIndex& index)
{
    //设置内容详情到右侧文本框
    QString id = Globals::model->item(index.row(), 0)->text();
    QString pubtime = Globals::model->item(index.row(), 1)->text();
    QString name = Globals::model->item(index.row(),2)->text();
    QString price = Globals::model->item(index.row(),3)->text();
    QString usernick = Globals::model->item(index.row(),4)->text();
    QString level = Globals::model->item(index.row(),5)->text();
    QString url = Globals::model->item(index.row(),6)->text();
    QString content = QString("%1,%2,%5\n发布时间:%3\n介绍:%4")
        .arg(usernick)
        .arg(level)
        .arg(pubtime)
        .arg(name)
        .arg(price);
    ui.textEdit_itemInfo->setText(content);
    ui.label_qrcode->setPixmap(Tools::createQRCode(url));
    //设置点击字体
    QModelIndex modelIndex = index.sibling(index.row(),0);
    Globals::model->setData(modelIndex, QColor(Qt::red), Qt::TextColorRole);
}

//保存按钮点击事件
void qt_listitem::on_pushButton_save_clicked()
{   
    int flag = 0;
    if (Globals::sql.contains("order by pubtime desc")) {
        Globals::sql.replace("order by pubtime desc", "");
        flag = 1;
    }
    //清空内容
    Globals::set.clear();
    Globals::model->removeRows(0, Globals::model->rowCount());
    QString allowed_text = ui.lineEdit_need->text();
    QString banned_text = ui.lineEdit_waste->text();
    //保存到数据库
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    if (query.prepare("update limitKey set allowedStr = ?,bannedStr=?  where id = 0")) {
        query.bindValue(0, allowed_text);
        query.bindValue(1, banned_text);
        query.exec();
    }
    //获得允许的SQL语句
    QString allowedSQL = CreateSQLStatement(allowed_text,ALLOWED);
    Globals::sql = Globals::sql.replace(Globals::prevAllowedFilterSQL, "");
    Globals::prevAllowedFilterSQL = allowedSQL;
    Globals::sql += " " + allowedSQL + " ";
    //获得Banned的SQL语句
    QString bannedSQL = CreateSQLStatement(banned_text, BANNED);
    Globals::sql = Globals::sql.replace(Globals::prevBannedFilterSQL, "");
    Globals::prevBannedFilterSQL = bannedSQL;
    Globals::sql +=" "+ bannedSQL+ " ";
    if (flag == 1) {
        Globals::sql += " order by pubtime desc ";
    }
}
//时间降序按钮点击事件
void qt_listitem::on_pushButton_timeDesc_clicked()
{
    Globals::set.clear();
    Globals::model->removeRows(0,Globals::model->rowCount());
    Globals::sql += " order by pubtime desc ";
    // ui.pushButton_timeDesc->setEnabled(false);
}

//过滤未认证按钮点击事件
void qt_listitem::on_pushButton_filterUncheck_clicked()
{
    Globals::set.clear();
    Globals::model->removeRows(0, Globals::model->rowCount());
    if (Globals::sql.contains(" order by pubtime desc")) {
        Globals::sql.replace("order by pubtime desc", " ");
        Globals::sql += " and level!='未认证' order by pubtime desc ";
    }
    else {
        Globals::sql += " and level!='未认证' ";
    }
   
}

//初始化按钮点击事件
void qt_listitem::on_pushButton_init_clicked()
{
    Globals::set.clear();
    Globals::model->removeRows(0, Globals::model->rowCount());
    Globals::sql = "select * from items where 1 = 1 ";
    ui.pushButton_timeDesc->setEnabled(true);
}

//查看SQL按钮点击事件
void qt_listitem::on_pushButton_checkSQL_clicked()
{
    QMessageBox::information(this, "当前SQL语句：", Globals::sql);
}

//初过滤价格按钮点击事件
void qt_listitem::on_pushButton_filterPrice_clicked()
{
    int flag = 0;
    if (Globals::sql.contains("order by pubtime desc")) {
        Globals::sql.replace("order by pubtime desc", "");
        flag = 1;
    }
    Globals::set.clear();
    Globals::model->removeRows(0, Globals::model->rowCount());
    QString high_price = ui.lineEdit_price_high->text();
    QString low_price = ui.lineEdit_price_low->text();
    QString _sql = QString(" and CAST(price AS UNSIGNED) >= %1 and CAST(price AS UNSIGNED) <=%2 ")
        .arg(low_price).arg(high_price);
    Globals::sql.replace(Globals::prevPriceFilterSQL,"");
    Globals::prevPriceFilterSQL = _sql;
    Globals::sql += _sql;
    if (flag == 1) {
        Globals::sql += " order by pubtime desc ";
    }
}

void updateTreeItemData(qt_listitem* parent,QTreeView* widget) {
    widget->setModel(Globals::model);
    //创建数据库连接
    QSqlDatabase* d = &SQLiteUtils::getInstance().getConnection();
    //执行查询操作
    if (d != nullptr) {
        QSqlQuery q;
        QString sql = Globals::sql + " limit 100";
        qDebug() << sql;
        if (q.exec(sql)) {
            while (q.next()) {
                QString id =  q.value("id").toString();
                if (Globals::set.contains(id)) {

                }
                else {
                    //说明不存在数据 插入数据并显示
                    Globals::set.insert(id);
                    QDateTime dateTime = QDateTime::fromTime_t(q.value("pubTime").toLongLong()/1000);
                    // 将QDateTime对象格式化为指定格式的字符串
                    QString format = "yyyy-MM-dd hh:mm:ss";
                    QString dateTimeString = dateTime.toString(format);
                    QList<QStandardItem*> row1;
                    QStandardItem* item1 = new QStandardItem(q.value("id").toString());
                    item1->setData(QVariant(QColor(Qt::white)), Qt::UserRole);
                    row1 << item1
                        << new QStandardItem(dateTimeString)
                        << new QStandardItem(q.value("content").toString())
                        << new QStandardItem(q.value("price").toString())
                        << new QStandardItem(q.value("usernick").toString())
                        << new QStandardItem(q.value("level").toString())
                        << new QStandardItem(q.value("url").toString());
                    Globals::model->appendRow(row1);
                }
               
            }
        }
        else {
            QMessageBox::warning(parent, "警告！请最后使用时间降序功能！", q.lastError().text());
            Globals::sql = "select * from items where 1 = 1 ";
        }
    }
   
}

void qt_listitem::timeOutEvent() {
    updateTreeItemData(this,ui.treeView);
}

void qt_listitem::on_pushButton_stasic_clicked() {
    qDebug() << "this is new windows!";
    QWidget* widget = new ItemsStastic();
    widget->show();
    connect(this,SIGNAL(sqlChanged(const QString)), widget, SLOT(receviedSQL(const QString)));
}

//分析数据
void qt_listitem::on_pushButton_analyse_clicked()
{
    //发送sql语句修改信号到另外一个窗口
    emit sqlChanged(Globals::sql);
}

QString createAllowSQL(QString pattern)
{
    QString sql = "";
    QStringList allowedList = pattern.split('&'); 
    for each(QString i in allowedList)
    {
        if (i.contains("(") && i.contains(")"))
        {
            QString sqli = " and (";
            //表示或者关系
            QStringList orList = i.replace("(", "").replace(")", "").split("|");
            for each(QString orPattern in orList)
            {
                sqli += " content like '%"+ orPattern +"%' or ";
            }
            sqli += "1=2 )";
            sql += sqli;
        }
        else
        {
            sql += " and content like '%"+i+"%' ";
        }
    }
    return sql;
}

QString createBanSQL(QString pattern)
{
    QString sql = "";
    for each(QString banStr in pattern.split("&"))
    {
        sql += " and content not like '%"+ banStr +"%'";
    }
    return sql;
}

QString CreateSQLStatement(QString pattern, int type)
 {
     if (type == ALLOWED)
     {
         return createAllowSQL(pattern);
     }

     if (type == BANNED)
     {
         return createBanSQL(pattern);
     }
     return "";
 }



void qt_listitem::initWindows() {
    //读取关键词
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    if (query.exec("select * from limitKey where id = 0")) {
        while (query.next()) {
            ui.lineEdit_need->setText(query.value("allowedStr").toString());
            ui.lineEdit_waste->setText(query.value("bannedStr").toString());
        }
    }
}

