#include "tool.h"
#include <QPainter>
#include <QEvent>
#include <QDebug>

Tool::Tool(ToolType type, QString filename) : type(type), filename(filename)
{

}

QRectF Tool::boundingRect() const
{
    return QRectF(-30, -30, 60, 60);
}

void Tool::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    option = option;
    widget = widget;
    painter->drawPixmap(-30, -30, 60, 60, QPixmap(filename));
}

QPainterPath Tool::shape() const
{
    QPainterPath path;
    path.addRect(-30, -30, 60, 60);
    return path;
}

bool Tool::sceneEvent(QEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        mousePressEvent((QGraphicsSceneMouseEvent *)event);
    }
    else if(event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        mouseReleaseEvent((QGraphicsSceneMouseEvent *)event);
    }
    else if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        mouseMoveEvent((QGraphicsSceneMouseEvent *)event);
    }
    else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick)
    {
        mouseDoubleClickEvent((QGraphicsSceneMouseEvent *)event);
    }
    return true;
}

void Tool::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendStatus(type);
    QGraphicsObject::mousePressEvent(event);
}

Tool::~Tool()
{
}

ToolGroup::ToolGroup()
{
}

void ToolGroup::createTool()
{
    for(int i=0; i<8; i++)
    {
        Tool *tool = new Tool((ToolType) i, QString(":icon/image/icon/topbutton(")+QString::number(i)+QString(").png"));
        addToGroup(tool);
        emit sendTool(tool);
        tool->setPos(QPoint(30+i*70, 10));
    }
    /*QGraphicsWidget * spad;      //铲地
    QGraphicsWidget * pack;      //背包
    QGraphicsWidget * water;     //浇水
    QGraphicsWidget * pyre;      //除虫
    QGraphicsWidget * weed;      //除草
    QGraphicsWidget * harv;      //收获
    QGraphicsWidget * alhar;     //全收
    QGraphicsWidget * kit;       //工具箱*/
}

QRectF ToolGroup::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
    return QRectF(0,-20,560,60);
}

void ToolGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    option = option;
    widget = widget;
    painter = painter;
}

QPainterPath ToolGroup::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    path.addRect(0, 0, 610, 80);
    return path;
}

ToolGroup::~ToolGroup()
{

}

