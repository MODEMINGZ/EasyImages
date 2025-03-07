#include "themainwindow.h"
#include "ui_themainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabBar>
#include <QDebug>
#include <QPoint>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QInputDialog>
#include <QListWidget>
#include "customitemlayer.h"
#include "customitemclass.h"
#include <QActionGroup>
#include <QLayoutItem>
#include "customtreewidgetlayer.h"
#include <algorithm>
#include <QVBoxLayout>
#include "cutimage.h"
#include <QQueue>
#include<QClipboard>
#include<QImage>
#include "dialogrgb.h"
#include "dialogcolorreplace.h"
#include "dialoglightness.h"
#include "dialoghsv.h"
#include "dialogwarmco.h"
#include "dialogcoldco.h"
#include "dialogbina.h"
#include "dialogcontract.h"
#include<QImageReader>
#include "dialoggetcolor.h"
#include "dialogaline.h"
template <class T>
void setBrushColor(T *item)
{
    QColor color = item->brush().color();
    color = QColorDialog::getColor(color, NULL, "选择填充颜色");
    if (color.isValid())
    {
        item->setBrush(QBrush(color));
    }
}

theMainWindow::theMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::theMainWindow)
{
    ui->setupUi(this);

    // Py_SetPythonHome(L"D:\\Anaconda3\\envs\\python38");
    // Py_Initialize();
    // if (!Py_IsInitialized())
    // {
    //     QMessageBox::information(this, "EasyImages", "Python模块初始化失败！", QMessageBox::Ok);
    // }
    this->initView();
    QImageReader::setAllocationLimit(0);
    setWindowIcon(QIcon(":/images/images/file_openfile.png"));
}

theMainWindow::~theMainWindow()
{
    // Py_Finalize();
    delete ui;
}

void theMainWindow::initView()
{
    setWindowTitle("EasyImages");

    // 初始化位置
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidget_fun);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidget_menu);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidget_layer);

    // 初始化tab
    ui->tabWidget->clear();
    ui->tabWidget->tabsClosable();
    setToolBarEnable(false);

    ui->action_copy->setEnabled(false);
    // 控制工具单选
    actionGroup = new QActionGroup(this);
    // 无法取消选择
    actionGroup->setExclusive(true);
    actionGroup->addAction(ui->action_bucket);
    actionGroup->addAction(ui->action_penBrush);
    actionGroup->addAction(ui->action_eraser);
    actionGroup->addAction(ui->action_line);
    actionGroup->addAction(ui->action_curve);
    actionGroup->addAction(ui->action_coloring);
    actionGroup->addAction(ui->action_arrow);
    actionGroup->addAction(ui->action_pen);
    actionGroup->addAction(ui->action_shape);
    actionGroup->addAction(ui->action_circle);
    actionGroup->addAction(ui->action_rectangle);
    actionGroup->addAction(ui->action_ellipse);
    actionGroup->addAction(ui->action_trapezoid);
    actionGroup->addAction(ui->action_triangle);
    actionGroup->setExclusive(true);
    lastCheckedAction = nullptr;
    connect(ui->action_circle, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_rectangle, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_ellipse, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_trapezoid, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_triangle, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_bucket, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_penBrush, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_eraser, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_line, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_curve, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_coloring, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_arrow, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_pen, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);
    connect(ui->action_shape, &QAction::triggered, this, &theMainWindow::slots_on_ActionTriggered);

    // 初始化导航树
    ui->treeWidget->clear();
    QTreeWidgetItem *item0 = new QTreeWidgetItem(ImageProType);
    item0->setText(0, "图像");
    item0->setIcon(0, QIcon(":/images/images/file_openfile.png"));
    ui->treeWidget->addTopLevelItem(item0);
    QTreeWidgetItem *item1 = new QTreeWidgetItem(ImageDrawProType);
    item1->setText(0, "新建画布");
    item1->setIcon(0, QIcon(":/images/images/graphic_draw.png"));
    ui->treeWidget->addTopLevelItem(item1);
    ui->treeWidget->expandAll();
    QTreeWidgetItem *item2 = new QTreeWidgetItem(EffectBarType);
    item2->setText(0, "效果栏");

    // 初始化状态栏
    labSceneCord = new QLabel("光标坐标：");
    labSceneCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCord);
    labItemCord = new QLabel("图元坐标：");
    labItemCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemCord);
    labItemInfo = new QLabel("图元信息：");
    labItemInfo->setMinimumWidth(200);
    ui->statusbar->addWidget(labItemInfo);
    tipInfo = new QLabel("提示:");
    tipInfo->setMinimumWidth(200);
    ui->statusbar->addWidget(tipInfo);
    tipInfo->setStyleSheet("QLabel { color : gray; }");

    // 确保初始
    ui->action_guide->setChecked(true);
    ui->action_property->setChecked(true);
    ui->action_layerWindow->setChecked(true);
    ui->action_drawWindow->setChecked(true);
    ui->action_coloredItem->setChecked(false);
    // layer
    // 隐藏箭头
    ui->stackedWidget->setStyleSheet("QStackedWidget:: QFrame {border: 0px}");

    // dockwidget合并排序
    this->tabifyDockWidget(ui->dockWidget_menu, ui->dockWidget_layer);
    this->tabifyDockWidget(ui->dockWidget_layer, ui->dockWidget_color);
    // 不透明度调整
    ui->spinBox_opacity->setRange(0, 100);
    ui->horizontalSlider_opacity->setRange(0, 100);
    // 数值联动
    connect(ui->spinBox_opacity, SIGNAL(valueChanged(int)), ui->horizontalSlider_opacity, SLOT(setValue(int)));
    connect(ui->horizontalSlider_opacity, SIGNAL(valueChanged(int)), ui->spinBox_opacity, SLOT(setValue(int)));

    // 属性框
    propertyForm = new CustomPropertyForm();
    propertyForm->setWindowFlag(Qt::SubWindow);
    ui->dockWidget_fun->setWidget(propertyForm);
    propertyForm->show();

    // 工具栏添加色块显示
    frontColor = new CustomLabelColor();
    frontColor->setToolTip("前景色");
    frontColor->setFixedSize(30, 30);
    ui->toolBar_2->addWidget(frontColor);

    backColor = new CustomLabelColor();
    backColor->setToolTip("背景色");
    backColor->setFixedSize(30, 30);
    backColor->setColor(Qt::white);
    ui->toolBar_2->addWidget(backColor);
    // 色板
    QVBoxLayout *colorLayout = new QVBoxLayout();
    colorWheel = new ColorWheel();
    colorSliderForm = new ColorSliderForm();
    ui->dockWidgetContents_3->setLayout(colorLayout);
    colorLayout->addWidget(colorWheel);
    colorLayout->addWidget(colorSliderForm);
    QHBoxLayout *brushSizeLayout = new QHBoxLayout();
    QLabel *brushSizeLabel = new QLabel("画笔大小:");
    brushSizeSpinBox = new QDoubleSpinBox();

    brushSizeLayout->addWidget(brushSizeLabel);
    brushSizeLayout->addWidget(brushSizeSpinBox);

    brushSizeSpinBox->setRange(0.1, 5000);
    brushSizeSpinBox->setValue(2);
    colorLayout->addLayout(brushSizeLayout);

    // 写出具体参数会No such signal报错
    connect(colorWheel, SIGNAL(colorChanged(const QColor &)), colorSliderForm, SLOT(onColorChanged(const QColor &)));
    connect(colorSliderForm, SIGNAL(colorChanged(const QColor &)), colorWheel, SLOT(setColor(QColor)));
    connect(frontColor, SIGNAL(colorChanged(const QColor &)), colorWheel, SLOT(setColor(QColor)));
    connect(colorWheel, SIGNAL(colorChanged(const QColor &)), frontColor, SLOT(setColor(const QColor &)));
    // 阻止循环报错
    //  connect(frontColor,SIGNAL(colorChanged(const QColor& )),colorSliderForm,SLOT(onColorChanged(const QColor&)));
    connect(colorSliderForm, SIGNAL(colorChanged(const QColor &)), frontColor, SLOT(setColor(const QColor &)));
    frontColor->setColor(qRgba(0, 0, 0, 100));
    // 窗口最大化
    // this->setWindowState(Qt::WindowMaximized);
    // 鼠标拖拽模式
    ui->action_modeDrag->setChecked(true);
    on_action_modeDrag_triggered(true);

}
// 关闭事件
void theMainWindow::closeEvent(QCloseEvent *e)
{
    int ret = QMessageBox::information(this, "EasyImages", "确认退出吗？", QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok)
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
}
// 导航窗口可见判断
void theMainWindow::on_dockWidget_menu_visibilityChanged(bool visible)
{
    if (ui->dockWidget_menu->isVisible() != ui->action_guide->isChecked())
    {
        ui->action_guide->setChecked(visible);
    }
}

// 导航窗口可见判断
void theMainWindow::on_action_guide_changed()
{
    if (ui->dockWidget_menu->isVisible() != ui->action_guide->isChecked())
    {
        ui->dockWidget_menu->setVisible(ui->action_guide->isChecked());
    }
}
// 标签页关闭
void theMainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QString str = ui->tabWidget->tabText(index);
    int ret = QMessageBox::information(this, str, "确认关闭吗？", QMessageBox::Ok | QMessageBox::Cancel);
    // Qt::MatchRecursive支持多层查找
    if (ret == QMessageBox::Ok)
    {
        QWidget *aWidget = ui->tabWidget->widget(index);
        if (aWidget)
        {
            QTreeWidgetItem *item = tabToTreeItemMap.value(aWidget);
            ui->stackedWidget->removeWidget(tabToViewMap[aWidget]->layerForm);
            tabToTreeItemMap.remove(aWidget);
            tabToViewMap.remove(aWidget);
            delete item;
            ui->tabWidget->removeTab(index);
            delete aWidget;
        }
    }
    else
    {
        return;
    }
}
// 标签页切换
void theMainWindow::on_tabWidget_currentChanged(int index)
{
    if (ui->tabWidget->count() == 0 || index == -1)
    {
        setToolBarEnable(false);
        ui->action_copy->setEnabled(false);
    }
    else
    {

        setToolBarEnable(true);
        ui->action_copy->setEnabled(true);
        Q_UNUSED(index);
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        QTreeWidgetItem *item = tabToTreeItemMap.value(ui->tabWidget->currentWidget());
        ui->treeWidget->setCurrentItem(item);
        // layer
        if (view)
        {
            ui->stackedWidget->addWidget(view->layerForm);
            ui->stackedWidget->setCurrentWidget(view->layerForm);
        }
    }
}
// 打开图像
void theMainWindow::on_action_openPicture_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "open image file",
        ".",
        "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if (fileName != "")
    {
        QImage *image = new QImage;
        if (image->load(fileName))
        {
            QFileInfo fileInfo(fileName);
            QString imageName = fileInfo.fileName();
            // this会无法适应屏幕
            GraphicDrawView *view = new GraphicDrawView();

            // QColor color(255, 255, 255, 0);
            // image->rect()
            view->scene->setSceneRect(0, 0, image->width(), image->height());
            QBrush transparentBrush(Qt::transparent);
            view->scene->setBackgroundBrush(transparentBrush);
            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
            view->scene->addItem(pixmapItem);
            pixmapItem->setData(view->ItemId, view->seqNum++);
            pixmapItem->setData(view->ItemDesciption, "背景");
            pixmapItem->setFlag(QGraphicsItem::ItemIsMovable, false);
            pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
            view->fitInView(view->scene->itemsBoundingRect(), Qt::KeepAspectRatio); // 视图比例适应

            qreal newCenterX = pixmapItem->sceneBoundingRect().center().x();
            qreal newCenterY = pixmapItem->sceneBoundingRect().center().y();
            pixmapItem->setTransformOriginPoint(newCenterX, newCenterY);

            // tab
            ui->tabWidget->addTab(view, QIcon(":/images/images/photo.png"), imageName);
            ui->tabWidget->setCurrentWidget(view);
            // tree
            QTreeWidgetItem *item = new QTreeWidgetItem(ImageType);
            item->setText(0, imageName);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            QTreeWidgetItem *parItem = ui->treeWidget->topLevelItem(0);
            parItem->addChild(item);
            tabToTreeItemMap[view] = item;
            tabToViewMap[view] = view;
            ui->treeWidget->setCurrentItem(item);
            // layer
            ui->stackedWidget->addWidget(view->layerForm);
            ui->stackedWidget->setCurrentWidget(view->layerForm);

            // 初始化子项
            CustomItemLayer *aLayer = new CustomItemLayer(GraphicDrawView::LayerType);
            aLayer->setText(0, "背景");
            aLayer->addItem(pixmapItem);
            view->layerForm->addTopLevelItem(aLayer);
            view->resizeLayer();
            view->itemToLayerMap[pixmapItem] = aLayer;
            view->layerForm->setCurrentItem(aLayer);
            aLayer->setSelected(true);
            // 更新按钮状态
            slots_set_btnV_checkState();
            slots_set_btnL_LockState();
            slots_set_opacity();

            // connect
            connect(view, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(on_mouseMovePoint(QPoint)));
            connect(view, SIGNAL(mouseClicked(QPoint)), this, SLOT(on_mouseClicked(QPoint)));
            connect(view, SIGNAL(mouseDoubleClick(QPoint)), this, SLOT(on_mouseDoubleClick(QPoint)));
            connect(view, SIGNAL(keyPress(QKeyEvent *)), this, SLOT(on_keyPress(QKeyEvent *)));
            connect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(slots_on_mouse_press(QMouseEvent *)));
            connect(view, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(slots_on_mouse_move(QMouseEvent *)));
            connect(view, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(slots_on_mouse_release(QMouseEvent *)));
            // layer
            connect(view->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slots_Item_checkState_changed(QTreeWidgetItem *, int)));
            connect(view->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_btnV_checkState()));
            connect(view->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slot_handle_layerVisibility(QTreeWidgetItem *, int)));
            connect(view->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slots_Item_LayerLock_changed(QTreeWidgetItem *, int)));
            // layerMenu
            connect(view->layerForm, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(slot_Layer_rightCliked(QTreeWidgetItem *, int)));
            connect(view->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_btnL_LockState()));
            connect(view->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_opacity()));
        }
    }
}

// 新建画布
void theMainWindow::on_action_createNew_triggered()
{
    cnt_GraphicDraw++;
    GraphicDrawView *aDraw = new GraphicDrawView(this);
    aDraw->setBackGround(Qt::white, 500, 400);
    QString name = QString("GraphicDraw %1").arg(cnt_GraphicDraw);
    ui->tabWidget->addTab(aDraw, QIcon(":/images/images/graphic_draw.png"), name);
    ui->tabWidget->setCurrentWidget(aDraw);
    aDraw->fitInView(aDraw->scene->sceneRect(), Qt::KeepAspectRatio); // 视图比例适应
    // itemsBoundingRect() 函数返回一个 QRectF 对象，表示 scene 中所有项的边界矩形。
    // aDraw->fitInView(aDraw->scene->itemsBoundingRect(), Qt::KeepAspectRatio);//视图比例适应
    // tree
    QTreeWidgetItem *item = new QTreeWidgetItem(ImageDrawType);
    item->setText(0, name);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    QTreeWidgetItem *parItem = ui->treeWidget->topLevelItem(1);
    parItem->addChild(item);
    ui->treeWidget->setCurrentItem(item);

    // tab
    tabToTreeItemMap[aDraw] = item;
    tabToViewMap[aDraw] = aDraw;
    // layer
    ui->stackedWidget->addWidget(aDraw->layerForm);
    ui->stackedWidget->setCurrentWidget(aDraw->layerForm);

    // 初始化子项
    CustomItemLayer *aLayer = new CustomItemLayer(GraphicDrawView::LayerType);
    aLayer->setText(0, "背景");
    aLayer->addItem(aDraw->scene->items().front());
    aDraw->layerForm->addTopLevelItem(aLayer);
    aDraw->resizeLayer();
    aDraw->itemToLayerMap[aDraw->scene->items().front()] = aLayer;
    aDraw->layerForm->setCurrentItem(aLayer);
    aLayer->setSelected(true);

    // 更新按钮状态
    slots_set_btnV_checkState();
    slots_set_btnL_LockState();
    slots_set_opacity();

    connect(aDraw, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(on_mouseMovePoint(QPoint)));
    connect(aDraw, SIGNAL(mouseClicked(QPoint)), this, SLOT(on_mouseClicked(QPoint)));
    connect(aDraw, SIGNAL(mouseDoubleClick(QPoint)), this, SLOT(on_mouseDoubleClick(QPoint)));
    connect(aDraw, SIGNAL(keyPress(QKeyEvent *)), this, SLOT(on_keyPress(QKeyEvent *)));
    connect(aDraw, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(slots_on_mouse_press(QMouseEvent *)));
    connect(aDraw, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(slots_on_mouse_move(QMouseEvent *)));
    connect(aDraw, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(slots_on_mouse_release(QMouseEvent *)));

    // layer
    connect(aDraw->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slots_Item_checkState_changed(QTreeWidgetItem *, int)));
    connect(aDraw->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_btnV_checkState()));
    connect(aDraw->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slot_handle_layerVisibility(QTreeWidgetItem *, int)));
    connect(aDraw->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slots_Item_LayerLock_changed(QTreeWidgetItem *, int)));
    // layerMenu
    connect(aDraw->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_btnL_LockState()));
    connect(aDraw->layerForm, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(slot_Layer_rightCliked(QTreeWidgetItem *, int)));
    connect(aDraw->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_opacity()));
}

