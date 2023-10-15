#ifndef HIGHSCORESFORM_H
#define HIGHSCORESFORM_H

#include <QWidget>
#include <QVector>

namespace Ui {
class HighScoresForm;
}

class HighScoresForm : public QWidget
{
    Q_OBJECT

public:
    explicit HighScoresForm(QWidget *parent = nullptr);

    void setScoreBoard(QVector<int> scores);

    ~HighScoresForm();

private:
    Ui::HighScoresForm *ui;

private slots:
    void returnButtonPressed();
};

#endif // HIGHSCORESFORM_H
