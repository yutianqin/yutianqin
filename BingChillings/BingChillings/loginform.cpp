#include "loginform.h"
#include "ui_loginform.h"

#include "mainwindow.h"
#include "newuserwindow.h"
#include "init.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    connect(ui->submitPushButton, &QPushButton::clicked, this, &LoginForm::validateUsernamePassword);
    connect(ui->newUserButton, &QPushButton::clicked, this, &LoginForm::newUserForm);
    connect(ui->guestButton, &QPushButton::clicked, this, &LoginForm::enterAsGuest);
}

LoginForm::~LoginForm()
{
    delete ui;
}


void LoginForm::validateUsernamePassword()
{
    QString userName = ui->usernameLineEdit->text();
    QString passWord = ui->passwordLineEdit->text();
    User user;
    bool foundUser = false;
    bool correcrPassword = false;

    for ( User &u : Init::users ) {
        if(foundUser){
            break;
        }
        if ( u.username() == userName ){
            foundUser = true;
            user = u;
            if ( !u.checkPassword(passWord) ) {
                ui->errorMessageLabel->setText("Wrong password");
                break;
            }
            correcrPassword = true;
        } else {
            ui->errorMessageLabel->setText("Cannot find the user");
            continue;
        }
    }

    if(foundUser && correcrPassword){
        MainWindow::username = user.username();
    MainWindow *mainWindow = new MainWindow();
    qDebug() << user.profilePictureFileName();
    mainWindow->setUserForm(user.profilePictureFileName(), user.username());
    mainWindow->show();
    this->close();
    }
    else{
    return;
    }
}

void LoginForm::newUserForm()
{
    NewUserWindow *newUserWindow = new NewUserWindow();
    newUserWindow->show();
}

void LoginForm::enterAsGuest()
{
    MainWindow::username = "guest";
    MainWindow *mainWindow = new MainWindow();
    mainWindow->setGuestForm();
    mainWindow->show();

    this->close();
}