// 选中树项
void theMainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QWidget *widget = tabToTreeItemMap.key(item);
    ui->tabWidget->setCurrentWidget(widget);
}
// 编辑树项
void theMainWindow::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    QWidget *widget = tabToTreeItemMap.key(item);
    ui->tabWidget->setCurrentWidget(widget);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), item->text(column));
}
// 鼠标移动事件
void theMainWindow::on_mouseMovePoint(QPoint point)
{
    if (bArrow)
    {
        if (pArrow)
        {
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(point);
            QPointF mpoint = pArrow->mapFromScene(pointScene);
            pArrow->setCurrentPoint(mpoint);
        }
    }
    else if (bShape)
    {
        if (pPolygonItem)
        {
            QPolygonF vec = polygonVec;
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(point);
            QPointF mpoint = pPolygonItem->mapFromScene(pointScene);
            vec << mpoint;
            pPolygonItem->setPolygon(vec);
        }
    }else if(bLine){
        if(pPath){
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(point);
            QPointF mpoint = pPath->mapFromScene(pointScene);
            QPainterPath p;
            p.moveTo(PointVec.first());
            for (int i = 1; i < PointVec.size(); ++i) {
                p.lineTo(PointVec[i]);
            }
            p.lineTo(mpoint);
            pPath->setPath(p);
        }
    }else if(bCurve){
        if(pPath){
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(point);
            QPointF mpoint = pPath->mapFromScene(pointScene);
            PointVec.push_back(mpoint);
            pPath->setPath(createBezierPath(PointVec));
            PointVec.pop_back();

        }
    }

    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QPointF pointScene = view->mapToScene(point); // 转换到Scene坐标
    QGraphicsItem *item = view->scene->focusItem();
    if (item != nullptr && item->data(view->ItemId) != 0)
    {
        // 左上角转换
        labItemCord->setText(QString::asprintf("图元坐标：(%.0f,%.0f)", item->mapToScene(0, 0).x(), item->mapToScene(0, 0).y()));
        labItemInfo->setText("图元信息：ID." + item->data(view->ItemId).toString() + "，" + item->data(view->ItemDesciption).toString());
        if (propertyForm && propertyForm->pItem)
        {
            if (item == propertyForm->pItem)
            {
                propertyForm->reSetInfo();
            }
        }
    }
    else
    {
        labItemCord->setText("图元坐标：null");
        labItemInfo->setText("图元信息：null");
    }
    labSceneCord->setText(QString::asprintf("光标坐标：(%.0f,%.0f)", pointScene.x(), pointScene.y()));
}
// 鼠标点击事件，获取属性
void theMainWindow::on_mouseClicked(QPoint point)
{
    if (bArrow)
    {
        if (checkSelectLayer())
        {
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            CustomItemLayer *layer = getCurrentLayer();
            if (!layer)
            {
                return;
            }
            if (pArrow)
            {
                QPointF pointScene = view->mapToScene(point);
                QPointF mpoint = pArrow->mapFromScene(pointScene);
                pArrow->addPoint(mpoint);
                pArrow->setCurrentPoint(mpoint);
            }
            else
            {
                CustomPathArrow *item = new CustomPathArrow();
                pArrow = item;
                QPointF pointScene = view->mapToScene(point);

                pArrow->setPos(pointScene);
                pArrow->addPoint(QPoint(0,0));
                pArrow->setCurrentPoint(QPoint(0,0));

                item->setPen(frontColor->m_color);
                QPen newPen = item->pen();
                newPen.setWidthF(this->brushSizeSpinBox->value());
                item->setPen(newPen);

                item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                item->setData(view->ItemId, view->seqNum++);
                item->setData(view->ItemDesciption, "箭头");

                view->scene->addItem(item);
                view->scene->clearSelection();
                item->setSelected(true);
                if (layer)
                {
                    layer->addItem(item);
                    layer->resize();
                    view->itemToLayerMap[item] = layer;
                }
            }
        }
    }
    else if (bShape)
    {
        if (checkSelectLayer())
        {
            if (pPolygonItem)
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                QPointF pointScene = view->mapToScene(point);
                QPointF mpoint = pPolygonItem->mapFromScene(pointScene);
                polygonVec << mpoint;
                pPolygonItem->setPolygon(polygonVec);
                return;
            }
            polygonVec.clear();
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            view->controled = true;
            CustomItemLayer *layer = getCurrentLayer();
            if (!layer)
            {
                return;
            }
            QGraphicsPolygonItem *item = new QGraphicsPolygonItem();
            pPolygonItem = item;
            QPointF pointScene = view->mapToScene(point);
            pPolygonItem->setPos(pointScene);
            polygonVec << QPointF(0, 0);
            pPolygonItem->setPolygon(polygonVec);

            item->setPen(frontColor->m_color);
            QPen newPen = item->pen();
            newPen.setWidthF(this->brushSizeSpinBox->value());
            newPen.setCapStyle(Qt::RoundCap);
            newPen.setJoinStyle(Qt::RoundJoin);
            item->setPen(newPen);
            if (coloredMode)
            {
                item->setPen(QPen(Qt::NoPen)); // 无边框
                // 随机填色
                item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
            }
            item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
            item->setData(view->ItemId, view->seqNum++);
            item->setData(view->ItemDesciption, "多边形");

            view->scene->addItem(item);
            view->scene->clearSelection();
            item->setSelected(true);
            if (layer)
            {
                layer->addItem(item);
                view->itemToLayerMap[item] = layer;
            }
        }
    }else if(bLine){
        if (checkSelectLayer())
        {
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            CustomItemLayer *layer = getCurrentLayer();
            if (!layer)
            {
                return;
            }
            if (pPath)
            {
                QPointF pointScene = view->mapToScene(point);
                QPointF mpoint = pPath->mapFromScene(pointScene);
                PointVec.push_back(mpoint);
                QPainterPath p;
                p.moveTo(PointVec.first());
                for (int i = 1; i < PointVec.size(); ++i) {
                    p.lineTo(PointVec[i]);
                }
                pPath->setPath(p);

            }
            else
            {
                QGraphicsPathItem *item = new QGraphicsPathItem();
                pPath = item;
                QPointF pointScene = view->mapToScene(point);
                view->controled = true;
                pPath->setPos(pointScene);
                PointVec.clear();
                PointVec.push_back(QPoint(0,0));

                item->setPen(frontColor->m_color);
                QPen newPen = item->pen();
                newPen.setWidthF(this->brushSizeSpinBox->value());
                item->setPen(newPen);

                item->setFlags(QGraphicsItem::ItemIsFocusable);
                item->setData(view->ItemId, view->seqNum++);
                item->setData(view->ItemDesciption, "直线");

                view->scene->addItem(item);
                view->scene->clearSelection();
                item->setSelected(true);
                if (layer)
                {
                    layer->addItem(item);
                    view->itemToLayerMap[item] = layer;
                }
            }
        }
    }else if(bCurve){
        if (checkSelectLayer())
        {
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            CustomItemLayer *layer = getCurrentLayer();
            if (!layer)
            {
                return;
            }
            if (pPath)
            {
                QPointF pointScene = view->mapToScene(point);
                QPointF mpoint = pPath->mapFromScene(pointScene);
                PointVec.push_back(mpoint);
                pPath->setPath(createBezierPath(PointVec));

            }
            else
            {
                QGraphicsPathItem *item = new QGraphicsPathItem();
                pPath = item;
                QPointF pointScene = view->mapToScene(point);
                view->controled = true;
                pPath->setPos(pointScene);
                PointVec.clear();
                PointVec.push_back(QPoint(0,0));

                item->setPen(frontColor->m_color);
                QPen newPen = item->pen();
                newPen.setWidthF(this->brushSizeSpinBox->value());
                item->setPen(newPen);

                item->setFlags(QGraphicsItem::ItemIsFocusable);
                item->setData(view->ItemId, view->seqNum++);
                item->setData(view->ItemDesciption, "贝塞尔曲线");

                view->scene->addItem(item);
                view->scene->clearSelection();
                item->setSelected(true);
                if (layer)
                {
                    layer->addItem(item);
                    view->itemToLayerMap[item] = layer;
                }
            }
        }
    }
    else
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        QPointF pointScene = view->mapToScene(point);                             // 转换到Scene坐标
        QGraphicsItem *item = view->scene->itemAt(pointScene, view->transform()); // 获取当下图形项
        this->curPos=point;
        // 图形组合区别
        if (view->itemToLayerMap.find(item) != view->itemToLayerMap.end())
        {
            this->propertyForm->setInfo(item, view);
            // setStateInfo
            if (item != nullptr)
            {
                if (item->data(GraphicDrawView::ItemId) != 0)
                {
                    this->oriTip = this->tipInfo->text();
                    this->tipInfo->setText("提示:方向键可移动图元");
                }
                labItemCord->setText(QString::asprintf("图元坐标：(%.0f,%.0f)", item->mapToScene(0, 0).x(), item->mapToScene(0, 0).y()));
                labItemInfo->setText("图元信息：ID." + item->data(view->ItemId).toString() + "，" + item->data(view->ItemDesciption).toString());
            }
            else
            {
                labItemCord->setText("图元坐标：null");
                labItemInfo->setText("图元信息：null");
            }
        }
    }
}
// 鼠标双击事件
void theMainWindow::on_mouseDoubleClick(QPoint point)
{
    if (bArrow)
    {
        if (pArrow)
        {
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(point);
            QPointF mpoint = pArrow->mapFromScene(pointScene);
            pArrow->addPoint(mpoint);
            pArrow->setFlag(QGraphicsItem::ItemIsMovable, true);
            pArrow = nullptr;
        }
    }
    else if (bShape)
    {
        if (pPolygonItem)
        {
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(point);
            QPointF mpoint = pPolygonItem->mapFromScene(pointScene);
            view->controled = false;
            polygonVec << mpoint;
            pPolygonItem->setPolygon(polygonVec);
            pPolygonItem->setFlag(QGraphicsItem::ItemIsMovable, true);
            pPolygonItem = nullptr;
            polygonVec.clear();
        }
    }
    else if (bErase)
    {
        // GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        // QPointF pointScene = view->mapToScene(point);                             // 转换到Scene坐标
        // QGraphicsItem *item = view->scene->itemAt(pointScene, view->transform()); // 获取当下图形项
        // if (item->data(GraphicDrawView::ItemId) == 0)
        // { // 背景
        //     return;
        // }
        // CustomItemLayer *layer = view->itemToLayerMap[item];
        // layer->pList.removeOne(item);
        // view->itemToLayerMap.remove(item);
        // view->scene->removeItem(item);
        // delete item;
        on_action_delete_triggered();
    }else if(bLine){
        if(pPath){
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(point);
            QPointF mpoint = pPath->mapFromScene(pointScene);
            QPainterPath p;
            p.moveTo(PointVec.first());
            for (int i = 1; i < PointVec.size(); ++i) {
                p.lineTo(PointVec[i]);
            }
            p.lineTo(mpoint);
            pPath->setPath(p);
            pPath->setFlag(QGraphicsItem::ItemIsSelectable,true);
            pPath->setFlag(QGraphicsItem::ItemIsMovable, true);
            pPath=nullptr;
            view->controled = false;
        }
    }else if(bCurve){
        if(pPath){
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            // PointVec.push_back(mpoint);
            pPath->setPath(createBezierPath(PointVec));
            PointVec.clear();

            pPath->setFlag(QGraphicsItem::ItemIsSelectable,true);
            pPath->setFlag(QGraphicsItem::ItemIsMovable, true);
            pPath=nullptr;
            view->controled = false;
        }
    }

    // else
    // {
    //     GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    //     QPointF pointScene = view->mapToScene(point);                             // 转换到Scene坐标
    //     QGraphicsItem *item = view->scene->itemAt(pointScene, view->transform()); // 获取当下图形项

    //     // if(item==nullptr){
    //     //     return;
    //     // }
    //     // switch(item->type()){

    //     // }
    // }
}
// 按键事件
void theMainWindow::on_keyPress(QKeyEvent *event)
{
    Q_UNUSED(event);
}
// 工具栏可用判断
void theMainWindow::setToolBarEnable(bool a)
{
    // 只能硬写，没有获取所有action的方法，除非降低效率
    ui->toolBar_2->setEnabled(a);
    // E
    ui->action_copy->setEnabled(a);
    ui->action_flipHorizontal->setEnabled(a);
    ui->action_anticlockwise90->setEnabled(a);
    ui->action_clockwise90->setEnabled(a);
    ui->action_rotate->setEnabled(a);
    ui->action_rotate180->setEnabled(a);
    ui->action_flipVertical->setEnabled(a);
    ui->action_diagonalFlip->setEnabled(a);
    ui->action_copyItem->setEnabled(a);
    ui->action_re_grayScale->setEnabled(a);
    // F
    ui->action_SaveTo->setEnabled(a);
    ui->action_addPhoto->setEnabled(a);
    ui->action_closeCurrentFile->setEnabled(a);
    // ui->action_createNew->setEnabled(a);
    // ui->action_exit->setEnabled(a);
    ui->action_export->setEnabled(a);
    ui->action_fastExport->setEnabled(a);
    // ui->action_openPicture->setEnabled(a);

    // J
    ui->action_Invert->setEnabled(a);
    ui->action_brightness->setEnabled(a);
    ui->action_colour->setEnabled(a);
    ui->action_grayScale->setEnabled(a);
    ui->action_colorReplace->setEnabled(a);
    ui->action_lightness->setEnabled(a);
    ui->action_bina->setEnabled(a);
    ui->action_contract->setEnabled(a);
    //T
    ui->action_gamma->setEnabled(a);
    ui->action_blur->setEnabled(a);
    ui->action_shapeImg->setEnabled(a);
    ui->action_coldCo->setEnabled(a);
    ui->action_warmCo->setEnabled(a);
    ui->action_avgK->setEnabled(a);
    // V
    ui->action__reduce->setEnabled(a);
    ui->action_100->setEnabled(a);
    ui->action_200->setEnabled(a);
    ui->action_enlarge->setEnabled(a);
    ui->action_matchHeight->setEnabled(a);
    ui->action_matchScreen->setEnabled(a);
    ui->action_matchWidth->setEnabled(a);
    // S
    ui->action_cancelSelect->setEnabled(a);
    ui->action_sAll->setEnabled(a);
    ui->action_sInvert->setEnabled(a);
    // toolBar
    ui->action_1_1->setEnabled(a);
    ui->action_addPhoto->setEnabled(a);
    ui->action_behind->setEnabled(a);
    ui->action_clear->setEnabled(a);
    ui->action_combine->setEnabled(a);
    ui->action_auxiliary_line->setEnabled(a);
    // ui->action_createNew->setEnabled(a);
    ui->action_delete->setEnabled(a);
    ui->action_front->setEnabled(a);
    ui->action_records->setEnabled(a);
    ui->action_reform->setEnabled(a);
    ui->action_scatter->setEnabled(a);
    ui->action_syncOff->setEnabled(a);
    ui->action_turnLeft->setEnabled(a);
    ui->action_turnRight->setEnabled(a);
    ui->action_undo->setEnabled(a);
    // layer
    ui->btn_addLayer->setEnabled(a);
    ui->btn_deleteLayer->setEnabled(a);
    ui->btn_layerGroup->setEnabled(a);
    ui->btn_lockLayer->setEnabled(a);
    ui->cbtn_visible->setEnabled(a);
    ui->cbtn_visible->setCheckable(a);
    ui->label_opacity->setEnabled(a);
    ui->label_per->setEnabled(a);
    ui->action_layerSee->setEnabled(a);
    ui->action_LayerNSee->setEnabled(a);
    ui->action_layerLock->setEnabled(a);
    ui->action_layerUnlock->setEnabled(a);
    ui->action_mergeLayer->setEnabled(a);
    ui->action_deleteLayer->setEnabled(a);
    ui->spinBox_opacity->setEnabled(a);
    ui->horizontalSlider_opacity->setEnabled(a);
    // propertyForm
    ui->dockWidget_fun->setEnabled(a);
    ui->dockWidget_layer->setEnabled(a);
    // Copyaction
    ui->action_copy->setEnabled(a);
    ui->action_copyX->setEnabled(a);
    ui->action_paste->setEnabled(a);
}
// 检查当前选中的图层是否规范
bool theMainWindow::checkSelectLayer()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view->layerForm->selectedItems().count() == 1 &&
        view->layerForm->selectedItems().front()->type() == GraphicDrawView::LayerType)
    {
        return true;
    }
    else
    {
        QMessageBox::information(this, "注意", "请选择单一图层", QMessageBox::Ok);
        return false;
    }
}

CustomItemLayer *theMainWindow::getCurrentLayer()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    // layer
    CustomItemLayer *layer = dynamic_cast<CustomItemLayer *>(view->layerForm->selectedItems().front());
    if (!layer)
    {
        return nullptr;
    }
    // 判断锁定
    if (!layer->icon(1).isNull())
    {
        QMessageBox::information(this, "EasyImages", "请解除锁定再操作！", QMessageBox::Ok);
        return nullptr;
    }
    return layer;
}

