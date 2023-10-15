#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamescene.h"
#include <QGraphicsView>
#include <QWidget>
#include <QTimer>
#include "user.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

    QTimer *dropCakeTimer;
    QGraphicsScene *bd_scene;
    GameScene* game_scene;
    QGraphicsView* view;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<User> users;
    void setUserForm(QString img, QString username);
    void setGuestForm();
    static QString username;
    void ifBirthday();

private:
    Ui::MainWindow *ui;
    void setDate();
    int index;

private slots:
    void playButtonPressed();
    void handleGameEnd(QString type, int lives, int score);
    void highScoreButtonPressed();
    void birthdayEffect();
};

#endif // MAINWINDOW_H
