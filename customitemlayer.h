#ifndef CUSTOMITEMLAYER_H
#define CUSTOMITEMLAYER_H
#include <QTreeWidgetItem>
#include<QGraphicsItem>
#include<QList>
class CustomItemLayer:public QTreeWidgetItem
{
public:
    QList<QGraphicsItem*>pList;
    CustomItemLayer(int type);
    ~CustomItemLayer();
    void resize();
    void setzValue(qreal min,qreal max);
    void setOpacity(qreal val);
    void addItem(QGraphicsItem*item);
    void sortZ();
    qreal opacity=100;
    qreal minZ=0;
    qreal maxZ=100;
    bool isLocked=false;
private:

    qreal LayerSpan=500;

};

#endif // CUSTOMITEMLAYER_H
