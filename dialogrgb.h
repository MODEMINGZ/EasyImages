#ifndef DIALOGRGB_H
#define DIALOGRGB_H

#include <QDialog>
#include <QElapsedTimer>
namespace Ui {
class DialogRGB;
}

class DialogRGB : public QDialog
{
    Q_OBJECT
    QElapsedTimer lastEmitTime; // 记录上次发射信号的时间
    const int emitInterval = 100; // 设置发射信号的间隔（毫秒）


public:
    explicit DialogRGB(QWidget *parent = nullptr);
    ~DialogRGB();
signals:
    void rChanged(int);
    void gChanged(int);
    void bChanged(int);
private slots:
    void on_horizontalSlider_blue_valueChanged(int value);

    void on_horizontalSlider_green_valueChanged(int value);

    void on_horizontalSlider_red_valueChanged(int value);

private:
    Ui::DialogRGB *ui;
    int preR=0;
    int preG=0;
    int preB=0;
};

#endif // DIALOGRGB_H
