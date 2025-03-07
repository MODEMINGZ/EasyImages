#include "custompropertyform.h"
#include "customitemlayer.h"
#include "ui_custompropertyform.h"
#include<QGraphicsView>
#include<QDebug>
#include <QPainterPath>
#include <QString>
#include "customlabelcolor.h"
#include<QTimer>
#include<QFileDialog>

CustomPropertyForm::CustomPropertyForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomPropertyForm)
{
    ui->setupUi(this);

    init();
    setDefault();
}

CustomPropertyForm::~CustomPropertyForm()
{
    delete ui;
}

//设置基本Info形式
void CustomPropertyForm::setInfo(QGraphicsItem *item, GraphicDrawView *view)
{
    if(item==nullptr){
        this->setDefault();
        return;
    }
    ui->tabWidget->setEnabled(true);
    this->pItem=item;
    this->pView=view;
    CustomItemLayer*layer=pView->itemToLayerMap[pItem];
    //直接考select判断有延迟
    bool isLocked=layer->isLocked;
    ui->spinBox_opacity->setMinimum(0);
    ui->spinBox_opacity->setMaximum(layer->opacity);
    ui->horizontalSlider_opacity->setMinimum(0);
    ui->horizontalSlider_opacity->setMaximum(layer->opacity);
    int itemCount=layer->pList.count();
    ui->spinBox_z->setMinimum(0);
    ui->spinBox_z->setMaximum(itemCount-1);
    ui->horizontalSlider_z->setMinimum(0);
    ui->horizontalSlider_z->setMaximum(itemCount-1);
    this->handle_info();

    //基本属性
    ui->doubleSpinBox_x->setEnabled(!isLocked);
    ui->doubleSpinBox_y->setEnabled(!isLocked);
    ui->doubleSpinBox_height->setEnabled(!isLocked);
    ui->doubleSpinBox_width->setEnabled(!isLocked);
    ui->doubleSpinBox_centerX->setEnabled(!isLocked);
    ui->doubleSpinBox_centerY->setEnabled(!isLocked);
    ui->spinBox_opacity->setEnabled(!isLocked);
    ui->spinBox_scale->setEnabled(!isLocked);
    ui->spinBox_z->setEnabled(!isLocked);
    ui->spinBox_rotate->setEnabled(!isLocked);
    ui->horizontalSlider_opacity->setEnabled(!isLocked);
    ui->horizontalSlider_scale->setEnabled(!isLocked);
    ui->horizontalSlider_z->setEnabled(!isLocked);
    ui->horizontalSlider_rotate->setEnabled(!isLocked);
    if(pItem->data(GraphicDrawView::ItemId)==0){
        ui->doubleSpinBox_x->setEnabled(false);
        ui->doubleSpinBox_y->setEnabled(false);
        ui->horizontalSlider_z->setEnabled(false);
        ui->spinBox_z->setEnabled(false);

    }else if(layer->pList.front()->data(GraphicDrawView::ItemId)==0){
        ui->spinBox_z->setMinimum(1);
        ui->horizontalSlider_z->setMinimum(1);
    }
    //响应属性

    QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
    if(tItem){
        //pen
        ui->doubleSpinBox_penSize->setValue(tItem->pen().widthF());
        //brushColor
        ui->tab_reset->setEnabled(true);
        if(tItem->brush().style()==Qt::NoBrush){
            //透明黑
            //QColor color(0, 0, 0, 0);
            ui->label_brushColor->clearColor();
        }else{
            ui->label_brushColor->setColor(tItem->brush().color());
        }
        //brushStyle
        Qt::BrushStyle brushStyle=tItem->brush().style();
        switch(brushStyle){
        case Qt::NoBrush:
            ui->comboBox_brushType->setCurrentIndex(noBrush);
            break;
        case Qt::SolidPattern:
            ui->comboBox_brushType->setCurrentIndex(Solid);
            break;
        case Qt::Dense1Pattern:
            ui->comboBox_brushType->setCurrentIndex(Dense1);
            break;
        case Qt::Dense2Pattern:
            ui->comboBox_brushType->setCurrentIndex(Dense2);
            break;
        case Qt::Dense3Pattern:
            ui->comboBox_brushType->setCurrentIndex(Dense3);
            break;
        case Qt::Dense4Pattern:
            ui->comboBox_brushType->setCurrentIndex(Dense4);
            break;
        case Qt::Dense5Pattern:
            ui->comboBox_brushType->setCurrentIndex(Dense5);
            break;
        case Qt::Dense6Pattern:
            ui->comboBox_brushType->setCurrentIndex(Dense6);
            break;
        case Qt::Dense7Pattern:
            ui->comboBox_brushType->setCurrentIndex(Dense7);
            break;
        case Qt::HorPattern:
            ui->comboBox_brushType->setCurrentIndex(HorPattern);
            break;
        case Qt::VerPattern:
            ui->comboBox_brushType->setCurrentIndex(VerPattern);
            break;
        case Qt::CrossPattern:
            ui->comboBox_brushType->setCurrentIndex(CrossPattern);
            break;
        case Qt::BDiagPattern:
            ui->comboBox_brushType->setCurrentIndex(BDiagPattern);
            break;
        case Qt::FDiagPattern:
            ui->comboBox_brushType->setCurrentIndex( FDiagPattern);
            break;
        case Qt::DiagCrossPattern:
            ui->comboBox_brushType->setCurrentIndex(DiagCrossPattern);
            break;
        default:
            break;
        }

        QPen pen=tItem->pen();
        switch(pen.style()){
        case Qt::SolidLine:
            ui->comboBox_penStyle->setCurrentIndex(SolidLine);
            break;
        case Qt::DashLine:
            ui->comboBox_penStyle->setCurrentIndex(DashLine);
            break;
        case Qt::DashDotLine:
            ui->comboBox_penStyle->setCurrentIndex(DashDotLine);
            break;
        case Qt::DashDotDotLine:
            ui->comboBox_penStyle->setCurrentIndex(DashDotDotLine);
            break;
        case Qt::NoPen:
            ui->comboBox_penStyle->setCurrentIndex(NoPen);
            break;
        default:
            break;
        }
        ui->label_penColor->setColor(tItem->pen().color());

        switch (pen.capStyle()){
        case Qt::SquareCap:
            ui->comboBox_penCap->setCurrentIndex(SquareCap);
            break;
        case Qt::FlatCap:
            ui->comboBox_penCap->setCurrentIndex(FlatCap);
            break;
        case Qt::RoundCap:
            ui->comboBox_penCap->setCurrentIndex(RoundCap);
            break;
        default:
            break;
        }

        switch(pen.joinStyle()){
        case Qt::BevelJoin:
            ui->comboBox_penJoint->setCurrentIndex(BevelJoin);
            break;
        case Qt::MiterJoin:
            ui->comboBox_penJoint->setCurrentIndex(MiterJoin);
            break;
        case Qt::RoundJoin:
            ui->comboBox_penJoint->setCurrentIndex(RoundJoin);
            break;
        default:
            break;
        }
        QGraphicsEllipseItem* eItem = dynamic_cast<QGraphicsEllipseItem*>(pItem);
        if(eItem){
            //以16单位为1度
            ui->spinBox_sAngle->setValue(qreal(eItem->startAngle())/16);
            ui->spinBox_spanAngle->setValue(qreal(eItem->spanAngle())/16);
            ui->label_27->setEnabled(true);
            ui->label_28->setEnabled(true);
            ui->spinBox_sAngle->setEnabled(true);
            ui->spinBox_spanAngle->setEnabled(true);

        }else{
            ui->spinBox_sAngle->setValue(0);
            ui->spinBox_spanAngle->setValue(0);
            ui->label_27->setEnabled(false);
            ui->label_28->setEnabled(false);
            ui->spinBox_sAngle->setEnabled(false);
            ui->spinBox_spanAngle->setEnabled(false);
        }

    }else{
        ui->spinBox_sAngle->setValue(0);
        ui->spinBox_spanAngle->setValue(0);
        ui->label_brushColor->clearColor();
        ui->label_penColor->clearColor();
        ui->tab_reset->setEnabled(false);
    }

    //文字属性
    ui->plainTextEdit_text->clear();
    QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
    if(textItem){
        ui->tab_text->setEnabled(true);
        //字体
        QFont font=textItem->font();
        ui->fontComboBox_text->setCurrentFont(font.family());
        //内容
        ui->plainTextEdit_text->setPlainText(textItem->toPlainText());
        //样式
        ui->checkBox_textBold->setChecked(font.bold());
        ui->checkBox_textItalic->setChecked(font.italic());
        ui->checkBox_textOverline->setChecked(font.overline());
        ui->checkBox_textStrikeOut->setChecked(font.strikeOut());
        ui->checkBox_textUnderline->setChecked(font.underline());
        //size
        ui->spinBox_textSize->setValue(font.pointSize());
        ui->spinBox_textSpan->setValue(font.letterSpacing());
        ui->spinBox_textWidth->setValue(textItem->textWidth());
        //字重
        switch(font.weight()){
        case QFont::Thin:
            ui->comboBox_textWeight->setCurrentIndex(Thin);
            break;
        case QFont::ExtraLight:
            ui->comboBox_textWeight->setCurrentIndex(ExtraLight);
            break;
        case QFont::Light:
            ui->comboBox_textWeight->setCurrentIndex(Light);
            break;
        case QFont::Normal:
            ui->comboBox_textWeight->setCurrentIndex(Normal);
            break;
        case QFont::Medium:
            ui->comboBox_textWeight->setCurrentIndex(Medium);
            break;
        case QFont::DemiBold:
            ui->comboBox_textWeight->setCurrentIndex(DemiBold);
            break;
        case QFont::Bold:
            ui->comboBox_textWeight->setCurrentIndex(Bold);
            break;
        case QFont::ExtraBold:
            ui->comboBox_textWeight->setCurrentIndex(ExtraBold);
            break;
        case QFont::Black:
            ui->comboBox_textWeight->setCurrentIndex(Black);
            break;
        default:
            break;
        }
        //颜色
        ui->label_textColor->setColor(textItem->defaultTextColor());
    }else{

        ui->label_textColor->clearColor();
        ui->tab_text->setEnabled(false);
    }

}

