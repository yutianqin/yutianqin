#include "user.h"
#include <QJsonDocument>
#include <QFile>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>
#include "init.h"



User::User() :
    firstName_(""),
    lastName_(""),
    dateOfBirth_(QDate()),
    gender_(""),
    profilePictureFileName_(""),
    username_(""),
    password_(""),
    scores_(QVector<int>())
{
    // Default constructor implementation
}


User::User(QString &firstName, QString &lastName, QDate &dateOfBirth,
           QString &gender, QString &profilePictureFileName, QString &username,
           QString &password, QVector<int> &arrayOfInts) :
    firstName_(firstName),
    lastName_(lastName),
    dateOfBirth_(dateOfBirth),
    gender_(gender),
    profilePictureFileName_(profilePictureFileName),
    username_(username),
    password_(password),
    scores_(arrayOfInts)
{
}

QString User::firstName(){
    return firstName_;
}

QString User::lastName(){
    return lastName_;
}

QDate User::dateOfBirth(){
    return dateOfBirth_;
}

QString User::gender(){
    return gender_;
}

QString User::profilePictureFileName(){
    return profilePictureFileName_;
}

QString User::username() const {
    return username_;
}

QString User::password() const {
    return password_;
}

QVector<int> User::scores(){
    return scores_;
}

void User::write()

{


    QDir currnetDir = QDir::current();
    QString filePath = currnetDir.relativeFilePath("../../../../BingChillings/users.json");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "write: Failed to open JSON file for reading" << file.errorString();
        return;
    }
    file.resize(0); // Clear the file contents

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray jsonArray = doc.array();

    for( User &u : Init::users){
        // Write to the JSON file
        QJsonObject json;

        json["firstName"] = u.firstName();
        json["lastName"] = u.lastName();
        json["dateOfBirth"] = u.dateOfBirth().toString(Qt::ISODate);
        json["gender"] = u.gender();
        json["profilePictureFileName"] = u.profilePictureFileName();
        json["username"] = u.username();
        json["password"] = u.password();
        QJsonArray scor;
        QVector<int> scores_ = u.scores();
        for (int value : scores_) {
            scor.append(value);
        }
        json["scores"] = scor;

        // Append the new user object to the array
        jsonArray.append(json);
    }

    // Write the updated JSON array back to the file
    file.seek(0); // Move the file pointer back to the beginning
    QJsonDocument updatedDoc(jsonArray);
    file.write(updatedDoc.toJson());
    file.close();
}

QString User::passwordHash(QString &password) {
    QByteArray passwordBytes = password.toUtf8();

    QCryptographicHash hasher(QCryptographicHash::Sha256);
    hasher.addData(passwordBytes);
    QByteArray hash = hasher.result();

    QString hashString = hash.toHex();

    return hashString;
}


bool User::checkPassword(QString &password){
    if (this->password() != passwordHash(password) ){
        return false;
    }else {
        return true;
    }
}

bool User::isBirthday() {
    QDate currentDate = QDate::currentDate();
    return currentDate.day() == dateOfBirth_.day() && currentDate.month() == dateOfBirth_.month();
}

void User::updateScore(int score)
{
    // Insert the new score into the scores_ QVector in descending order
    int pos = 0;
    while (pos < scores_.size() && scores_.at(pos) > score) {
        ++pos;
    }
    scores_.insert(pos, score);

    // Save the updated scores to the JSON file
    write(); // Call the existing write() method to save the updated user data
}






