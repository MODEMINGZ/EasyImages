#include "customundo.h"

QGraphicsItem *copyItems(GraphicDrawView *view,QGraphicsItem *item)
{
    QPointF offset(30, 30); // 设置偏移量
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
        QVariant descVariant = item->data(view->ItemDesciption);
        if (descVariant.isValid()) {
            QString description = descVariant.toString(); // 假设描述是字符串类型
            aitem->setData(view->ItemDesciption, description); // 设置到新item上
        } else {
            // 如果原item没有设置描述信息，则设置默认值或进行错误处理
            aitem->setData(view->ItemDesciption, "未设置描述");
        }

        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
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
        QVariant descVariant = item->data(view->ItemDesciption);
        if (descVariant.isValid()) {
            QString description = descVariant.toString(); // 假设描述是字符串类型
            aitem->setData(view->ItemDesciption, description); // 设置到新item上
        } else {
            // 如果原item没有设置描述信息，则设置默认值或进行错误处理
            aitem->setData(view->ItemDesciption, "未设置描述");
        }

        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
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
        QVariant descVariant = item->data(view->ItemDesciption);
        if (descVariant.isValid()) {
            QString description = descVariant.toString(); // 假设描述是字符串类型
            aitem->setData(view->ItemDesciption, description); // 设置到新item上
        } else {
            // 如果原item没有设置描述信息，则设置默认值或进行错误处理
            aitem->setData(view->ItemDesciption, "未设置描述");
        }

        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        return aitem;
    }
    else if (QGraphicsPixmapItem *pic = dynamic_cast<QGraphicsPixmapItem *>(item))
    {
        QGraphicsPixmapItem *aitem = new QGraphicsPixmapItem();
        aitem->setPixmap(pic->pixmap());

        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        QVariant descVariant = item->data(view->ItemDesciption);
        if (descVariant.isValid()) {
            QString description = descVariant.toString(); // 假设描述是字符串类型
            aitem->setData(view->ItemDesciption, description); // 设置到新item上
        } else {
            // 如果原item没有设置描述信息，则设置默认值或进行错误处理
            aitem->setData(view->ItemDesciption, "未设置描述");
        }

        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        return aitem;
    }
    else if (QGraphicsPathItem *path = dynamic_cast<QGraphicsPathItem *>(item))
    {
        QGraphicsPathItem *aitem = new QGraphicsPathItem();
        aitem->setPath(path->path());
        aitem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        aitem->setData(view->ItemId, view->seqNum++);
        QVariant descVariant = item->data(view->ItemDesciption);
        if (descVariant.isValid()) {
            QString description = descVariant.toString(); // 假设描述是字符串类型
            aitem->setData(view->ItemDesciption, description); // 设置到新item上
        } else {
            // 如果原item没有设置描述信息，则设置默认值或进行错误处理
            aitem->setData(view->ItemDesciption, "未设置描述");
        }

        aitem->setRotation(item->rotation());
        aitem->setScale(item->scale());
        return aitem;
    }
    else if (QGraphicsItemGroup *group = dynamic_cast<QGraphicsItemGroup *>(item))
    {
        QGraphicsItemGroup *newItemGroup = new QGraphicsItemGroup();

        // 遍历原图元组中的所有子项，并递归复制它们
        foreach (QGraphicsItem *childItem, group->childItems())
        {
            QGraphicsItem *copiedItem = copyItems(view,childItem); // 递归调用copyItems来复制子项
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
        newItemGroup->setRotation(group->rotation());
        newItemGroup->setScale(group->scale());
        // 返回新创建的图元组
        return newItemGroup;
    }
    return nullptr;
}

addCommand::addCommand(GraphicDrawView *view, QGraphicsItem *item)
{
    this->item=copyItems(view,item);
    this->layer=view->itemToLayerMap[item];
}

void addCommand::redo()
{
    view->scene->addItem(this->item);

}

void addCommand::undo()
{

}

moveCommand::moveCommand(QGraphicsItem*item, QPointF oldPos)
{
    this->item=item;
    Q_UNUSED(oldPos);
}

void moveCommand::redo()
{

}

void moveCommand::undo()
{

}
