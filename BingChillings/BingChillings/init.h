#ifndef INIT_H
#define INIT_H

#include <QWidget>
#include "user.h"

class Init
{
public:
    Init();

    static QVector<User> users;

    QVector<User> readFromJSON();

signals:

};

#endif // INIT_H
