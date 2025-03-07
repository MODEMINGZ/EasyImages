#include "colorsliderform.h"
#include "ui_colorsliderform.h"

ColorSliderForm::ColorSliderForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorSliderForm)
{
    ui->setupUi(this);

    //初始化颜色
    this->setColor(qRgba(0,0,0,100));

    //初始化色条
    ui->redSlider->init(ColorSlider::ColorSpecType::RGB, ColorSlider::ColorType::RED, mCurrentColor, 0.0, 255.0);
    ui->greenSlider->init(ColorSlider::ColorSpecType::RGB, ColorSlider::ColorType::GREEN, mCurrentColor, 0.0, 255.0);
    ui->blueSlider->init(ColorSlider::ColorSpecType::RGB, ColorSlider::ColorType::BLUE, mCurrentColor, 0.0, 255.0);
    ui->rgbAlphaSlider->init(ColorSlider::ColorSpecType::RGB, ColorSlider::ColorType::ALPHA, mCurrentColor, 0.0, 255.0);

    ui->hueSlider->init(ColorSlider::ColorSpecType::HSV, ColorSlider::ColorType::HUE, mCurrentColor, 0.0, 359.0);
    ui->saturationSlider->init(ColorSlider::ColorSpecType::HSV, ColorSlider::ColorType::SAT, mCurrentColor, 0.0, 255.0);
    ui->valueSlider->init(ColorSlider::ColorSpecType::HSV, ColorSlider::ColorType::VAL, mCurrentColor, 0.0, 255.0);
    ui->hsvAlphaSlider->init(ColorSlider::ColorSpecType::HSV, ColorSlider::ColorType::ALPHA, mCurrentColor, 0.0, 255.0);

    //色条
    auto onColorChangedSlider = static_cast<void(ColorSliderForm::*)(const QColor&)>(&ColorSliderForm::onColorChanged);
    connect(ui->redSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);
    connect(ui->greenSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);
    connect(ui->blueSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);
    connect(ui->rgbAlphaSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);
    connect(ui->hueSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);
    connect(ui->saturationSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);
    connect(ui->valueSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);
    connect(ui->hsvAlphaSlider, &ColorSlider::valueChanged, this, onColorChangedSlider);

    //色值
    auto spinBoxChanged = static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged);
    auto onColorChangedSpinBox = static_cast<void(ColorSliderForm::*)()>(&ColorSliderForm::onColorChanged);
    connect(ui->redSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->redSpinBox);
    connect(ui->greenSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->greenSpinBox);
    connect(ui->blueSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->blueSpinBox);
    connect(ui->rgbAlphaSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->rgbAlphaSpinBox);
    connect(ui->hueSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->hueSpinBox);
    connect(ui->saturationSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->saturationSpinBox);
    connect(ui->valueSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->valueSpinBox);
    connect(ui->hsvAlphaSpinBox, spinBoxChanged, this, onColorChangedSpinBox);
    clearFocusOnFinished(ui->hsvAlphaSpinBox);

    connect(ui->colorSpecTabWidget, &QTabWidget::currentChanged, this, &ColorSliderForm::onColorSpecChanged);
}

ColorSliderForm::~ColorSliderForm()
{
    delete ui;
}

QColor ColorSliderForm::getColor()
{
    return mCurrentColor;
}

void ColorSliderForm::setColor(QColor color)
{
    mCurrentColor=color;

    QSignalBlocker b1(ui->redSlider);
    QSignalBlocker b2(ui->greenSlider);
    QSignalBlocker b3(ui->blueSlider);
    QSignalBlocker b4(ui->rgbAlphaSlider);

    ui->redSlider->setRgb(mCurrentColor);
    ui->greenSlider->setRgb(mCurrentColor);
    ui->blueSlider->setRgb(mCurrentColor);
    ui->rgbAlphaSlider->setRgb(mCurrentColor);

    QSignalBlocker b5(ui->redSpinBox);
    QSignalBlocker b6(ui->greenSpinBox);
    QSignalBlocker b7(ui->blueSpinBox);
    QSignalBlocker b8(ui->rgbAlphaSpinBox);

    ui->redSpinBox->setValue(mCurrentColor.red());
    ui->greenSpinBox->setValue(mCurrentColor.green());
    ui->blueSpinBox->setValue(mCurrentColor.blue());
    ui->rgbAlphaSpinBox->setValue(mCurrentColor.alpha());

    QSignalBlocker b9(ui->hueSlider);
    QSignalBlocker b10(ui->saturationSlider);
    QSignalBlocker b11(ui->valueSlider);
    QSignalBlocker b12(ui->hsvAlphaSlider);

    ui->hueSlider->setHsv(mCurrentColor);
    ui->saturationSlider->setHsv(mCurrentColor);
    ui->valueSlider->setHsv(mCurrentColor);
    ui->hsvAlphaSlider->setHsv(mCurrentColor);

    QSignalBlocker b13(ui->hueSpinBox);
    QSignalBlocker b14(ui->saturationSpinBox);
    QSignalBlocker b15(ui->valueSpinBox);
    QSignalBlocker b16(ui->hsvAlphaSpinBox);

    ui->hueSpinBox->setValue(mCurrentColor.hue());
    ui->saturationSpinBox->setValue(qRound(mCurrentColor.saturation() / 2.55));
    ui->valueSpinBox->setValue(qRound(mCurrentColor.value() / 2.55));
    ui->hsvAlphaSpinBox->setValue(qRound(mCurrentColor.alpha() / 2.55));

    update();
}

void ColorSliderForm::onColorSpecChanged()
{
    isRgbColors = ui->colorSpecTabWidget->currentWidget() == ui->rgbTab;
    if (isRgbColors){
        mCurrentColor = mCurrentColor.toRgb();
    }
    else{
        mCurrentColor = mCurrentColor.toHsv();
    }

    this->setColor(this->mCurrentColor);

    emit colorSpecChanged(isRgbColors);
}

void ColorSliderForm::onColorChanged()
{
    if (isRgbColors) {
        mCurrentColor.setRgb(
            ui->redSpinBox->value(),
            ui->greenSpinBox->value(),
            ui->blueSpinBox->value(),
            ui->rgbAlphaSpinBox->value());
    } else {
        mCurrentColor.setHsv(
            ui->hueSpinBox->value(),
            static_cast<int>(ui->saturationSpinBox->value() * 2.55),
            static_cast<int>(ui->valueSpinBox->value() * 2.55),
            static_cast<int>(ui->hsvAlphaSpinBox->value() * 2.55));
    }

    this->setColor(mCurrentColor);
    emit colorChanged(mCurrentColor);
}

void ColorSliderForm::onColorChanged(const QColor &color)
{
    this->setColor(color);
    emit colorChanged(color);
}

void ColorSliderForm::clearFocusOnFinished(QAbstractSpinBox *spinBox)
{
    QObject::connect(spinBox, &QAbstractSpinBox::editingFinished, spinBox, &QAbstractSpinBox::clearFocus);
}
