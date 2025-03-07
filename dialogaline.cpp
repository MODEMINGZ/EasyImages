#include "dialogaline.h"
#include "ui_dialogaline.h"

DialogALine::DialogALine(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogALine)
{
    ui->setupUi(this);
    this->setWindowTitle("辅助网格");
    this->height=50;
    this->width=50;
    this->angle=0;
}

DialogALine::~DialogALine()
{
    delete ui;
}

void DialogALine::on_pushButton_2_clicked()
{
    this->reject();
}


void DialogALine::on_pushButton_clicked()
{
    this->accept();
}


void DialogALine::on_doubleSpinBox_valueChanged(double arg1)
{
    this->height=arg1;
}


void DialogALine::on_doubleSpinBox_2_valueChanged(double arg1)
{
    this->width=arg1;
}


void DialogALine::on_doubleSpinBox_3_valueChanged(double arg1)
{
    this->angle=arg1;
}

