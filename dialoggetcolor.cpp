#include "dialoggetcolor.h"
#include "ui_dialoggetcolor.h"
#include <QPixmap>
#include <QImage>
#include <QApplication>
#include <QWindow>
#include <QScreen>
#include<QTimer>
DialogGetColor::DialogGetColor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogGetColor)
{
    ui->setupUi(this);
    // 创建全局点击监听器
    globalClickListener = new GlobalClickListener(this);
    // 连接信号和槽
    connect(globalClickListener, &GlobalClickListener::globalMouseClicked,
            this, &DialogGetColor::onGlobalMouseClicked);
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

DialogGetColor::~DialogGetColor()
{
    delete ui;

}

QColor DialogGetColor::getColor()
{
    return ui->label_theColor->m_color;
}

void DialogGetColor::getingColor()
{

    int x = QCursor::pos().x();
    int y = QCursor::pos().y();
    ui->lineEdit_pos->setText(tr("x:%1  y:%2").arg(x).arg(y));

    //获取坐标像素点
    QWindow window;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return;
    }
    QPixmap pixmap = screen->grabWindow(0, x, y, 1, 1);

    int red, green, blue;
    if(!pixmap.isNull())
    {
        QImage image = pixmap.toImage();

        if(!image.isNull())
        {
            QColor color = image.pixel(0,0);
            red = color.red();
            green = color.green();
            blue = color.blue();

            //输出16进制的颜色
            QString hRed = QString::number(red,16).toUpper();
            QString hGreen = QString::number(green,16).toUpper();
            QString hBlue = QString::number(blue,16).toUpper();
            ui->lineEdit_hex->setText(tr("#%1%2%3").arg(hRed).arg(hGreen).arg(hBlue));

            //输出RGB
            ui->lineEdit_rgb->setText(tr("R:%1 G:%2 B:%3").arg(red).arg(green).arg(blue));
            ui->label_color->setColor(color);

        }
    }
}

void DialogGetColor::setColor()
{
    ui->label_theColor->setColor(ui->label_color->m_color);
}

void DialogGetColor::onGlobalMouseClicked(const QPoint &globalPos)
{
    setColor();
}



void DialogGetColor::on_pushButton_ok_clicked()
{
    // 确保删除全局点击监听器
    delete globalClickListener;
}

