#include "dialogcolorreplace.h"
#include "ui_dialogcolorreplace.h"

DialogColorReplace::DialogColorReplace(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogColorReplace)
{
    ui->setupUi(this);
    connect(ui->label_des,SIGNAL(colorChanged(QColor)),this,SLOT(slots_colorChanged2(QColor)));
    connect(ui->label_src,SIGNAL(colorChanged(QColor)),this,SLOT(slots_colorChanged1(QColor)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    ui->spinBox->setRange(0,359);
    ui->horizontalSlider->setRange(0,359);
    ui->spinBox->setValue(20);
    setWindowTitle("颜色替换");
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogColorReplace::~DialogColorReplace()
{
    delete ui;
}

void DialogColorReplace::on_pushButton_clicked()
{
    this->accept();
}

void DialogColorReplace::slots_colorChanged1(QColor a)
{
    emit srcColor(a);
}

void DialogColorReplace::slots_colorChanged2(QColor a)
{
    emit desColor(a);
}


void DialogColorReplace::on_horizontalSlider_valueChanged(int value)
{
    emit tolChanged(value);
    // preTol=value;
}

