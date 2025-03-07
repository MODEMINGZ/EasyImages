#ifndef DIALOGBINA_H
#define DIALOGBINA_H

#include <QDialog>

namespace Ui {
class DialogBina;
}

class DialogBina : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBina(QWidget *parent = nullptr);
    ~DialogBina();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);
signals:
    void bChanged(int);

private:
    Ui::DialogBina *ui;
};

#endif // DIALOGBINA_H