void theMainWindow::layerLock(QList<QTreeWidgetItem *> &items)
{
    for (auto i : items)
    {
        // 保留标记，日后或许启用
        i->setIcon(1, QIcon(":/images/images/lock.png"));
        QString currentText = i->text(0);
        if (!currentText.endsWith("     [已锁定]"))
        {
            i->setText(0, currentText + "     [已锁定]");
        }

        if (i->type() == GraphicDrawView::LayerClassType)
        {
            QList<QTreeWidgetItem *> tItems;
            int count = i->childCount();
            for (int j = 0; j < count; j++)
            {
                tItems.push_back(i->child(j));
            }
            layerLock(tItems);
        }
        else if (i->type() == GraphicDrawView::LayerType)
        {
            CustomItemLayer *layer = dynamic_cast<CustomItemLayer *>(i);
            bool f = 1;
            if (layer)
            {
                layer->isLocked = true;
                for (auto j : layer->pList)
                {
                    j->setFlag(QGraphicsItem::ItemIsMovable, false);
                    j->setFlag(QGraphicsItem::ItemIsSelectable, false);
                    if (f && propertyForm && propertyForm->pItem)
                    {
                        if (j == propertyForm->pItem)
                        {
                            propertyForm->reSetInfo();
                            f = 0;
                        }
                    }
                }
            }
        }
    }
}

void theMainWindow::layerUnlock(QList<QTreeWidgetItem *> &items)
{
    for (auto i : items)
    {
        i->setIcon(1, QIcon());
        QString currentText = i->text(0);
        if (currentText.endsWith("     [已锁定]"))
        {
            i->setText(0, currentText.replace("     [已锁定]", ""));
        }
        if (i->type() == GraphicDrawView::LayerClassType)
        {
            QList<QTreeWidgetItem *> tItems;
            int count = i->childCount();
            for (int j = 0; j < count; j++)
            {
                tItems.push_back(i->child(j));
            }
            layerUnlock(tItems);
        }
        else if (i->type() == GraphicDrawView::LayerType)
        {
            CustomItemLayer *layer = dynamic_cast<CustomItemLayer *>(i);
            if (layer)
            {
                bool f = 1;
                layer->isLocked = false;
                for (auto j : layer->pList)
                {
                    if (j->data(GraphicDrawView::ItemId) == 0)
                    {
                        j->setFlag(QGraphicsItem::ItemIsMovable, false);
                        j->setFlag(QGraphicsItem::ItemIsSelectable, false);
                    }
                    else
                    {
                        j->setFlag(QGraphicsItem::ItemIsMovable, true);
                        j->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    }
                    if (f && propertyForm && propertyForm->pItem)
                    {
                        if (j == propertyForm->pItem)
                        {
                            propertyForm->reSetInfo();
                            f = 0;
                        }
                    }
                }
            }
        }
    }
}

QGraphicsItem *theMainWindow::copyItems(QGraphicsItem *item)
{
    QPointF offset(30, 30); // 设置偏移量
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (QGraphicsTextItem *text = dynamic_cast<QGraphicsTextItem *>(item))
    {
        QGraphicsTextItem *aitem = new QGraphicsTextItem();
        aitem->setPlainText(text->toPlainText());
        aitem->setFont(text->font());
        aitem->setDefaultTextColor(text->defaultTextColor());

        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        aitem->setData(view->ItemDesciption, "文字");
        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        // aitem->setSelected(true);
        // item->setSelected(false);
        // 设置新位置，原图元位置加上偏移量
        aitem->setPos(item->pos().x() + offset.x(), item->pos().y() + offset.y());
        return aitem;
    }
    else if (QGraphicsEllipseItem *ellipse = dynamic_cast<QGraphicsEllipseItem *>(item))
    {
        QGraphicsEllipseItem *aitem = new QGraphicsEllipseItem();
        aitem->setPen(ellipse->pen());
        aitem->setBrush(ellipse->brush());
        aitem->setRect(ellipse->rect());

        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        aitem->setData(view->ItemDesciption, "椭圆");
        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        // aitem->setSelected(true);
        // item->setSelected(false);
        // 设置新位置，原图元位置加上偏移量
        aitem->setPos(item->pos().x() + offset.x(), item->pos().y() + offset.y());
        return aitem;
    }
    else if (QGraphicsRectItem *rect = dynamic_cast<QGraphicsRectItem *>(item))
    {
        QGraphicsRectItem *aitem = new QGraphicsRectItem();
        aitem->setPen(rect->pen());
        aitem->setBrush(rect->brush());
        aitem->setRect(rect->rect());

        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        aitem->setData(view->ItemDesciption, "矩形");
        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        // aitem->setSelected(true);
        // item->setSelected(false);
        // 设置新位置，原图元位置加上偏移量
        aitem->setPos(item->pos().x() + offset.x(), item->pos().y() + offset.y());
        return aitem;
    }
    else if (QGraphicsPolygonItem *polygon = dynamic_cast<QGraphicsPolygonItem *>(item))
    {
        QGraphicsPolygonItem *aitem = new QGraphicsPolygonItem();
        aitem->setPen(polygon->pen());
        aitem->setBrush(polygon->brush());
        aitem->setPolygon(polygon->polygon());

        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        aitem->setData(view->ItemDesciption, "多边形");
        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        // aitem->setSelected(true);
        // item->setSelected(false);
        // 设置新位置，原图元位置加上偏移量
        aitem->setPos(item->pos().x() + offset.x(), item->pos().y() + offset.y());
        return aitem;
    }
    else if (QGraphicsPixmapItem *pic = dynamic_cast<QGraphicsPixmapItem *>(item))
    {
        QGraphicsPixmapItem *aitem = new QGraphicsPixmapItem();
        aitem->setPixmap(pic->pixmap());

        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        aitem->setData(view->ItemDesciption, "多边形");
        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        // aitem->setSelected(true);
        // item->setSelected(false);
        // 设置新位置，原图元位置加上偏移量
        aitem->setPos(item->pos().x() + offset.x(), item->pos().y() + offset.y());
        return aitem;
    }
    else if (QGraphicsPathItem *path = dynamic_cast<QGraphicsPathItem *>(item))
    {
        QGraphicsPathItem *aitem = new QGraphicsPathItem();
        aitem->setPath(path->path());
        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        aitem->setData(view->ItemDesciption, "多边形");
        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        /*        aitem->setSelected(true);
                item->setSelected(false)*/
        ;
        // 设置新位置，原图元位置加上偏移量
        aitem->setPos(item->pos().x() + offset.x(), item->pos().y() + offset.y());
        return aitem;
    }
    else if (QGraphicsItemGroup *group = dynamic_cast<QGraphicsItemGroup *>(item))
    {
        QGraphicsItemGroup *newItemGroup = new QGraphicsItemGroup();

        // 遍历原图元组中的所有子项，并递归复制它们
        foreach (QGraphicsItem *childItem, group->childItems())
        {
            QGraphicsItem *copiedItem = copyItems(childItem); // 递归调用copyItems来复制子项
            if (copiedItem)
            {
                // 将复制的子项添加到新的图元组中
                newItemGroup->addToGroup(copiedItem);

                // 根据需要设置复制子项的属性，例如位置偏移
                copiedItem->setPos(childItem->pos() + offset);
            }
        }

        // 设置新图元组的属性
        newItemGroup->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        newItemGroup->setData(view->ItemId, view->seqNum++);
        newItemGroup->setData(view->ItemDesciption, "图元组");
        newItemGroup->setRotation(group->rotation());
        newItemGroup->setScale(group->scale());
        // newItemGroup->setSelected(true);
        // group->setSelected(false);

        // 设置新图元组的位置，原图元组位置加上偏移量
        newItemGroup->setPos(group->pos() + offset);

        // 返回新创建的图元组
        return newItemGroup;
    }
    return nullptr;
}

QPainterPath theMainWindow::createBezierPath(const QList<QPointF> &points)
{
    //贝塞尔曲线
   /* QPainterPath path;
    qreal precision=0.001;
    if(points.size()==1){
        path.moveTo(points[0]);
    }else if(points.size()==2){
        path.moveTo(points[0]);
        path.lineTo(points[1]);
    }else if(points.size()==3){
        path.moveTo(points[0]);
        path.quadTo(points[1], points[2]);
    }else if(points.size()==4){
        path.moveTo(points[0]);
        path.cubicTo(points[1],points[2],points[3]);
    }else if(points.size()>4){
        QList<QPointF> dest;
        int size=points.size();
        for(qreal t=0;t<1.000;t+=precision){
            QVector<qreal> coefficient(size, 0);
            coefficient[0] = 1.000;
            qreal u1 = 1.0 - t;

            for (int j = 1; j <= size - 1; j++) {
                qreal saved = 0.0;
                for (int k = 0; k < j; k++){
                    qreal temp = coefficient[k];
                    coefficient[k] = saved + u1 * temp;
                    saved = t * temp;
                }
                coefficient[j] = saved;
            }

            QPointF resultPoint;
            for (int i = 0; i < size; i++) {
                QPointF point = points.at(i);
                resultPoint = resultPoint + point * coefficient[i];
            }

            dest.append(resultPoint);

        }
        path.moveTo(points[0]);
        for(int i=1;i<dest.size();i++){
            path.lineTo(dest[i]);
        }

    }*/
    return generateSmoothCurve(points,false,0.5,16);

}

QPainterPath theMainWindow::generateSmoothCurve(QList<QPointF> _points, bool closed, double tension, int numberOfSegments)
{

    QList<double> points;
    foreach (QPointF p, _points) {
        points << p.x() << p.y();
    }

    QList<double> ps(points);
    QList<double> result;     // 存储生成的平滑曲线坐标
    double x, y;               // 存储计算出的x和y坐标
    double t1x, t2x, t1y, t2y; // 张力向量
    double c1, c2, c3, c4;     // 贝塞尔曲线系数
    double st;                 // 步长

    // 算法需要当前点数组的前一个点和后一个点。
    // 检查是绘制闭合曲线还是开放曲线。
    // 如果是闭合的，则将首尾点复制到数组的开始和结束位置
    // 如果是开放的，则将第一个点复制到数组的开始位置，将最后一个点复制到数组的结束位置
    if (closed)
    {
        ps.prepend(points[points.length() - 1]);
        ps.prepend(points[points.length() - 2]);
        ps.prepend(points[points.length() - 1]);
        ps.prepend(points[points.length() - 2]);
        ps.append(points[0]);
        ps.append(points[1]);
    }
    else
    {
        ps.prepend(points[1]);
        ps.prepend(points[0]);
        ps.append(points[points.length() - 2]);
        ps.append(points[points.length() - 1]);
    }

    // 1. 遍历点数组
    // 2. 遍历每两个点之间的每个分段，包括这两个点前后各一个点
    for (int i = 2; i < (ps.length() - 4); i += 2)
    {
        // 计算张力向量
        t1x = (ps[i + 2] - ps[i - 2]) * tension;
        t2x = (ps[i + 4] - ps[i]) * tension;
        t1y = (ps[i + 3] - ps[i - 1]) * tension;
        t2y = (ps[i + 5] - ps[i + 1]) * tension;

        for (int t = 0; t <= numberOfSegments; t++)
        {
            // 计算步长
            st = (double)t / (double)numberOfSegments;

            // 计算贝塞尔曲线系数
            c1 = 2 * qPow(st, 3) - 3 * qPow(st, 2) + 1;
            c2 = -2 * qPow(st, 3) + 3 * qPow(st, 2);
            c3 = qPow(st, 3) - 2 * qPow(st, 2) + st;
            c4 = qPow(st, 3) - qPow(st, 2);

            // 使用公共控制向量计算x和y坐标
            x = c1 * ps[i] + c2 * ps[i + 2] + c3 * t1x + c4 * t2x;
            y = c1 * ps[i + 1] + c2 * ps[i + 3] + c3 * t1y + c4 * t2y;

            // 将点存储在数组中
            result << x << y;
        }
    }
    // 使用的平滑曲线的坐标创建 QPainterPath
    QPainterPath path;
    path.moveTo(result[0], result[1]);
    for (int i = 2; i < result.length() - 2; i += 2) {
        path.lineTo(result[i+0], result[i+1]);
    }

    if (closed) {
        path.closeSubpath();
    }

    return path;
}

void theMainWindow::rePlaceColor()
{
    if(!(srcCo&&desCo)){
        return;
    }
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            if(qAbs(penColor.hue()-srcColor.hue())<=tol){
                pen.setColor(desColor);
            }
            QBrush brush = shape->brush();
            QColor brushColor = brush.color();
            if(qAbs(brushColor.hue()-srcColor.hue())<=tol){
                brush.setColor(desColor);
            }
            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image=pic->data(Qt::UserRole).value<QPixmap>().toImage();
            QColor oldColor;
            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    if(qAbs(oldColor.hue()-srcColor.hue())<=tol){
                        oldColor.setHsv(desColor.hue(),oldColor.saturation(),oldColor.value(),oldColor.alpha());
                        image.setPixel(x, y, oldColor.rgb());
                    }

                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}

QColor theMainWindow::HSVtoRGB(float h, float s, float v)
{
    int i;
    float f, p, q, t;
    if (s == 0) {
        // 在这种情况下，颜色将是灰色
        i = v * 255;
        return QColor(i, i, i);
    }
    h /= 60; // 色相范围 [0,6)
    i = static_cast<int>(h);
    f = h - i; // 因子和小数部分
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch (i) {
    case 0: return QColor(v * 255, t * 255, p * 255);
    case 1: return QColor(q * 255, v * 255, p * 255);
    case 2: return QColor(p * 255, v * 255, t * 255);
    case 3: return QColor(p * 255, q * 255, v * 255);
    case 4: return QColor(t * 255, p * 255, v * 255);
    default: return QColor(v * 255, p * 255, q * 255);
    }
}

// 矩形
void theMainWindow::on_action_rectangle_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        if (dragMode)
        {
            return;
        }
        QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, 100, 50);
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setPos(view->width() / 2, view->height() / 2);
        item->setData(view->ItemId, view->seqNum++);
        item->setData(view->ItemDesciption, "矩形");
        if (coloredMode)
        {
            item->setPen(QPen(Qt::NoPen)); // 无边框
            // 随机填色
            item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        }
        else
        {
            item->setPen(QPen(frontColor->m_color, 2)); // 黑色边框
            item->setBrush(Qt::NoBrush);                // 无内容
        }
        // 初始化置于画布视图中央
        QPointF center = view->mapToScene(view->viewport()->rect().center());
        item->setPos(center);
        view->scene->addItem(item);
        view->scene->clearSelection();
        item->setSelected(true);
        if (layer)
        {
            layer->addItem(item);
            view->itemToLayerMap[item] = layer;
            // item->setOpacity(layer->opacity/100);
        }
    }
}
// 椭圆
void theMainWindow::on_action_ellipse_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        if (dragMode)
        {
            return;
        }
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(0, 0, 100, 60);
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        item->setData(view->ItemId, view->seqNum++);
        item->setData(view->ItemDesciption, "椭圆");
        if (coloredMode)
        {
            item->setPen(QPen(Qt::NoPen)); // 无边框
            // 随机填色
            item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        }
        else
        {
            item->setPen(QPen(frontColor->m_color, 2)); // 黑色边框
            item->setBrush(Qt::NoBrush);                // 无内容
        }
        QPointF center = view->mapToScene(view->viewport()->rect().center());
        item->setPos(center);
        view->scene->addItem(item);
        view->scene->clearSelection();
        item->setSelected(true);
        // layer
        if (layer)
        {
            layer->addItem(item);
            view->itemToLayerMap[item] = layer;
            // item->setOpacity(layer->opacity/100);
        }
    }
}
// 屏幕适配
void theMainWindow::on_action_1_1_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view->scene->selectedItems().count() > 0)
    {
        QList<QGraphicsItem *> items = view->scene->selectedItems();
        for (QGraphicsItem *item : items)
        {
            item->setScale(1.0);
        }
    }
    else
    {
        view->scene->update();
        QRectF boundingRect = view->scene->itemsBoundingRect();
        // qDebug() << "Scene items bounding rect:" << boundingRect;
        view->fitInView(boundingRect, Qt::KeepAspectRatio);
    }
}
// 圆形
void theMainWindow::on_action_circle_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        if (dragMode)
        {
            return;
        }
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(0, 0, 80, 80);
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        // item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        item->setData(view->ItemId, view->seqNum++);
        item->setData(view->ItemDesciption, "椭圆");
        if (coloredMode)
        {
            item->setPen(QPen(Qt::NoPen)); // 无边框
            // 随机填色
            item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        }
        else
        {
            item->setPen(QPen(frontColor->m_color, 2));
            item->setBrush(Qt::NoBrush); // 无内容
        }
        QPointF center = view->mapToScene(view->viewport()->rect().center());
        item->setPos(center);
        view->scene->addItem(item);
        view->scene->clearSelection();
        item->setSelected(true);
        // layer
        if (layer)
        {
            layer->addItem(item);
            view->itemToLayerMap[item] = layer;
            // item->setOpacity(layer->opacity/100);
        }
    }
}

// 梯形
void theMainWindow::on_action_trapezoid_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        if (dragMode)
        {
            return;
        }
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem();
        QPolygonF points;
        points.append(QPointF(-40, -40));
        points.append(QPointF(40, -40));
        points.append(QPointF(100, 40));
        points.append(QPointF(-100, 40));
        item->setPolygon(points);
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setData(view->ItemId, view->seqNum++);
        item->setData(view->ItemDesciption, "梯形");
        if (coloredMode)
        {
            item->setPen(QPen(Qt::NoPen)); // 无边框
            // 随机填色
            item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        }
        else
        {
            item->setPen(QPen(frontColor->m_color, 2));
            item->setBrush(Qt::NoBrush); // 无内容
        }
        QPointF center = view->mapToScene(view->viewport()->rect().center());
        item->setPos(center);
        view->scene->addItem(item);
        view->scene->clearSelection();
        item->setSelected(true);
        // layer
        if (layer)
        {
            layer->addItem(item);
            view->itemToLayerMap[item] = layer;
            // item->setOpacity(layer->opacity/100);
        }
    }
}

