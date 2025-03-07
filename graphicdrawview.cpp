#include "graphicdrawview.h"
#include<QPixmap>
#include<QPainter>
#include<QColor>
#include<QScrollBar>
#include<QGraphicsRectItem>
#include"customitemclass.h"
GraphicDrawView::GraphicDrawView(QWidget *parent)
    : QGraphicsView{parent}
{
    //scene
    scene=new QGraphicsScene();
    setScene(scene);
    //view
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setAttribute(Qt::WA_DeleteOnClose);
    //渲染提示
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//反绘制拖影
    setRenderHint(QPainter::Antialiasing); //抗锯齿
    setRenderHint(QPainter::SmoothPixmapTransform); //平滑像素变换
    //实现鼠标中心缩放
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    //setDragMode(QGraphicsView::ScrollHandDrag);//启用拖拽

    //layer
    layerForm=new CustomTreeWidgetLayer();
    //子项可拖动
    layerForm->setDragEnabled(true);
    layerForm->setDragDropMode(QAbstractItemView::InternalMove);
    //隐藏表头
    layerForm->setHeaderHidden(true);
    //双击子项可编辑

    //可多选
    layerForm->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //绑定拖拽更新
    connect(layerForm,SIGNAL(CustomDropedSignal()),this,SLOT(slot_Layer_droped()));
    //导出背景透明
    QBrush transparentBrush(Qt::transparent);
    this->scene->setBackgroundBrush(transparentBrush);

}

GraphicDrawView::~GraphicDrawView()
{
    delete scene;
}

void GraphicDrawView::resizeLayer()
{
    qreal span=(maxZ-minZ)/theSpan;
    qreal cur=minZ+span;
    qreal pre=minZ;
    int count=this->layerForm->topLevelItemCount();
    //注意倒序
    for(int i=count-1;i>=0;i--){
        QTreeWidgetItem*item=this->layerForm->topLevelItem(i);
        if(item->type()==LayerClassType){
            CustomItemClass*tItem=dynamic_cast<CustomItemClass*>(item);
            if(tItem){
                tItem->setzValue(pre,cur);
                //递归的
                tItem->resize();
            }
        }else if(item->type()==LayerType){
            CustomItemLayer* tItem = dynamic_cast<CustomItemLayer*>(item);
            if(tItem){
                tItem->setzValue(pre,cur);
                tItem->resize();
            }
        }
        pre=cur;
        cur+=span;
    }
}
//画布
void GraphicDrawView::setBackGround(QColor brush, qreal width, qreal height)
{
    scene->setSceneRect(0,0,width,height);
    backColor=brush;
    QGraphicsRectItem*item=new QGraphicsRectItem(0,0,width,height);
    item->setData(this->ItemId,this->seqNum++);
    item->setData(this->ItemDesciption,"背景");
    item->setBrush(brush);
    item->setPen(QPen(Qt::NoPen));//无边框
    item->setFlag(QGraphicsRectItem::ItemIsSelectable,false);
    item->setFlag(QGraphicsRectItem::ItemIsMovable,false);
    item->setFlag(QGraphicsRectItem::ItemIsFocusable,false);
    scene->addItem(item);
    this->scene->clearSelection();
    QBrush transparentBrush(Qt::transparent);
    scene->setBackgroundBrush(transparentBrush);

    qreal newCenterX = item->sceneBoundingRect().center().x();
    qreal newCenterY = item->sceneBoundingRect().center().y();
    item->setTransformOriginPoint(newCenterX, newCenterY);
}
//处理滚动条与图形项移动
void GraphicDrawView::keyPressEvent(QKeyEvent *event)
{
    if(this->scene->selectedItems().count()>0){
        QList<QGraphicsItem*> items = this->scene->selectedItems();
        switch (event->key()) {
        // case Qt::Key_Delete:
        //     for (QGraphicsItem* item : items) {
        //         this->scene->removeItem(item);
        //         delete item;
        //     }
        //     break;
        case Qt::Key_Left:
            for (QGraphicsItem* item : items) {
                item->setX(-1+item->x());
            }
            break;
        case Qt::Key_Right:
            for (QGraphicsItem* item : items) {
                item->setX(1+item->x());
            }
            break;
        case Qt::Key_Up:
            for (QGraphicsItem* item : items) {
                item->setY(-1+item->y());
            }
            break;
        case Qt::Key_Down:
            for (QGraphicsItem* item : items) {
                item->setY(1+item->y());
            }
            break;
        case Qt::Key_Space:
            for (QGraphicsItem* item : items) {
                item->setRotation(90+item->rotation());
            }
            break;
        case Qt::Key_PageUp:
            for (QGraphicsItem* item : items) {
                item->setScale(0.1+item->scale());
            }
            break;
        case Qt::Key_PageDown:
            for (QGraphicsItem* item : items) {
                item->setScale(-0.1+item->scale());
            }
            break;
        default:
            event->ignore();
            break;
        }
        // //阻止按键事件进一步传播
        // if (handled) {
        //     event->accept();
        // }
    }else{
        QGraphicsView::keyPressEvent(event);
    }
    emit keyPress(event);
}

