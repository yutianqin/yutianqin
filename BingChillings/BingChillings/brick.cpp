#include "brick.h"


Brick::Brick(QObject *parent)
: QObject{parent}
{
    this->setRect(0, 0, width, height);
    brush = new QBrush();
    brush->setStyle(Qt::SolidPattern);
    brush->setColor(Qt::green);
    setBrush(*brush);
}

int Brick::get_height()
{
    return height;
}

int Brick::get_width()
{
    return width;
}

void Brick::set_health(int new_health){
    health = new_health;
}

void Brick::set_bonus_brick(){
    bonus_ball_brick = true;
    brush->setColor(Qt::cyan);
    setBrush(*brush);
}

void Brick::take_damage(){
    health--;
}

//return true if dead
bool Brick::update_status(){
    switch(health){
        case 3:
            brush->setColor(Qt::yellow);
            setBrush(*brush);
            return false;
        case 2:
            brush->setColor(Qt::yellow);
            setBrush(*brush);
            return false;
        case 1:
            brush->setColor(Qt::green);
            setBrush(*brush);
            return false;
        case 0:
            return true;
        default:
            return true;
    }
}

Brick::~Brick(){
    delete brush;
    if(bonus_ball_brick){
            emit mySignal("Continue");
    }
}