void CustomPropertyForm::setDefault()
{
    pItem=nullptr;
    pView=nullptr;
    ui->tabWidget->setEnabled(false);
    //描述
    ui->label_id->setText("null");
    ui->label_type->setText("null");
    ui->label_shape->setText("null");
    ui->plainTextEdit_description->clear();
    ui->label_layer->setText("null");
    //基本属性
    ui->doubleSpinBox_x->setValue(0);
    ui->doubleSpinBox_y->setValue(0);
    ui->doubleSpinBox_height->setValue(0);
    ui->doubleSpinBox_width->setValue(0);
    ui->doubleSpinBox_centerX->setValue(0);
    ui->doubleSpinBox_centerY->setValue(0);
    ui->spinBox_opacity->setValue(0);
    ui->spinBox_scale->setValue(0);
    ui->spinBox_z->setValue(0);
    ui->spinBox_rotate->setValue(0);
    //响应属性
    ui->doubleSpinBox_penSize->setValue(0);
    ui->label_brushColor->clearColor();
    ui->label_penColor->clearColor();
    ui->comboBox_brushType->setCurrentIndex(0);
    ui->comboBox_penStyle->setCurrentIndex(0);
    ui->comboBox_penCap->setCurrentIndex(0);
    ui->comboBox_penJoint->setCurrentIndex(0);
    ui->spinBox_sAngle->setValue(0);
    ui->spinBox_spanAngle->setValue(0);
}

