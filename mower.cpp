#include <iostream>

#include <QNetworkRequest>

#include "mower.h"

Mower::Mower(QNetworkAccessManager* net, std::string username, std::string password) :
		_net(net), _username(username), _password(password)
{
}
Mower::State Mower::getState()
{
	return _state;
}

void Mower::getStatus()
{
	_state = GettingStatus;
	QNetworkRequest req(QUrl("https://auth.lawn.gatech.edu/login_status.php"));
	_net->get(req);
}

Mower::Status Mower::strToStatus(std::string s)
{
	s = s.substr(0, s.find(','));

	if (s == "logged_in") {
		return Mower::LoggedIn;
	} else if (s == "not_authenticated") {
		return Mower::LoggedOut;
	} else if (s == "pending_login") {
		return Mower::PendingLogIn;
	} else if (s == "pending_logout") {
		return Mower::PendingLogOut;
	} else if (s == "not_on_LAWN") {
		return Mower::NotOnLAWN;
	}

	std::cerr << "LAWN ERROR: " << s;

	return Mower::Error;
}

void Mower::login()
{
	_state = LoggingIn;
	QNetworkRequest req(QUrl("https://auth.lawn.gatech.edu/index.php"));
    req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
	QByteArray data("output=text&username=");
	data += QUrl::toPercentEncoding(_username.c_str());
	data += QString("&password=");
	data += QUrl::toPercentEncoding(_password.c_str());
	_net->post(req, data);
}

void Mower::logout()
{
	_state = LoggingOut;
	QNetworkRequest req(QUrl("https://auth.lawn.gatech.edu/index.php"));
	QByteArray data("output=text&username=");
	data += QUrl::toPercentEncoding(_username.c_str());
	data += QString("&password=");
	data += QUrl::toPercentEncoding(_password.c_str());
	_net->post(req, data);
}
