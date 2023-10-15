#ifndef BRICK_H
#define BRICK_H

#include <QObject>
#include <QBrush>
#include <QGraphicsRectItem>

class Brick : public QObject, public QGraphicsRectItem
{
    int width = 100;
    int height = 20;
    int health = 1;
    bool bonus_ball_brick = false;
    Q_OBJECT
    QBrush* brush;

signals:
    void mySignal(const QString &str);
    void scoreSignal(const QString &str);

public:
    Brick(QObject *parent = nullptr);

    int get_height();
    int get_width();
    void set_health(int new_health);
    void set_bonus_brick();
    void take_damage();
    bool update_status();
    ~Brick();
};

#endif // BRICK_H
