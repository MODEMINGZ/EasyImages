#include "customitemlayer.h"
#include<QDragEnterEvent>
CustomItemLayer::CustomItemLayer(int type)
    :QTreeWidgetItem(type)//layerType
{
    //防止被另一项子项拖进
    this->setFlags(this->flags()&~Qt::ItemIsDropEnabled| Qt::ItemIsEditable);
    this->setCheckState(0,Qt::Checked);

}

CustomItemLayer::~CustomItemLayer()
{

}

void CustomItemLayer::resize()
{
    qreal span=(maxZ-minZ)/LayerSpan;
    qreal cur=minZ;
    for(auto i:pList){
        cur+=span;
        i->setZValue(cur);
    }
}

void CustomItemLayer::setzValue(qreal min, qreal max)
{
    minZ=min;
    maxZ=max;
}

void CustomItemLayer::setOpacity(qreal val)
{
    for(auto i:pList){
        if(i->opacity()*100!=this->opacity){//原先有值
            if(i->opacity()*100<val){
                //防止消失不见
                i->setOpacity(i->opacity()*(1+val/100));
            }else{
                i->setOpacity(i->opacity()*(val/100));
            }
        }else{
            i->setOpacity(val/100);
        }
    }
    this->opacity=val;
}

void CustomItemLayer::addItem(QGraphicsItem *item)
{
    this->pList.push_back(item);
    item->setOpacity(this->opacity/100);
    this->resize();
}

void CustomItemLayer::sortZ()
{
    std::sort(pList.begin(), pList.end(), [](auto a, auto b) {
        return a->opacity() < b->opacity();
    });
    resize();
}


