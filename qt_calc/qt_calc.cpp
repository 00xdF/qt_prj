#include "qt_calc.h"
#include <qpushbutton.h>
#include <qdebug.h>
#include <qstack.h>
#include<qmessagebox.h>
#include<math.h>

bool isNum(char c);

qt_calc::qt_calc(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Calc");
    this->setFixedSize(QSize(409,408));
    ui.lineEdit->setFont(QFont("黑体", 20));
    ui.lineEdit->setReadOnly(true);
    //寻找当前窗口的所有QPushButton
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for each (QPushButton *button in buttons)
    {
        if (button->text().contains("=")) {
            connect(button, SIGNAL(clicked()), this, SLOT(equals_Pressed()));
            continue;
        }
        if (button->text().contains("<-")) {
            connect(button, SIGNAL(clicked()), this, SLOT(back_Pressed()));
            continue;
        }
        if (button->text().contains("C")) {
            connect(button, SIGNAL(clicked()), this, SLOT(clear_Pressed()));
            continue;
        }
        if (button->text().contains("1/x")) {
            connect(button, SIGNAL(clicked()), this, SLOT(qiuDao()));
            continue;
        }
        if (button->text().contains("sqrt")) {
            connect(button, SIGNAL(clicked()), this, SLOT(sqrt()));
            continue;
        }
        button->setFont(QFont("黑体", 12));
        //连接点击信号和响应槽
        connect(button, SIGNAL(clicked()), this, SLOT(digistPressed()));
    }
}

qt_calc::~qt_calc()
{

}


void qt_calc::digistPressed() {
    QPushButton* button = (QPushButton*)sender();
    if (button->text().length()==1&&button->text() <= '9' && button->text() >= '0' 
        || button->text()=='+' || button->text() =='-' 
        || button->text()=='*'|| button->text()=='/'
        || button->text()=='.') {
        expression += button->text();
        ui.lineEdit->setText(expression);
    } 
}

void qt_calc::qiuDao() {
    equals_Pressed(); //先计算结果
    float res = ui.lineEdit->text().toFloat();
    if (res == 0) {
        QMessageBox::warning(this, "警告", "当前结果为0");
        return;
    }
    res = 1 / res;
    ui.lineEdit->setText(QString::number(res));


}void qt_calc::sqrtNum() {
    equals_Pressed(); //先计算结果
    float res = ui.lineEdit->text().toFloat();
    ui.lineEdit->setText(QString::number(sqrt(res)));
}

void qt_calc::equals_Pressed() {
    //计算事件
    QStack<float> nums;
    char c,op = '+';
    for (int i = 0;i < expression.length();i++) {
        QString num = "";
        c = expression.at(i).toLatin1();
        if (isNum(c)) {
            while (i<expression.length() && isNum(expression.at(i).toLatin1())) {
                c = expression.at(i).toLatin1();
                num += c;
                i++;
            }
            i--; //当前的i是符号 需要回退，不然会略过符号
            switch (op)
            {
                case '+':
                    nums.push(num.toFloat());
                    break;
                case '-':
                    nums.push(-num.toFloat());
                    break;
                case '*':
                    nums.push(nums.pop()*num.toFloat());
                    break;
                case '/':
                    nums.push(nums.pop() / num.toFloat());
                    break;
                default:
                    break;
            }
        }
        else {
            op = c;
        }
    }

    float res = 0;
    for each (float var in nums)
    {
        res += var;
    }
    ui.lineEdit->setText(QString::number(res));
}

void qt_calc::back_Pressed() {
    expression.chop(1);
    ui.lineEdit->setText(expression);
}

void qt_calc::clear_Pressed() {
    expression = "";
    ui.lineEdit->setText(expression);
}

bool isNum(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}