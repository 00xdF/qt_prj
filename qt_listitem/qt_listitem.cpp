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
//����ȫ�ֱ���
namespace Globals {
    QStandardItemModel* model = new QStandardItemModel();
    QSet<QString> set;
    QString sql = "select * from items where 1 = 1 ";
    QString prevPriceFilterSQL, prevAllowedFilterSQL,prevBannedFilterSQL;
}

const int ALLOWED = 0; 
const int BANNED = 1;

void updateTreeItemData(qt_listitem* parent, QTreeView* widget); //�����б�
QString CreateSQLStatement(QString pattern, int type); //����SQL���
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
    this->setWindowTitle(tr("������Ʒͳ��"));
    initWindows();

    //������ʱ���¼�
    QTimer* time = new QTimer(this);
    time->start(1000);
    //��ʼ��treeview ��������
    QStringList headers;
    headers << "ID " << "����ʱ��" << "��Ʒ����" << "�۸�" << "�û��ǳ�" << "���õȼ�" << "����";
    Globals::model->setHorizontalHeaderLabels(headers);
    /*
    //��ȡ���ݿ�����
    QSqlDatabase* db = &SQLUtils::getInstance().getConnection();
    //������ѯ
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
    //����timer��ʱ��
    connect(time, SIGNAL(timeout()), this, SLOT(timeOutEvent()));
    //�����б���Ŀ����ź�
    connect(ui.treeView, SIGNAL(clicked(const QModelIndex&)),
        this, SLOT(onRowClicked(const QModelIndex&)));


}

qt_listitem::~qt_listitem()
{}

void qt_listitem::onRowClicked(const QModelIndex& index)
{
    //�����������鵽�Ҳ��ı���
    QString id = Globals::model->item(index.row(), 0)->text();
    QString pubtime = Globals::model->item(index.row(), 1)->text();
    QString name = Globals::model->item(index.row(),2)->text();
    QString price = Globals::model->item(index.row(),3)->text();
    QString usernick = Globals::model->item(index.row(),4)->text();
    QString level = Globals::model->item(index.row(),5)->text();
    QString url = Globals::model->item(index.row(),6)->text();
    QString content = QString("%1,%2,%5\n����ʱ��:%3\n����:%4")
        .arg(usernick)
        .arg(level)
        .arg(pubtime)
        .arg(name)
        .arg(price);
    ui.textEdit_itemInfo->setText(content);
    ui.label_qrcode->setPixmap(Tools::createQRCode(url));
    //���õ������
    QModelIndex modelIndex = index.sibling(index.row(),0);
    Globals::model->setData(modelIndex, QColor(Qt::red), Qt::TextColorRole);
}

//���水ť����¼�
void qt_listitem::on_pushButton_save_clicked()
{   
    int flag = 0;
    if (Globals::sql.contains("order by pubtime desc")) {
        Globals::sql.replace("order by pubtime desc", "");
        flag = 1;
    }
    //�������
    Globals::set.clear();
    Globals::model->removeRows(0, Globals::model->rowCount());
    QString allowed_text = ui.lineEdit_need->text();
    QString banned_text = ui.lineEdit_waste->text();
    //���浽���ݿ�
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    if (query.prepare("update limitKey set allowedStr = ?,bannedStr=?  where id = 0")) {
        query.bindValue(0, allowed_text);
        query.bindValue(1, banned_text);
        query.exec();
    }
    //��������SQL���
    QString allowedSQL = CreateSQLStatement(allowed_text,ALLOWED);
    Globals::sql = Globals::sql.replace(Globals::prevAllowedFilterSQL, "");
    Globals::prevAllowedFilterSQL = allowedSQL;
    Globals::sql += " " + allowedSQL + " ";
    //���Banned��SQL���
    QString bannedSQL = CreateSQLStatement(banned_text, BANNED);
    Globals::sql = Globals::sql.replace(Globals::prevBannedFilterSQL, "");
    Globals::prevBannedFilterSQL = bannedSQL;
    Globals::sql +=" "+ bannedSQL+ " ";
    if (flag == 1) {
        Globals::sql += " order by pubtime desc ";
    }
}
//ʱ�併��ť����¼�
void qt_listitem::on_pushButton_timeDesc_clicked()
{
    Globals::set.clear();
    Globals::model->removeRows(0,Globals::model->rowCount());
    Globals::sql += " order by pubtime desc ";
    // ui.pushButton_timeDesc->setEnabled(false);
}

//����δ��֤��ť����¼�
void qt_listitem::on_pushButton_filterUncheck_clicked()
{
    Globals::set.clear();
    Globals::model->removeRows(0, Globals::model->rowCount());
    if (Globals::sql.contains(" order by pubtime desc")) {
        Globals::sql.replace("order by pubtime desc", " ");
        Globals::sql += " and level!='δ��֤' order by pubtime desc ";
    }
    else {
        Globals::sql += " and level!='δ��֤' ";
    }
   
}

//��ʼ����ť����¼�
void qt_listitem::on_pushButton_init_clicked()
{
    Globals::set.clear();
    Globals::model->removeRows(0, Globals::model->rowCount());
    Globals::sql = "select * from items where 1 = 1 ";
    ui.pushButton_timeDesc->setEnabled(true);
}

//�鿴SQL��ť����¼�
void qt_listitem::on_pushButton_checkSQL_clicked()
{
    QMessageBox::information(this, "��ǰSQL��䣺", Globals::sql);
}

//�����˼۸�ť����¼�
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
    //�������ݿ�����
    QSqlDatabase* d = &SQLiteUtils::getInstance().getConnection();
    //ִ�в�ѯ����
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
                    //˵������������ �������ݲ���ʾ
                    Globals::set.insert(id);
                    QDateTime dateTime = QDateTime::fromTime_t(q.value("pubTime").toLongLong()/1000);
                    // ��QDateTime�����ʽ��Ϊָ����ʽ���ַ���
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
            QMessageBox::warning(parent, "���棡�����ʹ��ʱ�併���ܣ�", q.lastError().text());
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

//��������
void qt_listitem::on_pushButton_analyse_clicked()
{
    //����sql����޸��źŵ�����һ������
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
            //��ʾ���߹�ϵ
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
    //��ȡ�ؼ���
    QSqlDatabase db = SQLiteUtils::getInstance().getConnection();
    QSqlQuery query;
    if (query.exec("select * from limitKey where id = 0")) {
        while (query.next()) {
            ui.lineEdit_need->setText(query.value("allowedStr").toString());
            ui.lineEdit_waste->setText(query.value("bannedStr").toString());
        }
    }
}

