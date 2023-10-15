#include "highscoresform.h"
#include "ui_highscoresform.h"

HighScoresForm::HighScoresForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HighScoresForm)
{
    ui->setupUi(this);
    connect(ui->returnButton, &QPushButton::clicked, this, &HighScoresForm::returnButtonPressed);
}

HighScoresForm::~HighScoresForm()
{
    delete ui;
}


void HighScoresForm::returnButtonPressed(){
    this->close();
}

void HighScoresForm::setScoreBoard(QVector<int> scores){
    ui->line1->setText(QString::number(scores[0]));
    ui->line2->setText(QString::number(scores[1]));
    ui->line3->setText(QString::number(scores[2]));
    ui->line4->setText(QString::number(scores[3]));
    ui->line5->setText(QString::number(scores[4]));
    ui->line6->setText(QString::number(scores[5]));
    ui->line7->setText(QString::number(scores[6]));
    ui->line8->setText(QString::number(scores[7]));
    ui->line9->setText(QString::number(scores[8]));
    ui->line10->setText(QString::number(scores[9]));
}
