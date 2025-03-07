#include "customtreewidgetlayer.h"

CustomTreeWidgetLayer::CustomTreeWidgetLayer() :QTreeWidget(){

    //set displayed columns
    //两列容易错乱
    this->setColumnCount(1);
}

CustomTreeWidgetLayer::~CustomTreeWidgetLayer()
{

}

void CustomTreeWidgetLayer::dropEvent(QDropEvent *event)
{
    QTreeWidget::dropEvent(event);
    emit this->CustomDropedSignal();
}