// 三角形
void theMainWindow::on_action_triangle_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        if (dragMode)
        {
            return;
        }
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem();
        QPolygonF points;
        points.append(QPointF(0, -40));
        points.append(QPointF(60, 40));
        points.append(QPointF(-60, 40));
        item->setPolygon(points);
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        // item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        item->setData(view->ItemId, view->seqNum++);
        item->setData(view->ItemDesciption, "三角形");
        if (coloredMode)
        {
            item->setPen(QPen(Qt::NoPen)); // 无边框
            // 随机填色
            item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        }
        else
        {
            item->setPen(QPen(frontColor->m_color, 2));
            item->setBrush(Qt::NoBrush); // 无内容
        }
        QPointF center = view->mapToScene(view->viewport()->rect().center());
        item->setPos(center);
        view->scene->addItem(item);
        view->scene->clearSelection();
        item->setSelected(true);
        // layer
        if (layer)
        {
            layer->addItem(item);
            view->itemToLayerMap[item] = layer;
            item->setOpacity(layer->opacity / 100);
        }
    }
}

// 文字
void theMainWindow::on_action_text_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        QString str = QInputDialog::getText(this, "输入文字", "请输入文字");
        if (str.isEmpty())
        {
            return;
        }
        QGraphicsTextItem *item = new QGraphicsTextItem(str);
        QFont font = this->font();
        item->setDefaultTextColor(frontColor->m_color);
        font.setPointSize(20);
        font.setBold(true);
        item->setFont(font);
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setData(view->ItemId, view->seqNum++);
        item->setData(view->ItemDesciption, "文字");
        QPointF center = view->mapToScene(view->viewport()->rect().center());
        item->setPos(center);
        view->scene->addItem(item);
        view->scene->clearSelection();
        item->setSelected(true);
        // layer
        if (layer)
        {
            layer->addItem(item);
            view->itemToLayerMap[item] = layer;
            // item->setOpacity(layer->opacity/100);
        }
    }
}

// 删除所选图形项（跨图层）
void theMainWindow::on_action_delete_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QGraphicsItem *> selectedItems = view->scene->selectedItems();
    for (QGraphicsItem *item : selectedItems)
    {
        if (view->itemToLayerMap.find(item) != view->itemToLayerMap.end())
        {
            CustomItemLayer *layer = view->itemToLayerMap[item];
            layer->pList.removeOne(item);
            view->itemToLayerMap.remove(item);
            view->scene->removeItem(item);
            delete item;
        }
        else
        {
            QGraphicsItemGroup *group = dynamic_cast<QGraphicsItemGroup *>(item);
            if (group)
            {
                view->scene->destroyItemGroup(group);
            }
        }
        // 删除图形项
    }
}

// 清除当前图层的所有图形项
void theMainWindow::on_action_clear_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = dynamic_cast<CustomItemLayer *>(view->layerForm->selectedItems().front());
        if (layer)
        {
            int count = layer->pList.count();
            QList<QGraphicsItem *> temp;
            for (int i = 0; i < count; i++)
            {
                QGraphicsItem *item = layer->pList[i];
                if (item->data(view->ItemId) != 0)
                {
                    temp.push_back(item);
                    view->itemToLayerMap.remove(item);
                    view->scene->removeItem(item);
                    delete item;
                }
            }
            // 防止动态改变使plist遍历混乱
            for (auto i : temp)
            {
                layer->pList.removeOne(i);
            }
        }
    }
}

// 放大
void theMainWindow::on_action_enlarge_triggered()
{

    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    int cnt = view->scene->selectedItems().count();
    if (cnt > 0)
    {
        QList<QGraphicsItem *> items = view->scene->selectedItems();
        for (QGraphicsItem *item : items)
        {

            item->setScale(item->scale() * 1.1);
            if (propertyForm && propertyForm->pItem)
            {
                if (item == propertyForm->pItem)
                {
                    propertyForm->reSetInfo();
                }
            }
        }
    }
    else
    {
        view->scale(1.1, 1.1);
    }
}
// 缩小
void theMainWindow::on_action__reduce_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    int cnt = view->scene->selectedItems().count();
    if (cnt > 0)
    {
        QList<QGraphicsItem *> items = view->scene->selectedItems();
        for (QGraphicsItem *item : items)
        {
            // 减法会使图形消失
            item->setScale(item->scale() * 0.9);
            if (propertyForm && propertyForm->pItem)
            {
                if (item == propertyForm->pItem)
                {
                    propertyForm->reSetInfo();
                }
            }
        }
    }
    else
    {
        view->scale(0.9, 0.9);
    }
}

// 左转30°
void theMainWindow::on_action_turnLeft_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    int cnt = view->scene->selectedItems().count();
    if (cnt > 0)
    {
        QList<QGraphicsItem *> items = view->scene->selectedItems();
        for (QGraphicsItem *item : items)
        {
            item->setTransformOriginPoint(item->boundingRect().center());
            item->setRotation(item->rotation() - 30);
            if (propertyForm && propertyForm->pItem)
            {
                if (item == propertyForm->pItem)
                {
                    propertyForm->reSetInfo();
                }
            }
        }
    }
    else
    {
        // 还得组合过的真旋转，因为view可以渲染到保存的图像，所以用假旋转
        //  QList <QGraphicsItem*>items= view->scene->items();
        //  for (QGraphicsItem* item : items) {
        //      item->setTransformOriginPoint(item->boundingRect().center());
        //      item->setRotation(item->rotation()+30);
        //  }
        view->rotate(-30);
    }
}

// 右转30°
void theMainWindow::on_action_turnRight_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    int cnt = view->scene->selectedItems().count();
    if (cnt > 0)
    {
        QList<QGraphicsItem *> items = view->scene->selectedItems();
        for (QGraphicsItem *item : items)
        {
            item->setTransformOriginPoint(item->boundingRect().center());
            item->setRotation(item->rotation() + 30);
            if (propertyForm && propertyForm->pItem)
            {
                if (item == propertyForm->pItem)
                {
                    propertyForm->reSetInfo();
                }
            }
        }
    }
    else
    {
        view->rotate(+30);
    }
}
// 取消旋转
void theMainWindow::on_action_syncOff_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view->scene->selectedItems().count() > 0)
    {
        QList<QGraphicsItem *> items = view->scene->selectedItems();
        for (QGraphicsItem *item : items)
        {
            item->setRotation(0);
        }
    }
    else
    {
        view->resetTransform();
    }
}
// 后置
void theMainWindow::on_action_behind_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QGraphicsItem *> items = view->scene->selectedItems();
    // 优先对最后的进行后置
    std::sort(items.begin(), items.end(), [](QGraphicsItem *a, QGraphicsItem *b)
              { return a->zValue() < b->zValue(); });
    for (QGraphicsItem *item : items)
    {
        // 区别图形组合
        if (view->itemToLayerMap.find(item) != view->itemToLayerMap.end())
        {
            CustomItemLayer *layer = view->itemToLayerMap[item];
            int currentIndex = layer->pList.indexOf(item);
            // 前移
            if (currentIndex - 1 >= 0)
            {
                // 不后置过背景
                if (layer->pList[currentIndex - 1]->data(GraphicDrawView::ItemId) == 0)
                {
                    return;
                }
                qSwap(layer->pList[currentIndex], layer->pList[currentIndex - 1]);
                layer->resize();
            }
        }
        else
        {
            QMessageBox::information(this, "EasyImages", "请解散组合后再进行后置", QMessageBox::Ok);
        }
    }
    if (propertyForm && propertyForm->pItem)
    {
        propertyForm->reSetInfo();
    }
}
// 前置
void theMainWindow::on_action_front_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QGraphicsItem *> items = view->scene->selectedItems();
    // 有循环，如1 2 3 4选中2 3,2先前置到3前，3再前置到2前，所以优先对最前的进行前置
    std::sort(items.begin(), items.end(), [](QGraphicsItem *a, QGraphicsItem *b)
              { return a->zValue() > b->zValue(); });
    for (QGraphicsItem *item : items)
    {
        // 区别图形组合
        if (view->itemToLayerMap.find(item) != view->itemToLayerMap.end())
        {
            CustomItemLayer *layer = view->itemToLayerMap[item];
            // 背景不前置
            if (item->data(GraphicDrawView::ItemId) == 0)
            {
                return;
            }
            int currentIndex = layer->pList.indexOf(item);
            // 后移
            if (currentIndex + 1 <= layer->pList.size() - 1)
            {
                qSwap(layer->pList[currentIndex], layer->pList[currentIndex + 1]);
                layer->resize();
            }
        }
        else
        {
            QMessageBox::information(this, "EasyImages", "请解散组合后再进行前置", QMessageBox::Ok);
        }
    }
    if (propertyForm && propertyForm->pItem)
    {
        propertyForm->reSetInfo();
    }
}

// 组合(跨图层)
void theMainWindow::on_action_combine_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view->scene->selectedItems().count() > 1)
    {
        QGraphicsItemGroup *group = new QGraphicsItemGroup;
        QList<QGraphicsItem *> items = view->scene->selectedItems();
        qreal maxZ = 0;
        for (QGraphicsItem *item : items)
        {
            item->setSelected(false);
            item->clearFocus();
            if (item->zValue() > maxZ)
            {
                maxZ = item->zValue();
            }
            group->addToGroup(item);
        }
        group->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        group->setZValue(maxZ);
        view->scene->addItem(group);
        view->scene->clearSelection();
        group->setSelected(true);
    }
}
// 打散(跨图层)
void theMainWindow::on_action_scatter_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view->scene->selectedItems().count() == 1)
    {
        QGraphicsItemGroup *group;
        group = (QGraphicsItemGroup *)view->scene->selectedItems().at(0);
        view->scene->destroyItemGroup(group);
    }
}

// 加入图片
void theMainWindow::on_action_addPhoto_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (checkSelectLayer())
    {

        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        QString fileName = QFileDialog::getOpenFileName(
            this, "open image file",
            ".",
            "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
        if (fileName != "")
        {
            // layer
            CustomItemLayer *layer = dynamic_cast<CustomItemLayer *>(view->layerForm->selectedItems().front());

            QImage *image = new QImage;
            if (image->load(fileName))
            {
                QFileInfo fileInfo(fileName);
                QString imageName = fileInfo.fileName();
                QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
                view->scene->addItem(pixmapItem);
                pixmapItem->setData(view->ItemId, view->seqNum++);
                pixmapItem->setData(view->ItemDesciption, "图片：" + imageName);
                pixmapItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                layer->addItem(pixmapItem);
                view->itemToLayerMap[pixmapItem] = layer;
            }
        }
    }
}

// 属性栏可见判断
void theMainWindow::on_action_property_changed()
{
    if (ui->dockWidget_fun->isVisible() != ui->action_property->isChecked())
    {
        ui->dockWidget_fun->setVisible(ui->action_property->isChecked());
    }
}
// 属性栏可见判断
void theMainWindow::on_dockWidget_fun_visibilityChanged(bool visible)
{
    if (ui->dockWidget_fun->isVisible() != ui->action_property->isChecked())
    {
        ui->action_property->setChecked(visible);
    }
}
// 图层栏可见判断
void theMainWindow::on_dockWidget_layer_visibilityChanged(bool visible)
{
    if (ui->dockWidget_layer->isVisible() != ui->action_layerWindow->isChecked())
    {
        ui->action_layerWindow->setChecked(visible);
    }
}

// 图层栏可见判断
void theMainWindow::on_action_layerWindow_changed()
{
    if (ui->dockWidget_layer->isVisible() != ui->action_layerWindow->isChecked())
    {
        ui->dockWidget_layer->setVisible(ui->action_layerWindow->isChecked());
    }
}

// 颜色栏可见判断
void theMainWindow::on_action_drawWindow_changed()
{
    if (ui->dockWidget_color->isVisible() != ui->action_drawWindow->isChecked())
    {
        ui->dockWidget_color->setVisible(ui->action_drawWindow->isChecked());
    }
}

// 颜色栏可见判断
void theMainWindow::on_dockWidget_color_visibilityChanged(bool visible)
{
    if (ui->dockWidget_color->isVisible() != ui->action_drawWindow->isChecked())
    {
        ui->action_drawWindow->setChecked(visible);
    }
}

// 100%图片视图
void theMainWindow::on_action_100_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    view->setTransform(QTransform());
}

// 200%图片视图
void theMainWindow::on_action_200_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    view->setTransform(QTransform::fromScale(2, 2));
}

// 关闭当前文件
void theMainWindow::on_action_closeCurrentFile_triggered()
{
    on_tabWidget_tabCloseRequested(ui->tabWidget->currentIndex());
}

// 匹配屏幕大小
void theMainWindow::on_action_matchScreen_triggered()
{
    on_action_1_1_triggered();
}

// 宽度匹配屏幕
void theMainWindow::on_action_matchWidth_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QRectF sceneRect = view->sceneRect();
    qreal viewWidth = view->width();
    qreal scale = viewWidth / sceneRect.width();
    view->setTransform(QTransform::fromScale(scale, scale));
}

// 高度匹配屏幕
void theMainWindow::on_action_matchHeight_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QRectF sceneRect = view->sceneRect();
    qreal viewHeight = view->height();
    qreal scale = viewHeight / sceneRect.height();
    view->setTransform(QTransform::fromScale(scale, scale));
}

// 水平翻转
void theMainWindow::on_action_flipHorizontal_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    // 视图上的翻转
    QTransform transform = view->transform();
    transform.scale(-1, 1); // 水平翻转
    view->setTransform(transform);
}

// 垂直翻转
void theMainWindow::on_action_flipVertical_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QTransform transform = view->transform();
    transform.scale(1, -1);
    view->setTransform(transform);
}

// 对角翻转
void theMainWindow::on_action_diagonalFlip_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QTransform transform = view->transform();
    transform.scale(-1, -1);
    view->setTransform(transform);
}

// 增加图层组
void theMainWindow::on_btn_layerGroup_clicked()
{

    CustomItemClass *aClass = new CustomItemClass(GraphicDrawView::LayerClassType);
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QString name = QString("组 %1").arg(view->classNum++);
    aClass->setText(0, name);
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();

    if (!items.empty())
    {
        if (items.count() > 1)
        {
            // 判断是否为子项
            auto isChildOf = [](QTreeWidgetItem *child, QList<QTreeWidgetItem *> items) -> bool
            {
                while ((child = child->parent()) != nullptr)
                {
                    if (items.contains(child))
                    {
                        return true;
                    }
                }
                return false;
            };
            // 筛选出外层的选中项
            QList<QTreeWidgetItem *> topItems;
            for (QTreeWidgetItem *item : items)
            {
                if (!isChildOf(item, items))
                {
                    topItems.append(item);
                }
            }
            // aClass插入位置
            QTreeWidgetItem *parItem = topItems.isEmpty() ? nullptr : topItems.first()->parent();
            int insertIndex;
            if (parItem)
            {
                insertIndex = parItem->indexOfChild(topItems.first());
            }
            else
            {
                insertIndex = view->layerForm->indexOfTopLevelItem(topItems.first());
            }
            // 去掉父联系再加
            for (QTreeWidgetItem *item : topItems)
            {
                if (item->parent())
                {
                    item->parent()->removeChild(item);
                }
                else
                {
                    view->layerForm->takeTopLevelItem(view->layerForm->indexOfTopLevelItem(item));
                }
                aClass->addChild(item);
            }

            if (parItem)
            {
                parItem->insertChild(insertIndex, aClass);
            }
            else
            {
                view->layerForm->insertTopLevelItem(insertIndex, aClass);
            }
            // 更新勾选
            slots_Item_checkState_changed(aClass->child(0), 0);
        }
        else
        {
            if (items.front()->type() == GraphicDrawView::LayerClassType)
            {
                items.front()->insertChild(0, aClass);
            }
            else
            {
                QTreeWidgetItem *parent = items.front()->parent();
                if (parent)
                {
                    int index = parent->indexOfChild(items.front());
                    parent->insertChild(index, aClass);
                }
                else
                {
                    view->layerForm->insertTopLevelItem(0, aClass);
                }
            }
            slots_Item_checkState_changed(aClass, 0);
        }
    }
    else
    {
        // 默认向前添加
        view->layerForm->insertTopLevelItem(0, aClass);
    }
    view->resizeLayer();
    view->layerForm->update();
}

// 增加图层
void theMainWindow::on_btn_addLayer_clicked()
{
    CustomItemLayer *aItem = new CustomItemLayer(GraphicDrawView::LayerType);
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QString name = QString("图层 %1").arg(view->LayerNum++);
    aItem->setText(0, name);
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();

    if (items.isEmpty())
    {
        view->layerForm->insertTopLevelItem(0, aItem);
    }
    else
    {
        if (items.front()->type() == GraphicDrawView::LayerClassType)
        {
            // 判断锁定
            if (!items.front()->icon(1).isNull())
            {
                QMessageBox::information(this, "EasyImages", "请解除锁定再操作！", QMessageBox::Ok);
                return;
            }
            // 默认向前添加
            items.front()->insertChild(0, aItem);
        }
        else
        {
            QTreeWidgetItem *parent = items.front()->parent();
            if (parent)
            {
                // 判断锁定
                if (!parent->icon(1).isNull())
                {
                    QMessageBox::information(this, "EasyImages", "请解除锁定再操作！", QMessageBox::Ok);
                    return;
                }
                int index = parent->indexOfChild(items.front());
                parent->insertChild(index, aItem);
            }
            else
            {
                int index = view->layerForm->indexOfTopLevelItem(items.front());
                view->layerForm->insertTopLevelItem(index, aItem);
            }
        }
    }
    // 更新勾选状态
    slots_Item_checkState_changed(aItem, 0);
    view->resizeLayer();
}