void CustomPropertyForm::reSetInfo()
{
    if(pItem){
        this->setInfo(pItem,pView);
    }else{
        this->setDefault();
    }
}

//设置info详情
void CustomPropertyForm::handle_info()
{
    //描述
    ui->label_id->setText(pItem->data(GraphicDrawView::ItemId).toString());
    ui->label_type->setText(QString("%1").arg(pItem->type()));
    ui->label_shape->setText(painterPathToString(pItem->shape()));
    ui->plainTextEdit_description->setPlainText((pItem->data(GraphicDrawView::ItemDesciption).toString()));
    CustomItemLayer*layer=pView->itemToLayerMap[pItem];
    ui->label_layer->setText(layer->text(0));
    //基本属性
    ui->doubleSpinBox_x->setValue(pItem->pos().x());
    ui->doubleSpinBox_y->setValue(pItem->pos().y());
    ui->doubleSpinBox_height->setValue((pItem->sceneBoundingRect().height()));
    ui->doubleSpinBox_width->setValue(pItem->sceneBoundingRect().width());
    ui->doubleSpinBox_centerX->setValue(pItem->mapToScene(pItem->transformOriginPoint()).x());
    ui->doubleSpinBox_centerY->setValue(pItem->mapToScene(pItem->transformOriginPoint()).y());
    ui->spinBox_opacity->setValue(pItem->opacity()*100);
    ui->spinBox_rotate->setValue(int(pItem->rotation())%360);
    ui->spinBox_scale->setValue(int(pItem->scale()*1000));
    int Zvalue=layer->pList.indexOf(pItem);
    ui->spinBox_z->setValue(Zvalue);

    // QGraphicsPixmapItem*pixItem=dynamic_cast<QGraphicsPixmapItem*>(pItem);
    // if(pixItem){
    //     qDebug()<<pixItem->shapeMode();
    // }
}

