#include "customlabelcolor.h"
#include<QColorDialog>
CustomLabelColor::CustomLabelColor(QObject *parent)
{
    Q_UNUSED(parent);
    this->setFrameShape(QFrame::Box);
    this->setFrameShadow(QFrame::Raised);
}

void CustomLabelColor::setColor(const QColor& color)
{
    this->hasColor=true;
    this->setText("");
    this->setAutoFillBackground(true);
    this->setPalette(QPalette(color));
    m_color=color;
    emit colorChanged(color);

}

void CustomLabelColor::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    QColorDialog*dia=new  QColorDialog();
    if(dia->exec()== QColorDialog::Accepted ){
        setColor(dia->currentColor());
    }
}

void CustomLabelColor::clearColor()
{
    this->setPalette(QPalette());
    this->setText("null");
}

