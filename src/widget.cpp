#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMaximumSize(240, 340);
    this->setMinimumSize(240, 340);

    this->setWindowTitle("计算器");
    QFont f("仿宋", 14);
    ui->mainEdit->setFont(f);

    QIcon con("D:\\QTproject\\QTonline\\calculator\\del.png");
    ui->delButton->setIcon(con);

    ui->equalButton->setStyleSheet("background:blue");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_oneButton_clicked()
{
    expression += "1";
    ui->mainEdit->setText(expression);
}


void Widget::on_twoButton_clicked()
{
    expression += "2";
    ui->mainEdit->setText(expression);
}


void Widget::on_threeButton_clicked()
{
    expression += "3";
    ui->mainEdit->setText(expression);
}


void Widget::on_fourButton_clicked()
{
    expression += "4";
    ui->mainEdit->setText(expression);
}


void Widget::on_fiveButton_clicked()
{
    expression += "5";
    ui->mainEdit->setText(expression);
}


void Widget::on_sixButton_clicked()
{
    expression += "6";
    ui->mainEdit->setText(expression);
}


void Widget::on_sevenButton_clicked()
{
    expression += "7";
    ui->mainEdit->setText(expression);
}


void Widget::on_eightButton_clicked()
{
    expression += "8";
    ui->mainEdit->setText(expression);
}


void Widget::on_nineButton_clicked()
{
    expression += "9";
    ui->mainEdit->setText(expression);
}


void Widget::on_leftButton_clicked()
{
    expression += "(";
    ui->mainEdit->setText(expression);
}


void Widget::on_rightButton_clicked()
{
    expression += ")";
    ui->mainEdit->setText(expression);
}


void Widget::on_addButton_clicked()
{
    expression += "+";
    ui->mainEdit->setText(expression);
}


void Widget::on_subButton_clicked()
{
    expression += "-";
    ui->mainEdit->setText(expression);
}


void Widget::on_mulButton_clicked()
{
    expression += "*";
    ui->mainEdit->setText(expression);
}


void Widget::on_divButton_clicked()
{
    expression += "/";
    ui->mainEdit->setText(expression);
}


void Widget::on_clearButton_clicked()
{
    expression.clear();
    ui->mainEdit->clear();
}

void Widget::on_zeroButton_clicked()
{
    expression += "0";
    ui->mainEdit->setText(expression);
}



void Widget::on_delButton_clicked()
{
    expression.chop(1);
    ui->mainEdit->setText(expression);
}


void Widget::on_equalButton_clicked()
{
    QStack<int> num_s;
    QStack<char> op_s;

    char opt[128] = {0};
    int num = 0;

    //转化expression到char*
    QByteArray ba;
    ba.append(expression.toUtf8());
    strcpy(opt,ba.data());

    State state = q_start;
    for(int i = 0; opt[i] != '\0'; i++){
        if(state == q_error) break;
        char ch = opt[i];
        switch(state){
        case q_start:
            if(isDigit(ch)){
                num = ch - '0';
                state = q_number;
            }
            else if(ch == '('){
                op_s.push(ch);
                state = q_lparent;
            }
            else
                state = q_error;
            break;

        case q_number:
            if(isDigit(ch)){
                num = num * 10 + ch - '0';
                state = q_number;
            }
            else if(isOp(ch)){
                num_s.push(num);
                num = 0;
                if( op_s.empty() || priority(ch) > priority(op_s.top()) ){
                    op_s.push(ch);
                    state = q_operator;
                }
                //需要进行出栈处理
                else{
                    while( !op_s.empty() && priority(ch) <= priority(op_s.top()) ){
                        num = calTop(num_s, op_s);
                        if(num == -1){
                            state = q_error;
                            break;
                        }
                        num_s.push(num);
                        num = 0;
                    }
                    op_s.push(ch);
                }
                state = q_operator;
            }
            else if(ch == ')'){
                num_s.push(num);
                num = 0;
                while( !op_s.empty() && op_s.top() != '(' ){
                    num = calTop(num_s, op_s);
                    if(num == -1){
                        state = q_error;
                        break;
                    }
                    num_s.push(num);
                    num = 0;
                }
                if(op_s.top() == '(') op_s.pop();
                state = q_rparent;
            }
            else
                state = q_error;
            break;

        case q_operator:
            if( isDigit(ch) ){
                num = ch - '0';
                state = q_number;
            }
            else if(ch == '('){
                op_s.push(ch);
                state = q_lparent;
            }
            else
                state = q_error;
            break;

        case q_lparent:
            if(isDigit(ch)){
                num = ch - '0';
                state = q_number;
            }
            else if( ch == '(' ){
                op_s.push(ch);
                state = q_lparent;
            }
            else
                state = q_error;
            break;

        case q_rparent:
            if( isOp(ch) ){
                if( op_s.empty() || priority(ch) > priority(op_s.top()) ){
                    op_s.push(ch);
                    state = q_operator;
                }
                else{
                    while( !op_s.empty() && priority(ch) <= priority(op_s.top()) ){
                        num = calTop(num_s, op_s);
                        if(num == -1){
                            state = q_error;
                            break;
                        }
                        num_s.push(num);
                        num = 0;
                    }
                    op_s.push(ch);
                }
                state = q_operator;
            }
            else if( ch == ')' ){
                while( !op_s.empty() && op_s.top() != '(' ){
                    num = calTop(num_s, op_s);
                    if(num == -1){
                        state = q_error;
                        break;
                    }
                    num_s.push(num);
                    num = 0;
                }
                if(op_s.top() == '(') op_s.pop();
                state = q_rparent;
            }
            else
                state = q_error;
            break;
        }
    }
    num_s.push(num);
    while(!op_s.empty() && !num_s.empty()){
        num = calTop(num_s, op_s);
        if(num == -1){
            state = q_error;
            break;
        }
        num_s.push(num);
        num = 0;
    }

    if(state == q_error){
        ui->mainEdit->setText("语法错误!");
    }
    else{
        expression.clear();
        expression += std::to_string(num_s.pop());
        ui->mainEdit->setText(expression);
    }
    expression.clear();
}

bool Widget::isDigit(const char ch){
    if(ch >= '0' && ch <= '9')
        return true;
    return false;
}

bool Widget::isOp(const char ch){
    if(ch == '+' || ch == '-' || ch == '/' || ch == '*')
        return true;
    return false;
}

int Widget::priority(const char ch){
    switch(ch){
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 3;
    case '(':
        return 1;
    }
}

int Widget::calTop(QStack<int>& number, QStack<char>& opera){
    int a = 0, b = 0;
    if(!number.empty() && !opera.empty()){
        b = number.pop();
        a = number.pop();
        char op = opera.pop();
        switch(op){
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b == 0) return -1;
            return a / b;
        default:
            return -1;
        }
    }
}
    