#ifndef GAME1SCENE_H
#define GAME1SCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "boy.h"



class game1scene : public QGraphicsScene
{
    Q_OBJECT
public:
    game1scene(QObject *parent = nullptr);

    boy *theBoy;

public slots: // Declare the spawnDroplet() function as a slot
    void spawnDroplet();

};

#endif // GAME1SCENE_H
