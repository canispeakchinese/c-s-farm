#include "showinforgroup.h"
#include <QPainter>

const int needExp[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

PersonDefin::PersonDefin(QString username, QString faceaddress, int level, int exp, int money):
    username(username), faceaddress(faceaddress), level(level), exp(exp), money(money)
{

}

PersonDefin::PersonDefin(const PersonDefin &person):username(person.username), faceaddress(person.faceaddress),
    level(person.level), exp(person.exp), money(person.money)
{

}

PersonDefin::~PersonDefin()
{

}

ShowInforGroup::ShowInforGroup(PersonDefin *person) : person(person)
{

}

QRectF ShowInforGroup::boundingRect() const//我猜是用来确定刷新区域的，但是不确定
{
    return QRectF(-150, -50, 300, 200);
}

void ShowInforGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    option = option;
    widget = widget;
    painter->drawPixmap(-150, -50, 300, 200, QPixmap(QString(":guiRes/image/guiRes/paizi.png")));
    painter->drawPixmap(-100, 0, 50, 50, QPixmap(person->faceaddress));
    painter->drawText(QRect(-40, 0, 200, 30), QString("等级：%1").arg(person->level));
    painter->drawText(QRect(-40, 30, 200, 20), QString("(%1/%2)").arg(person->exp).arg(needExp[person->level]));
    painter->drawText(QRect(-40, 50, 200, 30), QString("金币：%2").arg(person->money));
}

QPainterPath ShowInforGroup::shape() const//这个函数返回值确定的是这个item的作用范围，比如点击事件,碰撞检测
{
    QPainterPath path;
    path.addRect(-150, -50, 300, 200);
    return path;
}

void ShowInforGroup::moneyChange(int _money)
{
    person->money = _money;
    update();
}

bool ShowInforGroup::expAdd(int _exp)
{
    person->exp += _exp;
    if(person->exp >= needExp[person->level])
    {
        while(person->exp >= needExp[person->level])
        {
            person->exp -= needExp[person->level];
            person->level++;
        }
        update();
        return true;
    }
    update();
    return false;
}

ShowInforGroup::~ShowInforGroup()
{

}