// 删除图层/组
void theMainWindow::on_btn_deleteLayer_clicked()
{
    int ret = QMessageBox::information(this, "EasyImages", "确定删除选中图层吗？", QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel)
    {
        return;
    }
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();

    for (auto item : items)
    {
        // 注意有子项组多层嵌套,只选中了组的话，而没有选择组内的项就只删除组，组内的项提升到与原组同级
        if (item->type() == GraphicDrawView::LayerType)
        {
            CustomItemLayer *layer = dynamic_cast<CustomItemLayer *>(item);
            if (layer)
            {
                QList<QGraphicsItem *> forRemove;
                for (auto i : layer->pList)
                {
                    view->itemToLayerMap.remove(i);
                    forRemove.push_back(i);
                    view->scene->removeItem(i);
                    delete i;
                }
                for (auto i : forRemove)
                {
                    layer->pList.removeOne(i);
                }
            }
            delete item;
        }
        else if (item->type() == GraphicDrawView::LayerClassType)
        {
            QTreeWidgetItem *parent = item->parent();
            if (parent)
            {
                while (item->childCount() > 0)
                {
                    QTreeWidgetItem *child = item->child(0);
                    item->removeChild(child);
                    parent->insertChild(0, child);
                }
            }
            else
            {
                while (item->childCount() > 0)
                {
                    QTreeWidgetItem *child = item->child(0);
                    item->removeChild(child);
                    view->layerForm->addTopLevelItem(child);
                }
            }
            delete item;
        }
    }
    view->resizeLayer();
}

// 锁定图层/组
void theMainWindow::on_btn_lockLayer_clicked()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();
}

// checkBox改变图层可见性
void theMainWindow::on_cbtn_visible_clicked(bool checked)
{
    // 取消点击时checkbox的三态
    if (ui->cbtn_visible->checkState() == Qt::PartiallyChecked)
    {
        ui->cbtn_visible->setCheckState(Qt::Checked);
    }
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();
    if (items.empty())
    {
        ui->cbtn_visible->setCheckState(Qt::Unchecked);
        return;
    }
    for (auto item : items)
    {
        item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
    }
}
// 更新按钮状态
void theMainWindow::slots_set_btnV_checkState()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();
    bool f = 1;
    if (items.count() != 0)
    {
        for (auto item : items)
        {
            if (item->checkState(0) != items.front()->checkState(0))
            {
                f = 0;
                break;
            }
        }
        if (f)
        {
            ui->cbtn_visible->setCheckState(items.front()->checkState(0));
        }
        else
        {
            ui->cbtn_visible->setCheckState(Qt::Unchecked);
        }
    }
    else
    {
        ui->cbtn_visible->setCheckState(Qt::Unchecked);
    }
}
// 检查子项图层项选中状态改变，层级联动
void theMainWindow::slots_Item_checkState_changed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    slots_set_btnV_checkState();
    Qt::CheckState state = item->checkState(0);
    if (item->type() == GraphicDrawView::LayerClassType)
    {
        // 验证父项
        if (item->parent())
        {
            int parChildCount = item->parent()->childCount();
            int cnt = 0;
            for (int i = 0; i < parChildCount; i++)
            {
                QTreeWidgetItem *child = item->parent()->child(i);
                if (child->checkState(0) == state)
                {
                    cnt++;
                }
            }
            // 验证性，但多层嵌套，需要递归触发
            // disconnect(ui->stackedWidget->currentWidget(), SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slots_Item_checkState_changed(QTreeWidgetItem*, int)));
            if (cnt == parChildCount)
            {
                item->parent()->setCheckState(0, state);
            }
            else
            {
                item->parent()->setCheckState(0, Qt::PartiallyChecked);
            }
        }
        // 更新子项
        if (state != Qt::PartiallyChecked)
        {
            int childCount = item->childCount();
            for (int i = 0; i < childCount; i++)
            {
                QTreeWidgetItem *child = item->child(i);
                child->setCheckState(0, state);
            }
        }
    }
    else if (item->type() == GraphicDrawView::LayerType)
    {
        // 验证父项
        if (item->parent())
        {
            int parChildCount = item->parent()->childCount();
            int cnt = 0;
            for (int i = 0; i < parChildCount; i++)
            {
                QTreeWidgetItem *child = item->parent()->child(i);
                if (child->checkState(0) == state)
                {
                    cnt++;
                }
            }
            if (cnt == parChildCount)
            {
                item->parent()->setCheckState(0, state);
            }
            else
            {
                item->parent()->setCheckState(0, Qt::PartiallyChecked);
            }
        }
    }
}
// 是否基础图形项填色
void theMainWindow::on_action_coloredItem_triggered(bool checked)
{
    this->coloredMode = checked;
}
// 图层可见性改变
void theMainWindow::slot_handle_layerVisibility(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (item->type() == GraphicDrawView::LayerType)
    {
        CustomItemLayer *tItem = dynamic_cast<CustomItemLayer *>(item);
        if (tItem)
        {
            for (auto i : tItem->pList)
            {
                if (item->checkState(0) == Qt::Checked)
                {
                    i->setVisible(true);
                }
                else
                {
                    i->setVisible(false);
                }
            }
        }
    }
}
// 为什么不写在GraphicDrawView里，因为qApp的获取与action
void theMainWindow::slot_Layer_rightCliked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (qApp->mouseButtons() == Qt::RightButton)
    {
        QTreeWidget *tree = item->treeWidget();
        QMenu *menu = new QMenu(tree);
        menu->addAction(ui->action_layerSee);
        menu->addAction(ui->action_LayerNSee);
        menu->addAction(ui->action_layerLock);
        menu->addAction(ui->action_layerUnlock);
        menu->addAction(ui->action_mergeLayer);
        menu->addAction(ui->action_deleteLayer);

        // 显示在鼠标点击位置
        menu->exec(QCursor::pos());
    }
}

// 删除图层选中项
void theMainWindow::on_action_deleteLayer_triggered()
{
    on_btn_deleteLayer_clicked();
}

// 按钮锁定图层/组
void theMainWindow::on_btn_lockLayer_clicked(bool checked)
{
    if (checked)
    {
        on_action_layerLock_triggered();
    }
    else
    {
        on_action_layerUnlock_triggered();
    }
}

// 锁定图层
void theMainWindow::on_action_layerLock_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    CustomTreeWidgetLayer *widget = view->layerForm;
    QList<QTreeWidgetItem *> items = widget->selectedItems();
    layerLock(items);
}

// 解锁图层
void theMainWindow::on_action_layerUnlock_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    CustomTreeWidgetLayer *widget = view->layerForm;
    QList<QTreeWidgetItem *> items = widget->selectedItems();
    layerUnlock(items);
}

// 图层可见
void theMainWindow::on_action_layerSee_triggered()
{
    ui->cbtn_visible->setCheckState(Qt::Checked);
    on_cbtn_visible_clicked(true);
}

// 图层不可见
void theMainWindow::on_action_LayerNSee_triggered()
{
    ui->cbtn_visible->setCheckState(Qt::Unchecked);
    on_cbtn_visible_clicked(false);
}
// 检查子项图层锁状态改变,层级联动
void theMainWindow::slots_Item_LayerLock_changed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    bool state = (item->icon(1).isNull());
    if (item->parent())
    {
        int cnt = 0;
        int parCount = item->parent()->childCount();
        for (int i = 0; i < parCount; i++)
        {
            QTreeWidgetItem *child = item->parent()->child(i);
            if (state == child->icon(1).isNull())
            {
                cnt++;
            }
        }
        QString currentText = item->parent()->text(0);
        if (cnt == parCount)
        {
            if (state)
            {
                if (currentText.endsWith("     [已锁定]"))
                {
                    item->parent()->setText(0, currentText.replace("     [已锁定]", ""));
                }
                item->parent()->setIcon(1, QIcon());
            }
            else
            {
                if (!currentText.endsWith("     [已锁定]"))
                {
                    item->parent()->setText(0, currentText + "     [已锁定]");
                }
                item->parent()->setIcon(1, QIcon(":/images/images/lock.png"));
            }
        }
        else
        {
            if (currentText.endsWith("     [已锁定]"))
            {
                item->parent()->setText(0, currentText.replace("     [已锁定]", ""));
            }
            item->parent()->setIcon(1, QIcon());
        }
    }
}
// 设置锁定按钮状态
void theMainWindow::slots_set_btnL_LockState()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();
    if (items.count() != 0)
    {
        if (items.front()->icon(1).isNull())
        {
            ui->btn_lockLayer->setChecked(false);
        }
        else
        {
            ui->btn_lockLayer->setChecked(true);
        }
    }
    else
    {
        ui->btn_lockLayer->setChecked(false);
    }
}

// 不透明度数字槽
void theMainWindow::on_spinBox_opacity_valueChanged(int arg1)
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();
    if (!items.isEmpty())
    {
        for (auto item : items)
        {
            if (item->type() == GraphicDrawView::LayerType)
            {
                CustomItemLayer *tItem = dynamic_cast<CustomItemLayer *>(item);
                if (tItem)
                {
                    tItem->setOpacity((qreal)arg1);
                }
            }
            else if (item->type() == GraphicDrawView::LayerClassType)
            {
                CustomItemClass *tItem = dynamic_cast<CustomItemClass *>(item);
                if (tItem)
                {
                    tItem->setOpacity((qreal)arg1);
                }
            }
        }
    }
}
// 根据项变动更新不透明度状态
void theMainWindow::slots_set_opacity()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();
    if (!items.isEmpty())
    {
        QTreeWidgetItem *item = items.front();
        if (item->type() == GraphicDrawView::LayerType)
        {
            CustomItemLayer *tItem = dynamic_cast<CustomItemLayer *>(item);
            if (tItem)
            {
                ui->spinBox_opacity->setValue((int)tItem->opacity);
            }
        }
        else if (item->type() == GraphicDrawView::LayerClassType)
        {
            CustomItemClass *tItem = dynamic_cast<CustomItemClass *>(item);
            if (tItem)
            {
                ui->spinBox_opacity->setValue((int)tItem->opacity);
            }
        }
    }
}

// 导出为png
void theMainWindow::on_action_fastExport_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QGraphicsScene *scene = view->scene;
    qreal width = scene->itemsBoundingRect().width();
    qreal height = scene->itemsBoundingRect().height();
    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    // 绘制渲染，调整渲染的目标矩形以匹配所有项的边界
    scene->render(&painter, QRectF(0, 0, width, height), scene->itemsBoundingRect());
    painter.end(); // 结束绘制

    // 根据view的变换状态来翻转图片
    QTransform viewTransform = view->transform();
    if (viewTransform.isScaling()) {
        // 水平翻转检查
        if (viewTransform.m11() < 0) {
            image = image.mirrored(true, false); // 水平翻转
        }
        // 垂直翻转检查
        if (viewTransform.m22() < 0) {
            image = image.mirrored(false, true); // 垂直翻转
        }
    }
    // 保存
    QString name = ui->treeWidget->currentItem()->text(0);
    // 显示文件保存对话框，允许用户选择文件类型和名称
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("保存文件"),
                                                    QFileInfo(name).baseName(), // 默认的文件名留空
                                                    tr("PNG (*.png);;JPG (*.jpg);;GIF (*.gif);;BMP (*.bmp);;JPEG (*.jpeg);;")); // 过滤器包含多种格式

    if (!fileName.isEmpty())
    {
        // 提取文件后缀
        QString suffix = QFileInfo(fileName).suffix().toLower();

        // 根据后缀保存图片
        if (suffix == "png")
        {
            image.save(fileName);
        }
        else if (suffix == "jpg" || suffix == "jpeg")
        {
            image.save(fileName, "JPG"); // 明确指定格式
        }
        else if (suffix == "gif")
        {
            image.save(fileName, "GIF");
        }else if(suffix=="bmp"){
             image.save(fileName, "BMP");
        }
        else
        {
            QMessageBox::warning(this, "EasyImages", "不支持的图片格式！", QMessageBox::Ok);
            return;
        }
    }
}
void theMainWindow::slots_on_ActionTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        if (action == lastCheckedAction)
        {
            // 如果这个动作已经被选中，则取消选择并清除记录
            action->setChecked(false);
            lastCheckedAction = nullptr;
        }
        else
        {
            // 否则，记录这个动作为最后一次选中的动作
            lastCheckedAction = action;
        }
    }
}

void theMainWindow::on_action_arrow_changed()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (ui->action_arrow->isChecked())
    {
        if (tipInfo)
        {
            view->controled = true;
            tipInfo->setText("提示:双击完成操作;右键撤回");
            bArrow = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            ui->toolBar->setEnabled(true);
            view->controled = false;
            if (pArrow)
            {
                pArrow->clearCurrentPoint();
                pArrow->setFlag(QGraphicsItem::ItemIsMovable, true);
                pArrow->setFlag(QGraphicsItem::ItemIsSelectable, true);
                pArrow->setFlag(QGraphicsItem::ItemIsFocusable, true);
            }
            bArrow = false;
            pArrow = nullptr;
        }
    }
}

void theMainWindow::slots_on_mouse_press(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        lastPos = e->pos();
        startPoint = e->pos();
        if (bPen)
        {
            if (checkSelectLayer())
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                CustomItemLayer *layer = getCurrentLayer();
                if (!layer)
                {
                    return;
                }
                QGraphicsPathItem *item = new QGraphicsPathItem();
                pPen = item;
                QPointF pointScene = view->mapToScene(e->pos());
                // pPen->setTransformOriginPoint(pointScene);
                pPen->setPos(pointScene);

                item->setPen(frontColor->m_color);
                QPen newPen = item->pen();
                newPen.setWidthF(this->brushSizeSpinBox->value());
                newPen.setCapStyle(Qt::RoundCap);
                newPen.setJoinStyle(Qt::RoundJoin);
                item->setPen(newPen);

                item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                item->setData(view->ItemId, view->seqNum++);
                item->setData(view->ItemDesciption, "画笔");

                view->scene->addItem(item);
                view->scene->clearSelection();
                item->setSelected(true);
                if (layer)
                {
                    layer->addItem(item);
                    view->itemToLayerMap[item] = layer;
                }
            }
        } /*else if(bDraw){
             if(checkSelectLayer()){
                 GraphicDrawView*view=tabToViewMap[ui->tabWidget->currentWidget()];
                 CustomItemLayer*layer=getCurrentLayer();
                 if(!layer){
                     return;
                 }
             }
         }*/
        else if (bRectangle)
        {
            if (checkSelectLayer())
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                view->controled = true;
                CustomItemLayer *layer = getCurrentLayer();
                if (!layer)
                {
                    return;
                }
                QGraphicsRectItem *item = new QGraphicsRectItem();
                pRectItem = item;
                QPointF pointScene = view->mapToScene(e->pos());
                pRectItem->setPos(pointScene);

                item->setPen(frontColor->m_color);
                QPen newPen = item->pen();
                newPen.setWidthF(this->brushSizeSpinBox->value());
                newPen.setCapStyle(Qt::RoundCap);
                newPen.setJoinStyle(Qt::RoundJoin);
                item->setPen(newPen);
                if (coloredMode)
                {
                    item->setPen(QPen(Qt::NoPen)); // 无边框
                    // 随机填色
                    item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
                }
                item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                item->setData(view->ItemId, view->seqNum++);
                item->setData(view->ItemDesciption, "矩形");

                view->scene->addItem(item);
                view->scene->clearSelection();
                item->setSelected(true);
                if (layer)
                {
                    layer->addItem(item);
                    view->itemToLayerMap[item] = layer;
                }
            }
        }
        else if (bCircle || bEllipse)
        {
            if (checkSelectLayer())
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                view->controled = true;
                CustomItemLayer *layer = getCurrentLayer();
                if (!layer)
                {
                    return;
                }
                QGraphicsEllipseItem *item = new QGraphicsEllipseItem();
                pEllipseItem = item;
                QPointF pointScene = view->mapToScene(e->pos());
                pEllipseItem->setPos(pointScene);

                item->setPen(frontColor->m_color);
                QPen newPen = item->pen();
                newPen.setWidthF(this->brushSizeSpinBox->value());
                newPen.setCapStyle(Qt::RoundCap);
                newPen.setJoinStyle(Qt::RoundJoin);
                item->setPen(newPen);
                if (coloredMode)
                {
                    item->setPen(QPen(Qt::NoPen)); // 无边框
                    // 随机填色
                    item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
                }
                item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                item->setData(view->ItemId, view->seqNum++);
                if (bCircle)
                    item->setData(view->ItemDesciption, "圆形");
                else if (bEllipse)
                    item->setData(view->ItemDesciption, "椭圆");

                view->scene->addItem(item);
                view->scene->clearSelection();
                item->setSelected(true);
                if (layer)
                {
                    layer->addItem(item);
                    view->itemToLayerMap[item] = layer;
                }
            }
        }
        else if (bTriangle || bTrapezoid)
        {
            if (checkSelectLayer())
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                view->controled = true;
                CustomItemLayer *layer = getCurrentLayer();
                if (!layer)
                {
                    return;
                }
                QGraphicsPolygonItem *item = new QGraphicsPolygonItem();
                pPolygonItem = item;
                QPointF pointScene = view->mapToScene(e->pos());
                pPolygonItem->setPos(pointScene);

                item->setPen(frontColor->m_color);
                QPen newPen = item->pen();
                newPen.setWidthF(this->brushSizeSpinBox->value());
                newPen.setCapStyle(Qt::RoundCap);
                newPen.setJoinStyle(Qt::RoundJoin);
                item->setPen(newPen);
                if (coloredMode)
                {
                    item->setPen(QPen(Qt::NoPen)); // 无边框
                    // 随机填色
                    item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
                }
                item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                item->setData(view->ItemId, view->seqNum++);

                if (bTriangle)
                    item->setData(view->ItemDesciption, "三角形");
                else if (bTrapezoid)
                    item->setData(view->ItemDesciption, "梯形");

                view->scene->addItem(item);
                view->scene->clearSelection();
                item->setSelected(true);
                if (layer)
                {
                    layer->addItem(item);
                    view->itemToLayerMap[item] = layer;
                }
            }
        }else if(bBucket){
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            QPointF pointScene = view->mapToScene(e->pos());                             // 转换到Scene坐标
            QGraphicsItem *item = view->scene->itemAt(pointScene, view->transform()); // 获取当下图形项
            if(QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
                shape->setBrush(backColor->m_color);
                if (shape->brush().style() == Qt::NoBrush)
                {
                    QBrush brush=shape->brush();
                    brush.setStyle(Qt::SolidPattern);
                    shape->setBrush(brush);
                }
            }
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        if (bArrow)
        {
            if (pArrow)
            {
                pArrow->deletePoint();
            }
        }
        else if (bShape)
        {
            if (pPolygonItem)
            {
                if (polygonVec.isEmpty())
                {
                    return;
                }
                polygonVec.pop_back();
                pPolygonItem->setPolygon(polygonVec);
            }
        }else if(bLine){
            if(pPath){
                if(PointVec.empty()){
                    return;
                }else{
                    PointVec.pop_back();
                    if(PointVec.empty()){
                        return;
                    }
                }
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                QPointF pointScene = view->mapToScene(e->pos());
                QPointF mpoint = pPath->mapFromScene(pointScene);
                QPainterPath p;
                p.moveTo(PointVec.first());
                for (int i = 1; i < PointVec.size(); ++i) {
                    p.lineTo(PointVec[i]);
                }
                p.lineTo(mpoint);
                pPath->setPath(p);
            }
        }else if(bCurve){
            if(pPath){
                if(PointVec.empty()){
                    return;
                }else{
                    PointVec.pop_back();
                }
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                QPointF pointScene = view->mapToScene(e->pos());
                QPointF mpoint = pPath->mapFromScene(pointScene);
                PointVec.push_back(mpoint);
                pPath->setPath(createBezierPath(PointVec));
                PointVec.pop_back();
            }
        }
    }
}

