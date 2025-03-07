#ifndef CUSTOMTREEWIDGETLAYER_H
#define CUSTOMTREEWIDGETLAYER_H

#include <QTreeWidget>
#include<QDropEvent>
class CustomTreeWidgetLayer:public QTreeWidget
{
    Q_OBJECT
public:
    CustomTreeWidgetLayer();
    ~CustomTreeWidgetLayer();
protected:
    //拖拽事件更新
    void dropEvent(QDropEvent *event) override;
signals:
    void CustomDropedSignal();
};
//如何解决信号undefined reference to和undefined reference to `vtable for
//添加Q_OBJECT，重新qmake并构构建

#endif // CUSTOMTREEWIDGETLAYER_H
