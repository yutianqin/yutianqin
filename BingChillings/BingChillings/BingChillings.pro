QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cake.cpp \
    highscoresform.cpp \
    init.cpp \
    ball.cpp \
    brick.cpp \
    gamescene.cpp \
    loginform.cpp \
    main.cpp \
    mainwindow.cpp \
    newuserwindow.cpp \
    paddle.cpp \
    user.cpp

HEADERS += \
    cake.h \
    highscoresform.h \
    init.h \
    ball.h \
    brick.h \
    gamescene.h \
    loginform.h \
    mainwindow.h \
    newuserwindow.h \
    paddle.h \
    user.h

FORMS += \
    newuserwindow.ui \
    highscoresform.ui \
    loginform.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    users.json
