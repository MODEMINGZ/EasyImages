#include "cutimage.h"
#include "ui_cutimage.h"

#include <QPixmap>
#include <QStyledItemDelegate>
#include<QFileDialog>
#include<QMessageBox>

CutImage::CutImage(const QString &filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CutImage)
    , m_orignalImage(filename),
    file(filename)
{
    ui->setupUi(this);

    initView();
    initStyle();
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

CutImage::~CutImage()
{
    delete ui;
}

QPixmap CutImage::getCropImage()
{
    return ui->lb_showimage->getCroppedImage();
}

void CutImage::initView()
{
    this->setWindowTitle(tr("裁切图片"));
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);

    ui->rb_square->setChecked(true);
    on_rb_square_clicked();

    ui->rb_nofix->setChecked(true);
    ui->rb_free->setChecked(true);
    on_rb_free_clicked();

    int height = m_orignalImage.height();
    int width = m_orignalImage.width();
    ui->label_height->setText(QString("%1").arg(height));
    ui->label_width->setText(QString("%1").arg(width));

    if(height>900){
        height=9000;
    }
    if(width>1000){
        width=1000;
    }
    this->resize(width+100, height+220);

    ui->sb_height->setRange(1, height);
    ui->sb_width->setRange(1, width);

    ui->sb_linewidth->setRange(0, 10);
    ui->sb_lineheight->setRange(0, 10);

    ui->lb_showimage->setImage(m_orignalImage);
    ui->label_6->setEnabled(false);
    ui->sb_lineheight->setEnabled(false);
    ui->sb_linewidth->setEnabled(false);

}

void CutImage::initStyle()
{
    QStyledItemDelegate * itemDelegate = new QStyledItemDelegate(this);
    ui->cb_difficulty->setItemDelegate(itemDelegate);
}

void CutImage::on_cb_difficulty_currentIndexChanged(int index)
{
    disconnect(ui->sb_linewidth,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropInternalLineChange);
    disconnect(ui->sb_lineheight,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropInternalLineChange);

    switch (index) {
    case 0:
        ui->label_6->setEnabled(false);
        ui->sb_lineheight->setEnabled(false);
        ui->sb_linewidth->setEnabled(false);
        ui->lb_showimage->setCropBoxLine(4,4);
        break;
    case 1:
        ui->label_6->setEnabled(false);
        ui->sb_lineheight->setEnabled(false);
        ui->sb_linewidth->setEnabled(false);
        ui->lb_showimage->setCropBoxLine(5,5);
        break;
    case 2:
        ui->label_6->setEnabled(false);
        ui->sb_lineheight->setEnabled(false);
        ui->sb_linewidth->setEnabled(false);
        ui->lb_showimage->setCropBoxLine(6,6);
        break;
    case 3:
        ui->label_6->setEnabled(true);
        ui->sb_lineheight->setEnabled(true);
        ui->sb_linewidth->setEnabled(true);
        connect(ui->sb_linewidth,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropInternalLineChange);
        connect(ui->sb_lineheight,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropInternalLineChange);
        slotCropInternalLineChange();
        break;
    default:
        break;
    }
}

void CutImage::on_rb_square_clicked()
{
    ui->lb_showimage->setCropBoxShape(CropBox::CropBoxShape::Round);
}

void CutImage::on_rb_rectangle_clicked()
{
    ui->lb_showimage->setCropBoxShape(CropBox::CropBoxShape::Rect);
}

void CutImage::on_rb_nofix_clicked()
{
    disconnect(ui->sb_width,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropSizeChange);
    disconnect(ui->sb_height,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropSizeChange);

    ui->lb_showimage->setfixCropBox(this->width(), this->height(), false);
}

void CutImage::on_rb_fixszie_clicked()
{
    connect(ui->sb_width,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropSizeChange);
    connect(ui->sb_height,  static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &CutImage::slotCropSizeChange);

    slotCropSizeChange();
}

void CutImage::slotCropSizeChange()
{
    ui->lb_showimage->setfixCropBox(ui->sb_width->value(), ui->sb_height->value(), true);
}

void CutImage::slotCropInternalLineChange()
{
    ui->lb_showimage->setCropBoxLine(ui->sb_linewidth->value(), ui->sb_lineheight->value());
}

void CutImage::on_ptn_sure_clicked()
{
    QPixmap cropImage=this->getCropImage();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("文件保存"),
                                                    QFileInfo(file).baseName()+"_croped",
                                                    tr("图片 (*.png)"));

    if (!fileName.isNull())
    {
        cropImage.save(fileName);
        QMessageBox::information(this,"EasyImages","保存成功！",QMessageBox::Ok);
    }
}

void CutImage::on_ptn_cancel_clicked()
{
    this->reject();
}

void CutImage::on_rb_ratio_clicked()
{
    ui->lb_showimage->setCropBoxZoomMode(CropBox::ZoomMode::Square);
}

void CutImage::on_rb_free_clicked()
{
    ui->lb_showimage->setCropBoxZoomMode(CropBox::ZoomMode::Free);
}

void CutImage::on_rb_fixed_clicked()
{
    ui->lb_showimage->setCropBoxZoomMode(CropBox::ZoomMode::FixedRatio);
}
