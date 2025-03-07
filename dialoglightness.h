#ifndef DIALOGLIGHTNESS_H
#define DIALOGLIGHTNESS_H

#include <QDialog>

namespace Ui {
class DialogLightness;
}

class DialogLightness : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLightness(QWidget *parent = nullptr);
    ~DialogLightness();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);
signals:
    void lChanged(int);
private:
    Ui::DialogLightness *ui;
    int preL=0;
};

#endif // DIALOGLIGHTNESS_H
