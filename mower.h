#ifndef MOWER_H
#define MOWER_H

#include <string>

#include <QObject>
#include <QNetworkAccessManager>

class Mower : public QObject
{
	Q_OBJECT
public:
    Mower(QNetworkAccessManager* net, std::string username, std::string password);

    enum Status { LoggedIn, LoggedOut, PendingLogIn, PendingLogOut, NotOnLAWN, Error };
    enum State { LoggingIn, LoggingOut, GettingStatus, Idle };

    State getState();

    static Status strToStatus(std::string s);

public slots:
    void getStatus();
    void login();
    void logout();

private:
    QNetworkAccessManager* _net;
	std::string _username;
	std::string _password;
	State _state;
};

#endif // MOWER_H
