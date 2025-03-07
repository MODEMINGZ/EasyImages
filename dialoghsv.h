#ifndef DIALOGHSV_H
#define DIALOGHSV_H

#include <QDialog>

namespace Ui {
class DialogHSV;
}

class DialogHSV : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHSV(QWidget *parent = nullptr);
    ~DialogHSV();

signals:
    void hChanged(int);
    void sChanged(int);
    void vChanged(int);
private slots:
    void on_horizontalSlider_H_valueChanged(int value);

    void on_horizontalSlider_S_2_valueChanged(int value);

    void on_horizontalSlider_V_valueChanged(int value);

private:
    Ui::DialogHSV *ui;
    int preH=0;
    int preV=0;
    int preS=0;
};

#endif // DIALOGHSV_H
