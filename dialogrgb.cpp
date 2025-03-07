#include "dialogrgb.h"
#include "ui_dialogrgb.h"

DialogRGB::DialogRGB(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogRGB)
{
    ui->setupUi(this);
    connect(ui->spinBox_blue,SIGNAL(valueChanged(int)),ui->horizontalSlider_blue,SLOT(setValue(int)));
    connect(ui->spinBox_red,SIGNAL(valueChanged(int)),ui->horizontalSlider_red,SLOT(setValue(int)));
    connect(ui->spinBox_green,SIGNAL(valueChanged(int)),ui->horizontalSlider_green,SLOT(setValue(int)));
    connect(ui->horizontalSlider_green,SIGNAL(valueChanged(int)),ui->spinBox_green,SLOT(setValue(int)));
    connect(ui->horizontalSlider_red,SIGNAL(valueChanged(int)),ui->spinBox_red,SLOT(setValue(int)));
    connect(ui->horizontalSlider_blue,SIGNAL(valueChanged(int)),ui->spinBox_blue,SLOT(setValue(int)));
    ui->spinBox_blue->setRange(0,255);
    ui->horizontalSlider_blue->setRange(0,255);
    ui->spinBox_red->setRange(0,255);
    ui->horizontalSlider_red->setRange(0,255);
    ui->spinBox_green->setRange(0,255);
    ui->horizontalSlider_green->setRange(0,255);
    setWindowTitle("RGB调节");
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogRGB::~DialogRGB()
{
    delete ui;
}

void DialogRGB::on_horizontalSlider_blue_valueChanged(int value)
{

    emit bChanged(value-preB);
    preB=value;
}


void DialogRGB::on_horizontalSlider_green_valueChanged(int value)
{

    emit gChanged(value-preG);
    preG=value;
}


void DialogRGB::on_horizontalSlider_red_valueChanged(int value)
{
    emit rChanged(value-preR);
    preR=value;
}

