#include "cake.h"
#include <QTimer>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QRandomGenerator>
Cake::Cake(QObject *parent)
    : QObject{parent}
{
    // Generate a random integer between 0 and 10
    int randomNum = QRandomGenerator::global()->bounded(10);
    if (randomNum < 4) {
        this->setPixmap((QPixmap(":/images/images/cake.png")).scaled(40,40));
    }
    else if (randomNum >= 4 && randomNum < 6){
        this->setPixmap((QPixmap(":/images/images/partyface.png")).scaled(35,35));
    }
    else if(randomNum >= 6 && randomNum < 8){
        this->setPixmap((QPixmap(":/images/images/party.png")).scaled(35,35));
    }
    else {
        this->setPixmap((QPixmap(":/images/images/emoji-ice-cream.png")).scaled(30,40));
    }
    QTimer *timer_drop = new QTimer(this);
    connect(timer_drop, &QTimer::timeout, this, &Cake::move_cake );
    timer_drop->start(5);
}
void Cake::move_cake()
{
    // If the cake goes off the bottom of the scene, remove it
    if ( (this->y() > this->scene()->height()) ) {
        scene()->removeItem(this);
        delete this;
        return;
    }
    this->setPos(this->x(), this->y() + 1);
}


