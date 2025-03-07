#ifndef GRAPHICDRAWVIEW_H
#define GRAPHICDRAWVIEW_H

#include <QGraphicsView>
#include<QMouseEvent>
#include<QPoint>
#include<QGraphicsScene>
#include<QTreeWidget>
#include<QMap>
#include<QGraphicsItem>
#include <QLineF>
#include <QPainterPath>
#include"customitemlayer.h"
#include "customtreewidgetlayer.h"
// #include <QUndoStack>
// #include <QUndoView>
class GraphicDrawView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicDrawView(QWidget *parent = nullptr);
    ~GraphicDrawView();

    enum{ItemId=1,ItemDesciption};
    enum{AuxiliaryLineType=100001,LayerType,LayerClassType};
    int seqNum=0;
    //layer
    CustomTreeWidgetLayer*layerForm=nullptr;
    qreal minZ=0;
    qreal maxZ=100000;
    qreal theSpan=1000;
    void resizeLayer();
    int classNum=1;
    int LayerNum=1;
    QColor backColor;
    QGraphicsScene *scene;
    QMap<QGraphicsItem*,CustomItemLayer*>itemToLayerMap;
    bool controled=false;

    void setBackGround(QColor,qreal,qreal);
    // QUndoStack undoStack;
    // QUndoView undoView;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect)override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void slot_Layer_droped();

signals:
    void mouseMove(QMouseEvent *event);
    void mouseMovePoint(QPoint point);
    void mouseClicked(QPoint point);
    void mouseDoubleClick(QPoint point);
    void keyPress(QKeyEvent *event);
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
private:
};

#endif // GRAPHICDRAWVIEW_H