QString CustomPropertyForm::painterPathToString(const QPainterPath &path)
{
    /*
QPainterPath: Element count=5
 -> MoveTo(x=0, y=0)
 -> LineTo(x=1024, y=0)
 -> LineTo(x=1024, y=1024)
 -> LineTo(x=0, y=1024)
 -> LineTo(x=0, y=0)
    */
    QString pathDescription;
    int elementCount = path.elementCount();
    pathDescription+=QString("QPainterPath: Element count=%1\n").arg(elementCount);
    for (int i = 0; i < elementCount; ++i) {
        const QPainterPath::Element& element = path.elementAt(i);
        switch (element.type) {
        case QPainterPath::MoveToElement:
            pathDescription += QString("-> MoveTo(%1, %2)\n").arg(element.x).arg(element.y);
            break;
        case QPainterPath::LineToElement:
            pathDescription += QString("-> LineTo(%1, %2)\n").arg(element.x).arg(element.y);
            break;
        case QPainterPath::CurveToElement:
            pathDescription += QString("-> CurveTo(%1, %2)\n").arg(element.x).arg(element.y);
            break;
        case QPainterPath::CurveToDataElement:
            // CurveToDataElement 通常与 CurveToElement 一起使用，表示贝塞尔曲线的控制点
            pathDescription += QString("-> CurveToData(%1, %2)\n").arg(element.x).arg(element.y);
            break;
        default:
            pathDescription += "-> Unknown Element\n";
            break;
        }
    }
    return pathDescription.trimmed();
}

void CustomPropertyForm::slots_label_brushColor_colored(const QColor &color)
{
    if(pItem){
        QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
        if(tItem){
            QBrush newBrush=tItem->brush();
            newBrush.setColor(color);
            tItem->setBrush(newBrush);
        }
    }
}

void CustomPropertyForm::slots_label_penColor_colored(const QColor &color)
{
    if(pItem){
        QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
        if(tItem){
            QPen newPen = tItem->pen();
            newPen.setColor(color);
            tItem->setPen(newPen);
        }
    }
}

void CustomPropertyForm::slots_label_textColor_colored(const QColor &color)
{
    if(pItem){
        QGraphicsTextItem*textItem=dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            textItem->setDefaultTextColor(color);
        }
    }
}

