#include <QFile>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include "init.h"

QVector<User> Init::users;


Init::Init()
{
    this->users = readFromJSON();
}


QVector<User> Init::readFromJSON()
{

    QDir currnetDir = QDir::current();
    QString filePath = currnetDir.relativeFilePath("../../../../BingChillings/users.json");
    QJsonArray jsonArray;
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileContent = file.readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(fileContent);

        if (jsonDocument.isNull()){
            return users;
        }

        jsonArray = jsonDocument.array();
    }
    else {
        qDebug() << "readFromJSON: Failed to open file for reading.";
    }

    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject json = jsonArray[i].toObject();
        QString firstName = json["firstName"].toString();
        QString lastName = json["lastName"].toString();
        QDate dateOfBirth = QDate::fromString(json["dateOfBirth"].toString(), Qt::ISODate);
        QString gender = json["gender"].toString();
        QString profilePictureFileName = json["profilePictureFileName"].toString();
        QString username = json["username"].toString();
        QString password = json["password"].toString();

        QVector<int> scores;
        QJsonArray arrayOfIntsArray = json["scores"].toArray();
        for (int i = 0; i < arrayOfIntsArray.size(); ++i) {
            scores.append(arrayOfIntsArray[i].toInt());
        }
        User user(firstName, lastName, dateOfBirth, gender, profilePictureFileName, username, password, scores);
        this->users.append(user);
    }
    file.close();

    for(User &u : users){
        qDebug() << u.username();
    }
    return users;
}