void theMainWindow::on_action_clip_triggered()
{
    if (ui->action_fastExport->isEnabled())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        QGraphicsScene *scene = view->scene;
        qreal width = scene->itemsBoundingRect().width();
        qreal height = scene->itemsBoundingRect().height();
        QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        QPainter painter(&image);
        scene->render(&painter, QRectF(0, 0, width, height), scene->itemsBoundingRect());

        QString tempDir = "./temp/";
        QDir dir;
        if (!dir.exists(tempDir))
        {
            dir.mkpath(tempDir);
        }

        image.save(QString("%1%2.png").arg(tempDir).arg(++saveImgIndex));
        CutImage imgDia(QString("%1%2.png").arg(tempDir).arg(saveImgIndex));
        if (imgDia.exec() == QDialog::Accepted)
        {
            // 阻塞销毁
        }
        QFile::remove(QString("%1%2.png").arg(tempDir).arg(saveImgIndex));
    }
    else
    {
        QString file = QFileDialog::getOpenFileName(
            this, "open image file",
            ".",
            "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
        if (file.isEmpty())
        {
            return;
        }
        QFileInfo fileinfo(file);
        CutImage imgDia(file);
        if (imgDia.exec() == QDialog::Accepted)
        {
            if (imgDia.exec() == QDialog::Accepted)
            {
                // 阻塞销毁
            }
        }
    }
}

void theMainWindow::slots_on_mouse_move(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (bPen)
        {
            if (pPen)
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                // QPointF delta = view->mapToScene(event->pos())-view->mapToScene(lastPos);
                QPainterPath path;

                // QPointF controlPoint1 = (view->mapToScene(lastPos) + view->mapToScene(event->pos())) / 2;
                // QPointF controlPoint2 = controlPoint1;
                // path.cubicTo(controlPoint1, controlPoint2, view->mapToScene(event->pos()));

                QPointF ppoint = pPen->mapFromScene(view->mapToScene(lastPos));
                path.moveTo(ppoint);
                QPointF point = pPen->mapFromScene(view->mapToScene(event->pos()));
                path.lineTo(point);
                QPainterPath penPath = pPen->path();
                penPath.addPath(path);
                pPen->setPath(penPath);
                lastPos = event->pos();
            }
        }
        else if (bRectangle)
        {
            if (pRectItem)
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                QPointF ppointScene = view->mapToScene(startPoint);
                QPointF pointScene = view->mapToScene(event->pos());

                // 确定矩形的左上角点 (topLeft)
                QPointF topLeft = QPointF(qMin(ppointScene.x(), pointScene.x()), qMin(ppointScene.y(), pointScene.y()));

                // 计算矩形的宽度 (width) 和高度 (height)
                qreal width = qAbs(ppointScene.x() - pointScene.x());
                qreal height = qAbs(ppointScene.y() - pointScene.y());

                // 如果按住Ctrl键，则绘制正方形
                if (event->modifiers() & Qt::ControlModifier)
                {
                    qreal sideLength = qMin(width, height);
                    width = sideLength;
                    height = sideLength;
                }

                // 设置 pRectItem 的位置和大小
                pRectItem->setPos(topLeft);              // 设置矩形的左上角位置
                pRectItem->setRect(0, 0, width, height); // 设置矩形的宽度和高度
            }
        }
        else if (bCircle || bEllipse)
        {
            if (pEllipseItem)
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                QPointF ppointScene = view->mapToScene(startPoint);
                QPointF pointScene = view->mapToScene(event->pos());

                // 确定矩形的左上角点 (topLeft)
                QPointF topLeft = QPointF(qMin(ppointScene.x(), pointScene.x()), qMin(ppointScene.y(), pointScene.y()));

                // 计算矩形的宽度 (width) 和高度 (height)
                qreal width = qAbs(ppointScene.x() - pointScene.x());
                qreal height = qAbs(ppointScene.y() - pointScene.y());

                // 设置 pRectItem 的位置和大小
                pEllipseItem->setPos(topLeft); // 设置矩形的左上角位置
                if (bCircle)
                {
                    qreal radius = qMin(width, height) / 2;
                    pEllipseItem->setRect(0, 0, radius * 2, radius * 2);
                }
                else if (bEllipse)
                {
                    pEllipseItem->setRect(0, 0, width, height);
                }
            }
        }
        else if (bTriangle)
        {
            if (pPolygonItem)
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                QPointF ppointScene = view->mapToScene(startPoint);
                QPointF pointScene = view->mapToScene(event->pos());

                qreal width = qAbs(ppointScene.x() - pointScene.x());
                qreal height = qAbs(ppointScene.y() - pointScene.y());
                qreal halfWidth = width / 2.0;

                QPointF A, B, C;

                // 判断绘制方向，调整顶点位置以支持正立和倒立的等腰三角形
                if (ppointScene.y() < pointScene.y())
                {
                    // 正立等腰三角形
                    A = QPointF(halfWidth, 0);
                    B = QPointF(0, height);
                    C = QPointF(width, height);
                }
                else
                {
                    // 倒立等腰三角形
                    A = QPointF(halfWidth, height);
                    B = QPointF(0, 0);
                    C = QPointF(width, 0);
                }
                pPolygonItem->setPolygon(QPolygonF() << A << B << C);
            }
        }else if(bTrapezoid){
            if (pPolygonItem)
            {
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                QPointF ppointScene = view->mapToScene(startPoint);
                QPointF pointScene = view->mapToScene(event->pos());

                qreal width = qAbs(ppointScene.x() - pointScene.x());
                qreal height = qAbs(ppointScene.y() - pointScene.y());
                qreal halfWidth = width / 4.0;

                QPointF A, B, C,D;

                // 判断绘制方向，调整顶点位置以支持正立和倒立的等腰三角形
                if (ppointScene.y() < pointScene.y())
                {
                    // 正立等腰三角形
                    A = QPointF(halfWidth, 0);
                    B = QPointF(3*halfWidth, 0);
                    C = QPointF(width, height);
                    D=QPointF(0,height);
                }
                else
                {
                    // 倒立等腰三角形
                    A = QPointF(0,0);
                    B = QPointF(width, 0);
                    C = QPointF(3*halfWidth, height);
                    D=QPoint(halfWidth,height);
                }
                pPolygonItem->setPolygon(QPolygonF() << A << B << C<<D);
            }
        }
    }
    update();
}

void theMainWindow::on_action_penBrush_changed()
{
    if (ui->action_penBrush->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:不透明度状态下点击画笔痕迹即可消除噪点");
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            view->controled = true;
            bPen = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            view->controled = false;
            bPen = false;
            pPen = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::slots_on_mouse_release(QMouseEvent *event)
{
    if (!this->oriTip.isEmpty())
    {
        this->tipInfo->setText(this->oriTip);
    }
    if (event->button() == Qt::LeftButton)
    {
        if (bPen)
        {
            if (pPen)
            {
                pPen->setFlag(QGraphicsItem::ItemIsMovable, true);
                pPen = nullptr;
            }
        }
        else if (bRectangle)
        {
            if (pRectItem)
            {
                pRectItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                pRectItem = nullptr;
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                view->controled = false;
            }
        }
        else if (bCircle || bEllipse)
        {
            if (pEllipseItem)
            {
                pEllipseItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                pEllipseItem = nullptr;
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                view->controled = false;
            }
        }
        else if (bTriangle)
        {
            if (pPolygonItem)
            {
                pPolygonItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                pPolygonItem = nullptr;
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                view->controled = false;
            }
        }
        else if (bTrapezoid)
        {
            if (pPolygonItem)
            {
                pPolygonItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                pPolygonItem = nullptr;
                GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
                view->controled = false;
            }
        }
    }
}

void theMainWindow::on_action_pen_changed()
{
    if (ui->action_pen->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:画笔打开关闭期间痕迹为一图元");
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            view->controled = true;
            bDraw = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
            view->controled = false;
            bDraw = false;
            ;
            drawPix = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_virtualHand_triggered()
{
    // // 导入sys模块设置模块地址
    // PyRun_SimpleString("import sys");
    // PyRun_SimpleString("sys.argv = ['python.py']");
    // PyRun_SimpleString("sys.path.append('./')");
    // if (!pModule)
    // {
    //     // 创建模块指针
    //     pModule = PyImport_ImportModule("VirtualMouse");
    //     qDebug() << "fun pModule1";
    // }
    // // 创建函数指针
    // if (pModule)
    // {
    //     qDebug() << "fun pModule2";
    //     PyObject *pFunc = PyObject_GetAttrString(pModule, "startVirtualMouse");
    //     if (pFunc)
    //     {
    //         PyObject_CallFunction(pFunc, NULL);
    //     }
    // }
    // QMessageBox::information(this, "EasyImages", "测试中", QMessageBox::Ok);
}

void theMainWindow::on_action_bucket_changed()
{
    if (!this->dragMode || !ui->action_bucket->isCheckable())
    {
        return;
    }
    GraphicDrawView* view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (ui->action_bucket->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:单击填色");
            view->controled=true;
            bBucket = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            view->controled=false;
            bBucket = false;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_line_changed()
{
    if (!this->dragMode || !ui->action_line->isCheckable())
    {
        return;
    }
    if (ui->action_line->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:右键撤回;双击完成");
            bLine = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bLine = false;
            pPath = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_grayScale_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        //图元置灰
        //图像算法
        for(auto item:layer->pList){
            if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
                // 保存原始的笔和画刷（如果尚未保存）
                if (!shape->data(Qt::UserRole).isValid()) {
                    QVariantMap props;
                    props.insert("pen", shape->pen());
                    props.insert("brush", shape->brush());
                    shape->setData(Qt::UserRole, props);
                }

                // 获取并修改笔和画刷以应用灰度颜色
                QPen originalPen = shape->pen();
                QColor penColor = originalPen.color();
                int grayPen = qGray(penColor.rgb());
                originalPen.setColor(QColor(grayPen, grayPen, grayPen));

                QBrush originalBrush = shape->brush();
                if (originalBrush.style() == Qt::SolidPattern) {
                    QColor brushColor = originalBrush.color();
                    int grayBrush = qGray(brushColor.rgb());
                    originalBrush.setColor(QColor(grayBrush, grayBrush, grayBrush));
                }

                // 设置新的笔和画刷
                shape->setPen(originalPen);
                shape->setBrush(originalBrush);

            }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
                // 保存原始图像的副本（如果尚未保存）
                if (!pic->data(Qt::UserRole).isValid()) {
                    pic->setData(Qt::UserRole, pic->pixmap());
                }

                // 应用灰度效果
                QImage origin = pic->pixmap().toImage();
                QImage grayImage = origin.convertToFormat(QImage::Format_ARGB32);
                QColor oldColor;

                for (int x = 0; x < grayImage.width(); x++)
                {
                    for (int y = 0; y < grayImage.height(); y++)
                    {
                        oldColor = QColor(origin.pixel(x, y));
                        int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
                        grayImage.setPixel(x, y, qRgb(average, average, average));
                    }
                }
                pic->setPixmap(QPixmap::fromImage(grayImage));
            }
        }
    }
}

void theMainWindow::on_action_brightness_triggered()
{
    if (checkSelectLayer())
    {
        DialogHSV *dia=new DialogHSV();
        connect(dia,SIGNAL(hChanged(int)),this,SLOT(slots_handleHSV_H(int)));
        connect(dia,SIGNAL(sChanged(int)),this,SLOT(slots_handleHSV_S(int)));
        connect(dia,SIGNAL(vChanged(int)),this,SLOT(slots_handleHSV_V(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝

        }

    }
}

//反相
void theMainWindow::on_action_Invert_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        for(auto item:layer->pList){
            if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
                // 保存原始的笔和画刷（如果尚未保存）
                if (!shape->data(Qt::UserRole).isValid()) {
                    QVariantMap props;
                    props.insert("pen", shape->pen());
                    props.insert("brush", shape->brush());
                    shape->setData(Qt::UserRole, props);
                }

                QPen pen = shape->pen();
                QColor penColor = pen.color();
                penColor.setRed(255-penColor.red());
                penColor.setBlue(255-penColor.blue());
                penColor.setGreen(255-penColor.green());
                pen.setColor(penColor);

                QBrush brush = shape->brush();

                QColor brushColor = brush.color();
                brushColor.setRed(255-brushColor.red());
                brushColor.setBlue(255-brushColor.blue());
                brushColor.setGreen(255-brushColor.green());
                brush.setColor(brushColor);


                shape->setPen(pen);
                shape->setBrush(brush);

            }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
                // 保存原始图像的副本（如果尚未保存）
                if (!pic->data(Qt::UserRole).isValid()) {
                    pic->setData(Qt::UserRole, pic->pixmap());
                }

                QImage image= pic->pixmap().toImage();
                QColor oldColor;

                for (int x = 0; x <image.width(); x++)
                {
                    for (int y = 0; y < image.height(); y++)
                    {
                        oldColor = QColor(image.pixel(x, y));
                        int red=255-oldColor.red();
                        int blue=255-oldColor.blue();
                        int green=255-oldColor.green();
                        image.setPixel(x, y, qRgb(red, green, blue));
                    }
                }
                pic->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
}

void theMainWindow::on_action_colour_triggered()
{
    if (checkSelectLayer())
    {
        DialogRGB *dia=new DialogRGB();
        connect(dia,SIGNAL(rChanged(int)),this,SLOT(slots_handleRGB_R(int)));
        connect(dia,SIGNAL(gChanged(int)),this,SLOT(slots_handleRGB_G(int)));
        connect(dia,SIGNAL(bChanged(int)),this,SLOT(slots_handleRGB_B(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝

        }

    }
}

void theMainWindow::on_action_rotate180_triggered()
{
    GraphicDrawView* view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view) {
        // 获取当前视图的变换矩阵
        QTransform currentTransform = view->transform();

        // 进行旋转变换（180度）
        currentTransform.rotate(180);

        // 应用变换矩阵
        view->setTransform(currentTransform);
    }
}

void theMainWindow::on_action_clockwise90_triggered()
{
    GraphicDrawView* view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view) {
        // 获取当前视图的变换矩阵
        QTransform currentTransform = view->transform();

        // 添加顺时针旋转90度的变换
        currentTransform.rotate(90);

        // 设置新的变换矩阵
        view->setTransform(currentTransform);
    }
}

void theMainWindow::on_action_anticlockwise90_triggered()
{
    GraphicDrawView* view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view) {
        // 获取当前视图的变换矩阵
        QTransform currentTransform = view->transform();

        // 进行逆时针旋转90度变换
        currentTransform.rotate(-90);

        // 应用变换矩阵
        view->setTransform(currentTransform);
    }
}

void theMainWindow::on_action_rotate_triggered()
{
    // 获取当前选中的图像视图
    GraphicDrawView* view = tabToViewMap[ui->tabWidget->currentWidget()];
    if (view) {
        // 弹出对话框，让用户输入旋转角度
        bool ok;
        QString text = QInputDialog::getText(this, tr("输入旋转角度"), tr("旋转角度:"), QLineEdit::Normal, QString(), &ok);
        if (ok && !text.isEmpty()) {
            // 将用户输入的文本转换为浮点数
            qreal angle = text.toDouble();

            // 获取当前视图的变换矩阵
            QTransform currentTransform = view->transform();

            // 进行指定角度的旋转变换
            currentTransform.rotate(angle);

            // 应用变换矩阵
            view->setTransform(currentTransform);
        }
    }
}

void theMainWindow::on_action_copy_triggered()
{
    GraphicDrawView* aview = tabToViewMap[ui->tabWidget->currentWidget()];
    int currentIndex = ui->tabWidget->currentIndex();
    QString imageName = ui->tabWidget->tabText(currentIndex);
    imageName+="_";
    if (!aview) {
        return;
    }

    // 创建一个 QImage 以便将当前视图内容渲染到图像中
    QImage image(aview->scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent); // 填充透明背景

    QPainter painter(&image);
    aview->scene->render(&painter);
    // 根据view的变换状态来翻转图片
    QTransform viewTransform = aview->transform();
    if (viewTransform.isScaling()) {
        // 水平翻转检查
        if (viewTransform.m11() < 0) {
            image = image.mirrored(true, false); // 水平翻转
        }
        // 垂直翻转检查
        if (viewTransform.m22() < 0) {
            image = image.mirrored(false, true); // 垂直翻转
        }
    }
    // this会无法适应屏幕
    GraphicDrawView *view = new GraphicDrawView();

    // QColor color(255, 255, 255, 0);
    // image->rect()
    view->scene->setSceneRect(0, 0, image.width(), image.height());
    QBrush transparentBrush(Qt::transparent);
    view->scene->setBackgroundBrush(transparentBrush);
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    view->scene->addItem(pixmapItem);
    pixmapItem->setData(view->ItemId, view->seqNum++);
    pixmapItem->setData(view->ItemDesciption, "背景");
    pixmapItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
    view->fitInView(view->scene->itemsBoundingRect(), Qt::KeepAspectRatio); // 视图比例适应

    qreal newCenterX = pixmapItem->sceneBoundingRect().center().x();
    qreal newCenterY = pixmapItem->sceneBoundingRect().center().y();
    pixmapItem->setTransformOriginPoint(newCenterX, newCenterY);

    // tab
    ui->tabWidget->addTab(view, QIcon(":/images/images/photo.png"), imageName);
    ui->tabWidget->setCurrentWidget(view);
    // tree
    QTreeWidgetItem *item = new QTreeWidgetItem(ImageType);
    item->setText(0, imageName);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    QTreeWidgetItem *parItem = ui->treeWidget->topLevelItem(0);
    parItem->addChild(item);
    tabToTreeItemMap[view] = item;
    tabToViewMap[view] = view;
    ui->treeWidget->setCurrentItem(item);
    // layer
    ui->stackedWidget->addWidget(view->layerForm);
    ui->stackedWidget->setCurrentWidget(view->layerForm);

    // 初始化子项
    CustomItemLayer *aLayer = new CustomItemLayer(GraphicDrawView::LayerType);
    aLayer->setText(0, "背景");
    aLayer->addItem(pixmapItem);
    view->layerForm->addTopLevelItem(aLayer);
    view->resizeLayer();
    view->itemToLayerMap[pixmapItem] = aLayer;
    view->layerForm->setCurrentItem(aLayer);
    aLayer->setSelected(true);
    // 更新按钮状态
    slots_set_btnV_checkState();
    slots_set_btnL_LockState();
    slots_set_opacity();

    // connect
    connect(view, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(on_mouseMovePoint(QPoint)));
    connect(view, SIGNAL(mouseClicked(QPoint)), this, SLOT(on_mouseClicked(QPoint)));
    connect(view, SIGNAL(mouseDoubleClick(QPoint)), this, SLOT(on_mouseDoubleClick(QPoint)));
    connect(view, SIGNAL(keyPress(QKeyEvent *)), this, SLOT(on_keyPress(QKeyEvent *)));
    connect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(slots_on_mouse_press(QMouseEvent *)));
    connect(view, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(slots_on_mouse_move(QMouseEvent *)));
    connect(view, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(slots_on_mouse_release(QMouseEvent *)));
    // layer
    connect(view->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slots_Item_checkState_changed(QTreeWidgetItem *, int)));
    connect(view->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_btnV_checkState()));
    connect(view->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slot_handle_layerVisibility(QTreeWidgetItem *, int)));
    connect(view->layerForm, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(slots_Item_LayerLock_changed(QTreeWidgetItem *, int)));
    // layerMenu
    connect(view->layerForm, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(slot_Layer_rightCliked(QTreeWidgetItem *, int)));
    connect(view->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_btnL_LockState()));
    connect(view->layerForm, SIGNAL(itemSelectionChanged()), this, SLOT(slots_set_opacity()));


}

