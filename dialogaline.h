#ifndef DIALOGALINE_H
#define DIALOGALINE_H

#include <QDialog>

namespace Ui {
class DialogALine;
}

class DialogALine : public QDialog
{
    Q_OBJECT

public:
    explicit DialogALine(QWidget *parent = nullptr);
    ~DialogALine();
    double width,height,angle;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

private:
    Ui::DialogALine *ui;
};

#endif // DIALOGALINE_H
