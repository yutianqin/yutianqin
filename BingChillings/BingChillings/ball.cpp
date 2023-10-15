#include "ball.h"
#include "QtWidgets/qgraphicsscene.h"
#include "gamescene.h"
#include <iostream>
#include "brick.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 750;

Ball::Ball(double speed, double x_coord, double y_coord, GameScene* scene, QObject *parent)
    : QObject{parent}
{
    this->my_scene = scene;
    //randomly pick if the ball will go left or right to start
    if(arc4random() % 100 > 50){
        this->x_speed = speed;
    }
    else{
        this->x_speed = -speed;
    }

    this->y_speed = -speed;
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Ball::move_ball);
    setPos(x_coord, y_coord);
}

void Ball::set_speed(double new_speed)
{
    x_speed = new_speed;
    y_speed = new_speed;
}

void Ball::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        moveTimer->start(50);
    }
}

void Ball::move_ball(){

    //If ball has gone out of window, delete
    if (y() > my_scene->height()) {
        my_scene->lose_life();
        scene()->removeItem(this);
        delete this;
        return;
    }

    //Check left and right bounds
    if(x() + x_speed + (boundingRect().width() * 0.5) > scene()->width()){
        x_speed *= -1;
    }
    else if (x() + x_speed - (boundingRect().width() * 0.5) < 0.0){
        x_speed *= -1;
    }
    //Check top scene bounds
    else if (y() + y_speed - (boundingRect().width() * 0.5) < 0.0){
        y_speed *= -1;
    }
    this->setPos(x() + x_speed, y() + y_speed);


    //Respond to collision
    const QList<QGraphicsItem *> others = collidingItems();
    if(others.isEmpty()){
        return;
    }

    //If the paddle is the colliding item, simply reverse the y_speed
    QGraphicsItem* other = others[0];
    Paddle* paddle = dynamic_cast<Paddle*>(other);
    if(paddle){
        y_speed *= -1;
        this->setPos(x() + x_speed, y() + y_speed);
        return;
    }

    Brick* brick = dynamic_cast<Brick*>(other);
    if(brick){
        //check collision with left side of brick
        if(this->x() < other->x()){
            x_speed *= -1;
        }
        //check collision with right side of brick
        else if(this->x() > other->x() + brick->get_width()){
            x_speed *= -1;
        }
        //check collision with top of brick
        else if(this->y() < other->y()){
            y_speed *= -1;
        }
        //check collision with bottom of brick
        else if(this->y() > other->y() + brick->get_height()){
            y_speed *= -1;
        }
        //if dead, delete brick
        brick->take_damage();
        if(brick->update_status()){
            my_scene->update_score();
            scene()->removeItem(brick);
            delete brick;
        }
        this->setPos(x() + 1 * x_speed, y() + 1 * y_speed);
        return;
    }

    Ball* ball = dynamic_cast<Ball*>(other);
    if(ball){
        if(this->x() < other->x()){
            x_speed = -std::abs(x_speed);
        }
        else if (this->x() > other->x()){
            x_speed = std::abs(x_speed);
        }
        if(this->y() < other->y()){
            y_speed = -std::abs(y_speed);
        }
        else if(this->y() > other->y()){
            y_speed = std::abs(y_speed);
        }
        this->setPos(x() + x_speed, y() + y_speed);
        this->setPos(x() + x_speed, y() + y_speed);
        return;
    }



}

//set bounding rectangle for the ball
QRectF Ball::boundingRect() const
{
    return QRectF(-(0.5 * radius), -(0.5 * radius), radius, radius);
}

//set real collision path for ball
QPainterPath Ball::shape() const{
    QPainterPath p;
    p.addEllipse(boundingRect());
    return p;
}

//draw ball
void Ball::paint(QPainter* painter, const QStyleOptionGraphicsItem */* option */, QWidget* /* widget*/){
    painter->setBrush(Qt::white);
    painter->drawEllipse(this->boundingRect());
}

void Ball::start_timer_manual(){
    moveTimer->start(50);
}

//when ball is deleted from going out of bottom bounds, emits signal to gamescene
Ball::~Ball(){
    delete moveTimer;
    emit mySignal("New Ball!");
}
