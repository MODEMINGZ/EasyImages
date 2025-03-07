#include "dialogwarmco.h"
#include "ui_dialogwarmco.h"

DialogWarmCo::DialogWarmCo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogWarmCo)
{
    ui->setupUi(this);
    setWindowTitle("暖色调节");
    ui->spinBox_2->setRange(0,255);
    ui->horizontalSlider_2->setRange(0,255);
    connect(ui->spinBox_2,SIGNAL(valueChanged(int)),ui->horizontalSlider_2,SLOT(setValue(int)));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),ui->spinBox_2,SLOT(setValue(int)));
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogWarmCo::~DialogWarmCo()
{
    delete ui;
}

void DialogWarmCo::on_pushButton_clicked()
{
    this->accept();
}


void DialogWarmCo::on_horizontalSlider_2_valueChanged(int value)
{
    emit wChanged(value-preW);
    preW=value;
}

