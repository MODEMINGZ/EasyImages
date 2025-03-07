#include "dialogcoldco.h"
#include "ui_dialogcoldco.h"

DialogColdCo::DialogColdCo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogColdCo)
{
    ui->setupUi(this);
    setWindowTitle("冷色调节");
    ui->spinBox->setRange(0,255);
    ui->horizontalSlider->setRange(0,255);
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogColdCo::~DialogColdCo()
{
    delete ui;
}

void DialogColdCo::on_pushButton_clicked()
{
    this->accept();
}


void DialogColdCo::on_horizontalSlider_valueChanged(int value)
{
    emit cChanged(value-preC);
    preC=value;
}

