#ifndef THEMAINWINDOW_H
#define THEMAINWINDOW_H
#include <QMainWindow>
#include<QTreeWidgetItem>
#include<QGraphicsView>
#include<QMap>
#include<QLabel>
#include"graphicdrawview.h"
#include "custompropertyform.h"
#include<QColorDialog>
#include"customlabelcolor.h"
#include "colorsliderform.h"
#include"colorwheel.h"
#include"custompatharrow.h"
#include<QDoubleSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class theMainWindow;
}
QT_END_NAMESPACE

class theMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    theMainWindow(QWidget *parent = nullptr);
    ~theMainWindow();
    void closeEvent(QCloseEvent *e);
private slots:

    void on_dockWidget_menu_visibilityChanged(bool visible);

    void on_action_guide_changed();

    void on_tabWidget_tabCloseRequested(int index);

    void on_tabWidget_currentChanged(int index);

    void on_action_createNew_triggered();

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

    //显示鼠标光标处的视图坐标和场景坐标
    void on_mouseMovePoint(QPoint point) ;
    //显示图形项的属性
    void on_mouseClicked(QPoint point);
    //修改图形项
    void on_mouseDoubleClick(QPoint point);
    //选中图形项时，用按键可以操作
    void on_keyPress(QKeyEvent*event);

    void on_action_rectangle_triggered();

    void on_action_ellipse_triggered();

    void on_action_openPicture_triggered();

    void on_action_1_1_triggered();

    void on_action_circle_triggered();

    void on_action_trapezoid_triggered();

    void on_action_triangle_triggered();

    void on_action_text_triggered();

    void on_action_delete_triggered();

    void on_action_clear_triggered();

    void on_action_enlarge_triggered();

    void on_action__reduce_triggered();

    void on_action_turnLeft_triggered();

    void on_action_turnRight_triggered();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_action_syncOff_triggered();

    void on_action_behind_triggered();

    void on_action_front_triggered();

    void on_action_combine_triggered();

    void on_action_scatter_triggered();

    void on_action_addPhoto_triggered();

    void on_dockWidget_fun_visibilityChanged(bool visible);

    void on_action_property_changed();


    void on_dockWidget_layer_visibilityChanged(bool visible);

    void on_action_layerWindow_changed();

    void on_action_drawWindow_changed();

    void on_dockWidget_color_visibilityChanged(bool visible);

    void on_action_100_triggered();

    void on_action_200_triggered();

    void on_action_closeCurrentFile_triggered();

    void on_action_matchScreen_triggered();

    void on_action_matchWidth_triggered();

    void on_action_matchHeight_triggered();

    void on_action_flipHorizontal_triggered();

    void on_action_flipVertical_triggered();

    void on_action_diagonalFlip_triggered();

    void on_btn_layerGroup_clicked();

    void on_btn_addLayer_clicked();

    void on_btn_deleteLayer_clicked();

    void on_btn_lockLayer_clicked();

    void on_cbtn_visible_clicked(bool checked);
    //自定义槽函数不要以on开头，系统检测报错
    void slots_set_btnV_checkState();
    //connect with treeWidget
    void slots_Item_checkState_changed(QTreeWidgetItem* item, int column);

    void on_action_coloredItem_triggered(bool checked);

    void slot_handle_layerVisibility(QTreeWidgetItem*item,int column);
    //解决右键菜单
    void slot_Layer_rightCliked(QTreeWidgetItem *item, int column);

    void on_action_deleteLayer_triggered();

    void on_btn_lockLayer_clicked(bool checked);

    void on_action_layerLock_triggered();

    void on_action_layerUnlock_triggered();

    void on_action_layerSee_triggered();

    void on_action_LayerNSee_triggered();

    void slots_Item_LayerLock_changed(QTreeWidgetItem *, int);

    void slots_set_btnL_LockState();

    void on_spinBox_opacity_valueChanged(int arg1);

    void slots_set_opacity();

    void on_action_fastExport_triggered();

    void slots_on_ActionTriggered();

    void on_action_arrow_changed();

    void slots_on_mouse_press(QMouseEvent *e);

    void on_action_clip_triggered();

    void slots_on_mouse_move(QMouseEvent *event);

    void on_action_penBrush_changed();

    void slots_on_mouse_release(QMouseEvent *event);

    void on_action_pen_changed();

    void on_action_virtualHand_triggered();

    void on_action_bucket_changed();

    void on_action_line_changed();

    void on_action_grayScale_triggered();

    void on_action_brightness_triggered();

    void on_action_Invert_triggered();

    void on_action_colour_triggered();

    void on_action_rotate180_triggered();

    void on_action_clockwise90_triggered();

    void on_action_anticlockwise90_triggered();

    void on_action_rotate_triggered();

    void on_action_copy_triggered();

    void on_action_sAll_triggered();

    void on_action_cancelSelect_triggered();

    void on_action_sInvert_triggered();

    void on_action_auxiliary_line_triggered();

    void on_action_mergeLayer_triggered();

    void on_action_system2_triggered();

    void on_action_pen_triggered();

    void on_action_reform_triggered();

    void on_action_undo_triggered();

    void on_action_records_triggered();

    void on_action_shape_changed();

    void on_action_copyItem_triggered();

    void on_action_paste_triggered();

    void on_action_modeDrag_triggered(bool checked);

    void on_action_rectangle_changed();

    void on_action_circle_changed();

    void on_action_ellipse_changed();

    void on_action_triangle_changed();

    void on_action_trapezoid_changed();

    void on_action_copyX_triggered();

    void on_action_shape_triggered();

    void on_action_eraser_changed();

    void on_action_coloring_triggered();


    void on_action_curve_changed();

    void on_action_re_grayScale_triggered();

    void slots_handleRGB_R(int);
    void slots_handleRGB_G(int);
    void slots_handleRGB_B(int);

    void on_action_colorReplace_triggered();

    void slots_handleColorReplace_src(QColor);
    void slots_handleColorReplace_des(QColor);
    void slots_handleColorReplace_tol(int);

    void slots_handleHSV_H(int val);
    void slots_handleHSV_S(int val);
    void slots_handleHSV_V(int val);

    void on_action_blur_triggered();

    void on_action_shapeImg_triggered();

    void on_action_lightness_triggered();

    void slots_handleLightness(int val);

    void on_action_warmCo_triggered();

    void on_action_coldCo_triggered();

    void slots_handleWarmCo(int val);
    void slots_handleColdCo(int val);

    void on_action_avgK_triggered();

    void on_action_gamma_triggered();

    void on_action_bina_triggered();

    void slots_handleBina(int val);

    void on_action_contract_triggered();

    void slots_handleContractB(int val);
    void slots_handleContractC(int val);
    void slots_handleContract();