void CustomPropertyForm::init()
{
    //数值联动
    //基本属性
    ui->doubleSpinBox_centerX->setMaximum(INT_MAX);
    ui->doubleSpinBox_centerX->setMinimum(INT_MIN);
    ui->doubleSpinBox_centerY->setMaximum(INT_MAX);
    ui->doubleSpinBox_centerY->setMinimum(INT_MIN);
    ui->doubleSpinBox_x->setMaximum(INT_MAX);
    ui->doubleSpinBox_x->setMinimum(INT_MIN);
    ui->doubleSpinBox_y->setMaximum(INT_MAX);
    ui->doubleSpinBox_y->setMinimum(INT_MIN);
    ui->doubleSpinBox_height->setMaximum(INT_MAX);
    ui->doubleSpinBox_height->setMinimum(INT_MIN);
    ui->doubleSpinBox_width->setMaximum(INT_MAX);
    ui->doubleSpinBox_width->setMinimum(INT_MIN);

    connect(ui->spinBox_opacity, SIGNAL(valueChanged(int)), ui->horizontalSlider_opacity, SLOT(setValue(int)));
    connect(ui->horizontalSlider_opacity, SIGNAL(valueChanged(int)),  ui->spinBox_opacity, SLOT(setValue(int)));

    ui->spinBox_rotate->setMinimum(-360);
    ui->spinBox_rotate->setMaximum(360);
    ui->horizontalSlider_rotate->setMinimum(-360);
    ui->horizontalSlider_rotate->setMaximum(360);
    connect( ui->spinBox_rotate, SIGNAL(valueChanged(int)), ui->horizontalSlider_rotate, SLOT(setValue(int)));
    connect(ui->horizontalSlider_rotate, SIGNAL(valueChanged(int)),  ui->spinBox_rotate, SLOT(setValue(int)));

    ui->spinBox_scale->setMinimum(0);
    ui->spinBox_scale->setMaximum(15000);
    ui->horizontalSlider_scale->setMinimum(0);
    ui->horizontalSlider_scale->setMaximum(15000);
    connect( ui->spinBox_scale, SIGNAL(valueChanged(int)), ui->horizontalSlider_scale, SLOT(setValue(int)));
    connect(ui->horizontalSlider_scale, SIGNAL(valueChanged(int)),  ui->spinBox_scale, SLOT(setValue(int)));

    connect( ui->spinBox_z, SIGNAL(valueChanged(int)), ui->horizontalSlider_z, SLOT(setValue(int)));
    connect(ui->horizontalSlider_z, SIGNAL(valueChanged(int)),  ui->spinBox_z, SLOT(setValue(int)));
    //响应属性
    ui->label_27->setEnabled(false);
    ui->label_28->setEnabled(false);
    ui->spinBox_sAngle->setEnabled(false);
    ui->spinBox_spanAngle->setEnabled(false);

    ui->comboBox_brushType->clear();
    ui->comboBox_brushType->addItem("NoBrush",noBrush);
    ui->comboBox_brushType->addItem("Solid",Solid);
    ui->comboBox_brushType->addItem("Dense1",Dense1);
    ui->comboBox_brushType->addItem("Dense2",Dense2);
    ui->comboBox_brushType->addItem("Dense3",Dense3);
    ui->comboBox_brushType->addItem("Dense4",Dense4);
    ui->comboBox_brushType->addItem("Dense5",Dense5);
    ui->comboBox_brushType->addItem("Dense6",Dense6);
    ui->comboBox_brushType->addItem("Dense7",Dense7);

    ui->comboBox_brushType->addItem("HorPattern",HorPattern);
    ui->comboBox_brushType->addItem("VerPattern",VerPattern);
    ui->comboBox_brushType->addItem("CrosePattern",CrossPattern);
    ui->comboBox_brushType->addItem("BDiagPattern",BDiagPattern);
    ui->comboBox_brushType->addItem("FDiagPattern",FDiagPattern);
    ui->comboBox_brushType->addItem("DiagCrossPattern",DiagCrossPattern);

    ui->comboBox_penStyle->clear();
    ui->comboBox_penStyle->addItem("NoPen",NoPen);
    ui->comboBox_penStyle->addItem("Solid",SolidLine);
    ui->comboBox_penStyle->addItem("Dash",DashLine);
    ui->comboBox_penStyle->addItem("DashDot",DashDotLine);
    ui->comboBox_penStyle->addItem("DashDotDot",DashDotDotLine);

    ui->comboBox_penJoint->clear();
    ui->comboBox_penJoint->addItem("Bavel",BevelJoin);
    ui->comboBox_penJoint->addItem("Miter",MiterJoin);
    ui->comboBox_penJoint->addItem("Round",RoundJoin);

    ui->comboBox_penCap->clear();
    ui->comboBox_penCap->addItem("Square",SquareCap);
    ui->comboBox_penCap->addItem("Flat",FlatCap);
    ui->comboBox_penCap->addItem("Round",RoundCap);

    ui->spinBox_sAngle->setMinimum(-360);
    ui->spinBox_sAngle->setMaximum(360);
    ui->spinBox_sAngle->setSingleStep(5);
    ui->spinBox_spanAngle->setMaximum(360);
    ui->spinBox_spanAngle->setMinimum(-360);
    ui->spinBox_spanAngle->setSingleStep(5);

    //文字
    ui->spinBox_textSize->setMinimum(1);
    ui->spinBox_textSize->setMaximum(2000);
    ui->spinBox_textSpan->setMinimum(-100);
    ui->spinBox_textSpan->setMaximum(500);
    ui->spinBox_textWidth->setMinimum(-1);
    ui->spinBox_textWidth->setMaximum(INT_MAX);
    ui->spinBox_textWidth->setSingleStep(10);

    ui->comboBox_textWeight->clear();
    ui->comboBox_textWeight->addItem("Thin",Thin);
    ui->comboBox_textWeight->addItem("ExtraLight",ExtraLight);
    ui->comboBox_textWeight->addItem("Light",Light);
    ui->comboBox_textWeight->addItem("Normal",Normal);
    ui->comboBox_textWeight->addItem("Medium",Medium);
    ui->comboBox_textWeight->addItem("DemiBold",DemiBold);
    ui->comboBox_textWeight->addItem("Bold",Bold);
    ui->comboBox_textWeight->addItem("ExtraBold",ExtraBold);
    ui->comboBox_textWeight->addItem("Black",Black);

    //外观
    //Qt::SolidPattern
    //this->setStyleSheet("background-color: white;");
    ui->tabWidget->setCurrentWidget(ui->tab_basic);

    connect(ui->label_brushColor,SIGNAL(colorChanged(const QColor&)),this,SLOT(slots_label_brushColor_colored(const QColor&)));
    connect(ui->label_penColor,SIGNAL(colorChanged(const QColor&)),this,SLOT(slots_label_penColor_colored(const QColor&)));
    connect(ui->label_textColor,SIGNAL(colorChanged(const QColor&)),this,SLOT(slots_label_textColor_colored(const QColor&)));
}

