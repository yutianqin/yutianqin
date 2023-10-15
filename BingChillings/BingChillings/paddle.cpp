#include "paddle.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 750;

Paddle::Paddle(QGraphicsItem *parent)
{

    this->setRect(0, 0, width, height);
    this->setPos(WINDOW_WIDTH / 2, WINDOW_HEIGHT-(height + 10));
    brush = new QBrush();
    brush->setStyle(Qt::SolidPattern);
    brush->setColor(Qt::red);
    setBrush(*brush);

}

//Handle moving the paddle with key events
void Paddle::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left){
        if(x() >= 10){
            setPos(x() - 80, y());
        }

    }
    else if(event->key() == Qt::Key_Right){
        if(x() <= (WINDOW_WIDTH - width) - 20){
            setPos(x() + 80, y());
        }
    }
}

int Paddle::getWidth()
{
    return width;
}

int Paddle::getHeight()
{
    return height;
}

