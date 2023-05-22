#ifndef BOY_H
#define BOY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>



class boy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    boy(QObject *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) override;

signals:

};

#endif // BOY_H
