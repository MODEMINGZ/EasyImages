#include "colorslider.h"

#include <QWidget>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>


ColorSlider::ColorSlider(QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

ColorSlider::~ColorSlider()
{

}

void ColorSlider::init(ColorSpecType specType, ColorType type, const QColor &color, qreal min, qreal max)
{
    mMin = min;
    mMax = max;
    mColor = color;
    mColorType = type;
    mSpecType = specType;

    update();
}

// 绘制事件处理函数，在窗口部件需要重绘时被调用
void ColorSlider::paintEvent(QPaintEvent *)
{
    drawColorBox(mColor, size());

    QPainter painter(this);
    painter.drawPixmap(0, 0, mBoxPixmapSource);
    painter.end();

    drawPicker(mColor);
}

QLinearGradient ColorSlider::setColorSpec(const QColor &color)
{
    switch (mSpecType)
    {
    case HSV:
        return hsvGradient(color);
    case RGB:
        return rgbGradient(color);
    default:
        Q_UNREACHABLE();
    }
}

QLinearGradient ColorSlider::rgbGradient(const QColor &color)
{
    int val = 0;
    switch (mColorType){
    case RED:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromRgb(val,
                                                             255,
                                                             255,
                                                             color.alpha()));
        }
        break;
    case GREEN:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromRgb(color.red(),
                                                             val,
                                                             color.blue(),
                                                             color.alpha()));
        }
        break;
    case BLUE:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromRgb(color.red(),
                                                             color.green(),
                                                             val,
                                                             color.alpha()));
        }
        break;
    case ALPHA:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromRgb(0,
                                                             0,
                                                             0,
                                                             val));
        }
        break;
    default:
        Q_UNREACHABLE();
    }
    return mGradient;
}

QLinearGradient ColorSlider::hsvGradient(const QColor &color)
{
    int val = 0;
    switch (mColorType){
    case HUE:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromHsv(val,
                                                             255,
                                                             255,
                                                             color.alpha()));
        }
        break;
    case SAT:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromHsv(color.hsvHue(),
                                                             val,
                                                             color.value(),
                                                             color.alpha()));
        }
        break;
    case VAL:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromHsv(color.hsvHue(),
                                                             color.hsvSaturation(),
                                                             val,
                                                             color.alpha()));
        }
        break;
    case ALPHA:
        for (; val < mMax; val += 1){
            mGradient.setColorAt(val / mMax, QColor::fromHsv(0,
                                                             0,
                                                             0,
                                                             val));
        }
        break;
    default:
        Q_UNREACHABLE();
    }
    return mGradient;
}


void ColorSlider::drawColorBox(const QColor &color, QSize size)
{
    QStyleOption option;
    option.initFrom(this);

    QBrush backgroundBrush = option.palette.window();

    mBoxPixmapSource = QPixmap(size);

    QPainter painter(&mBoxPixmapSource);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(mBoxPixmapSource.rect(), backgroundBrush);

    mGradient = QLinearGradient(0,0,mBoxPixmapSource.width(),0);
    mGradient = setColorSpec(color);

    painter.end();

    painter.begin(&mBoxPixmapSource);
    //色块光标
    QBrush brush2(QBrush(QPixmap(":/images/images/color_brush.png")));

    painter.setBrush(brush2);
    QPen pen2;
    pen2.setWidthF(0);
    pen2.setColor(Qt::gray);
    pen2.setCosmetic(false);
    painter.setPen(pen2);
    painter.drawRoundedRect(0,
                            0,
                            mBoxPixmapSource.width(),
                            mBoxPixmapSource.height(),
                            4,
                            mBoxPixmapSource.width(),
                            Qt::SizeMode::AbsoluteSize);

    painter.end();

    painter.begin(&mBoxPixmapSource);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(mGradient);
    QPen pen;
    pen.setWidthF(0);
    pen.setColor(Qt::gray);
    pen.setCosmetic(false);
    painter.setPen(pen);

    painter.setBrush(brush);

    painter.drawRoundedRect(0,
                            0,
                            mBoxPixmapSource.width(),
                            mBoxPixmapSource.height(),
                            4,
                            mBoxPixmapSource.width(),
                            Qt::SizeMode::AbsoluteSize);
    painter.end();
}

QSize ColorSlider::sizeHint() const
{
    return {-1, 10};
}

void ColorSlider::mouseMoveEvent(QMouseEvent* event)
{
    colorPicked(event->pos());
}

void ColorSlider::mousePressEvent(QMouseEvent *event)
{
    colorPicked(event->pos());
}


