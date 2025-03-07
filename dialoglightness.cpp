#include "dialoglightness.h"
#include "ui_dialoglightness.h"

DialogLightness::DialogLightness(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogLightness)
{
    ui->setupUi(this);
    setWindowTitle("亮度调节");
    ui->spinBox->setRange(0,255);
    ui->horizontalSlider->setRange(0,255);
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogLightness::~DialogLightness()
{
    delete ui;
}

void DialogLightness::on_pushButton_clicked()
{
    this->accept();
}


void DialogLightness::on_horizontalSlider_valueChanged(int value)
{
    emit lChanged(value-preL);
    preL=value;
}

