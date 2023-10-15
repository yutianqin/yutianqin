#ifndef BALL_H
#define BALL_H


#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>

class GameScene;

class Ball : public QObject, public QGraphicsItem
{
    double radius = 32.0;
    double x_speed;
    double y_speed;

    GameScene* my_scene;
    Q_OBJECT
    QTimer* moveTimer;

    //bounding rectangle
    QRectF boundingRect() const;
    //real collision shape
    QPainterPath shape() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
signals:
    void mySignal(const QString &str);

private slots:
    void move_ball();

public:
    Ball(double speed, double x_coord, double y_coord, GameScene* scene, QObject *parent = nullptr);
    void set_speed(double new_speed);
    void keyPressEvent(QKeyEvent* event);
    void start_timer_manual();
    ~Ball();

};

#endif // BALL_H