void CustomPropertyForm::on_doubleSpinBox_x_valueChanged(double arg1)
{
    if(pItem){
        pItem->setPos(arg1,pItem->pos().y());
    }
}


void CustomPropertyForm::on_doubleSpinBox_y_valueChanged(double arg1)
{
    if(pItem){
        pItem->setPos(pItem->pos().x(),arg1);
    }
}


void CustomPropertyForm::on_doubleSpinBox_width_valueChanged(double arg1)
{
    if(pItem){
        qreal scaleX =  arg1/pItem->sceneBoundingRect().width() ;
        QTransform transform=pItem->transform();
        transform.scale(scaleX, 1);
        pItem->setTransform(transform);
        if(pItem->data(GraphicDrawView::ItemId)==0){
            pItem->scene()->setSceneRect(pItem->sceneBoundingRect());
        }
    }
}


void CustomPropertyForm::on_doubleSpinBox_height_valueChanged(double arg1)
{
    if(pItem){
        qreal scaleY =arg1/pItem->sceneBoundingRect().height();
        QTransform transform=pItem->transform();
        transform.scale(1, scaleY);
        pItem->setTransform(transform);
        if(pItem->data(GraphicDrawView::ItemId)==0){
            pItem->scene()->setSceneRect(pItem->sceneBoundingRect());
        }
    }
}


void CustomPropertyForm::on_spinBox_opacity_valueChanged(int arg1)
{
    if(pItem){
        pItem->setOpacity(qreal(arg1)/100);
    }
}


void CustomPropertyForm::on_spinBox_rotate_valueChanged(int arg1)
{
    if(pItem){
        pItem->setRotation(qreal(arg1));
        //更新一下，关联响应长宽,设置时钟防止变动太急
        QTimer::singleShot(200,this,&CustomPropertyForm::reSetInfo);
    }
}


void CustomPropertyForm::on_spinBox_scale_valueChanged(int arg1)
{
    if(pItem){
        //若为0，则长宽溢出
        if(arg1==0){
            return;
        }
        pItem->setScale(qreal(arg1)/1000);
        //更新一下，关联响应长宽
        QTimer::singleShot(200,this,&CustomPropertyForm::reSetInfo);
    }
}


void CustomPropertyForm::on_spinBox_z_valueChanged(int arg1)
{
    if(pItem){
        CustomItemLayer*layer=pView->itemToLayerMap[pItem];
        int currentIndex = layer->pList.indexOf(pItem);
        if (currentIndex != -1) {
            layer->pList.move(currentIndex, arg1);
        }
        layer->resize();
    }

}


void CustomPropertyForm::on_plainTextEdit_description_textChanged()
{
    if(pItem){
        pItem->setData(GraphicDrawView::ItemDesciption,ui->plainTextEdit_description->toPlainText());
    }
}


void CustomPropertyForm::on_comboBox_brushType_currentIndexChanged(int index)
{
    if(index>=0){
        if(pItem!=nullptr){
            //The inferior stopped because it received a signal from the operating system.
            //大概率为野指针错误
            QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
            if(tItem){
                QBrush aBrush=QBrush(tItem->brush().color(),Qt::NoBrush);
                switch(index){
                case noBrush:
                    aBrush=QBrush(tItem->brush().color(),Qt::NoBrush);
                    tItem->setBrush( aBrush);
                    break;
                case Solid:
                    aBrush=QBrush(tItem->brush().color(),Qt::SolidPattern);
                    tItem->setBrush(aBrush);
                    break;
                case Dense1:
                    aBrush=QBrush(tItem->brush().color(),Qt::Dense1Pattern);
                    tItem->setBrush(aBrush);
                    break;
                case Dense2:
                    aBrush=QBrush(tItem->brush().color(),Qt::Dense2Pattern);
                    tItem->setBrush(aBrush);
                    break;
                case Dense3:
                    aBrush=QBrush(tItem->brush().color(),Qt::Dense3Pattern);
                    tItem->setBrush(aBrush);
                    break;
                case Dense4:
                    aBrush=QBrush(tItem->brush().color(),Qt::Dense4Pattern);
                    tItem->setBrush(aBrush);
                    break;
                case Dense5:
                    aBrush=QBrush(tItem->brush().color(),Qt::Dense5Pattern);
                    tItem->setBrush(aBrush);
                    break;
                case Dense6:
                    aBrush=QBrush(tItem->brush().color(),Qt::Dense6Pattern);
                    tItem->setBrush(aBrush);
                    break;
                case Dense7:
                    aBrush=QBrush(tItem->brush().color(),Qt::Dense7Pattern);
                    tItem->setBrush(aBrush);
                    break;
                case HorPattern:
                    aBrush=QBrush(tItem->brush().color(),Qt::HorPattern);
                    tItem->setBrush(aBrush);
                    break;
                case VerPattern:
                    aBrush=QBrush(tItem->brush().color(),Qt::VerPattern);
                    tItem->setBrush(aBrush);
                    break;
                case CrossPattern:
                    aBrush=QBrush(tItem->brush().color(),Qt::CrossPattern);
                    tItem->setBrush(aBrush);
                    break;
                case BDiagPattern:
                    aBrush=QBrush(tItem->brush().color(),Qt::BDiagPattern);
                    tItem->setBrush(aBrush);
                    break;
                case FDiagPattern:
                    aBrush=QBrush(tItem->brush().color(),Qt::FDiagPattern);
                    tItem->setBrush(aBrush);
                    break;
                case DiagCrossPattern:
                    aBrush=QBrush(tItem->brush().color(),Qt::DiagCrossPattern);
                    tItem->setBrush(aBrush);
                    break;
                default:
                    break;
                }
            }
        }
    }
}


