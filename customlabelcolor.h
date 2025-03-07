#ifndef CUSTOMLABELCOLOR_H
#define CUSTOMLABELCOLOR_H

#include <QLabel>
#include <QColorDialog>
#include <QMouseEvent>

class CustomLabelColor : public QLabel
{
    Q_OBJECT
public:
    explicit CustomLabelColor(QObject *parent = nullptr);
    void clearColor();
    QColor m_color;
    bool hasColor=false;
signals:
    void colorChanged(const QColor& color);
public slots:
    void setColor(const QColor& color);
protected:
    void mousePressEvent(QMouseEvent *ev) override;
};

#endif // CUSTOMLABELCOLOR_H
