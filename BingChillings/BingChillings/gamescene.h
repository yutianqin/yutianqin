#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "paddle.h"
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QObject>

class GameScene : public QGraphicsScene
{

    int player_lives;
    int score;
    QString stats;
    Q_OBJECT
    Paddle* paddle;
    int active_balls;
    QGraphicsTextItem* scoreboard;


private slots:
    void continue_play();
    void bonus_ball();
signals:
    void end_game(QString source, int lives, int score);
public:
    GameScene();
    void lose_life();
    void update_score();
    void add_ball(double speed, double x_coord, double y_coord);
    ~GameScene();

private:
    void build_grid(int width, int height);
};

#endif // GAMESCENE_H