void CustomPropertyForm::on_doubleSpinBox_centerX_valueChanged(double arg1)
{
    if(pItem){
        //当前ui->doubleSpinBox_centerY->value()可能为0
        qreal oriY=pItem->mapToScene(pItem->transformOriginPoint()).y();
        QPointF point=pItem->mapFromScene(QPointF(arg1,oriY));
        pItem->setTransformOriginPoint(point);
    }
}


void CustomPropertyForm::on_doubleSpinBox_centerY_valueChanged(double arg1)
{
    if(pItem){
        qreal oriX=pItem->mapToScene(pItem->transformOriginPoint()).x();
        QPointF point=pItem->mapFromScene(QPointF(oriX,arg1));
        pItem->setTransformOriginPoint(point);
    }
}

void CustomPropertyForm::on_doubleSpinBox_penSize_valueChanged(double arg1)
{
    if(pItem){
        QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
        if(tItem){
            QPen newPen = tItem->pen();
            newPen.setWidthF(arg1);
            tItem->setPen(newPen);
            //直接这样应用不上
            //tItem->pen().setWidthF(arg1);
        }
    }
}


void CustomPropertyForm::on_comboBox_penStyle_currentIndexChanged(int index)
{
    if(pItem){
        QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
        if(tItem){
            switch(index){
            case NoPen:{
                QPen newPen = tItem->pen();
                newPen.setStyle(Qt::NoPen);
                tItem->setPen(newPen);
            }
                break;
            case DashLine:{
                QPen newPen = tItem->pen();
                newPen.setStyle(Qt::DashLine);
                tItem->setPen(newPen);
            }
                break;
            case DashDotLine:{
                QPen newPen = tItem->pen();
                newPen.setStyle(Qt::DashDotLine);
                tItem->setPen(newPen);
            }
                break;
            case DashDotDotLine:{
                QPen newPen = tItem->pen();
                newPen.setStyle(Qt::DashDotDotLine);
                tItem->setPen(newPen);
            }
                break;
            case SolidLine:{
                QPen newPen = tItem->pen();
                newPen.setStyle(Qt::SolidLine);
                tItem->setPen(newPen);
            }
                break;
            }
        }
    }
}


void CustomPropertyForm::on_comboBox_penCap_currentIndexChanged(int index)
{
    if(pItem){
        QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
        if(tItem){
            QPen pen=tItem->pen();
            switch (index){
            case SquareCap:{
                QPen newPen = pen;
                newPen.setCapStyle(Qt::SquareCap);
                tItem->setPen(newPen);
            }
                break;
            case FlatCap:{
                QPen newPen = pen;
                newPen.setCapStyle(Qt::FlatCap);
                tItem->setPen(newPen);
            }
                break;
            case RoundCap:{
                QPen newPen = pen;
                newPen.setCapStyle(Qt::RoundCap);
                tItem->setPen(newPen);
            }
                break;
            default:
                break;
            }

        }
    }
}


