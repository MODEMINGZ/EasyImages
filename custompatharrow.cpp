#include "custompatharrow.h"
#include<QPainter>
CustomPathArrow::CustomPathArrow(QGraphicsItem *parent):
    QGraphicsPathItem(parent) {
}

void CustomPathArrow::addPoint(const QPointF &point)
{
    points.push_back(point);
    update();
}

QRectF CustomPathArrow::boundingRect() const {
    if (points.isEmpty()) return QRectF();
    if(points.size()==2){
        QPointF A = points[0];
        QPointF B = points[1];

        qreal width = qAbs(B.x() - A.x()); // 计算两点之间的宽度
        qreal height = qAbs(B.y() - A.y()); // 计算两点之间的高度

        // 确保宽度和高度都不小于40
        width = qMax(width, 40.0);
        height = qMax(height, 40.0);

        // 确定矩形的左上角点
        qreal left = qMin(A.x(), B.x()) - (width - qAbs(B.x() - A.x())) / 2;
        qreal top = qMin(A.y(), B.y()) - (height - qAbs(B.y() - A.y())) / 2;

        // 返回构建的矩形
        return QRectF(left, top, width, height);

    }

    qreal arrowSize = 10; // 箭头大小，根据实际绘制箭头的大小调整
    qreal extra = arrowSize / 2.0; // 额外增加的边距，确保箭头不会超出边界

    qreal leftMost = points[0].x();
    qreal rightMost = points[0].x();
    qreal topMost = points[0].y();
    qreal bottomMost = points[0].y();

    for (const auto& point : points) {
        leftMost = qMin(leftMost, point.x() - extra);
        rightMost = qMax(rightMost, point.x() + extra);
        topMost = qMin(topMost, point.y() - extra);
        bottomMost = qMax(bottomMost, point.y() + extra);
    }
    qreal addW=0;
    qreal addH=0;
    qreal r=50;
    if(rightMost-topMost<r){
        addW=r-(rightMost-topMost);
    }
    if(bottomMost-topMost<r){
        addH=r-(bottomMost-topMost<r);
    }
    QRectF rect(QPointF(leftMost, topMost),QPointF(rightMost+addW,bottomMost+addH));
    return rect;
}
void CustomPathArrow::setCurrentPoint(const QPointF &point)
{
    currentPoint=point;
    update();
}

void CustomPathArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (points.isEmpty()) return;

    //绘制折线
    QPainterPath p;
    p.moveTo(points.first());
    for (int i = 1; i < points.size(); ++i) {
        p.lineTo(points[i]);
    }
    p.lineTo(currentPoint);

    setPath(p);

    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawPath(p);

    //最后一个点绘制箭头
    if (points.size() > 1) {
        QPointF lastPoint = points.back();
        QPointF preLastPoint = points[points.length()-2];

        if(lastPoint==preLastPoint){
            if(points.size()>2){
                preLastPoint = points[points.length()-3];
            }
        }

        qreal arrowSize = 10;
        QLineF line(lastPoint, preLastPoint);
        double angle = std::atan2(-line.dy(), line.dx());
        QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                              cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                              cos(angle + M_PI - M_PI / 3) * arrowSize);

        // 箭头
        QPainterPath arrowPath;
        arrowPath.moveTo(lastPoint);
        arrowPath.lineTo(arrowP1);
        arrowPath.moveTo(lastPoint);
        arrowPath.lineTo(arrowP2);
        arrowPath.closeSubpath();

        painter->drawPath(arrowPath);
        setPath(p+arrowPath);
    }
}

void CustomPathArrow::deletePoint()
{
    if (points.isEmpty()) return;
    points.pop_back();
    update();
}

void CustomPathArrow::clearCurrentPoint()
{
    if(points.size() > 1){
        this->currentPoint=points.back();
    }

}
