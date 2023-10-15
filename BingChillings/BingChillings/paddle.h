#ifndef PADDLE_H
#define PADDLE_H

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QBrush>

class Paddle : public QGraphicsRectItem
{
    int width = 200;
    int height = 20;
    QBrush* brush;

public:
    Paddle(QGraphicsItem *parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    int getWidth();
    int getHeight();
};

#endif // PADDLE_H
