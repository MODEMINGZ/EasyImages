#include "dialogbina.h"
#include "ui_dialogbina.h"

DialogBina::DialogBina(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogBina)
{
    ui->setupUi(this);
    setWindowTitle("二值化调节");
    ui->spinBox->setRange(0,255);
    ui->horizontalSlider->setRange(0,255);
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    ui->spinBox->setValue(100);
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogBina::~DialogBina()
{
    delete ui;
}

void DialogBina::on_pushButton_clicked()
{
    this->accept();
}


void DialogBina::on_horizontalSlider_valueChanged(int value)
{
    emit bChanged(value);
}

