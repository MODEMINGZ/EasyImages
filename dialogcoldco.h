#ifndef DIALOGCOLDCO_H
#define DIALOGCOLDCO_H

#include <QDialog>

namespace Ui {
class DialogColdCo;
}

class DialogColdCo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogColdCo(QWidget *parent = nullptr);
    ~DialogColdCo();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);
signals:
    void cChanged(int);
private:
    Ui::DialogColdCo *ui;
    int preC=0;
};

#endif // DIALOGCOLDCO_H
