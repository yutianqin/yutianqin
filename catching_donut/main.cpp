#include <QApplication>
#include <QGraphicsView>
#include <QMainWindow>
#include "game1scene.h"



int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    game1scene *scene = new game1scene;

    QGraphicsView *view = new QGraphicsView(scene);
    view->setFixedSize(910,512);
    view->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setScene(scene);
    view->show();

    return app.exec();
}