void theMainWindow::on_action_sAll_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        view->scene->clearSelection();
        for(auto item:layer->pList){
            item->setSelected(true);
        }
    }
}

void theMainWindow::on_action_cancelSelect_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        for(auto item:layer->pList){
            item->setSelected(false);
        }
    }
}

void theMainWindow::on_action_sInvert_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        for(auto item:layer->pList){
            item->setSelected(!item->isSelected());
        }
    }
}

void theMainWindow::on_action_auxiliary_line_triggered()
{
    DialogALine*dia=new DialogALine;
    int res=dia->exec();
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    if(res==QDialog::Accepted){
        // 辅助网格网孔参数
        double width=dia->width;
        double height=dia->height;
        double angle=dia->angle;// -360-360
        QRectF rect=view->scene->sceneRect();
        // 网格大小为rect的1.25倍 位置从左上角开始
        QRectF gridRect =QRectF(0,0,rect.width()*1.25,rect.height()*1.25);

        // 设置网格线条的笔刷
        QPen pen(Qt::DashLine);
        pen.setColor(Qt::blue);
        pen.setWidthF(1.0);

        // 绘制网格
        QGraphicsScene *scene = view->scene;
        CustomItemLayer *aLayer = new CustomItemLayer(GraphicDrawView::LayerType);
        QPainterPath path;

        for (double x = gridRect.left(); x <= gridRect.right(); x += width)
        {
            path.moveTo(x, gridRect.top());
            path.lineTo(x, gridRect.bottom());
        }

        for (double y = gridRect.top(); y <= gridRect.bottom(); y += height)
        {
            path.moveTo(gridRect.left(), y);
            path.lineTo(gridRect.right(), y);
        }

        // 旋转路径
        QTransform transform;
        transform.rotate(angle);
        path = transform.map(path);

        // 创建 QGraphicsPathItem
        QGraphicsPathItem *pathItem = scene->addPath(path, pen);

        pathItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsMovable);
        pathItem->setData(view->ItemId, view->seqNum++);
        pathItem->setData(view->ItemDesciption, "辅助网格");
        aLayer->addItem(pathItem);
        view->itemToLayerMap[pathItem] = aLayer;

        aLayer->setText(0, "*辅助网格图层");
        QList<QTreeWidgetItem *> list;
        aLayer->setOpacity(100);
        list.push_back(aLayer);
        layerLock(list);
        view->layerForm->insertTopLevelItem(0, aLayer);
        view->resizeLayer();
    }
}

void theMainWindow::on_action_mergeLayer_triggered()
{
    int ret = QMessageBox::information(this, "EasyImages", "确定合并选中图层吗？", QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel)
    {
        return;
    }
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QList<QTreeWidgetItem *> items = view->layerForm->selectedItems();
    //增加图层（根据头部判定)
    CustomItemLayer *aLayer = new CustomItemLayer(GraphicDrawView::LayerType);
    QString name = QString("图层 %1").arg(view->LayerNum++);
    aLayer->setText(0, name);
    QList<QGraphicsItem*>qitems;
    if (items.isEmpty())
    {
        view->layerForm->insertTopLevelItem(0, aLayer);
    }
    else
    {
        //图层组
        if (items.front()->type() == GraphicDrawView::LayerClassType)
        {
            // 判断锁定
            if (!items.front()->icon(1).isNull())
            {
                QMessageBox::information(this, "EasyImages", "请解除锁定再操作！", QMessageBox::Ok);
                return;
            }
            // 默认向前添加
            items.front()->insertChild(0, aLayer);
        }
        else//图层
        {
            QTreeWidgetItem *parent = items.front()->parent();
            if (parent)
            {
                // 判断锁定
                if (!parent->icon(1).isNull())
                {
                    QMessageBox::information(this, "EasyImages", "请解除锁定再操作！", QMessageBox::Ok);
                    return;
                }
                int index = parent->indexOfChild(items.front());
                parent->insertChild(index, aLayer);
            }
            else
            {
                int index = view->layerForm->indexOfTopLevelItem(items.front());
                view->layerForm->insertTopLevelItem(index, aLayer);
            }
        }
    }
    //分配Z轴
    view->resizeLayer();
    // 更新勾选状态
    slots_Item_checkState_changed(aLayer, 0);
    //移动图元到新图层
    for (auto it = items.rbegin(); it != items.rend(); ++it)
    {
        auto item = *it;
        if (item->type() == GraphicDrawView::LayerType)
        {
            CustomItemLayer *layer = dynamic_cast<CustomItemLayer *>(item);
            if (layer)
            {
                for (auto i : layer->pList)
                {
                    aLayer->addItem(i);
                    view->itemToLayerMap[i]=aLayer;
                }
            }
        }
    }
    //删除选中图层
    for (auto item : items)
    {
        if (item->type() == GraphicDrawView::LayerType)
        {
            delete item;
        }
        else if (item->type() == GraphicDrawView::LayerClassType)
        {
            QTreeWidgetItem *parent = item->parent();
            if (parent)
            {
                while (item->childCount() > 0)
                {
                    QTreeWidgetItem *child = item->child(0);
                    item->removeChild(child);
                    parent->insertChild(0, child);
                }
            }
            else
            {
                while (item->childCount() > 0)
                {
                    QTreeWidgetItem *child = item->child(0);
                    item->removeChild(child);
                    view->layerForm->addTopLevelItem(child);
                }
            }
            delete item;
        }
    }
    view->resizeLayer();
}

void theMainWindow::on_action_system2_triggered()
{
    //pass
}

void theMainWindow::on_action_pen_triggered()
{
    //pass
}

void theMainWindow::on_action_reform_triggered()
{
    //pass
}

void theMainWindow::on_action_undo_triggered()
{
    //pass
}

void theMainWindow::on_action_records_triggered()
{
    //pass
}

void theMainWindow::on_action_shape_changed()
{
    if (!this->dragMode || !ui->action_shape->isCheckable())
    {
        return;
    }
    if (ui->action_shape->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:当前为鼠标拖拽绘图模式，可在【编辑】中关闭；右击可撤销路径;双击完成");
            bShape = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bShape = false;
            pPolygonItem = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_copyItem_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    forCopy = view->scene->selectedItems();
    if (forCopy.isEmpty())
    {
        return;
    }
    else
    {
        Copyed = true;
        CopyedX = false;
    }
}

void theMainWindow::on_action_paste_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    QGraphicsScene *scene = view->scene;
    if (Copyed || CopyedX)
    {
        if (checkSelectLayer())
        {
            CustomItemLayer *layer = getCurrentLayer();
            if (!layer)
            {
                return;
            }
            for (QGraphicsItem *item : forCopy)
            {
                // 根据类型复制图元
                QGraphicsItem *copyItem = copyItems(item);
                // 添加到场景和图层
                scene->addItem(copyItem);
                layer->addItem(copyItem);
                view->itemToLayerMap[copyItem] = layer;
            }
        }
    }
    if (CopyedX)
    {
        CopyedX = false;

        for (QGraphicsItem *item : forCopy)
        {
            if (view->itemToLayerMap.find(item) != view->itemToLayerMap.end())
            {
                CustomItemLayer *layer = view->itemToLayerMap[item];
                layer->pList.removeOne(item);
                view->itemToLayerMap.remove(item);
                view->scene->removeItem(item);
                delete item;
            }
            else
            {
                QGraphicsItemGroup *group = dynamic_cast<QGraphicsItemGroup *>(item);
                if (group)
                {
                    view->scene->destroyItemGroup(group);
                }
            }
            // 删除图形项
        }
    }
}

void theMainWindow::on_action_modeDrag_triggered(bool checked)
{
    this->dragMode = checked;
    ui->action_circle->setCheckable(checked);
    ui->action_rectangle->setCheckable(checked);
    ui->action_ellipse->setCheckable(checked);
    ui->action_triangle->setCheckable(checked);
    ui->action_trapezoid->setCheckable(checked);
    ui->action_shape->setCheckable(checked);

    // 控制工具单选
    if (actionGroup1 != nullptr)
    {
        return;
    }
    actionGroup1 = new QActionGroup(this);
    // 无法取消选择
}

