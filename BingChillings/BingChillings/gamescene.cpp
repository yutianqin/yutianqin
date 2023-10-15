#include "gamescene.h"
#include "brick.h"
#include "ball.h"
#include <QDebug>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 750;

GameScene::GameScene()
{
    //Set initial lives and score
    player_lives = 3;
    score = 0;
    active_balls = 0;

    //create scoreboard
    scoreboard = new QGraphicsTextItem();
    std::string stats = "Lives: " + std::to_string(player_lives) + "\tScore: " + std::to_string(score);
    scoreboard->setPlainText(QString::fromStdString(stats));
    scoreboard->setDefaultTextColor(Qt::cyan);
    scoreboard->setScale(1.5);
    addItem(scoreboard);

    //create player paddle
    paddle = new Paddle();
    addItem(paddle);
    paddle->setFlag(QGraphicsItem::ItemIsFocusable);
    paddle->setFocus();

    //build bricks and ball
    build_grid(10, 8);
    int rand_x = arc4random() % (WINDOW_WIDTH - 100) + 100;
    add_ball(10, rand_x, WINDOW_HEIGHT / 2); //can update the speed later for difficulty;
    //setup background
    setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setBackgroundBrush(Qt::black);
}

//TODO: once a vector of balls exists, only lose a life if the vector is empty. Also check if empty, then pause game with continue
void GameScene::lose_life(){
    active_balls--;
    if(active_balls == 0){
        player_lives--;
        std::string stats = "Lives: " + std::to_string(player_lives) + "\tScore: " + std::to_string(score);
        scoreboard->setPlainText(QString::fromStdString(stats));
        if(player_lives == 0){
            emit end_game("end game", player_lives, score);
        }
    }
}

void GameScene::add_ball(double speed, double x_coord, double y_coord)
{
    Ball* ball = new Ball(speed, x_coord, y_coord, this);
    active_balls++;
    addItem(ball);
    connect(ball, &Ball::mySignal, this, &GameScene::continue_play);
    ball->start_timer_manual();
}

void GameScene::continue_play(){
    if(active_balls == 0){
        add_ball(10, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    }
}

void GameScene::update_score(){
    score += 5;
    std::string stats = "Lives: " + std::to_string(player_lives) + "\tScore: " + std::to_string(score);
    scoreboard->setPlainText(QString::fromStdString(stats));
    if(score == 400){
        score += 100*player_lives;
        emit end_game("end game", player_lives, score);
    }
}

void GameScene::bonus_ball(){
    int rand_x_coord = arc4random() % (WINDOW_WIDTH - 50) + 50;
    add_ball(10, rand_x_coord, WINDOW_HEIGHT / 2);
}

void GameScene::build_grid(int height, int width){
    for(int i = 0; i < width; ++i){
        int top_offset = 40;
        int left_offset = 60;
        for(int j = 0; j < height; ++j){

            Brick* brick = new Brick();
            if(j == (height - 2) && (i == 0 || i == 2 || i == 5 || i == width -1)){
                brick->set_bonus_brick();
                connect(brick, &Brick::mySignal, this, &GameScene::bonus_ball);
            }
            if(j == (height - 7) && (i == 1 || i == 6)){
                brick->set_bonus_brick();
                connect(brick, &Brick::mySignal, this, &GameScene::bonus_ball);
            }
            if(j == (height - 1) && (i > 1 && i < 6)){
                brick->set_health(2);
                brick->update_status();
            }
            if(j == (height - 5) && (i == 1 || i == 3 || i == 4 || i == 6)){
                brick->set_health(2);
                brick->update_status();
            }
            brick->setPos(i * (brick->get_width() + 10) + left_offset, j * (brick->get_height() + 10) + top_offset); //+10 is for padding
            addItem(brick);
        }
    }
}

GameScene::~GameScene(){
    delete paddle;
    delete scoreboard;
}
