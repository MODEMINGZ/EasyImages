#ifndef COLORSLIDER_H
#define COLORSLIDER_H

#include <QWidget>

class ColorSlider : public QWidget
{
    Q_OBJECT
public:


    explicit ColorSlider(QWidget* parent);
    ~ColorSlider() override;

    enum ColorType {HUE,SAT,VAL,RED,GREEN,BLUE,ALPHA};
    enum ColorSpecType {RGB,HSV,HSL,CMYK};

    void init(ColorSpecType specType, ColorType type, const QColor &color, qreal min, qreal max);

    QLinearGradient setColorSpec(const QColor &color);

    QColor color() { return mColor; }

    void setHsv(const QColor& hsv) {
        mColor.setHsv(hsv.hsvHue(),
                      hsv.hsvSaturation(),
                      hsv.value(),
                      hsv.alpha());
    }

    void setRgb(const QColor& rgb) {
        mColor.setRgb(rgb.red(),
                      rgb.green(),
                      rgb.blue(),
                      rgb.alpha());
    }

    void setColorSpecType(ColorSpecType newType) { this->mSpecType = newType; }
    void setColorType(ColorType newType) { this->mColorType = newType; }

    void setMin(qreal min) { mMin = min; }
    void setMax(qreal max) { mMax = max; }

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void valueChanged(QColor color);

private:

    void drawColorBox(const QColor &color, QSize size);
    void drawPicker(const QColor &color);
    QLinearGradient hsvGradient(const QColor &color);
    QLinearGradient rgbGradient(const QColor &color);

    void colorPicked(QPoint point);

    QPixmap mBoxPixmapSource;

    QColor mColor;
    qreal mMin = 0.0;
    qreal mMax = 0.0;

    ColorType mColorType = ColorType::HUE;
    ColorSpecType mSpecType = ColorSpecType::RGB;

    QLinearGradient mGradient;
};


#endif // COLORSLIDER_H
