#include "customitemclass.h"
#include"graphicdrawview.h"
#include"customitemlayer.h"
CustomItemClass::CustomItemClass(int type)
    :QTreeWidgetItem(type)//classType
{
    setIcon(0,QIcon(":/images/images/layer_class.png"));
    this->setCheckState(0,Qt::Checked);
    this->setFlags(this->flags()| Qt::ItemIsEditable);
}

CustomItemClass::~CustomItemClass()
{


}

void CustomItemClass::resize()
{
    qreal span=(maxZ-minZ)/LayerClassSpan;
    qreal pre=minZ;
    qreal cur=minZ+span;
    int childCount=this->childCount();
    //从尾到头
    for(int i=childCount-1;i>=0;i--){
        QTreeWidgetItem*item=this->child(i);
        if(item->type()==GraphicDrawView::LayerType){
            CustomItemLayer* tItem = dynamic_cast<CustomItemLayer*>(item);
            if(tItem){
                tItem->setzValue(pre,cur);
                tItem->resize();
            }
        }else if(item->type()==GraphicDrawView::LayerClassType){
            CustomItemClass* tItem = dynamic_cast<CustomItemClass*>(item);
            if(tItem){
                tItem->setzValue(pre,cur);
                tItem->resize();
            }
        }
        pre=cur;
        cur+=span;
    }
}

void CustomItemClass::setzValue(qreal min, qreal max)
{
    minZ=min;
    maxZ=max;
}

void CustomItemClass::setOpacity(qreal val)
{
    this->opacity=val;
    int childCount=this->childCount();
    for(int i=0;i<childCount;i++){
         QTreeWidgetItem*item=this->child(i);
        if(item->type()==GraphicDrawView::LayerType){
            CustomItemLayer* tItem = dynamic_cast<CustomItemLayer*>(item);
            if(tItem){
                tItem->setOpacity(val);
            }
        }else if(item->type()==GraphicDrawView::LayerClassType){
            CustomItemClass* tItem = dynamic_cast<CustomItemClass*>(item);
            if(tItem){
                tItem->setOpacity(val);
            }
        }
    }
}

