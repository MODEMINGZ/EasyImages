#include "dialoghsv.h"
#include "ui_dialoghsv.h"

DialogHSV::DialogHSV(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogHSV)
{
    ui->setupUi(this);
    connect(ui->horizontalSlider_H,SIGNAL(valueChanged(int)),ui->spinBox_H,SLOT(setValue(int)));
    connect(ui->horizontalSlider_S_2,SIGNAL(valueChanged(int)),ui->spinBox_S,SLOT(setValue(int)));
    connect(ui->horizontalSlider_V,SIGNAL(valueChanged(int)),ui->spinBox_V,SLOT(setValue(int)));
    connect(ui->spinBox_H,SIGNAL(valueChanged(int)),ui->horizontalSlider_H,SLOT(setValue(int)));
    connect(ui->spinBox_S,SIGNAL(valueChanged(int)),ui->horizontalSlider_S_2,SLOT(setValue(int)));
    connect(ui->spinBox_V,SIGNAL(valueChanged(int)),ui->horizontalSlider_V,SLOT(setValue(int)));
    ui->horizontalSlider_H->setRange(0,359);
    ui->horizontalSlider_V->setRange(0,255);
    ui->horizontalSlider_S_2->setRange(0,255);
    ui->spinBox_H->setRange(0,359);
    ui->spinBox_S->setRange(0,255);
    ui->spinBox_V->setRange(0,255);
    setWindowTitle("HSV调节");
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));

}

DialogHSV::~DialogHSV()
{
    delete ui;
}

void DialogHSV::on_horizontalSlider_H_valueChanged(int value)
{
    emit hChanged(value-preH);
    preH=value;
}


void DialogHSV::on_horizontalSlider_S_2_valueChanged(int value)
{
    emit sChanged(value-preS);
    preS=value;
}


void DialogHSV::on_horizontalSlider_V_valueChanged(int value)
{
    emit vChanged(value-preV);
    preV=value;
}