// 绘制取色器
void ColorSlider::drawPicker(const QColor &color)
{
    QPainter painter(this);
    qreal val = 0;
    QSize mPickerSize = QSize(10, this->height() - 1);

    QPen pen;
    pen.setWidth(0);
    pen.setColor(QColor(0, 0, 0, 255));

    switch (mSpecType)
    {
    case HSV:
        switch (mColorType)
        {
        case HUE:
            val = color.hsvHueF() * (mBoxPixmapSource.width() - mPickerSize.width());
            break;
        case SAT:
            if ((color.hsvSaturation() > 127 || color.value() < 127) && color.alpha() > 127)
            {
                pen.setColor(Qt::white);
            }
            val = color.hsvSaturationF() * (mBoxPixmapSource.width() - mPickerSize.width());
            break;
        case VAL:
            if (color.value() < 127 && color.alpha() > 127)
            {
                pen.setColor(Qt::white);
            }
            val = color.valueF() * (mBoxPixmapSource.width() - mPickerSize.width());
            break;
        case ALPHA:
            break;
        default:
            Q_UNREACHABLE();
        }
        break;
    case RGB:
        switch (mColorType)
        {
        case RED:
            val = color.redF() * (mBoxPixmapSource.width() - mPickerSize.width());
            break;
        case GREEN:
            if (color.alpha() > 127)
            {
                pen.setColor(Qt::white);
            }
            val = color.greenF() * (mBoxPixmapSource.width() - mPickerSize.width());
            break;
        case BLUE:
            if (color.alpha() > 127)
            {
                pen.setColor(Qt::white);
            }
            val = color.blueF() * (mBoxPixmapSource.width() - mPickerSize.width());
            break;
        case ALPHA:
            break;
        default:
            Q_UNREACHABLE();
        }
        break;
    default:
        Q_UNREACHABLE();
    }
    if (mColorType == ALPHA)
    {
        if (color.alpha() > 127)
        {
            pen.setColor(Qt::white);
        }
        val = color.alphaF() * (mBoxPixmapSource.width() - mPickerSize.width());
    }

    painter.setPen(pen);
    painter.drawRect(static_cast<int>(val), 0, mPickerSize.width(), mPickerSize.height());
    painter.end();
}

//取色
void ColorSlider::colorPicked(QPoint point)
{
    QColor colorPicked = mColor;
    int colorMax = static_cast<int>(mMax);

    int colorVal = point.x() * colorMax / mBoxPixmapSource.width();

    colorVal = (colorVal > colorMax) ? colorMax : colorVal;
    colorVal = (colorVal < 0) ? 0 : colorVal;

    switch (mSpecType)
    {
    case HSV:
        switch (mColorType)
        {
        case HUE:
        {
            colorPicked = QColor::fromHsv(colorVal,
                                          mColor.hsvSaturation(),
                                          mColor.value(),
                                          mColor.alpha());

            break;
        }
        case SAT:
        {
            colorPicked = QColor::fromHsv(mColor.hsvHue(),
                                          colorVal,
                                          mColor.value(),
                                          mColor.alpha());
            break;
        }
        case VAL:
        {
            colorPicked = QColor::fromHsv(mColor.hsvHue(),
                                          mColor.hsvSaturation(),
                                          colorVal,
                                          mColor.alpha());
            break;
        }
        case ALPHA:
        {

            colorPicked = QColor::fromHsv(mColor.hsvHue(),
                                          mColor.hsvSaturation(),
                                          mColor.value(),
                                          colorVal);
            break;
        }
        default:
            Q_UNREACHABLE();
        }
        break;
    case RGB:
        switch(mColorType){
        case RED:{
            colorPicked = QColor::fromRgb(colorVal,
                                          mColor.green(),
                                          mColor.blue(),
                                          mColor.alpha());

        }
            break;
        case GREEN:{
            colorPicked = QColor::fromRgb(mColor.red(),
                                          colorVal,
                                          mColor.blue(),
                                          mColor.alpha());
        }
            break;
        case BLUE:{
            colorPicked = QColor::fromRgb(mColor.red(),
                                          mColor.green(),
                                          colorVal,
                                          mColor.alpha());
        }
            break;
        case ALPHA:{
            colorPicked = QColor::fromRgb(mColor.red(),
                                          mColor.green(),
                                          mColor.blue(),
                                          colorVal);

        }
            break;
        default:
            Q_UNREACHABLE();
        }
        break;
    default:
        Q_UNREACHABLE();
    }
    mColor = colorPicked;
    emit valueChanged(mColor);
}
