#include "QtCore/qdatetime.h"
#include "gamescene.h"
#include "highscoresform.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "init.h"
#include "cake.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 750;

QString MainWindow::username;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0 ; i < Init::users.size() ; i++){
        if (Init::users[i].username() == username){
            index = i;
        }
    }
    qDebug() << username;
    qDebug() << Init::users.size();
    qDebug() << index;

    qDebug() << Init::users[index].isBirthday();
    // Need to initialize index from users
    if(Init::users[index].isBirthday()){
    ifBirthday();
    // Create a QGraphicsProxyWidget for the button
    QGraphicsProxyWidget *proxyPlayButton = new QGraphicsProxyWidget();
    QGraphicsProxyWidget *proxyScoreButton = new QGraphicsProxyWidget();
    proxyScoreButton->setWidget(ui->highScoresButton);
    // Add the proxyButton to the QGraphicsScene
    proxyPlayButton->setWidget(ui->playButton);
    bd_scene->addItem(proxyPlayButton);
    bd_scene->addItem(proxyScoreButton);

    dropCakeTimer = new QTimer(this);
    dropCakeTimer->setInterval(80);
    connect( dropCakeTimer, SIGNAL(timeout()), this, SLOT(birthdayEffect()) );
    dropCakeTimer->start();
    // Stop the dropCakeTimer after 3 seconds
    QTimer::singleShot(3000, dropCakeTimer, SLOT(stop()));
    ui->setupUi(this);
    }
    setDate();
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::playButtonPressed);
    connect(ui->highScoresButton, &QPushButton::clicked, this, &MainWindow::highScoreButtonPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playButtonPressed(){
    game_scene = new GameScene();
    view = new QGraphicsView();
    view->setScene(game_scene);
    view->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    view->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    connect(game_scene, &GameScene::end_game, this, &MainWindow::handleGameEnd);
    view->show();
    view->setFocus();
}

//slot for end game
void MainWindow::handleGameEnd(QString type, int lives, int score)
{
    view->close();
    delete view;
    std::string summary;
    if(lives == 0){
        summary = "Good Try! Final Score: " + std::to_string(score);
    }
    else{
        summary = "Congrats! \nLives Remaining: " + std::to_string(lives) + "\nFinal Score: " + std::to_string(score);
    }
    ui->scoreEdit->setPlainText(QString::fromStdString(summary));

    Init::users[index].updateScore(score);
}

void MainWindow::highScoreButtonPressed(){

    HighScoresForm *highScoreForm = new HighScoresForm();
    highScoreForm->setScoreBoard(Init::users[index].scores()); // Need to add QVector<int> scores
    highScoreForm->show();
}

void MainWindow::setUserForm(QString img, QString username){
    ui->usernameLabel->setText(username);
    ui->userIcon->setIcon(QIcon(img));
    ui->userIcon->setIconSize(QSize(50,50));

}

void MainWindow::setGuestForm(){
    ui->usernameLabel->setText("Guest");
    ui->userIcon->setIcon(QIcon(":/images/images/ice-cream.png"));
    ui->userIcon->setIconSize(QSize(50,50));
}

void MainWindow::setDate(){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("ddd MMMM d yyyy");
    ui->dateLineEdit->setText(formattedTime);
}

void MainWindow::ifBirthday()
{

    ui->birthdayLineEdit->setText("Happy Birthday");
    //Extra work
    bd_scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(this);
    view->setScene(bd_scene);
    view->setFixedSize(this->width(), this->height());
    bd_scene->setSceneRect(0, 0, view->width(), view->height());
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setAttribute(Qt::WA_TranslucentBackground);// Set the background to transparent
    view->show();
    view->setFocus();
}
void MainWindow::birthdayEffect()
{
    // Create a new droplet object and set its position
    Cake *cake = new Cake();
    cake->setPos( arc4random_uniform(this->bd_scene->width()-30), -30 );
    // Add the droplet to the scene
    this->bd_scene->addItem(cake);
}












