#include "boy.h"
#include "QtWidgets/qgraphicsscene.h"
#include <iostream>




boy::boy(QObject *parent)
    : QObject{parent}
{
    this->setPixmap((QPixmap(":/images/bigmouth.png")).scaled(270,270));
    this->setPos(400,265);
    //Make the object focusable and be focused
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();
}




void boy::keyPressEvent(QKeyEvent *event)
{
    // Handle left arrow key press
    if ( event->key() == Qt::Key_Left ){
        if ( this->x() < -20 ) {
            this->setPos( scene()->width() - 100, this->y() );
        }
        this->setPos( this->x() - 30, this->y() );
    }

    // Handle right arrow key press
    else if ( event->key() == Qt::Key_Right ){
        if ( this->x() > 650 ) {
            this->setPos( -150, this->y() );
        }
        this->setPos( this->x() + 30, this->y() );
    }

    // Call the base class implementation for other keys
    else {
        QGraphicsPixmapItem::keyPressEvent(event);
    }

}



