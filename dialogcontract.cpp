#include "dialogcontract.h"
#include "ui_dialogcontract.h"

DialogContract::DialogContract(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogContract)
{
    ui->setupUi(this);
    setWindowTitle("对比度调节");
    ui->spinBox_con->setRange(0,300);
    ui->horizontalSlider_con->setRange(0,300);
    connect(ui->spinBox_con,SIGNAL(valueChanged(int)),ui->horizontalSlider_con,SLOT(setValue(int)));
    connect(ui->horizontalSlider_con,SIGNAL(valueChanged(int)),ui->spinBox_con,SLOT(setValue(int)));
    ui->spinBox_bri->setRange(0,300);
    ui->horizontalSlider_bri->setRange(0,300);
    connect(ui->spinBox_bri,SIGNAL(valueChanged(int)),ui->horizontalSlider_bri,SLOT(setValue(int)));
    connect(ui->horizontalSlider_bri,SIGNAL(valueChanged(int)),ui->spinBox_bri,SLOT(setValue(int)));
    ui->spinBox_bri->setValue(150);
    ui->spinBox_con->setValue(150);
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogContract::~DialogContract()
{
    delete ui;
}

void DialogContract::on_pushButton_clicked()
{
    this->accept();
}


void DialogContract::on_horizontalSlider_bri_valueChanged(int value)
{
    emit bChanged(value);
}


void DialogContract::on_horizontalSlider_con_valueChanged(int value)
{
    emit cChanged(value);
}

