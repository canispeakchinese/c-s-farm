#include "showscenegroup.h"
#include "buttonitem.h"
#include "store.h"
#include "shop.h"
#include <QPainter>

ShowSceneGroup::ShowSceneGroup(int level) : level(level)
{
}

void ShowSceneGroup::createButton()
{
    ButtonItem * buttonitem;

    //buttonitem = new ButtonItem(-30, -30, 60, 60, QPixmap(":scene/image/scene/decorate.png"));
    //buttonitem->setPos(-30, -30);
    //addToGroup(buttonitem);

    store = new Store(level);
    buttonitem = new ButtonItem(-30, -30, 60, 60, QPixmap(":scene/image/scene/store.png"));
    connect(buttonitem,SIGNAL(clicked()),store,SLOT(show()));
    emit sendStore(store);
    buttonitem->setPos(-100, -30);
    addToGroup(buttonitem);

    shop = new Shop(level);
    buttonitem = new ButtonItem(-30, -30, 60, 60, QPixmap(":scene/image/scene/shop.png"));
    connect(buttonitem,SIGNAL(clicked()),shop,SLOT(show()));
    emit sendShop(shop);
    buttonitem->setPos(-170, -30);
    addToGroup(buttonitem);
}

QRectF ShowSceneGroup::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
    return QRectF(-170, -30, 170, 30);
}

void ShowSceneGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    option = option;
    widget = widget;
    painter = painter;
}

QPainterPath ShowSceneGroup::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    path.addRect(-170, -30, 170, 30);
    return path;
}

ShowSceneGroup::~ShowSceneGroup()
{

}