void CustomPropertyForm::on_comboBox_penJoint_currentIndexChanged(int index)
{
    if(pItem){
        QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
        if(tItem){
            QPen pen=tItem->pen();
            switch(index){
            case BevelJoin:{
                QPen newPen = pen;
                newPen.setJoinStyle(Qt::BevelJoin);
                tItem->setPen(newPen);
            }
                break;
            case MiterJoin:{
                QPen newPen = pen;
                newPen.setJoinStyle(Qt::MiterJoin);
                tItem->setPen(newPen);
            }
                break;
            case RoundJoin:{
                QPen newPen = pen;
                newPen.setJoinStyle(Qt::RoundJoin);
                tItem->setPen(newPen);
            }
                break;
            default:
                break;
            }
        }
    }
}


void CustomPropertyForm::on_spinBox_sAngle_valueChanged(double arg1)
{
    if(pItem){
        QGraphicsEllipseItem* eItem = dynamic_cast<QGraphicsEllipseItem*>(pItem);
        if(eItem){
            eItem->setStartAngle(int(arg1*16));
        }
    }
}


void CustomPropertyForm::on_spinBox_spanAngle_valueChanged(double arg1)
{
    if(pItem){
        QGraphicsEllipseItem* eItem = dynamic_cast<QGraphicsEllipseItem*>(pItem);
        if(eItem){
            eItem->setSpanAngle(int(arg1*16));
        }
    }
}


void CustomPropertyForm::on_plainTextEdit_text_textChanged()
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QString text=ui->plainTextEdit_text->toPlainText();
            if(text.isEmpty()){
                return;
            }
            textItem->setPlainText(text);
        }
    }
}

// DirectWrite: CreateFontFaceFromHDC() failed
// 为字体文件的问题，不用管
void CustomPropertyForm::on_fontComboBox_text_currentFontChanged(const QFont &f)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setFamily(f.family());
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_checkBox_textBold_clicked(bool checked)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setBold(checked);
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_checkBox_textItalic_clicked(bool checked)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setItalic(checked);
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_checkBox_textUnderline_clicked(bool checked)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setUnderline(checked);
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_checkBox_textOverline_clicked(bool checked)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setOverline(checked);
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_checkBox_textStrikeOut_clicked(bool checked)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setStrikeOut(checked);
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_spinBox_textSize_valueChanged(int arg1)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setPointSize(arg1);
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_spinBox_textSpan_valueChanged(double arg1)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            font.setLetterSpacing(QFont::AbsoluteSpacing,arg1);
            textItem->setFont(font);
        }
    }
}


void CustomPropertyForm::on_spinBox_textWidth_valueChanged(double arg1)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            textItem->setTextWidth(arg1);
        }
    }
}


void CustomPropertyForm::on_comboBox_textWeight_currentIndexChanged(int index)
{
    if(pItem){
        QGraphicsTextItem*textItem= dynamic_cast<QGraphicsTextItem*>(pItem);
        if(textItem){
            QFont font=textItem->font();
            switch(index){
            case Thin:{
                QFont newFont=font;
                newFont.setWeight(QFont::Thin);
                textItem->setFont(newFont);
            }
                break;
            case ExtraLight:{
                QFont newFont=font;
                newFont.setWeight(QFont::ExtraLight);
                textItem->setFont(newFont);
            }
                break;
            case Light:{
                QFont newFont=font;
                newFont.setWeight(QFont::Light);
                textItem->setFont(newFont);
            }
                break;
            case Normal:{
                QFont newFont=font;
                newFont.setWeight(QFont::Normal);
                textItem->setFont(newFont);
            }
                break;
            case Medium:{
                QFont newFont=font;
                newFont.setWeight(QFont::Medium);
                textItem->setFont(newFont);
            }
                break;
            case DemiBold:{
                QFont newFont=font;
                newFont.setWeight(QFont::DemiBold);
                textItem->setFont(newFont);
            }
                break;
            case Bold:{
                QFont newFont=font;
                newFont.setWeight(QFont::Bold);
                textItem->setFont(newFont);
            }
                break;
            case ExtraBold:{
                QFont newFont=font;
                newFont.setWeight(QFont::ExtraBold);
                textItem->setFont(newFont);
            }
                break;
            case Black:{
                QFont newFont=font;
                newFont.setWeight(QFont::Black);
                textItem->setFont(newFont);
            }
                break;
            default:
                break;
            }
        }
        this->reSetInfo();
    }
}


void CustomPropertyForm::on_pushButton_brush_clicked()
{
    if(pItem){
        QAbstractGraphicsShapeItem* tItem = dynamic_cast<QAbstractGraphicsShapeItem*>(pItem);
        if(tItem){
            QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
            if (!fileName.isEmpty())
            {
                QPixmap pixmap(fileName);
                if (!pixmap.isNull())
                {
                    QBrush brush(pixmap);
                    tItem->setBrush(brush);
                }
            }
        }
    }
}

