#ifndef NEWUSERWINDOW_H
#define NEWUSERWINDOW_H

#include <QWidget>
#include "user.h"

namespace Ui {
class NewUserWindow;
}

class NewUserWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NewUserWindow(QWidget *parent = nullptr);
    ~NewUserWindow();

private slots:
    void resetFields();
    void validateFields();

private:
    Ui::NewUserWindow *ui;
    User user;
    QString profilePicName;
    QString passwordHash(QString &password);
    bool isUserNameTaken(QString &usernameRequst);
    void setupGenderBox();
};

#endif // NEWUSERWINDOW_H
