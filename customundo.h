#ifndef CUSTOMUNDO_H
#define CUSTOMUNDO_H

#include <QObject>
#include <QUndoCommand>
#include "graphicdrawview.h"
#include<QGraphicsItem>
#include "customitemlayer.h"
class addCommand:public QUndoCommand
{
public:
    addCommand(GraphicDrawView*view,QGraphicsItem*item);

    void redo() override;
    void undo() override;
private:
    QGraphicsItem*item;
    GraphicDrawView*view;
    CustomItemLayer*layer;
    QPointF initPos;
};

class moveCommand:public QUndoCommand{
public:
    moveCommand(QGraphicsItem*item,QPointF oldPos);

    void redo() override;
    void undo() override;
private:
    QGraphicsItem*item;
    GraphicDrawView*view;
    QPointF oldPos;
    QPointF newPos;
};


#endif // CUSTOMUNDO_H
