#include <QtGlobal>
#include <QTimer>
#include "game1scene.h"
#include "boy.h"
#include "donut.h"




game1scene::game1scene(QObject *parent)
    : QGraphicsScene{parent}
{
    this->setBackgroundBrush(QBrush(QImage(":/images/donut_bg.jpg").scaledToHeight(512) .scaledToWidth(910)));
    this->setSceneRect(0, 0, 908, 510);

    theBoy = new boy(this);
    this->addItem(theBoy);


    // Create the falling droplets
    QTimer *dropletTimer = new QTimer(this);
    dropletTimer->setInterval(1000);
    connect( dropletTimer, SIGNAL(timeout()), this, SLOT(spawnDroplet()) );
    dropletTimer->start();
}




void game1scene::spawnDroplet()
{
    // Create a new droplet object and set its position
    donut* newDroplet = new donut();
    newDroplet->setPos( arc4random_uniform(850), arc4random_uniform(100) );

    // Add the droplet to the scene
    addItem(newDroplet);
}




