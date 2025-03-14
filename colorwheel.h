#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>

class ColorWheel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWheel(QWidget* parent=nullptr);

    QColor color();

signals:
    void colorSelected(const QColor& color);
    void colorChanged(const QColor& color);

public slots:
    void setColor(QColor color);

protected:
    //重写
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void paintEvent(QPaintEvent*) override;

private:
    void hueChanged(const int& hue);
    void saturationChanged(const int& sat);
    void valueChanged(const int& value);

    QColor pickColor(const QPoint& point);

    void drawHueIndicator(const int& hue);
    void drawPicker(const QColor& color);

    void drawWheelImage(const QSize& newSize);
    void drawSquareImage(const int& hue);
    void composeWheel(QPixmap& pixmap);

private:
    QSize mInitSize{ 20, 20 };
    QImage mWheelImage;
    QImage mSquareImage;
    QPixmap mWheelPixmap;

    int mWheelThickness = 20;
    QRect mWheelRect;
    QRect mSquareRect;
    QColor mCurrentColor = Qt::red;
    //判断
    bool mIsInWheel = false;
    bool mIsInSquare = false;

};

#endif // COLORWHEEL_H