void theMainWindow::on_action_rectangle_changed()
{
    if (!this->dragMode || !ui->action_rectangle->isCheckable())
    {
        return;
    }
    if (ui->action_rectangle->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:当前为鼠标拖拽绘图模式，可在【编辑】中关闭；按住Ctrl键可画正方");
            bRectangle = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bRectangle = false;
            pRectItem = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_circle_changed()
{
    if (!this->dragMode || !ui->action_circle->isCheckable())
    {
        return;
    }
    if (ui->action_circle->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:当前为鼠标拖拽绘图模式，可在【编辑】中关闭");
            bCircle = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bCircle = false;
            pEllipseItem = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_ellipse_changed()
{
    if (!this->dragMode || !ui->action_ellipse->isCheckable())
    {
        return;
    }
    if (ui->action_ellipse->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:当前为鼠标拖拽绘图模式，可在【编辑】中关闭");
            bEllipse = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bEllipse = false;
            pEllipseItem = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_triangle_changed()
{
    if (!this->dragMode || !ui->action_triangle->isCheckable())
    {
        return;
    }
    if (ui->action_triangle->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:当前为鼠标拖拽绘图模式，可在【编辑】中关闭");
            bTriangle = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bTriangle = false;
            pPolygonItem = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_trapezoid_changed()
{
    if (!this->dragMode || !ui->action_trapezoid->isCheckable())
    {
        return;
    }
    if (ui->action_trapezoid->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("提示:当前为鼠标拖拽绘图模式，可在【编辑】中关闭");
            bTrapezoid = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bTrapezoid = false;
            pPolygonItem = nullptr;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_copyX_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    forCopy = view->scene->selectedItems();
    if (forCopy.isEmpty())
    {
        return;
    }
    else
    {
        CopyedX = true;
        Copyed = false;
    }
}

void theMainWindow::on_action_shape_triggered()
{
    if (checkSelectLayer())
    {
        GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
        CustomItemLayer *layer = getCurrentLayer();
        if (!layer)
        {
            return;
        }
        if (dragMode)
        {
            return;
        }
        // 弹出输入框获取边数
        bool ok;
        int sides = QInputDialog::getInt(this, "输入边数", "请输入正多边形的边数:", 3, 3, 100, 1, &ok);
        if (!ok)
        {
            return; // 用户取消输入或输入无效
        }

        // 计算正多边形顶点（示例使用半径为50的正多边形）
        const int radius = 50;
        QPolygonF polygon;
        for (int i = 0; i < sides; ++i)
        {
            qreal angle = 2 * M_PI * i / sides;
            qreal x = radius * cos(angle);
            qreal y = radius * sin(angle);
            polygon << QPointF(x, y);
        }

        // 创建并配置多边形项目
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem(polygon);

        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setPos(view->width() / 2, view->height() / 2);
        item->setData(view->ItemId, view->seqNum++);
        item->setData(view->ItemDesciption, "多边形");
        if (coloredMode)
        {
            item->setPen(QPen(Qt::NoPen)); // 无边框
            // 随机填色
            item->setBrush(QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
        }
        else
        {
            QColor color;
            if (CurrentColor)
            {
                color = frontColor->m_color;
            }
            else
            {
                color = backColor->m_color;
            }
            item->setPen(QPen(color, 2)); // 黑色边框
            item->setBrush(Qt::NoBrush);  // 无内容
        }
        // 初始化置于画布视图中央
        QPointF center = view->mapToScene(view->viewport()->rect().center());
        item->setPos(center);
        view->scene->addItem(item);
        view->scene->clearSelection();
        item->setSelected(true);
        if (layer)
        {
            layer->addItem(item);
            view->itemToLayerMap[item] = layer;
            // item->setOpacity(layer->opacity/100);
        }
    }
}

void theMainWindow::on_action_eraser_changed()
{
    if (ui->action_eraser->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("双击删除当前所选图形");
            bErase = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bErase = false;
            ui->toolBar->setEnabled(true);
        }
    }
}

void theMainWindow::on_action_coloring_triggered()
{

    DialogGetColor* getCo=new DialogGetColor();
    int res = getCo->exec();
    if(res == QDialog::Accepted){
        frontColor->setColor(getCo->getColor());
    }

}


void theMainWindow::on_action_curve_changed()
{
    if (!this->dragMode || !ui->action_curve->isCheckable())
    {
        return;
    }
    if (ui->action_curve->isChecked())
    {
        if (tipInfo)
        { // 二重检查
            tipInfo->setText("右键撤销；双击完成");
            bCurve = true;
            ui->toolBar->setEnabled(false);
        }
    }
    else
    {
        if (tipInfo)
        {
            tipInfo->setText("提示:");
            bCurve = false;
            ui->toolBar->setEnabled(true);
        }
    }
}


void theMainWindow::on_action_re_grayScale_triggered()
{
    GraphicDrawView *view = tabToViewMap[ui->tabWidget->currentWidget()];
    auto items = view->scene->selectedItems();
    for(auto item:items){
        if (QGraphicsPixmapItem *pic = dynamic_cast<QGraphicsPixmapItem *>(item))
        {
            // 检查是否有保存的原始图像
            if (pic->data(Qt::UserRole).isValid()) {
                QPixmap originalPixmap = pic->data(Qt::UserRole).value<QPixmap>();
                pic->setPixmap(originalPixmap);
            }
        }else if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item))
        {
            if (shape->data(Qt::UserRole).isValid()) {
                QVariantMap props = shape->data(Qt::UserRole).value<QVariantMap>();
                if (props.contains("pen")) {
                    shape->setPen(props.value("pen").value<QPen>());
                }
                if (props.contains("brush")) {
                    shape->setBrush(props.value("brush").value<QBrush>());
                }
            }
        }
    }
    QPointF pointScene = view->mapToScene(this->curPos);                             // 转换到Scene坐标
    QGraphicsItem *item = view->scene->itemAt(pointScene, view->transform()); // 获取当下图形项
    if (item != nullptr && item->data(view->ItemId) == 0){
        if (QGraphicsPixmapItem *pic = dynamic_cast<QGraphicsPixmapItem *>(item))
        {
            // 检查是否有保存的原始图像
            if (pic->data(Qt::UserRole).isValid()) {
                QPixmap originalPixmap = pic->data(Qt::UserRole).value<QPixmap>();
                pic->setPixmap(originalPixmap);
            }
        }else if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item))
        {
            if (shape->data(Qt::UserRole).isValid()) {
                QVariantMap props = shape->data(Qt::UserRole).value<QVariantMap>();
                if (props.contains("pen")) {
                    shape->setPen(props.value("pen").value<QPen>());
                }
                if (props.contains("brush")) {
                    shape->setBrush(props.value("brush").value<QBrush>());
                }
            }
        }
    }
}

void theMainWindow::slots_handleRGB_R(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setRed((penColor.red()+val+256)%256);
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setRed((brushColor.red()+val+256)%256);
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    int red=(oldColor.red()+val+256)%256;
                    int blue=oldColor.blue();
                    int green=oldColor.green();
                    image.setPixel(x, y, qRgb(red, green, blue));
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}

void theMainWindow::slots_handleRGB_G(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setGreen((penColor.green()+val+256)%256);
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setGreen((brushColor.green()+val+256)%256);
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    int red=oldColor.red();
                    int blue=oldColor.blue();
                    int green=(oldColor.green()+val+256)%256;
                    image.setPixel(x, y, qRgb(red, green, blue));
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}

void theMainWindow::slots_handleRGB_B(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setBlue((penColor.blue()+val+256)%256);
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setBlue((brushColor.blue()+val+256)%256);
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    int red=oldColor.red();
                    int blue=(oldColor.blue()+val+256)%256;
                    int green=oldColor.green();
                    image.setPixel(x, y, qRgb(red, green, blue));
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}


void theMainWindow::on_action_colorReplace_triggered()
{
    if (checkSelectLayer())
    {
        DialogColorReplace *dia=new DialogColorReplace();
        connect(dia,SIGNAL(srcColor(QColor)),this,SLOT(slots_handleColorReplace_src(QColor)));
        connect(dia,SIGNAL(desColor(QColor)),this,SLOT(slots_handleColorReplace_des(QColor)));
        connect(dia,SIGNAL(tolChanged(int)),this,SLOT(slots_handleColorReplace_tol(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝
        }
        this->desCo=false;
        this->srcCo=false;
    }
}

void theMainWindow::slots_handleColorReplace_src(QColor a)
{
    this->srcColor=a;
    this->srcCo=true;
    rePlaceColor();
}

void theMainWindow::slots_handleColorReplace_des(QColor a)
{
    this->desColor=a;
    this->desCo=true;
    rePlaceColor();
}

void theMainWindow::slots_handleColorReplace_tol(int a)
{
    this->tol=a;
    rePlaceColor();
}

void theMainWindow::slots_handleHSV_H(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setHsv((penColor.hue()+val+360)%360,penColor.saturation(),penColor.value(),penColor.alpha());
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setHsv((brushColor.hue()+val+360)%360,brushColor.saturation(),brushColor.value(),brushColor.alpha());
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    oldColor.setHsv((oldColor.hue()+val+360)%360,oldColor.saturation(),oldColor.value(),oldColor.alpha());
                    image.setPixel(x, y, oldColor.rgb());
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}

void theMainWindow::slots_handleHSV_S(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setHsv(penColor.hue(),(penColor.saturation()+val+256)%256,penColor.value(),penColor.alpha());
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setHsv(brushColor.hue(),(brushColor.saturation()+val+256)%256,brushColor.value(),brushColor.alpha());
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    oldColor.setHsv(oldColor.hue(),(oldColor.saturation()+val+256)%256,oldColor.value(),oldColor.alpha());
                    image.setPixel(x, y, oldColor.rgb());
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}

void theMainWindow::slots_handleHSV_V(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setHsv(penColor.hue(),penColor.saturation(),(penColor.value()+val+256)%256,penColor.alpha());
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setHsv(brushColor.hue(),brushColor.saturation(),(brushColor.value()+val+256)%256,brushColor.alpha());
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    oldColor.setHsv(oldColor.hue(),oldColor.saturation(),(oldColor.value()+val+256)%256,oldColor.alpha());
                    image.setPixel(x, y, oldColor.rgb());
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}


void theMainWindow::on_action_blur_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        for(auto item:layer->pList){
            if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
                // 保存原始图像的副本（如果尚未保存）
                if (!pic->data(Qt::UserRole).isValid()) {
                    pic->setData(Qt::UserRole, pic->pixmap());
                }

                QImage ori= pic->pixmap().toImage();
                QImage image= pic->pixmap().toImage();

                int kernel [5][5]= {{0,0,1,0,0},
                                    {0,1,3,1,0},
                                    {1,3,7,3,1},
                                    {0,1,3,1,0},
                                    {0,0,1,0,0}};
                int kernelSize = 5;
                int sumKernel = 27;
                int r,g,b;
                QColor color;

                for(int x=kernelSize/2; x<ori.width()-(kernelSize/2); x++){
                    for(int y=kernelSize/2; y<ori.height()-(kernelSize/2); y++){

                        r = 0;
                        g = 0;
                        b = 0;

                        for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                            for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                                color = QColor(ori.pixel(x+i, y+j));
                                r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
                                g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
                                b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
                            }
                        }

                        r = qBound(0, r/sumKernel, 255);
                        g = qBound(0, g/sumKernel, 255);
                        b = qBound(0, b/sumKernel, 255);

                        image.setPixel(x,y, qRgb(r,g,b));

                    }
                }
                pic->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
}


void theMainWindow::on_action_shapeImg_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        for(auto item:layer->pList){
            if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
                // 保存原始图像的副本（如果尚未保存）
                if (!pic->data(Qt::UserRole).isValid()) {
                    pic->setData(Qt::UserRole, pic->pixmap());
                }

                QImage ori= pic->pixmap().toImage();
                QImage image= pic->pixmap().toImage();
                int kernel [3][3]= {{0,-1,0},
                                    {-1,5,-1},
                                    {0,-1,0}};
                int kernelSize = 3;
                int sumKernel = 1;
                int r,g,b;
                QColor color;

                for(int x=kernelSize/2; x<ori.width()-(kernelSize/2); x++){
                    for(int y=kernelSize/2; y<ori.height()-(kernelSize/2); y++){

                        r = 0;
                        g = 0;
                        b = 0;

                        for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                            for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                                color = QColor(ori.pixel(x+i, y+j));
                                r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
                                g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
                                b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
                            }
                        }

                        r = qBound(0, r/sumKernel, 255);
                        g = qBound(0, g/sumKernel, 255);
                        b = qBound(0, b/sumKernel, 255);
                        image.setPixel(x,y, qRgb(r,g,b));

                    }
                }

                pic->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
}


void theMainWindow::on_action_lightness_triggered()
{
    if (checkSelectLayer())
    {
        DialogLightness *dia=new DialogLightness();
        connect(dia,SIGNAL(lChanged(int)),this,SLOT(slots_handleLightness(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝

        }

    }
}

void theMainWindow::slots_handleLightness(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setRed((penColor.red()+val+256)%256);
            penColor.setBlue((penColor.blue()+val+256)%256);
            penColor.setGreen((penColor.green()+val+256)%256);
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setRed((brushColor.red()+val+256)%256);
            brushColor.setBlue((brushColor.blue()+val+256)%256);
            brushColor.setGreen((brushColor.green()+val+256)%256);
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    int red=(oldColor.red()+val+256)%256;
                    int blue=(oldColor.blue()+val+256)%256;
                    int green=(oldColor.green()+val+256)%256;
                    image.setPixel(x, y, qRgb(red, green, blue));
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}


void theMainWindow::on_action_warmCo_triggered()
{
    if (checkSelectLayer())
    {
        DialogWarmCo *dia=new DialogWarmCo();
        connect(dia,SIGNAL(wChanged(int)),this,SLOT(slots_handleWarmCo(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝

        }

    }
}


void theMainWindow::on_action_coldCo_triggered()
{
    if (checkSelectLayer())
    {
        DialogColdCo *dia=new DialogColdCo();
        connect(dia,SIGNAL(cChanged(int)),this,SLOT(slots_handleColdCo(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝

        }

    }
}

void theMainWindow::slots_handleWarmCo(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setRed((penColor.red()+val+256)%256);
            penColor.setGreen((penColor.green()+val+256)%256);
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setRed((brushColor.red()+val+256)%256);
            brushColor.setGreen((brushColor.green()+val+256)%256);
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    int red=(oldColor.red()+val+256)%256;
                    int blue=oldColor.blue();
                    int green=(oldColor.green()+val+256)%256;
                    image.setPixel(x, y, qRgb(red, green, blue));
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}

void theMainWindow::slots_handleColdCo(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QPen pen = shape->pen();
            QColor penColor = pen.color();
            penColor.setBlue((penColor.blue()+val+256)%256);
            pen.setColor(penColor);

            QBrush brush = shape->brush();

            QColor brushColor = brush.color();
            brushColor.setBlue((brushColor.blue()+val+256)%256);
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->pixmap().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    int red=oldColor.red();
                    int blue=(oldColor.blue()+val+256)%256;
                    int green=oldColor.green();
                    image.setPixel(x, y, qRgb(red, green, blue));
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}


void theMainWindow::on_action_avgK_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        for(auto item:layer->pList){
            if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
                // 保存原始图像的副本（如果尚未保存）
                if (!pic->data(Qt::UserRole).isValid()) {
                    pic->setData(Qt::UserRole, pic->pixmap());
                }

                QImage image= pic->pixmap().toImage();

                int kernel [3][3] = {
                                    {1,1,1},
                                    {1,1,1},
                                    {1,1,1}};
                int sizeKernel = 3;
                int sumKernel = 9;
                QColor color;
                for(int x = sizeKernel/2;x<image.width() - sizeKernel/2;x++)
                {
                    for(int y= sizeKernel/2;y<image.height() - sizeKernel/2;y++)
                    {
                        int r = 0;
                        int g = 0;
                        int b = 0;
                        for(int i = -sizeKernel/2;i<=sizeKernel/2;i++)
                        {
                            for(int j = -sizeKernel/2;j<=sizeKernel/2;j++)
                            {
                                color = QColor(image.pixel(x+i,y+j));
                                r+=color.red()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                                g+=color.green()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                                b+=color.blue()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                            }
                        }
                        r = qBound(0,r/sumKernel,255);
                        g = qBound(0,g/sumKernel,255);
                        b = qBound(0,b/sumKernel,255);
                        image.setPixel(x,y,qRgb( r,g,b));
                    }
                }
                pic->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
}


void theMainWindow::on_action_gamma_triggered()
{
    if (checkSelectLayer())
    {
        CustomItemLayer *layer = getCurrentLayer();
        for(auto item:layer->pList){
            if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
                // 保存原始图像的副本（如果尚未保存）
                if (!pic->data(Qt::UserRole).isValid()) {
                    pic->setData(Qt::UserRole, pic->pixmap());
                }

                QImage image= pic->pixmap().toImage();
                double d=1.2;
                QColor color;
                int height = image.height();
                int width = image.width();
                for (int i=0;i<width;i++){
                    for(int j=0;j<height;j++){
                        color = QColor(image.pixel(i,j));
                        double r = color.red();
                        double g = color.green();
                        double b = color.blue();
                        int R = qBound(0,(int)qPow(r,d),255);
                        int G = qBound(0,(int)qPow(g,d),255);
                        int B = qBound(0,(int)qPow(b,d),255);
                        image.setPixel(i,j,qRgb(R,G,B));
                    }
                }
                pic->setPixmap(QPixmap::fromImage(image));
            }
        }
    }
}


void theMainWindow::on_action_bina_triggered()
{
    if (checkSelectLayer())
    {
        DialogBina *dia=new DialogBina();
        connect(dia,SIGNAL(bChanged(int)),this,SLOT(slots_handleBina(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝

        }

    }
}

void theMainWindow::slots_handleBina(int val)
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }
            QVariantMap props = shape->data(Qt::UserRole).value<QVariantMap>();
            QPen pen = props.value("pen").value<QPen>();
            QColor penColor = pen.color();
            int gray=(penColor.red() + penColor.green() + penColor.blue()) / 3;
            if(gray<=val){
                gray=0;
            }else{
                gray=255;
            }
            penColor.setRgb(gray,gray,gray,penColor.alpha());
            //func
            pen.setColor(penColor);

            QBrush brush = props.value("brush").value<QBrush>();

            QColor brushColor = brush.color();
            gray=(brushColor.red() + brushColor.green() + brushColor.blue()) / 3;
            if(gray<=val){
                gray=0;
            }else{
                gray=255;
            }
            brushColor.setRgb(gray,gray,gray,brushColor.alpha());
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->data(Qt::UserRole).value<QPixmap>().toImage();
            QColor oldColor;

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    oldColor = QColor(image.pixel(x, y));
                    int gray=(oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
                    if(gray<=val){
                        gray=0;
                    }else{
                        gray=255;
                    }
                    oldColor.setRgb(gray,gray,gray,oldColor.alpha());
                    //func
                    image.setPixel(x, y, oldColor.rgb());
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}


void theMainWindow::on_action_contract_triggered()
{

    if (checkSelectLayer())
    {
        DialogContract *dia=new DialogContract();
        connect(dia,SIGNAL(cChanged(int)),this,SLOT(slots_handleContractC(int)));
        connect(dia,SIGNAL(bChanged(int)),this,SLOT(slots_handleContractB(int)));
        int res=dia->exec();
        if(res==QDialog::Rejected){//拒绝

        }

    }

}

void theMainWindow::slots_handleContractB(int val)
{
    this->conbri=val;
    slots_handleContract();

}

void theMainWindow::slots_handleContractC(int val)
{
    this->conCon=val;
    slots_handleContract();
}

void theMainWindow::slots_handleContract()
{
    CustomItemLayer *layer = getCurrentLayer();
    for(auto item:layer->pList){
        if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)){
            // 保存原始的笔和画刷（如果尚未保存）
            if (!shape->data(Qt::UserRole).isValid()) {
                QVariantMap props;
                props.insert("pen", shape->pen());
                props.insert("brush", shape->brush());
                shape->setData(Qt::UserRole, props);
            }

            QVariantMap props = shape->data(Qt::UserRole).value<QVariantMap>();
            QPen pen = props.value("pen").value<QPen>();
            QColor penColor = pen.color();
            float r = conbri * 0.01 * penColor.redF() - 150 + conCon;
            float g = conbri * 0.01 * penColor.greenF() - 150 + conCon;
            float b = conbri * 0.01 * penColor.blueF() - 150 + conCon;
            r = qBound<float>(0, r, 255);
            g = qBound<float>(0, g, 255);
            b = qBound<float>(0, b, 255);
            penColor.setRedF(r);
            penColor.setBlueF(b);
            penColor.setGreenF(g);

            QBrush brush = props.value("brush").value<QBrush>();
            QColor brushColor = brush.color();
            r = conbri * 0.01 * brushColor.redF() - 150 + conCon;
            g = conbri * 0.01 * brushColor.greenF() - 150 + conCon;
            b = conbri * 0.01 * brushColor.blueF() - 150 + conCon;
            r = qBound<float>(0, r, 255);
            g = qBound<float>(0, g, 255);
            b = qBound<float>(0, b, 255);
            brushColor.setRedF(r);
            brushColor.setBlueF(b);
            brushColor.setGreenF(g);
            brush.setColor(brushColor);

            shape->setPen(pen);
            shape->setBrush(brush);

        }else if(QGraphicsPixmapItem*pic = dynamic_cast<QGraphicsPixmapItem *>(item)){
            // 保存原始图像的副本（如果尚未保存）
            if (!pic->data(Qt::UserRole).isValid()) {
                pic->setData(Qt::UserRole, pic->pixmap());
            }

            QImage image= pic->data(Qt::UserRole).value<QPixmap>().toImage();

            for (int x = 0; x <image.width(); x++)
            {
                for (int y = 0; y < image.height(); y++)
                {
                    float r = conbri * 0.01 * qRed(image.pixel(x, y)) - 150 + conCon;
                    float g = conbri * 0.01 * qGreen(image.pixel(x, y)) - 150 + conCon;
                    float b = conbri * 0.01 * qBlue(image.pixel(x, y)) - 150 + conCon;
                    r = qBound<float>(0, r, 255);
                    g = qBound<float>(0, g, 255);
                    b = qBound<float>(0, b, 255);
                    image.setPixel(x, y, qRgb(r, g, b));
                }
            }
            pic->setPixmap(QPixmap::fromImage(image));
        }
    }
}

