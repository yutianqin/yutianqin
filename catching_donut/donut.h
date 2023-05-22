#ifndef WATER_H
#define WATER_H

#include <QObject>
#include <QGraphicsPixmapItem>



class donut : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    donut(QObject *parent = nullptr);

    void drop();

signals:

};

#endif // WATER_H
