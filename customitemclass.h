#ifndef CUSTOMITEMCLASS_H
#define CUSTOMITEMCLASS_H

#include <QTreeWidgetItem>
#include<QList>
class CustomItemClass:public QTreeWidgetItem
{
public:
    CustomItemClass(int type);
    ~CustomItemClass();
    void resize();
    void setzValue(qreal min,qreal max);
    void setOpacity(qreal val);
    qreal opacity=100;
private:
    qreal minZ=0;
    qreal maxZ=100;
    qreal LayerClassSpan=200;
};

#endif // CUSTOMITEMCLASS_H