protected:
    // void mousePressEvent(QMouseEvent *event);
    // void mouseReleaseEvent(QMouseEvent *event);
    // void mouseDoubleClickEvent(QMouseEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
private:
    Ui::theMainWindow *ui;
    void initView();
    //画布默认记号
    int cnt_GraphicDraw=0;
    //映射
    QMap<QWidget*,GraphicDrawView*>tabToViewMap;//提供类型识别
    QMap<QWidget*,QTreeWidgetItem*> tabToTreeItemMap;
    //目录树自定义类型
    enum CustomItemType { ImageProType = QTreeWidgetItem::UserType + 1, ImageType,ImageDrawProType,ImageDrawType,EffectBarType };
    bool CurrentColor=1;//默认前景色
    void setToolBarEnable(bool);
    QLabel  *labSceneCord=nullptr;
    QLabel  *labItemCord=nullptr;
    QLabel  *labItemInfo=nullptr;
    QLabel  *tipInfo=nullptr;
    QString oriTip;
    //随机颜色图块模式
    bool coloredMode=false;
    bool checkSelectLayer();
    CustomItemLayer *getCurrentLayer();
    //递归遍历
    void layerLock(QList<QTreeWidgetItem*> &items);
    void layerUnlock(QList<QTreeWidgetItem*> &items);
    //属性表
    CustomPropertyForm*propertyForm;
    //颜色
    CustomLabelColor*frontColor=nullptr;
    CustomLabelColor*backColor=nullptr;
    ColorWheel*colorWheel=nullptr;
    ColorSliderForm*colorSliderForm=nullptr;
    QDoubleSpinBox *brushSizeSpinBox=nullptr;
    //动作
    QAction*lastCheckedAction = nullptr;
    QActionGroup *actionGroup=nullptr;
    //工具
    bool bArrow=false;
    CustomPathArrow*pArrow=nullptr;
    bool bPen=false;
    QGraphicsPathItem*pPen=nullptr;
    QPoint lastPos;
    int saveImgIndex=0;
    bool bDraw=false;
    QPixmap*drawPix=nullptr;
    bool bErase=false;
    QPixmap*erasePix=nullptr;
    bool bBucket=false;
    //system1
    // PyObject* pModule=nullptr;
    //鼠标拖拽模式
    bool dragMode=true;
    QPoint startPoint;
    QAction *lastCheckedAction_item=nullptr;
    QActionGroup* actionGroup1=nullptr;
    bool bCircle=false;
    bool bRectangle=false;
    bool bEllipse=false;
    bool bTriangle=false;
    bool bTrapezoid=false;
    bool bShape=false;
    bool bLine=false;
    bool bCurve=false;
    QList<QPointF>PointVec;
    QGraphicsPathItem*pPath=nullptr;
    QGraphicsPolygonItem*pPolygonItem=nullptr;
    QGraphicsRectItem*pRectItem=nullptr;
    QGraphicsEllipseItem*pEllipseItem=nullptr;
    QPolygonF polygonVec;
    //粘贴序列
    QList<QGraphicsItem*>forCopy;
    bool Copyed=false;
    bool CopyedX=false;
    QGraphicsItem* copyItems(QGraphicsItem*item);
    QPainterPath createBezierPath(const QList<QPointF>& points) ;
    QPainterPath generateSmoothCurve(QList<QPointF> points, bool closed, double tension, int numberOfSegments);
    //replacecolor
    QColor desColor;
    QColor srcColor;
    int tol=0;
    bool desCo=false;
    bool srcCo=true;
    void rePlaceColor();
    QColor HSVtoRGB(float h, float s, float v);
    //contract
    int conbri;
    int conCon;
    //bucket
    QPoint curPos;


};
#endif // THEMAINWINDOW_H
