#include "donut.h"
#include <QTimer>
#include <iostream>
#include "QtWidgets/qgraphicsscene.h"



donut::donut(QObject *parent)
    : QObject{parent}
{
    this->setPixmap((QPixmap(":/images/donut.png")).scaled(80,80));

    QTimer *timer_drop = new QTimer(this);
    connect(timer_drop, &QTimer::timeout, this, &donut::drop );
    timer_drop->start(100);
}




void donut::drop()
{
    //1.Check if hit the bottom boundary
    //2.Check collision
    QList<QGraphicsItem *> colliding_items = this->collidingItems();
    if ( this->y() > 450 || !colliding_items.isEmpty() ) {
        this->scene()->removeItem(this);
        delete this;
        return;
    }

    //Just drop
    this->setPos( this->x(), this->y() + 5 );
}
