#ifndef DIALOGCOLORREPLACE_H
#define DIALOGCOLORREPLACE_H

#include <QDialog>

namespace Ui {
class DialogColorReplace;
}

class DialogColorReplace : public QDialog
{
    Q_OBJECT

public:
    explicit DialogColorReplace(QWidget *parent = nullptr);
    ~DialogColorReplace();

private slots:
    void on_pushButton_clicked();
    void slots_colorChanged1(QColor);
    void slots_colorChanged2(QColor);

    void on_horizontalSlider_valueChanged(int value);

signals:
    void desColor(QColor);
    void srcColor(QColor);
    void tolChanged(int);
private:
    int preTol=0;
    Ui::DialogColorReplace *ui;
};

#endif // DIALOGCOLORREPLACE_H
