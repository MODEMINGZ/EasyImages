#ifndef DIALOGWARMCO_H
#define DIALOGWARMCO_H

#include <QDialog>

namespace Ui {
class DialogWarmCo;
}

class DialogWarmCo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWarmCo(QWidget *parent = nullptr);
    ~DialogWarmCo();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_2_valueChanged(int value);
signals:
    void wChanged(int);
private:
    Ui::DialogWarmCo *ui;
    int preW=0;
};

#endif // DIALOGWARMCO_H
