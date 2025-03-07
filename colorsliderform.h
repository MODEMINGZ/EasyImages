#ifndef COLORSLIDERFORM_H
#define COLORSLIDERFORM_H

#include <QWidget>
#include <QPaintEvent>
#include<QAbstractSpinBox>
namespace Ui {
class ColorSliderForm;
}

class ColorSliderForm : public QWidget
{
    Q_OBJECT
    friend class ColorSliders;

public:
    explicit ColorSliderForm(QWidget *parent = nullptr);
    ~ColorSliderForm();
    QColor getColor();
    void setColor(QColor color);

signals:
    void colorChanged(const QColor& color);
    //页面变化
    void colorSpecChanged(const bool& isRgb);

private slots:
    void onColorChanged();
    void onColorChanged(const QColor& color);
    void onColorSpecChanged();

private:
    void clearFocusOnFinished(QAbstractSpinBox *spinBox);
    Ui::ColorSliderForm *ui;
    QColor mCurrentColor;
    bool isRgbColors=true;
};

#endif // COLORSLIDERFORM_H