void GraphicDrawView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}
//重写view背景
void GraphicDrawView::drawBackground(QPainter *painter, const QRectF &rect)
{
    int width = this->geometry().width();
    int height = this->geometry().height();
    QPixmap back(width, height);
    QPainter pter(&back);
    QColor clr_white(Qt::white);
    QColor clr_gray(240, 240, 240, 240);
    int spacing = 15;
    QColor useColor;
    for (int i = 0; i <= floor(width / spacing); i++)
    {
        for (int j = 0; j <= floor(height/ spacing); j++)
        {
            useColor = ((i + j) % 2 == 0 ? clr_white : clr_gray);
            pter.fillRect(i*spacing, j*spacing, spacing, spacing, useColor);
        }
    }
    painter->drawImage(rect, back.toImage());
}
//手动滚轮缩放防bug
void GraphicDrawView::wheelEvent(QWheelEvent *event)
{
    if (this->scene->items().size() == 0){
        return;
    }
    QPointF curPoint = event->position();
    QPointF scenePos = this->mapToScene(QPoint(curPoint.x(), curPoint.y()));
    qreal viewWidth = viewport()->width();
    qreal viewHeight = viewport()->height();
    qreal hScale = curPoint.x() / viewWidth;
    qreal vScale = curPoint.y() / viewHeight;

    int wheelDeltaValue = event->angleDelta().y();
    //防止在左右旋转后失效
    //qreal scaleFactor = this->transform().m11();//缩放因子
    // if ((scaleFactor < 0.1 && wheelDeltaValue<0)||(scaleFactor>50 &&wheelDeltaValue>0)){
    //     return;
    // }
    wheelDeltaValue > 0 ? scale(1.2, 1.2) : scale(1.0/1.2, 1.0/1.2);
    // 将scene坐标转换为放大缩小后的坐标;
    QPointF viewPoint = this->transform().map(scenePos);
    this->horizontalScrollBar()->setValue(int(viewPoint.x() - viewWidth * hScale ));
    this->verticalScrollBar()->setValue(int(viewPoint.y() - viewHeight * vScale ));
    //Schedules a redraw of the area rect on the scene.

    update();

}

void GraphicDrawView::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseRelease(event);
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicDrawView::slot_Layer_droped()
{
    this->resizeLayer();
}

//双击
void GraphicDrawView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){

        emit mouseDoubleClick(event->pos());
    }

    QGraphicsView::mouseDoubleClickEvent(event);
}
//按击
void GraphicDrawView::mousePressEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton){
        emit mouseClicked(event->pos());
    }
    emit mousePress(event);

    QGraphicsView::mousePressEvent(event);

}
//移动
void GraphicDrawView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMovePoint(event->pos());
    emit mouseMove(event);
    if(!controled){
        QGraphicsView::mouseMoveEvent(event);
    }
}
