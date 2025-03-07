#ifndef DIALOGCONTRACT_H
#define DIALOGCONTRACT_H

#include <QDialog>

namespace Ui {
class DialogContract;
}

class DialogContract : public QDialog
{
    Q_OBJECT

public:
    explicit DialogContract(QWidget *parent = nullptr);
    ~DialogContract();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_bri_valueChanged(int value);

    void on_horizontalSlider_con_valueChanged(int value);
signals:
    void bChanged(int);
    void cChanged(int);

private:
    Ui::DialogContract *ui;

};

#endif // DIALOGCONTRACT_H
