#include "shop.h"
#include "buttonitem.h"
#include <QGraphicsScene>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>

Shop::Shop(int level, QWidget *parent) :
    WareHouse(Buy, 4, level, parent)
{
    setMinimumSize(600, 500);
    setMaximumSize(600, 500);

    QPushButton * pushbutton = new QPushButton(QIcon(QPixmap(QString(":guiRes/image/guiRes/5.png"))),
                                               QString(""),this);
    pushbutton->setIconSize(QSize(35,35));
    connect(pushbutton,SIGNAL(clicked(bool)),this,SLOT(hide()));
    pushbutton->setGeometry(550, 10, 40, 40);

    tabwidget->setGeometry(70, 50, 460, 400);
}

void Shop::moneyChange(int money)
{
    Good good;
    for(set<Good>::iterator it = goods[Seed].begin(); it!=goods[Seed].end();)
    {
        good = *it;
        goods[Seed].erase(it);
        good.num = money/good.buyPrice;
        goods[Seed].insert(good);
        it++;
    }
    goodgroup[good.type]->updateGoodItem(goods[Seed]);
}

Shop::~Shop()
{

}
