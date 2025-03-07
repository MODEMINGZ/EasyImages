#ifndef CUSTOMPATHARROW_H
#define CUSTOMPATHARROW_H

#include <QObject>
#include <QGraphicsPathItem>
class CustomPathArrow:public QGraphicsPathItem
{
public:
    CustomPathArrow(QGraphicsItem *parent = nullptr);
    void addPoint(const QPointF &point);
    void setCurrentPoint(const QPointF &point);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void deletePoint();
    void clearCurrentPoint();
    QRectF boundingRect() const override;
private:
    QVector<QPointF> points;
    QPointF currentPoint=QPointF(0,0);
};

#endif // CUSTOMPATHARROW_H
